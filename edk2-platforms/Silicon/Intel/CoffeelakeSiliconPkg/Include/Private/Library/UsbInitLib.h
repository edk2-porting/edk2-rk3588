/** @file
  Header file for USB initialization library.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _USB_INIT_LIB_H_
#define _USB_INIT_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Common entry point for PCH and CPU xDCI controller

  @param[in]  UsbConfig           The USB_CONFIG policy instance
  @param[in]  XdciPciMmBase       xDCI PCI config space address
**/
VOID
XdciConfigure (
  IN  USB_CONFIG      *UsbConfig,
  IN  UINT64          XhciPciMmBase
  );

/**
  Common entry point for PCH and CPU xHCI controller

  @param[in]  UsbConfig           The USB_CONFIG policy instance
  @param[in]  XhciPciMmBase       xHCI PCI config space address
**/
VOID
XhciConfigure (
  IN  USB_CONFIG      *UsbConfig,
  IN  UINT64          XhciPciMmBase
  );

/**
  Configure xHCI after initialization

  @param[in]  UsbConfig           The USB_CONFIG policy instance
  @param[in]  XhciPciMmBase       XHCI PCI CFG Base Address
**/
VOID
XhciConfigureAfterInit (
  IN  USB_CONFIG      *UsbConfig,
  IN  UINT64          XhciPciMmBase
  );

/**
  Locks xHCI configuration by setting the proper lock bits in controller

  @param[in]  UsbConfig           The USB_CONFIG policy instance
  @param[in]  XhciPciBase         xHCI PCI config space address
**/
VOID
XhciLockConfiguration (
  IN  USB_CONFIG      *UsbConfig,
  IN  UINT64          XhciPciBase
  );

/**
  Tune the USB 2.0 high-speed signals quality.

  @param[in]  UsbConfig           The USB_CONFIG policy instance
**/
VOID
Usb2AfeProgramming (
  IN  USB_CONFIG      *UsbConfig
  );
#endif // _USB_INIT_LIB_H_
