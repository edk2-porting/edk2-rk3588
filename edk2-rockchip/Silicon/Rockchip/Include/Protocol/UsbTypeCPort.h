/** @file
*
*  USB Type-C port state, produced by a Type-C port controller driver and
*  consumed by the combo PHY so that it can map lanes to the orientation the
*  cable is actually plugged in with.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _USB_TYPE_C_PORT_PROTOCOL_H_
#define _USB_TYPE_C_PORT_PROTOCOL_H_

#define USB_TYPE_C_PORT_PROTOCOL_GUID   \
    { 0x2f1a8c6d, 0x74b9, 0x4e05, { 0x8c, 0x3b, 0x41, 0x9e, 0x6d, 0x2a, 0xb7, 0x50 } }

typedef struct _USB_TYPE_C_PORT_PROTOCOL USB_TYPE_C_PORT_PROTOCOL;

typedef enum {
  //
  // Nothing attached, or the orientation could not be determined.
  //
  UsbTypeCOrientationNone,
  //
  // CC1 is the live pin: the plug is in its "normal" orientation.
  //
  UsbTypeCOrientationNormal,
  //
  // CC2 is the live pin: the plug is flipped.
  //
  UsbTypeCOrientationFlipped
} USB_TYPE_C_ORIENTATION;

/**
  Return the orientation of the plug currently in the port.

  @param[in]  This          Protocol instance.
  @param[out] Orientation   Receives the detected orientation.

  @retval EFI_SUCCESS       Orientation was determined.
  @retval EFI_NOT_READY     Nothing is attached to the port.
  @retval EFI_DEVICE_ERROR  The port controller could not be reached.

**/
typedef
EFI_STATUS
(EFIAPI *USB_TYPE_C_PORT_GET_ORIENTATION)(
  IN  USB_TYPE_C_PORT_PROTOCOL   *This,
  OUT USB_TYPE_C_ORIENTATION     *Orientation
  );

struct _USB_TYPE_C_PORT_PROTOCOL {
  USB_TYPE_C_PORT_GET_ORIENTATION    GetOrientation;
  //
  // Identifies the USBDP combo PHY this port is wired to, matching
  // DP_PHY_PROTOCOL.Id.
  //
  UINT32                             PhyId;
};

extern EFI_GUID  gUsbTypeCPortProtocolGuid;

#endif // _USB_TYPE_C_PORT_PROTOCOL_H_
