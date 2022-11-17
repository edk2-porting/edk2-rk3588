/** @file

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <IndustryStandard/Acpi.h>

#define EFI_ACPI_6_1_GIC_ITS_INIT(GicITSHwId, GicITSBase)                                  \
  {                                                                                        \
    EFI_ACPI_6_1_GIC_ITS, sizeof (EFI_ACPI_6_1_GIC_ITS_STRUCTURE), EFI_ACPI_RESERVED_WORD, \
    GicITSHwId, GicITSBase, EFI_ACPI_RESERVED_DWORD                                        \
  }

#define EFI_ACPI_5_1_GICR_STRUCTURE_INIT(                                                  \
    GicRBase, GicRlength)                                                                  \
  {                                                                                        \
    EFI_ACPI_5_1_GICR, sizeof (EFI_ACPI_5_1_GICR_STRUCTURE), EFI_ACPI_RESERVED_WORD,       \
     GicRBase, GicRlength                                                                  \
  }

#define EFI_ACPI_6_1_GICC_AFFINITY_STRUCTURE_INIT(                                              \
    ProximityDomain, ACPIProcessorUID, Flags, ClockDomain)                                      \
  {                                                                                             \
    3, sizeof (EFI_ACPI_6_1_GICC_AFFINITY_STRUCTURE),ProximityDomain ,                          \
     ACPIProcessorUID,  Flags,  ClockDomain                                                     \
  }

#define EFI_ACPI_6_1_MEMORY_AFFINITY_STRUCTURE_INIT(                                              \
    ProximityDomain, AddressBaseLow, AddressBaseHigh, LengthLow, LengthHigh, Flags)               \
  {                                                                                               \
    1, sizeof (EFI_ACPI_6_1_MEMORY_AFFINITY_STRUCTURE),ProximityDomain , EFI_ACPI_RESERVED_WORD,  \
    AddressBaseLow, AddressBaseHigh, LengthLow, LengthHigh, EFI_ACPI_RESERVED_DWORD, Flags,       \
    EFI_ACPI_RESERVED_QWORD                                                                       \
  }

#define EFI_ACPI_6_1_GICC_STRUCTURE_INIT(GicId, AcpiCpuUid, Mpidr, Flags, PmuIrq,    \
    GicBase, GicVBase, GicHBase, GsivId, GicRBase, ProcessorPowerEfficiencyClass)    \
  {                                                                                  \
    EFI_ACPI_6_1_GIC, sizeof (EFI_ACPI_6_1_GIC_STRUCTURE), EFI_ACPI_RESERVED_WORD,   \
    GicId, AcpiCpuUid, Flags, 0, PmuIrq, 0, GicBase, GicVBase, GicHBase,             \
    GsivId, GicRBase, Mpidr, ProcessorPowerEfficiencyClass, {0, 0, 0}                \
  }

#define EFI_ACPI_6_1_GIC_DISTRIBUTOR_INIT(GicDistHwId, GicDistBase, GicDistVector, GicVersion)   \
  {                                                                                              \
    EFI_ACPI_6_1_GICD, sizeof (EFI_ACPI_6_1_GIC_DISTRIBUTOR_STRUCTURE), EFI_ACPI_RESERVED_WORD,  \
    GicDistHwId, GicDistBase, GicDistVector, GicVersion,                                         \
    {EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE}                     \
  }

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_PHYTIUM_OEM_ID           'F','T','-','L','T','D'                       // OEMID 6 bytes long
#define EFI_ACPI_PHYTIUM_OEM_TABLE_ID     SIGNATURE_64('P','H','Y','T','I','U','M',' ') // OEM table id 8 bytes long
#define EFI_ACPI_PHYTIUM_OEM_REVISION     0x20201111
#define EFI_ACPI_PHYTIUM_CREATOR_ID       SIGNATURE_32('P','H','Y','T')
#define EFI_ACPI_PHYTIUM_CREATOR_REVISION 0x20201111

// A macro to initialise the common header part of EFI ACPI tables as defined by
// EFI_ACPI_DESCRIPTION_HEADER structure.
#define PHYTIUM_ACPI_HEADER(Signature, Type, Revision) {              \
    Signature,                          /* UINT32  Signature */       \
    sizeof (Type),                      /* UINT32  Length */          \
    Revision,                           /* UINT8   Revision */        \
    0,                                  /* UINT8   Checksum */        \
    { EFI_ACPI_PHYTIUM_OEM_ID },            /* UINT8   OemId[6] */        \
    EFI_ACPI_PHYTIUM_OEM_TABLE_ID,      /* UINT64  OemTableId */      \
    EFI_ACPI_PHYTIUM_OEM_REVISION,      /* UINT32  OemRevision */     \
    EFI_ACPI_PHYTIUM_CREATOR_ID,        /* UINT32  CreatorId */       \
    EFI_ACPI_PHYTIUM_CREATOR_REVISION   /* UINT32  CreatorRevision */ \
  }

#endif // PLATFORM_H_
