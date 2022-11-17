/** @file CapabilityDescriptor.c
 * @brief Definitions for reading USB capability descriptor DisplayLink dock
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#include "UsbDisplayLink.h"
#include "UsbDescriptors.h"

/**
 * Check that the Capability Descriptor is valid and hasn't been tampered with.
 * @param Data Binary data of the Capability Descriptor received from the DisplayLink device
 * @param Length
 * @param out
 * @return
 */
STATIC EFI_STATUS
ValidateHeader (
    CONST IN VOID* Data,
    IN UINTN Length,
    OUT CONST VendorDescriptorGeneric** Out
    )
{
  if (Length < sizeof (VendorDescriptorGeneric)) {
    DEBUG ((DEBUG_ERROR, "Data too short (%d bytes) for capability descriptor header section\n", Length));
    return EFI_INVALID_PARAMETER;
  }
  CONST VendorDescriptorGeneric* Desc = (VendorDescriptorGeneric*)Data;
  if (Desc->Length > Length) {
    DEBUG ((DEBUG_ERROR, "Capability descriptor: Descriptor (%d bytes) exceeds buffer (%d bytes)\n",
          Length, Desc->Length));
    return EFI_BUFFER_TOO_SMALL;
  }
  if (Desc->Type != DESCRIPTOR_TYPE_DIRECTFB_CAPABILITY) {
    DEBUG ((DEBUG_ERROR, "Capability descriptor: invalid type (0x%08x)\n", Desc->Type));
    return EFI_UNSUPPORTED;
  }
  if (Desc->CapabilityVersion != 1) {
    DEBUG ((DEBUG_ERROR, "Capability descriptor: invalid version (%d)\n", Desc->CapabilityVersion));
    return EFI_INCOMPATIBLE_VERSION;
  }
  // Capability length must fit within remaining space
  if (Desc->CapabilityLength > (Desc->Length - (sizeof (Desc->Length) + sizeof (Desc->Type)))) {
    DEBUG ((DEBUG_ERROR, "Capability descriptor: invalid CapabilityLength (%d)\n", Desc->CapabilityLength));
    return EFI_INVALID_PARAMETER;
  }
  *Out = Desc;
  return EFI_SUCCESS;
}


/**
 * Check that the connected DisplayLink device supports the functionality that this driver requires, e.g. video formats
 * @param Data Binary data of the Capability Descriptor received from the DisplayLink device
 * @param Length
 * @param Output
 * @return
 */
EFI_STATUS
UsbDisplayLinkParseCapabilitiesDescriptor (
    CONST IN VOID* Data,
    IN UINTN Length,
    OUT VendorDescriptor* Output
    )
{
  CONST VendorDescriptorGeneric* Desc;
  EFI_STATUS Status;

  Desc = NULL;
  Status = ValidateHeader (Data, Length, &Desc);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Default capabilities
  Output->Capabilities1 = 0;

  CONST UINTN CapsHeaderLength = sizeof (Desc->CapabilityVersion) + sizeof (Desc->CapabilityLength);
  ASSERT (CapsHeaderLength < MAX_UINT8);

  UINTN DataRemaining;
  UINTN Offset;

  DataRemaining = Desc->CapabilityLength - CapsHeaderLength;
  Offset = 0;

  while (DataRemaining >= sizeof (DescriptorKLV)) {
    CONST DescriptorKLV* KeyHeader = (CONST DescriptorKLV*)(Desc->Klv + Offset);
    CONST UINTN KeyValueSize = sizeof (DescriptorKLV) + KeyHeader->Length;
    if (KeyValueSize > DataRemaining) {
      DEBUG ((DEBUG_ERROR, "Capability descriptor: invalid value Length (%d)\n", Desc->CapabilityLength));
      return EFI_INVALID_PARAMETER;
    }

    switch (KeyHeader->Key) {
      case CAPABILITIES1_KEY: {
        if (KeyHeader->Length != CAPABILITIES1_LENGTH) {
          DEBUG ((DEBUG_ERROR, "Capability descriptor: invalid length (%d) for Capabilities 1\n", KeyHeader->Length));
          return EFI_INVALID_PARAMETER;
        }
        Output->Capabilities1 = *(UINT32*)KeyHeader->Value;
        break;
      default:
        // Ignore unknown types
        break;
      }
    }
    DataRemaining -= KeyValueSize;
    Offset += KeyValueSize;
  }
  return EFI_SUCCESS;
}


/**
 * Check that the DisplayLink device supports the basic level of functionality to display GOP pixels.
 * @param Descriptor The USB descriptor received from the DisplayLink device
 * @return True we can bind, False we can't
 */
BOOLEAN
UsbDisplayLinkCapabilitiesSufficientToBind (
    CONST IN VendorDescriptor* Descriptor
    )
{
  BOOLEAN Sufficient;
  Sufficient = (BOOLEAN)(Descriptor->Capabilities1 & CAPABILITIES1_BASE_PROTOCOL);

  if (Sufficient == FALSE) {
    DEBUG ((DEBUG_ERROR, "DisplayLink device does not report support for base capabilites - reports x%x, required x%x\n", Descriptor->Capabilities1 & CAPABILITIES1_BASE_PROTOCOL));
  }
  return Sufficient;
}

