/** @file
  Implementation of Fsp Misc UPD Initialization.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PeiFspPolicyInitLib.h>

#include <Library/MemoryAllocationLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/DebugLib.h>
#include <Library/DebugPrintErrorLevelLib.h>

/**
  Performs FSP Misc UPD initialization.

  @param[in][out]  FspmUpd             Pointer to FSPM_UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
**/
EFI_STATUS
EFIAPI
PeiFspMiscUpdInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  EFI_STATUS                        Status;

  //
  // Locate system configuration variable
  //
  FspmUpd->FspmArchUpd.StackBase = (VOID *)(UINTN)(PcdGet32(PcdTemporaryRamBase) + PcdGet32(PcdTemporaryRamSize) - (PcdGet32(PcdFspTemporaryRamSize) + PcdGet32(PcdFspReservedBufferSize)));
  FspmUpd->FspmArchUpd.StackSize = PcdGet32(PcdFspTemporaryRamSize);

  Status = PeiServicesGetBootMode (&(FspmUpd->FspmArchUpd.BootMode));
  if (EFI_ERROR (Status)) {
    FspmUpd->FspmArchUpd.BootMode = BOOT_WITH_FULL_CONFIGURATION;
  }

  FspmUpd->FspmArchUpd.BootLoaderTolumSize = 0x0;

  //
  // Initialize S3 Data variable (S3DataPtr). It may be used for warm and fast boot paths.
  //
  FspmUpd->FspmArchUpd.NvsBufferPtr = NULL;

  return EFI_SUCCESS;
}
