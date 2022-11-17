/** @file
  SMBIOS Type 17 (Memory Device) table for ARM RD platforms.

  This file installs SMBIOS Type 17 (Memory Device) table for Arm's Reference
  Design platforms. It includes the specification of each installed memory
  device such as size of each device, bank locator, memory device type, and
  other related information.

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
    - SMBIOS Reference Specification 3.4.0, Chapter 7.18
**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Protocol/Smbios.h>

#include "SmbiosPlatformDxe.h"

#define TYPE17_STRINGS                                  \
  "Chip 0 Bank 0\0"                                     \
  "Chip 1 Bank 0\0"                                     \
  "Chip 2 Bank 0\0"                                     \
  "Chip 3 Bank 0\0"                                     \
  "Chip 0 Bank 1\0"                                     \
  "Chip 1 Bank 1\0"                                     \
  "Chip 2 Bank 1\0"                                     \
  "Chip 3 Bank 1\0"

typedef enum {
  Chip0Bank0 = 1,
  Chip1Bank0,
  Chip2Bank0,
  Chip3Bank0,
  Chip0Bank1,
  Chip1Bank1,
  Chip2Bank1,
  Chip3Bank1
} TYPE17_STRING_ELEMENTS;

/* SMBIOS Type17 structure */
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE17 Base;
  CHAR8               Strings[sizeof (TYPE17_STRINGS)];
} ARM_RD_SMBIOS_TYPE17;
#pragma pack()

