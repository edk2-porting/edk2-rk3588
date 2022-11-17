/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Uefi.h"
#include "Guid/SetupVariable.h"
#include "UncoreCommonIncludes.h"
#include <Library/PcdLib.h>

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

//
// Assigning default ID and base addresses here, these definitions are used by ACPI tables
// 

#define PCH_INTERRUPT_BASE              0

#if MAX_SOCKET > 4
#define PCH_IOAPIC                      0
#else
#define PCH_IOAPIC                      (1 << 0)
#endif

//
// This structure stores the base and size of the ACPI reserved memory used when
// resuming from S3.  This region must be allocated by the platform code.
//
typedef struct {
  UINT32  AcpiReservedMemoryBase;
  UINT32  AcpiReservedMemorySize;
  UINT32  SystemMemoryLength;
} RESERVED_ACPI_S3_RANGE;

#define RESERVED_ACPI_S3_RANGE_OFFSET (EFI_PAGE_SIZE - sizeof (RESERVED_ACPI_S3_RANGE))

//
// SMBUS Data
//
#define PCH_SMBUS_BASE_ADDRESS          0x0780

//
// CMOS usage
//

#define CMOS_WARM_RESET_COUNTER_OFFSET       0xBD    // 1 byte CMOS Space for passing warm reset counter to Dxe
                                                     //   due to reset in MRC Dxe always thinks that warm reset occurs
                                                     //   counter > 1 -> means WarmReset

//
// ACPI and legacy I/O register offsets from PMBASE
//
#define R_ACPI_PM1_STS                      0x00
#define R_ACPI_PM1_EN                       0x02
#define R_ACPI_PM1_CNT                      0x04
#define R_ACPI_PM1_TMR                      0x08
#define R_ACPI_PROC_CNT                     0x10
#define R_ACPI_PM2_CNT                      0x50
#define R_ACPI_GPE0_STS                     0x20
#define R_ACPI_GPE0_EN                      0x28
#define R_ACPI_SMI_EN                       0x30
#define R_ACPI_SMI_STS                      0x34
#define R_ACPI_ALT_GP_SMI_EN                0x38
#define R_ACPI_ALT_GP_SMI_STS               0x3A

#define R_ACPI_LV2                          0x14

#define R_IOPORT_CMOS_STANDARD_INDEX            0x70
#define R_IOPORT_CMOS_STANDARD_DATA             0x71

#define R_IOPORT_CMOS_UPPER_INDEX               0x72
#define R_IOPORT_CMOS_UPPER_DATA                0x73

#define R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS     0x0E

//
// Misc PCI register offsets and sizes
//
#define R_EFI_PCI_SVID                              0x2C
#define   V_EFI_PCI_SVID_SIZE                         0x2
#define R_EFI_PCI_SID                               0x2E
#define   V_EFI_PCI_SID_SIZE                          0x2

//
// Need min. of 24 MB PEI phase
//
#define PEI_MIN_MEMORY_SIZE             (EFI_PHYSICAL_ADDRESS) ((320 * 0x100000))

#endif
