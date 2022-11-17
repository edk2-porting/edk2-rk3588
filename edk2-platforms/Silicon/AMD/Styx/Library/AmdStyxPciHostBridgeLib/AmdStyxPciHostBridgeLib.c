/** @file
  PCI Host Bridge Library instance for AMD Seattle SOC

  Copyright (c) 2016, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiDxe.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>

#pragma pack(1)
typedef struct {
  ACPI_HID_DEVICE_PATH     AcpiDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_PCI_ROOT_BRIDGE_DEVICE_PATH;
#pragma pack ()

STATIC EFI_PCI_ROOT_BRIDGE_DEVICE_PATH mEfiPciRootBridgeDevicePath = {
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof(ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof(ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID(0x0A08), // PCI Express
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
};

GLOBAL_REMOVE_IF_UNREFERENCED
CHAR16 *mPciHostBridgeLibAcpiAddressSpaceTypeStr[] = {
  L"Mem", L"I/O", L"Bus"
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
  PCI_ROOT_BRIDGE     *RootBridge;

  *Count = 1;
  RootBridge = AllocateZeroPool (*Count * sizeof *RootBridge);

  RootBridge->Segment     = 0;

  RootBridge->Supports    = EFI_PCI_ATTRIBUTE_IDE_PRIMARY_IO |
                            EFI_PCI_ATTRIBUTE_IDE_SECONDARY_IO |
                            EFI_PCI_ATTRIBUTE_ISA_IO_16 |
                            EFI_PCI_ATTRIBUTE_ISA_MOTHERBOARD_IO |
                            EFI_PCI_ATTRIBUTE_VGA_MEMORY |
                            EFI_PCI_ATTRIBUTE_VGA_IO_16  |
                            EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO_16;
  RootBridge->Attributes  = RootBridge->Supports;

  RootBridge->DmaAbove4G  = TRUE;

  RootBridge->AllocationAttributes  = EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |
                                      EFI_PCI_HOST_BRIDGE_MEM64_DECODE ;

  RootBridge->Bus.Base              = PcdGet32 (PcdPciBusMin);
  RootBridge->Bus.Limit             = PcdGet32 (PcdPciBusMax);
  RootBridge->Io.Base               = PcdGet64 (PcdPciIoBase);
  RootBridge->Io.Limit              = PcdGet64 (PcdPciIoBase) + PcdGet64 (PcdPciIoSize) - 1;
  RootBridge->Mem.Base              = PcdGet32 (PcdPciMmio32Base);
  RootBridge->Mem.Limit             = PcdGet32 (PcdPciMmio32Base) + PcdGet32 (PcdPciMmio32Size) - 1;
  RootBridge->MemAbove4G.Base       = PcdGet64 (PcdPciMmio64Base);
  RootBridge->MemAbove4G.Limit      = PcdGet64 (PcdPciMmio64Base) + PcdGet64 (PcdPciMmio64Size) - 1;

  //
  // No separate ranges for prefetchable and non-prefetchable BARs
  //
  RootBridge->PMem.Base             = MAX_UINT64;
  RootBridge->PMem.Limit            = 0;
  RootBridge->PMemAbove4G.Base      = MAX_UINT64;
  RootBridge->PMemAbove4G.Limit     = 0;

  ASSERT (FixedPcdGet64 (PcdPciMmio32Translation) == 0);
  ASSERT (FixedPcdGet64 (PcdPciMmio64Translation) == 0);

  RootBridge->NoExtendedConfigSpace = FALSE;

  RootBridge->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath;

  return RootBridge;
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
  FreePool (Bridges);
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
  DEBUG ((EFI_D_ERROR, "PciHostBridge: Resource conflict happens!\n"));

  RootBridgeIndex = 0;
  Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Configuration;
  while (Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    DEBUG ((EFI_D_ERROR, "RootBridge[%d]:\n", RootBridgeIndex++));
    for (; Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR; Descriptor++) {
      ASSERT (Descriptor->ResType <
              (sizeof (mPciHostBridgeLibAcpiAddressSpaceTypeStr) /
               sizeof (mPciHostBridgeLibAcpiAddressSpaceTypeStr[0])
               )
              );
      DEBUG ((EFI_D_ERROR, " %s: Length/Alignment = 0x%lx / 0x%lx\n",
              mPciHostBridgeLibAcpiAddressSpaceTypeStr[Descriptor->ResType],
              Descriptor->AddrLen, Descriptor->AddrRangeMax
              ));
      if (Descriptor->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        DEBUG ((EFI_D_ERROR, "     Granularity/SpecificFlag = %ld / %02x%s\n",
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
