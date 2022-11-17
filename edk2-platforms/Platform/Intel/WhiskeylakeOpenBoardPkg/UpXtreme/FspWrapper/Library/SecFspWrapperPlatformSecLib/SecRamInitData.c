/** @file
  Provide TempRamInitParams data.

Copyright (c) 2020 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PcdLib.h>
#include <FspEas.h>
#include <FsptUpd.h>

#pragma pack(1)

GLOBAL_REMOVE_IF_UNREFERENCED CONST FSPT_UPD FsptUpdDataPtr = {
  // FSP_UPD_HEADER
  {
    FSPT_UPD_SIGNATURE,
    0x00,
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00
    }
  },
  // FSPT_CORE_UPD
  {
    FixedPcdGet32 (PcdFlashFvMicrocodeBase) + FixedPcdGet32 (PcdMicrocodeOffsetInFv),
    FixedPcdGet32 (PcdFlashFvMicrocodeSize) - FixedPcdGet32 (PcdMicrocodeOffsetInFv),
    0,          // Set CodeRegionBase as 0, so that caching will be 4GB-(CodeRegionSize > LLCSize ? LLCSize : CodeRegionSize) will be used.
    FixedPcdGet32 (PcdFlashCodeCacheSize),
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
  },
  // FSP_T_CONFIG
  {
    FixedPcdGet8 (PcdSerialIoUartDebugEnable),
    FixedPcdGet8 (PcdSerialIoUartNumber),
    0,
    0,
    FixedPcdGet32 (PcdSerialClockRate),
    FixedPcdGet64 (PcdPciExpressBaseAddress),
    FixedPcdGet32 (PcdPciExpressRegionLength),
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00
    }
  },
  // UpdTerminator
  0x55AA
};
#pragma pack()

