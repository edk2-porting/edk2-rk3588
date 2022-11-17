/** @file
  Tiger Lake U RVP Multi-Board Initialization Post-Memory library


  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
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
TigerlakeURvpBoardInitBeforeSiliconInit(
  VOID
  );

BOARD_POST_MEM_INIT_FUNC  mTigerlakeURvpBoardInitFunc = {
  TigerlakeURvpBoardInitBeforeSiliconInit,
  NULL, // BoardInitAfterSiliconInit
};

EFI_STATUS
EFIAPI
PeiTigerlakeURvpMultiBoardInitLibConstructor (
  VOID
  )
{
  if (LibPcdGetSku () == SkuIdTglU) {
    DEBUG ((DEBUG_INFO, "SKU_ID: 0x%x\n", LibPcdGetSku()));
    return RegisterBoardPostMemInit (&mTigerlakeURvpBoardInitFunc);
  }
  return EFI_SUCCESS;
}
