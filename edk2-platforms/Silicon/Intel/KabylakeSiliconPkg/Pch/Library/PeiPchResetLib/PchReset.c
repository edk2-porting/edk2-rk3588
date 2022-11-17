/** @file
  PCH RESET PEIM DRIVER.

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiPei.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <PchAccess.h>
#include <Ppi/Reset2.h>
#include <Ppi/PchReset.h>
#include <Library/ResetSystemLib.h>
#include <PchResetPlatformSpecific.h>

/**
  Execute Pch Reset from the host controller.

  @param[in] This                 Pointer to the PCH_RESET_PPI instance.
  @param[in] PchResetType         Pch Reset Types which includes PchColdReset, PchWarmReset,
                                  PchShutdownReset, PchGlobalReset

  @retval EFI_SUCCESS             Successfully completed.
  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
EFI_STATUS
EFIAPI
Reset (
  IN PCH_RESET_PPI        *This,
  IN PCH_RESET_TYPE       PchResetType
  )
{
  PCH_RESET_DATA          ResetData;

  switch (PchResetType) {
    case PchColdReset:
      ResetCold();
      break;

    case PchWarmReset:
      ResetWarm();
      break;

    case PchShutdownReset:
      ResetShutdown();
      break;

    case PchGlobalReset:
      CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
      StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
      ResetPlatformSpecific (sizeof (PCH_RESET_DATA), &ResetData);
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
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
  EFI_STATUS              Status;
  PCH_RESET_PPI           *PeiPchResetPpi;
  EFI_PEI_PPI_DESCRIPTOR  *PeiPchResetDescriptor;
  EFI_PEI_RESET2_PPI      *EfiPeiReset2Ppi;
  EFI_PEI_PPI_DESCRIPTOR  *EfiPeiReset2Descriptor;

  DEBUG ((DEBUG_INFO, "PchInitializeReset() Start\n"));

  PeiPchResetDescriptor = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  PeiPchResetPpi = (PCH_RESET_PPI *) AllocateZeroPool (sizeof (PCH_RESET_PPI));
  if ((PeiPchResetDescriptor == NULL) ||
      (PeiPchResetPpi == NULL)) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  ///
  /// Initialize the Reset ppi instance
  ///
  PeiPchResetPpi->Reset         = Reset;
  PeiPchResetDescriptor->Flags  = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PeiPchResetDescriptor->Guid   = &gPchResetPpiGuid;
  PeiPchResetDescriptor->Ppi    = PeiPchResetPpi;

  ///
  /// Install the PCH RESET PPI
  ///
  Status = PeiServicesInstallPpi (PeiPchResetDescriptor);
  ASSERT_EFI_ERROR (Status);

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

