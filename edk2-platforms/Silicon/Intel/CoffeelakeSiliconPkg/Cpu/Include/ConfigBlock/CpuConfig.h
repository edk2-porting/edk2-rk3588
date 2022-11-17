/** @file
  CPU Config Block.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

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
  - Deprecate and move SkipMpInit to CpuConfigLibPreMemConfig.
  <b>Revision 3</b>:
  - Move DebugInterfaceEnable from CPU_TEST_CONFIG.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Enable or Disable Advanced Encryption Standard (AES) feature.
    For some countries, this should be disabled for legal reasons.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 AesEnable                : 1;
  UINT32 SkipMpInit               : 1;            ///< @deprecated since revision 2. For Fsp only, Silicon Initialization will skip MP Initialization (including BSP) if enabled. For non-FSP, this should always be 0.
  UINT32 DebugInterfaceEnable     : 1;            ///< Enable or Disable processor debug features; <b>0: Disable</b>; 1: Enable.
  UINT32 RsvdBits                 : 28;           ///< Reserved for future use
  EFI_PHYSICAL_ADDRESS MicrocodePatchAddress;     ///< Pointer to microcode patch that is suitable for this processor.
} CPU_CONFIG;

#pragma pack (pop)

#endif // _CPU_CONFIG_H_
