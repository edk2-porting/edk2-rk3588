/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Vtd.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/IoMmu.h>
#include <Ppi/VtdInfo.h>

#include "IntelVTdDmarPei.h"

/**
  Dump DMAR DeviceScopeEntry.

  @param[in]  DmarDeviceScopeEntry      DMAR DeviceScopeEntry
**/
VOID
DumpDmarDeviceScopeEntry (
  IN EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER  *DmarDeviceScopeEntry
  )
{
  UINTN                                 PciPathNumber;
  UINTN                                 PciPathIndex;
  EFI_ACPI_DMAR_PCI_PATH                *PciPath;

  if (DmarDeviceScopeEntry == NULL) {
    return;
  }

  DEBUG ((DEBUG_INFO,
    "    *************************************************************************\n"
    ));
  DEBUG ((DEBUG_INFO,
    "    *       DMA-Remapping Device Scope Entry Structure                      *\n"
    ));
  DEBUG ((DEBUG_INFO,
    "    *************************************************************************\n"
    ));
  DEBUG ((DEBUG_INFO,
    (sizeof (UINTN) == sizeof (UINT64)) ?
    "    DMAR Device Scope Entry address ...................... 0x%016lx\n" :
    "    DMAR Device Scope Entry address ...................... 0x%08x\n",
    DmarDeviceScopeEntry
    ));
  DEBUG ((DEBUG_INFO,
    "      Device Scope Entry Type ............................ 0x%02x\n",
    DmarDeviceScopeEntry->Type
    ));
  switch (DmarDeviceScopeEntry->Type) {
  case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_ENDPOINT:
    DEBUG ((DEBUG_INFO,
      "        PCI Endpoint Device\n"
      ));
    break;
  case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_BRIDGE:
    DEBUG ((DEBUG_INFO,
      "        PCI Sub-hierachy\n"
      ));
    break;
  default:
    break;
  }
  DEBUG ((DEBUG_INFO,
    "      Length ............................................. 0x%02x\n",
    DmarDeviceScopeEntry->Length
    ));
  DEBUG ((DEBUG_INFO,
    "      Enumeration ID ..................................... 0x%02x\n",
    DmarDeviceScopeEntry->EnumerationId
    ));
  DEBUG ((DEBUG_INFO,
    "      Starting Bus Number ................................ 0x%02x\n",
    DmarDeviceScopeEntry->StartBusNumber
    ));

  PciPathNumber = (DmarDeviceScopeEntry->Length - sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER)) / sizeof (EFI_ACPI_DMAR_PCI_PATH);
  PciPath = (EFI_ACPI_DMAR_PCI_PATH *) (DmarDeviceScopeEntry + 1);
  for (PciPathIndex = 0; PciPathIndex < PciPathNumber; PciPathIndex++) {
    DEBUG ((DEBUG_INFO,
      "      Device ............................................. 0x%02x\n",
      PciPath[PciPathIndex].Device
      ));
    DEBUG ((DEBUG_INFO,
      "      Function ........................................... 0x%02x\n",
      PciPath[PciPathIndex].Function
      ));
  }

  DEBUG ((DEBUG_INFO,
    "    *************************************************************************\n\n"
    ));

  return;
}

