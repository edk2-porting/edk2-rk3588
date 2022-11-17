/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>

VOID
DumpCharArray (
  IN CHAR8  *Ch,
  IN UINTN  Size
  );

VOID
DumpAcpiTableHeader (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Table
  );

BOOLEAN
IsMmioExit (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN BOOLEAN               CheckAllocated
  );

CHAR8 *mDmarTypeString[] = {
  "DRHD  ",
  "RMRR  ",
  "ATSR  ",
  "RHSA  ",
  "ANDD  ",
};

STATIC CHAR8 mUnknownStr[11];

CHAR8 *
ShortNameOfDmarType(
  IN UINT16 Type
  )
{
  if (Type < sizeof(mDmarTypeString) / sizeof(mDmarTypeString[0])) {
    return mDmarTypeString[Type];
  } else {
    AsciiSPrint(mUnknownStr, sizeof(mUnknownStr), "[%04x]", Type);
    return mUnknownStr;
  }
}

CHAR8 *mDmarDeviceTypeString[] = {
  "[00]",
  "PCI",
  "P2P",
  "IOAPIC",
  "HPET",
  "ACPI",
};

CHAR8 *
ShortNameOfDmarDeviceType(
  IN UINT8 Type
  )
{
  if (Type < sizeof(mDmarDeviceTypeString) / sizeof(mDmarDeviceTypeString[0])) {
    return mDmarDeviceTypeString[Type];
  } else {
    AsciiSPrint(mUnknownStr, sizeof(mUnknownStr), "[%02x]", Type);
    return mUnknownStr;
  }
}

VOID
DumpDmarDeviceScope (
  IN EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *DmarDeviceScope,
  IN UINTN                                       Size
  )
{
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *Device;
  EFI_ACPI_DMAR_PCI_PATH                      *PciPathScope;
  EFI_ACPI_DMAR_PCI_PATH                      *PciPath;
  UINTN                                       PciPathSize;

  Device = DmarDeviceScope;
  while ((UINTN)Device < (UINTN)DmarDeviceScope + Size) {
    DEBUG ((DEBUG_INFO, " ("));
    DEBUG ((DEBUG_INFO, ShortNameOfDmarDeviceType(Device->Type)));
    DEBUG ((DEBUG_INFO, ": [0x%02x]", Device->Type));
    DEBUG ((DEBUG_INFO, " EnumId=0x%02x", Device->EnumerationId));
    DEBUG ((DEBUG_INFO, " Bus=0x%02x", Device->StartBusNumber));

    PciPathScope = (EFI_ACPI_DMAR_PCI_PATH *)(DmarDeviceScope + 1);
    PciPathSize = Device->Length - sizeof(EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER);
    PciPath = PciPathScope;
    while ((UINTN)PciPath < (UINTN)PciPathScope + PciPathSize) {
      DEBUG ((DEBUG_INFO, " Pci=0x%02x/0x%02x", PciPath->Device, PciPath->Function));
      PciPath ++;
    }

    DEBUG ((DEBUG_INFO, ")"));
    Device = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *)((UINTN)Device + Device->Length);
  }
}

