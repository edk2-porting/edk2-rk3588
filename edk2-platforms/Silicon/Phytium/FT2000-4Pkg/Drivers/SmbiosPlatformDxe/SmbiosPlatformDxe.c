/** @file
  This driver installs SMBIOS information for Phytium Durian platforms.

  Copyright (C) 2020, Phytium Technology Co Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#include <IndustryStandard/SmBios.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Platform.h>
#include <Protocol/Smbios.h>

// SMBIOS tables often reference each other using
// fixed constants, define a list of these constants
// for our hardcoded tables

#define TYPE0_STRINGS                                         \
  "PHYTIUM LTD\0"                         /* Vendor */        \
  "V1.0\0"                                /* BiosVersion */   \
  __DATE__"\0"                            /* BiosReleaseDate */

#define TYPE1_STRINGS                                        \
  "PHYTIUM LTD\0"                         /* Manufacturer */ \
  "Phytium Durian Development Platform\0" /* Product Name */ \
  "None\0"                                /* Version */      \
  "Not Set\0"                             /* SerialNumber */ \
  "Not set\0"                             /* SKUNumber */    \
  "FT-2000/4\0"                           /* Family */       \

#define TYPE2_STRINGS                                        \
  "PHYTIUM LTD\0"                         /* Manufacturer */ \
  "Phytium Durian Development Platform\0" /* Product Name */ \
  "None\0"                                /* Version */        \
  "Not Set\0"                             /* Serial */         \
  "Not Set\0"                             /* BaseBoardAssetTag */ \
  "Not Set\0"                             /* BaseBoardChassisLocation */

#define TYPE3_STRINGS                                       \
  "PHYTIUM LTD\0"                        /* Manufacturer */ \
  "None\0"                               /* Version */      \
  "Not Set\0"                            /* Serial  */      \
  "Not Set\0"                            /* AssetTag  */

#define TYPE4_STRINGS                                                   \
  "FT-2000/4\0"                          /* socket type */              \
  "PHYTIUM LTD\0"                        /* manufactuer */              \
  "FT-2000/4\0"                          /* processor version */        \
  "Not Set\0"                            /* SerialNumber */             \
  "Not Set\0"                            /* processor 2 description */  \
  "Not Set\0"                            /* AssetTag */


#define TYPE7_STRINGS                              \
  "L1 Instruction\0"                 /* L1I  */    \
  "L1 Data\0"                        /* L1D  */    \
  "L2\0"                             /* L2   */

#define TYPE7_L1DATA_STRINGS          \
  "L1 Data Cache\0"                  /* L1 data   */


#define TYPE7_L1INS_STRINGS           \
  "L1 Instruction Cache\0"          /* L1 ins   */

#define TYPE7_L2_STRINGS              \
  "L2 Cache\0"                      /* L2   */

#define TYPE7_L3_STRINGS              \
  "L3 Cache\0"                     /* L3   */


#define TYPE9_STRINGS                                     \
  "PCIE_SLOT0\0"                     /* Slot0 */          \
  "PCIE_SLOT1\0"                     /* Slot1 */          \
  "PCIE_SLOT2\0"                     /* Slot2 */          \
  "PCIE_SLOT3\0"                     /* Slot3 */

#define TYPE9_STRINGS_PCIE0X16                            \
  "PCIE0_X16\0"

#define TYPE9_STRINGS_PCIE0X1                             \
  "PCIE0_X1\0"

#define TYPE9_STRINGS_PCIE1X16                            \
  "PCIE1_X16\0"

#define TYPE9_STRINGS_PCIE1X1                             \
  "PCIE1_X1\0"

#define TYPE13_STRINGS                                    \
  "en|US|iso8859-1\0"                                     \
  "zh|CN|unicode\0"


#define TYPE16_STRINGS                                     \
  "\0"                               /* nothing */

