/** @file

  Copyright (c) 2017 - 2021, Arm Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - Cm or CM   - Configuration Manager
    - Obj or OBJ - Object
**/

#ifndef CONFIGURATION_MANAGER_H__
#define CONFIGURATION_MANAGER_H__

/** C array containing the compiled AML template.
    These symbols are defined in the auto generated C file
    containing the AML bytecode array.
*/
extern CHAR8  dsdt_aml_code[];
extern CHAR8  ssdtjunousb_aml_code[];
extern CHAR8  ssdtpci_aml_code[];

/** The configuration manager version
*/
#define CONFIGURATION_MANAGER_REVISION CREATE_REVISION (1, 0)

/** The OEM ID
*/
#define CFG_MGR_OEM_ID    { 'A', 'R', 'M', 'L', 'T', 'D' }

/** A helper macro for mapping a reference token
*/
#define REFERENCE_TOKEN(Field)                                           \
  (CM_OBJECT_TOKEN)((UINT8*)&ArmJunoPlatformRepositoryInfo +             \
    OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, Field))

/** A helper macro for populating the GIC CPU information
*/
#define GICC_ENTRY(                                                      \
          CPUInterfaceNumber,                                            \
          Mpidr,                                                         \
          PmuIrq,                                                        \
          VGicIrq,                                                       \
          EnergyEfficiency                                               \
          ) {                                                            \
    CPUInterfaceNumber,       /* UINT32  CPUInterfaceNumber           */ \
    CPUInterfaceNumber,       /* UINT32  AcpiProcessorUid             */ \
    EFI_ACPI_6_2_GIC_ENABLED, /* UINT32  Flags                        */ \
    0,                        /* UINT32  ParkingProtocolVersion       */ \
    PmuIrq,                   /* UINT32  PerformanceInterruptGsiv     */ \
    0,                        /* UINT64  ParkedAddress                */ \
    FixedPcdGet64 (                                                      \
      PcdGicInterruptInterfaceBase                                       \
      ),                      /* UINT64  PhysicalBaseAddress          */ \
    0x2C06F000,               /* UINT64  GICV                         */ \
    0x2C04F000,               /* UINT64  GICH                         */ \
    VGicIrq,                  /* UINT32  VGICMaintenanceInterrupt     */ \
    0,                        /* UINT64  GICRBaseAddress              */ \
    Mpidr,                    /* UINT64  MPIDR                        */ \
    EnergyEfficiency          /* UINT8   ProcessorPowerEfficiencyClass*/ \
    }

/** A helper macro for populating the Processor Hierarchy Node flags
*/
#define PROC_NODE_FLAGS(                                                \
          PhysicalPackage,                                              \
          AcpiProcessorIdValid,                                         \
          ProcessorIsThread,                                            \
          NodeIsLeaf,                                                   \
          IdenticalImplementation                                       \
          )                                                             \
  (                                                                     \
    PhysicalPackage |                                                   \
    (AcpiProcessorIdValid << 1) |                                       \
    (ProcessorIsThread << 2) |                                          \
    (NodeIsLeaf << 3) |                                                 \
    (IdenticalImplementation << 4)                                      \
  )

/** A helper macro for populating the Cache Type Structure's attributes
*/
#define CACHE_ATTRIBUTES(                                               \
          AllocationType,                                               \
          CacheType,                                                    \
          WritePolicy                                                   \
          )                                                             \
  (                                                                     \
    AllocationType |                                                    \
    (CacheType << 2) |                                                  \
    (WritePolicy << 4)                                                  \
  )

/** A function that prepares Configuration Manager Objects for returning.

  @param [in]  This        Pointer to the Configuration Manager Protocol.
  @param [in]  CmObjectId  The Configuration Manager Object ID.
  @param [in]  Token       A token for identifying the object.
  @param [out] CmObject    Pointer to the Configuration Manager Object
                           descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
typedef EFI_STATUS (*CM_OBJECT_HANDLER_PROC) (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token,
  IN  OUT   CM_OBJ_DESCRIPTOR                     * CONST CmObject
  );

/** The number of CPUs
*/
#define PLAT_CPU_COUNT          6

/** The number of ACPI tables to install
*/
#define PLAT_ACPI_TABLE_COUNT   10

/** The number of platform generic timer blocks
*/
#define PLAT_GTBLOCK_COUNT          1

/** The number of timer frames per generic timer block
*/
#define PLAT_GTFRAME_COUNT          2

