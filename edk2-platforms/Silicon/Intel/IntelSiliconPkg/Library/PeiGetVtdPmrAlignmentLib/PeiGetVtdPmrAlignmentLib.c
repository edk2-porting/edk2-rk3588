/** @file
  Library to get Global VTd PMR alignment information.
  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiGetVtdPmrAlignmentLib.h>
#include <Library/PeiServicesLib.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>
#include <IndustryStandard/Vtd.h>

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_STRUCT;

/**
  Get the protected low memory alignment.

  @param HostAddressWidth   The host address width.
  @param VtdUnitBaseAddress The base address of the VTd engine.

  @return protected low memory alignment. Ex: 0x100000
**/
UINT32
GetGlobalVTdPlmrAlignment (
  IN UINT8         HostAddressWidth,
  IN UINTN         VtdUnitBaseAddress
  )
{
  UINT32        Data32;

  MmioWrite32 (VtdUnitBaseAddress + R_PMEN_LOW_BASE_REG, 0xFFFFFFFF);
  Data32 = MmioRead32 (VtdUnitBaseAddress + R_PMEN_LOW_BASE_REG);
  Data32 = ~Data32 + 1;

  return Data32;
}

/**
  Get the protected high memory alignment.

  @param HostAddressWidth   The host address width.
  @param VtdUnitBaseAddress The base address of the VTd engine.

  @return protected high memory alignment. Ex: 0x100000
**/
UINT64_STRUCT
GetGlobalVTdPhmrAlignment (
  IN UINT8         HostAddressWidth,
  IN UINTN         VtdUnitBaseAddress
  )
{
  UINT64_STRUCT        Data64;

  MmioWrite64 (VtdUnitBaseAddress + R_PMEN_HIGH_BASE_REG, 0xFFFFFFFFFFFFFFFF);
  Data64.Data = MmioRead64 (VtdUnitBaseAddress + R_PMEN_HIGH_BASE_REG);
  Data64.Data = ~Data64.Data + 1;
  Data64.Data = Data64.Data & (LShiftU64 (1, HostAddressWidth) - 1);

  return Data64;
}

/**
  Get the global VT-d protected memory alignment.
  @return The maximum protected memory alignment. Ex: 0x100000
**/
UINTN
EFIAPI
GetGlobalVtdPmrAlignment (
)
{
  UINT32                            LowMemoryAlignment;
  UINT64_STRUCT                     HighMemoryAlignment;
  UINTN                             MemoryAlignment;
  UINT32                            GlobalVTdBaseAddress;
  EFI_STATUS                        Status;
  UINTN                             VtdIndex;
  EFI_ACPI_DMAR_STRUCTURE_HEADER    *DmarHeader;
  EFI_ACPI_DMAR_DRHD_HEADER         *DrhdHeader;
  EFI_ACPI_DMAR_HEADER              *AcpiDmarTable;

  //
  // Initialization
  //
  GlobalVTdBaseAddress = 0xFFFFFFFF;
  LowMemoryAlignment = 0;
  HighMemoryAlignment.Data = 0;
  MemoryAlignment = 0;
  Status = EFI_UNSUPPORTED;
  VtdIndex = 0;
  DmarHeader = NULL;
  DrhdHeader = NULL;
  AcpiDmarTable = NULL;

  //
  // Fetch the PEI DMAR ACPI Table that created and installed in PlatformVTdInfoSamplePei.c
  //
  Status = PeiServicesLocatePpi (
             &gEdkiiVTdInfoPpiGuid,
             0,
             NULL,
             (VOID **)&AcpiDmarTable
             );
  if (EFI_ERROR (Status)) {

    DEBUG ((DEBUG_ERROR, "PeiServicesLocatePpi gEdkiiVTdInfoPpiGuid failed\n"));
    Status = EFI_NOT_FOUND;
    MemoryAlignment = SIZE_1MB;

  } else {

    //
    // Seatch the DRHD structure with INCLUDE_PCI_ALL flag Set -> Global VT-d
    //
    DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *)((UINTN)(AcpiDmarTable + 1));
    while ((UINTN)DmarHeader < (UINTN)AcpiDmarTable + AcpiDmarTable->Header.Length) {
      switch (DmarHeader->Type) {
      case EFI_ACPI_DMAR_TYPE_DRHD:
        DrhdHeader = (EFI_ACPI_DMAR_DRHD_HEADER *) DmarHeader;
        if ((DrhdHeader->Flags & EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_PCI_ALL) == EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_PCI_ALL) {
          GlobalVTdBaseAddress = (UINT32)DrhdHeader->RegisterBaseAddress;
          DEBUG ((DEBUG_INFO,"  GlobalVTdBaseAddress: %x\n", GlobalVTdBaseAddress));
        }
        VtdIndex++;

        break;

      default:
        break;
      }
      DmarHeader = (EFI_ACPI_DMAR_STRUCTURE_HEADER *)((UINTN)DmarHeader + DmarHeader->Length);
    }

    if (GlobalVTdBaseAddress == 0xFFFFFFFF) {

      DEBUG ((DEBUG_ERROR, "Error! Please set INCLUDE_PCI_ALL flag to your Global VT-d\n"));
      MemoryAlignment = SIZE_1MB;

    } else {
      //
      // Get the alignment information from VT-d register
      //
      LowMemoryAlignment = GetGlobalVTdPlmrAlignment (AcpiDmarTable->HostAddressWidth, GlobalVTdBaseAddress);
      HighMemoryAlignment = GetGlobalVTdPhmrAlignment (AcpiDmarTable->HostAddressWidth, GlobalVTdBaseAddress);
      if (LowMemoryAlignment < HighMemoryAlignment.Data) {
        MemoryAlignment = (UINTN)HighMemoryAlignment.Data;
      } else {
        MemoryAlignment = LowMemoryAlignment;
      }
    }
  }

  return MemoryAlignment;
}
