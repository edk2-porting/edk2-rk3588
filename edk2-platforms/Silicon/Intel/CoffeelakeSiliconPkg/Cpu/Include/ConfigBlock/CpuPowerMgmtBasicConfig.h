/** @file
  CPU Power Management Basic Config Block.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_POWER_MGMT_BASIC_CONFIG_H_
#define _CPU_POWER_MGMT_BASIC_CONFIG_H_

#define CPU_POWER_MGMT_BASIC_CONFIG_REVISION 2

extern EFI_GUID gCpuPowerMgmtBasicConfigGuid;

#pragma pack (push,1)

/**
  CPU Power Management Basic Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added MinRingRatioLimit
  - Added MaxRingRatioLimit
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
  Sets the boot frequency starting from reset vector.
   - 0: Maximum battery performance.
   - <b>1: Maximum non-turbo performance</b>.
   - 2: Turbo performance.
  @note If Turbo is selected BIOS will start in max non-turbo mode and switch to Turbo mode.
  **/
  UINT32 BootFrequency                  : 2;
  UINT32 SkipSetBootPState              : 1;      ///< Choose whether to skip SetBootPState function for all APs; <b>0: Do not skip</b>; 1: Skip.
  /**
  Enable or Disable Intel Speed Shift Technology.
  Enabling allows for processor control of P-state transitions.
  0: Disable; <b>1: Enable;</b> Bit 1 is ignored.
  @note Currently this feature is recommended to be enabled only on win10
  **/
  UINT32 Hwp                            : 2;
  /**
  Hardware Duty Cycle Control configuration. 0: Disabled; <b>1: Enabled</b> 2-3:Reserved
  HDC enables the processor to autonomously force components to enter into an idle state to lower effective frequency.
  This allows for increased package level C6 residency.
  @note Currently this feature is recommended to be enabled only on win10
  **/
  UINT32 HdcControl                     : 2;
  UINT32 PowerLimit2                    : 1;      ///< Enable or Disable short duration Power Limit (PL2). 0: Disable; <b>1: Enable</b>
  UINT32 TurboPowerLimitLock            : 1;      ///< MSR 0x610[63] and 0x618[63]: Locks all Turbo power limit settings to read-only; <b>0: Disable</b>; 1: Enable (Lock).
  UINT32 PowerLimit3DutyCycle           : 8;      ///< Package PL3 Duty Cycle. Specifies the PL3 duty cycle percentage, Range 0-100. <b>Default: 0</b>.
  UINT32 PowerLimit3Lock                : 1;      ///< Package PL3 MSR 615h lock; <b>0: Disable</b>; 1: Enable (Lock).
  UINT32 PowerLimit4Lock                : 1;      ///< Package PL4 MSR 601h lock; <b>0: Disable</b>; 1: Enable (Lock).
  /**
  Tcc Offset Clamp for Runtime Average Temperature Limit (RATL) allows CPU to throttle below P1.
  For Y SKU, the recommended default for this policy is <b>1: Enabled</b>, which indicates throttling below P1 is allowed.
  For all other SKUs the recommended default are  <b>0: Disabled</b>.
  **/
  UINT32 TccOffsetClamp                 : 1;
  UINT32 TccOffsetLock                  : 1;      ///< Tcc Offset Lock for Runtime Average Temperature Limit (RATL) to lock temperature target MSR 1A2h; <b>0: Disabled</b>; 1: Enabled (Lock).
  UINT32 TurboMode                      : 1;      ///< Enable or Disable Turbo Mode. Disable; <b>1: Enable</b>
  UINT32 HwpInterruptControl            : 1;      ///< Set HW P-State Interrupts Enabled  for MISC_PWR_MGMT MSR 0x1AA[7]; <b>0: Disable</b>; 1: Enable.

  UINT32 RsvdBits                       : 9;      ///< Reserved for future use.

  /**
   1-Core Ratio Limit: LFM to Fused 1-Core Ratio Limit. For overclocking parts: LFM to Fused 1-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 255.
     - This 1-Core Ratio Limit Must be greater than or equal to 2-Core Ratio Limit, 3-Core Ratio Limit, 4-Core Ratio Limit.
  **/
  UINT8  OneCoreRatioLimit;
  /**
   2-Core Ratio Limit: LFM to Fused 2-Core Ratio Limit, For overclocking part: LFM to Fused 2-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 255.
     - This 2-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
  **/
  UINT8  TwoCoreRatioLimit;
  /**
   3-Core Ratio Limit: LFM to Fused 3-Core Ratio Limit, For overclocking part: LFM to Fused 3-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 255.
     - This 3-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
  **/
  UINT8  ThreeCoreRatioLimit;
  /**
   4-Core Ratio Limit: LFM to Fused 4-Core Ratio Limit, For overclocking part: LFM to Fused 4-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 255.
     - This 4-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
  **/
  UINT8  FourCoreRatioLimit;
  /**
   5-Core Ratio Limit: LFM to Fused 5-Core Ratio Limit, For overclocking part: LFM to Fused 5-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 255.
     - This 5-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
  **/
  UINT8  FiveCoreRatioLimit;
  /**
   6-Core Ratio Limit: LFM to Fused 6-Core Ratio Limit, For overclocking part: LFM to Fused 6-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 255.
     - This 6-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
  **/
  UINT8  SixCoreRatioLimit;
  /**
   7-Core Ratio Limit: LFM to Fused 7-Core Ratio Limit, For overclocking part: LFM to Fused 7-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 255.
     - This 7-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
  **/
  UINT8  SevenCoreRatioLimit;
  /**
   8-Core Ratio Limit: LFM to Fused 8-Core Ratio Limit, For overclocking part: LFM to Fused 8-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 255.
     - This 8-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
  **/
  UINT8  EightCoreRatioLimit;
  /**
  TCC Activation Offset. Offset from factory set TCC activation temperature at which the Thermal Control Circuit must be activated.
  TCC will be activated at (TCC Activation Temperature - TCC Activation Offset), in degrees Celcius.
  For Y SKU, the recommended default for this policy is  <b>15</b>
  For all other SKUs the recommended default are <b>0</b>, causing TCC to activate at TCC Activation temperature.
  @note The policy is recommended for validation purpose only.
  **/
  UINT8  TccActivationOffset;
  /**
  Intel Turbo Boost Max Technology 3.0
  Enabling it on processors with OS support will allow OS to exploit the diversity in max turbo frequency of the cores.
  0: Disable; <b>1: Enable;</b>
  **/
  UINT8  EnableItbm                     : 1;
  /**
  Intel Turbo Boost Max Technology 3.0 Driver
  Enabling it will load the driver upon ACPI device with HID = INT3510.
  <b>0: Disable;</b> 1: Enable;
  **/
  UINT8  EnableItbmDriver               : 1;
  UINT8  ReservedBits1                  : 6;      ///< Reserved for future use.
  UINT8  MinRingRatioLimit;                       ///< Minimum Ring Ratio Limit. Range from 0 to Max Turbo Ratio. 0 = AUTO/HW Default
  UINT8  MaxRingRatioLimit;                       ///< Maximum Ring Ratio Limit. Range from 0 to Max Turbo Ratio. 0 = AUTO/HW Default

  /**
  Package Long duration turbo mode power limit (PL1).
  Default is the TDP power limit of processor. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  **/
  UINT16 PowerLimit1;
  /**
  Package Short duration turbo mode power limit (PL2). Allows for short excursions above TDP power limit.
  Default = 1.25 * TDP Power Limit. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  **/
  UINT16 PowerLimit2Power;
  /**
  Package PL3 power limit. PL3 is the CPU Peak Power Occurences Limit.
  <b>Default: 0</b>. Range 0-65535. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  **/
  UINT16 PowerLimit3;
  /**
  Package PL4 power limit. PL4 is a Preemptive CPU Package Peak Power Limit, it will never be exceeded.
  Power is premptively lowered before limit is reached. <b>Default: 0</b>. Range 0-65535.
  Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  **/
  UINT16 PowerLimit4;
  /**
  Package Long duration turbo mode power limit (PL1) time window in seconds.
  Used in calculating the average power over time.
  Default: <b>0 - AUTO</b>, auto will program 28 seconds.
  Range: 0 - 128s
  **/
  UINT32 PowerLimit1Time;
  UINT32 PowerLimit3Time;                         ///< Package PL3 time window. Range from 3ms to 64ms.
  /**
  Tcc Offset Time Window can range from 5ms to 448000ms for Runtime Average Temperature Limit (RATL).
  For Y SKU, the recommended default for this policy is <b>5000: 5 seconds</b>, For all other SKUs the recommended default are <b>0: Disabled</b>
  **/
  UINT32 TccOffsetTimeWindowForRatl;
} CPU_POWER_MGMT_BASIC_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_MGMT_BASIC_CONFIG_H_
