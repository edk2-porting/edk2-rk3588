/** @file

  @copyright
  Copyright 2008 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_ACPI_UPDATE_LIB_H
#define _PLATFORM_ACPI_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#define PLATFORM_ACPI_FIX_UPDATE_SIGNATURE  SIGNATURE_32 ('A', 'C', 'P', 'F')
#define PLATFORM_ACPI_FIX_UPDATE_VERSION    01


// {81129EF8-391D-4f63-AE99-58517EC077E3}
#define   PLATFORM_ACPI_FIX_TABLE_GUID \
{ 0x81129ef8, 0x391d, 0x4f63, { 0xae, 0x99, 0x58, 0x51, 0x7e, 0xc0, 0x77, 0xe3 } }

typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  VOID                    *TablePtr;

} ACPI_FIX_UPDATE_TABLE;

EFI_STATUS
PlatformGetAcpiFixTableDataPointer (
  IN  VOID                          **TablePtr
);

STATIC  EFI_GUID gPlatformAcpiFixTableGuid = PLATFORM_ACPI_FIX_TABLE_GUID;

#endif //_PLATFORM_ACPI_UPDATE_LIB_H
