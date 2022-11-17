/** @file
  UbaGpioUpdateLib implementation.

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

#include <Library/UbaGpioUpdateLib.h>

EFI_STATUS
PlatformUpdateGpios (
  VOID
)
{
  EFI_STATUS                            Status;

  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  PLATFORM_GPIO_UPDATE_TABLE            GpioTable;
  UINTN                                 TableSize;
  UINTN                                 Index;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(GpioTable);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformGpioConfigDataGuid,
                                &GpioTable,
                                &TableSize
                                );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (GpioTable.Signature == PLATFORM_GPIO_UPDATE_SIGNATURE);
  ASSERT (GpioTable.Version == PLATFORM_GPIO_UPDATE_VERSION);

  for (Index = 0; GpioTable.Gpios[Index].Register != PLATFORM_END_OF_GPIO_LIST; Index++) {

    IoWrite32 (GpioTable.Gpios[Index].Register, GpioTable.Gpios[Index].Value);
  }

  return Status;
}