#define TYPE17_STRINGS_CHANNEL0                            \
  "SOCKET 0 CHANNEL 0 DIMM 0\0"     /* location */         \
  "Bank0\0"                         /* bank description */ \
  "Not Set\0"              \
  "Not Set\0"              \
  "Not Set\0"              \
  "Not Set\0"

#define TYPE17_STRINGS_CHANNEL1                            \
  "SOCKET 0 CHANNEL 1 DIMM 0\0"    /* location */          \
  "Bank0\0"                \
  "Not Set\0"              \
  "Not Set\0"              \
  "Not Set\0"              \
  "Not Set\0"


#define TYPE19_STRINGS                                \
  "\0"                              /* nothing */

#define TYPE32_STRINGS                                \
  "\0"                              /* nothing */

#define TYPE39_STRINGS                          \
  "Not specified\0"       /* not specified*/    \
  "Not specified\0"       /* not specified*/    \
  "Not specified\0"       /* not specified*/    \
  "Not specified\0"       /* not specified*/    \
  "Not specified\0"       /* not specified*/    \
  "Not specified\0"       /* not specified*/    \
  "Not specified\0"       /* not specified*/

#define TYPE38_STRINGS                          \
  "\0"

//
// Type definition and contents of the default SMBIOS table.
// This table covers only the minimum structures required by
// the SMBIOS specification (section 6.2, version 3.0)
//
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE0 Base;
  INT8              Strings[sizeof (TYPE0_STRINGS)];
} ARM_TYPE0;

typedef struct {
  SMBIOS_TABLE_TYPE1 Base;
  UINT8              Strings[sizeof (TYPE1_STRINGS)];
} ARM_TYPE1;

typedef struct {
  SMBIOS_TABLE_TYPE2 Base;
  UINT8              Strings[sizeof (TYPE2_STRINGS)];
} ARM_TYPE2;

typedef struct {
  SMBIOS_TABLE_TYPE3 Base;
  UINT8              Strings[sizeof (TYPE3_STRINGS)];
} ARM_TYPE3;

typedef struct {
  SMBIOS_TABLE_TYPE4 Base;
  UINT8              Strings[sizeof (TYPE4_STRINGS)];
} ARM_TYPE4;

typedef struct {
  SMBIOS_TABLE_TYPE7 Base;
  UINT8              Strings[sizeof (TYPE7_L1DATA_STRINGS)];
} ARM_TYPE7_L1DATA;

typedef struct {
  SMBIOS_TABLE_TYPE7 Base;
  UINT8              Strings[sizeof (TYPE7_L1INS_STRINGS)];
} ARM_TYPE7_L1INS;

typedef struct {
  SMBIOS_TABLE_TYPE7 Base;
  UINT8              Strings[sizeof (TYPE7_L2_STRINGS)];
} ARM_TYPE7_L2;

typedef struct {
  SMBIOS_TABLE_TYPE7 Base;
  UINT8              Strings[sizeof (TYPE7_L3_STRINGS)];
} ARM_TYPE7_L3;


typedef struct {
  SMBIOS_TABLE_TYPE9 Base;
  UINT8              Strings[sizeof (TYPE9_STRINGS)];
} ARM_TYPE9;

typedef struct {
  SMBIOS_TABLE_TYPE9 Base;
  UINT8              Strings[sizeof (TYPE9_STRINGS_PCIE0X16)];
} ARM_TYPE9_PCIE0X16;

typedef struct {
  SMBIOS_TABLE_TYPE9 Base;
  UINT8              Strings[sizeof (TYPE9_STRINGS_PCIE0X1)];
} ARM_TYPE9_PCIE0X1;

typedef struct {
  SMBIOS_TABLE_TYPE9 Base;
  UINT8              Strings[sizeof (TYPE9_STRINGS_PCIE1X16)];
} ARM_TYPE9_PCIE1X16;

typedef struct {
  SMBIOS_TABLE_TYPE9 Base;
  UINT8              Strings[sizeof (TYPE9_STRINGS_PCIE1X1)];
} ARM_TYPE9_PCIE1X1;


