/** @file
  This file contains a structure definition for the ACPI 5.0 Fixed ACPI
  Description Table (FADT).  The contents of this file should only be modified
  for bug fixes, no porting is required.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>

//
// FADT Definitions
//
#define EFI_ACPI_OEM_FADT_REVISION    0x00000000

#define EFI_ACPI_PREFERRED_PM_PROFILE 0x00 // To be fixed

#define EFI_ACPI_SCI_INT              0x0009
#define EFI_ACPI_SMI_CMD              0x000000B2

#define EFI_ACPI_ACPI_ENABLE          0 // To be fixed
#define EFI_ACPI_ACPI_DISABLE         0 // To be fixed
#define EFI_ACPI_S4_BIOS_REQ          0x00
#define EFI_ACPI_CST_CNT              0x00

#define EFI_ACPI_PSTATE_CNT           0x00
#define EFI_ACPI_GPE1_BASE            (EFI_ACPI_GPE1_BLK_BIT_WIDTH / 2)
#define EFI_ACPI_P_LVL2_LAT           0x0065 // 101
#define EFI_ACPI_P_LVL3_LAT           0x03E9 // 1001
#define EFI_ACPI_FLUSH_SIZE           0x0000
#define EFI_ACPI_FLUSH_STRIDE         0x0000
#define EFI_ACPI_DUTY_OFFSET          0x01
#define EFI_ACPI_DUTY_WIDTH           0x00

#define EFI_ACPI_DAY_ALRM             0x0D
#define EFI_ACPI_MON_ALRM             0x00
#define EFI_ACPI_CENTURY              0x32

//
// IA-PC Boot Architecture Flags
//

#define EFI_ACPI_IAPC_BOOT_ARCH       0 // To be fixed

//
// Fixed Feature Flags
//
#define EFI_ACPI_FIXED_FEATURE_FLAGS  0 // To be fixed

//
// PM1A Event Register Block Generic Address Information
//
#define EFI_ACPI_PM1A_EVT_BLK_ADDRESS_SPACE_ID  EFI_ACPI_2_0_SYSTEM_IO
#define EFI_ACPI_PM1A_EVT_BLK_BIT_WIDTH         0x20
#define EFI_ACPI_PM1A_EVT_BLK_BIT_OFFSET        0x00
#define EFI_ACPI_PM1A_EVT_BLK_ADDRESS           0 // To be fixed

//
// PM1B Event Register Block Generic Address Information
//
#define EFI_ACPI_PM1B_EVT_BLK_ADDRESS_SPACE_ID  EFI_ACPI_2_0_SYSTEM_IO
#define EFI_ACPI_PM1B_EVT_BLK_BIT_WIDTH         0x00
#define EFI_ACPI_PM1B_EVT_BLK_BIT_OFFSET        0x00
#define EFI_ACPI_PM1B_EVT_BLK_ADDRESS           0 // To be fixed

//
// PM1A Control Register Block Generic Address Information
//
#define EFI_ACPI_PM1A_CNT_BLK_ADDRESS_SPACE_ID  EFI_ACPI_2_0_SYSTEM_IO
#define EFI_ACPI_PM1A_CNT_BLK_BIT_WIDTH         0x10
#define EFI_ACPI_PM1A_CNT_BLK_BIT_OFFSET        0x00
#define EFI_ACPI_PM1A_CNT_BLK_ADDRESS           0 // To be fixed

//
// PM1B Control Register Block Generic Address Information
//
#define EFI_ACPI_PM1B_CNT_BLK_ADDRESS_SPACE_ID  EFI_ACPI_2_0_SYSTEM_IO
#define EFI_ACPI_PM1B_CNT_BLK_BIT_WIDTH         0x00
#define EFI_ACPI_PM1B_CNT_BLK_BIT_OFFSET        0x00
#define EFI_ACPI_PM1B_CNT_BLK_ADDRESS           0 // To be fixed

//
// PM2 Control Register Block Generic Address Information
//
#define EFI_ACPI_PM2_CNT_BLK_ADDRESS_SPACE_ID EFI_ACPI_2_0_SYSTEM_IO
#define EFI_ACPI_PM2_CNT_BLK_BIT_WIDTH        0x08
#define EFI_ACPI_PM2_CNT_BLK_BIT_OFFSET       0x00
#define EFI_ACPI_PM2_CNT_BLK_ADDRESS          0 // To be fixed

//
// Power Management Timer Control Register Block Generic Address
// Information
//
#define EFI_ACPI_PM_TMR_BLK_ADDRESS_SPACE_ID  EFI_ACPI_2_0_SYSTEM_IO
#define EFI_ACPI_PM_TMR_BLK_BIT_WIDTH         0x20
#define EFI_ACPI_PM_TMR_BLK_BIT_OFFSET        0x00
#define EFI_ACPI_PM_TMR_BLK_ADDRESS           0 // To be fixed

//
// General Purpose Event 0 Register Block Generic Address
// Information
//
#define EFI_ACPI_GPE0_BLK_ADDRESS_SPACE_ID  EFI_ACPI_2_0_SYSTEM_IO
#define EFI_ACPI_GPE0_BLK_BIT_WIDTH         0  // size of R_PCH_ACPI_GPE0_STS_127_96 + R_PCH_ACPI_GPE0_EN_127_96
#define EFI_ACPI_GPE0_BLK_BIT_OFFSET        0x00
#define EFI_ACPI_GPE0_BLK_ADDRESS           0 // To be fixed

//
// General Purpose Event 1 Register Block Generic Address
// Information
//
#define EFI_ACPI_GPE1_BLK_ADDRESS_SPACE_ID  EFI_ACPI_2_0_SYSTEM_IO
#define EFI_ACPI_GPE1_BLK_BIT_WIDTH         0x0
#define EFI_ACPI_GPE1_BLK_BIT_OFFSET        0x0
#define EFI_ACPI_GPE1_BLK_ADDRESS           0 // To be fixed
//
// Reset Register Generic Address Information
//
#define EFI_ACPI_RESET_REG_ADDRESS_SPACE_ID EFI_ACPI_2_0_SYSTEM_IO
#define EFI_ACPI_RESET_REG_BIT_WIDTH        0x08
#define EFI_ACPI_RESET_REG_BIT_OFFSET       0x00
#define EFI_ACPI_RESET_REG_ADDRESS          0x00000CF9
#define EFI_ACPI_RESET_VALUE                0x06

//
// Number of bytes decoded by PM1 event blocks (a and b)
//
#define EFI_ACPI_PM1_EVT_LEN  ((EFI_ACPI_PM1A_EVT_BLK_BIT_WIDTH + EFI_ACPI_PM1B_EVT_BLK_BIT_WIDTH) / 8)

//
// Number of bytes decoded by PM1 control blocks (a and b)
//
#define EFI_ACPI_PM1_CNT_LEN  ((EFI_ACPI_PM1A_CNT_BLK_BIT_WIDTH + EFI_ACPI_PM1B_CNT_BLK_BIT_WIDTH) / 8)

//
// Number of bytes decoded by PM2 control block
//
#define EFI_ACPI_PM2_CNT_LEN  (EFI_ACPI_PM2_CNT_BLK_BIT_WIDTH / 8)

//
// Number of bytes decoded by PM timer block
//
#define EFI_ACPI_PM_TMR_LEN (EFI_ACPI_PM_TMR_BLK_BIT_WIDTH / 8)

//
// Number of bytes decoded by GPE0 block
//
#define EFI_ACPI_GPE0_BLK_LEN (EFI_ACPI_GPE0_BLK_BIT_WIDTH / 8)

//
// Number of bytes decoded by GPE1 block
//
#define EFI_ACPI_GPE1_BLK_LEN (EFI_ACPI_GPE1_BLK_BIT_WIDTH / 8)

//
// Fixed ACPI Description Table
// Please modify all values in Fadt.h only.
//

EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE Fadt = {
  {
    EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE,
    sizeof (EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE),
    EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE_REVISION,

    //
    // Checksum will be updated at runtime
    //
    0x00,

    //
    // It is expected that these values will be updated at runtime
    //
    { ' ', ' ', ' ', ' ', ' ', ' ' },

    0,
    EFI_ACPI_OEM_FADT_REVISION,
    0,
    0
  },

  //
  // These addresses will be updated at runtime
  //
  0x00000000,
  0x00000000,

  EFI_ACPI_RESERVED_BYTE,
  EFI_ACPI_PREFERRED_PM_PROFILE,
  EFI_ACPI_SCI_INT,
  EFI_ACPI_SMI_CMD,
  EFI_ACPI_ACPI_ENABLE,
  EFI_ACPI_ACPI_DISABLE,
  EFI_ACPI_S4_BIOS_REQ,
  EFI_ACPI_PSTATE_CNT,

  EFI_ACPI_PM1A_EVT_BLK_ADDRESS,
  EFI_ACPI_PM1B_EVT_BLK_ADDRESS,
  EFI_ACPI_PM1A_CNT_BLK_ADDRESS,
  EFI_ACPI_PM1B_CNT_BLK_ADDRESS,
  EFI_ACPI_PM2_CNT_BLK_ADDRESS,
  EFI_ACPI_PM_TMR_BLK_ADDRESS,
  EFI_ACPI_GPE0_BLK_ADDRESS,
  EFI_ACPI_GPE1_BLK_ADDRESS,
  EFI_ACPI_PM1_EVT_LEN,
  EFI_ACPI_PM1_CNT_LEN,
  EFI_ACPI_PM2_CNT_LEN,
  EFI_ACPI_PM_TMR_LEN,
  EFI_ACPI_GPE0_BLK_LEN,
  EFI_ACPI_GPE1_BLK_LEN,
  EFI_ACPI_GPE1_BASE,

  //
  // Latest OS have C-State capability and CST_CNT SMI doesn't need to be defined.
  // CST_CNT SMI is not handled in BIOS and it can be removed safely.
  //
  EFI_ACPI_CST_CNT,
  EFI_ACPI_P_LVL2_LAT,
  EFI_ACPI_P_LVL3_LAT,
  EFI_ACPI_FLUSH_SIZE,
  EFI_ACPI_FLUSH_STRIDE,
  EFI_ACPI_DUTY_OFFSET,
  EFI_ACPI_DUTY_WIDTH,
  EFI_ACPI_DAY_ALRM,
  EFI_ACPI_MON_ALRM,
  EFI_ACPI_CENTURY,
  EFI_ACPI_IAPC_BOOT_ARCH,
  EFI_ACPI_RESERVED_BYTE,
  EFI_ACPI_FIXED_FEATURE_FLAGS,

  //
  // Reset Register Block
  //
  {
    EFI_ACPI_RESET_REG_ADDRESS_SPACE_ID,
    EFI_ACPI_RESET_REG_BIT_WIDTH,
    EFI_ACPI_RESET_REG_BIT_OFFSET,
    EFI_ACPI_5_0_BYTE,
    EFI_ACPI_RESET_REG_ADDRESS
  },
  EFI_ACPI_RESET_VALUE,
  {
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE
  },

  //
  // These addresses will be updated at runtime
  //
  0x0000000000000000, // X_FIRMWARE_CTRL
  0x0000000000000000, // X_DSDT

  {
    //
    // X_PM1a Event Register Block
    //
    EFI_ACPI_PM1A_EVT_BLK_ADDRESS_SPACE_ID,
    EFI_ACPI_PM1A_EVT_BLK_BIT_WIDTH,
    EFI_ACPI_PM1A_EVT_BLK_BIT_OFFSET,
    EFI_ACPI_5_0_WORD,
    EFI_ACPI_PM1A_EVT_BLK_ADDRESS
  },
  {
    //
    // X_PM1b Event Register Block
    //
    EFI_ACPI_PM1B_EVT_BLK_ADDRESS_SPACE_ID,
    EFI_ACPI_PM1B_EVT_BLK_BIT_WIDTH,
    EFI_ACPI_PM1B_EVT_BLK_BIT_OFFSET,
    EFI_ACPI_5_0_WORD,
    EFI_ACPI_PM1B_EVT_BLK_ADDRESS
  },
  {
    //
    // X_PM1a Control Register Block
    //
    EFI_ACPI_PM1A_CNT_BLK_ADDRESS_SPACE_ID,
    EFI_ACPI_PM1A_CNT_BLK_BIT_WIDTH,
    EFI_ACPI_PM1A_CNT_BLK_BIT_OFFSET,
    EFI_ACPI_5_0_WORD,
    EFI_ACPI_PM1A_CNT_BLK_ADDRESS
  },
  {
    //
    // X_PM1b Control Register Block
    //
    EFI_ACPI_PM1B_CNT_BLK_ADDRESS_SPACE_ID,
    EFI_ACPI_PM1B_CNT_BLK_BIT_WIDTH,
    EFI_ACPI_PM1B_CNT_BLK_BIT_OFFSET,
    EFI_ACPI_5_0_WORD,
    EFI_ACPI_PM1B_CNT_BLK_ADDRESS
  },
  {
    //
    // X_PM2 Control Register Block
    //
    EFI_ACPI_PM2_CNT_BLK_ADDRESS_SPACE_ID,
    EFI_ACPI_PM2_CNT_BLK_BIT_WIDTH,
    EFI_ACPI_PM2_CNT_BLK_BIT_OFFSET,
    EFI_ACPI_5_0_BYTE,
    EFI_ACPI_PM2_CNT_BLK_ADDRESS
  },
  {
    //
    // X_PM Timer Control Register Block
    //
    EFI_ACPI_PM_TMR_BLK_ADDRESS_SPACE_ID,
    EFI_ACPI_PM_TMR_BLK_BIT_WIDTH,
    EFI_ACPI_PM_TMR_BLK_BIT_OFFSET,
    EFI_ACPI_5_0_DWORD,
    EFI_ACPI_PM_TMR_BLK_ADDRESS
  },
  {
    //
    // X_General Purpose Event 0 Register Block
    //
    EFI_ACPI_GPE0_BLK_ADDRESS_SPACE_ID,
    EFI_ACPI_GPE0_BLK_BIT_WIDTH,
    EFI_ACPI_GPE0_BLK_BIT_OFFSET,
    EFI_ACPI_5_0_BYTE,
    EFI_ACPI_GPE0_BLK_ADDRESS
  },
  {
    //
    // X_General Purpose Event 1 Register Block
    //
    EFI_ACPI_GPE1_BLK_ADDRESS_SPACE_ID,
    EFI_ACPI_GPE1_BLK_BIT_WIDTH,
    EFI_ACPI_GPE1_BLK_BIT_OFFSET,
    EFI_ACPI_5_0_BYTE,
    EFI_ACPI_GPE1_BLK_ADDRESS
  },
  {
  //
  // Sleep Control Reg - update in DXE driver
  //
    0,
    0,
    0,
    0,
    0
  },
  {
  //
  // Sleep Status Reg - update in DXE driver
  //
    0,
    0,
    0,
    0,
    0
  }
};