/**
  Dump DMAR RMRR table.

  @param[in]  Rmrr              DMAR RMRR table
**/
VOID
DumpDmarRmrr (
  IN EFI_ACPI_DMAR_RMRR_HEADER  *Rmrr
  )
{
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER     *DmarDeviceScopeEntry;
  INTN                                            RmrrLen;

  if (Rmrr == NULL) {
    return;
  }

  DEBUG ((DEBUG_INFO,
    "  ***************************************************************************\n"
    ));
  DEBUG ((DEBUG_INFO,
    "  *       Reserved Memory Region Reporting Structure                        *\n"
    ));
  DEBUG ((DEBUG_INFO,
    "  ***************************************************************************\n"
    ));
  DEBUG ((DEBUG_INFO,
    (sizeof (UINTN) == sizeof (UINT64)) ?
    "  RMRR address ........................................... 0x%016lx\n" :
    "  RMRR address ........................................... 0x%08x\n",
    Rmrr
    ));
  DEBUG ((DEBUG_INFO,
    "    Type ................................................. 0x%04x\n",
    Rmrr->Header.Type
    ));
  DEBUG ((DEBUG_INFO,
    "    Length ............................................... 0x%04x\n",
    Rmrr->Header.Length
    ));
  DEBUG ((DEBUG_INFO,
    "    Segment Number ....................................... 0x%04x\n",
    Rmrr->SegmentNumber
    ));
  DEBUG ((DEBUG_INFO,
    "    Reserved Memory Region Base Address .................. 0x%016lx\n",
    Rmrr->ReservedMemoryRegionBaseAddress
    ));
  DEBUG ((DEBUG_INFO,
    "    Reserved Memory Region Limit Address ................. 0x%016lx\n",
    Rmrr->ReservedMemoryRegionLimitAddress
    ));

  RmrrLen  = Rmrr->Header.Length - sizeof(EFI_ACPI_DMAR_RMRR_HEADER);
  DmarDeviceScopeEntry = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) (Rmrr + 1);
  while (RmrrLen > 0) {
    DumpDmarDeviceScopeEntry (DmarDeviceScopeEntry);
    RmrrLen -= DmarDeviceScopeEntry->Length;
    DmarDeviceScopeEntry = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) ((UINTN) DmarDeviceScopeEntry + DmarDeviceScopeEntry->Length);
  }

  DEBUG ((DEBUG_INFO,
    "  ***************************************************************************\n\n"
    ));

  return;
}

/**
  Dump DMAR DRHD table.

  @param[in]  Drhd              DMAR DRHD table
**/
VOID
DumpDmarDrhd (
  IN EFI_ACPI_DMAR_DRHD_HEADER  *Drhd
  )
{
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER     *DmarDeviceScopeEntry;
  INTN                                            DrhdLen;

  if (Drhd == NULL) {
    return;
  }

  DEBUG ((DEBUG_INFO,
    "  ***************************************************************************\n"
    ));
  DEBUG ((DEBUG_INFO,
    "  *       DMA-Remapping Hardware Definition Structure                       *\n"
    ));
  DEBUG ((DEBUG_INFO,
    "  ***************************************************************************\n"
    ));
  DEBUG ((DEBUG_INFO,
    (sizeof (UINTN) == sizeof (UINT64)) ?
    "  DRHD address ........................................... 0x%016lx\n" :
    "  DRHD address ........................................... 0x%08x\n",
    Drhd
    ));
  DEBUG ((DEBUG_INFO,
    "    Type ................................................. 0x%04x\n",
    Drhd->Header.Type
    ));
  DEBUG ((DEBUG_INFO,
    "    Length ............................................... 0x%04x\n",
    Drhd->Header.Length
    ));
  DEBUG ((DEBUG_INFO,
    "    Flags ................................................ 0x%02x\n",
    Drhd->Flags
    ));
  DEBUG ((DEBUG_INFO,
    "      INCLUDE_PCI_ALL .................................... 0x%02x\n",
    Drhd->Flags & EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_PCI_ALL
    ));
  DEBUG ((DEBUG_INFO,
    "    Segment Number ....................................... 0x%04x\n",
    Drhd->SegmentNumber
    ));
  DEBUG ((DEBUG_INFO,
    "    Register Base Address ................................ 0x%016lx\n",
    Drhd->RegisterBaseAddress
    ));

  DrhdLen  = Drhd->Header.Length - sizeof (EFI_ACPI_DMAR_DRHD_HEADER);
  DmarDeviceScopeEntry = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) (Drhd + 1);
  while (DrhdLen > 0) {
    DumpDmarDeviceScopeEntry (DmarDeviceScopeEntry);
    DrhdLen -= DmarDeviceScopeEntry->Length;
    DmarDeviceScopeEntry = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) ((UINTN) DmarDeviceScopeEntry + DmarDeviceScopeEntry->Length);
  }

  DEBUG ((DEBUG_INFO,
    "  ***************************************************************************\n\n"
    ));

  return;
}

