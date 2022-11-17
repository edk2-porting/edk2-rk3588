/** @file
  Whiskey Lake U RVP Multi-Board Initialization Post-Memory library


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
WhiskeylakeURvpBoardInitBeforeSiliconInit (
  VOID
  );

BOARD_POST_MEM_INIT_FUNC  mWhiskeylakeURvpBoardInitFunc = {
  WhiskeylakeURvpBoardInitBeforeSiliconInit,
  NULL, // BoardInitAfterSiliconInit
};

EFI_STATUS
EFIAPI
PeiWhiskeylakeURvpMultiBoardInitLibConstructor (
  VOID
  )
{
  if (LibPcdGetSku () == BoardIdWhiskeyLakeRvp) {
    return RegisterBoardPostMemInit (&mWhiskeylakeURvpBoardInitFunc);
  }
  return EFI_SUCCESS;
}

