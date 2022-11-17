/** @file
  Pci Hotplug Driver : This file will perform specific PCI-EXPRESS
  Devics resource configuration.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//
#include "PciHotPlug.h"
#include <Ppi/SiPolicy.h>
#include <TbtBoardInfo.h>
#include <Library/PchPcieRpLib.h>
#include <Library/TbtCommonLib.h>

#define PCIE_NUM  (20)
#define PEG_NUM   (3)
#define PADDING_BUS (1)
#define PADDING_NONPREFETCH_MEM (1)
#define PADDING_PREFETCH_MEM (1)
#define PADDING_IO (1)
#define PADDING_NUM (PADDING_BUS + PADDING_NONPREFETCH_MEM + PADDING_PREFETCH_MEM + PADDING_IO)

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HPC_LOCATION          mPcieLocation[PCIE_NUM + PEG_NUM];

GLOBAL_REMOVE_IF_UNREFERENCED UINTN mHpcCount = 0;

GLOBAL_REMOVE_IF_UNREFERENCED PCIE_HOT_PLUG_DEVICE_PATH mHotplugPcieDevicePathTemplate = {
  ACPI,
  PCI(0xFF, 0xFF), // Dummy Device no & Function no
  END
};

/**
  Entry point for the driver.

  This routine reads the PlatformType GPI on FWH and produces a protocol
  to be consumed by the chipset driver to effect those settings.

  @param[in]  ImageHandle    An image handle.
  @param[in]  SystemTable    A pointer to the system table.

  @retval     EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
PciHotPlug (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                       Status;
  PCI_HOT_PLUG_INSTANCE            *PciHotPlug;
  UINTN                            Index;
  UINTN                            RpDev;
  UINTN                            RpFunc;
  PCIE_HOT_PLUG_DEVICE_PATH       *HotplugPcieDevicePath;
  UINT32                           PcieRootPortHpeData = 0;

  DEBUG ((DEBUG_INFO, "PciHotPlug Entry\n"));

  PcieRootPortHpeData = PcdGet32 (PcdPchPcieRootPortHpe);
  //
  // PCH Rootports Hotplug device path creation
  //
  for (Index = 0; Index < PCIE_NUM; Index++) {
    if (((PcieRootPortHpeData >> Index) & BIT0) == BIT0) { // Check the Rootport no's hotplug is set
      Status = GetPchPcieRpDevFun (Index, &RpDev, &RpFunc); // Get the actual device/function no corresponding to the Rootport no provided
      ASSERT_EFI_ERROR (Status);

      HotplugPcieDevicePath = NULL;
      HotplugPcieDevicePath = AllocatePool (sizeof (PCIE_HOT_PLUG_DEVICE_PATH));
      ASSERT (HotplugPcieDevicePath != NULL);
      if (HotplugPcieDevicePath == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CopyMem (HotplugPcieDevicePath, &mHotplugPcieDevicePathTemplate, sizeof (PCIE_HOT_PLUG_DEVICE_PATH));
      HotplugPcieDevicePath->PciRootPortNode.Device = (UINT8) RpDev; // Update real Device no
      HotplugPcieDevicePath->PciRootPortNode.Function = (UINT8) RpFunc; // Update real Function no

      mPcieLocation[mHpcCount].HpcDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)HotplugPcieDevicePath;
      mPcieLocation[mHpcCount].HpbDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)HotplugPcieDevicePath;
      mHpcCount++;

      DEBUG ((DEBUG_INFO, "(%02d) PciHotPlug (PCH RP#) : Bus 0x00, Device 0x%x, Function 0x%x is added to the Hotplug Device Path list \n", mHpcCount, RpDev, RpFunc));
    }
  }


  PciHotPlug = AllocatePool (sizeof (PCI_HOT_PLUG_INSTANCE));
  ASSERT (PciHotPlug != NULL);
  if (PciHotPlug == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize driver private data.
  //
  ZeroMem (PciHotPlug, sizeof (PCI_HOT_PLUG_INSTANCE));

  PciHotPlug->Signature                               = PCI_HOT_PLUG_DRIVER_PRIVATE_SIGNATURE;
  PciHotPlug->HotPlugInitProtocol.GetRootHpcList      = GetRootHpcList;
  PciHotPlug->HotPlugInitProtocol.InitializeRootHpc   = InitializeRootHpc;
  PciHotPlug->HotPlugInitProtocol.GetResourcePadding  = GetResourcePadding;

  Status = gBS->InstallProtocolInterface (
                  &PciHotPlug->Handle,
                  &gEfiPciHotPlugInitProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &PciHotPlug->HotPlugInitProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}


/**
  This procedure returns a list of Root Hot Plug controllers that require
  initialization during boot process

  @param[in]  This      The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol.
  @param[out] HpcCount  The number of Root HPCs returned.
  @param[out] HpcList   The list of Root HPCs. HpcCount defines the number of elements in this list.

  @retval EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
GetRootHpcList (
  IN EFI_PCI_HOT_PLUG_INIT_PROTOCOL    *This,
  OUT UINTN                            *HpcCount,
  OUT EFI_HPC_LOCATION                 **HpcList
  )
{
  *HpcCount = mHpcCount;
  *HpcList  = mPcieLocation;

  return EFI_SUCCESS;
}


/**
  This procedure Initializes one Root Hot Plug Controller
  This process may casue initialization of its subordinate buses

  @param[in]  This            The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol.
  @param[in]  HpcDevicePath   The Device Path to the HPC that is being initialized.
  @param[in]  HpcPciAddress   The address of the Hot Plug Controller function on the PCI bus.
  @param[in]  Event           The event that should be signaled when the Hot Plug Controller initialization is complete. Set to NULL if the caller wants to wait until the entire initialization process is complete. The event must be of the type EFI_EVT_SIGNAL.
  @param[out] HpcState        The state of the Hot Plug Controller hardware. The type EFI_Hpc_STATE is defined in section 3.1.

  @retval   EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
InitializeRootHpc (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL      *This,
  IN  EFI_DEVICE_PATH_PROTOCOL            *HpcDevicePath,
  IN  UINT64                              HpcPciAddress,
  IN  EFI_EVENT                           Event, OPTIONAL
  OUT EFI_HPC_STATE                       *HpcState
  )
{
  if (Event) {
    gBS->SignalEvent (Event);
  }

  *HpcState = EFI_HPC_STATE_INITIALIZED;

  return EFI_SUCCESS;
}


/**
  Returns the resource padding required by the PCI bus that is controlled by the specified Hot Plug Controller.

  @param[in]  This           The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol. initialized.
  @param[in]  HpcDevicePath  The Device Path to the Hot Plug Controller.
  @param[in]  HpcPciAddress  The address of the Hot Plug Controller function on the PCI bus.
  @param[out] HpcState       The state of the Hot Plug Controller hardware. The type EFI_HPC_STATE is defined in section 3.1.
  @param[out] Padding        This is the amount of resource padding required by the PCI bus under the control of the specified Hpc. Since the caller does not know the size of this buffer, this buffer is allocated by the callee and freed by the caller.
  @param[out] Attribute      Describes how padding is accounted for.

  @retval     EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
GetResourcePadding (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL  *This,
  IN  EFI_DEVICE_PATH_PROTOCOL        *HpcDevicePath,
  IN  UINT64                          HpcPciAddress,
  OUT EFI_HPC_STATE                   *HpcState,
  OUT VOID                            **Padding,
  OUT EFI_HPC_PADDING_ATTRIBUTES      *Attributes
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *PaddingResource;
  EFI_STATUS                        Status;
  UINT8                             RsvdExtraBusNum = 0;
  UINT16                            RsvdPcieMegaMem = 10;
  UINT8                             PcieMemAddrRngMax = 0;
  UINT16                            RsvdPciePMegaMem = 10;
  UINT8                             PciePMemAddrRngMax = 0;
  UINT8                             RsvdTbtExtraBusNum = 0;
  UINT16                            RsvdTbtPcieMegaMem = 10;
  UINT8                             TbtPcieMemAddrRngMax = 0;
  UINT16                            RsvdTbtPciePMegaMem = 10;
  UINT8                             TbtPciePMemAddrRngMax = 0;
  UINT8                             RsvdPcieKiloIo = 4;
  BOOLEAN                           SetResourceforTbt = FALSE;
  UINTN                             RpIndex;
  UINTN                             RpDev;
  UINTN                             RpFunc;

DEBUG ((DEBUG_INFO, "GetResourcePadding : Start \n"));

  PaddingResource = AllocatePool (PADDING_NUM * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));
  ASSERT (PaddingResource != NULL);
  if (PaddingResource == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *Padding = (VOID *) PaddingResource;

  RpDev = (UINTN) ((HpcPciAddress >> 16) & 0xFF);
  RpFunc = (UINTN) ((HpcPciAddress >> 8) & 0xFF);

  // Get the actual Rootport no corresponding to the device/function no provided
  if (RpDev == SA_PEG_DEV_NUM) {
    // PEG
    RpIndex = PCIE_NUM + RpFunc;
    DEBUG ((DEBUG_INFO, "GetResourcePadding : PEG Rootport no %02d Bus 0x00, Device 0x%x, Function 0x%x \n", (RpIndex-PCIE_NUM), RpDev, RpFunc));
  } else {
    // PCH
    Status = GetPchPcieRpNumber (RpDev, RpFunc, &RpIndex);
    DEBUG ((DEBUG_INFO, "GetResourcePadding : PCH Rootport no %02d Bus 0x00, Device 0x%x, Function 0x%x \n", RpIndex, RpDev, RpFunc));
  }

  GetRootporttoSetResourcesforTbt(RpIndex, &RsvdTbtExtraBusNum, &RsvdTbtPcieMegaMem ,&TbtPcieMemAddrRngMax ,&RsvdTbtPciePMegaMem ,&TbtPciePMemAddrRngMax, &SetResourceforTbt);
    if (SetResourceforTbt) {
      RsvdExtraBusNum = RsvdTbtExtraBusNum;
      RsvdPcieMegaMem = RsvdTbtPcieMegaMem;
      PcieMemAddrRngMax = TbtPcieMemAddrRngMax;
      RsvdPciePMegaMem = RsvdTbtPciePMegaMem;
      PciePMemAddrRngMax = TbtPciePMemAddrRngMax;
    }

  //
  // Padding for bus
  //
  ZeroMem (PaddingResource, PADDING_NUM * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));
  *Attributes                   = EfiPaddingPciBus;

  PaddingResource->Desc         = 0x8A;
  PaddingResource->Len          = 0x2B;
  PaddingResource->ResType      = ACPI_ADDRESS_SPACE_TYPE_BUS;
  PaddingResource->GenFlag      = 0x0;
  PaddingResource->SpecificFlag = 0;
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrRangeMax = 0;
  PaddingResource->AddrLen      = RsvdExtraBusNum;

  //
  // Padding for non-prefetchable memory
  //
  PaddingResource++;
  PaddingResource->Desc                 = 0x8A;
  PaddingResource->Len                  = 0x2B;
  PaddingResource->ResType              = ACPI_ADDRESS_SPACE_TYPE_MEM;
  PaddingResource->GenFlag              = 0x0;
    if (SetResourceforTbt) {
    PaddingResource->AddrSpaceGranularity = 32;
  } else {
    PaddingResource->AddrSpaceGranularity = 32;
  }
  PaddingResource->SpecificFlag         = 0;
  //
  // Pad non-prefetchable
  //
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = RsvdPcieMegaMem * 0x100000;
  if (SetResourceforTbt) {
    PaddingResource->AddrRangeMax = (1 << PcieMemAddrRngMax) - 1;
  } else {
    PaddingResource->AddrRangeMax = 1;
  }

  //
  // Padding for prefetchable memory
  //
  PaddingResource++;
  PaddingResource->Desc                 = 0x8A;
  PaddingResource->Len                  = 0x2B;
  PaddingResource->ResType              = ACPI_ADDRESS_SPACE_TYPE_MEM;
  PaddingResource->GenFlag              = 0x0;
    if (SetResourceforTbt) {
    PaddingResource->AddrSpaceGranularity = 32;
  } else {
    PaddingResource->AddrSpaceGranularity = 32;
  }
  PaddingResource->SpecificFlag         = 06;
  //
  // Padding for prefetchable memory
  //
  PaddingResource->AddrRangeMin = 0;
  if (SetResourceforTbt) {
    PaddingResource->AddrLen      = RsvdPciePMegaMem * 0x100000;
  } else {
    PaddingResource->AddrLen      = RsvdPcieMegaMem * 0x100000;
  }
  //
  // Pad 16 MB of MEM
  //
  if (SetResourceforTbt) {
    PaddingResource->AddrRangeMax = (1 << PciePMemAddrRngMax) - 1;
  } else {
    PaddingResource->AddrRangeMax = 1;
  }
  //
  // Alignment
  //
  // Padding for I/O
  //
  PaddingResource++;
  PaddingResource->Desc         = 0x8A;
  PaddingResource->Len          = 0x2B;
  PaddingResource->ResType      = ACPI_ADDRESS_SPACE_TYPE_IO;
  PaddingResource->GenFlag      = 0x0;
  PaddingResource->SpecificFlag = 0;
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = RsvdPcieKiloIo * 0x400;
  //
  // Pad 4K of IO
  //
  PaddingResource->AddrRangeMax = 1;
  //
  // Alignment
  //
  // Terminate the entries.
  //
  PaddingResource++;
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Desc     = ACPI_END_TAG_DESCRIPTOR;
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Checksum = 0x0;

  *HpcState = EFI_HPC_STATE_INITIALIZED | EFI_HPC_STATE_ENABLED;

  return EFI_SUCCESS;
}

