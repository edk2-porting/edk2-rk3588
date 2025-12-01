/** @file
  PCI Host Bridge Library instance for Rockchip Rk3588

  Copyright (c) 2023-2025, Mario Bălănică <mariobalanica02@gmail.com>
  Copyright (c) 2023, Molly Sophia <mollysophia379@gmail.com>
  Copyright (c) 2021, Jared McNeill <jmcneill@invisible.ca>
  Copyright (c) 2016, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiDxe.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/Rk3588Pcie.h>

#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>

#include "PciHostBridgeInit.h"

#pragma pack(1)
typedef struct {
  ACPI_HID_DEVICE_PATH        AcpiDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    EndDevicePath;
} EFI_PCI_ROOT_BRIDGE_DEVICE_PATH;
#pragma pack ()

STATIC EFI_PCI_ROOT_BRIDGE_DEVICE_PATH  mEfiPciRootBridgeDevicePath[] = {
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
      EISA_PNP_ID (0x0A08), // PCIe
      PCIE_SEGMENT_PCIE30X4
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
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCIe
      PCIE_SEGMENT_PCIE30X2
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
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCIe
      PCIE_SEGMENT_PCIE20L0
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
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCIe
      PCIE_SEGMENT_PCIE20L1
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
          (UINT8)sizeof (ACPI_HID_DEVICE_PATH),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCIe
      PCIE_SEGMENT_PCIE20L2
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
CHAR16  *mPciHostBridgeLibAcpiAddressSpaceTypeStr[] = {
  L"Mem", L"I/O", L"Bus"
};

PCI_ROOT_BRIDGE  mPciRootBridges[NUM_PCIE_CONTROLLER];

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
  UINTN  *Count
  )
{
  EFI_STATUS  Status;
  UINTN       Idx;
  UINTN       Loop;

  for (Idx = 0, Loop = 0; Idx < NUM_PCIE_CONTROLLER; Idx++) {
    if (IsPcieNumEnabled (Idx) == FALSE) {
      continue;
    }

    Status = InitializePciHost (Idx);
    if (EFI_ERROR (Status)) {
      continue;
    }

    mPciRootBridges[Loop].Segment  = Idx;
    mPciRootBridges[Loop].Supports = EFI_PCI_ATTRIBUTE_IDE_PRIMARY_IO |
                                     EFI_PCI_ATTRIBUTE_IDE_SECONDARY_IO |
                                     EFI_PCI_ATTRIBUTE_ISA_IO_16 |
                                     EFI_PCI_ATTRIBUTE_ISA_MOTHERBOARD_IO | \
                                     EFI_PCI_ATTRIBUTE_VGA_MEMORY | \
                                     EFI_PCI_ATTRIBUTE_VGA_IO_16  | \
                                     EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO_16;
    mPciRootBridges[Loop].Attributes            = mPciRootBridges[Loop].Supports;
    mPciRootBridges[Loop].DmaAbove4G            = TRUE;
    mPciRootBridges[Loop].NoExtendedConfigSpace = FALSE;
    mPciRootBridges[Loop].ResourceAssigned      = FALSE;
    mPciRootBridges[Loop].AllocationAttributes  = EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |
                                                  EFI_PCI_HOST_BRIDGE_MEM64_DECODE;

    mPciRootBridges[Loop].Bus.Base  = PCIE_BUS_BASE (Idx);
    mPciRootBridges[Loop].Bus.Limit = PCIE_BUS_LIMIT (Idx);

    mPciRootBridges[Loop].Io.Base        = PCIE_IO_BUS_BASE;
    mPciRootBridges[Loop].Io.Limit       = mPciRootBridges[Loop].Io.Base + PCIE_IO_SIZE - 1;
    mPciRootBridges[Loop].Io.Translation = MAX_UINT64 - PCIE_IO_TRANSLATION (Idx) + 1;

    mPciRootBridges[Loop].Mem.Base        = PCIE_MEM32_BUS_BASE;
    mPciRootBridges[Loop].Mem.Limit       = mPciRootBridges[Loop].Mem.Base + PCIE_MEM32_SIZE - 1;
    mPciRootBridges[Loop].Mem.Translation = MAX_UINT64 - PCIE_MEM32_TRANSLATION (Idx) + 1;

    mPciRootBridges[Loop].MemAbove4G.Base  = PCIE_MEM64_BASE (Idx);
    mPciRootBridges[Loop].MemAbove4G.Limit = mPciRootBridges[Loop].MemAbove4G.Base + PCIE_MEM64_SIZE - 1;

    mPciRootBridges[Loop].PMem.Base         = MAX_UINT64;
    mPciRootBridges[Loop].PMem.Limit        = 0;
    mPciRootBridges[Loop].PMemAbove4G.Base  = MAX_UINT64;
    mPciRootBridges[Loop].PMemAbove4G.Limit = 0;
    mPciRootBridges[Loop].DevicePath        = (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[Idx];

    Loop++;
  }

  *Count = Loop;
  if (Loop == 0) {
    return NULL;
  }

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
  PCI_ROOT_BRIDGE  *Bridges,
  UINTN            Count
  )
{
  // FreePool (Bridges);
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
  EFI_HANDLE  HostBridgeHandle,
  VOID        *Configuration
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Descriptor;
  UINTN                              RootBridgeIndex;

  DEBUG ((DEBUG_ERROR, "PciHostBridge: Resource conflict happens!\n"));

  RootBridgeIndex = 0;
  Descriptor      = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)Configuration;
  while (Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    DEBUG ((DEBUG_ERROR, "RootBridge[%d]:\n", RootBridgeIndex++));
    for ( ; Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR; Descriptor++) {
      ASSERT (
        Descriptor->ResType <
        (sizeof (mPciHostBridgeLibAcpiAddressSpaceTypeStr) /
         sizeof (mPciHostBridgeLibAcpiAddressSpaceTypeStr[0])
        )
        );
      DEBUG ((
        DEBUG_ERROR,
        " %s: Length/Alignment = 0x%lx / 0x%lx\n",
        mPciHostBridgeLibAcpiAddressSpaceTypeStr[Descriptor->ResType],
        Descriptor->AddrLen,
        Descriptor->AddrRangeMax
        ));
      if (Descriptor->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        DEBUG ((
          DEBUG_ERROR,
          "     Granularity/SpecificFlag = %ld / %02x%s\n",
          Descriptor->AddrSpaceGranularity,
          Descriptor->SpecificFlag,
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
