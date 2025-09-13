/* MANUAL_CODE_START (Bosch Copyright) */
/* ************************************************************************** *
 *
 *  Copyright (c) 2018 Robert Bosch GmbH, Germany
 *  All rights reserved.
 *
 * ************************************************************************** */
/* MANUAL_CODE_END */

/*
 *
 * Description:
 * This module notifies the crash information to the other subsystems in the vehicle by using
 * hard wired PWM signal and FLEXRAY / LIN signals.
 * Customer Specific Crash Output Analog(CROA).
 *
 * Reference to Documentation:
 *
 *
 * MANUAL_CODE_START (version info)
 * $Source: rb_co_CustAnalogCrashOutput.c $
 * $Revision: 1.1 $
 * $Author: Eric Monteiro (RBEI/ESA-PW4) (EOI5KOR) $
 * MANUAL_CODE_END
 *
 */

/* ***** SECTION Includes ***** */
#include "rb_co_CustAnalogCrashOutput.h"	/* My own header */

/* PRQA S 1-9999 suppress_foreign_warnings_rb_co_CustAnalogCrashOutput_c */
/* MANUAL_CODE_START (Manual Includes) */
#include "rb_co_CustDigitalCrashOutputCommon.h"
/* MANUAL_CODE_END */
/* PRQA L:suppress_foreign_warnings_rb_co_CustAnalogCrashOutput_c */
/* ***** END_SECTION Includes ***** */


/*
 * Check that imported macros with storage class "ImportedDefine" are defined
 */
#ifndef RB_co_EventsPulseWidth100ms_cb32
#error The variable for the parameter "RB_co_EventsPulseWidth100ms_cb32" is not defined
#endif

#ifndef RB_co_EventsPulseWidth150ms_cb32
#error The variable for the parameter "RB_co_EventsPulseWidth150ms_cb32" is not defined
#endif

#ifndef RB_co_EventsPulseWidth50ms_cb32
#error The variable for the parameter "RB_co_EventsPulseWidth50ms_cb32" is not defined
#endif

#ifndef RB_co_MaxNumberPulses_cu8
#error The variable for the parameter "RB_co_MaxNumberPulses_cu8" is not defined
#endif

#ifndef RB_co_NumberPulseWidthsACA_cu8
#error The variable for the parameter "RB_co_NumberPulseWidthsACA_cu8" is not defined
#endif

#ifndef RB_co_Pulse100ms_cu8
#error The variable for the parameter "RB_co_Pulse100ms_cu8" is not defined
#endif

#ifndef RB_co_Pulse150ms_cu8
#error The variable for the parameter "RB_co_Pulse150ms_cu8" is not defined
#endif

#ifndef RB_co_Pulse50ms_cu8
#error The variable for the parameter "RB_co_Pulse50ms_cu8" is not defined
#endif

rb_co_CustENSState_ten rb_co_CustENSState_en;

/* The flag which set if Over Voltage at KL15or KL30 is detected for 100ms.
* TRUE: The OverVoltage fault is present, do not activate/trigger ACA
* FALSE: No Fault, ACA can be Triggered							*/
static boolean rb_co_AllowedActivateACA_bo = TRUE;

extern uint32 rb_co_COCustTriggersRT_u32, rb_co_CODiagTriggersRT_u32;

/* Indicates if any pulse is sent via analog output */
/* (FALSE: no pulse is sent; TRUE: at least one pulse type is sent) */
boolean rb_co_AnalogOutActive_bo;

/* Indicates if pulse width is active (FALSE: inactive; TRUE: active) */
boolean rb_co_PulseWidthActive_abo[RB_co_NumberPulseWidthsACA_cu8];

/* Indicates the current pulse within the pulse sequence */
uint8 rb_co_AnalogSequenceIndicator_u8;

/* Indicates if any pulse is sent via analog output during diag request */
/* (FALSE: no pulse is sent; TRUE: at least one pulse type is sent) */
boolean rb_co_DiagAnalogActive_bo;

/* Sequence of analog output pulses */
uint8 rb_co_AnalogSequence_au8[RB_co_NumberPulseWidthsACA_cu8];