/**
  Dump DMAR ACPI table.

  @param[in]  Dmar              DMAR ACPI table
**/
VOID
DumpAcpiDMAR (
  IN EFI_ACPI_DMAR_HEADER       *Dmar
  )
{
  EFI_ACPI_DMAR_STRUCTURE_HEADER        *DmarHeader;
  INTN                                  DmarLen;

  if (Dmar == NULL) {
    return;
  }

  //
  // Dump Dmar table
  //
  DEBUG ((DEBUG_INFO,
    "*****************************************************************************\n"
    ));
  DEBUG ((DEBUG_INFO,
    "*         DMAR Table                                                        *\n"
    ));
  DEBUG ((DEBUG_INFO,
    "*****************************************************************************\n"
    ));

  DEBUG ((DEBUG_INFO,
    (sizeof (UINTN) == sizeof (UINT64)) ?
    "DMAR address ............................................. 0x%016lx\n" :
    "DMAR address ............................................. 0x%08x\n",
    Dmar
    ));

  DEBUG ((DEBUG_INFO,
    "  Table Contents:\n"
    ));
  DEBUG ((DEBUG_INFO,
    "    Host Address Width ................................... 0x%02x\n",
    Dmar->HostAddressWidth
    ));
  DEBUG ((DEBUG_INFO,
    "    Flags ................................................ 0x%02x\n",
    Dmar->Flags
    ));
  DEBUG ((DEBUG_INFO,
    "      INTR_REMAP ......................................... 0x%02x\n",
    Dmar->Flags & EFI_ACPI_DMAR_FLAGS_INTR_REMAP
    ));
  DEBUG ((DEBUG_INFO,
    "      X2APIC_OPT_OUT_SET ................................. 0x%02x\n",
    Dmar->Flags & EFI_ACPI_DMAR_FLAGS_X2APIC_OPT_OUT
    ));
  DEBUG ((DEBUG_INFO,
    "      DMA_CTRL_PLATFORM_OPT_IN_FLAG ...................... 0x%02x\n",
    Dmar->Flags & EFI_ACPI_DMAR_FLAGS_DMA_CTRL_PLATFORM_OPT_IN_FLAG
    ));

  DmarLen  = Dmar->Header.Length - sizeof (EFI_ACPI_DMAR_HEADER);
  DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *) (Dmar + 1);
  while (DmarLen > 0) {
    switch (DmarHeader->Type) {
    case EFI_ACPI_DMAR_TYPE_DRHD:
      DumpDmarDrhd ((EFI_ACPI_DMAR_DRHD_HEADER *) DmarHeader);
      break;
    case EFI_ACPI_DMAR_TYPE_RMRR:
      DumpDmarRmrr ((EFI_ACPI_DMAR_RMRR_HEADER *) DmarHeader);
      break;
    default:
      break;
    }
    DmarLen -= DmarHeader->Length;
    DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *) ((UINTN) DmarHeader + DmarHeader->Length);
  }

  DEBUG ((DEBUG_INFO,
    "*****************************************************************************\n\n"
    ));

  return;
}

/**
  Get VTd engine number.

  @param[in]  AcpiDmarTable     DMAR ACPI table

  @return the VTd engine number.
**/
UINTN
GetVtdEngineNumber (
  IN EFI_ACPI_DMAR_HEADER       *AcpiDmarTable
  )
{
  EFI_ACPI_DMAR_STRUCTURE_HEADER        *DmarHeader;
  UINTN                                 VtdIndex;

  VtdIndex = 0;
  DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *) ((UINTN) (AcpiDmarTable + 1));
  while ((UINTN) DmarHeader < (UINTN) AcpiDmarTable + AcpiDmarTable->Header.Length) {
    switch (DmarHeader->Type) {
    case EFI_ACPI_DMAR_TYPE_DRHD:
      VtdIndex++;
      break;
    default:
      break;
    }
    DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *) ((UINTN) DmarHeader + DmarHeader->Length);
  }
  return VtdIndex ;
}

