/** @file
*
*  Copyright (c) 2018-2021, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __SGI_ACPI_HEADER__
#define __SGI_ACPI_HEADER__

#include <IndustryStandard/Acpi.h>

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_ARM_OEM_ID           'A','R','M','L','T','D'   // OEMID 6 bytes long
#define EFI_ACPI_ARM_OEM_TABLE_ID     SIGNATURE_64 ('A','R','M','S','G','I',' ',' ') // OEM table id 8 bytes long
#define EFI_ACPI_ARM_OEM_REVISION     0x20140727
#define EFI_ACPI_ARM_CREATOR_ID       SIGNATURE_32('A','R','M',' ')
#define EFI_ACPI_ARM_CREATOR_REVISION 0x00000099

#define CORE_COUNT      FixedPcdGet32 (PcdCoreCount)
#define CLUSTER_COUNT   FixedPcdGet32 (PcdClusterCount)

// ACPI OSC Status bits
#define OSC_STS_BIT0_RES              (1U << 0)
#define OSC_STS_FAILURE               (1U << 1)
#define OSC_STS_UNRECOGNIZED_UUID     (1U << 2)
#define OSC_STS_UNRECOGNIZED_REV      (1U << 3)
#define OSC_STS_CAPABILITY_MASKED     (1U << 4)
#define OSC_STS_MASK                  (OSC_STS_BIT0_RES          | \
                                       OSC_STS_FAILURE           | \
                                       OSC_STS_UNRECOGNIZED_UUID | \
                                       OSC_STS_UNRECOGNIZED_REV  | \
                                       OSC_STS_CAPABILITY_MASKED)

// ACPI OSC for Platform-Wide Capability
#define OSC_CAP_CPPC_SUPPORT          (1U << 5)
#define OSC_CAP_CPPC2_SUPPORT         (1U << 6)
#define OSC_CAP_PLAT_COORDINATED_LPI  (1U << 7)
#define OSC_CAP_OS_INITIATED_LPI      (1U << 8)

#pragma pack(1)
// PPTT processor core structure
typedef struct {
  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR  Core;
  UINT32                                 ResourceOffset[2];
  EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE      DCache;
  EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE      ICache;
  EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE      L2Cache;
} RD_PPTT_CORE;

// PPTT processor cluster structure
typedef struct {
  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR  Cluster;
  UINT32                                 ResourceOffset;
  EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE      L3Cache;
  RD_PPTT_CORE                           Core[CORE_COUNT];
} RD_PPTT_CLUSTER;

// PPTT processor cluster structure without cache
typedef struct {
  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR  Cluster;
  RD_PPTT_CORE                           Core[CORE_COUNT];
} RD_PPTT_MINIMAL_CLUSTER;

// PPTT processor package structure
typedef struct {
  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR  Package;
  UINT32                                 ResourceOffset;
  EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE      Slc;
  RD_PPTT_MINIMAL_CLUSTER                Cluster[CLUSTER_COUNT];
} RD_PPTT_SLC_PACKAGE;
#pragma pack ()

//
// PPTT processor structure flags for different SoC components as defined in
// ACPI 6.3 specification
//

// Processor structure flags for SoC package
#define PPTT_PROCESSOR_PACKAGE_FLAGS                                           \
  {                                                                            \
    EFI_ACPI_6_3_PPTT_PACKAGE_PHYSICAL,                                        \
    EFI_ACPI_6_3_PPTT_PROCESSOR_ID_INVALID,                                    \
    EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD,                                 \
    EFI_ACPI_6_3_PPTT_NODE_IS_NOT_LEAF,                                        \
    EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL                                 \
  }

// Processor structure flags for cluster
#define PPTT_PROCESSOR_CLUSTER_FLAGS                                           \
  {                                                                            \
    EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL,                                    \
    EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID,                                      \
    EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD,                                 \
    EFI_ACPI_6_3_PPTT_NODE_IS_NOT_LEAF,                                        \
    EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL                                 \
  }

// Processor structure flags for cluster with multi-thread core
#define PPTT_PROCESSOR_CLUSTER_THREADED_FLAGS                                  \
  {                                                                            \
    EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL,                                    \
    EFI_ACPI_6_3_PPTT_PROCESSOR_ID_INVALID,                                    \
    EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD,                                 \
    EFI_ACPI_6_3_PPTT_NODE_IS_NOT_LEAF,                                        \
    EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL                                 \
  }

// Processor structure flags for single-thread core
#define PPTT_PROCESSOR_CORE_FLAGS                                              \
  {                                                                            \
    EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL,                                    \
    EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID,                                      \
    EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD,                                 \
    EFI_ACPI_6_3_PPTT_NODE_IS_LEAF                                             \
  }

// Processor structure flags for multi-thread core
#define PPTT_PROCESSOR_CORE_THREADED_FLAGS                                     \
  {                                                                            \
    EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL,                                    \
    EFI_ACPI_6_3_PPTT_PROCESSOR_ID_INVALID,                                    \
    EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD,                                 \
    EFI_ACPI_6_3_PPTT_NODE_IS_NOT_LEAF,                                        \
    EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL                                 \
  }

// Processor structure flags for CPU thread
#define PPTT_PROCESSOR_THREAD_FLAGS                                            \
  {                                                                            \
    EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL,                                    \
    EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID,                                      \
    EFI_ACPI_6_3_PPTT_PROCESSOR_IS_THREAD,                                     \
    EFI_ACPI_6_3_PPTT_NODE_IS_LEAF                                             \
  }

// PPTT cache structure flags as defined in ACPI 6.3 Specification
#define PPTT_CACHE_STRUCTURE_FLAGS                                             \
  {                                                                            \
    EFI_ACPI_6_3_PPTT_CACHE_SIZE_VALID,                                        \
    EFI_ACPI_6_3_PPTT_NUMBER_OF_SETS_VALID,                                    \
    EFI_ACPI_6_3_PPTT_ASSOCIATIVITY_VALID,                                     \
    EFI_ACPI_6_3_PPTT_ALLOCATION_TYPE_VALID,                                   \
    EFI_ACPI_6_3_PPTT_CACHE_TYPE_VALID,                                        \
    EFI_ACPI_6_3_PPTT_WRITE_POLICY_VALID,                                      \
    EFI_ACPI_6_3_PPTT_LINE_SIZE_VALID                                          \
  }

// PPTT cache attributes for data cache
#define PPTT_DATA_CACHE_ATTR                                                   \
  {                                                                            \
    EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE,                       \
    EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_DATA,                             \
    EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK                      \
  }

// PPTT cache attributes for instruction cache
#define PPTT_INST_CACHE_ATTR                                                   \
  {                                                                            \
    EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ,                             \
    EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_INSTRUCTION,                      \
    EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK                      \
  }

// PPTT cache attributes for unified cache
#define PPTT_UNIFIED_CACHE_ATTR                                                \
  {                                                                            \
    EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE,                       \
    EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_UNIFIED,                          \
    EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK                      \
  }

// A macro to initialise the common header part of EFI ACPI tables as defined by
// EFI_ACPI_DESCRIPTION_HEADER structure.
#define ARM_ACPI_HEADER(Signature, Type, Revision) {             \
    Signature,                      /* UINT32  Signature */       \
    sizeof (Type),                  /* UINT32  Length */          \
    Revision,                       /* UINT8   Revision */        \
    0,                              /* UINT8   Checksum */        \
    { EFI_ACPI_ARM_OEM_ID },        /* UINT8   OemId[6] */        \
    EFI_ACPI_ARM_OEM_TABLE_ID,      /* UINT64  OemTableId */      \
    EFI_ACPI_ARM_OEM_REVISION,      /* UINT32  OemRevision */     \
    EFI_ACPI_ARM_CREATOR_ID,        /* UINT32  CreatorId */       \
    EFI_ACPI_ARM_CREATOR_REVISION   /* UINT32  CreatorRevision */ \
  }

