/** @file
  CPU Config Block.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

#define CPU_CONFIG_REVISION 3

extern EFI_GUID gCpuConfigGuid;

#pragma pack (push,1)

/**
  CPU Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add SmbiosType4MaxSpeedOverride.
  <b>Revision 3</b>:
  - Add AvxDisable & Avx3Disable.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT32                MicrocodePatchRegionSize;
  EFI_PHYSICAL_ADDRESS  MicrocodePatchAddress;    ///< Pointer to microcode patch that is suitable for this processor.
  /**
    Enable or Disable Advanced Encryption Standard (AES) feature.
    For some countries, this should be disabled for legal reasons.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 AesEnable           : 1;
  /**
    Enable or Disable Trusted Execution Technology (TXT) feature.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 TxtEnable           : 1;
  UINT32 SkipMpInit          : 1;                 ///< For Fsp only, Silicon Initialization will skip MP Initialization (including BSP) if enabled. For non-FSP, this should always be 0.
  /**
    Enable or Disable or Auto for PPIN Support to view Protected Processor Inventory Number.
    - <b>0: Disable</b>
    -    1: Enable
    -    2: Auto : Feature is based on End Of Manufacturing (EOM) flag. If EOM is set, it is disabled.
  **/
  UINT32 PpinSupport         : 2;
  /**
    Enable or Disable #AC machine check on split lock.
    - <b>0: Disable</b>
    -    1: Enable
  **/
  UINT32 AcSplitLock         : 1;
  /**
  Enable or Disable Avx.
  -      1: Disable
  -  <b> 0: Enable</b>
  **/
  UINT32 AvxDisable          : 1;
  /**
  Enable or Disable Avx3.
  -      1: Disable
  -  <b> 0: Enable</b>
  **/
  UINT32 Avx3Disable         : 1;
  UINT32 RsvdBits            : 24;                ///< Reserved for future use
  /**
    Provide the option for platform to override the MaxSpeed field of Smbios Type 4.
    Value 4000 means 4000MHz.
    If this value is not zero, it dominates the field.
    If this value is zero, CPU RC will update the field according to the max radio.
    <b>default is 0.</b>
  **/
  UINT16 SmbiosType4MaxSpeedOverride;
  UINT8  Reserved0[2];                            ///< Reserved for future use
} CPU_CONFIG;

#pragma pack (pop)

#endif // _CPU_CONFIG_H_