/* Memory Device */
STATIC ARM_RD_SMBIOS_TYPE17 mArmRdSmbiosType17[] = {
  {
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_MEMORY_DEVICE,  // Type 17
        sizeof (SMBIOS_TABLE_TYPE17),   // Length
        SMBIOS_HANDLE_MEMORY_DEVICE0000
      },
      SMBIOS_HANDLE_PHYSICAL_MEMORY,    // Physical memory array handle
      0xFFFE,                           // Memory error info handle
      0xFFFF,                           // Total width unknown
      0xFFFF,                           // Data width unknown
      0,                                // Size, Update dynamically
      MemoryFormFactorOther,            // Form Factor
      0,                                // Device set, not part of a set
      0,                                // Device locator
      Chip0Bank0,                       // Chip 0 Bank 0
      MemoryTypeDram,                   // Memory type
      {0, 1},                           // Type details others
    },
    // Text strings (unformatted area)
    TYPE17_STRINGS
  },
  {
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_MEMORY_DEVICE,  // Type 17
        sizeof (SMBIOS_TABLE_TYPE17),   // Length
        SMBIOS_HANDLE_MEMORY_DEVICE0001
      },
      SMBIOS_HANDLE_PHYSICAL_MEMORY,    // Physical memory array handle
      0xFFFE,                           // Memory error info handle
      0xFFFF,                           // Total width unknown
      0xFFFF,                           // Data width unknown
      0,                                // Size, Update dynamically
      MemoryFormFactorOther,            // Form Factor
      0,                                // Device set, not part of a set
      0,                                // Device locator
      Chip0Bank1,                       // Chip 0 Bank 1
      MemoryTypeDram,                   // Memory type
      {0, 1},                           // Type details others
    },
    // Text strings (unformatted area)
    TYPE17_STRINGS
  },
  {
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_MEMORY_DEVICE,  // Type 17
        sizeof (SMBIOS_TABLE_TYPE17),   // Length
        SMBIOS_HANDLE_MEMORY_DEVICE0100
      },
      SMBIOS_HANDLE_PHYSICAL_MEMORY,    // Physical memory array handle
      0xFFFE,                           // Memory error info handle
      0xFFFF,                           // Total width unknown
      0xFFFF,                           // Data width unknown
      0,                                // Size, Update dynamically
      MemoryFormFactorOther,            // Form Factor
      0,                                // Device set, not part of a set
      0,                                // Device locator
      Chip1Bank0,                       // Chip 1 Bank 0
      MemoryTypeDram,                   // Memory type
      {0, 1},                           // Type details others
    },
    // Text strings (unformatted area)
    TYPE17_STRINGS
  },
  {
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_MEMORY_DEVICE,  // Type 17
        sizeof (SMBIOS_TABLE_TYPE17),   // Length
        SMBIOS_HANDLE_MEMORY_DEVICE0101
      },
      SMBIOS_HANDLE_PHYSICAL_MEMORY,    // Physical memory array handle
      0xFFFE,                           // Memory error info handle
      0xFFFF,                           // Total width unknown
      0xFFFF,                           // Data width unknown
      0,                                // Size, Update dynamically
      MemoryFormFactorOther,            // Form Factor
      0,                                // Device set, not part of a set
      0,                                // Device locator
      Chip1Bank1,                       // Chip 1 Bank 1
      MemoryTypeDram,                   // Memory type
      {0, 1},                           // Type details others
    },
    // Text strings (unformatted area)
    TYPE17_STRINGS
  },
  {
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_MEMORY_DEVICE,  // Type 17
        sizeof (SMBIOS_TABLE_TYPE17),   // Length
        SMBIOS_HANDLE_MEMORY_DEVICE0200
      },
      SMBIOS_HANDLE_PHYSICAL_MEMORY,    // Physical memory array handle
      0xFFFE,                           // Memory error info handle
      0xFFFF,                           // Total width unknown
      0xFFFF,                           // Data width unknown
      0,                                // Size, Update dynamically
      MemoryFormFactorOther,            // Form Factor
      0,                                // Device set, not part of a set
      0,                                // Device locator
      Chip2Bank0,                       // Chip 2 Bank 0
      MemoryTypeDram,                   // Memory type
      {0, 1},                           // Type details others
    },
    // Text strings (unformatted area)
    TYPE17_STRINGS
  },
  {
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_MEMORY_DEVICE,  // Type 17
        sizeof (SMBIOS_TABLE_TYPE17),   // Length
        SMBIOS_HANDLE_MEMORY_DEVICE0201
      },
      SMBIOS_HANDLE_PHYSICAL_MEMORY,    // Physical memory array handle
      0xFFFE,                           // Memory error info handle
      0xFFFF,                           // Total width unknown
      0xFFFF,                           // Data width unknown
      0,                                // Size, Update dynamically
      MemoryFormFactorOther,            // Form Factor
      0,                                // Device set, not part of a set
      0,                                // Device locator
      Chip2Bank1,                       // Chip 2 Bank 1
      MemoryTypeDram,                   // Memory type
      {0, 1},                           // Type details others
    },
    // Text strings (unformatted area)
    TYPE17_STRINGS
  },
  {
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_MEMORY_DEVICE,  // Type 17
        sizeof (SMBIOS_TABLE_TYPE17),   // Length
        SMBIOS_HANDLE_MEMORY_DEVICE0300
      },
      SMBIOS_HANDLE_PHYSICAL_MEMORY,    // Physical memory array handle
      0xFFFE,                           // Memory error info handle
      0xFFFF,                           // Total width unknown
      0xFFFF,                           // Data width unknown
      0,                                // Size, Update dynamically
      MemoryFormFactorOther,            // Form Factor
      0,                                // Device set, not part of a set
      0,                                // Device locator
      Chip3Bank0,                       // Chip 3 Bank 0
      MemoryTypeDram,                   // Memory type
      {0, 1},                           // Type details others
    },
    // Text strings (unformatted area)
    TYPE17_STRINGS
  },
  {
    {
      {
        // SMBIOS header
        EFI_SMBIOS_TYPE_MEMORY_DEVICE,  // Type 17
        sizeof (SMBIOS_TABLE_TYPE17),   // Length
        SMBIOS_HANDLE_MEMORY_DEVICE0301
      },
      SMBIOS_HANDLE_PHYSICAL_MEMORY,    // Physical memory array handle
      0xFFFE,                           // Memory error info handle
      0xFFFF,                           // Total width unknown
      0xFFFF,                           // Data width unknown
      0,                                // Size, Update dynamically
      MemoryFormFactorOther,            // Form Factor
      0,                                // Device set, not part of a set
      0,                                // Device locator
      Chip3Bank1,                       // Chip 3 Bank 1
      MemoryTypeDram,                   // Memory type
      {0, 1},                           // Type details others
    },
    // Text strings (unformatted area)
    TYPE17_STRINGS
  },
};

