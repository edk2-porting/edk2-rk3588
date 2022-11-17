/** @file

Copyright (c) 2017 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>

/**
  This board service detects the board type.

  @retval EFI_SUCCESS   The board was detected successfully.
  @retval EFI_NOT_FOUND The board could not be detected.
**/
EFI_STATUS
EFIAPI
BoardDetect (
  VOID
  )
{
  BOARD_DETECT_FUNC          *BoardDetectFunc;
  UINTN                      Index;
  EFI_STATUS                 Status;

  for (Index = 0; ; Index++) {
    Status = PeiServicesLocatePpi(
               &gBoardDetectGuid,
               Index,
               NULL,
               (VOID **)&BoardDetectFunc
               );
    if (EFI_ERROR(Status)) {
      break;
    }
    if (BoardDetectFunc->BoardDetect != NULL) {
      BoardDetectFunc->BoardDetect ();
    }
  }
  return EFI_SUCCESS;
}

/**
  This board service initializes board-specific debug devices.

  @retval EFI_SUCCESS   Board-specific debug initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardDebugInit (
  VOID
  )
{
  BOARD_PRE_MEM_INIT_FUNC    *BoardPreMemInit;
  EFI_STATUS                 Status;

  Status = PeiServicesLocatePpi(
             &gBoardPreMemInitGuid,
             0,
             NULL,
             (VOID **)&BoardPreMemInit
             );
  if (!EFI_ERROR(Status)) {
    if (BoardPreMemInit->BoardDebugInit != NULL) {
      return BoardPreMemInit->BoardDebugInit ();
    }
  }
  return EFI_SUCCESS;
}

/**
  This board service detects the boot mode.

  @retval EFI_BOOT_MODE The boot mode.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_BOOT_MODE
EFIAPI
BoardBootModeDetect (
  VOID
  )
{
  BOARD_PRE_MEM_INIT_FUNC    *BoardPreMemInit;
  EFI_STATUS                 Status;

  Status = PeiServicesLocatePpi(
             &gBoardPreMemInitGuid,
             0,
             NULL,
             (VOID **)&BoardPreMemInit
             );
  if (!EFI_ERROR(Status)) {
    if (BoardPreMemInit->BoardBootModeDetect != NULL) {
      return BoardPreMemInit->BoardBootModeDetect ();
    }
  }
  return BOOT_WITH_FULL_CONFIGURATION;
}

/**
  A hook for board-specific initialization prior to memory initialization.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitBeforeMemoryInit (
  VOID
  )
{
  BOARD_PRE_MEM_INIT_FUNC    *BoardPreMemInit;
  EFI_STATUS                 Status;

  Status = PeiServicesLocatePpi(
             &gBoardPreMemInitGuid,
             0,
             NULL,
             (VOID **)&BoardPreMemInit
             );
  if (!EFI_ERROR(Status)) {
    if (BoardPreMemInit->BoardInitBeforeMemoryInit != NULL) {
      return BoardPreMemInit->BoardInitBeforeMemoryInit ();
    }
  }
  return EFI_SUCCESS;
}

/**
  A hook for board-specific initialization after memory initialization.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitAfterMemoryInit (
  VOID
  )
{
  BOARD_PRE_MEM_INIT_FUNC    *BoardPreMemInit;
  EFI_STATUS                 Status;

  Status = PeiServicesLocatePpi(
             &gBoardPreMemInitGuid,
             0,
             NULL,
             (VOID **)&BoardPreMemInit
             );
  if (!EFI_ERROR(Status)) {
    if (BoardPreMemInit->BoardInitAfterMemoryInit != NULL) {
      return BoardPreMemInit->BoardInitAfterMemoryInit ();
    }
  }
  return EFI_SUCCESS;
}

/**
  A hook for board-specific initialization prior to disabling temporary RAM.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitBeforeTempRamExit (
  VOID
  )
{
  BOARD_PRE_MEM_INIT_FUNC    *BoardPreMemInit;
  EFI_STATUS                 Status;

  Status = PeiServicesLocatePpi(
             &gBoardPreMemInitGuid,
             0,
             NULL,
             (VOID **)&BoardPreMemInit
             );
  if (!EFI_ERROR(Status)) {
    if (BoardPreMemInit->BoardInitBeforeTempRamExit != NULL) {
      return BoardPreMemInit->BoardInitBeforeTempRamExit ();
    }
  }
  return EFI_SUCCESS;
}

/**
  A hook for board-specific initialization after disabling temporary RAM.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitAfterTempRamExit (
  VOID
  )
{
  BOARD_PRE_MEM_INIT_FUNC    *BoardPreMemInit;
  EFI_STATUS                 Status;

  Status = PeiServicesLocatePpi(
             &gBoardPreMemInitGuid,
             0,
             NULL,
             (VOID **)&BoardPreMemInit
             );
  if (!EFI_ERROR(Status)) {
    if (BoardPreMemInit->BoardInitAfterTempRamExit != NULL) {
      return BoardPreMemInit->BoardInitAfterTempRamExit ();
    }
  }
  return EFI_SUCCESS;
}

/**
  A hook for board-specific initialization prior to silicon initialization.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitBeforeSiliconInit (
  VOID
  )
{
  BOARD_POST_MEM_INIT_FUNC   *BoardPostMemInit;
  EFI_STATUS                 Status;

  Status = PeiServicesLocatePpi(
             &gBoardPostMemInitGuid,
             0,
             NULL,
             (VOID **)&BoardPostMemInit
             );
  if (!EFI_ERROR(Status)) {
    if (BoardPostMemInit->BoardInitBeforeSiliconInit != NULL) {
      return BoardPostMemInit->BoardInitBeforeSiliconInit ();
    }
  }
  return EFI_SUCCESS;
}

/**
  A hook for board-specific initialization after silicon initialization.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitAfterSiliconInit (
  VOID
  )
{
  BOARD_POST_MEM_INIT_FUNC   *BoardPostMemInit;
  EFI_STATUS                 Status;

  Status = PeiServicesLocatePpi(
             &gBoardPostMemInitGuid,
             0,
             NULL,
             (VOID **)&BoardPostMemInit
             );
  if (!EFI_ERROR(Status)) {
    if (BoardPostMemInit->BoardInitAfterSiliconInit != NULL) {
      return BoardPostMemInit->BoardInitAfterSiliconInit ();
    }
  }
  return EFI_SUCCESS;
}