typedef struct {
  SMBIOS_TABLE_TYPE13 Base;
  UINT8              Strings[sizeof (TYPE13_STRINGS)];
} ARM_TYPE13;

typedef struct {
  SMBIOS_TABLE_TYPE16 Base;
  UINT8              Strings[sizeof (TYPE16_STRINGS)];
} ARM_TYPE16;

typedef struct {
  SMBIOS_TABLE_TYPE17 Base;
  UINT8               Strings[sizeof (TYPE17_STRINGS_CHANNEL0)];
} ARM_TYPE17_CHANNEL0;

typedef struct {
  SMBIOS_TABLE_TYPE17 Base;
  UINT8               Strings[sizeof (TYPE17_STRINGS_CHANNEL1)];
} ARM_TYPE17_CHANNEL1;

typedef struct {
  SMBIOS_TABLE_TYPE19 Base;
  UINT8              Strings[sizeof (TYPE19_STRINGS)];
} ARM_TYPE19;

typedef struct {
  SMBIOS_TABLE_TYPE32 Base;
  UINT8              Strings[sizeof (TYPE32_STRINGS)];
} ARM_TYPE32;

typedef struct {
  SMBIOS_TABLE_TYPE38 Base;
  UINT8              Strings[sizeof (TYPE38_STRINGS)];
} ARM_TYPE38;

typedef struct {
  SMBIOS_TABLE_TYPE39 Base;
  UINT8              Strings[sizeof (TYPE39_STRINGS)];
} ARM_TYPE39;

enum SMBIOS_REFRENCE_HANDLES {
  SMBIOS_HANDLE_L1I = 0x1000,
  SMBIOS_HANDLE_L1D,
  SMBIOS_HANDLE_L2,
  SMBIOS_HANDLE_L3,
  SMBIOS_HANDLE_MOTHERBOARD,
  SMBIOS_HANDLE_CHASSIS,
  SMBIOS_HANDLE_CLUSTER,
  SMBIOS_HANDLE_MEMORY,
  SMBIOS_HANDLE_DIMM_0,
  SMBIOS_HANDLE_DIMM_1
};

#define SERIAL_LEN 10  //this must be less than the buffer len allocated in the type1 structure

#pragma pack()

//BIOS Information (Type 0)
ARM_TYPE0  BiosInfo_Type0 = {
    {
      { // SMBIOS_STRUCTURE Hdr
        EFI_SMBIOS_TYPE_BIOS_INFORMATION,        // UINT8 Type
        sizeof (SMBIOS_TABLE_TYPE0),             // UINT8 Length
        SMBIOS_HANDLE_PI_RESERVED
      },
      1,                                           //Vendor
      2,                                           //BiosVersion
      0x8800,                                      //BiosSegment
      3,                                           //BiosReleaseDate
      0xFF,                                        //BiosSize
      {                                            //BiosCharacteristics
        0,                                         // Reserved                          :2
        0,                                         // Unknown                           :1
        0,                                         // BiosCharacteristicsNotSupported   :1
        0,                                         // IsaIsSupported                    :1
        0,                                         // McaIsSupported                    :1
        0,                                         // EisaIsSupported                   :1
        1,                                         // PciIsSupported                    :1
        0,                                         // PcmciaIsSupported                 :1
        0,                                         // PlugAndPlayIsSupported            :1
        0,                                         // ApmIsSupported                    :1
        1,                                         // BiosIsUpgradable                  :1
        0,                                         // BiosShadowingAllowed              :1
        0,                                         // VlVesaIsSupported                 :1
        0,                                         // EscdSupportIsAvailable            :1
        1,                                         // BootFromCdIsSupported             :1
        1,                                         // SelectableBootIsSupported         :1
        0,                                         // RomBiosIsSocketed                 :1
        0,                                         // BootFromPcmciaIsSupported         :1
        0,                                         // EDDSpecificationIsSupported       :1
        0,                                         // JapaneseNecFloppyIsSupported      :1
        0,                                         // JapaneseToshibaFloppyIsSupported  :1
        0,                                         // Floppy525_360IsSupported          :1
        0,                                         // Floppy525_12IsSupported           :1
        0,                                         // Floppy35_720IsSupported           :1
        0,                                         // Floppy35_288IsSupported           :1
        0,                                         // PrintScreenIsSupported            :1
        0,                                         // Keyboard8042IsSupported           :1
        0,                                         // SerialIsSupported                 :1
        0,                                         // PrinterIsSupported                :1
        0,                                         // CgaMonoIsSupported                :1
        0,                                         // NecPc98                           :1
        0                                          // ReservedForVendor                 :3
      },
      {
        0x03,                                      //BIOSCharacteristicsExtensionBytes[0]
        0x0D                                       //BIOSCharacteristicsExtensionBytes[1]
      },
      0xFF,                                        //SystemBiosMajorRelease;
      0xFF,                                        //SystemBiosMinorRelease;
      0xFF,                                        //EmbeddedControllerFirmwareMajorRelease;
      0xFF,                                        //EmbeddedControllerFirmwareMinorRelease;
  },
  TYPE0_STRINGS
};