/** Update the memory size fields in SMBIOS Memory Device (Type 17) table.

  @param  [in]  Type17Table  Pointer to the Type 17 table.
  @param  [in]  MemorySize   Memory size available on the platform.
                             - If no memory device is installed, this value
                               is 0.
                             - If size is unknown, this value is MAX_UINT64.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER Invalid Type 17 Table pointer.
**/
STATIC
UINTN
UpdateMemorySize (
  IN  SMBIOS_TABLE_TYPE17   *Type17Table,
  IN  UINT64                MemorySize
  )
{
  if (Type17Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  /* Ref: SMBIOS Specifiation, Version 3.4.0, Document Identifier: DSP0134,
     Table 75 – Memory Device (Type 17) structure, description for Size field.
     If the value is 0, no memory device is installed in the socket; if
     the size is unknown, the field value is FFFFh.
  */
  if (MemorySize == 0) {
    Type17Table->Size = 0;
    Type17Table->ExtendedSize = 0;
    return EFI_SUCCESS;
  }

  if (MemorySize == MAX_UINT64) {
    Type17Table->Size = MAX_UINT16;
    Type17Table->ExtendedSize = 0;
    return EFI_SUCCESS;
  }

  /* Ref: SMBIOS Specifiation, Version 3.4.0, Document Identifier: DSP0134,
     Table 75 – Memory Device (Type 17) structure, description for Size field.
     If the size is 32 GB-1 MB or greater, the field value is 7FFFh and the
     actual size is stored in the Extended Size field.
  */
  if (MemorySize < (SIZE_32GB - SIZE_1MB)) {
    /* Ref: SMBIOS Specifiation, Version 3.4.0, Document Identifier: DSP0134,
       section 7.18.5 Memory Device — Extended Size
       For compatibility with older SMBIOS parsers, memory devices
       smaller than (32 GB - 1 MB) should be represented using their
       size in the Size field, leaving the Extended Size field set to 0.
    */
    Type17Table->ExtendedSize = 0;

    /* Ref: SMBIOS Specifiation, Version 3.4.0, Document Identifier: DSP0134,
       Table 75 – Memory Device (Type 17) structure, description for Size field.
       The granularity in which the value is specified depends on the setting
       of the most-significant bit (bit 15). If the bit is 0, the value is
       specified in megabyte units; if the bit is 1, the value is specified
       in kilobyte units.
       For example, the value 8100h identifies a 256 KB memory device
       and 0100h identifies a 256 MB memory device.
    */
    if (MemorySize < SIZE_1MB) {
      Type17Table->Size = MemorySize >> 10;
      Type17Table->Size |= BIT15;
    } else {
      Type17Table->Size = MemorySize >> 20;
    }
    return EFI_SUCCESS;
  }

  /* Ref: SMBIOS Specifiation, Version 3.4.0, Document Identifier: DSP0134,
      section 7.18.5 Memory Device — Extended Size
      The Extended Size field is intended to represent memory devices
      larger than 32,767 MB (32 GB - 1 MB), which cannot be described
      using the Size field. This field is only meaningful if the value
      in the Size field is 7FFFh.
  */
  Type17Table->Size = 0x7FFF;

  /* Ref: SMBIOS Specifiation, Version 3.4.0, Document Identifier: DSP0134,
     section 7.18.5 Memory Device — Extended Size
     Bit 31 is reserved for future use and must be set to 0.
     Bits 30:0 represent the size of the memory device in megabytes.
     EXAMPLE: 0000_8000h indicates a 32 GB memory device (32,768 MB),
              0002_0000h represents a 128 GB memory device (131,072 MB), and
              0000_7FFFh represents a 32,767 MB (32 GB – 1 MB) device.
  */
  Type17Table->ExtendedSize = (MemorySize >> 20) & (~BIT31);
  return EFI_SUCCESS;
}

/**
  Install SMBIOS memory device Table.

  Install the SMBIOS memory device (type 17) table for RD platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
InstallType17MemoryDevice (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  )
{
  EFI_STATUS Status;
  EFI_SMBIOS_HANDLE SmbiosHandle;
  UINT8  Idx;

  /* Get system memory information */
  for (Idx = 0; Idx < (FixedPcdGet32 (PcdChipCount) * 2); Idx += 2) {
    Status = UpdateMemorySize (
               &mArmRdSmbiosType17[Idx].Base,
               PcdGet64 (PcdSystemMemorySize) + SIZE_16MB
             );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "SMBIOS: Failed to update DRAM size for chip%d.\n",
        Idx / 2
      ));
    } else {
      mArmRdSmbiosType17[Idx].Base.MemoryTechnology = MemoryTechnologyDram;
      mArmRdSmbiosType17[Idx].Base.MemoryOperatingModeCapability.Bits.
        VolatileMemory = 1;
    }

    if (PcdGet64 (PcdDramBlock2Size) != 0) {
      Status = UpdateMemorySize (
                 &mArmRdSmbiosType17[Idx + 1].Base,
                 PcdGet64 (PcdDramBlock2Size)
               );
      if (EFI_ERROR (Status)) {
        DEBUG ((
          DEBUG_ERROR,
          "SMBIOS: Failed to update DRAM block 2 size for chip%d.\n",
          Idx / 2
        ));
      } else {
        mArmRdSmbiosType17[Idx + 1].Base.MemoryTechnology =
          MemoryTechnologyDram;
        mArmRdSmbiosType17[Idx + 1].Base.MemoryOperatingModeCapability.Bits.
          VolatileMemory = 1;
      }
    }
  }

  /* Install valid entries */
  for (Idx = 0; Idx < (FixedPcdGet32 (PcdChipCount) * 2); Idx++) {
    SmbiosHandle =
      ((EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType17[Idx])->Handle;
    Status = Smbios->Add (
                       Smbios,
                       NULL,
                       &SmbiosHandle,
                       (EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType17[Idx]
                       );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "SMBIOS: Failed to install Type17 SMBIOS table.\n"
        ));
      break;
    }
  }

  return Status;
}
