/** @file
  Power Management policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PM_CONFIG_H_
#define _PM_CONFIG_H_

#define PM_CONFIG_REVISION 4
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
  PchSlpS360us,
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
  PchSlpSus0ms,
  PchSlpSus500ms,
  PchSlpSus1s,
  PchSlpSus4s
} PCH_SLP_SUS_MIN_ASSERT;

typedef enum {
  PchSlpA0ms,
  PchSlpA4s,
  PchSlpA98ms,
  PchSlpA2s
} PCH_SLP_A_MIN_ASSERT;

/**
  The PCH_PM_CONFIG block describes expected miscellaneous power management settings.
  The PowerResetStatusClear field would clear the Power/Reset status bits, please
  set the bits if you want PCH Init driver to clear it, if you want to check the
  status later then clear the bits.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Deprecate CapsuleResetType and the capsule update always uses warmreset cycle.
  <b>Revision 3</b>:
  - Added SlpS0VmEnable.
  <b>Revision 4</b>
  - Deprecate PciePllSsc and moved to PCH_HSIO_PCIE_PREMEM_CONFIG
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
  UINT32                  LpcClockRun          :  1;
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
    <b>(Test)</b>
    When set to true, this bit disallows host reads to PMC XRAM.
    BIOS must set this bit (to disable and lock the feature) prior to passing control to OS
    0:Disable, <b>1:Enable</b>
  **/
  UINT32                  PmcReadDisable               :  1;
  /**
   @deprecated This determines the type of reset issued during the capsule update process by UpdateCapsule().
   Always Warm reset.
  **/
  UINT32                  CapsuleResetType             :  1;
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

    @warning: In SKL PCH VCCPRIM_CORE must NOT be reduced based on SLP_S0# being asserted.
    If a platform is using SLP_S0 to lower PCH voltage the below policy must be disabled.
  **/
  UINT32                  SlpS0Enable                  :  1;
  UINT32                  MeWakeSts                    :  1;     ///< Clear the ME_WAKE_STS bit in the Power and Reset Status (PRSTS) register. 0: Disable; <b>1: Enable</b>.
  UINT32                  WolOvrWkSts                  :  1;     ///< Clear the WOL_OVR_WK_STS bit in the Power and Reset Status (PRSTS) register. 0: Disable; <b>1: Enable</b>.
  /**
    Set true to enable TCO timer.
    When FALSE, it disables PCH ACPI timer, and stops TCO timer.
    @note: This will have significant power impact when it's enabled.
    If TCO timer is disabled, uCode ACPI timer emulation must be enabled,
    and WDAT table must not be exposed to the OS.
    <b>0: Disable</b>, 1: Enable
  **/
  UINT32                  EnableTcoTimer               : 1;
  /**
    Indicates platform has support for VCCPrim_Core Voltage Margining in SLP_S0# asserted state.
    0: Disable, <b>1: Enable</b>
  **/
  UINT32                  SlpS0VmEnable                : 1;
  UINT32                  RsvdBits1                    : 16;
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
    @deprecated since revision 4
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