/* Counters for pulses of analog output */
uint8 rb_co_AnalogPulseCounter_au8[RB_co_NumberPulseWidthsACA_cu8];

void rb_co_CustAnalog2msBG(void);
void rb_co_CheckForTriggers(void);
boolean rb_coas_CallBackAfterOverallCycleRT(uint8 ChannelAll_u8);
static void rb_co_PrepareAnalogSequence(uint8 PulseWidthType_u8);
static void rb_co_SetFirstPulse(void);
static void rb_co_SetNextPulseRT(void);
static void rb_co_ResetAnalogEvents(void);
static void rb_co_DetectOverVoltageFault(void);


/* ***** SECTION API functions ***** */
/* ****************************************************************************
 * API public: rb_co_CustAnalog2msBG
 *
 * Description:
 *   This API is used for performing 2ms cyclic operation of Crash Output related modules.
 * Arguments:
 *   None
 * Return:
 *   None
 * Usage guide & Scheduling:
 *   Called Cyclically in 2ms.
 * Remarks:
 *   None.
 * ****************************************************************************/
void rb_co_CustAnalog2msBG(void)
{
	boolean IsAOutConfigured_bo;   /* local variable to check whether analog crash output is configured */
	rb_itm_TestState_ten TestState_en;   /* local variable to check the ITM state */

	/* State machine for initial state and steady state handling*/
	switch (rb_co_CustENSState_en)
	{
	case rb_co_InitialState_e:

		/* Get the Aout configuration state */
		IsAOutConfigured_bo = rb_syco_IsAOutConfiguredALL(rb_syco_AOutCrashOutput1_e);
		/* Get the Initial Test Manager state */
		TestState_en = rb_itm_GetTestState(rb_itm_PsemInitPsifProgramming_e);

		/* Check whether Aout is configured and ITM is passed */
		if((IsAOutConfigured_bo == TRUE) AND_THEN (rb_itm_TestPassed_e == TestState_en))
		{
			/* Change the state to Steady state */
			rb_co_CustENSState_en = rb_co_Steadystate_e;
		}
		break;

	case rb_co_Steadystate_e:

		 /* Check for over voltage fault */
		 rb_co_DetectOverVoltageFault();

		 /* Checks for analog output trigger and starts corresponding output pulses */
		 rb_co_CheckForTriggers();
		 /* Reset events of analog output */
		 rb_co_ResetAnalogEvents();
		 break;

	default:
		 break;
	}

}


/* ****************************************************************************
 * API private: rb_co_CheckForTriggers
 *
 * Description:
 *    Checks new flags for analog output and starts corresponding output pulses.
 *
 * Arguments:
 *    None
 *
 * Return:
 *    None
 *
 * Usage guide & Scheduling:
 *    Called from rb_co_CustAnalog2msBG.
 *
 * Remarks:
 *    None
 * ****************************************************************************/
