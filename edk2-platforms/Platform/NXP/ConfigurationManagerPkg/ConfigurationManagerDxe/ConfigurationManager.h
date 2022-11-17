/** @file
  Configuration Manager headers

  Copyright 2020 NXP
  Copyright 2020 Puresoftware Ltd

  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - Cm or CM   - Configuration Manager
    - Obj or OBJ - Object
**/

#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include <Platform.h>
#include <PlatformAcpiTableGenerator.h>

/** The configuration manager version
*/
#define CONFIGURATION_MANAGER_REVISION CREATE_REVISION (0, 0)

/** The OEM ID
*/
#define CFG_MGR_OEM_ID    { 'N', 'X', 'P', ' ', ' ', ' ' }

/** A helper macro for populating the GIC CPU information
 */
#define GICC_ENTRY(                                                   \
    CPUInterfaceNumber,                                               \
    Mpidr,                                                            \
    PmuIrq,                                                           \
    VGicIrq,                                                          \
    EnergyEfficiency                                                  \
    ) {                                                               \
  CPUInterfaceNumber,       /* UINT32  CPUInterfaceNumber         */  \
  CPUInterfaceNumber,       /* UINT32  AcpiProcessorUid           */  \
  EFI_ACPI_6_2_GIC_ENABLED, /* UINT32  Flags                      */  \
  0,                        /* UINT32  ParkingProtocolVersion     */  \
  PmuIrq,                   /* UINT32  PerformanceInterruptGsiv   */  \
  0,                        /* UINT64  ParkedAddress              */  \
  GICC_BASE,                /* UINT64  PhysicalBaseAddress        */  \
  GICV_BASE,                /* UINT64  GICV                       */  \
  GICH_BASE,                /* UINT64  GICH                       */  \
  VGicIrq,                  /* UINT32  VGICMaintenanceInterrupt   */  \
  0,                        /* UINT64  GICRBaseAddress            */  \
  Mpidr,                    /* UINT64  MPIDR                      */  \
  EnergyEfficiency          /* UINT8   ProcessorPowerEfficiency   */  \
}

/**
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

/** The number of ACPI tables to install
*/
#define CM_MANDATORY_ACPI_TABLES  5
#define PLAT_ACPI_TABLE_COUNT     (CM_MANDATORY_ACPI_TABLES + OEM_ACPI_TABLES)

/** A structure describing the platform configuration
    manager repository information
*/
typedef struct PlatformRepositoryInfo {
  /// Configuration Manager Information
  CM_STD_OBJ_CONFIGURATION_MANAGER_INFO     CmInfo;

  /// List of ACPI tables
  CM_STD_OBJ_ACPI_TABLE_INFO                CmAcpiTableList[PLAT_ACPI_TABLE_COUNT];

  /// Boot architecture information
  CM_ARM_BOOT_ARCH_INFO                     BootArchInfo;

  /// Power management profile information
  CM_ARM_POWER_MANAGEMENT_PROFILE_INFO      PmProfileInfo;

  /// Generic timer information
  CM_ARM_GENERIC_TIMER_INFO                 GenericTimerInfo;

  /// Generic timer block information
  CM_ARM_GTBLOCK_INFO                       GTBlockInfo[PLAT_GTBLOCK_COUNT];

  /// Generic timer frame information
  CM_ARM_GTBLOCK_TIMER_FRAME_INFO           GTBlock0TimerInfo[PLAT_GTFRAME_COUNT];

  /// Watchdog information
  CM_ARM_GENERIC_WATCHDOG_INFO              Watchdog;

  /// GIC CPU interface information
  CM_ARM_GICC_INFO                          GicCInfo[PLAT_CPU_COUNT];

  /// GIC distributor information
  CM_ARM_GICD_INFO                          GicDInfo;

  /// GIC Redistributor information
  CM_ARM_GIC_REDIST_INFO                    GicRedistInfo;

  /// GIC ITS information
  CM_ARM_GIC_ITS_INFO                       GicItsInfo;

  /// PCI configuration space information
  CM_ARM_PCI_CONFIG_SPACE_INFO              PciConfigInfo[PLAT_PCI_CONFG_COUNT];

  /// Serial port information for serial port console redirection port
  CM_ARM_SERIAL_PORT_INFO                   SpcrSerialPort;

  /// Fsl Board Revision
  UINT32                                    FslBoardRevision;
} FSL_PLATFORM_REPOSITORY_INFO;

/*
 * GTDT_GTIMER_FLAGS
 * IT trigger (Level/Edge- Bit 0) and Polarity (Low/High) Bit 1
 * Set bit-0 is 0 (Level trigger), Bit 1 1 (Active low)
 */
#define GTDT_GTIMER_FLAGS           (EFI_ACPI_6_1_GTDT_TIMER_FLAG_TIMER_INTERRUPT_POLARITY)

/*
 * Timer Frame IT High Level triggered
 * IT trigger (Level/Edge- Bit 0) and Polarity (Low/High) Bit 1
 * Set  bit-0 is 0 (Level trigger), Bit 1 0 (Active High)
 */
#define GTDT_FRAME_FLAGS 0

/*
 * Timer frame status
 * Access - Secure or non secure <-- Bit 0
 * State - Save (meaning always on) or Lose Context  <-- Bit 1
 * Set Bit 0 1 as Secure and Bit 1 zero as lose context
*/
#define GTDT_FRAME_COMMON_FLAGS EFI_ACPI_6_1_GTDT_GT_BLOCK_COMMON_FLAG_SECURE_TIMER

/*
 * Watchdog flags
 * IT trigger (Level/Edge- Bit 0), Polarity (Low/High) Bit 1, Secured Bit 2
 * Set Level trigger (Bit 0 as 0)
 * Active High (Bit 1 as 0)
 * Non secure (Bit 2 as 0)
 */
#define SBSA_WATCHDOG_FLAGS 0
#define SBSA_SEC_WATCHDOG_FLAGS EFI_ACPI_6_1_GTDT_GT_BLOCK_COMMON_FLAG_SECURE_TIMER

#define GT_BLOCK_FRAME_RES_BASE  MAX_UINT64

#endif // CONFIGURATION_MANAGER_H