// EFI_ACPI_6_2_GIC_STRUCTURE
#define EFI_ACPI_6_2_GICC_STRUCTURE_INIT(GicId, AcpiCpuUid, Mpidr, Flags,      \
  PmuIrq, GicBase, GicVBase, GicHBase, GsivId, GicRBase, Efficiency)           \
  {                                                                            \
    EFI_ACPI_6_2_GIC,                     /* Type */                           \
    sizeof (EFI_ACPI_6_2_GIC_STRUCTURE),  /* Length */                         \
    EFI_ACPI_RESERVED_WORD,               /* Reserved */                       \
    GicId,                                /* CPUInterfaceNumber */             \
    AcpiCpuUid,                           /* AcpiProcessorUid */               \
    Flags,                                /* Flags */                          \
    0,                                    /* ParkingProtocolVersion */         \
    PmuIrq,                               /* PerformanceInterruptGsiv */       \
    0,                                    /* ParkedAddress */                  \
    GicBase,                              /* PhysicalBaseAddress */            \
    GicVBase,                             /* GICV */                           \
    GicHBase,                             /* GICH */                           \
    GsivId,                               /* VGICMaintenanceInterrupt */       \
    GicRBase,                             /* GICRBaseAddress */                \
    Mpidr,                                /* MPIDR */                          \
    Efficiency,                           /* ProcessorPowerEfficiencyClass */  \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,             /* Reserved2[0] */                   \
      EFI_ACPI_RESERVED_BYTE,             /* Reserved2[1] */                   \
      EFI_ACPI_RESERVED_BYTE              /* Reserved2[2] */                   \
    }                                                                          \
  }