/** For all Juno revisions, the cache and processor topology is identical.

  Terms 'big' and 'LITTLE' are used in favour of A72, A57 or A53 to take into
  account that Juno r2 uses Cortex-A72 as 'big' cores, whereas Juno r0 and r1
  have Cortex-A57 as 'big' cores.
*/

/** The number of Processor Hierarchy Nodes
    - one package node
    - two cluster nodes ('big' + 'LITTLE')
    - two 'big' cores
    - four 'LITTLE' cores
*/
#define PLAT_PROC_HIERARCHY_NODE_COUNT  9

/** The number of unique cache structures:
    - 'big' core L1 instruction cache
    - 'big' core L1 data cache
    - 'big' core L2 cache
    - 'LITTLE' core L1 instruction cache
    - 'LITTLE' core L1 data cache
    - 'LITTLE' core L2 cache
*/
#define PLAT_CACHE_COUNT                6

/** The number of resources private to the entire 'big' cluster
    - L2 cache
*/
#define BIG_CLUSTER_RESOURCE_COUNT  1

/** The number of resources private to 'big' core instance
    - L1 data cache
    - L1 instruction cache
*/
#define BIG_CORE_RESOURCE_COUNT  2

/** The number of resources private to the entire 'LITTLE' cluster
    - L2 cache
*/
#define LITTLE_CLUSTER_RESOURCE_COUNT  1

/** The number of resources private to each 'LITTLE' core instance
    - L1 data cache
    - L1 instruction cache
*/
#define LITTLE_CORE_RESOURCE_COUNT  2

/** A structure describing the platform configuration
    manager repository information
*/
typedef struct PlatformRepositoryInfo {
  /// Configuration Manager Information
  CM_STD_OBJ_CONFIGURATION_MANAGER_INFO CmInfo;

  /// List of ACPI tables
  CM_STD_OBJ_ACPI_TABLE_INFO            CmAcpiTableList[PLAT_ACPI_TABLE_COUNT];

  /// Boot architecture information
  CM_ARM_BOOT_ARCH_INFO                 BootArchInfo;

  /// Power management profile information
  CM_ARM_POWER_MANAGEMENT_PROFILE_INFO  PmProfileInfo;

  /// GIC CPU interface information
  CM_ARM_GICC_INFO                      GicCInfo[PLAT_CPU_COUNT];

  /// GIC distributor information
  CM_ARM_GICD_INFO                      GicDInfo;

  /// Generic timer information
  CM_ARM_GENERIC_TIMER_INFO             GenericTimerInfo;

  /// Generic timer block information
  CM_ARM_GTBLOCK_INFO                   GTBlockInfo[PLAT_GTBLOCK_COUNT];

  /// Generic timer frame information
  CM_ARM_GTBLOCK_TIMER_FRAME_INFO       GTBlock0TimerInfo[PLAT_GTFRAME_COUNT];

  /// Watchdog information
  CM_ARM_GENERIC_WATCHDOG_INFO          Watchdog;

  /** Serial port information for the
      serial port console redirection port
  */
  CM_ARM_SERIAL_PORT_INFO               SpcrSerialPort;

  /// Serial port information for the DBG2 UART port
  CM_ARM_SERIAL_PORT_INFO               DbgSerialPort;

  /// PCI configuration space information
  CM_ARM_PCI_CONFIG_SPACE_INFO          PciConfigInfo;

  /// GIC MSI Frame information
  CM_ARM_GIC_MSI_FRAME_INFO             GicMsiFrameInfo;

  // Processor topology information
  CM_ARM_PROC_HIERARCHY_INFO            ProcHierarchyInfo[PLAT_PROC_HIERARCHY_NODE_COUNT];

  // Cache information
  CM_ARM_CACHE_INFO                     CacheInfo[PLAT_CACHE_COUNT];

  // 'big' cluster private resources
  CM_ARM_OBJ_REF                        BigClusterResources[BIG_CLUSTER_RESOURCE_COUNT];

  // 'big' core private resources
  CM_ARM_OBJ_REF                        BigCoreResources[BIG_CORE_RESOURCE_COUNT];

  // 'LITTLE' cluster private resources
  CM_ARM_OBJ_REF                        LittleClusterResources[LITTLE_CLUSTER_RESOURCE_COUNT];

  // 'LITTLE' core private resources
  CM_ARM_OBJ_REF                        LittleCoreResources[LITTLE_CORE_RESOURCE_COUNT];

  /// Juno Board Revision
  UINT32                                JunoRevision;
} EDKII_PLATFORM_REPOSITORY_INFO;

#endif // CONFIGURATION_MANAGER_H__
