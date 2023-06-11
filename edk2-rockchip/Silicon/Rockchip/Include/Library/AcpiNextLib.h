/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/



#ifndef __ACPI_NEXT_LIB_H__
#define __ACPI_NEXT_LIB_H__

#include <PlatformArch.h>

///
/// ITS Affinity Structure Definition
///
#pragma pack(1)
typedef struct {
  UINT8   Type;
  UINT8   Length;
  UINT32  ProximityDomain;
  UINT16  Reserved;
  UINT32  ItsHwId;
} EFI_ACPI_6_2_ITS_AFFINITY_STRUCTURE;
#pragma pack()

#define EFI_ACPI_6_1_GIC_ITS_INIT(GicITSHwId, GicITSBase) \
  { \
    EFI_ACPI_6_1_GIC_ITS, sizeof (EFI_ACPI_6_1_GIC_ITS_STRUCTURE), EFI_ACPI_RESERVED_WORD, \
    GicITSHwId, GicITSBase, EFI_ACPI_RESERVED_DWORD\
  }

#define EFI_ACPI_5_1_GICR_STRUCTURE_INIT(    \
    GicRBase, GicRlength)                                   \
  {                                                                                  \
    EFI_ACPI_5_1_GICR, sizeof (EFI_ACPI_5_1_GICR_STRUCTURE), EFI_ACPI_RESERVED_WORD,   \
     GicRBase, GicRlength \
  }

#define EFI_ACPI_6_1_GICC_AFFINITY_STRUCTURE_INIT(                                              \
    ProximityDomain, ACPIProcessorUID, Flags, ClockDomain)                                      \
  {                                                                                             \
    3, sizeof (EFI_ACPI_6_1_GICC_AFFINITY_STRUCTURE),ProximityDomain ,                          \
     ACPIProcessorUID,  Flags,  ClockDomain                                                     \
  }

#define EFI_ACPI_6_2_ITS_AFFINITY_STRUCTURE_INIT(                                               \
    ProximityDomain, ItsId)                                                                     \
  {                                                                                             \
    4, sizeof (EFI_ACPI_6_2_ITS_AFFINITY_STRUCTURE), ProximityDomain,                           \
    EFI_ACPI_RESERVED_WORD, ItsId                                                               \
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

#define EFI_ACPI_6_1_GIC_DISTRIBUTOR_INIT(GicDistHwId, GicDistBase, GicDistVector, GicVersion) \
  { \
    EFI_ACPI_6_1_GICD, sizeof (EFI_ACPI_6_1_GIC_DISTRIBUTOR_STRUCTURE), EFI_ACPI_RESERVED_WORD, \
    GicDistHwId, GicDistBase, GicDistVector, GicVersion, \
    {EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE} \
  }


#pragma pack(1)
//
// Define the number of each table type.
// This is where the table layout is modified.
//
#define EFI_ACPI_PROCESSOR_LOCAL_GICC_AFFINITY_STRUCTURE_COUNT  (MAX_SOCKET*CORE_NUM_PER_SOCKET)

typedef struct {
  EFI_ACPI_6_2_SYSTEM_RESOURCE_AFFINITY_TABLE_HEADER          Header;
  EFI_ACPI_6_2_MEMORY_AFFINITY_STRUCTURE                      Memory[EFI_ACPI_MEMORY_AFFINITY_STRUCTURE_COUNT];
  EFI_ACPI_6_2_GICC_AFFINITY_STRUCTURE                        Gicc[EFI_ACPI_PROCESSOR_LOCAL_GICC_AFFINITY_STRUCTURE_COUNT];
  EFI_ACPI_6_2_ITS_AFFINITY_STRUCTURE                         Its[EFI_ACPI_6_2_ITS_AFFINITY_STRUCTURE_COUNT];
} EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE;

#pragma pack()
#endif

