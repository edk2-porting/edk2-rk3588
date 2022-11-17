/** @file
  Comet Lake U LP3 Multi-Board Initialization Pre-Memory library


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
CometlakeURvpBoardDetect(
  VOID
  );

EFI_STATUS
EFIAPI
CometlakeURvpMultiBoardDetect (
  VOID
  );

EFI_BOOT_MODE
EFIAPI
CometlakeURvpBoardBootModeDetect(
  VOID
  );

EFI_STATUS
EFIAPI
CometlakeURvpBoardDebugInit(
  VOID
  );

EFI_STATUS
EFIAPI
CometlakeURvpBoardInitBeforeMemoryInit(
  VOID
  );

BOARD_DETECT_FUNC  mCometlakeURvpBoardDetectFunc = {
  CometlakeURvpMultiBoardDetect
};

BOARD_PRE_MEM_INIT_FUNC  mCometlakeURvpBoardPreMemInitFunc = {
  CometlakeURvpBoardDebugInit,
  CometlakeURvpBoardBootModeDetect,
  CometlakeURvpBoardInitBeforeMemoryInit,
  NULL, // BoardInitAfterMemoryInit
  NULL, // BoardInitBeforeTempRamExit
  NULL, // BoardInitAfterTempRamExit
};

EFI_STATUS
EFIAPI
CometlakeURvpMultiBoardDetect(
  VOID
  )
{
  CometlakeURvpBoardDetect();
  if (LibPcdGetSku () == BoardIdCometLakeULpddr3Rvp) {
    RegisterBoardPreMemInit (&mCometlakeURvpBoardPreMemInitFunc);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiCometlakeURvpMultiBoardInitPreMemLibConstructor (
  VOID
  )
{
  return RegisterBoardDetect (&mCometlakeURvpBoardDetectFunc);
}