void rb_co_CheckForTriggers(void)
{
	/* Check whether pre-conditions are satisfied */
	if ((rb_co_AllowedActivateACA_bo == TRUE) AND_THEN ((rb_co_COCustTriggersRT_u32 != 0u) OR_ELSE (rb_co_CODiagTriggersRT_u32 != 0u)))
	{
		/* Check if a flag is set that corresponds to pulse width 50ms */
		if (((rb_co_COCustTriggersRT_u32 & RB_co_EventsPulseWidth50ms_cb32) > 0U)
				OR_ELSE (((rb_co_CODiagTriggersRT_u32 & RB_co_DiagPulseWidth50ms_cb32) > 1U) AND_THEN (rb_co_DiagAnalogActive_bo == TRUE)))
		{
			/* Prepare analog output sequence for a new impact that requires a 50ms pulse */
			rb_co_PrepareAnalogSequence(((uint8)RB_co_Pulse50ms_cu8));
			rb_co_CODiagTriggersRT_u32 = rb_co_CODiagTriggersRT_u32 & (~RB_co_DiagPulseWidth50ms_cb32);
		}

		/* Check if a flag is set that corresponds to pulse width 100ms */
		if (((rb_co_COCustTriggersRT_u32 & (uint32)RB_co_EventsPulseWidth100ms_cb32) > 0U)
				OR_ELSE (((rb_co_CODiagTriggersRT_u32 & RB_co_DiagPulseWidth100ms_cb32) > 1U) AND_THEN (rb_co_DiagAnalogActive_bo == TRUE)))
		{
			/* Prepare analog output sequence for a new impact that requires a 100ms pulse */
			rb_co_PrepareAnalogSequence(((uint8)RB_co_Pulse100ms_cu8));
			rb_co_CODiagTriggersRT_u32 = rb_co_CODiagTriggersRT_u32 & (~RB_co_DiagPulseWidth100ms_cb32);
		}

		/* Check if a flag is set that corresponds to pulse width 150ms */
		if (((rb_co_COCustTriggersRT_u32 & ((uint32)RB_co_EventsPulseWidth150ms_cb32)) > 0U)
				OR_ELSE (((rb_co_CODiagTriggersRT_u32 & ((uint32)RB_co_DiagPulseWidth150ms_cb32)) > 1U) AND_THEN (rb_co_DiagAnalogActive_bo == TRUE)))
		{
			/* Prepare analog output sequence for a new impact that requires a 150ms pulse */
			rb_co_PrepareAnalogSequence(((uint8)RB_co_Pulse150ms_cu8));
			rb_co_CODiagTriggersRT_u32 = rb_co_CODiagTriggersRT_u32 & (~RB_co_DiagPulseWidth150ms_cb32);
		}


		/* No pulse is currently sent via analog output */
		if (FALSE == rb_co_AnalogOutActive_bo)
		{
			/* Set indicator that analog output pulses are sent */
			rb_co_AnalogOutActive_bo = TRUE;
			/* Reset sequence indicator to first element of sequence */
			rb_co_AnalogSequenceIndicator_u8 = 0U;
			/* Set first pulse */
			rb_co_SetFirstPulse();
		}


	}

}

/* ****************************************************************************
 * INTERFACE API public: rb_coas_CallBackAfterOverallCycleRT
 *
 * Description:
 *    This API is used for hand shake mechanism when PWM signal Overall cycle
 *    transmissions are completed.
 *
 * Arguments:
 *    Channel_u8 : Channel number in which the PWM signal is generated.
 *
 * Return:
 *     TRUE: if Call back function is triggered.
 * 	   FALSE: if Call back function is not triggered.
 *
 * Usage guide & Scheduling:
 *    This function shall be called when ever the Overall cycle transmissions are completed
 *    .
 *
 * Remarks:
 *    None
 * ****************************************************************************/
boolean rb_coas_CallBackAfterOverallCycleRT(uint8 ChannelAll_u8)  /* PRQA S 3206 */  /* Interface given by Core Asset */
{
	/* Return value is initialized to:                                                                                     */
	/* "New signal pattern was not triggered in this callback function, so it has to be restarted by the calling function" */
	boolean RetVal_bo = FALSE;

	/* Check if any pulses are sent via analog output */
	if (TRUE == rb_co_AnalogOutActive_bo)
	{
		/* Determine and send next pulse */
		rb_co_SetNextPulseRT();
	}
	/* No pulses shall be sent via analog output */
	else
	{
		/* No new signal pattern has to be restarted by the calling function */
		RetVal_bo = TRUE;
	}

	return RetVal_bo;
}


/* ****************************************************************************
 * API private: rb_co_ResetAnalogEvents
 *
 * Description:
 *    Reset events of analog output.
 *
 * Arguments:
 *    None
 *
 * Return:
 *    None
 *
 * Usage guide & Scheduling:
 *    Called in BG.
 *
 * Remarks:
 *    None
 * ****************************************************************************/
