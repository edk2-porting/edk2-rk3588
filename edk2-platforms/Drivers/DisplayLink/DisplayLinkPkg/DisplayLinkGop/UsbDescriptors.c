/**
 * @file UsbDescriptors.c
 * @brief Functions to read USB Interface and Capabilities descriptors
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#include "UsbDisplayLink.h"
#include "UsbDescriptors.h"

/**
 *
 * @param UsbIo
 * @param descriptorType
 * @param index
 * @param Buffer
 * @param Length
 * @param UsbStatus
 * @return
 */
STATIC EFI_STATUS
ReadDescriptor (
    IN EFI_USB_IO_PROTOCOL *UsbIo,
    UINT8 DescriptorType,
    UINT8 Index,
    VOID* Buffer,
    UINT16 Length,
    UINT32* UsbStatus)
{
  EFI_STATUS Status;

  UINT8 Header[2];
  ZeroMem (Header, sizeof (Header));

  EFI_USB_DEVICE_REQUEST Request;
  ZeroMem (&Request, sizeof (Request));
  Request.RequestType = USB_ENDPOINT_DIR_IN | USB_REQ_TYPE_STANDARD | USB_TARGET_DEVICE;
  Request.Request = USB_REQ_GET_DESCRIPTOR;
  Request.Index = 0;
  Request.Value = DescriptorType << 8 | Index;
  Request.Length = sizeof (Header);

  // Read the descriptor header to see how many bytes it contains
  Status = UsbIo->UsbControlTransfer (
      UsbIo,
      &Request,
      EfiUsbDataIn,
      DISPLAYLINK_USB_CTRL_TIMEOUT,
      Header,
      sizeof (Header),
      UsbStatus);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read length of descriptor type x%x, index %u (code %r, USB status x%x)\n",
          DescriptorType, Index, Status, *UsbStatus));
    return Status;
  }
  CONST UINT16 TotalLength = Header[0];

  // Now we know the size of it, we can read the entire descriptor
  Request.Length = TotalLength;

  Status = UsbIo->UsbControlTransfer (
      UsbIo,
      &Request,
      EfiUsbDataIn,
      DISPLAYLINK_USB_CTRL_TIMEOUT,
      Buffer,
      TotalLength,
      UsbStatus);

  return Status;
}

/**
  Perform a USB control transfer to read the DisplayLink vendor descriptor.

  @param UsbIo   Pointer to the instance of the USBIO protocol
  @param Buffer  Pointer to the buffer where descriptor should be written
  @param Length  Length of buffer (and the maximum amount of descriptor data that shall be read)

  @retval EFI_SUCCESS  The descriptor has been copied into Buffer
  @retval Other        The descriptor could not be read
**/
EFI_STATUS
ReadCapabilitiesDescriptor (
    IN EFI_USB_IO_PROTOCOL *UsbIo,
    OUT VOID* Buffer,
    IN UINT16 Length)
{
  UINT32 UsbStatus;
  EFI_STATUS Status;

  Status = ReadDescriptor (
      UsbIo,
      DESCRIPTOR_TYPE_DIRECTFB_CAPABILITY,
      0,
      Buffer,
      Length,
      &UsbStatus);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Could not read capabilities descriptor from DL device (code %r, USB status x%x). Unrecognised firmware version?\n", Status, UsbStatus));
  }

  return Status;
}


/**
 An alternative to the UBSIO protocol function EFI_USB_IO_GET_INTERFACE_DESCRIPTOR.
 This version allows you to specify an index.
 * @param UsbIo                 Pointer to the instance of the USBIO protocol
 * @param interfaceDescriptor   Where the descriptor should be written
 * @param index                 The index of the descriptor required (the standard USBIO function doesn't let you do this)
 * @return
 */
EFI_STATUS
UsbDisplayLinkGetInterfaceDescriptor (
    IN EFI_USB_IO_PROTOCOL *UsbIo,
    OUT EFI_USB_INTERFACE_DESCRIPTOR* InterfaceDescriptor,
    UINT8 Index
    )
{
  UINT32 UsbStatus;
  EFI_STATUS Status;

  Status = ReadDescriptor (
      UsbIo,
      USB_DESC_TYPE_INTERFACE,
      Index,
      InterfaceDescriptor,
      sizeof (EFI_USB_INTERFACE_DESCRIPTOR),
      &UsbStatus);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "USB control transfer failed while reading interface descriptor (code %r, USB status x%x)\n", Status, UsbStatus));
  }

  return Status;
}