//System Information (Type 1).
ARM_TYPE1 SystemInfo_Type1 = {
    {
      {                                               // Hdr
        EFI_SMBIOS_TYPE_SYSTEM_INFORMATION,           // Type,
        sizeof (SMBIOS_TABLE_TYPE1),                  // UINT8 Length
        SMBIOS_HANDLE_PI_RESERVED                     // Handle
      },
      1,                                              // Manufacturer
      2,                                              // ProductName
      3,                                              // Version
      4,                                              // SerialNumber
      {                                               // Uuid
          0x12345678, 0x1234, 0x5678, {0x90, 0xab, 0xcd, 0xde, 0xef, 0xaa, 0xbb, 0xcc}
      },
      SystemWakeupTypePowerSwitch,                    // SystemWakeupType
      5,                                              // SKUNumber,
      6                                               // Family
  },
  TYPE1_STRINGS
};

//Base Board (or Module) Information (Type 2)
ARM_TYPE2 BaseboardInfo_Type2 = {
    {
      {                                                       // Hdr
        EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION,                // Type,
        sizeof (SMBIOS_TABLE_TYPE2),                          // UINT8 Length
        SMBIOS_HANDLE_MOTHERBOARD                             // Handle
      },
      1,                                                      // BaseBoardManufacturer
      2,                                                      // BaseBoardProductName
      3,                                                      // BaseBoardVersion
      4,                                                      // BaseBoardSerialNumber
      5,                                                      // BaseBoardAssetTag
      {                                                       // FeatureFlag
        1,                                                    // Motherboard           :1
        0,                                                    // RequiresDaughterCard  :1
        0,                                                    // Removable             :1
        1,                                                    // Replaceable           :1
        0,                                                    // HotSwappable          :1
        0                                                     // Reserved              :3
      },
      6,                                                      // BaseBoardChassisLocation
      0,                                                      // ChassisHandle;
      BaseBoardTypeMotherBoard,                               // BoardType;
      0,                                                      // NumberOfContainedObjectHandles;
      {
        0
      }                                                       // ContainedObjectHandles[1];
  },
  TYPE2_STRINGS
};

