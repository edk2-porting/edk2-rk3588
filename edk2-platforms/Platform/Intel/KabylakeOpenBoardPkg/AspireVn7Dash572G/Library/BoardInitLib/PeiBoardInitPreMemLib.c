/** @file
  Aspire VN7-572G Board Initialization Pre-Memory library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BoardInitLib.h>
#include <Library/DebugLib.h>

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardDebugInit (
  VOID
  );

EFI_BOOT_MODE
EFIAPI
AspireVn7Dash572GBoardBootModeDetect (
  VOID
  );

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardInitBeforeMemoryInit (
  VOID
  );

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardInitAfterMemoryInit (
  VOID
  );

EFI_STATUS
EFIAPI
BoardDetect (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "%a(): Deferred until LPC programming is complete\n", __FUNCTION__));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BoardDebugInit (
  VOID
  )
{
  return AspireVn7Dash572GBoardDebugInit ();
}

EFI_BOOT_MODE
EFIAPI
BoardBootModeDetect (
  VOID
  )
{
  return AspireVn7Dash572GBoardBootModeDetect ();
}

EFI_STATUS
EFIAPI
BoardInitBeforeMemoryInit (
  VOID
  )
{
  return AspireVn7Dash572GBoardInitBeforeMemoryInit ();
}

EFI_STATUS
EFIAPI
BoardInitAfterMemoryInit (
  VOID
  )
{
  return AspireVn7Dash572GBoardInitAfterMemoryInit ();
}

EFI_STATUS
EFIAPI
BoardInitBeforeTempRamExit (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BoardInitAfterTempRamExit (
  VOID
  )
{
  return EFI_SUCCESS;
}
