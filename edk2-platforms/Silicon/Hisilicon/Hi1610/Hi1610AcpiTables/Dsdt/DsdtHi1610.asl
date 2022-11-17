/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/

#include "Hi1610Platform.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 1, "HISI  ", "HIP06   ", EFI_ACPI_ARM_OEM_REVISION) {
     include ("Lpc.asl")
     include ("D03Mbig.asl")
     include ("CPU.asl")
     include ("D03Usb.asl")
     include ("D03Hns.asl")
     include ("D03Sas.asl")
     include ("D03Pci.asl")
}
