/** @file

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiBoardInit.h"


#include <Library/PcdLib.h>
#include <Library/UbaUsbOcUpdateLib.h>
#include <PchLimits.h>
#include <ConfigBlock/UsbConfig.h>
#include <ConfigBlock/Usb2PhyConfig.h>

USB_OVERCURRENT_PIN TypeCooperCityRPUsb20OverCurrentMappings[PCH_MAX_USB2_PORTS] = {
                          UsbOverCurrentPin0,      //Port00: Zepher        ,OC0#
                          UsbOverCurrentPin1,      //Port01: Read Connector,OC1#
                          UsbOverCurrentPinSkip,   //Port02: User bay      ,OC0#
                          UsbOverCurrentPinSkip,   //Port03: iBMC USB 1.1  ,no OCn#
                          UsbOverCurrentPinSkip,   //Port04: NONE          ,no OCn#
                          UsbOverCurrentPin2,      //Port05: Read Connector,OC2#
                          UsbOverCurrentPin1,      //Port06: Read Connector,OC1#
                          UsbOverCurrentPin2,      //Port07: Read Connector,OC2#
                          UsbOverCurrentPinSkip,   //Port08: NONE          ,no OCn#
                          UsbOverCurrentPinSkip,   //Port09: NONE          ,no OCn#
                          UsbOverCurrentPinSkip,   //Port10: iBMC USB 2.0  ,no OCn#
                          UsbOverCurrentPin4,      //Port11: Front Panel   ,OC4#
                          UsbOverCurrentPinSkip,   //Port12: NONE          ,no OCn#
                          UsbOverCurrentPin4      //Port13: Front Panel   ,OC4#
                       };

USB_OVERCURRENT_PIN TypeCooperCityRPUsb30OverCurrentMappings[PCH_MAX_USB3_PORTS] = {
                          UsbOverCurrentPin0,
                          UsbOverCurrentPin1,
                          UsbOverCurrentPin1,
                          UsbOverCurrentPin2,
                          UsbOverCurrentPin3,
                          UsbOverCurrentPin3,
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

EFI_STATUS
TypeCooperCityRPPlatformUpdateUsbOcMappings (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  //#
  //# USB, see PG 104 in GZP SCH
  //#

//  USB2      USB3      Port                            OC
//
//Port00:     PORT5     Back Panel                      ,OC0#
//Port01:     PORT2     Back Panel                      ,OC0#
//Port02:     PORT3     Back Panel                      ,OC1#
//Port03:     PORT0     NOT USED                        ,NA
//Port04:               BMC1.0                          ,NA
//Port05:               INTERNAL_2X5_A                  ,OC2#
//Port06:               INTERNAL_2X5_A                  ,OC2#
//Port07:               NOT USED                        ,NA
//Port08:               EUSB (AKA SSD)                  ,NA
//Port09:               INTERNAL_TYPEA                  ,OC6#
//Port10:     PORT1     Front Panel                     ,OC5#
//Port11:               NOT USED                        ,NA
//Port12:               BMC2.0                          ,NA
//Port13:     PORT4     Front Panel                     ,OC5#

  EFI_STATUS                   Status;

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPeiPlatformUbaOcConfigDataGuid,
                                 &TypeCooperCityRPUsbOcUpdate,
                                 sizeof(TypeCooperCityRPUsbOcUpdate)
                                 );

  return Status;
}


