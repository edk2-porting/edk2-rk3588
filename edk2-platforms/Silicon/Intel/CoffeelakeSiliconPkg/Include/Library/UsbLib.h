/** @file
  Header file of available functions in general USB Library

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _USB_LIB_H_
#define _USB_LIB_H_

#include <Uefi/UefiBaseType.h>

/*
  Disables requested ports through Port Disable Override register programming

  @param[in]  XhciMmioBase        xHCI Memory BAR0 address
  @param[in]  Usb2DisabledPorts   Disabled ports bitmask with a bit for each USB2 port
                                  i.e. BIT0 is Port 0, BIT1 is Port 1 etc
  @param[in]  Usb3DisabledPorts   Disabled ports bitmask with a bit for each USB3 port
                                  i.e. BIT0 is Port 0, BIT1 is Port 1 etc

  @retval EFI_SUCCESS             Programming ended successfully and no errors occured
          EFI_ACCESS_DENIED       Port Disable Override register was locked and write
                                  didn't go through. Platform may require restart to unlock.
*/
EFI_STATUS
UsbDisablePorts (
  IN  UINTN   XhciMmioBase,
  IN  UINT32  Usb2DisabledPorts,
  IN  UINT32  Usb3DisabledPorts
  );

#endif // _USB_LIB_H
