/** @file

  Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2018, Linaro Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Protocol/PciRootBridgeIo.h>

#define ENUM_HB_NUM 8

#define EFI_PCI_SUPPORT   (EFI_PCI_ATTRIBUTE_ISA_MOTHERBOARD_IO | \
                           EFI_PCI_ATTRIBUTE_IDE_SECONDARY_IO | \
                           EFI_PCI_ATTRIBUTE_IDE_PRIMARY_IO | \
                           EFI_PCI_ATTRIBUTE_ISA_IO_16  | \
                           EFI_PCI_ATTRIBUTE_VGA_MEMORY | \
                           EFI_PCI_ATTRIBUTE_VGA_IO_16  | \
                           EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO_16)

#define EFI_PCI_ATTRIBUTE  EFI_PCI_SUPPORT

#pragma pack(1)
typedef struct {
  ACPI_HID_DEVICE_PATH     AcpiDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_PCI_ROOT_BRIDGE_DEVICE_PATH;
#pragma pack ()

STATIC EFI_PCI_ROOT_BRIDGE_DEVICE_PATH mEfiPciRootBridgeDevicePath [ENUM_HB_NUM] = {
//Host Bridge 0
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID(0x0A03), // PCI
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

//Host Bridge 2
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID(0x0A03), // PCI
      2
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

//Host Bridge 4
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID(0x0A03), // PCI
      4
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

//Host Bridge 5
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID(0x0A03), // PCI
      5
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

//Host Bridge 6
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID(0x0A03), // PCI
      6
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

//Host Bridge 8
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID(0x0A03), // PCI
      8
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

//Host Bridge 10
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID(0x0A03), // PCI
      10
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

//Host Bridge 11
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID(0x0A03), // PCI
      11
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

STATIC PCI_ROOT_BRIDGE gRootBridge [ENUM_HB_NUM] = {
//Host Bridge 0
  {
    0,                                              // Segment
    EFI_PCI_SUPPORT,                                // Supports
    EFI_PCI_ATTRIBUTE,                              // Attributes
    TRUE,                                           // DmaAbove4G
    FALSE,                                          // NoExtendedConfigSpace
    FALSE,                                          // ResourceAssigned
    EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
    { // Bus
      00,
      0x3F
    },
    { // Io (32K)
      0,
      0x7FFF
    },
    { // Mem (256M - 64K - 1)
      0xE0000000,
      0xEFFEFFFF
    },
    { // MemAbove4G (8T + 256G)
      0x80000000000,
      0x83FFFFFFFFF
    },
    { // PMem
      0xE0000000,
      0xEFFEFFFF
    },
    { // PMemAbove4G
      0x80000000000,
      0x83FFFFFFFFF
    },
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[0]
  },

  //Host Bridge 2
  {
    0,                                              // Segment
    EFI_PCI_SUPPORT,                                // Supports
    EFI_PCI_ATTRIBUTE,                              // Attributes
    TRUE,                                           // DmaAbove4G
    FALSE,                                          // NoExtendedConfigSpace
    FALSE,                                          // ResourceAssigned
    EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |          // AllocationAttributes
    EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
    { // Bus
      0x7A,
      0x7A
    },
    { // Io
      MAX_UINT32,
      0
    },
    { // Mem
      MAX_UINT32,
      0
    },
    { // MemAbove4G
      0x20c000000,
      0x20c1fffff
    },
    { // PMem
      MAX_UINT32,
      0
    },
    { // PMemAbove4G
      MAX_UINT64,
      0
    },
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[1]
  },

  //Host Bridge 4
  {
    0,                                              // Segment
    EFI_PCI_SUPPORT,                                // Supports
    EFI_PCI_ATTRIBUTE,                              // Attributes
    TRUE,                                           // DmaAbove4G
    FALSE,                                          // NoExtendedConfigSpace
    FALSE,                                          // ResourceAssigned
    EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |          // AllocationAttributes
    EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
    { // Bus
      0x7C,
      0x7D
    },
    { // Io
      MAX_UINT32,
      0
    },
    { // Mem
      MAX_UINT32,
      0
    },
    { // MemAbove4G
      0x120000000,
      0x13fffffff
    },
    { // PMem
      MAX_UINT32,
      0
    },
    { // PMemAbove4G
      MAX_UINT64,
      0
    },
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[2]
  },

  //Host Bridge 5
  {
    0,                                              // Segment
    EFI_PCI_SUPPORT,                                // Supports
    EFI_PCI_ATTRIBUTE,                              // Attributes
    TRUE,                                           // DmaAbove4G
    FALSE,                                          // NoExtendedConfigSpace
    FALSE,                                          // ResourceAssigned
    EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |          // AllocationAttributes
    EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
    { // Bus
      0x74,
      0x76
    },
    { // Io
      MAX_UINT32,
      0
    },
    { // Mem
      0xA2000000,
      0xA2ffffff
    },
    { // MemAbove4G
      0x144000000,
      0x147ffffff
    },
    { // PMem
      MAX_UINT32,
      0
    },
    { // PMemAbove4G
      MAX_UINT64,
      0
    },
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[3]
  },
  //Host Bridge 6
  {
    0,                                              // Segment
    EFI_PCI_SUPPORT,                                // Supports
    EFI_PCI_ATTRIBUTE,                              // Attributes
    TRUE,                                           // DmaAbove4G
    FALSE,                                          // NoExtendedConfigSpace
    FALSE,                                          // ResourceAssigned
    EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
    { // Bus
      0x80,
      0x9F
    },
    { // Io (32K)
      0x0,
      0x7FFF
    },
    { // Mem (256M - 64K -1)
      0xF0000000,
      0xFFFEFFFF
    },
    { // MemAbove4G (8T + 256G)
      0x480000000000,
      0x483FFFFFFFFF
    },
    { // PMem
      0xF0000000,
      0xFFFEFFFF
    },
    { // PMemAbove4G
      0x480000000000,
      0x483FFFFFFFFF
    },
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[4]
  },

  //Host Bridge 8
  {
    0,                                              // Segment
    EFI_PCI_SUPPORT,                                // Supports
    EFI_PCI_ATTRIBUTE,                              // Attributes
    TRUE,                                           // DmaAbove4G
    FALSE,                                          // NoExtendedConfigSpace
    FALSE,                                          // ResourceAssigned
    EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |          // AllocationAttributes
    EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
    { // Bus
      0xBA,
      0xBA
    },
    { // Io
      MAX_UINT32,
      0
    },
    { // Mem
      MAX_UINT32,
      0
    },
    { // MemAbove4G
      0x40020c000000,
      0x40020c1fffff
    },
    { // PMem
      MAX_UINT32,
      0
    },
    { // PMemAbove4G
      MAX_UINT64,
      0
    },
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[5]
  },

  //Host Bridge 10
  {
    0,                                              // Segment
    EFI_PCI_SUPPORT,                                // Supports
    EFI_PCI_ATTRIBUTE,                              // Attributes
    TRUE,                                           // DmaAbove4G
    FALSE,                                          // NoExtendedConfigSpace
    FALSE,                                          // ResourceAssigned
    EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |          // AllocationAttributes
    EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
    { // Bus
      0xBC,
      0xBD
    },
    { // Io
      MAX_UINT32,
      0
    },
    { // Mem
      MAX_UINT32,
      0
    },
    { // MemAbove4G
      0x400120000000,
      0x40013fffffff
    },
    { // PMem
      MAX_UINT32,
      0
    },
    { // PMemAbove4G
      MAX_UINT64,
      0
    },
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[6]
  },

  //Host Bridge 11
  {
    0,                                              // Segment
    EFI_PCI_SUPPORT,                                // Supports
    EFI_PCI_ATTRIBUTE,                              // Attributes
    TRUE,                                           // DmaAbove4G
    FALSE,                                          // NoExtendedConfigSpace
    FALSE,                                          // ResourceAssigned
    EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |          // AllocationAttributes
    EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
    { // Bus
      0xB4,
      0xB6
    },
    { // Io
      MAX_UINT32,
      0
    },
    { // Mem
      0xA3000000,
      0xA3ffffff
    },
    { // MemAbove4G
      0x400144000000,
      0x400147ffffff
    },
    { // PMem
      MAX_UINT32,
      0
    },
    { // PMemAbove4G
      MAX_UINT64,
      0
    },
    (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[7]
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
  UINTN *Count
  )
{
  *Count = ENUM_HB_NUM;

  return gRootBridge;
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
  if (Bridges == NULL && Count == 0) {
    return;
  }

  do {
    --Count;
    FreePool (Bridges[Count].DevicePath);
  } while (Count > 0);

  FreePool (Bridges);
}

STATIC CONST CHAR16 mPciHostBridgeLibAcpiAddressSpaceTypeStr[][4] = {
  L"Mem", L"I/O", L"Bus"
};

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

  DEBUG ((DEBUG_ERROR, "\n PciHostBridge: Resource conflict happens!\n"));
  RootBridgeIndex = 0;
  Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)Configuration;
  while (Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    DEBUG ((DEBUG_ERROR, "RootBridge[%d]:\n", RootBridgeIndex++));
    for (; Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR; Descriptor++) {
      ASSERT (Descriptor->ResType <
              ARRAY_SIZE (mPciHostBridgeLibAcpiAddressSpaceTypeStr)
              );
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
