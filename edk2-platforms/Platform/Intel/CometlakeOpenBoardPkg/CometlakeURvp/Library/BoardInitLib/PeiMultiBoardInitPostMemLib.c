/** @file
  Comet Lake U LP3 Multi-Board Initialization Post-Memory library


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
CometlakeURvpBoardInitBeforeSiliconInit(
  VOID
  );

BOARD_POST_MEM_INIT_FUNC  mCometlakeURvpBoardInitFunc = {
  CometlakeURvpBoardInitBeforeSiliconInit,
  NULL, // BoardInitAfterSiliconInit
};

EFI_STATUS
EFIAPI
PeiCometlakeURvpMultiBoardInitLibConstructor (
  VOID
  )
{
  if (LibPcdGetSku () == BoardIdCometLakeULpddr3Rvp) {
    return RegisterBoardPostMemInit (&mCometlakeURvpBoardInitFunc);
  }
  return EFI_SUCCESS;
}

