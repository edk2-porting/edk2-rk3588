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
#include <IndustryStandard/WindowsSmmSecurityMitigationTable.h>

VOID
DumpCharArray (
  IN CHAR8  *Ch,
  IN UINTN  Size
  );

VOID
DumpAcpiTableHeader (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Table
  );

VOID
DumpAcpiWsmt (
  IN EFI_ACPI_WSMT_TABLE  *Wsmt
  )
{
  DumpAcpiTableHeader (&Wsmt->Header);
  DEBUG ((DEBUG_INFO, "         "));
  DEBUG ((DEBUG_INFO, " ProtectionFlags=0x%08x\n", Wsmt->ProtectionFlags));
}
