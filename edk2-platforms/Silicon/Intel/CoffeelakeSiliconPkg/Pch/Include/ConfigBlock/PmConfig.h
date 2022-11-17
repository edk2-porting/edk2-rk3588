/** @file
  Power Management policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PM_CONFIG_H_
#define _PM_CONFIG_H_

#define PM_CONFIG_REVISION 5
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
  S0ixDisQAuto,
  S0ixDisQMax,
} S0IX_DISQ_PROBE_TYPE;

typedef enum {
  SlpS0OverrideDisabled  = 0x0,
  SlpS0OverrideEnabled   = 0x1,
  SlpS0OverrideAuto      = 0x2,
  SlpS0OverrideMax
} SLP_S0_OVERRIDE;

/**
  The PCH_PM_CONFIG block describes expected miscellaneous power management settings.
  The PowerResetStatusClear field would clear the Power/Reset status bits, please
  set the bits if you want PCH Init driver to clear it, if you want to check the
  status later then clear the bits.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add PsOnEnable and PowerButtonDebounce.
  <b>Revision 3</b>:
  - Add CpuC10GatePinEnable in PCH_PM_CONFIG.
  <b>Revision 4</b>:
  - Add PmcDbgMsgEn.
  - Removed PmcReadDisable in PCH_PM_CONFIG.
  <b>Revision 5</b>:
  - Add ModPhySusPgEnable
  <b>Revision 6</b>:
  - Add SlpS0WithGbeSupport
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
    be enabled. 0: Disable; <b>1: Enable</b>
  **/
  UINT32                  LpcClockRun          :  1;         /// 0: Disable; <b>1: Enable</b>
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
  /**
    Indicates whether SLP_S0# is to be asserted when PCH reaches idle state.
    When set to one SLP_S0# will be asserted in idle state.
    When set to zero SLP_S0# will not toggle and is always drivern high.
    0:Disable, <b>1:Enable</b>

    If a platform is using SLP_S0 to lower PCH voltage the below policy must be disabled.
  **/
  UINT32                  SlpS0Enable                  :  1;
  /**
    SLP_S0 Voltage Margining Runtime Control.
    PCH VCCPRIM_CORE Voltage Margining is under ACPI control. Software in runtime
    may change VCCPRIM_CORE supply voltage based on conditions like HDAudio power state
    after SLP_S0# assertion. Enable VM runtime control requires ACPI VMON method
    which will allow configuring VCCPRIM_CORE supply voltage. If this configuration is used
    ACPI VMON method needs to be provided as it is not implemented in RC.
    This setting is dependent on PMIC/VR type used on the platform.
    <b>0: Disable</b>; 1: Enable.
  **/
  UINT32                  SlpS0VmRuntimeControl        :  1;
  /**
    SLP_S0 0.70V Voltage Margining Support.
    Indicates whether SLP_S0# Voltage Margining supports setting PCH VCCPRIM_CORE down to 0.70V.
    This setting is dependent on PMIC/VR type used on the platform.
    <b>0: Disable</b>; 1: Enable.
  **/
  UINT32                  SlpS0Vm070VSupport           :  1;
  /**
    SLP_S0 0.75V Voltage Margining Support.
    Indicates whether SLP_S0# Voltage Margining supports setting PCH VCCPRIM_CORE down to 0.75V.
    This setting is dependent on PMIC/VR type used on the platform.
    <b>0: Disable</b>; 1: Enable.
  **/
  UINT32                  SlpS0Vm075VSupport           :  1;
  /**
    Decide if SLP_S0# needs to be overriden (de-asserted) when system is in debug mode. This is available since CNP-B0.
    Select 'Auto', it will be auto-configured according to probe type. Select 'Enabled' will disable SLP_S0# assertion whereas 'Disabled' will enable SLP_S0# assertion when debug is enabled.
    This policy should keep 'Auto', other options are intended for advanced configuration only.
    please refer to SLP_S0_OVERRIDE
    0: Disable; 1: Enable; <b>2:Auto</b>
  **/
  UINT32                  SlpS0Override                :  2;
  /**
    SLP_S0# disqualify for debug prode
    used to configure power management setting per debug probe to be disqualified from S0ix.
    Reminder: USB3 DbC supports S0 only. DCI OOB (aka BSSB) uses CCA probe
    Select 'Auto', it will be auto-configured according to probe type. 'No Change' will keep PMC default settings. Or select the desired debug probe type for S0ix Override settings.\nReminder: USB3 DbC supports S0 only. DCI OOB (aka BSSB) uses CCA probe.
    Note: This policy should keep 'Auto', other options are intended for advanced configuration only.
    please refer to S0IX_DISQ_PROBE_TYPE
    0: No Probe; 1: DCI OOB; 2: USB2 DbC; <b>3:Auto</b>
  **/
  UINT32                  SlpS0DisQForDebug            :  3;
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
    Enable/Disable SLP_S0 with GBE Support. This policy is ignored when GbE is not present.
    0: Disable; <b>1: Enable</b>.
    Default is 0 when paired with WHL V0 stepping CPU and 1 for all other CPUs.
  **/
  UINT32                  SlpS0WithGbeSupport          :  1;

  UINT32                  RsvdBits1                    :  5;
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
  UINT8                   Rsvd0[2];                             ///< Reserved bytes

} PCH_PM_CONFIG;

#pragma pack (pop)

#endif // _PM_CONFIG_H_
