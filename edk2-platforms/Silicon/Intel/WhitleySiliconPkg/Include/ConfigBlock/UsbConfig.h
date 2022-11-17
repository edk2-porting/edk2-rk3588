/** @file
  Common USB policy shared between PCH and CPU
  Contains general features settings for xHCI and xDCI

@copyright
  Copyright 2017 - 2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _USB_CONFIG_H_
#define _USB_CONFIG_H_

#define USB_CONFIG_REVISION 1
extern EFI_GUID gUsbConfigGuid;

#define MAX_USB2_PORTS  16
#define MAX_USB3_PORTS  10

#pragma pack (push,1)

#define PCH_USB_OC_PINS_MAX  8  ///< Maximal possible number of USB Over Current pins

///
/// Overcurrent pins, the values match the setting of EDS, please refer to EDS for more details
///
typedef enum {
  UsbOverCurrentPin0 = 0,
  UsbOverCurrentPin1,
  UsbOverCurrentPin2,
  UsbOverCurrentPin3,
  UsbOverCurrentPin4,
  UsbOverCurrentPin5,
  UsbOverCurrentPin6,
  UsbOverCurrentPin7,
  UsbOverCurrentPinMax,
  UsbOverCurrentPinSkip = 0xFF
} USB_OVERCURRENT_PIN;

/**
  This structure configures per USB2.0 port settings like enabling and overcurrent protection
**/
typedef struct {
  /**
    These members describe the specific over current pin number of USB 2.0 Port N.
    It is SW's responsibility to ensure that a given port's bit map is set only for
    one OC pin Description. USB2 and USB3 on the same combo Port must use the same
    OC pin (see: USB_OVERCURRENT_PIN).
  **/
  UINT32     OverCurrentPin     :  8;
  UINT32     Enable             :  1;     ///< 0: Disable; <b>1: Enable</b>.
  UINT32     RsvdBits0          : 23;     ///< Reserved bits
} USB2_PORT_CONFIG;

/**
  This structure configures per USB3.x port settings like enabling and overcurrent protection
**/
typedef struct {
  /**
    These members describe the specific over current pin number of USB 2.0 Port N.
    It is SW's responsibility to ensure that a given port's bit map is set only for
    one OC pin Description. USB2 and USB3 on the same combo Port must use the same
    OC pin (see: USB_OVERCURRENT_PIN).
  **/
  UINT32     OverCurrentPin     :  8;
  UINT32     Enable             :  1;     ///< 0: Disable; <b>1: Enable</b>.
  UINT32     RsvdBits0          : 23;     ///< Reserved bits
} USB3_PORT_CONFIG;


/**
  The XDCI_CONFIG block describes the configurations
  of the xDCI Usb Device controller.
**/
typedef struct {
  /**
    This member describes whether or not the xDCI controller should be enabled.
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32  Enable              :  1;
  UINT32  RsvdBits0           : 31;     ///< Reserved bits
} XDCI_CONFIG;

#pragma pack (pop)

#endif // _USB_CONFIG_H_
