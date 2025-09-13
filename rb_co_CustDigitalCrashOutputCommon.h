#ifndef RB_CO_CUSTDIGITALCRASHOUTPUTCOMMON_H
#define RB_CO_CUSTDIGITALCRASHOUTPUTCOMMON_H

/* MANUAL_CODE_START (Bosch Copyright) */
/* ************************************************************************** *
 *
 *  Copyright (c) 2023 Robert Bosch GmbH, Germany
 *  All rights reserved.
 *
 * ************************************************************************** */
/* MANUAL_CODE_END */

/*
 * Description:
 * This module notifies the crash information to the other subsystems in the vehicle by using hard wired PWM signal and FLEXRAY / LIN signals.
 * Customer Specific Crash Output.
 *
 *
 *
 * Code generator version : 4.8.15/2021-01-25
 */

/* PRQA S 1-9999 suppress_foreign_warnings_rb_co_CustDigitalCrashOutputCommon_h */

/* ***** SECTION Includes ***** */






/*** custLib Includes ***/
  #include "rb_coi_CrashOutputInterface.h"

/* MANUAL_CODE_START (Manual Includes) */
/* MANUAL_CODE_END */

/* PRQA L:suppress_foreign_warnings_rb_co_CustDigitalCrashOutputCommon_h */

#include "rb_co_CustCrashOutput_Interfaces.h"

/*** Config Headers ***/
#include "rb_co_CustCrashOutput_Cfg.h"

/* ***** END_SECTION Includes ***** */


/* ***** SECTION Defines ***** */
/*** User definitions (#defines, enums, variables, ...) ***/
/* MANUAL_CODE_START (Definitions) */
/* MANUAL_CODE_END */


/*** Second User definitions Section  (#defines, enums, variables, ...) ***/
/* MANUAL_CODE_START (UserDefinitions) */
extern boolean rb_co_AnalogOutActive_bo;
/* MANUAL_CODE_END */
/* ***** END_SECTION Defines ***** */

/* ***** SECTION Defines of all interfaces of "rb_co_CustDigitalCrashOutputCommon" ***** */
/*** modeled defines ***/

/* RB_co_DiagCrashDetfltIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagCrashDetfltIndx_cu8 (uint8)16
/* RB_co_DiagCrashDetfltIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagCrashDetfltSNAIndx_cu8 (uint8)17
/* RB_co_DiagHVPyrofltIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagHVPyrofltIndx_cu8 (uint8)18
/* RB_co_DiagHVPyrofltIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagHVPyrofltSNAIndx_cu8 (uint8)19
/* RB_co_DiagImpactF2LINIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagImpactF2LINIndx_cu8 (uint8)26
/* RB_co_DiagImpactF3LINIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagImpactF3LINIndx_cu8 	(uint8)27
/* RB_co_DiagImpactHvChrgLINIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagImpactHvChrgLINIndx_cu8 (uint8)29
/* RB_co_DiagImpactHvChrgLINI1ndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagImpactHvChrgLINIndx1_cu8 (uint8)30
/* RB_co_DiagImpactRollovrtyp1LINIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagImpactRollovrtyp1LINIndx_cu8 (uint8)28
/* RB_co_DiagImpactRollovrtyp2LINIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagImpactRollovrtyp2LINIndx_cu8 (uint8)21
/* RB_co_DiagImpactXLINSglIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagImpactXLINSglIndx_cu8 (uint8)20
/* RB_co_DiagImpactYLINIndx_cu8
 * Description missing in Model!
 */
#define RB_co_DiagImpactYLINIndx_cu8 (uint8)25
/* RB_co_CustImpactVehPosSideLeft_cu8
 * Description missing in Model!
 */
#define RB_co_CustImpactVehPosSideLeft_cu8 rb_cap_TxFrSglORCCrashDetFlt_e
/* RB_co_CustImpactVehPosSideRight_cu8
 * Description missing in Model!
 */
#define RB_co_CustImpactVehPosSideRight_cu8 rb_cap_TxFrSglTerm30cPyrofuseFlt_e
/* RB_co_DiagArraySizeMaxCount_cu8
 * Description missing in Model!
 */

#define RB_co_DiagArraySizeMaxCount_cu8 (uint8)0x0C
/* RB_co_CustComFltFalse_cu8
 * Description missing in Model!
 */
