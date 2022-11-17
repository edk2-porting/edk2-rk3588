/** @file
  PCI Host Bridge Library instance for ARM FVP Model

  Copyright (c) 2021, Arm Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Protocol/PciRootBridgeIo.h>
#include <ArmPlatform.h>

#pragma pack(1)

/** A structure describing the PCI root bridge device path.
*/
typedef struct {
  /// ACPI Device path.
  ACPI_HID_DEVICE_PATH     AcpiDevicePath;
  /// END Device Path tag.
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_PCI_ROOT_BRIDGE_DEVICE_PATH;

#pragma pack ()

STATIC EFI_PCI_ROOT_BRIDGE_DEVICE_PATH mEfiPciRootBridgeDevicePath = {
  // ACPI device path
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8)((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID (0x0A08), // PCIe
    0
  },
  // End device path tag
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

STATIC PCI_ROOT_BRIDGE mRootBridge = {
  0,                                              // Segment
  0,                                              // Supports
  0,                                              // Attributes
  FALSE,                                          // DmaAbove4G
  FALSE,                                          // NoExtendedConfigSpace
  FALSE,                                          // ResourceAssigned
  EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |          // AllocationAttributes
  EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
  {
    // Bus
    FixedPcdGet32 (PcdPciBusMin),
    FixedPcdGet32 (PcdPciBusMax)
  },
  {
    // Io
    FixedPcdGet64 (PcdPciIoBase),
    FixedPcdGet64 (PcdPciIoBase) + FixedPcdGet64 (PcdPciIoSize) - 1
  },
  {
    // Mem
    FixedPcdGet32 (PcdPciMmio32Base),
    FixedPcdGet32 (PcdPciMmio32Base) + FixedPcdGet32 (PcdPciMmio32Size) - 1
  },
  {
    // MemAbove4G
    FixedPcdGet64 (PcdPciMmio64Base),
    FixedPcdGet64 (PcdPciMmio64Base) + FixedPcdGet64 (PcdPciMmio64Size) - 1
  },
  {
    // PMem
    MAX_UINT64,
    0
  },
  {
    // PMemAbove4G
    MAX_UINT64,
    0
  },
  (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath
};

#ifndef MDEPKG_NDEBUG
STATIC CONST CHAR16 mAcpiAddrSpaceTypeStr[][4] = {
  L"Mem", L"I/O", L"Bus"
};
#endif

/**
  Return all the root bridge instances in an array.

  @param [out] Count  Return the count of root bridge instances.

  @returns All the root bridge instances in an array.
           The array should be passed into PciHostBridgeFreeRootBridges()
           when it's not used.
**/
PCI_ROOT_BRIDGE *
EFIAPI
PciHostBridgeGetRootBridges (
  OUT UINTN *Count
  )
{
  UINT32 SysId;

  // Check if the platform is FVP RevC
  SysId = MmioRead32 (ARM_VE_SYS_ID_REG);
  if ((SysId & ARM_FVP_SYS_ID_REV_MASK) == ARM_FVP_BASE_REVC_REV) {
    // There is a single Root Bridge instance on the FVP RevC Model
    *Count = 1;
    return &mRootBridge;
  }

  // Other FVP models do not have PCIe
  *Count = 0;
  return NULL;
}

/**
  Free the root bridge instances array returned
  from PciHostBridgeGetRootBridges().

  @param [in] Bridges The root bridge instances array.
  @param [in] Count   The count of the array.
**/
VOID
EFIAPI
PciHostBridgeFreeRootBridges (
  IN PCI_ROOT_BRIDGE *Bridges,
  IN UINTN           Count
  )
{
}

/**
  Inform the platform that the resource conflict happens.

  @param [in] HostBridgeHandle  Handle of the Host Bridge.
  @param [in] Configuration     Pointer to PCI I/O and PCI memory resource
                                descriptors. The Configuration contains the
                                resources for all the root bridges. The
                                resource for each root bridge is terminated
                                with END descriptor and an additional END is
                                appended indicating the end of the entire
                                resources. The resource descriptor field
                                values follow the description in
                                EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
                                .SubmitResources().
**/
VOID
EFIAPI
PciHostBridgeResourceConflict (
  IN EFI_HANDLE                      HostBridgeHandle,
  IN VOID                            *Configuration
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
      ASSERT (Descriptor->ResType < ARRAY_SIZE (mAcpiAddrSpaceTypeStr));
      DEBUG ((
        DEBUG_ERROR,
        " %s: Length/Alignment = 0x%lx / 0x%lx\n",
        mAcpiAddrSpaceTypeStr[Descriptor->ResType],
        Descriptor->AddrLen, Descriptor->AddrRangeMax
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

    // Skip the END descriptor for root bridge
    ASSERT (Descriptor->Desc == ACPI_END_TAG_DESCRIPTOR);
    Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)(
                   (EFI_ACPI_END_TAG_DESCRIPTOR *)Descriptor + 1
                   );
  }
}
