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

USB_OVERCURRENT_PIN TypeCooperCityRPUsb20OverCurrentMappings[PCH_MAX_USB2_PORTS] = {
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
                          UsbOverCurrentPin4       //Port13: NC
                       };

USB_OVERCURRENT_PIN TypeCooperCityRPUsb30OverCurrentMappings[PCH_MAX_USB3_PORTS] = {
                          UsbOverCurrentPin6,     //Port01: Front Panel
                          UsbOverCurrentPin6,     //Port02: Front Panel
                          UsbOverCurrentPin0,     //Port03: Rear Panel
                          UsbOverCurrentPin1,     //Port04: Rear Panel
                          UsbOverCurrentPin1,     //Port05: Rear Panel
                          UsbOverCurrentPinSkip,  //Port06: NC
                          UsbOverCurrentPinSkip,
                          UsbOverCurrentPinSkip,
                          UsbOverCurrentPinSkip,
                          UsbOverCurrentPinSkip
                       };

USB2_PHY_PARAMETERS         TypeCooperCityRPUsb20AfeParams[PCH_H_XHCI_MAX_USB2_PHYSICAL_PORTS] = {
                        {7, 0, 2, 1},   // PP0
                        {7, 0, 2, 1},   // PP1
                        {7, 0, 2, 1},   // PP2
                        {7, 0, 2, 1},   // PP3
                        {7, 0, 2, 1},   // PP4
                        {7, 0, 2, 1},   // PP5
                        {7, 0, 2, 1},   // PP6
                        {7, 0, 2, 1},   // PP7
                        {7, 0, 2, 1},   // PP8
                        {7, 0, 2, 1},   // PP9
                        {7, 0, 2, 1},   // PP10
                        {7, 0, 2, 1},   // PP11
                        {7, 0, 2, 1},   // PP12
                        {7, 0, 2, 1},   // PP13
                      };

EFI_STATUS
TypeCooperCityRPPlatformUsbOcUpdateCallback (
  IN OUT   USB_OVERCURRENT_PIN   **Usb20OverCurrentMappings,
  IN OUT   USB_OVERCURRENT_PIN   **Usb30OverCurrentMappings,
  IN OUT   USB2_PHY_PARAMETERS        **Usb20AfeParams
)
{
   *Usb20OverCurrentMappings   = &TypeCooperCityRPUsb20OverCurrentMappings[0];
   *Usb30OverCurrentMappings   = &TypeCooperCityRPUsb30OverCurrentMappings[0];
   *Usb20AfeParams             = TypeCooperCityRPUsb20AfeParams;
   return EFI_SUCCESS;
}

PLATFORM_USBOC_UPDATE_TABLE  TypeCooperCityRPUsbOcUpdate =
{
   PLATFORM_USBOC_UPDATE_SIGNATURE,
   PLATFORM_USBOC_UPDATE_VERSION,
   TypeCooperCityRPPlatformUsbOcUpdateCallback
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

  DEBUG((DEBUG_INFO, "UBA:UsbOcUpdate-TypeCooperCityRP\n"));
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
                                     &TypeCooperCityRPUsbOcUpdate,
                                     sizeof(TypeCooperCityRPUsbOcUpdate)
                                     );

  return Status;
}
