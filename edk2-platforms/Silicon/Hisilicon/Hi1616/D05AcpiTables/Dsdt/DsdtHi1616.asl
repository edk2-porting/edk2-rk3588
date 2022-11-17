/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015-2016, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015-2016, Linaro Limited. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/

#include "Hi1616Platform.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 2, "HISI  ", "HIP07   ", EFI_ACPI_ARM_OEM_REVISION) {
  include ("Lpc.asl")
  include ("D05Mbig.asl")
  include ("Com.asl")
  include ("CPU.asl")
  include ("D05I2c.asl")
  include ("D05Usb.asl")
  include ("D05Hns.asl")
  include ("D05Sas.asl")
  include ("D05Pci.asl")
}