VOID
DumpAcpiDmar (
  IN EFI_ACPI_DMAR_HEADER  *Dmar
  )
{
  EFI_ACPI_DMAR_STRUCTURE_HEADER        *DmarStructHeader;
  INTN                                  DmarLen;
  EFI_ACPI_DMAR_DRHD_HEADER             *Drhd;
  EFI_ACPI_DMAR_RMRR_HEADER             *Rmrr;
  EFI_ACPI_DMAR_ATSR_HEADER             *Atsr;
  EFI_ACPI_DMAR_RHSA_HEADER             *Rhsa;
  EFI_ACPI_DMAR_ANDD_HEADER             *Andd;

  DumpAcpiTableHeader (&Dmar->Header);
  DEBUG ((DEBUG_INFO, "         "));
  DEBUG ((DEBUG_INFO, " HostAddressWidth=0x%02x Flags=0x%02x\n", Dmar->HostAddressWidth, Dmar->Flags));
    
  //
  // Sub table
  //
  DmarLen  = Dmar->Header.Length - sizeof(EFI_ACPI_DMAR_HEADER);
  DmarStructHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *)(Dmar + 1);
  while (DmarLen > 0) {
    switch (DmarStructHeader->Type) {
    case EFI_ACPI_DMAR_TYPE_DRHD:
      Drhd = (EFI_ACPI_DMAR_DRHD_HEADER *)DmarStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfDmarType(DmarStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%04x]", DmarStructHeader->Type));
      DEBUG ((DEBUG_INFO, " Flags=0x%02x", Drhd->Flags));
      DEBUG ((DEBUG_INFO, " Segment=0x%04x", Drhd->SegmentNumber));
      DEBUG ((DEBUG_INFO, " Register=0x%016lx", Drhd->RegisterBaseAddress));
      if ((Drhd->Flags & EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_PCI_ALL) != 0) {
        DEBUG ((DEBUG_INFO, " (INCLUDE_PCI_ALL)"));
      } else {
        DumpDmarDeviceScope ((EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *)(Drhd + 1), Drhd->Header.Length - sizeof(EFI_ACPI_DMAR_DRHD_HEADER));
      }
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_DMAR_TYPE_RMRR:
      Rmrr = (EFI_ACPI_DMAR_RMRR_HEADER *)DmarStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfDmarType(DmarStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%04x]", DmarStructHeader->Type));
      DEBUG ((DEBUG_INFO, " Segment=0x%04x", Rmrr->SegmentNumber));
      DEBUG ((DEBUG_INFO, " Reserved=(0x%016lx-0x%016lx)", Rmrr->ReservedMemoryRegionBaseAddress, Rmrr->ReservedMemoryRegionLimitAddress));
      DumpDmarDeviceScope ((EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *)(Rmrr + 1), Rmrr->Header.Length - sizeof(EFI_ACPI_DMAR_RMRR_HEADER));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_DMAR_TYPE_ATSR:
      Atsr = (EFI_ACPI_DMAR_ATSR_HEADER *)DmarStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfDmarType(DmarStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%04x]", DmarStructHeader->Type));
      DEBUG ((DEBUG_INFO, " Flags=0x%02x", Atsr->Flags));
      DEBUG ((DEBUG_INFO, " Segment=0x%04x", Atsr->SegmentNumber));
      if ((Atsr->Flags & EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS) != 0) {
        DEBUG ((DEBUG_INFO, " (ALL_PORTS)"));
      } else {
        DumpDmarDeviceScope ((EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *)(Atsr + 1), Atsr->Header.Length - sizeof(EFI_ACPI_DMAR_ATSR_HEADER));
      }
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_DMAR_TYPE_RHSA:
      Rhsa = (EFI_ACPI_DMAR_RHSA_HEADER *)DmarStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfDmarType(DmarStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%04x]", DmarStructHeader->Type));
      DEBUG ((DEBUG_INFO, " Register=0x%016lx", Rhsa->RegisterBaseAddress));
      DEBUG ((DEBUG_INFO, " ProximityDomain=0x%08x", Rhsa->ProximityDomain));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    case EFI_ACPI_DMAR_TYPE_ANDD:
      Andd = (EFI_ACPI_DMAR_ANDD_HEADER *)DmarStructHeader;
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfDmarType(DmarStructHeader->Type)));
      DEBUG ((DEBUG_INFO, ": [0x%04x]", DmarStructHeader->Type));
      DEBUG ((DEBUG_INFO, " DeviceNumber=0x%02x", Andd->AcpiDeviceNumber));
      DEBUG ((DEBUG_INFO, " ObjectName=%a", Andd + 1));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    default:
      DEBUG ((DEBUG_INFO, "         "));
      DEBUG ((DEBUG_INFO, " "));
      DEBUG ((DEBUG_INFO, ShortNameOfDmarType(DmarStructHeader->Type)));
      DEBUG ((DEBUG_INFO, "\n"));
      break;
    }
    DmarStructHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *)((UINT8 *)DmarStructHeader + DmarStructHeader->Length);
    DmarLen         -= DmarStructHeader->Length;
  }
}

EFI_STATUS
CheckAcpiDmar (
  IN EFI_ACPI_DMAR_HEADER  *Dmar
  )
{
  EFI_ACPI_DMAR_STRUCTURE_HEADER        *DmarStructHeader;
  INTN                                  DmarLen;
  EFI_ACPI_DMAR_DRHD_HEADER             *Drhd;
    
  DmarLen  = Dmar->Header.Length - sizeof(EFI_ACPI_DMAR_HEADER);
  DmarStructHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *)(Dmar + 1);
  while (DmarLen > 0) {
    switch (DmarStructHeader->Type) {
    case EFI_ACPI_DMAR_TYPE_DRHD:
      Drhd = (EFI_ACPI_DMAR_DRHD_HEADER *)DmarStructHeader;
      if (!IsMmioExit (Drhd->RegisterBaseAddress, SIZE_4KB, TRUE)) {
        DEBUG ((DEBUG_ERROR, "DMAR.DRHD resource (0x%x) is not reported correctly.\n", Drhd->RegisterBaseAddress));
        return EFI_NOT_STARTED;
      }
      break;
    default:
      break;
    }
    DmarStructHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *)((UINT8 *)DmarStructHeader + DmarStructHeader->Length);
    DmarLen         -= DmarStructHeader->Length;
  }
  return EFI_SUCCESS;
}