static void rb_co_ResetAnalogEvents(void)
{

	/* Array with all possible pulse widths */
	const uint8 PulseWidths_cau8[RB_co_NumberPulseWidthsACA_cu8] =
	{
		RB_co_Pulse50ms_cu8,
		RB_co_Pulse100ms_cu8,
		RB_co_Pulse150ms_cu8
	};

	/* Loop counter */
	uint8 ArrayIndex_u8;

	/* Check if on analog out any pulse is currently sent */
	if ((TRUE == rb_co_PulseWidthActive_abo[RB_co_Pulse50ms_cu8 - 1u]) ||
		(TRUE == rb_co_PulseWidthActive_abo[RB_co_Pulse100ms_cu8 - 1u]) ||
		(TRUE == rb_co_PulseWidthActive_abo[RB_co_Pulse150ms_cu8 - 1u]))
	{
		/* Check which ACA pulse widths are currently active */
		for (ArrayIndex_u8 = 0; ArrayIndex_u8 < RB_co_NumberPulseWidthsACA_cu8; ArrayIndex_u8++)
		{
			/* Is maximum number of pulses reached for this pulse width? */
			if (rb_co_AnalogPulseCounter_au8[ArrayIndex_u8] >= RB_co_MaxNumberPulses_cu8)
			{
				/* Reset "active" indication and pulse counter */
				rb_co_PulseWidthActive_abo[ArrayIndex_u8] = FALSE;
				rb_co_AnalogPulseCounter_au8[ArrayIndex_u8] = 0;

				/* Reset deactivated elements in sequence */
				/* Current pulse width was on first position of sequence */
				if (PulseWidths_cau8[ArrayIndex_u8] == rb_co_AnalogSequence_au8[0])
				{
					/* Shift rear elements of sequence to the beginning */
					rb_co_AnalogSequence_au8[0] = rb_co_AnalogSequence_au8[1];
					rb_co_AnalogSequence_au8[1] = rb_co_AnalogSequence_au8[2];
					rb_co_AnalogSequence_au8[2] = 0u;
				}
				/* Current pulse width was on second position of sequence */
				else if (PulseWidths_cau8[ArrayIndex_u8] == rb_co_AnalogSequence_au8[1])
				{
					/* Shift last element of sequence to the middle */
					rb_co_AnalogSequence_au8[1] = rb_co_AnalogSequence_au8[2];
					rb_co_AnalogSequence_au8[2] = 0u;
				}
				/* Current pulse width was on last position of sequence */
				else
				{
					rb_co_AnalogSequence_au8[2] = 0u;
				}
			}
		}
	}
	/* No pulses to be sent on analog out any more, but indicator still active */
	else if (TRUE == rb_co_AnalogOutActive_bo)
	{
		/* Reset indicator as no analog output pulses to be sent any more */
		rb_co_AnalogOutActive_bo = FALSE;

		/* Reset sequence indicator to first element of sequence */
		rb_co_AnalogSequenceIndicator_u8 = 0;

		/* Reset diag indicator as no analog output pulses to be sent any more */
		rb_co_DiagAnalogActive_bo = FALSE;
	}
	else
	{
		/* Needed according programming guideline */
	}

}


/* ****************************************************************************
 * API private: rb_co_PrepareAnalogSequence
 *
 * Description:
 *    Prepares the analog output sequence.
 *
 * Arguments:
 *    PulseWidthType_u8: Determines the pulse width type (50ms / 100ms / 150ms)
 *
 * Return:
 *    None
 *
 * Usage guide & Scheduling:
 *    Called in BG when a new crash event was detected.
 *
 * Remarks:
 *    None
 * ****************************************************************************/
static void rb_co_PrepareAnalogSequence(uint8 PulseWidthType_u8)
{
	/* Reset counter */
	rb_co_AnalogPulseCounter_au8[PulseWidthType_u8 - 1u] = 0;
	/* Set active indication to "active" */
	rb_co_PulseWidthActive_abo[PulseWidthType_u8 - 1u] = TRUE;

	 /* Check that given pulse type is not yet contained in sequence */
	if ((rb_co_AnalogSequence_au8[0] != PulseWidthType_u8) AND_THEN
		(rb_co_AnalogSequence_au8[1] != PulseWidthType_u8) AND_THEN
		(rb_co_AnalogSequence_au8[2] != PulseWidthType_u8) )
	{
		/*  Sequence was empty */
		if (0u == rb_co_AnalogSequence_au8[0])
		{
			rb_co_AnalogSequence_au8[0] = PulseWidthType_u8;
		}
		/* One pulse type is already sent, so add second pulse type */
		else if (0u == rb_co_AnalogSequence_au8[1])
		{
			rb_co_AnalogSequence_au8[1] = PulseWidthType_u8;
		}
		/* Two different pulse types are already sent, so add third pulse type */
		else
		{
			rb_co_AnalogSequence_au8[2] = PulseWidthType_u8;
		}
	}
}


