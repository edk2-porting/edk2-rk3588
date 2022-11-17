/** @file
  SMBIOS Type 32 (System Boot Information) table for ARM RD platforms.

  This file installs SMBIOS Type 32 (System Boot Information) table for Arm's
  Reference Design platforms. It includes information about the System Boot
  Status.

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
    - SMBIOS Reference Specification 3.4.0, Chapter 7.33
**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Protocol/Smbios.h>

#include "SmbiosPlatformDxe.h"

#define TYPE32_STRINGS                                  \
  "\0"                          /* Null string */

/* SMBIOS Type32 structure */
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE32 Base;
  CHAR8               Strings[sizeof (TYPE32_STRINGS)];
} ARM_RD_SMBIOS_TYPE32;
#pragma pack()

/* System Boot Information */
STATIC ARM_RD_SMBIOS_TYPE32 mArmRdSmbiosType32 = {
  {
    {
      // SMBIOS header
      EFI_SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION,  // Type 32
      sizeof (SMBIOS_TABLE_TYPE32),     // Length
      SMBIOS_HANDLE_PI_RESERVED
    },
    {0},                                // Reserved field
    BootInformationStatusNoError        // Boot status
  },
  // Text strings (unformatted area)
  TYPE32_STRINGS
};

/**
  Install SMBIOS system boot information

  Install the SMBIOS system boot information (type 32) table for RD platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
InstallType32SystemBootInformation (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  )
{
  EFI_STATUS Status;
  EFI_SMBIOS_HANDLE SmbiosHandle;

  SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType32)->Handle;

  /* Install type 32 table */
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosHandle,
                     (EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType32
                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "SMBIOS: Failed to install Type32 SMBIOS table.\n"
      ));
  }

  return Status;
}
