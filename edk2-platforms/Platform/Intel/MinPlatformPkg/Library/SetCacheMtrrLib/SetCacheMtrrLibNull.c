/** @file

NULL instances of SetCacheMtrr library functions.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

/**
  Set Cache Mtrr.
**/
VOID
EFIAPI
SetCacheMtrr (
  VOID
  )
{
  return;
}

/**
  Update MTRR setting in EndOfPei phase.
  This function will clear temporary memory (CAR) phase MTRR settings
  and configure MTRR to cover permanent memory.

  @retval  EFI_SUCCESS  The function completes successfully.
**/
EFI_STATUS
EFIAPI
SetCacheMtrrAfterEndOfPei (
  VOID
  )
{
  return EFI_SUCCESS;
}
