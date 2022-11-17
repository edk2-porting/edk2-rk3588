/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
// EDK and EDKII have different GUID formats
//
#include <Uefi/UefiBaseType.h>
#include <Ppi/PchPolicy.h>
#include "PlatformHost.h"
#include <Guid/SetupVariable.h>
#include <Guid/PchRcVariable.h>
#include <Library/PchInfoLib.h>


VOID
UpdatePchUsbConfig (
  IN PCH_USB_CONFIG            *PchUsbConfig,
  IN SYSTEM_CONFIGURATION      *SetupVariables,
  IN PCH_RC_CONFIGURATION      *PchRcVariables,
  IN VOID                      *Usb20OverCurrentMappings,
  IN VOID                      *Usb30OverCurrentMappings
  )
/*++

Routine Description:

  This function performs PCH USB Platform Policy initialzation

Arguments:
  PchUsbConfig                    Pointer to PCH_USB_CONFIG data buffer
  SetupVariables                  Pointer to Setup variable
  PlatformType                    PlatformType specified
  PlatformFlavor                  PlatformFlavor specified
  BoardType                       BoardType specified

Returns:

--*/
{
  UINTN  PortIndex;

  PchUsbConfig->UsbPrecondition = PchRcVariables->UsbPrecondition;

    for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb2PortNum (); PortIndex++) {
      if (PchRcVariables->PchUsbHsPort[PortIndex] == 1) {
      PchUsbConfig->PortUsb20[PortIndex].Enable = TRUE;
    } else {
        PchUsbConfig->PortUsb20[PortIndex].Enable = FALSE;
      }
    }
    for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
      if (PchRcVariables->PchUsbSsPort[PortIndex] == 1) {
      PchUsbConfig->PortUsb30[PortIndex].Enable = TRUE;
    } else {
        PchUsbConfig->PortUsb30[PortIndex].Enable = FALSE;
    }
  }

  //
  // xDCI (USB device) related settings from setup variable
  //
  if(PchRcVariables->PchXdciSupport == 1) {
    PchUsbConfig->XdciConfig.Enable= TRUE;
  } else {
    PchUsbConfig->XdciConfig.Enable= FALSE;
  }

  //
  // XHCI USB Over Current Pins disabled, update it based on setup option.
  //
  PchUsbConfig->XhciOcMapEnabled = PchRcVariables->XhciOcMapEnabled;

  //
  // XHCI Wake On USB configured based on user input through setup option
  //
  PchUsbConfig->XhciWakeOnUsb = SetupVariables->XhciWakeOnUsbEnabled;
  //
  // XHCI option to disable MSIs
  //
  PchUsbConfig->XhciDisMSICapability = PchRcVariables->XhciDisMSICapability;

  //
  // Platform Board programming per the layout of each port.
  //
  // OC Map for USB2 Ports
  for (PortIndex=0;PortIndex<PCH_MAX_USB2_PORTS;PortIndex++) {
    PchUsbConfig->PortUsb20[PortIndex].OverCurrentPin = (UINT8)((PCH_USB_OVERCURRENT_PIN *)Usb20OverCurrentMappings)[PortIndex];
  }

  // OC Map for USB3 Ports
  for (PortIndex=0;PortIndex<PCH_MAX_USB3_PORTS;PortIndex++) {
    PchUsbConfig->PortUsb30[PortIndex].OverCurrentPin = (UINT8)((PCH_USB_OVERCURRENT_PIN *)Usb30OverCurrentMappings)[PortIndex];
  }

}
