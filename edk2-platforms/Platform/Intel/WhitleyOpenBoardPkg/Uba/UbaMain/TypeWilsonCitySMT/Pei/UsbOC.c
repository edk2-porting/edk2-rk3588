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

USB_OVERCURRENT_PIN TypeWilsonCitySMTUsb20OverCurrentMappings[PCH_MAX_USB2_PORTS] = {
                          UsbOverCurrentPin0,
                          UsbOverCurrentPin1,
                          UsbOverCurrentPin1,
                          UsbOverCurrentPin2,
                          UsbOverCurrentPin3,
                          UsbOverCurrentPin3,
                          UsbOverCurrentPin7,
                          UsbOverCurrentPin7,
                          UsbOverCurrentPin6,
                          UsbOverCurrentPin4,
                          UsbOverCurrentPin6,
                          UsbOverCurrentPin4,
                          UsbOverCurrentPin5,
                          UsbOverCurrentPin4,
                          UsbOverCurrentPinSkip,
                          UsbOverCurrentPinSkip
                       };

USB_OVERCURRENT_PIN TypeWilsonCitySMTUsb30OverCurrentMappings[PCH_MAX_USB3_PORTS] = {
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

USB2_PHY_PARAMETERS         TypeWilsonCitySMTUsb20AfeParams[PCH_H_XHCI_MAX_USB2_PHYSICAL_PORTS] = {
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
TypeWilsonCitySMTPlatformUsbOcUpdateCallback (
  IN OUT   USB_OVERCURRENT_PIN   **Usb20OverCurrentMappings,
  IN OUT   USB_OVERCURRENT_PIN   **Usb30OverCurrentMappings,
  IN OUT   USB2_PHY_PARAMETERS        **Usb20AfeParams
)
{
  *Usb20OverCurrentMappings   = &TypeWilsonCitySMTUsb20OverCurrentMappings[0];
  *Usb30OverCurrentMappings   = &TypeWilsonCitySMTUsb30OverCurrentMappings[0];

  *Usb20AfeParams   = TypeWilsonCitySMTUsb20AfeParams;
  return EFI_SUCCESS;
}

PLATFORM_USBOC_UPDATE_TABLE  TypeWilsonCitySMTUsbOcUpdate =
{
   PLATFORM_USBOC_UPDATE_SIGNATURE,
   PLATFORM_USBOC_UPDATE_VERSION,
   TypeWilsonCitySMTPlatformUsbOcUpdateCallback
};

EFI_STATUS
TypeWilsonCitySMTPlatformUpdateUsbOcMappings (
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
                                 &TypeWilsonCitySMTUsbOcUpdate,
                                 sizeof(TypeWilsonCitySMTUsbOcUpdate)
                                 );

  return Status;
}


