/** @file
*
*  Copyright (c) 2013-2014, ARM Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __ACPITABLES_H__
#define __ACPITABLES_H__

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_OEM_ID           'S','N','I',' ',' ',' '
#define EFI_ACPI_OEM_TABLE_ID     SIGNATURE_64('S','Y','N','Q','U','A','C','R')
#define EFI_ACPI_OEM_REVISION     FixedPcdGet32 (PcdAcpiDefaultOemRevision)
#define EFI_ACPI_CREATOR_ID       SIGNATURE_32('L','N','R','O')
#define EFI_ACPI_CREATOR_REVISION FixedPcdGet32 (PcdAcpiDefaultCreatorRevision)

// A macro to initialise the common header part of EFI ACPI tables as defined by
// EFI_ACPI_DESCRIPTION_HEADER structure.
#define __ACPI_HEADER(Signature, Type, Revision) {                \
    Signature,                      /* UINT32  Signature */       \
    sizeof (Type),                  /* UINT32  Length */          \
    Revision,                       /* UINT8   Revision */        \
    0,                              /* UINT8   Checksum */        \
    { EFI_ACPI_OEM_ID },            /* UINT8   OemId[6] */        \
    EFI_ACPI_OEM_TABLE_ID,          /* UINT64  OemTableId */      \
    EFI_ACPI_OEM_REVISION,          /* UINT32  OemRevision */     \
    EFI_ACPI_CREATOR_ID,            /* UINT32  CreatorId */       \
    EFI_ACPI_CREATOR_REVISION       /* UINT32  CreatorRevision */ \
  }

#define EFI_ACPI_6_0_GIC_REDISTRIBUTOR_INIT(RedisRegionAddr, RedisDiscLength) \
  {                                                                           \
    EFI_ACPI_6_0_GICR,                                                        \
    sizeof (EFI_ACPI_6_0_GICR_STRUCTURE),                                     \
    0,                                                                        \
    RedisRegionAddr,                                                          \
    RedisDiscLength                                                           \
  }

#define EFI_ACPI_6_0_GIC_ITS_FRAME_INIT(Id, PhysAddress)                      \
  {                                                                           \
    EFI_ACPI_6_0_GIC_ITS,                                                     \
    sizeof (EFI_ACPI_6_0_GIC_ITS_STRUCTURE),                                  \
    0,                                                                        \
    Id,                                                                       \
    PhysAddress,                                                              \
    0                                                                         \
  }

#define EFI_ACPI_6_0_SBSA_GENERIC_WATCHDOG_STRUCTURE_INIT(RefreshFramePhys,   \
    ControlFramePhys, WatchdogTimerGSIV, WatchdogTimerFlags)                  \
  {                                                                           \
    EFI_ACPI_6_0_GTDT_SBSA_GENERIC_WATCHDOG,                                  \
    sizeof(EFI_ACPI_6_0_GTDT_SBSA_GENERIC_WATCHDOG_STRUCTURE),                \
    EFI_ACPI_RESERVED_WORD,                                                   \
    RefreshFramePhys,                                                         \
    ControlFramePhys,                                                         \
    WatchdogTimerGSIV,                                                        \
    WatchdogTimerFlags                                                        \
  }

#endif
