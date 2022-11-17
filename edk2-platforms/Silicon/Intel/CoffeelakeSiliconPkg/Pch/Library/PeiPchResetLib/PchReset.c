/** @file
  PCH RESET PEIM DRIVER.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/Reset2.h>
#include <Ppi/PchReset.h>
#include <Library/PchResetLib.h>
#include <Library/ResetSystemLib.h>


/**
  Resets the entire platform.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.

**/
VOID
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE     ResetType,
  IN EFI_STATUS         ResetStatus,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  )
{
  switch (ResetType) {
  case EfiResetWarm:
    ResetWarm ();
    break;

  case EfiResetCold:
    ResetCold ();
    break;

  case EfiResetShutdown:
    ResetShutdown ();
    return;

  case EfiResetPlatformSpecific:
    ResetPlatformSpecific (DataSize, ResetData);
    return;

  default:
    return;
  }

  //
  // Given we should have reset getting here would be bad
  //
  ASSERT (FALSE);
  CpuDeadLoop();
}

/**
  Initialize PCH Reset APIs

  @retval EFI_SUCCESS             APIs are installed successfully
  @retval EFI_OUT_OF_RESOURCES    Can't allocate pool
**/
EFI_STATUS
EFIAPI
PchInitializeReset (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_PEI_RESET2_PPI        *EfiPeiReset2Ppi;
  EFI_PEI_PPI_DESCRIPTOR    *EfiPeiReset2Descriptor;

  DEBUG ((DEBUG_INFO, "PchInitializeReset() Start\n"));


  EfiPeiReset2Descriptor = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  EfiPeiReset2Ppi = (EFI_PEI_RESET2_PPI *) AllocateZeroPool (sizeof (EFI_PEI_RESET2_PPI));
  if ((EfiPeiReset2Descriptor == NULL) ||
      (EfiPeiReset2Ppi == NULL)) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  ///
  /// Initialize the EFI Reset2 ppi instance
  ///
  EfiPeiReset2Ppi->ResetSystem  = ResetSystem;

  EfiPeiReset2Descriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  EfiPeiReset2Descriptor->Guid  = &gEfiPeiReset2PpiGuid;
  EfiPeiReset2Descriptor->Ppi   = EfiPeiReset2Ppi;

  Status = PeiServicesInstallPpi (EfiPeiReset2Descriptor);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "PchInitializeReset() End\n"));

  return Status;
}

