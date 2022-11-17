/** @file
  Tiger Lake U RVP Multi-Board Initialization Pre-Memory library


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
TigerlakeURvpBoardDetect (
  VOID
  );

EFI_STATUS
EFIAPI
TigerlakeURvpMultiBoardDetect (
  VOID
  );

EFI_BOOT_MODE
EFIAPI
TigerlakeURvpBoardBootModeDetect (
  VOID
  );

EFI_STATUS
EFIAPI
TigerlakeURvpBoardDebugInit (
  VOID
  );

EFI_STATUS
EFIAPI
TigerlakeURvpBoardInitBeforeMemoryInit (
  VOID
  );


BOARD_DETECT_FUNC  mTigerlakeURvpBoardDetectFunc = {
  TigerlakeURvpMultiBoardDetect
};

BOARD_PRE_MEM_INIT_FUNC  mTigerlakeURvpBoardPreMemInitFunc = {
  TigerlakeURvpBoardDebugInit,
  TigerlakeURvpBoardBootModeDetect,
  TigerlakeURvpBoardInitBeforeMemoryInit,
  NULL, // BoardInitAfterMemoryInit
  NULL, // BoardInitBeforeTempRamExit
  NULL, // BoardInitAfterTempRamExit
};

EFI_STATUS
EFIAPI
TigerlakeURvpMultiBoardDetect (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, " In TglUMultiBoardDetect \n"));

  TigerlakeURvpBoardDetect ();

  if (LibPcdGetSku () == SkuIdTglU) {
    RegisterBoardPreMemInit (&mTigerlakeURvpBoardPreMemInitFunc);
  } else {
    DEBUG ((DEBUG_WARN,"Not a Valid TigerLake U Board\n"));
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiTigerlakeURvpMultiBoardInitPreMemLibConstructor (
  VOID
  )
{
  return RegisterBoardDetect (&mTigerlakeURvpBoardDetectFunc);
}
