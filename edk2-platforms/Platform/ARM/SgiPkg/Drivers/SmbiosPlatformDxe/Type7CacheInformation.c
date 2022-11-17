/** @file
  SMBIOS Type 7 (Cache information) table for ARM RD platforms.

  This file installs SMBIOS Type 7 (Cache information) table for Arm's
  Reference Design platforms. It includes information about cache levels
  implemented, cache configuration, ways of associativity and other
  information related to cache memory installed.

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
    - SMBIOS Reference Specification 3.4.0, Chapter 7.8
**/

#include <Library/DebugLib.h>
#include <Protocol/Smbios.h>

#include "SgiPlatform.h"
#include "SmbiosPlatformDxe.h"

#define TYPE7_STRINGS                                   \
  "L1 Instruction\0"            /* L1I */               \
  "L1 Data\0"                   /* L1D */               \
  "L2\0"                        /* L2  */               \
  "L3\0"                        /* L3  */               \
  "SLC\0"                       /* L4  */

typedef enum {
  L1Instruction = 1,
  L1Data,
  L2,
  L3,
  Slc,
} TYPE7_STRING_ELEMENTS;

/* SMBIOS Type7 structure */
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE7  Base;
  CHAR8               Strings[sizeof (TYPE7_STRINGS)];
} ARM_RD_SMBIOS_TYPE7;
#pragma pack()

/* Cache information */
STATIC ARM_RD_SMBIOS_TYPE7 mArmRdSmbiosType7[] = {
  {   // Entry 0, L1 instruction cache
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_CACHE_INFORMATION, // Type 7
        sizeof (SMBIOS_TABLE_TYPE7),       // Length
        SMBIOS_HANDLE_L1I_CACHE,           // Handle number
      },
      L1Instruction,
      (
        (1 << 8) | // Write-back
        (1 << 7) | // Cache enabled
        (1 << 3) | // Cache socketed
        0x0        // Cache level 1
      ),
      0xFFFF,      // Uses Maximum cache size 2 field
      0xFFFF,      // Uses Installed cache size 2 field
      {0, 1},      // Supported SRAM type unknown
      {0, 1},      // Current SRAM type unknown
      0,           // Cache Speed Unknown
      0x02,        // Error correction type unknown
      0x03,        // Instruction cache
      0,           // Associativity, update dynamically
      0,           // Maximum cache size 2, update dynamically
      0            // Installed cache size 2, update dynamically
    },
    // Text strings (unformatted area)
    TYPE7_STRINGS
  },
  {   // Entry 1, L1 data cache
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_CACHE_INFORMATION, // Type 7
        sizeof (SMBIOS_TABLE_TYPE7),       // Length
        SMBIOS_HANDLE_L1D_CACHE,           // Handle number
      },
      L1Data,
      (
        (1 << 8) | // Write-back
        (1 << 7) | // Cache enabled
        (1 << 3) | // Cache socketed
        0x0        // Cache level 1
      ),
      0xFFFF,      // Uses Maximum cache size 2 field
      0xFFFF,      // Uses Installed cache size 2 field
      {0, 1},      // Supported SRAM type unknown
      {0, 1},      // Current SRAM type unknown
      0,           // Cache Speed Unknown
      0x02,        // Error correction type unknown
      0x04,        // Data cache
      0,           // Associativity, update dynamically
      0,           // Maximum cache size 2, update dynamically
      0            // Installed cache size 2, update dynamically
    },
    // Text strings (unformatted area)
    TYPE7_STRINGS
  },
  {   // Entry 2, L2 cache
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_CACHE_INFORMATION, // Type 7
        sizeof (SMBIOS_TABLE_TYPE7),       // Length
        SMBIOS_HANDLE_L2_CACHE,            // Handle number
      },
      L2,
      (
        (1 << 8) | // Write-back
        (1 << 7) | // Cache enabled
        (1 << 3) | // Cache socketed
        0x1        // Cache level 2
      ),
      0xFFFF,      // Uses Maximum cache size 2 field
      0xFFFF,      // Uses Installed cache size 2 field
      {0, 1},      // Supported SRAM type unknown
      {0, 1},      // Current SRAM type unknown
      0,           // Cache Speed Unknown
      0x02,        // Error correction type unknown
      0x05,        // Unified cache
      0,           // Associativity, update dynamically
      0,           // Maximum cache size 2, update dynamically
      0            // Installed cache size 2, update dynamically
    },
    // Text strings (unformatted area)
    TYPE7_STRINGS
  },
  {   // Entry 3, L3 cache
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_CACHE_INFORMATION, // Type 7
        sizeof (SMBIOS_TABLE_TYPE7),       // Length
        SMBIOS_HANDLE_L3_CACHE,            // Handle number
      },
      L3,
      (
        (1 << 8) | // Write-back
        (1 << 7) | // Cache enabled
        (1 << 3) | // Cache socketed
        0x2        // Cache level 3
      ),
      0xFFFF,      // Uses Maximum cache size 2 field
      0xFFFF,      // Uses Installed cache size 2 field
      {0, 1},      // Supported SRAM type unknown
      {0, 1},      // Current SRAM type unknown
      0,           // Cache Speed Unknown
      0x02,        // Error correction type unknown
      0x05,        // Unified cache
      0,           // Associativity, update dynamically
      0,           // Maximum cache size 2, update dynamically
      0            // Installed cache size 2, update dynamically
    },
    // Text strings (unformatted area)
    TYPE7_STRINGS
  },
  {   // Entry 4, SLC Cache
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_CACHE_INFORMATION, // Type 7
        sizeof (SMBIOS_TABLE_TYPE7),       // Length
        SMBIOS_HANDLE_L4_CACHE,            // Handle number
      },
      Slc,
      (
        (1 << 8) | // Write-back
        (1 << 7) | // Cache enabled
        (1 << 3) | // Cache socketed
        0x3        // Cache level 4
      ),
      0xFFFF,      // Uses Maximum cache size 2 field
      0xFFFF,      // Uses Installed cache size 2 field
      {0, 1},      // Supported SRAM type unknown
      {0, 1},      // Current SRAM type unknown
      0,           // Cache Speed Unknown
      0x02,        // Error correction type unknown
      0x05,        // Unified cache
      0,           // Associativity, update dynamically
      0,           // Maximum cache size 2, update dynamically
      0            // Installed cache size 2, update dynamically
    },
    // Text strings (unformatted area)
    TYPE7_STRINGS
  }
};

