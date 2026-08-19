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

//
// The power contract in force on the port.
//
typedef struct {
  //
  // TRUE when a USB Power Delivery contract was negotiated. When FALSE the
  // remaining fields describe what Type-C current advertisement alone allows,
  // which is what a port without PD is limited to.
  //
  BOOLEAN    PdNegotiated;
  UINT32     VoltageMv;
  UINT32     CurrentMa;
} USB_TYPE_C_POWER_CONTRACT;

/**
  Return the power contract currently in force on the port.

  @param[in]  This        Protocol instance.
  @param[out] Contract    Receives the contract.

  @retval EFI_SUCCESS     Contract returned.
  @retval EFI_NOT_READY   Nothing is attached to the port.

**/
typedef
EFI_STATUS
(EFIAPI *USB_TYPE_C_PORT_GET_POWER_CONTRACT)(
  IN  USB_TYPE_C_PORT_PROTOCOL     *This,
  OUT USB_TYPE_C_POWER_CONTRACT    *Contract
  );

//
// State of DisplayPort Alternate Mode on the port.
//
typedef struct {
  //
  // The partner advertises the DisplayPort SVID and we successfully entered
  // the mode.
  //
  BOOLEAN    Entered;
  //
  // A display is present downstream of the partner. Over USB-C this is the
  // only source of hot-plug detect: there is no HPD wire, the state arrives in
  // a DisplayPort Status VDM.
  //
  BOOLEAN    HpdAsserted;
  //
  // Pin assignment selected for the link, as the DisplayPort Alt Mode
  // specification numbers them. C and E carry four DisplayPort lanes; D
  // carries two alongside USB 3.
  //
  UINT8      PinAssignment;
  //
  // DisplayPort lanes the selected pin assignment gives us, either 2 or 4.
  //
  UINT8      DpLanes;
} USB_TYPE_C_DP_ALT_MODE;

#define USB_TYPE_C_DP_PIN_ASSIGN_C  BIT2
#define USB_TYPE_C_DP_PIN_ASSIGN_D  BIT3
#define USB_TYPE_C_DP_PIN_ASSIGN_E  BIT4

/**
  Return the DisplayPort Alternate Mode state of the port.

  @param[in]  This        Protocol instance.
  @param[out] AltMode     Receives the state.

  @retval EFI_SUCCESS       State returned.
  @retval EFI_UNSUPPORTED   The partner does not do DisplayPort Alt Mode.
  @retval EFI_NOT_READY     Nothing is attached to the port.

**/
typedef
EFI_STATUS
(EFIAPI *USB_TYPE_C_PORT_GET_DP_ALT_MODE)(
  IN  USB_TYPE_C_PORT_PROTOCOL   *This,
  OUT USB_TYPE_C_DP_ALT_MODE     *AltMode
  );

struct _USB_TYPE_C_PORT_PROTOCOL {
  USB_TYPE_C_PORT_GET_ORIENTATION       GetOrientation;
  USB_TYPE_C_PORT_GET_POWER_CONTRACT    GetPowerContract;
  USB_TYPE_C_PORT_GET_DP_ALT_MODE       GetDpAltMode;
  //
  // Identifies the USBDP combo PHY this port is wired to, matching
  // DP_PHY_PROTOCOL.Id.
  //
  UINT32                             PhyId;
};

extern EFI_GUID  gUsbTypeCPortProtocolGuid;

#endif // _USB_TYPE_C_PORT_PROTOCOL_H_