#define RB_co_CustComFltFalse_cu8 (uint8)1
/* RB_co_CustComFltTrue_cu8
 * Description missing in Model!
 */
#define RB_co_CustComFltTrue_cu8 	(uint8)2
/* RB_co_CustComNotActv_cu8
 * Description missing in Model!
 */
#define RB_co_CustComNotActv_cu8 (uint8)0
/* RB_co_CustComSNA_cu8
 * Description missing in Model!
 */
#define RB_co_CustComSNA_cu8  (uint8)3

/* Value to indicate VehPosUnknwn */
#define RB_co_CustVehPosUnknwn_cu8 (uint8)0 /* private */
/* Mask to indicate VehPosRoof */
#define RB_co_CustVehPosRoofMask_cu32 RB_utbm_GetSingleBitMaskU32(rb_co_ImpactVehPosiRoof_e) /* private */

/* Mask to indicate VehPosSLeft */
#define RB_co_CustVehPosSLeftMask_cu32 RB_utbm_GetSingleBitMaskU32(rb_co_ImpactVehPosiSideLeft_e) /* private */

/* Mask to indicate VehPosSRight */
#define RB_co_CustVehPosSRightMask_cu32 RB_utbm_GetSingleBitMaskU32(rb_co_ImpactVehPosiSideRight_e) /* private */

/* Mask to indicate VehPosSna */
#define RB_co_CustVehPosSnaMask_cu32 (RB_co_CustVehPosRoofMask_cu32 | RB_co_CustVehPosSLeftMask_cu32 | RB_co_CustVehPosSRightMask_cu32)/* private */
/*** modeled variables ***/
/*! rb_co_COCustBATDHVTriggerStatus_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_COCustBATDHVTriggerStatus_bo;
/*! rb_co_COCustBATDHVTriggerStatusCopy_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_COCustBATDHVTriggerStatusCopy_bo;
/*! rb_co_COCustTriggersRT_u32:  Purpose:
Goal:
Brief Description: */
extern uint32 rb_co_COCustTriggersRT_u32;
/*! rb_co_COMfaultPresentsts_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_COMfaultPresentsts_bo;
/*! rb_co_COMfaultRecoverysts_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_COMfaultRecoverysts_bo;
/*! rb_co_CrashOutputFltStatus_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_CrashOutputFltStatus_bo;
/*! rb_co_CrashTriggerStatus_u32:  Purpose:
Goal:
Brief Description: */
extern uint32 rb_co_CrashTriggerStatus_u32;
/*! rb_co_CrashTriggerStatusCopy_u32:  Purpose:
Goal:
Brief Description: */
extern uint32 rb_co_CrashTriggerStatusCopy_u32;
/*! ARRAY rb_co_CustFrSglList_caen[RB_co_CustCOMSglCrashTrigLen_cu8]:     Purpose:
Goal:
Brief Description: */
extern const rb_cap_TxFrSglList_ten rb_co_CustFrSglList_caen [rb_co_CrashTriggersMAX_e];
/*! rb_co_CustSendPyroFltRec_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_CustSendPyroFltRec_bo;
/*! rb_co_DiagCOActive_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_DiagCOActive_bo;
/*! rb_co_DiagCrashDetectionFlt_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_DiagCrashDetectionFlt_bo;
/*! rb_co_DiagDigitalActive_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_DiagDigitalActive_bo;
/*! rb_co_DiagPyroFlt_bo:  Purpose:
Goal:
Brief Description: */
extern boolean rb_co_DiagPyroFlt_bo;
/*! ARRAY rb_co_CrashSignalSetTime_au32[29]:     This array holds the time stamps */
extern uint32 rb_co_CrashSignalSetTime_au32 [31];
/*! rb_co_CustCrashActiveFlag_bo:  Flag to indicate sleep mode not to go to sleep in case of Crash */
extern boolean rb_co_CustCrashActiveFlag_bo;
/*! rb_co_CrashSignalSetTimePyro_u32:  This variable holds the time stamps for Pyro */
extern uint32 rb_co_CrashSignalSetTimePyro_u32;
/* ***** END_SECTION Defines of all interfaces of "rb_co_CustDigitalCrashOutputCommon" ***** */






#endif /* RB_CO_CUSTDIGITALCRASHOUTPUTCOMMON_H */