/**
  Install SMBIOS Cache information Table

  Install the SMBIOS Cache information (type 7) table for Arm's Reference
  Design platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_NOT_FOUND         Unknown product id.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
InstallType7CacheInformation (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  )
{
  EFI_STATUS Status;
  EFI_SMBIOS_HANDLE SmbiosHandle;
  UINT8 CacheIdx;

  /* Update the cache attributes based on the product */
  switch (SgiGetProductId ()) {
  case Sgi575:
    /* L1 instruction cache */
    mArmRdSmbiosType7[0].Base.MaximumCacheSize2 = 64;     // 64KB
    mArmRdSmbiosType7[0].Base.InstalledSize2 = 64;        // 64KB
    mArmRdSmbiosType7[0].Base.Associativity = CacheAssociativity4Way;
    /* L1 data cache */
    mArmRdSmbiosType7[1].Base.MaximumCacheSize2 = 64;     // 64KB
    mArmRdSmbiosType7[1].Base.InstalledSize2 = 64;        // 64KB
    mArmRdSmbiosType7[1].Base.Associativity = CacheAssociativity16Way;
    /* L2 cache */
    mArmRdSmbiosType7[2].Base.MaximumCacheSize2 = 512;    // 512KB
    mArmRdSmbiosType7[2].Base.InstalledSize2 = 512;       // 512KB
    mArmRdSmbiosType7[2].Base.Associativity = CacheAssociativity8Way;
    /* L3 cache */
    mArmRdSmbiosType7[3].Base.MaximumCacheSize2 = 2048;   // 2MB
    mArmRdSmbiosType7[3].Base.InstalledSize2 = 2048;      // 2MB
    mArmRdSmbiosType7[3].Base.Associativity = CacheAssociativity16Way;
    break;
  case RdN1Edge:
  case RdN1EdgeX2:
    /* L1 instruction cache */
    mArmRdSmbiosType7[0].Base.MaximumCacheSize2 = 64;    // 64KB
    mArmRdSmbiosType7[0].Base.InstalledSize2 = 64;       // 64KB
    mArmRdSmbiosType7[0].Base.Associativity = CacheAssociativity4Way;
    /* L1 data cache */
    mArmRdSmbiosType7[1].Base.MaximumCacheSize2 = 64;    // 64KB
    mArmRdSmbiosType7[1].Base.InstalledSize2 = 64;       // 64KB
    mArmRdSmbiosType7[1].Base.Associativity = CacheAssociativity4Way;
    /* L2 cache */
    mArmRdSmbiosType7[2].Base.MaximumCacheSize2 = 512;   // 512KB
    mArmRdSmbiosType7[2].Base.InstalledSize2 = 512;      // 512KB
    mArmRdSmbiosType7[2].Base.Associativity = CacheAssociativity8Way;
    /* L3 cache */
    mArmRdSmbiosType7[3].Base.MaximumCacheSize2 = 2048;  // 2MB
    mArmRdSmbiosType7[3].Base.InstalledSize2 = 2048;     // 2MB
    mArmRdSmbiosType7[3].Base.Associativity = CacheAssociativity16Way;
    /* System level cache */
    mArmRdSmbiosType7[4].Base.MaximumCacheSize2 = 8192;  // 8MB SLC per chip
    mArmRdSmbiosType7[4].Base.InstalledSize2 = 8192;     // 8MB SLC per chip
    mArmRdSmbiosType7[4].Base.Associativity = CacheAssociativity16Way;
    break;
  case RdE1Edge:
    /* L1 instruction cache */
    mArmRdSmbiosType7[0].Base.MaximumCacheSize2 = 32;    // 32KB
    mArmRdSmbiosType7[0].Base.InstalledSize2 = 32;       // 32KB
    mArmRdSmbiosType7[0].Base.Associativity = CacheAssociativity4Way;
    /* L1 data cache */
    mArmRdSmbiosType7[1].Base.MaximumCacheSize2 = 32;    // 32KB
    mArmRdSmbiosType7[1].Base.InstalledSize2 = 32;       // 32KB
    mArmRdSmbiosType7[1].Base.Associativity = CacheAssociativity4Way;
    /* L2 cache */
    mArmRdSmbiosType7[2].Base.MaximumCacheSize2 = 256;   // 256KB
    mArmRdSmbiosType7[2].Base.InstalledSize2 = 256;      // 256KB
    mArmRdSmbiosType7[2].Base.Associativity = CacheAssociativity4Way;
    /* L3 cache */
    mArmRdSmbiosType7[3].Base.MaximumCacheSize2 = 2048;  // 2MB
    mArmRdSmbiosType7[3].Base.InstalledSize2 = 2048;     // 2MB
    mArmRdSmbiosType7[3].Base.Associativity = CacheAssociativity16Way;
    /* System level cache */
    mArmRdSmbiosType7[4].Base.MaximumCacheSize2 = 8192;  // 8MB SLC
    mArmRdSmbiosType7[4].Base.InstalledSize2 = 8192;     // 8MB SLC
    mArmRdSmbiosType7[4].Base.Associativity = CacheAssociativity16Way;
    break;
  case RdV1:
  case RdV1Mc:
    /* L1 instruction cache */
    mArmRdSmbiosType7[0].Base.MaximumCacheSize2 = 64;    // 64KB
    mArmRdSmbiosType7[0].Base.InstalledSize2 = 64;       // 64KB
    mArmRdSmbiosType7[0].Base.Associativity = CacheAssociativity4Way;
    /* L1 data cache */
    mArmRdSmbiosType7[1].Base.MaximumCacheSize2 = 64;    // 64KB
    mArmRdSmbiosType7[1].Base.InstalledSize2 = 64;       // 64KB
    mArmRdSmbiosType7[1].Base.Associativity = CacheAssociativity4Way;
    /* L2 cache */
    mArmRdSmbiosType7[2].Base.MaximumCacheSize2 = 1024;  // 1MB
    mArmRdSmbiosType7[2].Base.InstalledSize2 = 1024;     // 1MB
    mArmRdSmbiosType7[2].Base.Associativity = CacheAssociativity8Way;
    /* System level cache */
    mArmRdSmbiosType7[4].Base.MaximumCacheSize2 = 16384; // 16MB SLC per chip
    mArmRdSmbiosType7[4].Base.InstalledSize2 = 16384;    // 16MB SLC per chip
    mArmRdSmbiosType7[4].Base.Associativity = CacheAssociativity16Way;
    break;
  case RdN2:
    /* L1 instruction cache */
    mArmRdSmbiosType7[0].Base.MaximumCacheSize2 = 64;    // 64KB
    mArmRdSmbiosType7[0].Base.InstalledSize2 = 64;       // 64KB
    mArmRdSmbiosType7[0].Base.Associativity = CacheAssociativity4Way;
    /* L1 data cache */
    mArmRdSmbiosType7[1].Base.MaximumCacheSize2 = 64;    // 64KB
    mArmRdSmbiosType7[1].Base.InstalledSize2 = 64;       // 64KB
    mArmRdSmbiosType7[1].Base.Associativity = CacheAssociativity4Way;
    /* L2 cache */
    mArmRdSmbiosType7[2].Base.MaximumCacheSize2 = 1024;  // 1MB
    mArmRdSmbiosType7[2].Base.InstalledSize2 = 1024;     // 1MB
    mArmRdSmbiosType7[2].Base.Associativity = CacheAssociativity8Way;
    /* System level cache */
    mArmRdSmbiosType7[4].Base.MaximumCacheSize2 = 32768; // 32MB SLC
    mArmRdSmbiosType7[4].Base.InstalledSize2 = 32768;    // 32MB SLC
    mArmRdSmbiosType7[4].Base.Associativity = CacheAssociativity16Way;
    break;
  case RdN2Cfg1:
    /* L1 instruction cache */
    mArmRdSmbiosType7[0].Base.MaximumCacheSize2 = 64;    // 64KB
    mArmRdSmbiosType7[0].Base.InstalledSize2 = 64;       // 64KB
    mArmRdSmbiosType7[0].Base.Associativity = CacheAssociativity4Way;
    /* L1 data cache */
    mArmRdSmbiosType7[1].Base.MaximumCacheSize2 = 64;    // 64KB
    mArmRdSmbiosType7[1].Base.InstalledSize2 = 64;       // 64KB
    mArmRdSmbiosType7[1].Base.Associativity = CacheAssociativity4Way;
    /* L2 cache */
    mArmRdSmbiosType7[2].Base.MaximumCacheSize2 = 1024;  // 1MB
    mArmRdSmbiosType7[2].Base.InstalledSize2 = 1024;     // 1MB
    mArmRdSmbiosType7[2].Base.Associativity = CacheAssociativity8Way;
    /* System level cache */
    mArmRdSmbiosType7[4].Base.MaximumCacheSize2 = 8192;  // 8MB SLC
    mArmRdSmbiosType7[4].Base.InstalledSize2 = 8192;     // 8MB SLC
    mArmRdSmbiosType7[4].Base.Associativity = CacheAssociativity16Way;
    break;
  }

  /* Install valid cache information tables */
  for (CacheIdx = 0; CacheIdx < ARRAY_SIZE (mArmRdSmbiosType7); CacheIdx++) {
    if (mArmRdSmbiosType7[CacheIdx].Base.MaximumCacheSize2 == 0) {
      continue;
    }

    SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType7[CacheIdx])->Handle;
    Status = Smbios->Add (
                       Smbios,
                       NULL,
                       &SmbiosHandle,
                       (EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType7[CacheIdx]
                       );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "SMBIOS: Failed to install Type7 SMBIOS table.\n"
        ));
    }
  }

  return Status;
}
