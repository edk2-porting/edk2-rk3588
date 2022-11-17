/** @file
  SMBIOS Type 19 (Memory Array Mapped Address) table for ARM RD platforms.

  This file installs SMBIOS Type 19 (Memory Array Mapped Address) table for Arm's
  Reference Design platforms. It includes information about the address mapping
  for a Physical Memory Array.

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
    - SMBIOS Reference Specification 3.4.0, Chapter 7.20
**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Protocol/Smbios.h>

#include "SmbiosPlatformDxe.h"

#define TYPE19_STRINGS                                  \
  "\0"                          /* Null string */

/* SMBIOS Type19 structure */
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE19 Base;
  CHAR8               Strings[sizeof (TYPE19_STRINGS)];
} ARM_RD_SMBIOS_TYPE19;
#pragma pack()

/* Memory Array Mapped Address */
STATIC ARM_RD_SMBIOS_TYPE19 mArmRdSmbiosType19 = {
  {
    {
      // SMBIOS header
      EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS,  // Type 19
      sizeof (SMBIOS_TABLE_TYPE19),     // Length
      SMBIOS_HANDLE_PI_RESERVED,        // Assign an unused handle number
    },
    0,                                  // Starting address
    0,                                  // Ending address
    SMBIOS_HANDLE_PHYSICAL_MEMORY,      // Memory array handle
    1                                   // Partition width
  },
  // Text strings (unformatted area)
  TYPE19_STRINGS
};

/**
  Install SMBIOS memory array mapped address table

  Install the SMBIOS memory array mapped address (type 19) table for RD
  platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
InstallType19MemoryArrayMappedAddress (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  )
{
  EFI_STATUS Status;
  EFI_SMBIOS_HANDLE SmbiosHandle;

  SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType19)->Handle;

  mArmRdSmbiosType19.Base.StartingAddress = 0xFFFFFFFF;
  mArmRdSmbiosType19.Base.EndingAddress = 0xFFFFFFFF;
  mArmRdSmbiosType19.Base.ExtendedStartingAddress =
          PcdGet64 (PcdSystemMemoryBase);
  mArmRdSmbiosType19.Base.ExtendedEndingAddress =
          (PcdGet64 (PcdSystemMemoryBase) +
           PcdGet64 (PcdSystemMemorySize) +
           SIZE_16MB    // 16MB Trusted DRAM
          ) - 1;

  /* Install type 19 table */
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosHandle,
                     (EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType19
                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "SMBIOS: Failed to install Type19 SMBIOS table.\n"
      ));
  }

  return Status;
}
