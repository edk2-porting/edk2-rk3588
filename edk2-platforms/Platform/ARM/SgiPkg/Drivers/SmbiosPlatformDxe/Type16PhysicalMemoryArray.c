/** @file
  SMBIOS Type 16 (Physical Memory Array) table for ARM RD platforms.

  This file installs SMBIOS Type 16 (Physical Memory Array) table for Arm's
  Reference Design platforms. It describes a collection of memory devices that
  operate together to form a memory address. It includes information about
  number of devices, total memory installed, error correction mechanism used
  and other related information.

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
    - SMBIOS Reference Specification 3.4.0, Chapter 7.17
**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Protocol/Smbios.h>

#include "SmbiosPlatformDxe.h"

#define TYPE16_STRINGS                                  \
  "\0"                          /* Null string */

/* SMBIOS Type16 structure */
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE16 Base;
  CHAR8               Strings[sizeof (TYPE16_STRINGS)];
} ARM_RD_SMBIOS_TYPE16;
#pragma pack()

/* Physical Memory Array */
STATIC ARM_RD_SMBIOS_TYPE16 mArmRdSmbiosType16 = {
  {
    {
      // SMBIOS header
      EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, // Type 16
      sizeof (SMBIOS_TABLE_TYPE16),          // Length
      SMBIOS_HANDLE_PHYSICAL_MEMORY
    },
    MemoryArrayLocationSystemBoard,     // Location
    MemoryArrayUseSystemMemory,         // Used as system memory
    MemoryErrorCorrectionUnknown,       // Error correction
    0x80000000, // Maximum capacity in KiB, uses Extended Maximum capacity field
    0xFFFE,     // Memory error info handle, does not provide this info
    0,          // Num of memory devices, update dymamically
    0           // Extended Maximum capacity, update dymamically
  },
  // Text strings (unformatted area)
  TYPE16_STRINGS
};

/**
  Install SMBIOS physical memory array table.

  Install the SMBIOS physical memory array (type 16) table for Arm's Reference
  Design platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
InstallType16PhysicalMemoryArray (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  )
{
  EFI_STATUS Status;
  EFI_SMBIOS_HANDLE SmbiosHandle;
  UINT16 NumOfMemoryDevices = 1;
  UINT64 InstalledMemory;

  SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType16)->Handle;

  /* Include 16MB of Trusted DRAM as well */
  InstalledMemory = PcdGet64 (PcdSystemMemorySize) + SIZE_16MB;
  if (PcdGet64 (PcdDramBlock2Size) != 0) {
    NumOfMemoryDevices++;
    InstalledMemory += PcdGet64 (PcdDramBlock2Size);
  }

  mArmRdSmbiosType16.Base.ExtendedMaximumCapacity =
    InstalledMemory * FixedPcdGet32 (PcdChipCount);
  mArmRdSmbiosType16.Base.NumberOfMemoryDevices =
    NumOfMemoryDevices * FixedPcdGet32 (PcdChipCount);

  /* Install type 16 table */
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosHandle,
                     (EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType16
                     );
   if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "SMBIOS: Failed to install Type16 SMBIOS table.\n"
      ));
  }

  return Status;
}
