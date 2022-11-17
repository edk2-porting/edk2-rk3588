/** @file
  Acpi Table generator headers

  Copyright 2020 NXP
  Copyright 2020 Puresoftware Ltd

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef PLATFORM_ACPI_TABLE_GENERATOR_H
#define PLATFORM_ACPI_TABLE_GENERATOR_H

typedef enum PlatAcpiTableId {
  PlatAcpiTableIdReserved = 0x0000,             ///< Reserved
  PlatAcpiTableIdDsdt,
  PlatAcpiTableIdMax
} PLAT_ACPI_TABLE_ID;

#endif // PLATFORM_ACPI_TABLE_GENERATOR_H
