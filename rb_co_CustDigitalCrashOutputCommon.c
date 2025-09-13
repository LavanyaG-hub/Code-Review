/* MANUAL_CODE_START (Bosch Copyright) */
/* ************************************************************************** *
 *
 *  Copyright (c) 2023 Robert Bosch GmbH, Germany
 *  All rights reserved.
 *
 * ************************************************************************** */
/* MANUAL_CODE_END */

/*
 *
 * Description:
 * This module notifies the crash information to the other subsystems in the vehicle by using hard wired PWM signal and FLEXRAY / LIN signals.
 * Customer Specific Crash Output.
 *
 *
 *
 * Code generator version : 4.8.15/2021-01-25
 */

/* ***** SECTION Includes ***** */
/* PRQA S 1-9999 suppress_foreign_warnings_rb_co_CustDigitalCrashOutputCommon_c */
/*** Direct Includes of dependent foreign components ***/
#include "rb_cdem_CustDiagnosticEventMgr_Interfaces.h"
#include "rb_smm_CustSleepModeManagement_Interfaces.h"

/* MANUAL_CODE_START (Manual Includes) */
#include "rb_co_CrashOutput_Cfg.h"
#include "rb_smm_CustSleepModeManagement.h"
#include "rb_sqmf_squibfiring.h"
#include "rb_sycg_SysConfGeneral.h"
#include "rb_bswm_BasicSwMgt.h"
#include "rb_sycf_SysConfSqm.h"
#include "rb_co_CustDigitalCrashOutputCommon.h"
#include "rb_co_CustCrashOutput_Interfaces.h"
#include "rb_drc_RoutineControl_Cfg.h" /* own configuration header */
#ifdef RB_co_CustDigitalCrashOutputMMAOption_cs
#include "rb_co_CustDigitalCrashOutputMMA.h"
#endif
#ifdef RB_co_CustPostCrashOutputOption_cs
#include "rb_co_CustPostCrashOutput.h"
#else
#include "rb_co_CustPostCrashOutputTPL.h"
#endif
#ifdef RB_mcb_CustMultiCollisionBrakeMgtOption_cs
#include "rb_mcb_CustMultiCollisionBrakeMgt.h"
#include "rb_mcbm_CustMainPathMgt.h"
#include "rb_mcbi_CustMultiCollisionBrakeMgtInterface.h"
#endif
#include "rb_co_CustDigitalCrashOutput_Cfg.h"
/* MANUAL_CODE_END */
/* PRQA L:suppress_foreign_warnings_rb_co_CustDigitalCrashOutputCommon_c */
/* ***** END_SECTION Includes ***** */


/*** Interface includes of own component ***/
#ifdef RB_co_CustDigitalCrashOutputMRAOption_cs
#include "rb_co_CustDigitalCrashOutputMRA.h"
#endif
#include "rb_co_CustAnalogCrashOutput.h"

/* ***** SECTION Local Definitions ***** */

/*** Local typedefs ***/

/*** User constants & defines ***/

/* Value to indicate VehPosRoof */
#define RB_co_CustVehPosRoof_cu8 (uint8)1 /* private */

/* Value to indicate CustVehPosSLeft */
#define RB_co_CustVehPosSLeft_cu8 (uint8)2 /* private */

/* Value to indicate VehPosSRight_cu8 */
#define RB_co_CustVehPosSRight_cu8 (uint8)3 /* private */

/* Value to indicate CustVehPosSna */
#define RB_co_CustVehPosSna_cu8 (uint8)7/* private */

/* Value to indicate CustVehPosSna */
#define RB_co_CustImpctHvChrgNoCrash_cu8 (uint8)2 /* private */

/* Description missing in Model! */
#define RB_co_CustVehPosAllMask_cu32 	(RB_co_CustVehPosRoofMask_cu32 | RB_co_CustVehPosSLeftMask_cu32 | RB_co_CustVehPosSRightMask_cu32 | RB_co_CustVehPosSnaMask_cu32) /* private */

/* Mask to indicate ECU not locked. */
#define RB_co_EcuNotLocked_cu8 (uint8)1 /* private */

/* Mask to indicate ECU KeyID mismatch */
#define RB_co_EcuKeyIDMismatch_cu8 (uint8)2 /* private */

/* Mask to indicate HV charging mode */
#define RB_co_HVChargingMode_cu8 (uint8)4 /* private */

/* Mask value to indicate valid values for the Analog crashes */
#define RB_co_CustDiagAnalogMask_cu32 (RB_co_DiagPulseWidth50ms_cb32|RB_co_DiagPulseWidth100ms_cb32|RB_co_DiagPulseWidth150ms_cb32)
/* RB_co_CustCoSARTrigMask_cu32
 * Description missing in Model!
 */
#define rb_co_CustCoSARTrigMask_cu32 RB_utbm_GetSingleBitMaskU32(rb_co_SARTrig_e)


/* Pointer To Store the Starting Address Of Array Received from Diag */
static const uint8 *rb_co_CoDiagRxSignals_pcu8;

/* MANUAL_CODE_START (Local constants and typedefs) */
/* MANUAL_CODE_END */


/*** Modeled variable definitions ***/
/*** Protected Data Interfaces (PDIs) ***/
/*! rb_co_COCustBATDHVTriggerStatus_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_COCustBATDHVTriggerStatus_bo;

/*! rb_co_COCustBATDHVTriggerStatusCopy_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_COCustBATDHVTriggerStatusCopy_bo;

/*! rb_co_COCustTriggersRT_u32:  Purpose:
 Goal: To get the Algo Trigger info from CA Crash Output*/
uint32 rb_co_COCustTriggersRT_u32;

/*! rb_co_COCustTriggersCpy_u32:  Purpose:
 Goal: To provide the Algo Crash Information to POST CRASH feature */
uint32 rb_co_COCustTriggersCpy_u32;

/*! rb_co_COMfaultPresentsts_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_COMfaultPresentsts_bo;

/*! rb_co_COMfaultRecoverysts_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_COMfaultRecoverysts_bo;

/*! rb_co_CrashOutputFltStatus_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_CrashOutputFltStatus_bo;

/*! rb_co_CrashTriggerStatus_u32:  Purpose:
 Goal:
 Brief Description: */
uint32 rb_co_CrashTriggerStatus_u32;

