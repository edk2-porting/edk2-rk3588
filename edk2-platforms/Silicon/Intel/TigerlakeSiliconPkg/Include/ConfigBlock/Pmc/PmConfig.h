/** @file
  Power Management policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PM_CONFIG_H_
#define _PM_CONFIG_H_

#include <ConfigBlock.h>

#define PM_CONFIG_REVISION 2
extern EFI_GUID gPmConfigGuid;

#pragma pack (push,1)

/**
  This structure allows to customize PCH wake up capability from S5 or DeepSx by WOL, LAN, PCIE wake events.
**/
typedef struct {
  /**
    Corresponds to the PME_B0_S5_DIS bit in the General PM Configuration B (GEN_PMCON_B) register.
    When set to 1, this bit blocks wake events from PME_B0_STS in S5, regardless of the state of PME_B0_EN.
    When cleared (default), wake events from PME_B0_STS are allowed in S5 if PME_B0_EN = 1. <b>0: Disable</b>; 1: Enable.
  **/
  UINT32  PmeB0S5Dis         :  1;
  UINT32  WolEnableOverride  :  1;      ///< Corresponds to the "WOL Enable Override" bit in the General PM Configuration B (GEN_PMCON_B) register. 0: Disable; <b>1: Enable</b>.
  UINT32  PcieWakeFromDeepSx :  1;      ///< Determine if enable PCIe to wake from deep Sx. <b>0: Disable</b>; 1: Enable.
  UINT32  WoWlanEnable       :  1;      ///< Determine if WLAN wake from Sx, corresponds to the "HOST_WLAN_PP_EN" bit in the PWRM_CFG3 register. <b>0: Disable</b>; 1: Enable.
  UINT32  WoWlanDeepSxEnable :  1;      ///< Determine if WLAN wake from DeepSx, corresponds to the "DSX_WLAN_PP_EN" bit in the PWRM_CFG3 register. <b>0: Disable</b>; 1: Enable.
  UINT32  LanWakeFromDeepSx  :  1;      ///< Determine if enable LAN to wake from deep Sx. 0: Disable; <b>1: Enable</b>.
  UINT32  RsvdBits0          : 26;
} PCH_WAKE_CONFIG;

typedef enum {
  PchDeepSxPolDisable,
  PchDpS5BatteryEn,
  PchDpS5AlwaysEn,
  PchDpS4S5BatteryEn,
  PchDpS4S5AlwaysEn,
  PchDpS3S4S5BatteryEn,
  PchDpS3S4S5AlwaysEn
} PCH_DEEP_SX_CONFIG;

typedef enum {
  PchSlpS360us = 1,
  PchSlpS31ms,
  PchSlpS350ms,
  PchSlpS32s
} PCH_SLP_S3_MIN_ASSERT;

typedef enum {
  PchSlpS4PchTime,     ///< The time defined in PCH EDS Power Sequencing and Reset Signal Timings table
  PchSlpS41s,
  PchSlpS42s,
  PchSlpS43s,
  PchSlpS44s
} PCH_SLP_S4_MIN_ASSERT;

typedef enum {
  PchSlpSus0ms = 1,
  PchSlpSus500ms,
  PchSlpSus1s,
  PchSlpSus4s,
} PCH_SLP_SUS_MIN_ASSERT;

typedef enum {
  PchSlpA0ms = 1,
  PchSlpA4s,
  PchSlpA98ms,
  PchSlpA2s,
} PCH_SLP_A_MIN_ASSERT;

typedef enum {
  S0ixDisQNoChange,
  S0ixDisQDciOob,
  S0ixDisQUsb2Dbc,
  S0ixDisQMax,
} S0IX_DISQ_PROBE_TYPE;

/**
  Low Power Mode Enable config.
  Used to configure if respective S0i2/3 sub-states are to be supported
  by the platform. Each bit corresponds to one LPM state - LPMx->BITx.
  Some sub-states will require external FETs controlled by EXT_PWR_GATE#/EXT_PWR_GATE2# pins
  to gate v1p05-PHY or v1p05-IS supplies
**/
typedef union {
  struct {
    UINT32  S0i2p0En     : 1;        ///< LPM0 - S0i2.0 Enable
    UINT32  S0i2p1En     : 1;        ///< LPM1 - S0i2.1 Enable
    /**
      LPM2 - S0i2.2 Enable.
      Requires EXT_PWR_GATE# controlled FET to gate v1p05 PHY.
      Refer to V1p05PhyExtFetControlEn.
    **/
    UINT32  S0i2p2En     : 1;
    UINT32  S0i3p0En     : 1;        ///< LPM3 - S0i3.0 Enable
    UINT32  S0i3p1En     : 1;        ///< LPM4 - S0i3.1 Enable
    UINT32  S0i3p2En     : 1;        ///< LPM5 - S0i3.2 Enable
    /**
      LPM5 - S0i3.3 Enable.
      Requires EXT_PWR_GATE# controlled FET to gate v1p05 PHY.
      Refer to V1p05PhyExtFetControlEn.
    **/
    UINT32  S0i3p3En     : 1;
    /**
      LPM7 - S0i3.4 Enable.
      Requires EXT_PWR_GATE2# controlled FET to gate v1p05-SRAM/ISCLK.
      Refer to V1p05IsExtFetControlEn.
    **/
    UINT32  S0i3p4En     : 1;
    UINT32  Reserved     : 24;       ///< Reserved
  } Field;
  UINT32  Val;
} PMC_LPM_S0IX_SUB_STATE_EN;

