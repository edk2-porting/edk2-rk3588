/**
 * @file UsbDescriptors.h
 * @brief Functions to read USB Interface and Capabilities descriptors
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

  /**
  Type of the Direct Framebuffer capability descriptor.
  This is a vendor specific USB descriptor for DisplayLink.
  @see NR-140082 Section 3.5
  **/
#define DESCRIPTOR_TYPE_DIRECTFB_CAPABILITY 0x5e

  /**
  Identifiers for capabllity keys
  @see NR-140082 Section 3.2
  **/

  /**
  Key for Capabilities 1 - See section 3.2.1
  **/
#define CAPABILITIES1_KEY 0x0

  /**
  Lengths for capabllity fields
  **/
#define CAPABILITIES1_LENGTH 0x4

  /**
  Bits for the capability bitmask Capabilities1
  **/

  /**
  This is the first capability defined for the protocol.
  It represents the mode of operation as of initial release.
  If a device ever breaks compatibility with this initial release,
  it will cease
  to support CapabilityBaseProtocol.
  **/
#define CAPABILITIES1_BASE_PROTOCOL (1 << 0)

  /**
  Idealised VendorDescriptor which is the result
  of parsing vendor descriptor from device.
  **/
  typedef struct {
    UINT32 Capabilities1;
  } VendorDescriptor;

#pragma pack(push, 1)
  typedef struct {
    UINT16 Key; /** Really of type enum DescrptorKeys */
    UINT8 Length;
    UINT8 Value[];
  } DescriptorKLV;

  typedef struct {
    UINT8 Length;
    UINT8 Type;
    UINT16 CapabilityVersion;
    UINT8 CapabilityLength;
    UINT8 Klv[];
  } VendorDescriptorGeneric;
#pragma pack(pop)


EFI_STATUS UsbDisplayLinkGetInterfaceDescriptor (
    IN EFI_USB_IO_PROTOCOL *UsbIo,
    EFI_USB_INTERFACE_DESCRIPTOR* InterfaceDescriptor,
    UINT8 index
    );

EFI_STATUS ReadCapabilitiesDescriptor (IN EFI_USB_IO_PROTOCOL *UsbIo, VOID* Buffer, UINT16 Length);

/**
Parse data in buffer to a VendorDescriptor, if possible.

@param Data      Buffer
@param Length    Length of buffer

@retval EFI_SUCCESS      The descriptor was parsed successfully
@retval EFI_UNSUPPORTED  Simple Pointer Protocol is not installed on Controller.
**/
EFI_STATUS
UsbDisplayLinkParseCapabilitiesDescriptor (
  CONST IN VOID* Data,
  IN UINTN Length,
  OUT VendorDescriptor* Descriptor
);

/**
Decide if binding may proceed, given capabilities

@retval TRUE   Binding may proceed
@retval FALSE  Binding is not possible
**/
BOOLEAN
UsbDisplayLinkCapabilitiesSufficientToBind (
  CONST IN VendorDescriptor* Descriptor
);

#endif // USB_DESCRIPTORS_H_
