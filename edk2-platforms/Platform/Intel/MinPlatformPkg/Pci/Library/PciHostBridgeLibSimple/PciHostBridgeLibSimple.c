/** @file
  PciHostBridge Library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiDxe.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PciHostBridgeLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED CHAR16 *mPciHostBridgeLibAcpiAddressSpaceTypeStr[] = {
  L"Mem", L"I/O", L"Bus"
};
ACPI_HID_DEVICE_PATH mRootBridgeDeviceNodeTemplate = {
  {
    ACPI_DEVICE_PATH,
    ACPI_DP,
    {
      (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
      (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
    }
  },
  EISA_PNP_ID (0x0A03),
  0
};

PCI_ROOT_BRIDGE mRootBridgeTemplate = {
  0,
  EFI_PCI_ATTRIBUTE_ISA_MOTHERBOARD_IO |
  EFI_PCI_ATTRIBUTE_IDE_PRIMARY_IO |
  EFI_PCI_ATTRIBUTE_ISA_IO |
  EFI_PCI_ATTRIBUTE_ISA_IO_16 |
  EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO |
  EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO_16 |
  EFI_PCI_ATTRIBUTE_VGA_MEMORY |
  EFI_PCI_ATTRIBUTE_VGA_IO |
  EFI_PCI_ATTRIBUTE_VGA_IO_16, // Supports;
  0, // Attributes;
  FALSE, // DmaAbove4G;
  FALSE, // NoExtendedConfigSpace;
  FALSE, // ResourceAssigned;
  EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM, // AllocationAttributes
  { 0, 255 }, // Bus
  { 0, 0 }, // Io - to be fixed later
  { 0, 0 }, // Mem - to be fixed later
  { 0, 0 }, // MemAbove4G - to be fixed later
  { 0, 0 }, // PMem - COMBINE_MEM_PMEM indicating no PMem and PMemAbove4GB
  { 0, 0 }, // PMemAbove4G
  NULL // DevicePath;
};

/**
  Return all the root bridge instances.

  @param Count  Return the count of root bridge instances.

  @return All the root bridge instances, it will be NULL if system has insufficient memory
          resources available and count will be zero.
**/

PCI_ROOT_BRIDGE *
EFIAPI
PciHostBridgeGetRootBridges (
  UINTN                                 *Count
  )
{
  UINT8 Index;
  PCI_ROOT_BRIDGE *RootBridge;

  RootBridge = AllocateZeroPool (sizeof (PCI_ROOT_BRIDGE) * PcdGet8 (PcdPciSegmentCount));
  if (RootBridge == NULL) {
    DEBUG ((DEBUG_ERROR, "PciHostBridge: Out of resource\n"));
    *Count = 0;
    return RootBridge;
  }

  mRootBridgeTemplate.Mem.Base = PcdGet32 (PcdPciReservedMemBase);
  if (PcdGet32(PcdPciReservedMemLimit) != 0) {
    mRootBridgeTemplate.Mem.Limit = PcdGet32 (PcdPciReservedMemLimit);
  } else {
    mRootBridgeTemplate.Mem.Limit = (UINT32) PcdGet64 (PcdPciExpressBaseAddress) - 1;
  }

  mRootBridgeTemplate.MemAbove4G.Base = PcdGet64 (PcdPciReservedMemAbove4GBBase);
  mRootBridgeTemplate.MemAbove4G.Limit = PcdGet64 (PcdPciReservedMemAbove4GBLimit);
  
  mRootBridgeTemplate.PMem.Base = PcdGet32 (PcdPciReservedPMemBase);
  mRootBridgeTemplate.PMem.Limit = PcdGet32 (PcdPciReservedPMemLimit);
  mRootBridgeTemplate.PMemAbove4G.Base = PcdGet64 (PcdPciReservedPMemAbove4GBBase);
  mRootBridgeTemplate.PMemAbove4G.Limit = PcdGet64 (PcdPciReservedPMemAbove4GBLimit);

  if (mRootBridgeTemplate.MemAbove4G.Base < mRootBridgeTemplate.MemAbove4G.Limit) {
    mRootBridgeTemplate.AllocationAttributes |= EFI_PCI_HOST_BRIDGE_MEM64_DECODE;
  }

  mRootBridgeTemplate.Io.Base = PcdGet16 (PcdPciReservedIobase);
  mRootBridgeTemplate.Io.Limit = PcdGet16 (PcdPciReservedIoLimit);

  mRootBridgeTemplate.DmaAbove4G = PcdGetBool (PcdPciDmaAbove4G);
  mRootBridgeTemplate.NoExtendedConfigSpace = PcdGetBool (PcdPciNoExtendedConfigSpace);
  mRootBridgeTemplate.ResourceAssigned = PcdGetBool (PcdPciResourceAssigned);

  for (Index = 0; Index < PcdGet8 (PcdPciSegmentCount); Index ++) {
    mRootBridgeDeviceNodeTemplate.UID = Index;
    mRootBridgeTemplate.Segment = Index;
    mRootBridgeTemplate.DevicePath = NULL;
    mRootBridgeTemplate.DevicePath = AppendDevicePathNode (NULL, &mRootBridgeDeviceNodeTemplate.Header);
    CopyMem (RootBridge + Index, &mRootBridgeTemplate, sizeof (PCI_ROOT_BRIDGE));
  }

  *Count = PcdGet8 (PcdPciSegmentCount);
  return RootBridge;
}

