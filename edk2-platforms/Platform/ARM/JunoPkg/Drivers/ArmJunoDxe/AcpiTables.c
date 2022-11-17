/** @file

  This file contains support for ACPI Tables that are generated at boot time.

  Copyright (c) 2015, ARM Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "ArmPlatform.h"
#include "ArmJunoDxeInternal.h"

#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>

/*
 * Memory Mapped Configuration Space Access Table (MCFG)
 */
typedef struct {
  EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER                        Header;
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE Entry;
} MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ACCESS_TABLE;

MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ACCESS_TABLE mAcpiMcfgTable = {
    {
        ARM_ACPI_HEADER (
          EFI_ACPI_5_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE,
          MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ACCESS_TABLE,
          EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_SPACE_ACCESS_TABLE_REVISION
        ),
        0, // Reserved
    }, {
        FixedPcdGet32 (PcdPciConfigurationSpaceBaseAddress),
        0, // PciSegmentGroupNumber
        FixedPcdGet32 (PcdPciBusMin),
        FixedPcdGet32 (PcdPciBusMax),
        0 // Reserved;
    }
};

/**
 * Callback called when ACPI Protocol is installed
 */
VOID
AcpiPciNotificationEvent (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  )
{
  EFI_STATUS                Status;
  EFI_ACPI_TABLE_PROTOCOL   *AcpiTableProtocol;
  UINTN                     AcpiTableKey;

  //
  // Ensure the ACPI protocol is installed
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  (VOID**)&AcpiTableProtocol
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Install MCFG Table
  //
  AcpiTableKey = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (AcpiTableProtocol, &mAcpiMcfgTable, sizeof (mAcpiMcfgTable), &AcpiTableKey);
  ASSERT_EFI_ERROR (Status);
}