//System Enclosure or Chassis (Type 3)
ARM_TYPE3 SystemEnclosure_Type3 = {
    {
      {                                                       // Hdr
        EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE  ,                   // Type,
        sizeof (SMBIOS_TABLE_TYPE3),                          // UINT8 Length
        SMBIOS_HANDLE_CHASSIS                                 // Handle
      },
      1,                                                      // Manufactrurer
      MiscChassisTypeMainServerChassis,                       // Type
      2,                                                      // Version
      3,                                                      // SerialNumber
      4,                                                      // AssetTag
      ChassisStateSafe,                                       // BootupState
      ChassisStateSafe,                                       // PowerSupplyState
      ChassisStateSafe,                                       // ThermalState
      ChassisSecurityStatusNone,                              // SecurityState
      {
        0,                                                    // OemDefined[0]
        0,                                                    // OemDefined[1]
        0,                                                    // OemDefined[2]
        0                                                     // OemDefined[3]
      },
      2,                                                      // Height
      1,                                                      // NumberofPowerCords
      0,                                                      // ContainedElementCount
      0,                                                      // ContainedElementRecordLength
      {                                                       // ContainedElements[0]
        {
            0,                                                // ContainedElementType
            0,                                                // ContainedElementMinimum
            0                                                 // ContainedElementMaximum
        }
      }
  },
  TYPE3_STRINGS
};

//Processor Infomation (Type 4)
ARM_TYPE4 ProcessorInfo_Type4 = {
  {
    {                                           //Header
      EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION,    //Type
      sizeof (SMBIOS_TABLE_TYPE4),              //Length
      SMBIOS_HANDLE_CLUSTER                     //Handle
    },
    1,                                          //Socket
    CentralProcessor,                           //ProcessorType
    ProcessorFamilyIndicatorFamily2,            //ProcessorFamily
    2,                                          //ProcessorManufacture
    {                                           //ProcessorId
      {                                         //Signature
        0
      },
      {                                         //FeatureFlags
        0
      }
    },
    3,                                          //ProcessorVersion
    {                                           //Voltage
      0, 0, 0, 1, 0, 1
    },
    1,                                          //ExternalClock
    1,                                          //MaxSpeed
    0,                                          //CurrentSpeed
    0x41,                                       //Status
    ProcessorUpgradeUnknown,                    //ProcessorUpgrade
    SMBIOS_HANDLE_L1D,                          //L1Ins
    SMBIOS_HANDLE_L2,                           //L1Data
    SMBIOS_HANDLE_L3,                           //L2
    4,                                          //SerialNumber
    5,                                          //AssetTag
    6,                                          //PartNumber

    4,                                          //CoreCount
    0,                                          //EnabledCoreCount
    0,                                          //ThreadCount
    0x00EC,                                     //ProcessorCharacteristics

    ProcessorFamilyARMv8,                       //ProcessorFamily2

    0,                                          //CoreCount2
    0,                                          //EnabledCoreCount2
    0                                           //ThreadCount2
  },
  TYPE4_STRINGS
};

//Cache Information (Type7) L1 DATA
ARM_TYPE7_L1DATA L1Data_Type7 = {
  {
    {                                               //Header
      EFI_SMBIOS_TYPE_CACHE_INFORMATION,            //Type
      sizeof (SMBIOS_TABLE_TYPE7),                  //Length
      SMBIOS_HANDLE_L1D                             //Handle
    },
    1,                                              //SocketDesignation
    0x0180,                                         //CacheConfiguration
    0,                                              //MaximumCacheSize
    0,                                              //InstalledSize
    {                                               //SupportedSRAMType
      0, 0, 0, 0, 0, 1, 0, 0
    },
    {                                               //CurrentSRAMType
        0, 0, 0, 0, 0, 1, 0, 0
    },
    0,                                              //CacheSpeed
    CacheErrorSingleBit,                            //ErrorCorrectionType
    CacheTypeData,                                  //SystemCacheType
    CacheAssociativity8Way,                         //Associativity
    128,
    128
  },
  TYPE7_L1DATA_STRINGS
};

