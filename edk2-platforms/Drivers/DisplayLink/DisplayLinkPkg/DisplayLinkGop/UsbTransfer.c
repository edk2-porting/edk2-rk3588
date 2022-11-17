/**
 * @file UsbTransfer.c
 * @brief Wrapper of UEFI USB bulk and control transfer interface for USB DisplayLink driver.
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#include "UsbDisplayLink.h"

/**
 * Write the data to the DisplayLink device using the USBIO protocol.
 * @param UsbDisplayLinkDev
 * @param Buffer
 * @param DataLen
 * @param USBStatus
 * @return
  * EFI_SUCCESS   The bulk transfer has been successfully executed.
  * EFI_INVALID_PARAMETER   If DeviceEndpoint is not valid.
  * EFI_INVALID_PARAMETER   Data is NULL.
  * EFI_INVALID_PARAMETER   DataLength is NULL.
  * EFI_INVALID_PARAMETER   Status is NULL.
  * EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  * EFI_TIMEOUT   The bulk transfer cannot be completed within Timeout timeframe.
  * EFI_DEVICE_ERROR  The transfer failed other than timeout, and the transfer status is returned in Status.
 */
EFI_STATUS
DlUsbBulkWrite (
    IN USB_DISPLAYLINK_DEV* UsbDisplayLinkDev,
    IN CONST UINT8* Buffer,
    IN UINTN DataLen,
    OUT UINT32 *USBStatus
    )
{
  EFI_STATUS Status;
  Status = UsbDisplayLinkDev->UsbIo->UsbBulkTransfer (
    UsbDisplayLinkDev->UsbIo,
    UsbDisplayLinkDev->BulkOutEndpointDescriptor.EndpointAddress,
    (VOID*)Buffer,
    &DataLen,
    DISPLAYLINK_USB_BULK_TIMEOUT,
    USBStatus);

  return Status;
}

/**
* Read data from the DisplayLink device using the USBIO protocol.
* @param UsbDisplayLinkDev
* @param Buffer
* @param BufferLen
* @return 0 if an error occurred or 0 bytes were read, otherwise the number of bytes read
 */
UINTN
DlUsbBulkRead (
    IN USB_DISPLAYLINK_DEV* UsbDisplayLinkDev,
    IN UINT8* Buffer,
    IN UINTN BufferLen
    )
{
  UINT32 Result;
  UINTN ReadLen;
  EFI_STATUS Status;

  ReadLen = BufferLen;

  Status = UsbDisplayLinkDev->UsbIo->UsbBulkTransfer (
    UsbDisplayLinkDev->UsbIo,
    UsbDisplayLinkDev->BulkInEndpointDescriptor.EndpointAddress,
    Buffer,
    &ReadLen,
    DISPLAYLINK_USB_BULK_TIMEOUT,
    &Result);

  if (EFI_ERROR (Status)) {
    return 0;
  }

  return ReadLen;
}


/**
Send a control message (e.g set video mode) message to the DisplayLink device.

@param  Device                   USB device handle.
@param  request                  Request type, e.g. SET_VIDEO_MODE
@param  value
@param  controlMsg               Pointer to the message to send.
@param  controlMsgLen            Length of the message.

@retval EFI_SUCCESS            Successfully sent message.

**/
EFI_STATUS
DlUsbSendControlWriteMessage (
  IN USB_DISPLAYLINK_DEV *Device,
  IN UINT8 Request,
  IN UINT16 Value,
  IN CONST VOID *ControlMsg,
  IN UINT16 ControlMsgLen
  )
{
  EFI_STATUS             Status;
  UINT32                 UsbStatus;
  EFI_USB_DEVICE_REQUEST UsbRequest;

  ZeroMem (&UsbRequest, sizeof (UsbRequest));
  UsbRequest.RequestType = USB_REQ_TYPE_VENDOR | USB_TARGET_INTERFACE;
  UsbRequest.Index = Device->InterfaceDescriptor.InterfaceNumber;
  UsbRequest.Request = Request;
  UsbRequest.Value = Value;
  UsbRequest.Length = ControlMsgLen;

  Status = Device->UsbIo->UsbControlTransfer (
    Device->UsbIo,
    &UsbRequest,
    EfiUsbDataOut,
    DISPLAYLINK_USB_CTRL_TIMEOUT,
    (VOID *)ControlMsg,
    ControlMsgLen,
    &UsbStatus);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "USB write control transfer failed - %r (USB status x%x).\n", Status, UsbStatus));
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}


/**
Request data from the DisplayLink device (e.g. the monitor EDID)

@param  Device                   USB device handle.
@param  request                  Request type, e.g. GET_OUTPUT_EDID
@param  value
@param  controlMsg               Pointer to the message to send.
@param  controlMsgLen            Length of the message.

@retval EFI_SUCCESS            Successfully sent message.

**/
EFI_STATUS
DlUsbSendControlReadMessage (
  IN USB_DISPLAYLINK_DEV *Device,
  IN UINT8 Request,
  IN UINT16 Value,
  OUT VOID *ControlMsg,
  IN UINT16 ControlMsgLen
  )
{
  EFI_STATUS             Status;
  UINT32                 UsbStatus;
  EFI_USB_DEVICE_REQUEST UsbRequest;

  ZeroMem (&UsbRequest, sizeof (UsbRequest));
  UsbRequest.RequestType = USB_REQ_TYPE_VENDOR | USB_TARGET_INTERFACE | USB_ENDPOINT_DIR_IN;
  UsbRequest.Request = Request;
  UsbRequest.Value = Value;
  UsbRequest.Index = Device->InterfaceDescriptor.InterfaceNumber;
  UsbRequest.Length = ControlMsgLen;

  Status = Device->UsbIo->UsbControlTransfer (
    Device->UsbIo,
    &UsbRequest,
    EfiUsbDataIn,
    DISPLAYLINK_USB_CTRL_TIMEOUT,
    (VOID *)ControlMsg,
    ControlMsgLen,
    &UsbStatus);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "USB read control transfer failed - %r (USB status x%x).\n", Status, UsbStatus));
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}
