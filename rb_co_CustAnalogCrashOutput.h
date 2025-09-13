/*
 * File: rb_co_CustAnalogCrashOutput.h
 *
 */

#ifndef RB_CO_CUSTANALOGCRASHOUTPUT_H
#define RB_CO_CUSTANALOGCRASHOUTPUT_H

/* Child system includes */
/* PRQA S 1-9999 suppress_foreign_warnings_rb_co_CustAnalogCrashOutput_h */
/* Includes for objects with custom storage classes. */
#include "rb_syco_SysConfAOut_Cfg.h"
#include "rb_co_CrashOutput_Cfg.h"
#include "rb_co_CustCrashOutput_Cfg.h"
#include "rb_syco_SysConfAOut.h"
#include "rb_itm_InitialTestMgt.h"
#include "rb_coi_CrashOutputInterface.h"
#include "rb_coas_AnalogSignalGenerator.h"
#include "rb_pom_CustVoltageMonitoring.h"

/* Macros for accessing real-time model data structure */
/* PRQA L:suppress_foreign_warnings_rb_co_CustAnalogCrashOutput_h */

/* Imported (extern) states */
extern rb_co_CustENSState_ten rb_co_CustENSState_en;

extern uint32 rb_co_CODiagTriggersRT_u32;
extern boolean rb_co_DiagAnalogActive_bo;
/*
 * Exported States
 *
 * Note: Exported states are block states with an exported global
 * storage class designation.  Code generation will declare the memory for these
 * states and exports their symbols.
 *
 */
extern uint32 rb_co_COCustTriggersRT_u32;

extern void rb_co_CustAnalog2msBG(void);

/* Sequence of analog output pulses */
extern uint8 rb_co_AnalogSequence_au8[RB_co_NumberPulseWidthsACA_cu8];

/* Counters for pulses of analog output */
extern uint8 rb_co_AnalogPulseCounter_au8[RB_co_NumberPulseWidthsACA_cu8];
extern boolean rb_co_PulseWidthActive_abo[RB_co_NumberPulseWidthsACA_cu8];

#endif


