/** @file
  UbaSoftStrapUpdateLib implementation.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/UbaSoftStrapUpdateLib.h>


EFI_STATUS
GetPchSoftSoftStrapTable (
  IN  VOID                    **PchSoftStrapTable
  )
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_PCH_SOFTSTRAP_UPDATE     PchSoftStrapUpdate;
  UINTN                             Size;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Size = sizeof(PchSoftStrapUpdate);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformPchSoftStrapConfigDataGuid,
                                &PchSoftStrapUpdate,
                                &Size
                                );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (PchSoftStrapUpdate.Signature == PLATFORM_SOFT_STRAP_UPDATE_SIGNATURE);
  ASSERT (PchSoftStrapUpdate.Version == PLATFORM_SOFT_STRAP_UPDATE_VERSION);

  *PchSoftStrapTable = PchSoftStrapUpdate.PchSoftStrapTablePtr;

  return Status;
}

VOID
PlatformSpecificPchSoftStrapUpdate (
  IN OUT  UINT8                 *FlashDescriptorCopy
  )
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_PCH_SOFTSTRAP_UPDATE     PchSoftStrapUpdate;
  UINTN                             Size;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return;
  }

  Size = sizeof(PchSoftStrapUpdate);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformPchSoftStrapConfigDataGuid,
                                &PchSoftStrapUpdate,
                                &Size
                                );
  if (EFI_ERROR(Status)) {
    return;
  }

  ASSERT (PchSoftStrapUpdate.Signature == PLATFORM_SOFT_STRAP_UPDATE_SIGNATURE);
  ASSERT (PchSoftStrapUpdate.Version == PLATFORM_SOFT_STRAP_UPDATE_VERSION);

  PchSoftStrapUpdate.PchSoftStrapPlatformSpecificUpdate (FlashDescriptorCopy);
}

