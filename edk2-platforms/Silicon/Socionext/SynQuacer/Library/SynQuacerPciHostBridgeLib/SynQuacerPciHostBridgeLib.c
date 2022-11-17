/** @file
  PCI Host Bridge Library instance for Socionext SynQuacer ARM SOC

  Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Platform/Pcie.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Protocol/PciRootBridgeIo.h>

#pragma pack(1)
typedef struct {
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
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)(sizeof(ACPI_HID_DEVICE_PATH)),
          (UINT8)(sizeof(ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCI Express
      1
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        END_DEVICE_PATH_LENGTH,
        0
      }
    }
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED
CHAR16 *mPciHostBridgeLibAcpiAddressSpaceTypeStr[] = {
  L"Mem", L"I/O", L"Bus"
};

#ifndef MDE_CPU_ARM
#define PCI_ALLOCATION_ATTRIBUTES       EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM | \
                                        EFI_PCI_HOST_BRIDGE_MEM64_DECODE
#else
#define PCI_ALLOCATION_ATTRIBUTES       EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM
#endif

PCI_ROOT_BRIDGE mPciRootBridges[] = {
  {
    0,                                      // Segment
    0,                                      // Supports
    0,                                      // Attributes
    TRUE,                                   // DmaAbove4G
    FALSE,                                  // NoExtendedConfigSpace
    FALSE,                                  // ResourceAssigned
    PCI_ALLOCATION_ATTRIBUTES,              // AllocationAttributes
    { SYNQUACER_PCI_SEG0_BUSNUM_MIN,
      SYNQUACER_PCI_SEG0_BUSNUM_MAX },      // Bus
    { SYNQUACER_PCI_SEG0_PORTIO_MIN,
      SYNQUACER_PCI_SEG0_PORTIO_MAX,
      MAX_UINT64 - SYNQUACER_PCI_SEG0_PORTIO_OFFSET + 1 },   // Io
    { SYNQUACER_PCI_SEG0_MMIO32_MIN,
      SYNQUACER_PCI_SEG0_MMIO32_MAX,
      MAX_UINT64 - SYNQUACER_PCI_SEG0_MMIO32_XLATE + 1 },    // Mem
#ifndef MDE_CPU_ARM
    { SYNQUACER_PCI_SEG0_MMIO64_MIN,
      SYNQUACER_PCI_SEG0_MMIO64_MAX },      // MemAbove4G
#else
    { MAX_UINT64, 0x0 },                    // MemAbove4G
#endif
    { MAX_UINT64, 0x0 },                    // PMem
    { MAX_UINT64, 0x0 },                    // PMemAbove4G
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[0]
  }, {
    1,                                      // Segment
    0,                                      // Supports
    0,                                      // Attributes
    TRUE,                                   // DmaAbove4G
    FALSE,                                  // NoExtendedConfigSpace
    FALSE,                                  // ResourceAssigned
    PCI_ALLOCATION_ATTRIBUTES,              // AllocationAttributes
    { SYNQUACER_PCI_SEG1_BUSNUM_MIN,
      SYNQUACER_PCI_SEG1_BUSNUM_MAX },      // Bus
    { SYNQUACER_PCI_SEG1_PORTIO_MIN,
      SYNQUACER_PCI_SEG1_PORTIO_MAX,
      MAX_UINT64 - SYNQUACER_PCI_SEG1_PORTIO_OFFSET + 1 },   // Io
    { SYNQUACER_PCI_SEG1_MMIO32_MIN,
      SYNQUACER_PCI_SEG1_MMIO32_MAX,
      MAX_UINT64 - SYNQUACER_PCI_SEG1_MMIO32_XLATE + 1 },    // Mem
#ifndef MDE_CPU_ARM
    { SYNQUACER_PCI_SEG1_MMIO64_MIN,
      SYNQUACER_PCI_SEG1_MMIO64_MAX },      // MemAbove4G
#else
    { MAX_UINT64, 0x0 },                    // MemAbove4G
#endif
    { MAX_UINT64, 0x0 },                    // PMem
    { MAX_UINT64, 0x0 },                    // PMemAbove4G
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[1]
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
  switch (PcdGet8 (PcdPcieEnableMask)) {
  default:
      ASSERT (FALSE);
  case 0x0:
    *Count = 0;
    return NULL;
  case 0x1:
  case 0x2:
    *Count = 1;
    return &mPciRootBridges[PcdGet8 (PcdPcieEnableMask) - 1];
  case 0x3:
    *Count = ARRAY_SIZE (mPciRootBridges);
    return mPciRootBridges;
  }
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
