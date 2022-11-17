/** @file
  Phytium ACPI ASL Sources.

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Platform.h>

DefinitionBlock ("DsdtTable.aml", "DSDT", 2, "FT-LTD", "PHYTIUM ", EFI_ACPI_PHYTIUM_OEM_REVISION) {
  include ("Cpu.asl")
  include ("Uart.asl")
}
