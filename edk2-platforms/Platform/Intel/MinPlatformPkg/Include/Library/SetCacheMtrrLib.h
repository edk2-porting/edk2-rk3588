/** @file

Header for SetCacheMtrr library functions.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SET_CACHE_MTRR_LIB_H_
#define _SET_CACHE_MTRR_LIB_H_

/**
  Set Cache Mtrr.
**/
VOID
EFIAPI
SetCacheMtrr (
  VOID
  );

/**
  Update MTRR setting in EndOfPei phase.
  This function will clear temporary memory (CAR) phase MTRR settings
  and configure MTRR to cover permanent memory.

  @retval  EFI_SUCCESS  The function completes successfully.
  @retval  Others       Some error occurs.
**/
EFI_STATUS
EFIAPI
SetCacheMtrrAfterEndOfPei (
  VOID
  );

#endif