/*! rb_co_CrashTriggerStatusCopy_u32:  Purpose:
 Goal:
 Brief Description: */
uint32 rb_co_CrashTriggerStatusCopy_u32;

/*! rb_co_CustSendPyroFltRec_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_CustSendPyroFltRec_bo;

/*! rb_co_DiagCOActive_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_DiagCOActive_bo;

/*! rb_co_DiagCrashDetectionFlt_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_DiagCrashDetectionFlt_bo;

/*! rb_co_DiagDigitalActive_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_DiagDigitalActive_bo;

/*! rb_co_DiagPyroFlt_bo:  Purpose:
 Goal:
 Brief Description: */
boolean rb_co_DiagPyroFlt_bo;

/*! ARRAY rb_co_CrashSignalSetTime_au32[31]:     This array holds the time stamps */
/* Size based on the values in the array "rb_co_CustDiagTimIndx_cau8" */
uint32 rb_co_CrashSignalSetTime_au32 [31];

/*! rb_co_CustCrashActiveFlag_bo:  Flag to indicate sleep mode not to go to sleep in case of Crash */
boolean rb_co_CustCrashActiveFlag_bo;

/*! rb_co_CrashSignalSetTimePyro_u32:  This variable holds the time stamps for Pyro */
uint32 rb_co_CrashSignalSetTimePyro_u32;

/*** Public Data Interfaces (PDIs) ***/
/*! rb_co_CODiagTriggersRT_u32:  Description missing in Model! */
uint32 rb_co_CODiagTriggersRT_u32; /* public */

/*! rb_co_SupportedDigitalCoTriggers_u32:  variable to support mask for Impact SL/SR, PSIS and
 * min signals along with the existing mask */
uint32 rb_co_SupportedDigitalCoTriggers_u32 = 0u;

volatile uint32 rb_co_COPSISTrigger_u32;

/*** Variable definitions inherited via architecture branch ***/
/*** User variable definitions ***/
/* MANUAL_CODE_START (Local variables) */

/* MANUAL_CODE_END */
/* ***** END_SECTION Local Definitions ***** */

/* ***** SECTION Local function prototypes ***** */
/*** modeled private function prototypes ***/
static void rb_co_CustComCrashFaultHandling(uint8 SignalVal_u8);
static void rb_co_CustComsignalHandlingInit(void);
static void rb_co_CheckCrashFaultsStatus(void);
static void rb_co_CheckCOMFaultsStatus(void);
static void rb_co_CustDigitalCOResetHandling(void);
static void rb_co_CustPyrofusePostcrashSignalling(void);
static void rb_co_CustCoResetToDefault(void);
static void rb_co_CustDigitalDiagCOHandling(void);
static void rb_co_CustChkCOTrigFromDynnamicBit(void);
/* MANUAL_CODE_START (Manual function prototypes) */
extern void rb_co_CustPostCrashCyclic(void);
/* Common ROM Cfg table to send the impact signals on Fr and Lin bus */
extern const rb_co_FrLinSgl_tst rb_co_SigInfo_cast[rb_co_CrashTriggersMAX_e];
/* MANUAL_CODE_END */
/* ***** END_SECTION Local function prototypes ***** */

/* ***** SECTION API functions ***** */
/* ****************************************************************************
 * API public: rb_co_CustDigitalCOInit
 *
 * Description:
 *   This API is used for performing initialization of digital Crash Output module.
 * Arguments:
 *   None
 * Return:
 *    None
 * Usage guide & Scheduling:
 *    Called after SDM is available
 * Remarks:
 *   None.
 * ****************************************************************************/
void rb_co_CustDigitalCOInit(void)
{
	/* MANUAL_CODE_START (99c5a154-bdf0-40fb-b5f0-12820201d3b6) */

	/* Enable/Disable the Impact Side, PSIS and Min signals based on Special behavior bit status */
	rb_co_CustChkCOTrigFromDynnamicBit();
	/* COM signal handling */
	rb_co_CustComsignalHandlingInit();
	/* Send Vehicle Position on Bus*/
	rb_cdi_CustSendVehPosn(FALSE);
	/* Update Pyro fault signal as no fault */
	rb_cdi_CustSendPyrofuseFlt(RB_co_CustComFltFalse_cu8);
	/* Send Impact HV charge status on Bus*/
	rb_cli_CustSendImpctHvChrg(RB_co_CustComNotActv_cu8);
	/* Copy impact signals to signal group in COM */
	rb_cdi_CopyImpactDataToSignalGroup();

	/* MANUAL_CODE_END */

}
/* ***** END_API [rb_co_CustDigitalCOInit] ***** */

/* ****************************************************************************
 * API public: rb_co_CustDigitalCOCyclic
 *
 * Description:
 *   This API is used to handle the Event notification signal customer specific requirements.
 * Arguments:
 *    None
 * Return:
 *   None
 * Usage guide & Scheduling:
 *    Should be called in 2ms Background in all the operational modes of ECU.
 * Remarks:
 *   None
 * ****************************************************************************/
