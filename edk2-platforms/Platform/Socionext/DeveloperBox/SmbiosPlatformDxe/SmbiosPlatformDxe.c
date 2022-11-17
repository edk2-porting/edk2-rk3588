/** @file

  This driver installs SMBIOS information for Socionext SynQuacer platforms

  Copyright (c) 2015, ARM Limited. All rights reserved.
  Copyright (c) 2018, Linaro, Ltd. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <IndustryStandard/SmBios.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Smbios.h>

STATIC EFI_SMBIOS_PROTOCOL       *mSmbios;

//
// Type definition and contents of the default SMBIOS table.
// This table covers only the minimum structures required by
// the SMBIOS specification (section 6.2, version 3.0)
//
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE0  Base;
  CHAR8               Strings[];
} ARM_TYPE0;

typedef struct {
  SMBIOS_TABLE_TYPE1  Base;
  CHAR8               Strings[];
} ARM_TYPE1;

typedef struct {
  SMBIOS_TABLE_TYPE2  Base;
  CHAR8               Strings[];
} ARM_TYPE2;

typedef struct {
  SMBIOS_TABLE_TYPE3  Base;
  CHAR8               Strings[];
} ARM_TYPE3;

typedef struct {
  SMBIOS_TABLE_TYPE4  Base;
  CHAR8               Strings[];
} ARM_TYPE4;

typedef struct {
  SMBIOS_TABLE_TYPE7  Base;
  CHAR8               Strings[];
} ARM_TYPE7;

typedef struct {
  SMBIOS_TABLE_TYPE9  Base;
  CHAR8               Strings[];
} ARM_TYPE9;

typedef struct {
  SMBIOS_TABLE_TYPE16 Base;
  CHAR8               Strings[];
} ARM_TYPE16;

typedef struct {
  SMBIOS_TABLE_TYPE17 Base;
  CHAR8               Strings[];
} ARM_TYPE17;

typedef struct {
  SMBIOS_TABLE_TYPE19 Base;
  CHAR8               Strings[];
} ARM_TYPE19;

typedef struct {
  SMBIOS_TABLE_TYPE32 Base;
  CHAR8               Strings[];
} ARM_TYPE32;
#pragma pack()

enum {
  SMBIOS_HANDLE_A53_L1I = 0x1000,
  SMBIOS_HANDLE_A53_L1D,
  SMBIOS_HANDLE_A53_L2,
  SMBIOS_HANDLE_A53_L3,
  SMBIOS_HANDLE_MOTHERBOARD,
  SMBIOS_HANDLE_CHASSIS,
  SMBIOS_HANDLE_A53_CLUSTER,
  SMBIOS_HANDLE_MEMORY,
};

// BIOS information (section 7.1)
STATIC CONST ARM_TYPE0 mArmDefaultType0 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_BIOS_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE0),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,                                            // Vendor
    2,                                            // BiosVersion
    0xE800,                                       // BiosSegment
    3,                                            // BiosReleaseDate
    (FixedPcdGet32 (PcdFdSize) - 1) / SIZE_64KB,  // BiosSize
    {
      0, 0, 0, 0, 0, 0,
      1,                                          // PCI supported
      0,
      1,                                          // PNP supported
      0,
      1,                                          // BIOS upgradable
      0, 0, 0,
      1,                                          // Boot from CD
      1,                                          // Selectable boot
    },
    { 0x3, 0xC, },                                // BIOSCharacteristicsExtensionBytes[2]

    FixedPcdGet32 (PcdFirmwareRevision) >> 16,    // SystemBiosMajorRelease
    FixedPcdGet32 (PcdFirmwareRevision) & 0xff,   // SystemBiosMinorRelease
    0xFF,                                         // EmbeddedControllerFirmwareMajorRelease
    0xFF                                          // EmbeddedControllerFirmwareMinorRelease
  }, {
    FIRMWARE_VENDOR " \0"
    "build #" BUILD_NUMBER "\0"
    __DATE__ "\0"
  }
};

// System information (section 7.2)
STATIC CONST ARM_TYPE1 mArmDefaultType1 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_INFORMATION,
      sizeof(SMBIOS_TABLE_TYPE1),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,     // Manufacturer
    2,     // Product Name
    0,     // Version
    0,     // Serial
    { 0xbf4ec78a, 0x431d, 0x4eb6, { 0xbb, 0xc9, 0x0c, 0x06, 0x19, 0x05, 0xca, 0x13 }},
    6,     // Wakeup type
    0,     // SKU
    0,     // Family
  }, {
    "Socionext\0"
    "SynQuacer E-series DeveloperBox\0"
  }
};

// Enclosure
STATIC CONST ARM_TYPE3 mArmDefaultType3 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE,
      sizeof (SMBIOS_TABLE_TYPE3),
      SMBIOS_HANDLE_CHASSIS,
    },
    1,   // Manufacturer
    4,   // Enclosure type (low profile desktop)
    2,   // Version
    0,   // Serial
    0,   // Asset tag
    ChassisStateUnknown,          // boot chassis state
    ChassisStateSafe,             // power supply state
    ChassisStateSafe,             // thermal state
    ChassisSecurityStatusNone,    // security state
    { 0, 0, 0, 0 },               // OEM defined
    1,                            // 1U height
    1,                            // number of power cords
    0,                            // no contained elements
  }, {
    "InWin\0"
    "BK623\0"
  }
};

STATIC CONST ARM_TYPE4 mArmDefaultType4 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE4),
      SMBIOS_HANDLE_A53_CLUSTER,
    },
    0,                                // socket type
    3,                                // processor type CPU
    ProcessorFamilyIndicatorFamily2,  // processor family, acquire from field2
    1,                                // manufacturer
    {},                               // processor id
    2,                                // version
    { 0, 0, 0, 0, 0, 1 },             // voltage
    0,                                // external clock
    1000,                             // max speed
    1000,                             // current speed
    0x41,                             // status
    ProcessorUpgradeNone,
    SMBIOS_HANDLE_A53_L1D,            // l1 cache handle
    SMBIOS_HANDLE_A53_L2,             // l2 cache handle
    SMBIOS_HANDLE_A53_L3,             // l3 cache handle
    0,                                // serial not set
    0,                                // asset not set
    3,                                // part number
    24,                               // core count in socket
    24,                               // enabled core count in socket
    24,                               // threads per socket
    0xEC,                             // processor characteristics
    ProcessorFamilyARM,               // ARM core
  }, {
    "ARM Ltd.\0"
    "Cortex-A53\0"
    "0xd03\0"
  }
};

STATIC CONST ARM_TYPE7 mArmDefaultType7_l1i = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_CACHE_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE7),
      SMBIOS_HANDLE_A53_L1I,
    },
    1,
    0x380,                      // L1 enabled
    32,                         // 32k i cache max
    32,                         // 32k installed
    { 0, 1 },                   // SRAM type
    { 0, 1 },                   // SRAM type
    0,                          // unknown speed
    CacheErrorParity,
    CacheTypeInstruction,
    CacheAssociativity2Way,
  }, {
    "L1 Instruction\0"
  }
};

STATIC CONST ARM_TYPE7 mArmDefaultType7_l1d = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_CACHE_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE7),
      SMBIOS_HANDLE_A53_L1D,
    },
    1,
    0x180,                      // L1 enabled, WB
    32,                         // 32k d cache max
    32,                         // 32k installed
    { 0, 1 },                   // SRAM type
    { 0, 1 },                   // SRAM type
    0,                          // unknown speed
    CacheErrorSingleBit,
    CacheTypeData,
    CacheAssociativity4Way,
  }, {
    "L1 Data\0"
  }
};

STATIC CONST ARM_TYPE7 mArmDefaultType7_l2 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_CACHE_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE7),
      SMBIOS_HANDLE_A53_L2,
    },
    1,
    0x181,                      // L2 enabled, WB
    256,                        // 256 KB cache max
    256,                        // 256 KB installed
    { 0, 1 },                   // SRAM type
    { 0, 1 },                   // SRAM type
    0,                          // unknown speed
    CacheErrorSingleBit,
    CacheTypeUnified,
    CacheAssociativity16Way,
  }, {
    "L2\0"
  }
};

STATIC CONST ARM_TYPE7 mArmDefaultType7_l3 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_CACHE_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE7),
      SMBIOS_HANDLE_A53_L3,
    },
    1,
    0x182,                      // L3 enabled, WB
    4096,                       // 4M cache max
    4096,                       // 4M installed
    { 0, 1 },                   // SRAM type
    { 0, 1 },                   // SRAM type
    0,                          // unknown speed
    CacheErrorSingleBit,
    CacheTypeUnified,
    CacheAssociativity16Way,
  }, {
    "L3\0"
  }
};

// Slots
STATIC CONST ARM_TYPE9 mArmDefaultType9_0 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS,
      sizeof (SMBIOS_TABLE_TYPE9),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,
    SlotTypePciExpressGen2X16,
    SlotDataBusWidth4X,
    SlotUsageUnknown,
    SlotLengthLong,
    0,
    { 1 },
    {},
    1,
    0,
    0,
  }, {
    "J-PCIEX16\0"
  }
};

STATIC CONST ARM_TYPE9 mArmDefaultType9_1 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS,
      sizeof (SMBIOS_TABLE_TYPE9),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,
    SlotTypePciExpressGen2X1,
    SlotDataBusWidth1X,
    SlotUsageUnknown,
    SlotLengthShort,
    0,
    { 1 },
    {},
    0x0,
    0x3,
    0x0,
  }, {
    "J-PCIE1\0"
  }
};

STATIC CONST ARM_TYPE9 mArmDefaultType9_2 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS,
      sizeof (SMBIOS_TABLE_TYPE9),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,
    SlotTypePciExpressGen2X1,
    SlotDataBusWidth1X,
    SlotUsageUnknown,
    SlotLengthShort,
    0,
    { 1 },
    {},
    0x0,
    0x5,
    0x0,
  }, {
    "J-PCIE2\0"
  }
};

// Memory array
STATIC CONST ARM_TYPE16 mArmDefaultType16 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY,
      sizeof (SMBIOS_TABLE_TYPE16),
      SMBIOS_HANDLE_MEMORY,
    },
    MemoryArrayLocationSystemBoard,     // on motherboard
    MemoryArrayUseSystemMemory,         // system RAM
    MemoryErrorCorrectionNone,
    0x4000000,                          // max 64 GB
    0xFFFE,                             // No error information structure
    4,                                  // 4 DIMMs
  }, {
    "\0"
  }
};

// Memory device
STATIC CONST ARM_TYPE17 mArmDefaultType17_1 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_MEMORY_DEVICE,
      sizeof (SMBIOS_TABLE_TYPE17),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    SMBIOS_HANDLE_MEMORY,           // array to which this module belongs
    0xFFFE,                         // no errors
    72,                             // single DIMM with ECC
    64,                             // data width of this device (64-bits)
    0xFFFF,                         // size unknown
    0x09,                           // DIMM
    1,                              // part of a set
    1,                              // device locator
    0,                              // bank locator
    MemoryTypeDdr4,                 // DDR4
    {},                             // type detail
    0,                              // ? MHz
    0,                              // manufacturer
    0,                              // serial
    0,                              // asset tag
    0,                              // part number
    0,                              // rank
  }, {
    "DIMM1\0"
  }
};

STATIC CONST ARM_TYPE17 mArmDefaultType17_2 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_MEMORY_DEVICE,
      sizeof (SMBIOS_TABLE_TYPE17),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    SMBIOS_HANDLE_MEMORY,           // array to which this module belongs
    0xFFFE,                         // no errors
    72,                             // single DIMM with ECC
    64,                             // data width of this device (64-bits)
    0xFFFF,                         // size unknown
    0x09,                           // DIMM
    1,                              // part of a set
    1,                              // device locator
    0,                              // bank locator
    MemoryTypeDdr4,                 // DDR4
    {},                             // type detail
    0,                              // ? MHz
    0,                              // manufacturer
    0,                              // serial
    0,                              // asset tag
    0,                              // part number
    0,                              // rank
  }, {
    "DIMM2\0"
  }
};

STATIC CONST ARM_TYPE17 mArmDefaultType17_3 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_MEMORY_DEVICE,
      sizeof (SMBIOS_TABLE_TYPE17),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    SMBIOS_HANDLE_MEMORY,           // array to which this module belongs
    0xFFFE,                         // no errors
    72,                             // single DIMM with ECC
    64,                             // data width of this device (64-bits)
    0xFFFF,                         // size unknown
    0x09,                           // DIMM
    1,                              // part of a set
    1,                              // device locator
    0,                              // bank locator
    MemoryTypeDdr4,                 // DDR4
    {},                             // type detail
    0,                              // ? MHz
    0,                              // manufacturer
    0,                              // serial
    0,                              // asset tag
    0,                              // part number
    0,                              // rank
  }, {
    "DIMM3\0"
  }
};

STATIC CONST ARM_TYPE17 mArmDefaultType17_4 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_MEMORY_DEVICE,
      sizeof (SMBIOS_TABLE_TYPE17),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    SMBIOS_HANDLE_MEMORY,           // array to which this module belongs
    0xFFFE,                         // no errors
    72,                             // single DIMM with ECC
    64,                             // data width of this device (64-bits)
    0xFFFF,                         // size unknown
    0x09,                           // DIMM
    1,                              // part of a set
    1,                              // device locator
    0,                              // bank locator
    MemoryTypeDdr4,                 // DDR4
    {},                             // type detail
    0,                              // ? MHz
    0,                              // manufacturer
    0,                              // serial
    0,                              // asset tag
    0,                              // part number
    0,                              // rank
  }, {
    "DIMM4\0"
  }
};

// Memory array mapped address, this structure
// is overridden by InstallMemoryStructure
STATIC CONST ARM_TYPE19 mArmDefaultType19 = {
  {
    {  // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS,
      sizeof (SMBIOS_TABLE_TYPE19),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    0xFFFFFFFF,             // invalid, look at extended addr field
    0xFFFFFFFF,
    SMBIOS_HANDLE_MEMORY,   // handle
    1,
    0x0,
    0x0,
  }, {
    "\0"
  }
};

// System boot info
STATIC CONST ARM_TYPE32 mArmDefaultType32 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE32),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    {},
    BootInformationStatusNoError,
  }, {
    "\0"
  }
};

STATIC SMBIOS_STRUCTURE * CONST FixedTables[] = {
  (SMBIOS_STRUCTURE *)&mArmDefaultType0.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType1.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType3.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType7_l1i.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType7_l1d.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType7_l2.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType7_l3.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType4.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType9_0.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType9_1.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType9_2.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType16.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType17_1.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType17_2.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType17_3.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType17_4.Base.Hdr,
  (SMBIOS_STRUCTURE *)&mArmDefaultType32.Base.Hdr,
};

STATIC
EFI_STATUS
InstallMemoryStructure (
  IN UINT64                    StartingAddress,
  IN UINT64                    RegionLength
  )
{
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  CHAR8                     Buffer[sizeof (mArmDefaultType19)];
  ARM_TYPE19                *Descriptor;

  CopyMem (Buffer, &mArmDefaultType19, sizeof (Buffer));

  Descriptor = (ARM_TYPE19 *)Buffer;
  Descriptor->Base.ExtendedStartingAddress = StartingAddress;
  Descriptor->Base.ExtendedEndingAddress = StartingAddress + RegionLength;
  SmbiosHandle = Descriptor->Base.Hdr.Handle;

  return mSmbios->Add (mSmbios, NULL, &SmbiosHandle, &Descriptor->Base.Hdr);
}

STATIC
VOID
InstallAllStructures (
   VOID
   )
{
  EFI_STATUS                Status;
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  UINTN                     Idx;
  EFI_PEI_HOB_POINTERS      Hob;

  for (Idx = 0; Idx < ARRAY_SIZE (FixedTables); Idx++) {
    SmbiosHandle = FixedTables[Idx]->Handle;
    Status = mSmbios->Add (mSmbios, NULL, &SmbiosHandle, FixedTables[Idx]);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_WARN, "%a: failed to add SMBIOS type %u table - %r\n",
        __FUNCTION__, FixedTables[Idx]->Type, Status));
      break;
    }
  }

  for (Hob.Raw = GetHobList ();
       !END_OF_HOB_LIST (Hob);
       Hob.Raw = GET_NEXT_HOB (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR &&
        Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) {
      Status = InstallMemoryStructure (Hob.ResourceDescriptor->PhysicalStart,
                                       Hob.ResourceDescriptor->ResourceLength);
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_WARN, "%a: failed to add SMBIOS type 19 table - %r\n",
          __FUNCTION__, Status));
        break;
      }
    }
  }
}

/**
   Installs SMBIOS information for SynQuacer platform

   @param ImageHandle     Module's image handle
   @param SystemTable     Pointer of EFI_SYSTEM_TABLE

   @retval EFI_SUCCESS    Smbios data successfully installed
   @retval Other          Smbios data was not installed

**/
EFI_STATUS
EFIAPI
SmbiosPlatformDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL,
                  (VOID **)&mSmbios);
  ASSERT_EFI_ERROR (Status);

  InstallAllStructures ();

  return EFI_SUCCESS;
}
