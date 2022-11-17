/** @file
  Sample to provide TempRamInitParams data.

  @copyright
  Copyright (c) 2014 - 2021, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <FspEas.h>
#include <FsptUpd.h>

GLOBAL_REMOVE_IF_UNREFERENCED CONST FSPT_UPD FsptUpdDataPtr = {
  {
    FSPT_UPD_SIGNATURE,
    0x00,
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
  },
  {
    FixedPcdGet32 (PcdFlashFvMicrocodeBase) + FixedPcdGet32 (PcdMicrocodeOffsetInFv),
    FixedPcdGet32 (PcdFlashFvMicrocodeSize) - FixedPcdGet32 (PcdMicrocodeOffsetInFv),
    FixedPcdGet32 (PcdFlashSecCacheRegionBase),
    FixedPcdGet32 (PcdFlashSecCacheRegionSize),
  }
};

