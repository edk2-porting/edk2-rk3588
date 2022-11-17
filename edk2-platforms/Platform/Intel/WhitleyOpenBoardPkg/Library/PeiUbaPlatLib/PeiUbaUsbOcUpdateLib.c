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
#include <Library/UbaUsbOcUpdateLib.h>

EFI_STATUS
PlatformGetUsbOcMappings (
  IN OUT   USB_OVERCURRENT_PIN   **Usb20OverCurrentMappings,
  IN OUT   USB_OVERCURRENT_PIN   **Usb30OverCurrentMappings,
  IN OUT   USB2_PHY_PARAMETERS   **Usb20AfeParams
)
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  PLATFORM_USBOC_UPDATE_TABLE           UsbOcUpdateTable;
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

  TableSize = sizeof(UsbOcUpdateTable);
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPeiPlatformUbaOcConfigDataGuid,
                                &UsbOcUpdateTable,
                                &TableSize
                                );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (UsbOcUpdateTable.Signature == PLATFORM_USBOC_UPDATE_SIGNATURE);
  ASSERT (UsbOcUpdateTable.Version == PLATFORM_USBOC_UPDATE_VERSION);

  UsbOcUpdateTable.CallUsbOcUpdate( Usb20OverCurrentMappings,
                                    Usb30OverCurrentMappings,
                                    Usb20AfeParams
                                  );
  return Status;
}
