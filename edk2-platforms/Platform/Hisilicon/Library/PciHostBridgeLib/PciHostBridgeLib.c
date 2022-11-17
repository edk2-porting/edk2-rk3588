/** @file
  PCI Host Bridge Library instance for Hisilicon D0x

  Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2017 - 2018, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiDxe.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/OemMiscLib.h>
#include <Library/PcdLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/PlatformPciLib.h>

#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Protocol/PciRootBridgeIo.h>


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
    EISA_PNP_ID(0x0A03), // PCI
    0
  }, {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

STATIC PCI_ROOT_BRIDGE mRootBridgeTemplate = {
  0,                                              // Segment
  0,                                              // Supports
  0,                                              // Attributes
  TRUE,                                           // DmaAbove4G
  FALSE,                                          // NoExtendedConfigSpace
  FALSE,                                          // ResourceAssigned
  EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |          // AllocationAttributes
  EFI_PCI_HOST_BRIDGE_MEM64_DECODE,
  {
    // Bus
    0,
    0
  }, {
    // Io
    0,
    0,
    0
  }, {
    // Mem
    MAX_UINT64,
    0,
    0
  }, {
    // MemAbove4G
    MAX_UINT64,
    0,
    0
  }, {
    // PMem
    MAX_UINT64,
    0,
    0
  }, {
    // PMemAbove4G
    MAX_UINT64,
    0,
    0
  },
  (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath
};

STATIC
EFI_STATUS
ConstructRootBridge (
    PCI_ROOT_BRIDGE                     *Bridge,
    PCI_ROOT_BRIDGE_RESOURCE_APPETURE   *Appeture
    )
{
  EFI_PCI_ROOT_BRIDGE_DEVICE_PATH *DevicePath;
  CopyMem (Bridge, &mRootBridgeTemplate, sizeof *Bridge);
  Bridge->Segment = Appeture->Segment;
  Bridge->Bus.Base = Appeture->BusBase;
  Bridge->Bus.Limit = Appeture->BusLimit;
  Bridge->Io.Base = Appeture->IoBase;
  // According to UEFI 2.7, device address = host address + translation
  Bridge->Io.Translation = Appeture->IoBase - Appeture->CpuIoRegionBase;
  // IoLimit is actually an address in CPU view
  // TODO: improve the definition of PCI_ROOT_BRIDGE_RESOURCE_APPETURE
  Bridge->Io.Limit = Appeture->IoLimit + Bridge->Io.Translation;
  if (Appeture->PciRegionBase > MAX_UINT32) {
    Bridge->MemAbove4G.Base = Appeture->PciRegionBase;
    Bridge->MemAbove4G.Limit = Appeture->PciRegionLimit;
    Bridge->MemAbove4G.Translation = Appeture->PciRegionBase - Appeture->CpuMemRegionBase;
  } else {
    Bridge->Mem.Base = Appeture->PciRegionBase;
    Bridge->Mem.Limit = Appeture->PciRegionLimit;
    Bridge->Mem.Translation = Appeture->PciRegionBase - Appeture->CpuMemRegionBase;
  }

  DevicePath = AllocateCopyPool(sizeof mEfiPciRootBridgeDevicePath, &mEfiPciRootBridgeDevicePath);
  if (DevicePath == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] AllocatePool failed!\n", __FUNCTION__, __LINE__));
    return EFI_OUT_OF_RESOURCES;
  }

  DevicePath->AcpiDevicePath.UID = Bridge->Segment;
  Bridge->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)DevicePath;
  return EFI_SUCCESS;
}

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
  EFI_STATUS                  Status;
  UINTN                       Loop1;
  UINTN                       Loop2;
  UINT32                      PcieRootBridgeMask;
  UINTN                       RootBridgeCount = 0;
  PCI_ROOT_BRIDGE             *Bridges;

  // Set default value to 0 in case we got any error.
  *Count = 0;


  if (!OemIsMpBoot())
  {
    PcieRootBridgeMask = PcdGet32(PcdPcieRootBridgeMask);
  }
  else
  {
    PcieRootBridgeMask = PcdGet32(PcdPcieRootBridgeMask2P);
  }

  for (Loop1 = 0; Loop1 < PCIE_MAX_HOSTBRIDGE; Loop1++) {
    if (((PcieRootBridgeMask >> (PCIE_MAX_ROOTBRIDGE * Loop1)) & 0xFF ) == 0) {
      continue;
    }

    for (Loop2 = 0; Loop2 < PCIE_MAX_ROOTBRIDGE; Loop2++) {
      if (!(((PcieRootBridgeMask >> (PCIE_MAX_ROOTBRIDGE * Loop1)) >> Loop2 ) & 0x01)) {
        continue;
      }
      RootBridgeCount++;
    }
  }

  Bridges = AllocatePool (RootBridgeCount * sizeof *Bridges);
  if (Bridges == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a:%d] - AllocatePool failed!\n", __FUNCTION__, __LINE__));
    return NULL;
  }

  for (Loop1 = 0; Loop1 < PCIE_MAX_HOSTBRIDGE; Loop1++) {
    if (((PcieRootBridgeMask >> (PCIE_MAX_ROOTBRIDGE * Loop1)) & 0xFF ) == 0) {
      continue;
    }

    for (Loop2 = 0; Loop2 < PCIE_MAX_ROOTBRIDGE; Loop2++) {
      if (!(((PcieRootBridgeMask >> (PCIE_MAX_ROOTBRIDGE * Loop1)) >> Loop2 ) & 0x01)) {
        continue;
      }
      Status = ConstructRootBridge (&Bridges[*Count], &mResAppeture[Loop1][Loop2]);
      if (EFI_ERROR (Status)) {
        continue;
      }
      (*Count)++;
    }
  }

  if (*Count == 0) {
    FreePool (Bridges);
    return NULL;
  }
  return Bridges;
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
  UINTN Index;

  for (Index = 0; Index < Count; Index++) {
    FreePool (Bridges[Index].DevicePath);
  }

  if (Bridges != NULL) {
    FreePool (Bridges);
  }
}


#ifndef MDEPKG_NDEBUG
STATIC CONST CHAR16 mPciHostBridgeLibAcpiAddressSpaceTypeStr[][4] = {
  L"Mem", L"I/O", L"Bus"
};
#endif

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