/**
  Get PCI device information from DMAR DevScopeEntry.

  @param[in]  Segment           The segment number.
  @param[in]  DmarDevScopeEntry DMAR DevScopeEntry
  @param[out] Bus               The bus number.
  @param[out] Device            The device number.
  @param[out] Function          The function number.

  @retval EFI_SUCCESS  The PCI device information is returned.
**/
EFI_STATUS
GetPciBusDeviceFunction (
  IN  UINT16                                      Segment,
  IN  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *DmarDevScopeEntry,
  OUT UINT8                                       *Bus,
  OUT UINT8                                       *Device,
  OUT UINT8                                       *Function
  )
{
  EFI_ACPI_DMAR_PCI_PATH                          *DmarPciPath;
  UINT8                                           MyBus;
  UINT8                                           MyDevice;
  UINT8                                           MyFunction;

  DmarPciPath = (EFI_ACPI_DMAR_PCI_PATH *) ((UINTN) (DmarDevScopeEntry + 1));
  MyBus = DmarDevScopeEntry->StartBusNumber;
  MyDevice = DmarPciPath->Device;
  MyFunction = DmarPciPath->Function;

  switch (DmarDevScopeEntry->Type) {
  case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_ENDPOINT:
  case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_BRIDGE:
    while ((UINTN) DmarPciPath + sizeof (EFI_ACPI_DMAR_PCI_PATH) < (UINTN) DmarDevScopeEntry + DmarDevScopeEntry->Length) {
      MyBus = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (Segment, MyBus, MyDevice, MyFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
      DmarPciPath ++;
      MyDevice = DmarPciPath->Device;
      MyFunction = DmarPciPath->Function;
    }
    break;
  case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC:
  case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_MSI_CAPABLE_HPET:
  case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ACPI_NAMESPACE_DEVICE:
    break;
  }

  *Bus = MyBus;
  *Device = MyDevice;
  *Function = MyFunction;

  return EFI_SUCCESS;
}

/**
  Return the index of PCI data.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
  @param[in]  Segment           The Segment used to identify a VTd engine.
  @param[in]  SourceId          The SourceId used to identify a VTd engine and table entry.

  @return The index of the PCI data.
  @retval (UINTN)-1  The PCI data is not found.
**/
UINTN
GetPciDataIndex (
  IN VTD_UNIT_INFO              *VTdUnitInfo,
  IN UINT16                     Segment,
  IN VTD_SOURCE_ID              SourceId
  )
{
  UINTN                         Index;
  VTD_SOURCE_ID                 *PciSourceId;
  PEI_PCI_DEVICE_DATA           *PciDeviceDataBase;

  if (Segment != VTdUnitInfo->Segment) {
    return (UINTN)-1;
  }

  for (Index = 0; Index < VTdUnitInfo->PciDeviceInfo.PciDeviceDataNumber; Index++) {
    PciDeviceDataBase = (PEI_PCI_DEVICE_DATA*) (UINTN) VTdUnitInfo->PciDeviceInfo.PciDeviceData;
    PciSourceId = &PciDeviceDataBase[Index].PciSourceId;
    if ((PciSourceId->Bits.Bus == SourceId.Bits.Bus) &&
        (PciSourceId->Bits.Device == SourceId.Bits.Device) &&
        (PciSourceId->Bits.Function == SourceId.Bits.Function) ) {
      return Index;
    }
  }

  return (UINTN)-1;
}


/**
  Register PCI device to VTd engine.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
  @param[in]  Segment           The segment of the source.
  @param[in]  SourceId          The SourceId of the source.
  @param[in]  DeviceType        The DMAR device scope type.
  @param[in]  CheckExist        TRUE: ERROR will be returned if the PCI device is already registered.
                                FALSE: SUCCESS will be returned if the PCI device is registered.

  @retval EFI_SUCCESS           The PCI device is registered.
  @retval EFI_OUT_OF_RESOURCES  No enough resource to register a new PCI device.
  @retval EFI_ALREADY_STARTED   The device is already registered.

**/
EFI_STATUS
RegisterPciDevice (
  IN VTD_UNIT_INFO              *VTdUnitInfo,
  IN UINT16                     Segment,
  IN VTD_SOURCE_ID              SourceId,
  IN UINT8                      DeviceType,
  IN BOOLEAN                    CheckExist
  )
{
  PEI_PCI_DEVICE_INFORMATION    *PciDeviceInfo;
  VTD_SOURCE_ID                 *PciSourceId;
  UINTN                         PciDataIndex;
  UINTN                         PciDeviceDataSize;
  PEI_PCI_DEVICE_DATA           *NewPciDeviceData;
  PEI_PCI_DEVICE_DATA           *PciDeviceDataBase;

  PciDeviceInfo = &VTdUnitInfo->PciDeviceInfo;

  PciDataIndex = GetPciDataIndex (VTdUnitInfo, Segment, SourceId);
  if (PciDataIndex == (UINTN)-1) {
    //
    // Register new
    //

    if (PciDeviceInfo->PciDeviceDataNumber >= PciDeviceInfo->PciDeviceDataMaxNumber) {
      //
      // Reallocate
      //
      PciDeviceDataSize = sizeof(*NewPciDeviceData) * (PciDeviceInfo->PciDeviceDataMaxNumber + MAX_VTD_PCI_DATA_NUMBER);
      DEBUG ((DEBUG_INFO, "New PciDeviceDataSize:%d Page:%d\n", PciDeviceDataSize, EFI_SIZE_TO_PAGES (PciDeviceDataSize)));
      NewPciDeviceData = AllocateZeroPages (EFI_SIZE_TO_PAGES(PciDeviceDataSize));
      if (NewPciDeviceData == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      PciDeviceInfo->PciDeviceDataMaxNumber += MAX_VTD_PCI_DATA_NUMBER;
      if (PciDeviceInfo->PciDeviceData != 0) {
        CopyMem (NewPciDeviceData, (VOID *) (UINTN) PciDeviceInfo->PciDeviceData, sizeof (*NewPciDeviceData) * PciDeviceInfo->PciDeviceDataNumber);
        FreePages((VOID *) (UINTN) PciDeviceInfo->PciDeviceData, PciDeviceInfo->PciDeviceDataPageSize);
      }
      PciDeviceInfo->PciDeviceData = (UINT32) (UINTN) NewPciDeviceData;
      PciDeviceInfo->PciDeviceDataPageSize = (UINT32) EFI_SIZE_TO_PAGES (PciDeviceDataSize);
    }

    ASSERT (PciDeviceInfo->PciDeviceDataNumber < PciDeviceInfo->PciDeviceDataMaxNumber);

    PciDeviceDataBase = (PEI_PCI_DEVICE_DATA *) (UINTN) PciDeviceInfo->PciDeviceData;
    PciSourceId = &PciDeviceDataBase[PciDeviceInfo->PciDeviceDataNumber].PciSourceId;
    PciSourceId->Bits.Bus = SourceId.Bits.Bus;
    PciSourceId->Bits.Device = SourceId.Bits.Device;
    PciSourceId->Bits.Function = SourceId.Bits.Function;

    DEBUG ((DEBUG_INFO, "  RegisterPciDevice: PCI S%04x B%02x D%02x F%02x", Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));

    PciDeviceDataBase[PciDeviceInfo->PciDeviceDataNumber].DeviceType = DeviceType;

    if ((DeviceType != EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_ENDPOINT) &&
        (DeviceType != EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_BRIDGE)) {
      DEBUG ((DEBUG_INFO, " (*)"));
    }
    DEBUG ((DEBUG_INFO, "\n"));

    PciDeviceInfo->PciDeviceDataNumber++;
  } else {
    if (CheckExist) {
      DEBUG ((DEBUG_INFO, "  RegisterPciDevice: PCI S%04x B%02x D%02x F%02x already registered\n", Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
      return EFI_ALREADY_STARTED;
    }
  }

  return EFI_SUCCESS;
}

/**
  Process DMAR DHRD table.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
  @param[in]  DmarDrhd          The DRHD table.

**/
VOID
ProcessDhrd (
  IN VTD_UNIT_INFO              *VTdUnitInfo,
  IN EFI_ACPI_DMAR_DRHD_HEADER  *DmarDrhd
  )
{
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER     *DmarDevScopeEntry;
  UINT8                                           Bus;
  UINT8                                           Device;
  UINT8                                           Function;
  EFI_STATUS                                      Status;
  VTD_SOURCE_ID                                   SourceId;

  DEBUG ((DEBUG_INFO,"  VTD BaseAddress -  0x%016lx\n", DmarDrhd->RegisterBaseAddress));
  VTdUnitInfo->VtdUnitBaseAddress = (UINT32) DmarDrhd->RegisterBaseAddress;

  VTdUnitInfo->EnableQueuedInvalidation = 0;

  DEBUG ((DEBUG_INFO,"  VTD Segment - %d\n", DmarDrhd->SegmentNumber));
  VTdUnitInfo->Segment = DmarDrhd->SegmentNumber;

  VTdUnitInfo->FixedSecondLevelPagingEntry = 0;
  VTdUnitInfo->RmrrSecondLevelPagingEntry = 0;
  VTdUnitInfo->RootEntryTable = 0;
  VTdUnitInfo->ExtRootEntryTable = 0;
  VTdUnitInfo->RootEntryTablePageSize = 0;
  VTdUnitInfo->ExtRootEntryTablePageSize = 0;

  VTdUnitInfo->PciDeviceInfo.IncludeAllFlag = 0;
  VTdUnitInfo->PciDeviceInfo.PciDeviceDataMaxNumber = 0;
  VTdUnitInfo->PciDeviceInfo.PciDeviceDataNumber = 0;
  VTdUnitInfo->PciDeviceInfo.PciDeviceDataPageSize = 0;
  VTdUnitInfo->PciDeviceInfo.PciDeviceData = 0;

  if ((DmarDrhd->Flags & EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_PCI_ALL) != 0) {
    VTdUnitInfo->PciDeviceInfo.IncludeAllFlag = TRUE;
    DEBUG ((DEBUG_INFO,"  ProcessDhrd: with INCLUDE ALL\n"));
  } else {
    VTdUnitInfo->PciDeviceInfo.IncludeAllFlag = FALSE;
    DEBUG ((DEBUG_INFO,"  ProcessDhrd: without INCLUDE ALL\n"));
  }

  VTdUnitInfo->PciDeviceInfo.PciDeviceDataNumber = 0;
  VTdUnitInfo->PciDeviceInfo.PciDeviceDataMaxNumber = 0;
  VTdUnitInfo->PciDeviceInfo.PciDeviceDataPageSize = 0;
  VTdUnitInfo->PciDeviceInfo.PciDeviceData = 0;

  DmarDevScopeEntry = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) ((UINTN) (DmarDrhd + 1));
  while ((UINTN)DmarDevScopeEntry < (UINTN) DmarDrhd + DmarDrhd->Header.Length) {

    Status = GetPciBusDeviceFunction (DmarDrhd->SegmentNumber, DmarDevScopeEntry, &Bus, &Device, &Function);
    if (EFI_ERROR (Status)) {
      return;
    }

    DEBUG ((DEBUG_INFO,"  ProcessDhrd: "));
    switch (DmarDevScopeEntry->Type) {
      case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_ENDPOINT:
      DEBUG ((DEBUG_INFO,"PCI Endpoint"));
      break;
    case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_BRIDGE:
      DEBUG ((DEBUG_INFO,"PCI-PCI bridge"));
      break;
    case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC:
      DEBUG ((DEBUG_INFO,"IOAPIC"));
      break;
    case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_MSI_CAPABLE_HPET:
      DEBUG ((DEBUG_INFO,"MSI Capable HPET"));
      break;
    case EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ACPI_NAMESPACE_DEVICE:
      DEBUG ((DEBUG_INFO,"ACPI Namespace Device"));
      break;
    }
    DEBUG ((DEBUG_INFO," S%04x B%02x D%02x F%02x\n", DmarDrhd->SegmentNumber, Bus, Device, Function));

    SourceId.Bits.Bus = Bus;
    SourceId.Bits.Device = Device;
    SourceId.Bits.Function = Function;

    Status = RegisterPciDevice (VTdUnitInfo, DmarDrhd->SegmentNumber, SourceId, DmarDevScopeEntry->Type, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,"RegisterPciDevice Failed !\n"));
    }

    DmarDevScopeEntry = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) ((UINTN) DmarDevScopeEntry + DmarDevScopeEntry->Length);
  }
}

