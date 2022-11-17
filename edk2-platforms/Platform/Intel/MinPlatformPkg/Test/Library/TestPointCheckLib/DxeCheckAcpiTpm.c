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
#include <IndustryStandard/TcpaAcpi.h>
#include <IndustryStandard/Tpm2Acpi.h>

VOID
DumpCharArray (
  IN CHAR8  *Ch,
  IN UINTN  Size
  );

VOID
DumpAcpiTableHeader (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Table
  );

BOOLEAN
IsMmioExit (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN BOOLEAN               CheckAllocated
  );

VOID
DumpAcpiTpm2 (
  IN EFI_TPM2_ACPI_TABLE  *Tpm2
  )
{
  DumpAcpiTableHeader (&Tpm2->Header);
  DEBUG ((DEBUG_INFO, "         "));
  DEBUG ((DEBUG_INFO, " Flags=0x%08x ControlArea=0x%016lx StartMethod=0x%08x\n", Tpm2->Flags, Tpm2->AddressOfControlArea, Tpm2->StartMethod));
}

VOID
DumpAcpiTcpaClient (
  IN EFI_TCG_CLIENT_ACPI_TABLE  *TcpaClient
  )
{
  DumpAcpiTableHeader (&TcpaClient->Header);
  DEBUG ((DEBUG_INFO, "         "));
  DEBUG ((DEBUG_INFO, " PlatformClass=0x%04x Laml=0x%08x Lasa=0x%016lx\n",
    TcpaClient->PlatformClass, TcpaClient->Laml, TcpaClient->Lasa));
}

VOID
DumpAcpiTcpaServer (
  IN EFI_TCG_SERVER_ACPI_TABLE  *TcpaServer
  )
{
  DumpAcpiTableHeader (&TcpaServer->Header);
  DEBUG ((DEBUG_INFO, "         "));
  DEBUG ((DEBUG_INFO, " PlatformClass=0x%04x Laml=0x%016lx Lasa=0x%016lx\n",
    TcpaServer->PlatformClass, TcpaServer->Laml, TcpaServer->Lasa));
}

VOID
DumpAcpiTcpa (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Tcpa
  )
{
  EFI_TCG_CLIENT_ACPI_TABLE  *TcpaClient;
  TcpaClient = (VOID *)Tcpa;
  switch (TcpaClient->PlatformClass) {
  case TCG_PLATFORM_TYPE_CLIENT:
    DumpAcpiTcpaClient((VOID *)Tcpa);
    break;
  case TCG_PLATFORM_TYPE_SERVER:
    DumpAcpiTcpaServer((VOID *)Tcpa);
    break;
  default:
    DumpAcpiTableHeader (Tcpa);
    DEBUG ((DEBUG_INFO, "         "));
    DEBUG ((DEBUG_INFO, " PlatformClass=0x%04x\n", TcpaClient->PlatformClass));
    break;
  }
}

EFI_STATUS
CheckAcpiTpm2 (
  IN EFI_TPM2_ACPI_TABLE  *Tpm2
  )
{
  switch (Tpm2->StartMethod) {
  case EFI_TPM2_ACPI_TABLE_START_METHOD_TIS:
    if (!IsMmioExit (0xFED40000, SIZE_4KB, TRUE)) {
      DEBUG ((DEBUG_ERROR, "TPM2 resource (0x%x) is not reported correctly.\n", 0xFED40000));
      return EFI_NOT_STARTED;
    }
    break;
  default:
    break;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
CheckAcpiTcpaClient (
  IN EFI_TCG_CLIENT_ACPI_TABLE  *TcpaClient
  )
{
  if (!IsMmioExit (0xFED40000, SIZE_4KB, TRUE)) {
    DEBUG ((DEBUG_ERROR, "TCPA.client resource (0x%x) is not reported correctly.\n", 0xFED40000));
    return EFI_NOT_STARTED;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
CheckAcpiTcpaServer (
  IN EFI_TCG_SERVER_ACPI_TABLE  *TcpaServer
  )
{
  if (!IsMmioExit (TcpaServer->BaseAddress.Address, SIZE_4KB, TRUE)) {
    DEBUG ((DEBUG_ERROR, "TCPA.server resource (0x%x) is not reported correctly.\n", 0xFED40000));
    return EFI_NOT_STARTED;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
CheckAcpiTcpa (
  IN EFI_ACPI_DESCRIPTION_HEADER  *Tcpa
  )
{
  EFI_TCG_CLIENT_ACPI_TABLE  *TcpaClient;
  TcpaClient = (VOID *)Tcpa;
  switch (TcpaClient->PlatformClass) {
  case TCG_PLATFORM_TYPE_CLIENT:
    return CheckAcpiTcpaClient((VOID *)Tcpa);
    break;
  case TCG_PLATFORM_TYPE_SERVER:
    return CheckAcpiTcpaServer((VOID *)Tcpa);
    break;
  default:
    break;
  }
  return EFI_UNSUPPORTED;
}