void rb_co_CustDigitalCOCyclic(void){
	/* MANUAL_CODE_START (74894518-d21a-4f39-9883-184aa0039ab8) */
	/* Check if bus fault is present */
	rb_co_CheckCOMFaultsStatus();

	/* Check if crash detected fault has occurred */
	rb_co_CheckCrashFaultsStatus();
	/* Check for the pre-conditions to enable crash output.
	 * -ECU has to be locked
	 * -Key check should be error free
	 * ECU Mounting direction related faults shall not be present */
	/*For real crash, ECU should be locked and key ID check should be passed. For diag request, preconditions not required*/

	/* Clear bits related to new CO FCL Algo Notification flags  */
	rb_co_COAlgoTriggersRT_u32 &= rb_co_SupportedDigitalCoTriggers_u32; 

	if(TRUE == rb_co_CustCyclicPreconditionCheck())
	{
		/* Suspends (disables) all interrupts to prevent interruption from real time or IRQ*/
		rb_osi_SuspendAllInterruptsALL();
		/* Get the algo trigger information */
		rb_co_COCustTriggersRT_u32 = rb_co_COAlgoTriggersRT_u32;
		/* Clear Algo. trigger bits . Retain the SAR bit using  mask */
		rb_co_COAlgoTriggersRT_u32 &= RB_co_CustCoSARTrigMask_cu32;
		/* Enables the interrupts disabled by rb_osi_SuspendAllInterruptsALL  */
		rb_osi_ResumeAllInterruptsALL();

		/* Clear SAR bit as CRO doesn't need SAR bit information */
		rb_co_COCustTriggersRT_u32 &= (~RB_co_CustCoSARTrigMask_cu32);
		/* to be use to set and clear ,Post Crash timestamp */
		rb_co_COCustTriggersCpy_u32 |= rb_co_COCustTriggersRT_u32;

		/* Check if HV Charge is active */
		if(rb_smm_IsHVChargingActive()==FALSE)
		{
			/* Function call to check BATDHV threshold and send fault signal accordingly */
			rb_co_CustPyrofusePostcrashSignalling();
			/* Check for analog output */
			rb_co_CustAnalog2msBG();
		}
		/* compiler switch to have POST CRASH ASIL-C implementation */
#ifdef RB_mcb_CustMultiCollisionBrakeMgtOption_cs
		/* Check if MCB is enabled*/
		if (rb_mcb_CheckMCBDisable() == FALSE)
		{
			/* Call MCB cyclic API */
			rb_mcb_Cyclic2ms();
		}
		else
		{
			/* Reset the all CO impact signal, as MCB deactivated */
			rb_co_CustCoResetToDefault();
			/*Suspends (diables) all interrupts to prevent interruption from real time or IRQ*/
			rb_osi_SuspendAllInterruptsALL();
			/* Clear the trigger bit if Precondition not met*/
			/*Clear Algo. trigger bits .Retain the SAR it using mask*/
			rb_co_COAlgoTriggersRT_u32 &= RB_co_CustCoSARTrigMask_cu32;
			/*compiler switch will be removed once MMA will have POST CRASH ASIL-C implementation*/
#ifdef RB_mcb_CustMultiCollisionBrakeMgtOption_cs
			rb_co_MCBAlgoTriggerscopyRT_u32=0u;
#endif
			/*Enables the interrupts disabled by rb_osi_SuspendAllInterruptsALL */
			rb_osi_ResumeAllInterruptsALL();
			/*After precondition failure,if precondition passed Clearing trigger variable to avoid CO trigger.*/
			rb_co_COCustTriggersCpy_u32=0u;
			rb_co_CrashTriggerStatus_u32=0u;
			rb_co_CrashTriggerStatusCopy_u32=0u;
		}
#endif
		/* Call post crash ASIL B cyclic API - Protected with module level compiler switch */
		rb_co_CustPostCrashCyclic();

		/* Check if Diag active and triggered */
		if (rb_co_DiagCOActive_bo == TRUE)    // Diag crash output
		{
			/* Check whether diag analog is requested */
			if ((rb_co_CODiagTriggersRT_u32 & (RB_co_CustDiagAnalogMask_cu32)) > 0U)
			{
				rb_co_DiagAnalogActive_bo = TRUE;
			}
			rb_co_CustDigitalDiagCOHandling();   /* Trigger all Diag CO signals, except Analog CO */
		}

		/* Handle CO reset */
		rb_co_CustDigitalCOResetHandling();

		/* Clear the flag if both analog and digital diag is inactive */
		if ((rb_co_DiagDigitalActive_bo == FALSE) && (rb_co_DiagAnalogActive_bo == FALSE))
		{
			rb_co_DiagCOActive_bo = FALSE;
		}
	}
	else
	{
		/* Reset all the impact signal as Precondition not satisfied*/
		rb_co_CustCoResetToDefault();
		/* Suspends (disables) all interrupts to prevent interruption from real time or IRQ*/
		rb_osi_SuspendAllInterruptsALL();
		/*Clear the trigger bit if Precondition not met */
		/* Clear Algo. trigger bits . Retain the SAR bit using  mask */
		rb_co_COAlgoTriggersRT_u32 &= RB_co_CustCoSARTrigMask_cu32;
		/*compiler switch to have POST CRASH ASIL-C implementation*/
#ifdef RB_mcb_CustMultiCollisionBrakeMgtOption_cs
		rb_co_MCBAlgoTriggerscopyRT_u32 = 0u;
#endif
		/* Enables the interrupts disabled by rb_osi_SuspendAllInterruptsALL  */
		rb_osi_ResumeAllInterruptsALL();

	}

	/* Copy Impact data to signal group */
	rb_cdi_CopyImpactDataToSignalGroup();
	if(rb_co_CrashTriggerStatus_u32 == 0u)
	{
		/* Clear status if no impact active bits are transmitted */
		rb_co_CustCrashActiveFlag_bo = FALSE;
	}
	/* MANUAL_CODE_END */

}/*PRQA S 7010 */ /*PRQA S 7003 */ /*Advisory - There is no functional impact *//*PRQA S 7008 */ /* QAC deviation as per the CDAB12_006  */
/* ***** END_API [rb_co_CustDigitalCOCyclic] ***** */

/* ****************************************************************************
 * API public: rb_co_CustDigitalDiagCOHandling
 *
 * Description:
 *   This API is used to handle Digital Diag Crash Output.
 * Arguments:
 *    None
 * Return:
 *   None
 * Usage guide & Scheduling:
 *    Should be called in 2ms Background in all the operational modes of ECU.
 * Remarks:
 *   None
 * ****************************************************************************/
