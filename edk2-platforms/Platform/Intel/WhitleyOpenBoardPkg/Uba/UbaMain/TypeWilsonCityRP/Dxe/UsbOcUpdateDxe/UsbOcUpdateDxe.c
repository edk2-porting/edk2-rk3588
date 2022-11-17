/** @file

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "UsbOcUpdateDxe.h"

#include <Library/UbaUsbOcUpdateLib.h>
#include <PchLimits.h>
#include <ConfigBlock/UsbConfig.h>
#include <ConfigBlock/Usb2PhyConfig.h>

USB_OVERCURRENT_PIN TypeWilsonCityRPUsb20OverCurrentMappings[PCH_MAX_USB2_PORTS] = {
                          UsbOverCurrentPinSkip,   //Port00: BMC
                          UsbOverCurrentPinSkip,   //Port01: BMC
                          UsbOverCurrentPin0,      //Port02: Rear Panel
                          UsbOverCurrentPin1,      //Port03: Rear Panel
                          UsbOverCurrentPin1,      //Port04: Rear Panel
                          UsbOverCurrentPinSkip,   //Port05: NC
                          UsbOverCurrentPinSkip,   //Port06: NC
                          UsbOverCurrentPin4,      //Port07: Type A internal
                          UsbOverCurrentPinSkip,   //Port08: NC
                          UsbOverCurrentPinSkip,   //Port09: NC
                          UsbOverCurrentPin6,      //Port10: Front Panel
                          UsbOverCurrentPinSkip,   //Port11: NC
                          UsbOverCurrentPin6,      //Port12: Front Panel
                          UsbOverCurrentPinSkip,   //Port13: NC
                          UsbOverCurrentPinSkip,
                          UsbOverCurrentPinSkip
                       };

USB_OVERCURRENT_PIN TypeWilsonCityRPUsb30OverCurrentMappings[PCH_MAX_USB3_PORTS] = {
                          UsbOverCurrentPin6,    //Port01: Front Panel
                          UsbOverCurrentPin6,    //Port02: Front Panel
                          UsbOverCurrentPin0,    //Port03: Rear Panel
                          UsbOverCurrentPin1,    //Port04: Rear Panel
                          UsbOverCurrentPin1,    //Port05: Rear Panel
                          UsbOverCurrentPinSkip, //Port06: NC
                          UsbOverCurrentPinSkip,
                          UsbOverCurrentPinSkip,
                          UsbOverCurrentPinSkip,
                          UsbOverCurrentPinSkip
                       };

USB2_PHY_PARAMETERS         TypeWilsonCityRPUsb20AfeParams[PCH_H_XHCI_MAX_USB2_PHYSICAL_PORTS] = {
                        {3, 0, 3, 1},   // PP0
                        {5, 0, 3, 1},   // PP1
                        {3, 0, 3, 1},   // PP2
                        {0, 5, 1, 1},   // PP3
                        {3, 0, 3, 1},   // PP4
                        {3, 0, 3, 1},   // PP5
                        {3, 0, 3, 1},   // PP6
                        {3, 0, 3, 1},   // PP7
                        {2, 2, 1, 0},   // PP8
                        {6, 0, 2, 1},   // PP9
                        {2, 2, 1, 0},   // PP10
                        {6, 0, 2, 1},   // PP11
                        {0, 5, 1, 1},   // PP12
                        {7, 0, 2, 1},   // PP13
                      };

EFI_STATUS
TypeWilsonCityRPPlatformUsbOcUpdateCallback (
  IN OUT   USB_OVERCURRENT_PIN   **Usb20OverCurrentMappings,
  IN OUT   USB_OVERCURRENT_PIN   **Usb30OverCurrentMappings,
  IN OUT   USB2_PHY_PARAMETERS        **Usb20AfeParams
)
{
  *Usb20OverCurrentMappings   = &TypeWilsonCityRPUsb20OverCurrentMappings[0];
  *Usb30OverCurrentMappings   = &TypeWilsonCityRPUsb30OverCurrentMappings[0];

  *Usb20AfeParams   = TypeWilsonCityRPUsb20AfeParams;
  return EFI_SUCCESS;
}

PLATFORM_USBOC_UPDATE_TABLE  TypeWilsonCityRPUsbOcUpdate =
{
   PLATFORM_USBOC_UPDATE_SIGNATURE,
   PLATFORM_USBOC_UPDATE_VERSION,
   TypeWilsonCityRPPlatformUsbOcUpdateCallback
};

/**
  The Driver Entry Point.

  The function is the driver Entry point.

  @param ImageHandle   A handle for the image that is initializing this driver
  @param SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
UsbOcUpdateEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                          Status;
  UBA_CONFIG_DATABASE_PROTOCOL        *UbaConfigProtocol = NULL;

  DEBUG((EFI_D_INFO, "UBA:UsbOcUpdate-TypeWilsonCityRP\n"));
  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gDxePlatformUbaOcConfigDataGuid,
                                     &TypeWilsonCityRPUsbOcUpdate,
                                     sizeof(TypeWilsonCityRPUsbOcUpdate)
                                     );

  return Status;
}
