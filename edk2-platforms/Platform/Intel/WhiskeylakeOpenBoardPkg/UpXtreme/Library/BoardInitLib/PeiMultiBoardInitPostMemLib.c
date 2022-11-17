/** @file
  Platform Hook Library instances


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
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
UpXtremeBoardInitBeforeSiliconInit (
  VOID
  );

BOARD_POST_MEM_INIT_FUNC  mUpXtremeBoardInitFunc = {
  UpXtremeBoardInitBeforeSiliconInit,
  NULL, // BoardInitAfterSiliconInit
};

EFI_STATUS
EFIAPI
PeiUpXtremeMultiBoardInitLibConstructor (
  VOID
  )
{
  if (LibPcdGetSku () == BoardIdUpXtreme) {
    return RegisterBoardPostMemInit (&mUpXtremeBoardInitFunc);
  }
  return EFI_SUCCESS;
}

