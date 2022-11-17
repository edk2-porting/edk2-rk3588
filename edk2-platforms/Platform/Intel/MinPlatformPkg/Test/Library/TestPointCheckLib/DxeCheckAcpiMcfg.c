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
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>

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

EFI_STATUS
DumpAcpiMcfg (
  IN EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER  *Mcfg
  )
{
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE   *Struct;
  UINTN                                                                                   Count;
  UINTN                                                                                   Index;

  DumpAcpiTableHeader (&Mcfg->Header);

  Count = Mcfg->Header.Length - sizeof(EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER);
  Count = Count / sizeof(EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE);
  Struct = (VOID *)(Mcfg + 1);
  for (Index = 0; Index < Count; Index++) {
    DEBUG ((DEBUG_INFO, "         "));
    DEBUG ((DEBUG_INFO, " Segment :"));
    DEBUG ((DEBUG_INFO, " BaseAddress=0x%016lx", Struct->BaseAddress));
    DEBUG ((DEBUG_INFO, " Seg=0x%04x Bus=(0x%02x-0x%02x)",
      Struct->PciSegmentGroupNumber,
      Struct->StartBusNumber,
      Struct->EndBusNumber
      ));
    DEBUG ((DEBUG_INFO, "\n"));
    Struct++;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
CheckAcpiMcfg (
  IN EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER  *Mcfg
  )
{
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE   *Struct;
  UINTN                                                                                   Count;
  UINTN                                                                                   Index;

  Count = Mcfg->Header.Length - sizeof(EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER);
  Count = Count / sizeof(EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE);
  Struct = (VOID *)(Mcfg + 1);
  for (Index = 0; Index < Count; Index++) {
    if (!IsMmioExit (Struct->BaseAddress, (Struct->EndBusNumber - Struct->StartBusNumber + 1) * SIZE_1MB, TRUE)) {
      DEBUG ((DEBUG_ERROR, "MCFG resource (0x%x) is not reported correctly.\n", Struct->BaseAddress));
      return EFI_NOT_STARTED;
    }
    Struct++;
  }
  return EFI_SUCCESS;
}