/**
  Description of Global Reset Trigger/Event Mask register
**/
typedef union {
  struct {
    UINT32 Reserved1     : 1;
    UINT32 Pbo           : 1;
    UINT32 PmcUncErr     : 1;
    UINT32 PchThrm       : 1;
    UINT32 MePbo         : 1;
    UINT32 CpuThrm       : 1;
    UINT32 Megbl         : 1;
    UINT32 LtReset       : 1;
    UINT32 PmcWdt        : 1;
    UINT32 MeWdt         : 1;
    UINT32 PmcFw         : 1;
    UINT32 PchpwrFlr     : 1;
    UINT32 SyspwrFlr     : 1;
    UINT32 Reserved2     : 1;
    UINT32 MiaUxsErr     : 1;
    UINT32 MiaUxErr      : 1;
    UINT32 CpuThrmWdt    : 1;
    UINT32 MeUncErr      : 1;
    UINT32 AdrGpio       : 1;
    UINT32 OcwdtNoicc    : 1;
    UINT32 OcwdtIcc      : 1;
    UINT32 CseHecUncErr  : 1;
    UINT32 PmcSramUncErr : 1;
    UINT32 PmcIromParity : 1;
    UINT32 PmcRfFusaErr  : 1;
    UINT32 Reserved3     : 4;
    UINT32 PpbrParityErr : 1;
    UINT32 Reserved4     : 2;
  } Field;
  UINT32 Value;
} PMC_GLOBAL_RESET_MASK;