VOID
EFIAPI
PciHostBridgeFreeRootBridges (
  PCI_ROOT_BRIDGE *Bridges,
  UINTN           Count
  )
{
  ASSERT (Count <= PcdGet8 (PcdPciSegmentCount));
  FreePool (Bridges->DevicePath);
}

/**
  Inform the platform that the resource conflict happens.

  @param HostBridgeHandle Handle of the Host Bridge.
  @param Configuration    Pointer to PCI I/O and PCI memory resource descriptors.
                          The Configuration contains the resources for all the
                          root bridges. The resource for each root bridge is
                          terminated with END descriptor and an additional END
                          is appended indicating the end of the whole resources.
                          The resource descriptor field values follow the description
                          in EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL.SubmitResources().
**/

VOID
EFIAPI
PciHostBridgeResourceConflict (
  EFI_HANDLE                        HostBridgeHandle,
  VOID                              *Configuration
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *Descriptor;
  UINTN                             RootBridgeIndex;
  DEBUG ((DEBUG_ERROR, "PciHostBridge: Resource conflict happens!\n"));

  RootBridgeIndex = 0;
  Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Configuration;
  while (Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    DEBUG ((DEBUG_ERROR, "RootBridge[%d]:\n", RootBridgeIndex++));
    for (; Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR; Descriptor++) {
      ASSERT (Descriptor->ResType <
              sizeof (mPciHostBridgeLibAcpiAddressSpaceTypeStr) / sizeof (mPciHostBridgeLibAcpiAddressSpaceTypeStr[0])
              );
      DEBUG ((DEBUG_ERROR, " %s: Length/Alignment = 0x%lx / 0x%lx\n",
              mPciHostBridgeLibAcpiAddressSpaceTypeStr[Descriptor->ResType], Descriptor->AddrLen, Descriptor->AddrRangeMax));
      if (Descriptor->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        DEBUG ((DEBUG_ERROR, "     Granularity/SpecificFlag = %ld / %02x%s\n",
                Descriptor->AddrSpaceGranularity, Descriptor->SpecificFlag,
                ((Descriptor->SpecificFlag & EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_CACHEABLE_PREFETCHABLE) != 0) ? L" (Prefetchable)" : L""
                ));
      }
    }
    //
    // Skip the END descriptor for root bridge
    //
    ASSERT (Descriptor->Desc == ACPI_END_TAG_DESCRIPTOR);
    Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) ((EFI_ACPI_END_TAG_DESCRIPTOR *) Descriptor + 1);
  }

  ASSERT (FALSE);
}