static void rb_co_CustDigitalDiagCOHandling(void)
{
	uint8 SigIndex_u8; /* Loop to all the signals in DiagCo table */
	boolean IsTimerExpired_bo;
	static boolean rb_co_IsDiagTrigTimerStarted_bo;
	static uint32 rb_co_CoDiagTimer_u32; /*Timer variable to check CO timer for the Diag_CO */

	/*Check if RID $0312 is requested and Diag digital flag is set*/
	if(rb_co_DiagDigitalActive_bo == TRUE)
	{
		/* Get current time stamp value from timer */
		if (rb_co_IsDiagTrigTimerStarted_bo == FALSE)
		{
			rb_co_CoDiagTimer_u32 = rb_tim_GetTimestampALL(); /* Common timer for all signals */
			rb_co_IsDiagTrigTimerStarted_bo = TRUE;
		}

		for(SigIndex_u8=0;SigIndex_u8<RB_drc_DiagCOConfigTable_cu8;SigIndex_u8++)
		{
			if(rb_drc_ActDiagCoInfo_cast[SigIndex_u8].CallbackFunction2 == NULL_PTR)
			{
				/*update single bit signal on bus*/
				/* CallBack Function used to call the Single Bit Signals and set their values as requested */
				rb_drc_ActDiagCoInfo_cast[SigIndex_u8].CallbackFunction1(rb_drc_ActDiagCoInfo_cast[SigIndex_u8].rb_Signal_name_en,(boolean) (rb_co_CoDiagRxSignals_pcu8[SigIndex_u8]));
			}
			else
			{
				/* CallBack Function used to call the Multi Bit Signals and set their values as requested */
				rb_drc_ActDiagCoInfo_cast[SigIndex_u8].CallbackFunction2((uint8) (rb_co_CoDiagRxSignals_pcu8[SigIndex_u8]));
			}
		}

		IsTimerExpired_bo = rb_tim_IsTimeExpiredALL((uint32)RB_co_DigitalCrashDuration_cu32,rb_co_CoDiagTimer_u32);
		/*Check for the Diag timer is started and expired*/
		if (IsTimerExpired_bo == TRUE)
		{
			rb_co_DiagDigitalActive_bo = FALSE;
			rb_co_IsDiagTrigTimerStarted_bo = FALSE;
			/*Reset the signal values which is purely triggered as per algo flags and purely independent with faults availability*/
			for(SigIndex_u8=0;SigIndex_u8<RB_drc_DiagCOConfigTable_cu8;SigIndex_u8++)
			{
				if((rb_drc_ActDiagCoInfo_cast[SigIndex_u8].CallbackFunction2 == NULL_PTR) AND_THEN
						(rb_drc_ActDiagCoInfo_cast[SigIndex_u8].Bitmask_u8 == 0x01u))
					/* Check if Signal length is 1 bit (i.e., Mask is 0x01) - Then only Reset the Signal to 0x00.
					 * All 1 bit signal have 0x00 - No Crash; 0x01 - Crash Triggered */
				{
					rb_drc_ActDiagCoInfo_cast[SigIndex_u8].CallbackFunction1(rb_drc_ActDiagCoInfo_cast[SigIndex_u8].rb_Signal_name_en,0x00);
				}
				else
				{
					/* send vehicle Position on bus */
					/* Same signal rb_cap_TxFrSglVehPosn_e is used for vehicle positions right, left and roof
					 *  hence any one position we can send as an argument*/
					rb_co_CustSendVehiclePosition(rb_co_ImpactVehPosiSideRight_e);

					/* Reset Multi bit Crash Signal - PSIS / ImpactMin  with 2 bit length. */

					/* If requested value is '0x03 - SNA'value means feature not enabled, then signal shall not be reset to '0x01 - No Crash'
					 * Reset value to '0x01 - No Crash'; Only if 'PSIS; ImpactMin' related Dynamic Bit are set */
					if ((rb_sycg_IsDynamicBitSet(rb_sycg_Impact5PSISLRCrashOutputSignals_e) == TRUE) AND_THEN
							((rb_cap_TxFrSglImpactPSISLt_e >= rb_drc_ActDiagCoInfo_cast[SigIndex_u8].rb_Signal_name_en) AND_THEN
									(rb_cap_TxFrSglImpactPSISRt_e <= rb_drc_ActDiagCoInfo_cast[SigIndex_u8].rb_Signal_name_en)))
					{
						/* Reset signal to '0x01 - No Crash' value */
						rb_drc_ActDiagCoInfo_cast[SigIndex_u8].CallbackFunction1(rb_drc_ActDiagCoInfo_cast[SigIndex_u8].rb_Signal_name_en,0x01);
					}
					if ((rb_sycg_IsDynamicBitSet(rb_sycg_Impact5MinImpactCrashOutputSignals_e) == TRUE) AND_THEN
							((rb_cap_TxFrSglImpactMinF_e >= rb_drc_ActDiagCoInfo_cast[SigIndex_u8].rb_Signal_name_en) AND_THEN
									(rb_cap_TxFrSglImpactMinSR_e <= rb_drc_ActDiagCoInfo_cast[SigIndex_u8].rb_Signal_name_en)))
					{
						/* Reset signal to '0x01 - No Crash' value */
						rb_drc_ActDiagCoInfo_cast[SigIndex_u8].CallbackFunction1(rb_drc_ActDiagCoInfo_cast[SigIndex_u8].rb_Signal_name_en,0x01);
					}
				}
			}
		}
	}
}


/* ****************************************************************************
 * API public: rb_co_CustNotifyAlgoTriggerRT
 *
 * Description:
 *  Notifies crash detected information to sleep mode module

 * Arguments:
 *   AlgoTriggerInfoRT_u32: Purpose:
Goal:
Brief Description:
 * Return:
 *   None
 * Usage guide & Scheduling:
 *   None
 * Remarks:
 *   None
 * ****************************************************************************/
void rb_co_CustNotifyAlgoTriggerRT(uint32 AlgoTriggerInfoRT_u32)/*PRQA S 3206 */ /* QAC deviation as per the CDAB12_060  */
{
	/* MANUAL_CODE_START (c9069290-f2b8-49ea-85e4-bbffbb52829c) */
	rb_co_COPSISTrigger_u32 |= AlgoTriggerInfoRT_u32;
	/* Clear bits related to new CO FCL Algo Notification flags */
	AlgoTriggerInfoRT_u32 &= rb_co_SupportedDigitalCoTriggers_u32;
	/* Call other related functionality if resulting value is Non-zero */
	if (AlgoTriggerInfoRT_u32 != 0u)
	{
		/* Provide notification to sleep mode management */
		rb_smm_CrashDetectionNotification(); 	/* PRQA S 6685 NotifyAlgoLabel*/
		/*compiler switch to have POST CRASH ASIL-C implementation */
#ifdef RB_mcbm_CustMainPathMgtOption_cs
		rb_co_MCBAlgoTriggerscopyRT_u32 |= AlgoTriggerInfoRT_u32;
#endif
		/* PRQA L: NotifyAlgoLabel*/

	}


	/* MANUAL_CODE_END */

}
/* ***** END_API [rb_co_CustNotifyAlgoTriggerRT] ***** */

