/** @file
  Implementation of Fsp Misc UPD Initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>

#include <Library/DebugLib.h>
#include <Library/PeiLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesLib.h>

#include <FspEas.h>
#include <FspmUpd.h>
#include <FspsUpd.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/PciLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/MemoryOverwriteControl.h>
#include <PchAccess.h>

#include "PeiMiscPolicyUpdate.h"

/**
  Performs FSP Misc UPD initialization.

  @param[in,out]    FspmUpd                 Pointer to FSPM_UPD Data.

  @retval           EFI_SUCCESS             FSP UPD Data is updated.
  @retval           EFI_NOT_FOUND           An instance of gEfiPeiReadOnlyVariable2PpiGuid
                                            could not be located.
  @retval           EFI_OUT_OF_RESOURCES    Insufficent resources to allocate a memory buffer.
**/
EFI_STATUS
EFIAPI
PeiFspMiscUpdUpdatePreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VariableSize;
  VOID                              *MemorySavedData;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  VariableSize = 0;
  MemorySavedData = NULL;
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"MemoryConfig",
                               &gFspNonVolatileStorageHobGuid,
                               NULL,
                               &VariableSize,
                               MemorySavedData
                               );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    MemorySavedData = AllocatePool (VariableSize);
    if (MemorySavedData == NULL) {
      ASSERT (MemorySavedData != NULL);
      return EFI_OUT_OF_RESOURCES;
    }

    DEBUG ((DEBUG_INFO, "VariableSize is 0x%x\n", VariableSize));
    Status = VariableServices->GetVariable (
                                 VariableServices,
                                 L"MemoryConfig",
                                 &gFspNonVolatileStorageHobGuid,
                                 NULL,
                                 &VariableSize,
                                 MemorySavedData
                                 );
    if (Status == EFI_SUCCESS) {
      FspmUpd->FspmArchUpd.NvsBufferPtr = MemorySavedData;
    } else {
      DEBUG ((DEBUG_ERROR, "Fail to retrieve Variable:\"MemoryConfig\" gMemoryConfigVariableGuid, Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }
  FspmUpd->FspmArchUpd.NvsBufferPtr = MemorySavedData;

  return EFI_SUCCESS;
}


