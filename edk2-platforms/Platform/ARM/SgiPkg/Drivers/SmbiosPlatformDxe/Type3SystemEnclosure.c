/** @file
  SMBIOS Type 3 (System enclosure) table for ARM RD platforms.

  This file installs SMBIOS Type 3 (System enclosure) table for Arm Reference
  Design platforms. SMBIOS Type 3 table (System Enclosure) includes information
  about manufacturer, type, serial number and other information related to
  system enclosure.

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
    - SMBIOS Reference Specification 3.4.0, Chapter 7.4
**/

#include <Library/DebugLib.h>
#include <Protocol/Smbios.h>

#include "SmbiosPlatformDxe.h"

#define TYPE3_STRINGS                                   \
  "ARM LTD\0"                   /* Manufacturer */      \
  "Version not set\0"           /* Version */           \
  "Serial not set\0"            /* Serial */            \
  "Asset Tag not set\0"         /* Asset Tag */

typedef enum {
  ManufacturerName = 1,
  Version,
  SerialNumber,
  AssetTag
} TYPE3_STRING_ELEMENTS;

/* SMBIOS Type3 structure */
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE3  Base;
  CHAR8               Strings[sizeof (TYPE3_STRINGS)];
} ARM_RD_SMBIOS_TYPE3;
#pragma pack()

/* System information */
STATIC ARM_RD_SMBIOS_TYPE3 mArmRdSmbiosType3 = {
  {
    {
      // SMBIOS header
      EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE,   // Type 3
      sizeof (SMBIOS_TABLE_TYPE1),        // Length
      SMBIOS_HANDLE_ENCLOSURE,            // Assign an unused handle number
    },
    ManufacturerName,               // Manufacturer
    2,                              // Enclosure type unknown
    Version,                        // Version
    SerialNumber,                   // Serial
    AssetTag,                       // Asset Tag
    ChassisStateSafe,               // Boot chassis state
    ChassisStateSafe,               // Power supply state
    ChassisStateSafe,               // Thermal state
    ChassisSecurityStatusUnknown,   // Security Status
    {0},                            // BIOS vendor specific Information
  },
  // Text strings (unformatted)
  TYPE3_STRINGS
};

/**
  Install SMBIOS System Enclosure Table

  Install the SMBIOS System Enclosure (type 3) table for Arm's Reference Design
  platforms.

  @param[in]  Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
InstallType3SystemEnclosure (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  )
{
  EFI_STATUS Status;
  EFI_SMBIOS_HANDLE SmbiosHandle;

  SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType3)->Handle;

  /* Install type 3 table */
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosHandle,
                     (EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType3
                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "SMBIOS: Failed to install Type3 SMBIOS table.\n"
      ));
  }

  return Status;
}
