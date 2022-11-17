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
UpXtremeBoardDetect (
  VOID
  );

EFI_STATUS
EFIAPI
UpXtremeMultiBoardDetect (
  VOID
  );

EFI_BOOT_MODE
EFIAPI
UpXtremeBoardBootModeDetect (
  VOID
  );

EFI_STATUS
EFIAPI
UpXtremeBoardDebugInit (
  VOID
  );

EFI_STATUS
EFIAPI
UpXtremeBoardInitBeforeMemoryInit (
  VOID
  );

BOARD_DETECT_FUNC  mUpXtremeBoardDetectFunc = {
  UpXtremeMultiBoardDetect
};

BOARD_PRE_MEM_INIT_FUNC  mUpXtremeBoardPreMemInitFunc = {
  UpXtremeBoardDebugInit,
  UpXtremeBoardBootModeDetect,
  UpXtremeBoardInitBeforeMemoryInit,
  NULL, // BoardInitAfterMemoryInit
  NULL, // BoardInitBeforeTempRamExit
  NULL, // BoardInitAfterTempRamExit
};

EFI_STATUS
EFIAPI
UpXtremeMultiBoardDetect (
  VOID
  )
{
  UpXtremeBoardDetect ();
  if (LibPcdGetSku () == BoardIdUpXtreme) {
    RegisterBoardPreMemInit (&mUpXtremeBoardPreMemInitFunc);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiUpXtremeMultiBoardInitPreMemLibConstructor (
  VOID
  )
{
  return RegisterBoardDetect (&mUpXtremeBoardDetectFunc);
}

