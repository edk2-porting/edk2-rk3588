/** @file
  PCI Host Bridge Library instance for Marvell Armada 70x0/80x0

  Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>
  Copyright (c) 2019 Marvell International Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiDxe.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/BoardDesc.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Protocol/PciRootBridgeIo.h>

#pragma pack(1)
typedef struct {
  ACPI_HID_DEVICE_PATH     AcpiDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_PCI_ROOT_BRIDGE_DEVICE_PATH;
#pragma pack ()

STATIC CONST EFI_PCI_ROOT_BRIDGE_DEVICE_PATH mEfiPciRootBridgeDevicePathTemplate = {
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8)((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID (0x0A08), // PCI Express
    0 // AcpiDevicePath.UID
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
  MV_BOARD_PCIE_DESCRIPTION CONST *BoardPcieDescription;
  MARVELL_BOARD_DESC_PROTOCOL     *BoardDescriptionProtocol;
  EFI_PCI_ROOT_BRIDGE_DEVICE_PATH *EfiPciRootBridgeDevicePath;
  MV_PCIE_CONTROLLER CONST        *PcieController;
  PCI_ROOT_BRIDGE                 *PciRootBridges;
  PCI_ROOT_BRIDGE                 *RootBridge;
  EFI_STATUS                       Status;
  UINTN                            Index;

  *Count = 0;

  /* Obtain list of available controllers */
  Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                  NULL,
                  (VOID **)&BoardDescriptionProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot locate BoardDesc protocol\n",
      __FUNCTION__));
    return NULL;
  }

  Status = BoardDescriptionProtocol->PcieDescriptionGet (
                                       BoardDescriptionProtocol,
                                       &BoardPcieDescription);
  if (Status == EFI_NOT_FOUND) {
    /* No controllers used on the platform, exit silently */
    return NULL;
  } else if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot get Pcie board desc from BoardDesc protocol\n",
      __FUNCTION__));
    return NULL;
  }

  /* Assign return values */
  PciRootBridges = AllocateZeroPool (BoardPcieDescription->PcieControllerCount *
                                     sizeof (PCI_ROOT_BRIDGE));
  if (PciRootBridges == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Fail to allocate resources\n", __FUNCTION__));
    return NULL;
  }

  *Count = BoardPcieDescription->PcieControllerCount;
  RootBridge = PciRootBridges;

  /* Fill information of all root bridge instances */
  for (Index = 0; Index < *Count; Index++, RootBridge++) {
    EfiPciRootBridgeDevicePath = AllocateCopyPool (
                                   sizeof (EFI_PCI_ROOT_BRIDGE_DEVICE_PATH),
                                   &mEfiPciRootBridgeDevicePathTemplate
                                   );
    EfiPciRootBridgeDevicePath->AcpiDevicePath.UID = Index;

    PcieController = &(BoardPcieDescription->PcieControllers[Index]);

    RootBridge->Segment   = Index;
    RootBridge->Supports  = 0;
    RootBridge->Attributes  = RootBridge->Supports;

    RootBridge->DmaAbove4G  = FALSE;

#ifndef MDE_CPU_ARM
    RootBridge->AllocationAttributes  = EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |
                                        EFI_PCI_HOST_BRIDGE_MEM64_DECODE;
#else
    RootBridge->AllocationAttributes  = EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM;
#endif

    RootBridge->Bus.Base = PcieController->PcieBusMin;
    RootBridge->Bus.Limit = PcieController->PcieBusMax;
    RootBridge->Io.Base = PcieController->PcieIoWinBase;
    RootBridge->Io.Limit = PcieController->PcieIoWinBase +
                           PcieController->PcieIoWinSize - 1;
    RootBridge->Io.Translation = MAX_UINT64 -
                                 PcieController->PcieIoTranslation + 1;
    RootBridge->Mem.Base = PcieController->PcieMmio32WinBase;
    RootBridge->Mem.Limit = PcieController->PcieMmio32WinBase +
                            PcieController->PcieMmio32WinSize - 1;
    RootBridge->Mem.Translation = MAX_UINT64 -
                                  PcieController->PcieMmio32Translation + 1;
#ifndef MDE_CPU_ARM
    RootBridge->MemAbove4G.Base = PcieController->PcieMmio64WinBase;
    RootBridge->MemAbove4G.Limit = PcieController->PcieMmio64WinBase +
                                   PcieController->PcieMmio64WinSize - 1;
    RootBridge->MemAbove4G.Translation = MAX_UINT64 -
                                      PcieController->PcieMmio64Translation + 1;
#else
    RootBridge->MemAbove4G.Base = MAX_UINT64;
    RootBridge->MemAbove4G.Limit = 0;
    RootBridge->MemAbove4G.Translation = 0;
#endif

    /* No separate ranges for prefetchable and non-prefetchable BARs */
    RootBridge->PMem.Base           = MAX_UINT64;
    RootBridge->PMem.Limit          = 0;
    RootBridge->PMemAbove4G.Base    = MAX_UINT64;
    RootBridge->PMemAbove4G.Limit   = 0;


    RootBridge->NoExtendedConfigSpace = FALSE;

    RootBridge->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)EfiPciRootBridgeDevicePath;
  }

  return PciRootBridges;
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
  CHAR16 *MemoryTypeDescription[] = { L"(Prefetchable)", L"" };
  CHAR16 *MemoryType;
  UINTN                              RootBridgeIndex;

  DEBUG ((DEBUG_ERROR, "PciHostBridge: Resource conflict happens!\n"));

  RootBridgeIndex = 0;
  Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)Configuration;

  while (Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {

    DEBUG ((DEBUG_ERROR, "RootBridge[%d]:\n", RootBridgeIndex++));

    for (; Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR; Descriptor++) {
      ASSERT (Descriptor->ResType <
              ARRAY_SIZE (mPciHostBridgeLibAcpiAddressSpaceTypeStr));

      DEBUG ((DEBUG_ERROR,
        " %s: Length/Alignment = 0x%lx / 0x%lx\n",
        mPciHostBridgeLibAcpiAddressSpaceTypeStr[Descriptor->ResType],
        Descriptor->AddrLen, Descriptor->AddrRangeMax));

      if (Descriptor->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        if (Descriptor->SpecificFlag &
            EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_CACHEABLE_PREFETCHABLE) {
          MemoryType = MemoryTypeDescription[0];
        } else {
          MemoryType = MemoryTypeDescription[1];
        }

        DEBUG ((DEBUG_ERROR,
          "     Granularity/SpecificFlag = %ld / %02x%s\n",
          Descriptor->AddrSpaceGranularity,
          Descriptor->SpecificFlag,
          MemoryType));
      }
    }
    /* Skip the END descriptor for root bridge */
    ASSERT (Descriptor->Desc == ACPI_END_TAG_DESCRIPTOR);
    Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)(
                  (EFI_ACPI_END_TAG_DESCRIPTOR *)Descriptor + 1);
  }
}
