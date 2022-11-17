/** @file
  CPU Power Management VR Config Block.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_POWER_MGMT_VR_CONFIG_H_
#define _CPU_POWER_MGMT_VR_CONFIG_H_

#define CPU_POWER_MGMT_VR_CONFIG_REVISION 7

extern EFI_GUID gCpuPowerMgmtVrConfigGuid;

#pragma pack (push,1)

///
/// Defines the maximum number of VR domains supported.
/// @warning: Changing this define would cause DWORD alignment issues in policy structures.
///
#define MAX_NUM_VRS         5

/**
  CPU Power Management VR Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Updated Acoustic Noise Mitigation.
  <b>Revision 3</b>:
  - Deprecate PsysOffset and added PsysOffset1 for Psys Offset Correction
  <b>Revision 4</b>:
  - Deprecate TdcTimeWindow and added TdcTimeWindow1 for TDC Time
    Added Irms support.
  <b>Revision 5</b>:
  - Add RfiMitigation.
  <b>Revision 6</b>:
  - Added an option to Enable/Disable FIVR Spread Spectrum
  <b>Revision 7</b>:
  - Add Dynamic Periodicity Alteration (DPA) tuning feature
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT32 AcousticNoiseMitigation        : 1;      ///< Enable or Disable Acoustic Noise Mitigation feature. <b>0: Disabled</b>; 1: Enabled
  /**
   VR specific mailbox commands.
   <b>00b - no VR specific command sent.</b>
   01b - A VR mailbox command specifically for the MPS IMPV8 VR will be sent.
   10b - VR specific command sent for PS4 exit issue.
   11b - Reserved.
   **/
  UINT32 SendVrMbxCmd                   : 2;
  UINT32 EnableMinVoltageOverride       : 1;      ///< Enable or disable Minimum Voltage override for minimum voltage runtime and minimum voltage C8. <b>0: Disabled</b> 1: Enabled.
  UINT32 RfiMitigation                  : 1;      ///< Enable or Disable RFI Mitigation. <b>0: Disable - DCM is the IO_N default</b>; 1: Enable - Enable IO_N DCM/CCM switching as RFI mitigation.
  UINT32 RsvdBits                       : 27;     ///< Reserved for future use.
  UINT8  PsysSlope;                               ///< PCODE MMIO Mailbox: Platform Psys slope correction. <b>0: Auto</b> Specified in 1/100 increment values. Range is 0-200. 125 = 1.25.
  UINT8  PsysOffset;                              ///< PCODE MMIO Mailbox: Platform Psys offset correction. <b>0: Auto</b> Units 1/4, Range 0-255. Value of 100 = 100/4 = 25 offset. Deprecated
  UINT8  FivrSpreadSpectrum;                      ///< Set the Spread Spectrum Range. <b>1.5%</b>, Range: 0.5%, 1%, 1.5%, 2%, 3%, 4%, 5%, 6%. Each Range is translated to internally encoded values. 0.5% = 0, 1% = 3, 1.5% = 8, 2% = 18, 3% = 28, 4% = 34, 5% = 39, 6% = 44.
  UINT8  RsvdBytes0;
  /**
   PCODE MMIO Mailbox: Set the desired RFI frequency, in increments of 100KHz.
   <b>0: Auto</b>
   Range varies based on XTAL clock:
    - 0-1918 (Up to 191.8HMz) for 24MHz clock.
    - 0-1535 (Up to 153.5MHz) for 19MHz clock.
  **/
  UINT16 FivrRfiFrequency;
  UINT8  RsvdBytes1[2];
  /** @name VR Settings
  The VR related settings are sorted in an array where each index maps to the VR domain as defined below:
   - 0 = System Agent VR
   - 1 = IA Core VR
   - 2 = Ring Vr
   - 3 = GT VR
   - 4 = FIVR VR

  The VR settings for a given domain must be populated in the appropriate index.
  **/
  ///@{
  UINT16 TdcCurrentLimit[MAX_NUM_VRS];            ///< PCODE MMIO Mailbox: Thermal Design Current current limit. Specified in 1/8A units. Range is 0-4095. 1000 = 125A. <b>0: 0 Amps</b>
  UINT16 AcLoadline[MAX_NUM_VRS];                 ///< PCODE MMIO Mailbox: AcLoadline in 1/100 mOhms (ie. 1250 = 12.50 mOhm); Range is 0-6249. <b>Intel Recommended Defaults vary by domain and SKU.</b>
  UINT16 DcLoadline[MAX_NUM_VRS];                 ///< PCODE MMIO Mailbox: DcLoadline in 1/100 mOhms (ie. 1250 = 12.50 mOhm); Range is 0-6249.<b>Intel Recommended Defaults vary by domain and SKU.</b>
  UINT16 Psi1Threshold[MAX_NUM_VRS];              ///< PCODE MMIO Mailbox: Power State 1 current cuttof in 1/4 Amp increments. Range is 0-128A.
  UINT16 Psi2Threshold[MAX_NUM_VRS];              ///< PCODE MMIO Mailbox: Power State 2 current cuttof in 1/4 Amp increments. Range is 0-128A.
  UINT16 Psi3Threshold[MAX_NUM_VRS];              ///< PCODE MMIO Mailbox: Power State 3 current cuttof in 1/4 Amp increments. Range is 0-128A.
  INT16  ImonOffset[MAX_NUM_VRS];                 ///< PCODE MMIO Mailbox: Imon offset correction. Value is a 2's complement signed integer. Units 1/1000, Range 0-63999. For an offset = 12.580, use 12580. <b>0: Auto</b>
  UINT16 IccMax[MAX_NUM_VRS];                     ///< PCODE MMIO Mailbox: VR Icc Max limit. 0-255A in 1/4 A units. 400 = 100A. <b>Default: 0 - Auto, no override</b>
  UINT16 VrVoltageLimit[MAX_NUM_VRS];             ///< PCODE MMIO Mailbox: VR Voltage Limit. Range is 0-7999mV.
  UINT16 ImonSlope[MAX_NUM_VRS];                  ///< PCODE MMIO Mailbox: Imon slope correction. Specified in 1/100 increment values. Range is 0-200. 125 = 1.25. <b>0: Auto</b>
  UINT8  Psi3Enable[MAX_NUM_VRS];                 ///< PCODE MMIO Mailbox: Power State 3 enable/disable; 0: Disable; <b>1: Enable</b>.
  UINT8  Psi4Enable[MAX_NUM_VRS];                 ///< PCODE MMIO Mailbox: Power State 4 enable/disable; 0: Disable; <b>1: Enable</b>.
  UINT8  VrConfigEnable[MAX_NUM_VRS];             ///< Enable/Disable BIOS configuration of VR; 0: Disable; <b>1: Enable.</b>
  UINT8  TdcEnable[MAX_NUM_VRS];                  ///< PCODE MMIO Mailbox: Thermal Design Current enable/disable; <b>0: Disable; </b>1: Enable
  UINT8  TdcTimeWindow[MAX_NUM_VRS];              ///< @deprecated. PCODE MMIO Mailbox: Thermal Design Current time window. Defined in milli seconds. <b>1ms default</b>
  UINT8  TdcLock[MAX_NUM_VRS];                    ///< PCODE MMIO Mailbox: Thermal Design Current Lock; <b>0: Disable</b>; 1: Enable.
  UINT8  FastPkgCRampDisable[MAX_NUM_VRS];        ///< Disable Fast Slew Rate for Deep Package C States for VR IA,GT,SA,VLCC,FIVR domain based on Acoustic Noise Mitigation feature enabled. <b>0: False</b>; 1: True
  UINT8  SlowSlewRate[MAX_NUM_VRS];               ///< Slew Rate configuration for Deep Package C States for VR VR IA,GT,SA,VLCC,FIVR domain based on Acoustic Noise Mitigation feature enabled. <b>0: Fast/2</b>; 1: Fast/4; 2: Fast/8; 3: Fast/16
  ///@}
  UINT16 MinVoltageRuntime;                       ///< PCODE MMIO Mailbox: Minimum voltage for runtime. Valid if EnableMinVoltageOverride = 1 .Range 0 to 1999mV. <b> 0: 0mV </b>
  UINT16 MinVoltageC8;                            ///< PCODE MMIO Mailbox: Minimum voltage for C8. Valid if EnableMinVoltageOverride = 1. Range 0 to 1999mV. <b> 0: 0mV </b>
  UINT16 PsysOffset1;                             ///< PCODE MMIO Mailbox: Platform Psys offset correction. <b>0: Auto</b> Units 1/1000, Range 0-63999. For an offset of 25.348, enter 25348.
  UINT8  RsvdBytes2[2];
  UINT32 TdcTimeWindow1[MAX_NUM_VRS];             ///< PCODE MMIO Mailbox: Thermal Design Current time window. Defined in milli seconds. <b>1ms default</b>
  UINT8  Irms[MAX_NUM_VRS];                       ///< PCODE MMIO Mailbox: Current root mean square. <b>0: Disable</b>; 1: Enable.
  UINT8  FivrSpectrumEnable;                      ///< Enable or Disable FIVR Spread Spectrum 0: Disable; <b> 1: Enable.</b>
  UINT8  Rsvd1[2];
  UINT8  PreWake;                                 ///< PCODE MMIO Mailbox: Acoustic Noise Mitigation Range. This can be programmed only if AcousticNoiseMitigation is enabled.<b>Default Value = 0 micro ticks</b> Defines the max pre-wake randomization time in micro ticks. Range is 0-255.
  UINT8  RampUp;                                  ///< PCODE MMIO Mailbox: Acoustic Noise Mitigation Range. This can be programmed only if AcousticNoiseMitigation is enabled.<b>Default Value = 0 micro ticks</b> Defines the max ramp up randomization time in micro ticks. Range is 0-255.
  UINT8  RampDown;                                ///< PCODE MMIO Mailbox: Acoustic Noise Mitigation Range. This can be programmed only if AcousticNoiseMitigation is enabled.<b>Default Value = 0 micro ticks</b> Defines the max ramp down randomization time in micro ticks. Range is 0-255.
  UINT8  Rsvd2[1];
} CPU_POWER_MGMT_VR_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_MGMT_VR_CONFIG_H_
