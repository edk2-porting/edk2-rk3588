/** @file
 *
 * PCI Host Bridge Library instance for Bcm2711 ARM SOC
 *
 * Copyright (c) 2019, Jeremy Linton
 * Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/Bcm2711.h>
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PciHostBridgeLib.h>
#include <PiDxe.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>

#pragma pack(1)

typedef PACKED struct {
  ACPI_HID_DEVICE_PATH     AcpiDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_PCI_ROOT_BRIDGE_DEVICE_PATH;

#pragma pack ()

STATIC CONST EFI_PCI_ROOT_BRIDGE_DEVICE_PATH mEfiPciRootBridgeDevicePath[] = {
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCI Express
      0
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        END_DEVICE_PATH_LENGTH,
        0
      }
    }
  },
};

GLOBAL_REMOVE_IF_UNREFERENCED
CHAR16 *mPciHostBridgeLibAcpiAddressSpaceTypeStr[] = {
  L"Mem", L"I/O", L"Bus"
};

// These should come from the PCD...
#define BCM2711_PCI_SEG0_BUSNUM_MIN     0x00
#define BCM2711_PCI_SEG0_BUSNUM_MAX     0xFF
#define BCM2711_PCI_SEG0_PORTIO_MIN     0x01
#define BCM2711_PCI_SEG0_PORTIO_MAX     0x00 // MIN>MAX disables PIO
#define BCM2711_PCI_SEG0_PORTIO_OFFSET  0x00
// The bridge thinks its MMIO is here (which means it can't access this area in phy ram)
#define BCM2711_PCI_SEG0_MMIO32_MIN     PCIE_TOP_OF_MEM_WIN
#define BCM2711_PCI_SEG0_MMIO32_MAX     (PCIE_TOP_OF_MEM_WIN + PCIE_BRIDGE_MMIO_LEN)
// The CPU views it via a window here..
#define BCM2711_PCI_SEG0_MMIO32_XLATE   (PCIE_CPU_MMIO_WINDOW - PCIE_TOP_OF_MEM_WIN)

// We might be able to size another region?
#define BCM2711_PCI_SEG0_MMIO64_MIN     0x00
#define BCM2711_PCI_SEG0_MMIO64_MAX     0x00

//
// See description in MdeModulePkg/Include/Library/PciHostBridgeLib.h
//
PCI_ROOT_BRIDGE mPciRootBridges[] = {
  {
    0,                                      // Segment
    0,                                      // Supports
    0,                                      // Attributes
    FALSE,                                  // DmaAbove4G
    FALSE,                                  // NoExtendedConfigSpace (true=256 byte config, false=4k)
    FALSE,                                  // ResourceAssigned
    EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM,   // AllocationAttributes
    { BCM2711_PCI_SEG0_BUSNUM_MIN,
      BCM2711_PCI_SEG0_BUSNUM_MAX },        // Bus
    { BCM2711_PCI_SEG0_PORTIO_MIN,
      BCM2711_PCI_SEG0_PORTIO_MAX,
      MAX_UINT64 - BCM2711_PCI_SEG0_PORTIO_OFFSET + 1 },   // Io
    { BCM2711_PCI_SEG0_MMIO32_MIN,
      BCM2711_PCI_SEG0_MMIO32_MAX,
      MAX_UINT64 - BCM2711_PCI_SEG0_MMIO32_XLATE + 1 },    // Mem
    { MAX_UINT64, 0x0 },                    // MemAbove4G
    { MAX_UINT64, 0x0 },                    // Pefetchable Mem
    { MAX_UINT64, 0x0 },                    // Pefetchable MemAbove4G
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[0]
  }
};

/**
  Return all the root bridge instances in an array.

  @param Count  Return the count of root bridge instances.

  @return All the root bridge instances in an array.
          The array should be passed into PciHostBridgeFreeRootBridges()
          when it's not used.
**/
PCI_ROOT_BRIDGE *
EFIAPI
PciHostBridgeGetRootBridges (
  OUT UINTN     *Count
  )
{
    *Count = ARRAY_SIZE (mPciRootBridges);
    return mPciRootBridges;
}

/**
  Free the root bridge instances array returned from PciHostBridgeGetRootBridges().

  @param Bridges The root bridge instances array.
  @param Count   The count of the array.
**/
VOID
EFIAPI
PciHostBridgeFreeRootBridges (
  PCI_ROOT_BRIDGE *Bridges,
  UINTN           Count
  )
{
}

/**
  Inform the platform that the resource conflict happens.

  @param HostBridgeHandle Handle of the Host Bridge.
  @param Configuration    Pointer to PCI I/O and PCI memory resource
                          descriptors. The Configuration contains the resources
                          for all the root bridges. The resource for each root
                          bridge is terminated with END descriptor and an
                          additional END is appended indicating the end of the
                          entire resources. The resource descriptor field
                          values follow the description in
                          EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
                          .SubmitResources().
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
              ARRAY_SIZE (mPciHostBridgeLibAcpiAddressSpaceTypeStr));
      DEBUG ((DEBUG_ERROR, " %s: Length/Alignment = 0x%lx / 0x%lx\n",
              mPciHostBridgeLibAcpiAddressSpaceTypeStr[Descriptor->ResType],
              Descriptor->AddrLen, Descriptor->AddrRangeMax
              ));
      if (Descriptor->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        DEBUG ((DEBUG_ERROR, "     Granularity/SpecificFlag = %ld / %02x%s\n",
                Descriptor->AddrSpaceGranularity, Descriptor->SpecificFlag,
                ((Descriptor->SpecificFlag &
                  EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_CACHEABLE_PREFETCHABLE
                  ) != 0) ? L" (Prefetchable)" : L""
                ));
      }
    }
    //
    // Skip the END descriptor for root bridge
    //
    ASSERT (Descriptor->Desc == ACPI_END_TAG_DESCRIPTOR);
    Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)(
                   (EFI_ACPI_END_TAG_DESCRIPTOR *)Descriptor + 1
                   );
  }
}
