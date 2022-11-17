/** @file
  Multi-board post-memory initialization.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#include <PlatformBoardId.h>

EFI_STATUS
EFIAPI
GalagoPro3BoardInitBeforeSiliconInit (
  VOID
  );

BOARD_POST_MEM_INIT_FUNC  mGalagoPro3BoardInitFunc = {
  GalagoPro3BoardInitBeforeSiliconInit,
  NULL, // BoardInitAfterSiliconInit
};

EFI_STATUS
EFIAPI
PeiGalagoPro3MultiBoardInitLibConstructor (
  VOID
  )
{
  if (LibPcdGetSku () == BoardIdGalagoPro3) {
    return RegisterBoardPostMemInit (&mGalagoPro3BoardInitFunc);
  }
  return EFI_SUCCESS;
}