/**
  Dump the PCI device information managed by this VTd engine.

  @param[in]  VTdInfo           The VTd engine context information.
  @param[in]  VtdIndex          The index of VTd engine.

**/
VOID
DumpPciDeviceInfo (
  IN VTD_INFO                   *VTdInfo,
  IN UINTN                      VtdIndex
  )
{
  UINTN                         Index;
  PEI_PCI_DEVICE_DATA           *PciDeviceDataBase;

  DEBUG ((DEBUG_INFO,"PCI Device Information (Number 0x%x, IncludeAll - %d):\n",
    VTdInfo->VtdUnitInfo[VtdIndex].PciDeviceInfo.PciDeviceDataNumber,
    VTdInfo->VtdUnitInfo[VtdIndex].PciDeviceInfo.IncludeAllFlag
    ));

  PciDeviceDataBase = (PEI_PCI_DEVICE_DATA *) (UINTN) VTdInfo->VtdUnitInfo[VtdIndex].PciDeviceInfo.PciDeviceData;

  for (Index = 0; Index < VTdInfo->VtdUnitInfo[VtdIndex].PciDeviceInfo.PciDeviceDataNumber; Index++) {
    DEBUG ((DEBUG_INFO,"  S%04x B%02x D%02x F%02x\n",
      VTdInfo->VtdUnitInfo[VtdIndex].Segment,
      PciDeviceDataBase[Index].PciSourceId.Bits.Bus,
      PciDeviceDataBase[Index].PciSourceId.Bits.Device,
      PciDeviceDataBase[Index].PciSourceId.Bits.Function
      ));
  }
}

