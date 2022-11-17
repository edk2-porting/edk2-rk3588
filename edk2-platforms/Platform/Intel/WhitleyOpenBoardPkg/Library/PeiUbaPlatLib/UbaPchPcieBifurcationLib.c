/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

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
#include <Library/UbaPcieBifurcationUpdateLib.h>

EFI_STATUS
PlatformGetPchPcieBifurcationConfig (
  IN OUT   PCIE_BIFURCATION_CONFIG         **PchPcieBifurcationConfig,
  IN OUT   PCH_SLOT_CONFIG_DATA_ENTRY_EX   **PchSlotConfig
)
{
  EFI_STATUS                                    Status;
  UBA_CONFIG_DATABASE_PPI                       *UbaConfigPpi = NULL;
  PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_TABLE    BifurcationUpdateTable;
  UINTN                                         TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(BifurcationUpdateTable);
  Status = UbaConfigPpi->GetData (
                           UbaConfigPpi,
                           &gPlatformUbaPcieBifurcationGuid,
                           &BifurcationUpdateTable,
                           &TableSize
                           );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (BifurcationUpdateTable.Signature == PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_SIGNATURE);
  ASSERT (BifurcationUpdateTable.Version == PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_VERSION);

  BifurcationUpdateTable.CallPcieBifurcationUpdate(PchPcieBifurcationConfig, PchSlotConfig);
  return Status;
}
