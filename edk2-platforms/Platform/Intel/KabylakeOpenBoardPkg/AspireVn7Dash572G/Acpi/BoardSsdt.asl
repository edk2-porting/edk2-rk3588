/** @file
  This file contains the Aspire VN7-572G SSDT Table ASL code.

  Copyright (c) 2021, Baruch Binyamin Doron
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock (
  "Board.aml",
  "SSDT",
  0x02,
  "ACRSKL",
  "AcerSKL ",
  0x20141018
  )
{
  External (\MDBG, MethodObj)

  // Debug print helper
  Method (DBGH, 1)
  {
    // If present, print to ACPI debug feature's buffer
    If (CondRefOf (\MDBG))
    {
      \MDBG (Arg0)
    }
    // Always use "Debug" object for operating system
    Debug = Arg0
  }

  Include ("ec.asl")
  Include ("mainboard.asl")
}
