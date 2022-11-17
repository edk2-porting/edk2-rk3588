/** @file
  CPU Overclocking Config Block.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _CPU_OVERCLOCKING_PREMEM_CONFIG_H_
#define _CPU_OVERCLOCKING_PREMEM_CONFIG_H_

#define CPU_OVERCLOCKING_PREMEM_CONFIG_REVISION 1

extern EFI_GUID gCpuOverclockingPreMemConfigGuid;

#pragma pack (push,1)

/**
  CPU Overclocking Premem Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
  Overclocking support. This controls whether OC mailbox transactions are sent.
  If disabled, all policies in this config block besides OcSupport and OcLock will be ignored.
  <b>0: Disable</b>;
  1: Enable.
  @note If PcdOverclockEnable is disabled, this should also be disabled.
  **/
  UINT32 OcSupport            :  1;
  UINT32 OcLock               :  1;               ///< If enabled, sets OC lock bit in MSR 0x194[20], locking the OC mailbox and other OC configuration settings.; <b>0: Disable</b>; 1: Enable (Lock).
  /**
  Core voltage mode, specifies which voltage mode the processor will be operating.
  <b>0: Adaptive Mode</b> allows the processor to interpolate a voltage curve when beyond fused P0 range;
  1: Override, sets one voltage for for the entire frequency range, Pn-P0.
  **/
  UINT32 CoreVoltageMode      :  1;
  UINT32 CorePllVoltageOffset :  6;               ///< Core PLL voltage offset. <b>0: No offset</b>. Range 0-63 in 17.5mv units.
  UINT32 Avx2RatioOffset      :  5;               ///< AVX2 Ratio Offset. <b>0: No offset</b>. Range is 0-31. Used to lower the AVX ratio to maximize possible ratio for SSE workload.
  UINT32 Avx3RatioOffset      :  5;               ///< AVX3 Ratio Offset. <b>0: No offset</b>. Range is 0-31. Used to lower the AVX3 ratio to maximize possible ratio for SSE workload.
  UINT32 BclkAdaptiveVoltage  :  1;               ///< Bclk Adaptive Voltage enable/disable. <b>0: Disabled</b>, 1: Enabled. When enabled, the CPU V/F curves are aware of BCLK frequency when calculated.
  /**
  Ring Downbin enable/disable.
  When enabled, the CPU will force the ring ratio to be lower than the core ratio.
  Disabling will allow the ring and core ratios to run at the same frequency.
  Uses OC Mailbox command 0x19.
  <b>0: Disables Ring Downbin feature</b>. 1: Enables Ring downbin feature.
  **/
  UINT32 RingDownBin          :  1;
  UINT32 RsvdBits             : 11;               ///< Reserved for future use
  /**
  Maximum core turbo ratio override allows to increase CPU core frequency beyond the fused max turbo ratio limit (P0).
  <b>0. no override/HW defaults.</b>. Range 0-83.
  **/
  UINT16 CoreMaxOcRatio;
  /**
  The core voltage override which is applied to the entire range of cpu core frequencies.
  Used when CoreVoltageMode = Override.
  <b>0. no override</b>. Range 0-2000 mV.
  **/
  UINT16 CoreVoltageOverride;
  /**
  Adaptive Turbo voltage target used to define the interpolation voltage point when the cpu is operating in turbo mode range.
  Used when CoreVoltageMode = Adaptive.
  <b>0. no override</b>. Range 0-2000mV.
  **/
  UINT16 CoreVoltageAdaptive;
  /**
  The core voltage offset applied on top of all other voltage modes. This offset is applied over the entire frequency range.
  This is a 2's complement number in mV units. <b>Default: 0</b> Range: -1000 to 1000.
  **/
  INT16  CoreVoltageOffset;
  UINT8  RingMaxOcRatio;                          ///< Maximum ring ratio override allows to increase CPU ring frequency beyond the fused max turbo ratio limit. <b>0: Hardware defaults.</b>  Range: 0-83.
  UINT8  RingMinOcRatio;                          ///< Minimum ring ratio override. <b>0: Hardware defaults.</b> Range: 0-83.
  UINT32 GtPllVoltageOffset     :  6;             ///< GT PLL voltage offset. <b>0: No offset</b>. Range 0-63 in 17.5mv units.
  UINT32 RingPllVoltageOffset   :  6;             ///< Ring PLL voltage offset. <b>0: No offset</b>. Range 0-63 in 17.5mv units.
  UINT32 SaPllVoltageOffset     :  6;             ///< System Agent PLL voltage offset. <b>0: No offset</b>. Range 0-63 in 17.5mv units.
  UINT32 McPllVoltageOffset     :  6;             ///< Memory Controller PLL voltage offset. <b>0: No offset</b>. Range 0-63 in 17.5mv units.
  UINT32 RsvdBits1              :  8;
  UINT8  Rsvd[2];                                 ///< Reserved for future use & Config block alignment
} CPU_OVERCLOCKING_PREMEM_CONFIG;

