/** @file
  CPU Config Block.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

#include <ConfigBlock/CpuConfigFspData.h>
#define CPU_CONFIG_REVISION 3


extern EFI_GUID gCpuConfigGuid;

#pragma pack (push,1)

/**
  CPU Configuration Structure.
  This structure is identical to CPU_CONFIG_FSP.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add SkipMpInit :  Silicon Initialization will skip MP Initialization for FSP only
  <b>Revision 3</b>:
  - Deprecated and moved FclkFrequency, VmxEnable to CPU_CONFIG_LIB_PREMEM_CONFIG
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Enable or Disable Advanced Encryption Standard (AES) feature.
    For some countries, this should be disabled for legal reasons.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 AesEnable           : 1;
  /**
  @deprecated since revision 3. Moved to CPU_CONFIG_LIB_PREMEM_CONFIG
  Processor Early Power On Configuration FCLK setting.
   - <b>0: 800 MHz (ULT/ULX)</b>.
   - <b>1: 1 GHz (DT/Halo)</b>. Not supported on ULT/ULX.
   - 2: 400 MHz.
   - 3: Reserved.
  **/
  UINT32 FClkFrequency       : 2;
  UINT32 EnableRsr           : 1;                 ///< Enable or Disable RSR feature; 0: Disable; <b>1: Enable </b>
  /**
  Policies to obtain CPU temperature.
   - <b>0: ACPI thermal management uses EC reported temperature values</b>.
   - 1: ACPI thermal management uses DTS SMM mechanism to obtain CPU temperature values.
   - 2: ACPI Thermal Management uses EC reported temperature values and DTS SMM is used to handle Out of Spec condition.
  **/
  UINT32 EnableDts           : 2;
  UINT32 SmmbaseSwSmiNumber  : 8;                 ///< Software SMI number for handler to save CPU information in SMRAM.
  /**
    @deprecated since revision 3. Moved to CPU_CONFIG_LIB_PREMEM_CONFIG
    Enable or Disable Virtual Machine Extensions (VMX) feature.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 VmxEnable           : 1;
  /**
    Enable or Disable Trusted Execution Technology (TXT) feature.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 TxtEnable           : 1;
  UINT32 SkipMpInit          : 1;                 ///< For Fsp only, Silicon Initialization will skip MP Initialization (including BSP) if enabled. For non-FSP, this should always be 0.
  UINT32 RsvdBits            : 15;                ///< Reserved for future use
  EFI_PHYSICAL_ADDRESS MicrocodePatchAddress;     ///< Pointer to microcode patch that is suitable for this processor.
} CPU_CONFIG;

/**
  FSP CPU Configuration Structure.
  This structure is identical to CPU_CONFIG.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;                     ///< Config Block Header
  UINT32 Data;                                    ///< Config Block Data
  EFI_PHYSICAL_ADDRESS MicrocodePatchAddress;     ///< Pointer to microcode patch that is suitable for this processor.
} CPU_CONFIG_FSP;

typedef union {
  CPU_CONFIG CpuConfig;
  CPU_CONFIG_FSP CpuConfigFsp;
} CPU_CONFIG_UNION;

#pragma pack (pop)

#endif // _CPU_CONFIG_H_
