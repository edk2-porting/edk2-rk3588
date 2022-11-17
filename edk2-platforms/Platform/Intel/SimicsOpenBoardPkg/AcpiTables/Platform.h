/** @file
  Platform specific defines for constructing ACPI tables

  Copyright (c) 2013 - 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ACPI_PLATFORM_H_
#define _ACPI_PLATFORM_H_

#include <PiDxe.h>
#include <IndustryStandard/Acpi.h>

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_OEM_ID           'S','I','M','I','C','S'   // OEMID 6 bytes long
#define EFI_ACPI_OEM_TABLE_ID     SIGNATURE_64('S','I','M','I','C','S','T','B') // OEM table id 8 bytes long
#define EFI_ACPI_OEM_REVISION     0x02000820
#define EFI_ACPI_CREATOR_ID       SIGNATURE_32('Q','S','P',' ')
#define EFI_ACPI_CREATOR_REVISION 0x00000097

#define INT_MODEL       0x01
#define SCI_INT_VECTOR  0x0009
#define SMI_CMD_IO_PORT 0xB2
#define ACPI_ENABLE     0x0E1
#define ACPI_DISABLE    0x01E
#define S4BIOS_REQ      0x00
#define PM1a_EVT_BLK    0x00000400
#define PM1b_EVT_BLK    0x00000000
#define PM1a_CNT_BLK    0x00000404
#define PM1b_CNT_BLK    0x00000000
#define PM2_CNT_BLK     0x00000450
#define PM_TMR_BLK      0x00000408
#define GPE0_BLK        0x00000420
#define GPE1_BLK        0x00000000
#define PM1_EVT_LEN     0x04
#define PM1_CNT_LEN     0x04
#define PM2_CNT_LEN     0x01
#define PM_TM_LEN       0x04
#define GPE0_BLK_LEN    0x10
#define GPE1_BLK_LEN    0x00
#define GPE1_BASE       0x00
#define RESERVED        0x00
#define P_LVL2_LAT      0x0065
#define P_LVL3_LAT      0x03E9
#define FLUSH_SIZE      0x0400
#define FLUSH_STRIDE    0x0010
#define DUTY_OFFSET     0x00
#define DUTY_WIDTH      0x00
#define DAY_ALRM        0x0D
#define MON_ALRM        0x00
#define CENTURY         0x00
#define FLAG            (EFI_ACPI_2_0_WBINVD | \
                         EFI_ACPI_2_0_PROC_C1 | \
                         EFI_ACPI_2_0_SLP_BUTTON | \
                         EFI_ACPI_2_0_RTC_S4 | \
                         EFI_ACPI_2_0_RESET_REG_SUP)
#define RESET_REG       0xCF9
#define RESET_VALUE     (BIT2 | BIT1) // PIIX3 Reset CPU + System Reset

//
// Byte-aligned IO port register block initializer for
// EFI_ACPI_2_0_GENERIC_ADDRESS_STRUCTURE
//
#define GAS2_IO(Base, Size) {                             \
          EFI_ACPI_2_0_SYSTEM_IO, /* AddressSpaceId    */ \
          (Size) * 8,             /* RegisterBitWidth  */ \
          0,                      /* RegisterBitOffset */ \
          0,                      /* Reserved          */ \
          (Base)                  /* Address           */ \
          }

#endif
