/** @file
  This file contains a structure definition for the ACPI 1.0 High Precision Event Timer
  Description Table (HPET).  The contents of this file should only be modified
  for bug fixes, no porting is required.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//

#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/HighPrecisionEventTimerTable.h>

//
// HPET Definitions
//
#define EFI_ACPI_OEM_HPET_REVISION    0x00000001

#define EFI_ACPI_EVENT_TIMER_BLOCK_ID                   0x0 // To be filled

//
// Event Timer Block Base Address Information
//
#define EFI_ACPI_EVENT_TIMER_BLOCK_ADDRESS_SPACE_ID EFI_ACPI_3_0_SYSTEM_MEMORY
#define EFI_ACPI_EVENT_TIMER_BLOCK_BIT_WIDTH        0x40
#define EFI_ACPI_EVENT_TIMER_BLOCK_BIT_OFFSET       0x00
#define EFI_ACPI_EVENT_TIMER_ACCESS_SIZE            0x00
#define EFI_ACPI_EVENT_TIMER_BLOCK_ADDRESS          0x0 // To be filled

#define EFI_ACPI_HPET_NUMBER                        0x00

#define EFI_ACPI_MIN_CLOCK_TICK                     0x0080

#define EFI_ACPI_HPET_ATTRIBUTES                    0x00

//
// High Precision Event Timer Table
// Please modify all values in Hpet.h only.
//

EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER Hpet = {
  {
    EFI_ACPI_3_0_HIGH_PRECISION_EVENT_TIMER_TABLE_SIGNATURE,
    sizeof (EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER),
    EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_REVISION,

    //
    // Checksum will be updated at runtime
    //
    0x00,

    //
    // It is expected that these values will be updated at runtime
    //
    { ' ', ' ', ' ', ' ', ' ', ' ' },

    0,
    EFI_ACPI_OEM_HPET_REVISION,
    0,
    0
  },

  EFI_ACPI_EVENT_TIMER_BLOCK_ID,
  {
    EFI_ACPI_EVENT_TIMER_BLOCK_ADDRESS_SPACE_ID,
    EFI_ACPI_EVENT_TIMER_BLOCK_BIT_WIDTH,
    EFI_ACPI_EVENT_TIMER_BLOCK_BIT_OFFSET,
    EFI_ACPI_EVENT_TIMER_ACCESS_SIZE,
    EFI_ACPI_EVENT_TIMER_BLOCK_ADDRESS
  },
  EFI_ACPI_HPET_NUMBER,
  EFI_ACPI_MIN_CLOCK_TICK,
  EFI_ACPI_HPET_ATTRIBUTES
};