//Cache Information (Type7) L1 INS
ARM_TYPE7_L1INS L1Ins_Type7 = {
  {
    {                                               //Header
      EFI_SMBIOS_TYPE_CACHE_INFORMATION,            //Type
      sizeof (SMBIOS_TABLE_TYPE7),                  //Length
      SMBIOS_HANDLE_L1I                             //Handle
    },
    1,                                              //SocketDesignation
    0x0180,                                         //CacheConfiguration
    0,                                              //MaximumCacheSize
    0,                                              //InstalledSize
    {                                               //SupportedSRAMType
      0, 0, 0, 0, 0, 1, 0, 0
    },
    {                                               //CurrentSRAMType
       0, 0, 0, 0, 0, 1, 0, 0
    },
    0,                                              //CacheSpeed
    CacheErrorParity,                               //ErrorCorrectionType
    CacheTypeInstruction,                           //SystemCacheType
    CacheAssociativity8Way,                         //Associativity
    128,
    128
  },
  TYPE7_L1INS_STRINGS
};

//Cache Information (Type7) L2
ARM_TYPE7_L2 L2_Type7 = {
  {
    {                                               //Header
        EFI_SMBIOS_TYPE_CACHE_INFORMATION,          //Type
        sizeof (SMBIOS_TABLE_TYPE7),                //Length
        SMBIOS_HANDLE_L2                            //Handle
    },
    1,                                              //SocketDesignation
    0x0281,                                         //CacheConfiguration
    0,                                              //MaximumCacheSize
    0,                                              //InstalledSize
    {                                               //SupportedSRAMType
       0, 0, 0, 0, 0, 1, 0, 0
    },
    {                                               //CurrentSRAMType
       0, 0, 0, 0, 0, 1, 0, 0
    },
    0,                                              //CacheSpeed
    CacheErrorSingleBit,                            //ErrorCorrectionType
    CacheTypeUnified,                               //SystemCacheType
    CacheAssociativity8Way,                         //Associativity
    4096,
    4096
  },
  TYPE7_L2_STRINGS
};

//Cache Information (Type7) L3
ARM_TYPE7_L3 L3_Type7 = {
  {
    {                                               //Header
      EFI_SMBIOS_TYPE_CACHE_INFORMATION,            //Type
      sizeof (SMBIOS_TABLE_TYPE7),                  //Length
      SMBIOS_HANDLE_L3                              //Handle
    },
    1,                                              //SocketDesignation
    0x0281,                                         //CacheConfiguration
    0,                                              //MaximumCacheSize
    0,                                              //InstalledSize
    {                                               //SupportedSRAMType
       0, 0, 0, 0, 0, 1, 0, 0
    },
    {                                               //CurrentSRAMType
       0, 0, 0, 0, 0, 1, 0, 0
    },
    0,                                              //CacheSpeed
    CacheErrorSingleBit,                            //ErrorCorrectionType
    CacheTypeUnified,                               //SystemCacheType
    CacheAssociativity8Way,                         //Associativity
    4096,
    4096
  },
  TYPE7_L3_STRINGS
};

//PCIE0_X16   (Type 9)
ARM_TYPE9_PCIE0X16 Pcie0X16_Type9 = {
  {
    {  // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS,        // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),         // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED
    },
    1,
    SlotTypePciX,
    SlotDataBusWidth16X,
    SlotUsageInUse,
    SlotLengthLong,
    0,
    {0, 0, 1, 1, 0, 0, 0, 0}, //unknown
    {1, 0, 0, 0, 0},  //PME and SMBUS
    0,
    0,
    0,
  },
  TYPE9_STRINGS_PCIE0X16
};

//PCIE0_X1    (Type 9)
ARM_TYPE9_PCIE0X1 Pcie0X1_Type9 = {
  {
    {  // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS,        // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),         // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED
    },
    1,
    SlotTypePciX,
    SlotDataBusWidth1X,
    SlotUsageAvailable,
    SlotLengthShort,
    1,
    {0, 0, 1, 1, 0, 0, 0, 0}, //unknown
    {1, 0, 0, 0, 0},  //PME and SMBUS
    0xFF,
    0xFF,
    0xFF,
  },
  TYPE9_STRINGS_PCIE0X1
};