/* ****************************************************************************
 * API public: rb_co_CustSuppressFiringCondition
 *
 * Description:
 *   This API is used to provide the following information to algo.:
 *   bit 0: if set, ECU is Not Locked
 *   bit 1: if set, KeyID Mismatch
 *   bit 2: if set, HVChargingMode enabled
 * Arguments:
 *   None
 * Return:
 *   uint8
 * Usage guide & Scheduling:
 *   Called by Algo
 * Remarks:
 *   None
 * ****************************************************************************/
uint8 rb_co_CustSuppressFiringCondition(void)
{
	/* MANUAL_CODE_START (1e84a879-5713-4957-89d3-fa3f50013e48) */
	uint8 RetVal_u8 = 0;
	/* Check if Ecu is not locked*/
	if(rb_delu_CustIsEcuLocked() == FALSE)
	{
		/* Set bit 0 to indicate ECU not locked */
		RetVal_u8 = RB_co_EcuNotLocked_cu8;
	}

	/* Get the key ID status and check if Key ID is not passed */
	if(rb_delu_KeyCheckPassed_e != rb_delu_CustGetKeyCheckResult())
	{
		/* Set bit 1 to indicate KeyID mismatch */
		RetVal_u8 = RetVal_u8 | RB_co_EcuKeyIDMismatch_cu8;
	}

	/* Check if HV Charge is active */
	if(rb_smm_IsHVChargingActive()== TRUE)
	{
		/* Set bit 2 to indicate HV charging mode */
		RetVal_u8 = RetVal_u8 | RB_co_HVChargingMode_cu8;
	}

	return RetVal_u8;
	/* MANUAL_CODE_END */

}
/* ***** END_API [rb_co_CustSuppressFiringCondition] ***** */

/* ****************************************************************************
 * API public: rb_co_CustNotifyBATDHVCrashTrigger
 *
 * Description:
 *    This API is used to Notify BATDHV crash trigger status
 * Arguments:
 *   None
 * Return:
 *     TRUE : BATDHV crash is Trigger
 *   FALSE : BATDHV crash is not Trigger
 * Usage guide & Scheduling:
 *    Called by Battery Disconnect Management
 * Remarks:
 *    None
 * ****************************************************************************/
boolean rb_co_CustNotifyBATDHVCrashTrigger(void)
{
	/* MANUAL_CODE_START (822bec13-7a00-41ac-ab17-3f9765cf668e) */
	boolean BATDHVCrashStatus_bo = FALSE ;

	if(( (rb_co_COCustTriggersRT_u32) & (RB_co_CustBATDHVTriggerMask_cb32)) != 0u)
	{
		BATDHVCrashStatus_bo = TRUE ;
	}

	return  BATDHVCrashStatus_bo ;
	/* MANUAL_CODE_END */

}
/* ***** END_API [rb_co_CustNotifyBATDHVCrashTrigger] ***** */

/* ****************************************************************************
 * API public: rb_co_CustStartActTest
 *
 * Description:
 *   None
 * Arguments:
 *   CODiagTrigger_u32: Diag request from Routine control
 *   ActTestStatus_pbo: Actuator test status(TRUE or FALSE)
 * Return:
 *   None
 * Usage guide & Scheduling:
 *   None
 * Remarks:
 *   None
 * ****************************************************************************/
void rb_co_CustStartActTest(uint32 CODiagTrigger_u32, const uint8 ReceivedSignals_au8[])
{
	/* MANUAL_CODE_START (bf4b0856-7d55-420d-9f2e-fbbbcd4dc5f5) */
	/* Check if crash status is empty and Diag flag is false */
	if ((rb_co_CrashTriggerStatus_u32 == 0u) && (rb_co_DiagCOActive_bo == FALSE))
	{
		/* Get the ACO buffer from Routine control */
		rb_co_CODiagTriggersRT_u32 = CODiagTrigger_u32;
		/* Get the Digital buffer from Routine control */
		rb_co_CoDiagRxSignals_pcu8 = &ReceivedSignals_au8[0u];

		/* Set the Diag flag */
		rb_co_DiagCOActive_bo = TRUE;

		/*Set the Diag Digital Flag*/
		rb_co_DiagDigitalActive_bo = TRUE;

	}
	else
	{
		/* Clear the Diag flag */
		rb_co_DiagCOActive_bo = FALSE;
	}
	/* MANUAL_CODE_END */

}
/* ***** END_API [rb_co_CustStartActTest] ***** */

/* ****************************************************************************
 * API private: rb_co_CustComCrashFaultHandling
 *
 * Description:
 *    This API is used to handle the transmission of crash detection faults.
 * Arguments:
 *    None
 * Return:
 *    None
 * Usage guide & Scheduling:
 *    Shall be called to transmit fault information.
 * Remarks:
 *   None
 * ****************************************************************************/
static void rb_co_CustComCrashFaultHandling(uint8 SignalVal_u8)
{
	/* MANUAL_CODE_START (c1b388cd-3ae0-46bf-96a5-bd29b3d3f7e3) */
	/* Check if crash detection fault is present */
	rb_cdi_CustSendCrashDetFlt(SignalVal_u8);
	/* Do not trigger "Impct_HvChrg_ST3" signal value as 'Not Active' or 'No Crash'
	 * When Algo provided "rb_co_ImpctHvChrg_e" input in real crash scenario or
	 * Diag service requested for HV charge signal update */
	if((rb_co_DiagDigitalActive_bo == FALSE) AND_THEN ((rb_co_CrashTriggerStatus_u32 & (RB_utbm_GetSingleBitMaskU32(rb_co_ImpctHvChrg_e))) == 0u))
	{
		if(SignalVal_u8 == RB_co_CustComFltTrue_cu8)
		{
			/* Send Not active when fault is present */
			rb_cli_CustSendImpctHvChrg(RB_co_CustComNotActv_cu8);
		}
		else
		{
			/* Send No crash when fault is not present */
			rb_cli_CustSendImpctHvChrg(RB_co_CustImpctHvChrgNoCrash_cu8);
		}
	}
	/* MANUAL_CODE_END */

}
/* ***** END_API [rb_co_CustComCrashFaultHandling] ***** */

/* ****************************************************************************
 * API private: rb_co_CustComsignalHandling
 *
 * Description:
 *    This API is used to handle the transmission of crash information on Flexray and LIN.
 * Arguments:
 *    None
 * Return:
 *    None
 * Usage guide & Scheduling:
 *   Shall be called to transmit crash information.
 * Remarks:
 *    None
 * ****************************************************************************/
