/** @file
  This file contains a structure definition for the ACPI 5.0 Firmware ACPI
  Control Structure (FACS).  The contents of this file should only be modified
  for bug fixes, no porting is required.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//

#include <IndustryStandard/Acpi.h>

//
// FACS Definitions
//
#define EFI_ACPI_FIRMWARE_WAKING_VECTOR 0x00000000
#define EFI_ACPI_GLOBAL_LOCK            0x00000000

//
// Firmware Control Structure Feature Flags are defined in AcpiX.0.h
//
#define EFI_ACPI_FIRMWARE_CONTROL_STRUCTURE_FLAGS 0x00000000

#define EFI_ACPI_X_FIRMWARE_WAKING_VECTOR         0x0000000000000000

#define EFI_ACPI_OSPM_FLAGS                       0x00000000


//
// Firmware ACPI Control Structure
// Please modify all values in Facs.h only.
//

EFI_ACPI_5_0_FIRMWARE_ACPI_CONTROL_STRUCTURE Facs = {
  EFI_ACPI_5_0_FIRMWARE_ACPI_CONTROL_STRUCTURE_SIGNATURE,
  sizeof (EFI_ACPI_5_0_FIRMWARE_ACPI_CONTROL_STRUCTURE),

  //
  // Hardware Signature will be updated at runtime
  //
  0x00000000,

  EFI_ACPI_FIRMWARE_WAKING_VECTOR,
  EFI_ACPI_GLOBAL_LOCK,
  EFI_ACPI_FIRMWARE_CONTROL_STRUCTURE_FLAGS,
  EFI_ACPI_X_FIRMWARE_WAKING_VECTOR,
  EFI_ACPI_5_0_FIRMWARE_ACPI_CONTROL_STRUCTURE_VERSION,
  {
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE
  },
  EFI_ACPI_OSPM_FLAGS,
  {
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE,
    EFI_ACPI_RESERVED_BYTE
  }
};
