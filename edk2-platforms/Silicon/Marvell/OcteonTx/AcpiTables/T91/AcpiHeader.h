/** @file

  Multiple APIC Description Table (MADT)

  Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>
  Copyright (C) 2019, Marvell International Ltd. and its affiliates.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <IndustryStandard/Acpi.h>

#define MV_UART_AS32(Address) { EFI_ACPI_5_0_SYSTEM_MEMORY, 32, 0, EFI_ACPI_5_0_BYTE, Address }

#define ACPI_OEM_ID_ARRAY        {'M','V','E','B','U',' '}
#define ACPI_OEM_REVISION        0
#define ACPI_CREATOR_ID          SIGNATURE_32('L','N','R','O')
#define ACPI_CREATOR_REVISION    0

#if defined(CN9130)
#define ACPI_OEM_TABLE_ID        SIGNATURE_64('C','N','9','1','3','0',' ',' ')
#elif defined (CN9131)
#define ACPI_OEM_TABLE_ID        SIGNATURE_64('C','N','9','1','3','1',' ',' ')
#endif

/**
 * A macro to initialize the common header part of EFI ACPI tables
 * as defined by EFI_ACPI_DESCRIPTION_HEADER structure.
 **/
#define __ACPI_HEADER(sign, type, rev) {                \
  sign,                   /* UINT32  Signature */       \
  sizeof (type),          /* UINT32  Length */          \
  rev,                    /* UINT8   Revision */        \
  0,                      /* UINT8   Checksum */        \
  ACPI_OEM_ID_ARRAY,      /* UINT8   OemId[6] */        \
  ACPI_OEM_TABLE_ID,      /* UINT64  OemTableId */      \
  ACPI_OEM_REVISION,      /* UINT32  OemRevision */     \
  ACPI_CREATOR_ID,        /* UINT32  CreatorId */       \
  ACPI_CREATOR_REVISION   /* UINT32  CreatorRevision */ \
  }
