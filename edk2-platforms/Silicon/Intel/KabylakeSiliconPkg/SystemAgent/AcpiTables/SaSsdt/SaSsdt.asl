/** @file
  This file contains the SystemAgent SSDT Table ASL code.
  It defines a Global NVS table which exchanges datas between OS
  and BIOS.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock (
  "SaSsdt.aml",
  "SSDT",
  0x02,
  "SaSsdt",
  "SaSsdt ",
  0x3000
  )
{
  include ("SaNvs.asl")
  include ("Sa.asl")
}