//PCIE1_X16   (Type 9)
ARM_TYPE9_PCIE1X16 Pcie1X16_Type9 = {
  {
    {  // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS,        // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),         // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED
    },
    1,
    SlotTypePciX,
    SlotDataBusWidth16X,
    SlotUsageAvailable,
    SlotLengthLong,
    2,
    {0, 0, 1, 1, 0, 0, 0, 0}, //unknown
    {1, 0, 0, 0, 0},       //PME and SMBUS
    0xFF,
    0xFF,
    0xFF,
  },
  TYPE9_STRINGS_PCIE1X16
};

//PCIE1_X1  (Type 9)
ARM_TYPE9_PCIE1X1 Pcie1X1_Type9 = {
  {
    {  // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED
    },
    1,
    SlotTypePciX,
    SlotDataBusWidth1X,
    SlotUsageAvailable,
    SlotLengthShort,
    3,
    {0, 0, 1, 1, 0, 0, 0, 0}, //unknown
    {1, 0, 0, 0, 0},       //PME and SMBUS
    0xFF,
    0xFF,
    0xFF,
  },
  TYPE9_STRINGS_PCIE1X1
};

//Bios Language Information (Type13)
ARM_TYPE13 BiosLangInfo_Type13 = {
  {
    { // SMBIOS_STRUCTURE Hdr
    EFI_SMBIOS_TYPE_BIOS_LANGUAGE_INFORMATION, // UINT8 Type
    sizeof (SMBIOS_TABLE_TYPE13),              // UINT8 Length
    SMBIOS_HANDLE_PI_RESERVED
    },
    2,
    0,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    2
  },
  TYPE13_STRINGS
};

//Physical Memory Array (Type 16)
ARM_TYPE16 MemArray_Type16 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE16),          // UINT8 Length
      SMBIOS_HANDLE_MEMORY
    },
    MemoryArrayLocationSystemBoard,
    MemoryArrayUseSystemMemory,
    MemoryErrorCorrectionNone,
    0x1000000,                                 //16G
    0xFFFE,
    2
  },
  TYPE16_STRINGS
};

//Memory Device (Type17)
ARM_TYPE17_CHANNEL0 MemDev_Type17_0 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_MEMORY_DEVICE, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE17),  // UINT8 Length
      SMBIOS_HANDLE_DIMM_0
    },
    SMBIOS_HANDLE_MEMORY, //array to which this module belongs
    0xFFFE,               //no errors
    64,                   //single DIMM, no ECC is 64bits (for ecc this would be 72)
    64,                   //data width of this device (64-bits)
    0x4000,               //16GB
    0x09,                 //FormFactor
    0,                    //not part of a set
    1,                    //right side of board
    2,                    //bank 0
    MemoryTypeDdr4,                    //LP DDR4
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //unbuffered
    2400,                              //2400Mhz DDR
    3, //Manufacturer
    4, //serial
    5, //asset tag
    6, //part number
    0, //attrbute
    0x2000,     //  8G
    2400,       //2400MHz
    1500,       //Max V
    1500,       //Max V
    1500,       //Configure V
  },
  TYPE17_STRINGS_CHANNEL0
};

//Memory Device (Type17)
ARM_TYPE17_CHANNEL1 MemDev_Type17_1 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_MEMORY_DEVICE, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE17),  // UINT8 Length
      SMBIOS_HANDLE_DIMM_1
    },
    SMBIOS_HANDLE_MEMORY, //array to which this module belongs
    0xFFFE,               //no errors
    64,                   //single DIMM, no ECC is 64bits (for ecc this would be 72)
    64,                   //data width of this device (64-bits)
    0x2000,               //8GB
    0x09,                 //FormFactor
    0,                    //not part of a set
    1,                    //right side of board
    2,                    //bank 0
    MemoryTypeDdr4,                    //LP DDR4
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //unbuffered
    2400,                              //2400Mhz DDR
    3, //varies between diffrent production runs
    4, //serial
    5, //asset tag
    6, //part number
    0, //attrbute
    0x4000,     //  16G
    2400,       //2400MHz
    1500,       //Max V
    1500,       //Max V
    1500,       //Configure V
  },
  TYPE17_STRINGS_CHANNEL1
};

