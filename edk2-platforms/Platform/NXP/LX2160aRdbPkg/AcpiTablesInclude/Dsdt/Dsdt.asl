/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2020 NXP
  Copyright 2020 Puresoftware Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Platform.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 2, "NXP  ", "LX2160  ", EFI_ACPI_ARM_OEM_REVISION) {
  include ("Clk.asl")
}
