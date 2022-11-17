/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2018, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/

#include "Hi1620Platform.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 2, "HISI  ", "HIP08   ", EFI_ACPI_ARM_OEM_REVISION) {
  include ("Com.asl")
  include ("CPU.asl")
  include ("Hi1620Pci.asl")
  include ("Hi1620Mbig.asl")
  include ("Hi1620Rde.asl")
  include ("Hi1620Sec.asl")
  include ("ipmi.asl")
  include ("LpcUart_clk.asl")
  include ("Hi1620Ged.asl")
  include ("Hi1620Power.asl")
  include ("Hi1620Apei.asl")
  include ("Hi1620Mctp.asl")
  include ("Pv680UncorePmu.asl")
}
