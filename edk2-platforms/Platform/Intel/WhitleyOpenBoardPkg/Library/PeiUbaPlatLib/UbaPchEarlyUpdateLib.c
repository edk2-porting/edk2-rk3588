/** @file

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
#include <Library/IoLib.h>

#include <Library/UbaPchEarlyUpdateLib.h>

EFI_STATUS
PlatformPchLanConfig (
  IN SYSTEM_CONFIGURATION         *SystemConfig
)
{
  EFI_STATUS                            Status;

  UBA_CONFIG_DATABASE_PPI         *UbaConfigPpi = NULL;
  PLATFORM_PCH_EARLY_UPDATE_TABLE       PchEarlyUpdateTable;
  UINTN                                 TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(PchEarlyUpdateTable);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformPchEarlyConfigDataGuid,
                                &PchEarlyUpdateTable,
                                &TableSize
                                );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (PchEarlyUpdateTable.Signature == PLATFORM_PCH_EARLY_UPDATE_SIGNATURE);
  ASSERT (PchEarlyUpdateTable.Version == PLATFORM_PCH_EARLY_UPDATE_VERSION);

  Status = PchEarlyUpdateTable.ConfigLan (SystemConfig);

  return Status;
}

EFI_STATUS
PlatformInitLateHook (
  IN SYSTEM_CONFIGURATION         *SystemConfig
)
{
  EFI_STATUS                            Status;

  UBA_CONFIG_DATABASE_PPI         *UbaConfigPpi = NULL;
  PLATFORM_PCH_EARLY_UPDATE_TABLE       PchEarlyUpdateTable;
  UINTN                                 TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(PchEarlyUpdateTable);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformPchEarlyConfigDataGuid,
                                &PchEarlyUpdateTable,
                                &TableSize
                                );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (PchEarlyUpdateTable.Signature == PLATFORM_PCH_EARLY_UPDATE_SIGNATURE);
  ASSERT (PchEarlyUpdateTable.Version == PLATFORM_PCH_EARLY_UPDATE_VERSION);

  if (PchEarlyUpdateTable.InitLateHook == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = PchEarlyUpdateTable.InitLateHook (SystemConfig);

  return Status;
}