/**
  The PCH_PM_CONFIG block describes expected miscellaneous power management settings.
  The PowerResetStatusClear field would clear the Power/Reset status bits, please
  set the bits if you want PCH Init driver to clear it, if you want to check the
  status later then clear the bits.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>
  - Added C10DynamicThresholdAdjustment
**/
typedef struct {
  CONFIG_BLOCK_HEADER     Header;                           ///< Config Block Header

  PCH_WAKE_CONFIG         WakeConfig;                       ///< Specify Wake Policy
  UINT32                  PchDeepSxPol       :  4;          ///< Deep Sx Policy. Refer to PCH_DEEP_SX_CONFIG for each value. Default is <b>PchDeepSxPolDisable</b>.
  UINT32                  PchSlpS3MinAssert  :  4;          ///< SLP_S3 Minimum Assertion Width Policy. Refer to PCH_SLP_S3_MIN_ASSERT for each value. Default is <b>PchSlpS350ms</b>.
  UINT32                  PchSlpS4MinAssert  :  4;          ///< SLP_S4 Minimum Assertion Width Policy. Refer to PCH_SLP_S4_MIN_ASSERT for each value. Default is <b>PchSlpS44s</b>.
  UINT32                  PchSlpSusMinAssert :  4;          ///< SLP_SUS Minimum Assertion Width Policy. Refer to PCH_SLP_SUS_MIN_ASSERT for each value. Default is <b>PchSlpSus4s</b>.
  UINT32                  PchSlpAMinAssert   :  4;          ///< SLP_A Minimum Assertion Width Policy. Refer to PCH_SLP_A_MIN_ASSERT for each value. Default is <b>PchSlpA2s</b>.
  UINT32                  RsvdBits0          : 12;
  /**
    This member describes whether or not the LPC ClockRun feature of PCH should
    be enabled. <b>0: Disable</b>; 1: Enable
  **/
  UINT32                  SlpStrchSusUp        :  1;        ///< <b>0: Disable</b>; 1: Enable SLP_X Stretching After SUS Well Power Up
  /**
    Enable/Disable SLP_LAN# Low on DC Power. 0: Disable; <b>1: Enable</b>.
    Configure On DC PHY Power Diable according to policy SlpLanLowDc.
    When this is enabled, SLP_LAN# will be driven low when ACPRESENT is low.
    This indicates that LAN PHY should be powered off on battery mode.
    This will override the DC_PP_DIS setting by WolEnableOverride.
  **/
  UINT32                  SlpLanLowDc          :  1;
  /**
    PCH power button override period.
    000b-4s, 001b-6s, 010b-8s, 011b-10s, 100b-12s, 101b-14s
    <b>Default is 0: 4s</b>
  **/
  UINT32                  PwrBtnOverridePeriod :  3;
  /**
    <b>(Test)</b>
    Disable/Enable PCH to CPU enery report feature. <b>0: Disable</b>; 1: Enable.
    Enery Report is must have feature. Wihtout Energy Report, the performance report
    by workloads/benchmarks will be unrealistic because PCH's energy is not being accounted
    in power/performance management algorithm.
    If for some reason PCH energy report is too high, which forces CPU to try to reduce
    its power by throttling, then it could try to disable Energy Report to do first debug.
    This might be due to energy scaling factors are not correct or the LPM settings are not
    kicking in.
  **/
  UINT32                  DisableEnergyReport  :  1;
  /**
    When set to Disable, PCH will internal pull down AC_PRESENT in deep SX and during G3 exit.
    When set to Enable, PCH will not pull down AC_PRESENT.
    This setting is ignored when DeepSx is not supported.
    Default is <b>0:Disable</b>
  **/
  UINT32                  DisableDsxAcPresentPulldown  :  1;
  /**
    Power button native mode disable.
    While FALSE, the PMC's power button logic will act upon the input value from the GPIO unit, as normal.
    While TRUE, this will result in the PMC logic constantly seeing the power button as de-asserted.
    <b>Default is FALSE.</b>
  **/
  UINT32                  DisableNativePowerButton     :  1;
  UINT32                  MeWakeSts                    :  1;     ///< Clear the ME_WAKE_STS bit in the Power and Reset Status (PRSTS) register. 0: Disable; <b>1: Enable</b>.
  UINT32                  WolOvrWkSts                  :  1;     ///< Clear the WOL_OVR_WK_STS bit in the Power and Reset Status (PRSTS) register. 0: Disable; <b>1: Enable</b>.
  /*
    Set true to enable TCO timer.
    When FALSE, it disables PCH ACPI timer, and stops TCO timer.
    @note: This will have significant power impact when it's enabled.
    If TCO timer is disabled, uCode ACPI timer emulation must be enabled,
    and WDAT table must not be exposed to the OS.
    <b>0: Disable</b>, 1: Enable
  */
  UINT32                  EnableTcoTimer               : 1;
  /*
    When VRAlert# feature pin is enabled and its state is '0',
    the PMC requests throttling to a T3 Tstate to the PCH throttling unit.
    <b>0: Disable</b>; 1: Enable.
  */
  UINT32                  VrAlert                      : 1;
  /**
    Decide if PS_ON is to be enabled. This is available on desktop only.
    PS_ON is a new C10 state from the CPU on desktop SKUs that enables a
    lower power target that will be required by the California Energy
    Commission (CEC). When FALSE, PS_ON is to be disabled.}
    <b>0: Disable</b>; 1: Enable.
  **/
  UINT32                  PsOnEnable                   :  1;
  /**
    Enable/Disable platform support for CPU_C10_GATE# pin to control gating
    of CPU VccIO and VccSTG rails instead of SLP_S0# pin. This policy needs
    to be set if board design includes support for CPU_C10_GATE# pin.
    0: Disable; <b>1: Enable</b>
  **/
  UINT32                  CpuC10GatePinEnable          :  1;
  /**
    Control whether to enable PMC debug messages to Trace Hub.
    When Enabled, PMC HW will send debug messages to trace hub;
    When Disabled, PMC HW will never send debug meesages to trace hub.
    @note: When enabled, system may not enter S0ix
    <b>0: Disable</b>; 1: Enable.
  **/
  UINT32                  PmcDbgMsgEn                  :  1;
  /**
    Enable/Disable ModPHY SUS Power Domain Dynamic Gating.
    EXT_PWR_GATE# signal (if supported on platform) can be used to
    control external FET for power gating ModPHY
    @note: This setting is not supported and ignored on PCH-H
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32                  ModPhySusPgEnable            :  1;
  /**
    <b>(Test)</b>
    This policy option enables USB2 PHY SUS Well Power Gating functionality.
    @note: This setting is not supported and ignored on PCH-H
    0: disable USB2 PHY SUS Well Power Gating
    <b>1: enable USB2 PHY SUS Well Power Gating</b>
  **/
  UINT32                  Usb2PhySusPgEnable           :  1;
  /**
    Enable Os Idle Mode.
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32                  OsIdleEnable                 :  1;
  /**
    Enable control using EXT_PWR_GATE# pin of external FET
    to power gate v1p05-PHY
    <b>0: Disable</b>; 1: Enable.
  **/
  UINT32                  V1p05PhyExtFetControlEn      :  1;
  /**
    Enable control using EXT_PWR_GATE2# pin of external FET
    to power gate v1p05-IS supply
    <b>0: Disable</b>; 1: Enable.
  **/
  UINT32                  V1p05IsExtFetControlEn       :  1;
  /**
    Enable/Disable the Low Power Mode Host S0ix Auto-Demotion
    feature. This feature enables the PMC to autonomously manage
    the deepest allowed S0ix substate to combat thrashing between
    power management states.
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32                  S0ixAutoDemotion             :  1;
  /**
    Enable/Disable Latch Events C10 Exit. When this bit is set to 1,
    SLP_S0# entry events in SLP_S0_DEBUG_REGx registers are captured
    on C10 exit (instead of C10 entry which is default)
    <b>0: Disable</b>; 1: Enable.
  **/
  UINT32                  LatchEventsC10Exit           :  1;
  UINT32                  RsvdBits1                    :  10;
  /*
    Power button debounce configuration
    Debounce time can be specified in microseconds. Only certain values according
    to below formula are supported:
     DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(glitch filter clock period).
    RTC clock with f = 32 KHz is used for glitch filter.
     DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(31.25 us).
    Supported DebounceTime values are following:
     DebounceTime = 0 -> Debounce feature disabled
     DebounceTime > 0 && < 250us -> Not supported
     DebounceTime = 250us - 1024000us -> Supported range (DebounceTime = 250us * 2^n)
    For values not supported by HW, they will be rounded down to closest supported one
    <b>Default is 0</b>
  */
  UINT32                  PowerButtonDebounce;
  /**
    Reset Power Cycle Duration could be customized in the unit of second. Please refer to EDS
    for all support settings. PCH HW default is 4 seconds, and range is 1~4 seconds, where
    <b>0 is default</b>, 1 is 1 second, 2 is 2 seconds, ... 4 is 4 seconds.
    And make sure the setting correct, which never less than the following register.
    - GEN_PMCON_B.SLP_S3_MIN_ASST_WDTH
    - GEN_PMCON_B.SLP_S4_MIN_ASST_WDTH
    - PWRM_CFG.SLP_A_MIN_ASST_WDTH
    - PWRM_CFG.SLP_LAN_MIN_ASST_WDTH
  **/
  UINT8                   PchPwrCycDur;
  /**
    Specifies the Pcie Pll Spread Spectrum Percentage
    The value of this policy is in 1/10th percent units.
    Valid spread range is 0-20. A value of 0xFF is reserved for AUTO.
    A value of 0 is SSC of 0.0%. A value of 20 is SSC of 2.0%
    The default is <b>0xFF: AUTO - No BIOS override</b>.
  **/
  UINT8                   PciePllSsc;
  /**
    Tells BIOS to enable C10 dynamic threshold adjustment mode.
    BIOS will only attemt to enable it on PCH SKUs which support it.
  **/
  UINT8                   C10DynamicThresholdAdjustment;
  UINT8                   Rsvd0[1];                             ///< Reserved bytes
  /**
    <b>(Test)</b>
    Low Power Mode Enable/Disable config.
    Configure if respective S0i2/3 sub-states are to be supported
    by the platform. By default all sub-states are enabled but
    for test purpose respective states can be disabled.
    <b>Default is 0xFF</b>
  **/
  PMC_LPM_S0IX_SUB_STATE_EN      LpmS0ixSubStateEnable;
  /*
    Set true to enable Timed GPIO 0 timer.
    <b>0: Disable</b>, 1: Enable
  */
  UINT32                  EnableTimedGpio0             : 1;
  /*
    Set true to enable Timed GPIO 1 timer.
    <b>0: Disable</b>, 1: Enable
  */
  UINT32                  EnableTimedGpio1             : 1;
  UINT32                  Rsvdbits                     : 30;

  /**
    Set true to enable override of Global Reset Event/Trigger masks.
    Values from GlobalResetTriggerMask and GlobalResetEventMask will
    be used as override value.
    <b>0: Disable</b>, 1: Enable
  **/
  UINT8                   GlobalResetMasksOverride;
  UINT8                   Rsvd1[3];             ///< Reserved bytes
  /*
    Mask for enabling Global Reset Trigger prevention
  */
  PMC_GLOBAL_RESET_MASK   GlobalResetTriggerMask;
  /*
    Mask for enabling Global Reset Event prevention
  */
  PMC_GLOBAL_RESET_MASK   GlobalResetEventMask;
} PCH_PM_CONFIG;

#pragma pack (pop)

#endif // _PM_CONFIG_H_