// EFI_ACPI_6_2_GIC_DISTRIBUTOR_STRUCTURE
#define EFI_ACPI_6_2_GIC_DISTRIBUTOR_INIT(GicDistHwId, GicDistBase,            \
  GicDistVector, GicVersion)                                                   \
  {                                                                            \
    EFI_ACPI_6_2_GICD,                    /* Type */                           \
    sizeof (EFI_ACPI_6_2_GIC_DISTRIBUTOR_STRUCTURE),                           \
    EFI_ACPI_RESERVED_WORD,               /* Reserved1 */                      \
    GicDistHwId,                          /* GicId */                          \
    GicDistBase,                          /* PhysicalBaseAddress */            \
    GicDistVector,                        /* SystemVectorBase */               \
    GicVersion,                           /* GicVersion */                     \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,             /* Reserved2[0] */                   \
      EFI_ACPI_RESERVED_BYTE,             /* Reserved2[1] */                   \
      EFI_ACPI_RESERVED_BYTE              /* Reserved2[2] */                   \
    }                                                                          \
  }

// EFI_ACPI_6_2_GICR_STRUCTURE
#define EFI_ACPI_6_2_GIC_REDISTRIBUTOR_INIT(RedisRegionAddr, RedisDiscLength)  \
  {                                                                            \
    EFI_ACPI_6_2_GICR,                    /* Type */                           \
    sizeof (EFI_ACPI_6_2_GICR_STRUCTURE), /* Length */                         \
    EFI_ACPI_RESERVED_WORD,               /* Reserved */                       \
    RedisRegionAddr,                      /* DiscoveryRangeBaseAddress */      \
    RedisDiscLength                       /* DiscoveryRangeLength */           \
  }

// EFI_ACPI_6_2_GIC_ITS_STRUCTURE
#define EFI_ACPI_6_2_GIC_ITS_INIT(GicItsId, GicItsBase)                        \
  {                                                                            \
    EFI_ACPI_6_2_GIC_ITS,                 /* Type */                           \
    sizeof (EFI_ACPI_6_2_GIC_ITS_STRUCTURE),                                   \
    EFI_ACPI_RESERVED_WORD,               /* Reserved */                       \
    GicItsId,                             /* GicItsId */                       \
    GicItsBase,                           /* PhysicalBaseAddress */            \
    EFI_ACPI_RESERVED_DWORD               /* DiscoveryRangeLength */           \
  }

// EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE
#define EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE_INIT(                           \
          ProximityDomain, Base, Length, Flags)                                \
  {                                                                            \
    1, sizeof (EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE), ProximityDomain,       \
    EFI_ACPI_RESERVED_WORD, (Base) & 0xffffffff,                               \
    (Base) >> 32, (Length) & 0xffffffff,                                       \
    (Length) >> 32, EFI_ACPI_RESERVED_DWORD, Flags,                            \
    EFI_ACPI_RESERVED_QWORD                                                    \
  }

// EFI_ACPI_6_3_GICC_AFFINITY_STRUCTURE
#define EFI_ACPI_6_3_GICC_AFFINITY_STRUCTURE_INIT(                             \
          ProximityDomain, ACPIProcessorUID, Flags, ClockDomain)               \
  {                                                                            \
    3, sizeof (EFI_ACPI_6_3_GICC_AFFINITY_STRUCTURE), ProximityDomain,         \
    ACPIProcessorUID,  Flags,  ClockDomain                                     \
  }

//
// HMAT related structures
//
// Memory Proximity Domain Attributes Structure
// Refer Section 5.2.27.3 in ACPI Specification, Version 6.3
#define EFI_ACPI_6_3_HMAT_STRUCTURE_MEMORY_PROXIMITY_DOMAIN_ATTRIBUTES_INIT(   \
    Flags, ProximityDomainForAttachedIntiator, ProximityDomainForMemory)       \
  {                                                                            \
    EFI_ACPI_6_3_HMAT_TYPE_MEMORY_PROXIMITY_DOMAIN_ATTRIBUTES,                 \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE                                                   \
    },                                                                         \
    sizeof (EFI_ACPI_6_3_HMAT_STRUCTURE_MEMORY_PROXIMITY_DOMAIN_ATTRIBUTES),   \
    {                                                                          \
      Flags,                                                                   \
      0                                                                        \
    },                                                                         \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE                                                   \
    },                                                                         \
    ProximityDomainForAttachedIntiator,                                        \
    ProximityDomainForMemory,                                                  \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE                                                   \
    }                                                                          \
  }