/**
  Parse DMAR DRHD table.

  @param[in]  AcpiDmarTable     DMAR ACPI table

  @return EFI_SUCCESS  The DMAR DRHD table is parsed.

**/
EFI_STATUS
ParseDmarAcpiTableDrhd (
  IN EFI_ACPI_DMAR_HEADER               *AcpiDmarTable
  )
{
  EFI_ACPI_DMAR_STRUCTURE_HEADER        *DmarHeader;
  UINTN                                 VtdUnitNumber;
  UINTN                                 VtdIndex;
  VTD_INFO                              *VTdInfo;

  VtdUnitNumber = GetVtdEngineNumber (AcpiDmarTable);
  if (VtdUnitNumber == 0) {
    return EFI_UNSUPPORTED;
  }

  VTdInfo = BuildGuidHob (&mVTdInfoGuid, sizeof (VTD_INFO) + (VtdUnitNumber - 1) * sizeof (VTD_UNIT_INFO));
  ASSERT(VTdInfo != NULL);
  if (VTdInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize the engine mask to all.
  //
  VTdInfo->AcpiDmarTable    = (UINT32) (UINTN) AcpiDmarTable;
  VTdInfo->HostAddressWidth = AcpiDmarTable->HostAddressWidth;
  VTdInfo->VTdEngineCount   = (UINT32) VtdUnitNumber;

  VtdIndex = 0;
  DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *) ((UINTN) (AcpiDmarTable + 1));
  while ((UINTN) DmarHeader < (UINTN) AcpiDmarTable + AcpiDmarTable->Header.Length) {
    switch (DmarHeader->Type) {
    case EFI_ACPI_DMAR_TYPE_DRHD:
      ASSERT (VtdIndex < VtdUnitNumber);
      ProcessDhrd (&VTdInfo->VtdUnitInfo[VtdIndex], (EFI_ACPI_DMAR_DRHD_HEADER *) DmarHeader);
      VtdIndex++;

      break;

    default:
      break;
    }
    DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *) ((UINTN) DmarHeader + DmarHeader->Length);
  }
  ASSERT (VtdIndex == VtdUnitNumber);

  for (VtdIndex = 0; VtdIndex < VtdUnitNumber; VtdIndex++) {
    DumpPciDeviceInfo (VTdInfo, VtdIndex);
  }

  return EFI_SUCCESS;
}