static void rb_co_CustComsignalHandlingInit(void)
{
	/* MANUAL_CODE_START (c6eaf43f-47e9-4f1b-b9da-f50c63747698) */
	/*To loop Crash signals*/
	rb_co_CrashTriggers_ten  CrashTrigger_en;

	/*This is the temp fix for MMA and VANEA project to send impact SL SR values as '1', when Dyn bit is not set,
	 * This condition shall be discraded once the k-matrix ORC_STAR_35_Ecu_Extract_2025_05a0_AR2011 implementation is considered*/
	if (rb_sycg_IsDynamicBitSet(rb_sycg_Impact1SideLRCrashOutputSignals_e) == FALSE)
	{
		/* Send crash signal on Flexray */
		rb_cdi_CustSendImpactStatus(rb_co_SigInfo_cast[rb_co_ImpactSL_e].FrSigName_en,rb_co_SigInfo_cast[rb_co_ImpactSL_e].FrSigValAfterCoTimeExp_u8);
		/* Send crash signal on Flexray */
		rb_cdi_CustSendImpactStatus(rb_co_SigInfo_cast[rb_co_ImpactSR_e].FrSigName_en,rb_co_SigInfo_cast[rb_co_ImpactSR_e].FrSigValAfterCoTimeExp_u8);
	}

	for(CrashTrigger_en= rb_co_ImpactF2_e; CrashTrigger_en < rb_co_CrashTriggersMAX_e; CrashTrigger_en++ )
	{
		/* Co Init value shall be sent only if related CO trigger is relevant (i.e., Not rb_co_NotToBeChked_e) OR If Algo Trigger  needs to be supported - Few Algo triggers are supported based on 'Dynamic Bits' setting */
		if((TRUE ==  rb_utbm_AreAllBitsSetU32ALL(rb_co_SupportedDigitalCoTriggers_u32,RB_utbm_GetSingleBitMaskU32(((uint32)CrashTrigger_en)))) AND_THEN
				(rb_co_SigInfo_cast[CrashTrigger_en].Deploysts_en != rb_co_NotToBeChked_e))
		{
			/* Send crash signal on Flexray */
			rb_cdi_CustSendImpactStatus(rb_co_SigInfo_cast[CrashTrigger_en].FrSigName_en,rb_co_SigInfo_cast[CrashTrigger_en].FrSigValAfterCoTimeExp_u8);
			/* Send crash signal on LIN */ 
			/*The below cdi interface shall be used to send multi bit signals on Lin bus*/
			rb_cdi_CustSendImpactStatus(rb_co_SigInfo_cast[CrashTrigger_en].LinSigName_en,rb_co_SigInfo_cast[CrashTrigger_en].LinSigValAfterCoTimeExp_u8);
		}
	}
	/* MANUAL_CODE_END */
}
/* ***** END_API [rb_co_CustComsignalHandling] ***** */

/* ****************************************************************************
 * API private: rb_co_CheckCrashFaultsStatus
 *
 * Description:
 *    This API is used to detect crash detection faults.
 * Arguments:
 *    None
 * Return:
 *    None
 * Usage guide & Scheduling:
 *    Shall be called to detect different crash.
 * Remarks:
 *   None
 * ****************************************************************************/
static void rb_co_CheckCrashFaultsStatus(void)
{
	/* MANUAL_CODE_START (f95e672d-2f15-4e79-87df-ec2d682753ac) */
	uint32 AlgoExternalResetOrDisable_u32;
	AlgoExternalResetOrDisable_u32 = ( rb_bswm_GetExtendedAlgoStateALL(rb_bswm_AlgoReset_e) | rb_bswm_GetExtendedAlgoStateALL(rb_bswm_AlgoDisable_e) );
	if(rb_co_DiagDigitalActive_bo == FALSE)
	{
		/* Check if crash detected fault has occured */

		if(TRUE == rb_co_CustCrashPreconditionCheck())
		{
			/* Transmit crash detected fault on Flexray and LIN */
			rb_co_CustComCrashFaultHandling(RB_co_CustComFltTrue_cu8);
		}
		else
		{
			/* Transmit crash detected fault on Flexray and LIN */
			rb_co_CustComCrashFaultHandling(RB_co_CustComFltFalse_cu8);
		}
		if ((rb_utbm_IsAnyBitSetU32ALL(AlgoExternalResetOrDisable_u32,RB_utbm_GetSingleBitMaskU32((uint32)RB_bswm_DisableAlgoAll_cu16))) == TRUE )
		{
			rb_cli_CustSendImpctHvChrg(RB_co_CustComNotActv_cu8);
		}

		/* Check if pyro fault has occured */
		if (rb_cdem_CustDemIsPyroFltActive() == TRUE)
		{
			/* Transmit Pyro fault on Flexray */
			rb_cdi_CustSendPyrofuseFlt(RB_co_CustComFltTrue_cu8);
		}
		else
		{
			/* Check if BATDHV is activated or not */
			if(rb_co_COCustBATDHVTriggerStatus_bo!=TRUE)
			{
				/* Transmit Pyro fault on Flexray */
				rb_cdi_CustSendPyrofuseFlt(RB_co_CustComFltFalse_cu8);
			}
		}
	}
	/* MANUAL_CODE_END */

}/*PRQA S 7010 */ /*Advisory - There is no functional impact */

/* ***** END_API [rb_co_CheckCrashFaultsStatus] ***** */

/* ****************************************************************************
 * API private: rb_co_CheckCOMFaultsStatus
 *
 * Description:
 *    This API is used to detect COM bus faults.
 * Arguments:
 *    None
 * Return:
 *    None
 * Usage guide & Scheduling:
 *    Shall be called to detect bus faults.
 * Remarks:
 *    None
 * ****************************************************************************/
static void rb_co_CheckCOMFaultsStatus(void)
{
	/* MANUAL_CODE_START (36600b95-f45a-43ea-bed5-e63f3e977535) */
	boolean LocalIsFrFltDet_bo;
	boolean LocalIsLinFltDet_bo;

	LocalIsFrFltDet_bo = rb_cfi_IsFRBusFaultDetected();

	LocalIsLinFltDet_bo = rb_cli_IsLINBusFaultDetected();

	/* Check if either Flexray bus or LIN bus fault is present */
	if((LocalIsFrFltDet_bo == TRUE) || (LocalIsLinFltDet_bo == TRUE))
	{
		rb_co_COMfaultPresentsts_bo = TRUE;
	}
	else
	{
		rb_co_COMfaultPresentsts_bo = FALSE;
	}
	/* MANUAL_CODE_END */

}
/* ***** END_API [rb_co_CheckCOMFaultsStatus] ***** */

