/** @file
  Efi Server Management Status Code Header File.

  @copyright
  Copyright 2011 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SM_STATUS_CODE_H_
#define _SM_STATUS_CODE_H_

//
// Computing Unit Host Processor Subclass Error Code definitions.
//
#define CU_HP_EC_MISMATCH_STEPPING              (EFI_SUBCLASS_SPECIFIC | 0x0000000E)
#define CU_HP_EC_MISMATCH_FAMILY                (EFI_SUBCLASS_SPECIFIC | 0x0000000F)
#define CU_HP_EC_MISMATCH_MODEL                 (EFI_SUBCLASS_SPECIFIC | 0x00000010)
#define CU_HP_EC_MISMATCH_FSB                   (EFI_SUBCLASS_SPECIFIC | 0x00000011)
#define CU_HP_EC_MISMATCH_SPEED                 (EFI_SUBCLASS_SPECIFIC | 0x00000012)
#define CU_HP_EC_UNSUPPORTED_PROCESSOR_FAMILY   (EFI_SUBCLASS_SPECIFIC | 0x00000013)
#define CU_HP_EC_MISMATCH_CACHE_SIZE            (EFI_SUBCLASS_SPECIFIC | 0x00000014)
#define CU_HP_EC_UNSUPPORTED_CHIPSET            (EFI_SUBCLASS_SPECIFIC | 0x00000015)
#define CU_HP_EC_MISMATCH_VOLTAGE               (EFI_SUBCLASS_SPECIFIC | 0x00000016)

//
// Computing Unit Firmware Processor Subclass Error Code definitions.
//
#define CU_FP_EC_FRB2_WATCHDOG_TIMEOUT          (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define CU_FP_EC_OS_WATCHDOG_TIMEOUT            (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define CU_FP_EC_SDR_EMPTY                      (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define CU_FP_EC_FORCE_UPDATE_MODE              (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define CU_FP_EC_FW_MISMATCH                    (EFI_SUBCLASS_SPECIFIC | 0x00000007)

//
// Computing Unit Memory Subclass Error Code definitions.
//
#define CU_MEMORY_EC_FAILED                     (EFI_SUBCLASS_SPECIFIC | 0x0000000B)
#define CU_MEMORY_EC_UNSUPPORTED                (EFI_SUBCLASS_SPECIFIC | 0x0000000C)
#define CU_MEMORY_EC_CLTT_FAILED                (EFI_SUBCLASS_SPECIFIC | 0x0000000D)
#define CU_MEMORY_EC_DCPMM_INV_POP              (EFI_SUBCLASS_SPECIFIC | 0x0000000E)

//
// Peripheral Class Error Code definitions.
//
#define P_EC_FW_CORRUPTED              0x0000000A
#define P_EC_RESOURCE_CONSUMED_BY_BMC  0x0000000B

//
// IO Bus Subclass definitions.
//
#define IO_BUS_PCI_EXPRESS                      (IO_BUS | 0x000D0000)

//
// IO Bus Class PCI Subclass Error Code definitions.
//
#define IOB_PCI_EC_OUT_OF_RESOURCES             (EFI_SUBCLASS_SPECIFIC | 0x00000002)

//
// IO Bus Class PCI EXPRESS Subclass Error Code definitions.
//
#define IOB_PCI_EXP_EC_PERR                     (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define IOB_PCI_EXP_EC_SERR                     (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define IOB_PCI_EXP_EC_TRAINING_FAILED          (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define IOB_PCI_EXP_EC_DEGRADED_LINK            (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define IOB_PCI_EXP_EC_IBIST_FAILED             (EFI_SUBCLASS_SPECIFIC | 0x00000004)

//
// Software Subclass definitions.
//
#define SOFTWARE_SYSTEM_ERROR                   (EFI_SOFTWARE | 0x00130000)
#define SYSTEM_FRONT_PANEL                      (EFI_SOFTWARE | 0x00140000)
#define SOFTWARE_EFI_BMC                        (EFI_SOFTWARE | 0x00150000)

//
// Software Class DXE BS Driver Subclass Progress Code definitions.
//
#define SW_DXE_BS_END_OF_POST                   (EFI_SUBCLASS_SPECIFIC | 0x00000007)

//
// Software Class Error Code definitions.
//
#define SW_EC_PWD_CLEAR_JMPR_SET       0x00000012

//
// Software Class PEI Module Subclass Error Code definitions.
//
#define SW_PEIM_WATCHDOG_TIMEOUT                (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define SW_PEIM_OS_WATCHDOG_TIMEOUT             (EFI_SUBCLASS_SPECIFIC | 0x00000003)

//
// Software Class System Subclass Error Code definitions.
//
#define SW_EC_CMOS_DATE_TIME_ERROR              (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define SW_EC_BACKUP_BIOS_FLASH_ERROR           (EFI_SUBCLASS_SPECIFIC | 0x00000001)


#endif // _SM_STATUS_CODE_H_