/* ****************************************************************************
 * API private: rb_co_SetFirstPulse
 *
 * Description:
 *    Starts the first pulse of the analog output sequence.
 *
 * Arguments:
 *    None
 *
 * Return:
 *    None
 *
 * Usage guide & Scheduling:
 *    Called from rb_co_CustENSCyclic2ms.
 *
 * Remarks:
 *    None
 * ****************************************************************************/
static void rb_co_SetFirstPulse(void)
{

	/* Is next pulse 50ms? */
	if (RB_co_Pulse50ms_cu8 == rb_co_AnalogSequence_au8[rb_co_AnalogSequenceIndicator_u8])
	{
		/* Increment pulse counter */
		rb_co_AnalogPulseCounter_au8[(RB_co_Pulse50ms_cu8 - 1u)] = (rb_co_AnalogPulseCounter_au8[(RB_co_Pulse50ms_cu8 - 1u)]) + 1u;

		/* Update new pattern on the channel */
		rb_coas_SendAnalogSignal(rb_co_Impact50_e, 0u);

	}
	/* Is next pulse 100ms? */
	if (RB_co_Pulse100ms_cu8 == rb_co_AnalogSequence_au8[rb_co_AnalogSequenceIndicator_u8])
	{
		/* Increment pulse counter */
		rb_co_AnalogPulseCounter_au8[(RB_co_Pulse100ms_cu8 - 1u)] = (rb_co_AnalogPulseCounter_au8[(RB_co_Pulse100ms_cu8 - 1u)]) + 1u;

		/* Update new pattern on the channel */
		rb_coas_SendAnalogSignal(rb_co_Impact100_e, 0u);

	}
	/* Is next pulse 150ms? */
	if (RB_co_Pulse150ms_cu8 == rb_co_AnalogSequence_au8[rb_co_AnalogSequenceIndicator_u8])
	{
		/* Increment pulse counter */
		rb_co_AnalogPulseCounter_au8[(RB_co_Pulse150ms_cu8 - 1u)] = (rb_co_AnalogPulseCounter_au8[(RB_co_Pulse150ms_cu8 - 1u)]) + 1u;

		/* Update new pattern on the channel */
		rb_coas_SendAnalogSignal(rb_co_Impact150_e, 0u);

	}
}

/* ****************************************************************************
 * API private: rb_co_SetNextPulseRT
 *
 * Description:
 *    Determines and starts the next pulse of the analog output sequence.
 *
 * Arguments:
 *    None
 *
 * Return:
 *    None
 *
 * Usage guide & Scheduling:
 *    Called from rb_coas_CallBackAfterOverallCycleRT.
 *
 * Remarks:
 *    None
 * ****************************************************************************/
static void rb_co_SetNextPulseRT(void)
{

	/* Next element of pulse width sequence */
	rb_co_AnalogSequenceIndicator_u8++;
	/* Repeat sequence when finished */
	rb_co_AnalogSequenceIndicator_u8 = rb_co_AnalogSequenceIndicator_u8 % RB_co_NumberPulseWidthsACA_cu8;

	/* When sequence is shorter than maximum length, restart from the beginning */
	if (0u == rb_co_AnalogSequence_au8[rb_co_AnalogSequenceIndicator_u8])
	{
		rb_co_AnalogSequenceIndicator_u8 = 0;
	}

	/* Is next pulse 50ms? */
	if (RB_co_Pulse50ms_cu8 == rb_co_AnalogSequence_au8[rb_co_AnalogSequenceIndicator_u8])
	{
		/* Increment pulse counter */
		rb_co_AnalogPulseCounter_au8[(RB_co_Pulse50ms_cu8 - 1u)] = (rb_co_AnalogPulseCounter_au8[(RB_co_Pulse50ms_cu8 - 1u)]) + 1u;

		/* Update new pattern on the channel */
		rb_coas_SendAnalogSignalRT(rb_co_Impact50_e, 0u);
	}
	/* Is next pulse 100ms? */
	else if (RB_co_Pulse100ms_cu8 == rb_co_AnalogSequence_au8[rb_co_AnalogSequenceIndicator_u8])
	{
		/* Increment pulse counter */
		rb_co_AnalogPulseCounter_au8[(RB_co_Pulse100ms_cu8 - 1u)] = (rb_co_AnalogPulseCounter_au8[(RB_co_Pulse100ms_cu8 - 1u)]) + 1u;

		/* Update new pattern on the channel */
		rb_coas_SendAnalogSignalRT(rb_co_Impact100_e, 0u);
	}
	/* Is next pulse 150ms? */
	else if (RB_co_Pulse150ms_cu8 == rb_co_AnalogSequence_au8[rb_co_AnalogSequenceIndicator_u8])
	{
		/* Increment pulse counter */
		rb_co_AnalogPulseCounter_au8[(RB_co_Pulse150ms_cu8 - 1u)] = (rb_co_AnalogPulseCounter_au8[(RB_co_Pulse150ms_cu8 - 1u)]) + 1u;

		/* Update new pattern on the channel */
		rb_coas_SendAnalogSignalRT(rb_co_Impact150_e, 0u);
	}
	else
	{
		/* else-path needed according programming standard / QAC check */
	}

}


