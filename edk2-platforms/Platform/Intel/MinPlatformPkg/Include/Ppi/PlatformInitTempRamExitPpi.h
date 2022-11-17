/** @file
  This file defines the PPI for notifying PlatformInitPreMem
  of temporary memory being disabled.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PLATFORM_INIT_TEMP_RAM_EXIT_PPI_H_
#define _PLATFORM_INIT_TEMP_RAM_EXIT_PPI_H_

#include <PiPei.h>

//
// Forward declaration for the PLATFORM_INIT_TEMP_RAM_EXIT_PPI.
//
typedef struct _PLATFORM_INIT_TEMP_RAM_EXIT_PPI PLATFORM_INIT_TEMP_RAM_EXIT_PPI;

/**
  A hook for platform-specific initialization prior to disabling temporary RAM.

  @retval EFI_SUCCESS   The platform initialization was successful.
  @retval EFI_NOT_READY The platform has not been detected yet.
**/
typedef
EFI_STATUS
(EFIAPI *PLATFORM_INIT_BEFORE_TEMP_RAM_EXIT) (
  VOID
  );

/**
  A hook for platform-specific initialization after disabling temporary RAM.

  @retval EFI_SUCCESS   The platform initialization was successful.
  @retval EFI_NOT_READY The platform has not been detected yet.
**/
typedef
EFI_STATUS
(EFIAPI *PLATFORM_INIT_AFTER_TEMP_RAM_EXIT) (
  VOID
  );

///
/// This PPI provides functions for notifying PlatformInitPreMem
/// of temporary memory being disabled.
///
struct _PLATFORM_INIT_TEMP_RAM_EXIT_PPI {
  PLATFORM_INIT_BEFORE_TEMP_RAM_EXIT    PlatformInitBeforeTempRamExit;
  PLATFORM_INIT_AFTER_TEMP_RAM_EXIT     PlatformInitAfterTempRamExit;
};

extern EFI_GUID gPlatformInitTempRamExitPpiGuid;

#endif // _PLATFORM_INIT_TEMP_RAM_EXIT_PPI_H_