/**
  Process DMAR RMRR table.

  @param[in]  VTdInfo           The VTd engine context information.
  @param[in]  DmarRmrr          The RMRR table.

**/
VOID
ProcessRmrr (
  IN VTD_INFO                                     *VTdInfo,
  IN EFI_ACPI_DMAR_RMRR_HEADER                    *DmarRmrr
  )
{
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER     *DmarDevScopeEntry;
  UINT8                                           Bus;
  UINT8                                           Device;
  UINT8                                           Function;
  EFI_STATUS                                      Status;
  VTD_SOURCE_ID                                   SourceId;

  DEBUG ((DEBUG_INFO,"  PEI RMRR (Base 0x%016lx, Limit 0x%016lx)\n", DmarRmrr->ReservedMemoryRegionBaseAddress, DmarRmrr->ReservedMemoryRegionLimitAddress));

  if ((DmarRmrr->ReservedMemoryRegionBaseAddress == 0) ||
      (DmarRmrr->ReservedMemoryRegionLimitAddress == 0)) {
    return ;
  }

  DmarDevScopeEntry = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) ((UINTN) (DmarRmrr + 1));
  while ((UINTN) DmarDevScopeEntry < (UINTN) DmarRmrr + DmarRmrr->Header.Length) {
    if (DmarDevScopeEntry->Type != EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_ENDPOINT) {
      DEBUG ((DEBUG_INFO,"RMRR DevScopeEntryType is not endpoint, type[0x%x] \n", DmarDevScopeEntry->Type));
      return;
    }

    Status = GetPciBusDeviceFunction (DmarRmrr->SegmentNumber, DmarDevScopeEntry, &Bus, &Device, &Function);
    if (EFI_ERROR (Status)) {
      continue;
    }

    DEBUG ((DEBUG_INFO,"RMRR S%04x B%02x D%02x F%02x\n", DmarRmrr->SegmentNumber, Bus, Device, Function));

    SourceId.Bits.Bus = Bus;
    SourceId.Bits.Device = Device;
    SourceId.Bits.Function = Function;

    Status = EnableRmrrPageAttribute (
               VTdInfo,
               DmarRmrr->SegmentNumber,
               SourceId,
               DmarRmrr->ReservedMemoryRegionBaseAddress,
               DmarRmrr->ReservedMemoryRegionLimitAddress,
               EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "EnableRmrrPageAttribute : %r\n", Status));
    }

    DmarDevScopeEntry = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) ((UINTN) DmarDevScopeEntry + DmarDevScopeEntry->Length);
  }
}

/**
  Parse DMAR DRHD table.

  @param[in]  VTdInfo           The VTd engine context information.

**/
VOID
ParseDmarAcpiTableRmrr (
  IN VTD_INFO                           *VTdInfo
  )
{
  EFI_ACPI_DMAR_HEADER                  *AcpiDmarTable;
  EFI_ACPI_DMAR_STRUCTURE_HEADER        *DmarHeader;

  AcpiDmarTable = (EFI_ACPI_DMAR_HEADER *) (UINTN) VTdInfo->AcpiDmarTable;

  DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *) ((UINTN) (AcpiDmarTable + 1));
  while ((UINTN) DmarHeader < (UINTN) AcpiDmarTable + AcpiDmarTable->Header.Length) {
    switch (DmarHeader->Type) {
    case EFI_ACPI_DMAR_TYPE_RMRR:
      ProcessRmrr (VTdInfo, (EFI_ACPI_DMAR_RMRR_HEADER *) DmarHeader);
      break;
    default:
      break;
    }
    DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *) ((UINTN) DmarHeader + DmarHeader->Length);
  }
}