//Memory Array Mapped Address (Type 19)
ARM_TYPE19 MemArrayMapAddr_Type19 = {
  {
    {  // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE19),                // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED
    },
    0,
    0x1000000,  //16G
    SMBIOS_HANDLE_MEMORY, //handle
    2,
    0,      //starting addr of first 2GB
    0,      //ending addr of first 2GB
  },
  TYPE19_STRINGS
};

//System Boot Information (Type 32)
ARM_TYPE32 SystemBoot_Type32 = {
  {
    {
      EFI_SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION,        // Type,
      sizeof (SMBIOS_TABLE_TYPE32),                   // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED
    },
    {                                                 // Reserved[6]
      0,
      0,
      0,
      0,
      0,
      0
    },
    BootInformationStatusNoError                     // BootInformationStatus
  },
  TYPE32_STRINGS
};

VOID *DefaultCommonTables[]=
{
  &BiosInfo_Type0,
  &SystemInfo_Type1,
  &BaseboardInfo_Type2,
  &SystemEnclosure_Type3,
  &ProcessorInfo_Type4,
  &L1Data_Type7,
  &L1Ins_Type7,
  &L2_Type7,
  &L3_Type7,
  &Pcie0X16_Type9,
  &Pcie0X1_Type9,
  &Pcie1X16_Type9,
  &Pcie1X1_Type9,
  &MemArray_Type16,
  &MemDev_Type17_0,
  &MemDev_Type17_1,
  &MemArrayMapAddr_Type19,
  &BiosLangInfo_Type13,
  &SystemBoot_Type32,
  NULL
};


/**
   Installed a whole table worth of structructures.

   @param[in] Smbios      The Pointer of Smbios Protocol.

   @retval EFI_SUCCESS    Table data successfully installed.
   @retval Other          Table data was not installed.

**/
EFI_STATUS
InstallStructures (
  IN EFI_SMBIOS_PROTOCOL       *Smbios,
  IN VOID *DefaultTables[]
  )
{
  EFI_STATUS                Status;
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  UINT32                    TableEntry;

  Status = EFI_SUCCESS;

  for ( TableEntry =0; DefaultTables[TableEntry] != NULL; TableEntry++)
  {
    SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER *)DefaultTables[TableEntry])->Handle;
    Status = Smbios->Add (
    Smbios,
    NULL,
    &SmbiosHandle,
    (EFI_SMBIOS_TABLE_HEADER *) DefaultTables[TableEntry]
    );
    if (EFI_ERROR (Status))
      break;
    }

  return Status;
}


/**
   Installed All SMBIOS information.

   @param[in] Smbios      The Pointer of Smbios Protocol.

   @retval EFI_SUCCESS    SMBIOS information successfully installed.
   @retval Other          SMBIOS information was not installed.

**/
STATIC
EFI_STATUS
InstallAllStructures (
  IN EFI_SMBIOS_PROTOCOL       *Smbios
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  Status = InstallStructures (Smbios, DefaultCommonTables);
  ASSERT_EFI_ERROR (Status);

  return Status;
}


/**
   Find the smbios protocol and installed SMBIOS information
   for ARM platforms.

   @param[in] ImageHandle     Module's image handle.
   @param[in] SystemTable     Pointer of EFI_SYSTEM_TABLE.

   @retval EFI_SUCCESS    Smbios data successfully installed.
   @retval Other          Smbios data was not installed.

**/
EFI_STATUS
EFIAPI
SmbiosTablePublishEntry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_SMBIOS_PROTOCOL       *Smbios;

  //
  // Find the SMBIOS protocol
  //
  Status = gBS->LocateProtocol (
    &gEfiSmbiosProtocolGuid,
    NULL,
    (VOID **)&Smbios
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallAllStructures (Smbios);

  return Status;
}
