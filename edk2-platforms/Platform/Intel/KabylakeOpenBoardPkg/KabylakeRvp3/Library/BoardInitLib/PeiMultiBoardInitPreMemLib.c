/** @file
  Kaby Lake RVP 3 Multi-Board Initialization Pre-Memory library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
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
KabylakeRvp3BoardDetect (
  VOID
  );

EFI_STATUS
EFIAPI
KabylakeRvp3MultiBoardDetect (
  VOID
  );

EFI_BOOT_MODE
EFIAPI
KabylakeRvp3BoardBootModeDetect (
  VOID
  );

EFI_STATUS
EFIAPI
KabylakeRvp3BoardDebugInit (
  VOID
  );

EFI_STATUS
EFIAPI
KabylakeRvp3BoardInitBeforeMemoryInit (
  VOID
  );

BOARD_DETECT_FUNC  mKabylakeRvp3BoardDetectFunc = {
  KabylakeRvp3MultiBoardDetect
};

BOARD_PRE_MEM_INIT_FUNC  mKabylakeRvp3BoardPreMemInitFunc = {
  KabylakeRvp3BoardDebugInit,
  KabylakeRvp3BoardBootModeDetect,
  KabylakeRvp3BoardInitBeforeMemoryInit,
  NULL, // BoardInitAfterMemoryInit
  NULL, // BoardInitBeforeTempRamExit
  NULL, // BoardInitAfterTempRamExit
};

EFI_STATUS
EFIAPI
KabylakeRvp3MultiBoardDetect (
  VOID
  )
{
  KabylakeRvp3BoardDetect ();
  if ((LibPcdGetSku () == BoardIdKabyLakeYLpddr3Rvp3) || (LibPcdGetSku () == BoardIdSkylakeRvp3)) {
    RegisterBoardPreMemInit (&mKabylakeRvp3BoardPreMemInitFunc);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiKabylakeRvp3MultiBoardInitPreMemLibConstructor (
  VOID
  )
{
  return RegisterBoardDetect (&mKabylakeRvp3BoardDetectFunc);
}