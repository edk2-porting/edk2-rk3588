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
#include <Library/UbaClocksConfigLib.h>

EFI_STATUS
ConfigurePlatformClock (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *SmbusPpi
)
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  PLATFORM_CLOCKS_CONFIG_TABLE          PlatformClocksConfigTable;
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

  TableSize = sizeof(PlatformClocksConfigTable);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformClocksConfigDataGuid,
                                &PlatformClocksConfigTable,
                                &TableSize
                                );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (PlatformClocksConfigTable.Signature == PLATFORM_CLOCKS_CONFIG_SIGNATURE);
  ASSERT (PlatformClocksConfigTable.Version == PLATFORM_CLOCKS_CONFIG_VERSION);

  Status = PlatformClocksConfigTable.CallUpdate(PeiServices, NotifyDescriptor, SmbusPpi);

  return Status;
}
