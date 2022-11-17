/** @file
  This library provides support for various platform-specific DXE drivers.

  Copyright (c) 2014 - 2015, AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _AMDSTYX_ACPI_LIB_H_
#define _AMDSTYX_ACPI_LIB_H_

#include <IndustryStandard/Acpi.h>

#define MAX_CORES                        8

#define EFI_ACPI_AMD_OEM_ID_ARRAY        {'A','M','D','I','N','C'}
#define EFI_ACPI_AMD_OEM_TABLE_ID        SIGNATURE_64('S','E','A','T','T','L','E',' ')
#define EFI_ACPI_AMD_OEM_REVISION        0
#define EFI_ACPI_AMD_CREATOR_ID          SIGNATURE_32('A','M','D',' ')
#define EFI_ACPI_AMD_CREATOR_REVISION    0

/**
 * A macro to initialize the common header part of EFI ACPI tables
 * as defined by EFI_ACPI_DESCRIPTION_HEADER structure.
 **/
#define AMD_ACPI_HEADER(sign, type, rev) {                      \
  sign,                           /* UINT32  Signature */       \
  sizeof (type),                  /* UINT32  Length */          \
  rev,                            /* UINT8   Revision */        \
  0,                              /* UINT8   Checksum */        \
  EFI_ACPI_AMD_OEM_ID_ARRAY,      /* UINT8   OemId[6] */        \
  EFI_ACPI_AMD_OEM_TABLE_ID,      /* UINT64  OemTableId */      \
  EFI_ACPI_AMD_OEM_REVISION,      /* UINT32  OemRevision */     \
  EFI_ACPI_AMD_CREATOR_ID,        /* UINT32  CreatorId */       \
  EFI_ACPI_AMD_CREATOR_REVISION   /* UINT32  CreatorRevision */ \
  }

#define NULL_GAS               {EFI_ACPI_5_1_SYSTEM_MEMORY,  0, 0, EFI_ACPI_5_1_UNDEFINED, 0L}
#define AMD_GAS8(address)      {EFI_ACPI_5_1_SYSTEM_MEMORY,  8, 0, EFI_ACPI_5_1_BYTE,      address}
#define AMD_GAS16(address)     {EFI_ACPI_5_1_SYSTEM_MEMORY, 16, 0, EFI_ACPI_5_1_WORD,      address}
#define AMD_GAS32(address)     {EFI_ACPI_5_1_SYSTEM_MEMORY, 32, 0, EFI_ACPI_5_1_DWORD,     address}
#define AMD_GAS64(address)     {EFI_ACPI_5_1_SYSTEM_MEMORY, 64, 0, EFI_ACPI_5_1_QWORD,     address}
#define AMD_GASN(address)      AMD_GAS32(address)

#endif // _AMDSTYX_ACPI_LIB_H_

