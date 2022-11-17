/** @file
  Board pre-memory initialization.

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
GalagoPro3BoardDetect (
  VOID
  );

EFI_STATUS
EFIAPI
GalagoPro3MultiBoardDetect (
  VOID
  );

EFI_BOOT_MODE
EFIAPI
GalagoPro3BoardBootModeDetect (
  VOID
  );

EFI_STATUS
EFIAPI
GalagoPro3BoardDebugInit (
  VOID
  );

EFI_STATUS
EFIAPI
GalagoPro3BoardInitBeforeMemoryInit (
  VOID
  );

BOARD_DETECT_FUNC  mGalagoPro3BoardDetectFunc = {
  GalagoPro3MultiBoardDetect
};

BOARD_PRE_MEM_INIT_FUNC  mGalagoPro3BoardPreMemInitFunc = {
  GalagoPro3BoardDebugInit,
  GalagoPro3BoardBootModeDetect,
  GalagoPro3BoardInitBeforeMemoryInit,
  NULL, // BoardInitAfterMemoryInit
  NULL, // BoardInitBeforeTempRamExit
  NULL, // BoardInitAfterTempRamExit
};

EFI_STATUS
EFIAPI
GalagoPro3MultiBoardDetect (
  VOID
  )
{
  GalagoPro3BoardDetect ();
  if (LibPcdGetSku () == BoardIdGalagoPro3) {
    RegisterBoardPreMemInit (&mGalagoPro3BoardPreMemInitFunc);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiGalagoPro3MultiBoardInitPreMemLibConstructor (
  VOID
  )
{
  return RegisterBoardDetect (&mGalagoPro3BoardDetectFunc);
}