// System Locality Latency and Bandwidth Information Structure
// Refer Section 5.2.27.4 in ACPI Specification, Version 6.3
#define EFI_ACPI_6_3_HMAT_STRUCTURE_SYSTEM_LOCALITY_LATENCY_AND_BANDWIDTH_INFO_INIT(  \
    Flags, DataType, NumInitiatorProximityDomains,                                    \
    NumTargetProximityDomains, EntryBaseUnit)                                         \
  {                                                                                   \
    EFI_ACPI_6_3_HMAT_TYPE_SYSTEM_LOCALITY_LATENCY_AND_BANDWIDTH_INFO,                \
    {                                                                                 \
      EFI_ACPI_RESERVED_BYTE,                                                         \
      EFI_ACPI_RESERVED_BYTE                                                          \
    },                                                                                \
    sizeof (EFI_ACPI_6_3_HMAT_STRUCTURE_SYSTEM_LOCALITY_LATENCY_AND_BANDWIDTH_INFO) + \
      (4 * NumInitiatorProximityDomains) + (4 * NumTargetProximityDomains) +          \
      (2 * NumInitiatorProximityDomains * NumTargetProximityDomains),                 \
    {                                                                                 \
      Flags,                                                                          \
      0                                                                               \
    },                                                                                \
    DataType,                                                                         \
    {                                                                                 \
      EFI_ACPI_RESERVED_BYTE,                                                         \
      EFI_ACPI_RESERVED_BYTE                                                          \
    },                                                                                \
    NumInitiatorProximityDomains,                                                     \
    NumTargetProximityDomains,                                                        \
    {                                                                                 \
      EFI_ACPI_RESERVED_BYTE,                                                         \
      EFI_ACPI_RESERVED_BYTE,                                                         \
      EFI_ACPI_RESERVED_BYTE,                                                         \
      EFI_ACPI_RESERVED_BYTE                                                          \
    },                                                                                \
    EntryBaseUnit                                                                     \
  }

// Memory Side Cache Information Structure
// Refer Section 5.2.27.5 in ACPI Specification, Version 6.3
#define EFI_ACPI_6_3_HMAT_STRUCTURE_MEMORY_SIDE_CACHE_INFO_INIT(               \
    MemoryProximityDomain, MemorySideCacheSize, CacheAttributes,               \
    NumberOfSmbiosHandles)                                                     \
  {                                                                            \
    EFI_ACPI_6_3_HMAT_TYPE_MEMORY_SIDE_CACHE_INFO,                             \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE                                                   \
    },                                                                         \
    sizeof (EFI_ACPI_6_3_HMAT_STRUCTURE_MEMORY_SIDE_CACHE_INFO) +              \
      (NumberOfSmbiosHandles * 2),                                             \
    MemoryProximityDomain,                                                     \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE                                                   \
    },                                                                         \
    MemorySideCacheSize,                                                       \
    CacheAttributes,                                                           \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE                                                   \
    },                                                                         \
    NumberOfSmbiosHandles                                                      \
  }

/** A macro to initialise the Memory Side Cache Information Attributes.
    See Table 5.124 in ACPI Specification, Version 6.3

  @param [in] TotalCacheLevels    Total Cache Levels for this Memory Proximity.
  @param [in] CacheLevel          Cache Level described in this structure.
  @param [in] CacheAssociativity  Cache Associativity.
  @param [in] WritePolicy         Write Policy.
  @param [in] CacheLineSize       Cache Line size in bytes.
**/
#define HMAT_STRUCTURE_MEMORY_SIDE_CACHE_INFO_CACHE_ATTRIBUTES_INIT(           \
  TotalCacheLevels, CacheLevel, CacheAssociativity, WritePolicy, CacheLineSize \
  )                                                                            \
{                                                                              \
  TotalCacheLevels, CacheLevel, CacheAssociativity, WritePolicy, CacheLineSize \
}

// EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR
#define EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR_INIT(Length, Flag, Parent,       \
  ACPIProcessorID, NumberOfPrivateResource)                                    \
  {                                                                            \
    EFI_ACPI_6_3_PPTT_TYPE_PROCESSOR,                 /* Type 0 */             \
    Length,                                           /* Length */             \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
    },                                                                         \
    Flag,                                             /* Processor flags */    \
    Parent,                                           /* Ref to parent node */ \
    ACPIProcessorID,                                  /* UID, as per MADT */   \
    NumberOfPrivateResource                           /* Resource count */     \
  }

// EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE
#define EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE_INIT(Flag, NextLevelCache, Size,     \
  NoOfSets, Associativity, Attributes, LineSize)                               \
  {                                                                            \
    EFI_ACPI_6_3_PPTT_TYPE_CACHE,                     /* Type 1 */             \
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE),       /* Length */             \
    {                                                                          \
      EFI_ACPI_RESERVED_BYTE,                                                  \
      EFI_ACPI_RESERVED_BYTE,                                                  \
    },                                                                         \
    Flag,                                             /* Cache flags */        \
    NextLevelCache,                                   /* Ref to next level */  \
    Size,                                             /* Size in bytes */      \
    NoOfSets,                                         /* Num of sets */        \
    Associativity,                                    /* Num of ways */        \
    Attributes,                                       /* Cache attributes */   \
    LineSize                                          /* Line size in bytes */ \
  }

/** Helper macro for CPPC _CPC object initialization. Use of this macro is
    restricted to ASL file and not to TDL file.

    @param [in] DesiredPerfReg      Fastchannel address for desired performance
                                    register.
    @param [in] PerfLimitedReg      Fastchannel address for performance limited
                                    register.
    @param [in] GranularityMHz      Granularity of the performance scale.
    @param [in] HighestPerf         Highest performance in linear scale.
    @param [in] NominalPerf         Nominal performance in linear scale.
    @param [in] LowestNonlinearPerf Lowest non-linear performnce in linear
                                    scale.
    @param [in] LowestPerf          Lowest performance in linear scale.
    @param [in] RefPerf             Reference performance in linear scale.
**/
#define CPPC_PACKAGE_INIT(DesiredPerfReg, PerfLimitedReg, GranularityMHz,      \
  HighestPerf, NominalPerf, LowestNonlinearPerf, LowestPerf, RefPerf)          \
  {                                                                            \
    23,                                 /* NumEntries */                       \
    3,                                  /* Revision */                         \
    HighestPerf,                        /* Highest Performance */              \
    NominalPerf,                        /* Nominal Performance */              \
    LowestNonlinearPerf,                /* Lowest Nonlinear Performance */     \
    LowestPerf,                         /* Lowest Performance */               \
    /* Guaranteed Performance Register */                                      \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    /* Desired Performance Register */                                         \
    ResourceTemplate () { Register (SystemMemory, 32, 0, DesiredPerfReg, 3) }, \
    /* Minimum Performance Register */                                         \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    /* Maximum Performance Register */                                         \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    /* Performance Reduction Tolerance Register */                             \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    /* Time Window Register */                                                 \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    /* Counter Wraparound Time */                                              \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    /* Reference Performance Counter Register */                               \
    ResourceTemplate () { Register (FFixedHW, 64, 0, 1, 4) },                  \
    /* Delivered Performance Counter Register */                               \
    ResourceTemplate () { Register (FFixedHW, 64, 0, 0, 4) },                  \
    /* Performance Limited Register */                                         \
    ResourceTemplate () { Register (SystemMemory, 32, 0, PerfLimitedReg, 3) }, \
    /* CPPC Enable Register */                                                 \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    /* Autonomous Selection Enable Register */                                 \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    /* Autonomous Activity Window Register */                                  \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    /* Energy Performance Preference Register */                               \
    ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },               \
    RefPerf,                            /* Reference Performance */            \
    (LowestPerf * GranularityMHz),      /* Lowest Frequency */                 \
    (NominalPerf * GranularityMHz),     /* Nominal Frequency */                \
  }

// Power state dependancy (_PSD) for CPPC

/** Helper macro to initialize Power state dependancy (_PSD) object required
    for CPPC. Use of this macro is restricted to ASL file and not to TDL file.

    @param [in] Domain              The dependency domain number to which this
                                    P-state entry belongs.
**/
#define PSD_INIT(Domain)                                                       \
  {                                                                            \
    5,              /* Entries */                                              \
    0,              /* Revision */                                             \
    Domain,         /* Domain */                                               \
    0xFD,           /* Coord Type- SW_ANY */                                   \
    1               /* Processors */                                           \
  }

#endif /* __SGI_ACPI_HEADER__ */