/* ****************************************************************************
 * API private: rb_co_CustSendVehiclePosition
 *
 * Description:
 *    This API is used determine vehicle end position.
 * Arguments:
 *    None
 * Return:
 *    None
 * Usage guide & Scheduling:
 *   None
 * Remarks:
 *    None
 * ****************************************************************************/
void rb_co_CustSendVehiclePosition(rb_co_CrashTriggers_ten CrashTriggers_en)
{
	/* MANUAL_CODE_START (4a543d06-93c4-4f29-a4f8-3e203e1a30ed) */
	uint8 VehPos_u8;
	uint32 NewVehPosn_u32;
	static uint32 rb_co_OldVehPosn_u32; /*To hold the Old vehicle position value.*/

	/*Check for the Diag Digital CO not active
	 * Reason: VehPosn:SNA not applicable in the real crash scenario and same supported only in Diag service $0312.
	 * Once CO timer triggered through Diag service $0312: Vehicle position will not change value again until CO time duration exceeds.*/
	if(rb_co_DiagCOActive_bo == FALSE)
	{
		/* NewVehPosn and OldVehPosn useful for real crash scenario*/
		/* Hold the provided vehicle position as new position */
		NewVehPosn_u32 = rb_co_CrashTriggerStatus_u32 & RB_co_CustVehPosAllMask_cu32;
		/* Clear vehicle position */
		rb_co_CrashTriggerStatus_u32 = rb_utbm_ClearBitsU32ALL(rb_co_CrashTriggerStatus_u32, RB_co_CustVehPosAllMask_cu32);
		rb_co_CrashTriggerStatusCopy_u32 = rb_utbm_ClearBitsU32ALL(rb_co_CrashTriggerStatusCopy_u32, RB_co_CustVehPosAllMask_cu32);
		/* Check the New vehicle position with old position */
		if(NewVehPosn_u32 == rb_co_OldVehPosn_u32)
		{
			/* Nothing to do */
		}
		else
		{
			/* Different vehicle position received, clear old vehicle position bit and send latest Postion only on 'VehPosn' signal.
			 * Note: Algo gives 3 different end position indication - Roof; Left; Right. Only 1 indication is set at any point of Time.
			 * Also, 'VehPosn' signal can send only one value. */
			NewVehPosn_u32 = NewVehPosn_u32 & (~rb_co_OldVehPosn_u32);
		}
		/* Consider New vehicle position as Old position for next iteration */
		rb_co_OldVehPosn_u32 = NewVehPosn_u32;
		/* Update the Vehicle position as per New position provided by algo flags */
		rb_co_CrashTriggerStatus_u32 = rb_utbm_SetBitsU32ALL(rb_co_CrashTriggerStatus_u32, NewVehPosn_u32);
		/* Update the Vehicle position, it will be in use if any communication fault occur*/
		rb_co_CrashTriggerStatusCopy_u32 = rb_utbm_SetBitsU32ALL(rb_co_CrashTriggerStatusCopy_u32, NewVehPosn_u32);
	}
	/* Check if vehicle position information*/
	switch(rb_co_CrashTriggerStatus_u32 & RB_co_CustVehPosAllMask_cu32)
	{
	/* Check if vehicle end position is on the roof */
	case RB_co_CustVehPosRoofMask_cu32:
		VehPos_u8 = RB_co_CustVehPosRoof_cu8;
		break;

		/* Check if vehicle end position is on the side left */
	case RB_co_CustVehPosSLeftMask_cu32:
		VehPos_u8 = RB_co_CustVehPosSLeft_cu8;
		break;

		/* Check if vehicle end position is on the side right */
	case  RB_co_CustVehPosSRightMask_cu32:
		VehPos_u8 = RB_co_CustVehPosSRight_cu8;
		break;

		/* Check if vehicle end position is on the SNA : supported only Diag service */
	case RB_co_CustVehPosSnaMask_cu32:
		VehPos_u8 = RB_co_CustVehPosSna_cu8;
		break;

		/*vehicle end position is Unknown */
	default:
		VehPos_u8 = RB_co_CustVehPosUnknwn_cu8;
		break;
	}
	/*Send vehicle position information on Fr Bus only, it is not supported in Lin */
	rb_cdi_CustSendImpactStatus(rb_co_SigInfo_cast[CrashTriggers_en].FrSigName_en, VehPos_u8);
}
/* MANUAL_CODE_END */
/* ***** END_API [rb_co_CustSendVehiclePosition] ***** */

/* ****************************************************************************
 * API private: rb_co_CustDigitalCOResetHandling
 *
 * Description:
 *  This function to reset CO BATDHV signal after CO duration completion.

 * Arguments:
 *   None
 * Return:
 *   None
 * Usage guide & Scheduling:
 *   None
 * Remarks:
 *   None
 * ****************************************************************************/
static void rb_co_CustDigitalCOResetHandling(void)
{
	/* MANUAL_CODE_START (de77b45c-ace4-4c9a-a6da-8022341a6f04) */
	boolean IsTimerExpired_bo;
	uint32 TimeStampPyro_u32;

	/* Check whether the BATDHV triggered or not */
	if(rb_co_COCustBATDHVTriggerStatus_bo == TRUE)
	{
		/* Get the current time stamp value */
		TimeStampPyro_u32 = rb_co_CrashSignalSetTimePyro_u32;
		/* Get the CO timer duration status */
		IsTimerExpired_bo = rb_tim_IsTimeExpiredALL(((uint32)RB_co_DigitalCrashDuration_cu32),TimeStampPyro_u32);

		/* CO duration completed */
		if(IsTimerExpired_bo == TRUE)
		{
			rb_co_COCustBATDHVTriggerStatus_bo =FALSE;
			/* Transmit crash information on bus */
			rb_cdi_CustSendPyrofuseFlt(RB_co_CustComFltFalse_cu8);
			if(rb_co_COMfaultPresentsts_bo==FALSE)
			{
				rb_co_COCustBATDHVTriggerStatusCopy_bo=FALSE;
			}
		}
	}
	else
	{
		/* Do nothing */
	}
	/* MANUAL_CODE_END */
}
/* ***** END_API [rb_co_CustDigitalCOResetHandling] ***** */