/* ****************************************************************************
 * API private: rb_co_DetectOverVoltageFault
 *
 * Description:
 *    Detects the OverVoltage on KL15 and KL30.
 *    Set flag, if the Voltage on KL15 and KL30 is detected > 22V for 100ms. AND
 *    Reset data of Pulses which is used by Callback function to generate pulses.
 *
 * Arguments:
 *    None
 *
 * Return:
 *    None
 *
 * Usage guide & Scheduling:
 *    Called from rb_co_CustAnalog2msBG.
 *
 * Remarks:
 *    None
 * ****************************************************************************/
static void rb_co_DetectOverVoltageFault(void)
{
	uint16 Clamp30Voltage_u16;
	uint16 Clamp15Voltage_u16;
	static uint8 rb_co_OverVoltageDetectedCnt_u8 = 0u;

	/* Read KL 15 and KL30 voltage */
	Clamp30Voltage_u16 = rb_pom_CustReadBatteryVoltage();
	Clamp15Voltage_u16 =  rb_pom_CustReadIgnitionVoltage();
	/* Detect Over voltage on KL15 and KL30
	 * if it exceed 22V for 100ms, set flag to not to update ACA*/
	if( ( (Clamp30Voltage_u16 >= 2200u) AND_THEN (Clamp30Voltage_u16 < 0xFFFFu) ) OR_ELSE
			( (Clamp15Voltage_u16 >= 2200u) AND_THEN (Clamp15Voltage_u16 < 0xFFFFu)) )
	{

		/* The function is called from 2ms, if 50*2 = 100ms */
		if(rb_co_OverVoltageDetectedCnt_u8 < 50u)
		{
			/*if voltage is greater than 22V, increment the counter*/
			rb_co_OverVoltageDetectedCnt_u8 = rb_co_OverVoltageDetectedCnt_u8 + 1u;
		}
		else
		{
			/* The over voltage detected for 100 ms -> Set flag to deactivate ACA*/
			rb_co_AllowedActivateACA_bo = FALSE;

			/* Reset data and pulse counter which is used for Pulse Generation */
			rb_co_PulseWidthActive_abo[0] = FALSE;
			rb_co_PulseWidthActive_abo[1] = FALSE;
			rb_co_PulseWidthActive_abo[2] = FALSE;
			rb_co_AnalogSequence_au8[0] = 0;
			rb_co_AnalogSequence_au8[1] = 0;
			rb_co_AnalogSequence_au8[2] = 0;
			rb_co_AnalogPulseCounter_au8[0] = 0u;
			rb_co_AnalogPulseCounter_au8[1] = 0u;
			rb_co_AnalogPulseCounter_au8[2] = 0u;
		}
	}
	else
	{
		/*Reset the counter*/
		rb_co_OverVoltageDetectedCnt_u8 = 0u;

		/*Reset ACA activation flag*/
		rb_co_AllowedActivateACA_bo = TRUE;

	}


}