/**
  @deprecated CPU_OVERCLOCKING_CONFIG since revision 2.
  This structure is not used, use CPU_OVERCLOCKING_PREMEM_CONFIG instead

  CPU Overclocking Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.

  <b>Revision 2</b>:
  - Deprecated CPU_OVERCLOCKING_CONFIG

**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT32 OcSupport            :  1;               ///< @deprecated since revision 2. Overclocking support. This controls whether OC mailbox transactions are sent. <b>0: Disable</b>; 1: Enable.
  UINT32 OcLock               :  1;               ///< @deprecated since revision 2. If enabled, sets OC lock bit in MSR 0x194[20], locking the OC mailbox and other OC configuration settings.; <b>0: Disable</b>; 1: Enable (Lock).
  /**
  Core voltage mode, specifies which voltage mode the processor will be operating.
  <b>0: Adaptive Mode</b> allows the processor to interpolate a voltage curve when beyond fused P0 range;
  1: Override, sets one voltage for for the entire frequency range, Pn-P0.
  **/
  UINT32 CoreVoltageMode      :  1;
  UINT32 CorePllVoltageOffset :  6;               ///< @deprecated since revision 2. Core PLL voltage offset. <b>0: No offset</b>. Range 0-63 in 15mv units.
  UINT32 Avx2RatioOffset      :  5;               ///< @deprecated since revision 2. AVX2 Ratio Offset. <b>0: No offset</b>. Range is 0-31. Used to lower the AVX ratio to maximize possible ratio for SSE workload.
  UINT32 BclkAdaptiveVoltage  :  1;               ///< @deprecated since revision 2. Bclk Adaptive Voltage enable/disable. <b>0: Disabled</b>, 1: Enabled. When enabled, the CPU V/F curves are aware of BCLK frequency when calculated.
  UINT32 RsvdBits             : 17;               ///< Reserved for future use
  /**
  @deprecated since revision 2.
  Maximum core turbo ratio override allows to increase CPU core frequency beyond the fused max turbo ratio limit (P0).
  <b>0. no override/HW defaults.</b>. Range 0-83.
  **/
  UINT16 CoreMaxOcRatio;
  /**
  @deprecated since revision 2.
  The core voltage override which is applied to the entire range of cpu core frequencies.
  Used when CoreVoltageMode = Override.
  <b>0. no override</b>. Range 0-2000 mV.
  **/
  UINT16 CoreVoltageOverride;
  /**
  @deprecated since revision 2.
  Adaptive Turbo voltage target used to define the interpolation voltage point when the cpu is operating in turbo mode range.
  Used when CoreVoltageMode = Adaptive.
  <b>0. no override</b>. Range 0-2000mV.
  **/
  UINT16 CoreVoltageAdaptive;
  /**
  @deprecated since revision 2.
  The core voltage offset applied on top of all other voltage modes. This offset is applied over the entire frequency range.
  This is a 2's complement number in mV units. <b>Default: 0</b> Range: -1000 to 1000.
  **/
  INT16  CoreVoltageOffset;
  UINT8  RingMaxOcRatio;                          ///< @deprecated since revision 2. Maximum ring ratio override allows to increase CPU ring frequency beyond the fused max turbo ratio limit. <b>0: Hardware defaults.</b>  Range: 0-83.
  UINT8  RingMinOcRatio;                          ///< @deprecated since revision 2. Minimum clr turbo ratio override. <b>0: Hardware defaults.</b> Range: 0-83.
  UINT8  Rsvd[2];                                 ///< Reserved for future use & Config block alignment
} CPU_OVERCLOCKING_CONFIG;

#pragma pack (pop)

#endif // _CPU_OVERCLOCKING_PREMEM_CONFIG_H_
