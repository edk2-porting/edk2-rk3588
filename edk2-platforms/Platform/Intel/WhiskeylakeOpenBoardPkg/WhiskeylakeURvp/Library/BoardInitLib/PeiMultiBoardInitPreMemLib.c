/** @file
  Whiskey Lake U RVP Multi-Board Initialization Pre-Memory library


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
WhiskeylakeURvpBoardDetect (
  VOID
  );

EFI_STATUS
EFIAPI
WhiskeylakeURvpMultiBoardDetect (
  VOID
  );

EFI_BOOT_MODE
EFIAPI
WhiskeylakeURvpBoardBootModeDetect (
  VOID
  );

EFI_STATUS
EFIAPI
WhiskeylakeURvpBoardDebugInit (
  VOID
  );

EFI_STATUS
EFIAPI
WhiskeylakeURvpBoardInitBeforeMemoryInit (
  VOID
  );

BOARD_DETECT_FUNC  mWhiskeylakeURvpBoardDetectFunc = {
  WhiskeylakeURvpMultiBoardDetect
};

BOARD_PRE_MEM_INIT_FUNC  mWhiskeylakeURvpBoardPreMemInitFunc = {
  WhiskeylakeURvpBoardDebugInit,
  WhiskeylakeURvpBoardBootModeDetect,
  WhiskeylakeURvpBoardInitBeforeMemoryInit,
  NULL, // BoardInitAfterMemoryInit
  NULL, // BoardInitBeforeTempRamExit
  NULL, // BoardInitAfterTempRamExit
};

EFI_STATUS
EFIAPI
WhiskeylakeURvpMultiBoardDetect (
  VOID
  )
{
  WhiskeylakeURvpBoardDetect ();
  if (LibPcdGetSku () == BoardIdWhiskeyLakeRvp) {
    RegisterBoardPreMemInit (&mWhiskeylakeURvpBoardPreMemInitFunc);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiWhiskeylakeURvpMultiBoardInitPreMemLibConstructor (
  VOID
  )
{
  return RegisterBoardDetect (&mWhiskeylakeURvpBoardDetectFunc);
}