/* ****************************************************************************
 * API private: rb_co_CustPyrofusePostcrashSignalling
 *
 * Description:
 *   This API is used to Send pyrofuse fault signal on the bus for 10       seconds based
 *   on BATDHV threshold being active .
 *   Two conditions are possible
 *   1.BATDHV pyrofuse solution
 *   2.48v Switch management
 * Arguments:
 *   None
 * Return:
 *   None
 * Usage guide & Scheduling:
 *    called by 2ms cyclic Api
 * Remarks:
 *   None
 * ****************************************************************************/
static void rb_co_CustPyrofusePostcrashSignalling(void)
{
	/* MANUAL_CODE_START (9ba0b2fa-ebf2-4432-bbb0-8fc6ac79fdcc) */

	/*Check for the BATDHV Threshold   */
	if((rb_co_CustNotifyBATDHVCrashTrigger() == TRUE) OR_ELSE (rb_co_CustSendPyroFltRec_bo==TRUE)  )
	{
		/* Check for the BATDHV Pyrofuse Solution configured or 48v switch */
		if((rb_sycf_IsSquibConfiguredALL(rb_sycf_BATDHV_e)==TRUE) OR_ELSE
				((rb_sycg_IsStaticBitSet(rb_sycg_BatteryDisconnectSwitchConfig_e) == TRUE) AND_THEN (rb_sycg_IsStaticBitSet(rb_sycg_BatteryDisconnectSwitchMonitor_e) == TRUE))/* PRQA S 3415 */
				OR_ELSE (rb_co_CustSendPyroFltRec_bo==TRUE))
		{
			/*the Variable holds BATDHV trigger status */
			rb_co_COCustBATDHVTriggerStatus_bo = TRUE ;
			/*Copying the BATDHV trigger for post crash purpose */
			rb_co_COCustBATDHVTriggerStatusCopy_bo=TRUE;
			/* Starting the timer to send the signal for 10secs */
			rb_co_CrashSignalSetTimePyro_u32 = rb_tim_GetTimestampALL();

			/*Based on the activation threshold the postcrash signalling is done  */
			rb_cdi_CustSendPyrofuseFlt(RB_co_CustComFltTrue_cu8);
			/*Reset Pyrofuse recovery variable to false */
			rb_co_CustSendPyroFltRec_bo=FALSE;

		}

		else{/*do nothing */}

	}
	else
	{
		/*Do Nothing */
	}
	/* MANUAL_CODE_END */

}
/* ***** END_API [rb_co_CustPyrofusePostcrashSignalling] ***** */

/* ****************************************************************************
 * API private: rb_co_CustCoResetToDefault
 *
 * Description:
 *   This API is used to clear all FR+LIN CO signal's, if MCB feature disabled/CO precondition not satisfied.
 * Arguments:
 *   None
 * Return:
 *   None
 * Usage guide & Scheduling:
 * To set the CO signal to default values, incase of precondition failure
 * Remarks: rb_mcb_McbClearImpactSignal() API will not set HV charge CO signal to default value,
 * Hence HvCharge signal handled separately..
 *   None
 * ****************************************************************************/
static void rb_co_CustCoResetToDefault(void)
{
	rb_co_CrashTriggers_ten SigIndex_en;
	/* Check for Clear signal requested after real crash scenario */
	if(rb_co_DiagDigitalActive_bo == FALSE)
	{
		/* Loop through all Crash Signals except HvChrg signal*/
		for (SigIndex_en = rb_co_ImpactF2_e; SigIndex_en < rb_co_CrashTriggersMAX_e; SigIndex_en++)
		{
			if(SigIndex_en != rb_co_ImpctHvChrg_e)
			{
				/*Reset the CO signal to the default value */
#ifdef RB_mcb_CustMultiCollisionBrakeMgtOption_cs
				rb_mcb_McbClearImpactSignal(SigIndex_en);   /* compiler switch to have POST CRASH ASIL-C implementation */
#endif
				/* Call post crash ASIL B Implementation API - Protected with module level compiler switch */
				rb_co_CustPostCOClearSignal(SigIndex_en);	/* MMA R9 project specific */

			}

		}
	}
}
/* ***** END_API [rb_co_CustCoResetToDefault] ***** */

/* ****************************************************************************
 * API private: rb_Co_CustCheckDCoTrigFrmSplBehBit
 *
 * Description:
 *   This API Enables/Disables Impact Side,Impact PSIS and Impact Min signals based on Dynamic behavior bit status
 * Arguments:
 *   None
 * Return:
 *   None
 * Usage guide & Scheduling:
 * Called in rb_co_CustDigitalCOInit() function
 *
 *   None
 * ****************************************************************************/
static void rb_co_CustChkCOTrigFromDynnamicBit(void)
{
	/* Enable/Disable the supported Crash Trigger  impact signals based on Dynamic behavior bit status*/
	/*Enable the Impact Side signals if the Dynamic behaviour bit is set*/
	if (rb_sycg_IsDynamicBitSet(rb_sycg_Impact1SideLRCrashOutputSignals_e) == TRUE)
	{
		rb_co_SupportedDigitalCoTriggers_u32 |= RB_co_ImpactSLSRMask_cu32;
	}
	/*Enable the Impact PSIS signals if the Dynamic behaviour bit is set*/
	if (rb_sycg_IsDynamicBitSet(rb_sycg_Impact5PSISLRCrashOutputSignals_e) == TRUE)
	{
		rb_co_SupportedDigitalCoTriggers_u32 |= RB_co_ImpactPSISLRMask_cu32;
	}
	/*Enable the Impact Min signals if the Dynamic behaviour bit is set*/
	if (rb_sycg_IsDynamicBitSet(rb_sycg_Impact5MinImpactCrashOutputSignals_e) == TRUE)
	{
		rb_co_SupportedDigitalCoTriggers_u32 |= RB_co_ImpactMinSigMask_cu32;
	}
	/* Merge with CO triggers Mask supported by Algo */
	rb_co_SupportedDigitalCoTriggers_u32 |= RB_co_CustMinSuppFCLMask_cu32;
}

/* ***** END_API [rb_co_CustCheckDCoTrigFrmSplBehBit] ***** */
/* ***** END_SECTION API functions ***** */


