/** @file
 *
 *  Static SMBIOS Table for the RaspberryPi platform
 *  Derived from EmulatorPkg package
 *
 *  Note - Arm SBBR ver 1.2 required and recommended SMBIOS structures:
 *    BIOS Information (Type 0)
 *    System Information (Type 1)
 *    Board Information (Type 2) - Recommended
 *    System Enclosure (Type 3)
 *    Processor Information (Type 4) - CPU Driver
 *    Cache Information (Type 7) - For cache that is external to processor
 *    Port Information (Type 8) - Recommended for platforms with physical ports
 *    System Slots (Type 9) - If system has slots
 *    OEM Strings (Type 11) - Recommended
 *    BIOS Language Information (Type 13) - Recommended
 *    System Event Log (Type 15) - Recommended (does not exit on RPi)
 *    Physical Memory Array (Type 16)
 *    Memory Device (Type 17) - For each socketed system-memory Device
 *    Memory Array Mapped Address (Type 19) - One per contiguous block per Physical Memroy Array
 *    System Boot Information (Type 32)
 *    IPMI Device Information (Type 38) - Required for platforms with IPMIv1.0 BMC Host Interface (not applicable to RPi)
 *    Onboard Devices Extended Information (Type 41) - Recommended
 *    Redfish Host Interface (Type 42) - Required for platforms supporting Redfish Host Interface (not applicable to RPi)
 *
 *  Copyright (c) 2017-2018, Andrey Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2013, Linaro.org
 *  Copyright (c) 2012, Apple Inc. All rights reserved.<BR>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Copyright (c) 2020, ARM Limited. All rights reserved.
 *  Copyright (c) 2021-2022 Rockchip Electronics Co., Ltd.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Base.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <Guid/SmBios.h>
#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TimeBaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PrintLib.h>
#define ASSET_TAG_STR_STORAGE_SIZE  33
#define SMB_IS_DIGIT(c)  (((c) >= '0') && ((c) <= '9'))

/***********************************************************************
        SMBIOS data definition  TYPE0  BIOS Information
************************************************************************/
SMBIOS_TABLE_TYPE0 mBIOSInfoType0 = {
  { EFI_SMBIOS_TYPE_BIOS_INFORMATION, sizeof (SMBIOS_TABLE_TYPE0), 0 },
  1,                                                     // Vendor String
  2,                                                     // BiosVersion String
  (UINT16) (FixedPcdGet32 (PcdFdBaseAddress) / 0x10000), // BiosSegment
  3,                                                     // BiosReleaseDate String
  (UINT8) (FixedPcdGet32 (PcdFdSize) / 0x10000),         // BiosSize (in 64KB)
  {                     // BiosCharacteristics
    0,    //  Reserved                          :2;  ///< Bits 0-1.
    0,    //  Unknown                           :1;
    0,    //  BiosCharacteristicsNotSupported   :1;
    0,    //  IsaIsSupported                    :1;
    0,    //  McaIsSupported                    :1;
    0,    //  EisaIsSupported                   :1;
    0,    //  PciIsSupported                    :1; /// No PCIe support since we hide ECAM from the OS
    0,    //  PcmciaIsSupported                 :1;
    0,    //  PlugAndPlayIsSupported            :1;
    0,    //  ApmIsSupported                    :1;
    1,    //  BiosIsUpgradable                  :1;
    0,    //  BiosShadowingAllowed              :1;
    0,    //  VlVesaIsSupported                 :1;
    0,    //  EscdSupportIsAvailable            :1;
    0,    //  BootFromCdIsSupported             :1;
    1,    //  SelectableBootIsSupported         :1;
    0,    //  RomBiosIsSocketed                 :1;
    0,    //  BootFromPcmciaIsSupported         :1;
    0,    //  EDDSpecificationIsSupported       :1;
    0,    //  JapaneseNecFloppyIsSupported      :1;
    0,    //  JapaneseToshibaFloppyIsSupported  :1;
    0,    //  Floppy525_360IsSupported          :1;
    0,    //  Floppy525_12IsSupported           :1;
    0,    //  Floppy35_720IsSupported           :1;
    0,    //  Floppy35_288IsSupported           :1;
    0,    //  PrintScreenIsSupported            :1;
    0,    //  Keyboard8042IsSupported           :1;
    0,    //  SerialIsSupported                 :1;
    0,    //  PrinterIsSupported                :1;
    0,    //  CgaMonoIsSupported                :1;
    0,    //  NecPc98                           :1;
    0     //  ReservedForVendor                 :32; ///< Bits 32-63. Bits 32-47 reserved for BIOS vendor
    ///< and bits 48-63 reserved for System Vendor.
  },
  {       // BIOSCharacteristicsExtensionBytes[]
    0x01, //  AcpiIsSupported                   :1;
          //  UsbLegacyIsSupported              :1;
          //  AgpIsSupported                    :1;
          //  I2OBootIsSupported                :1;
          //  Ls120BootIsSupported              :1;
          //  AtapiZipDriveBootIsSupported      :1;
          //  Boot1394IsSupported               :1;
          //  SmartBatteryIsSupported           :1;
          //  BIOSCharacteristicsExtensionBytes[1]
    0x0c, //  BiosBootSpecIsSupported              :1;
          //  FunctionKeyNetworkBootIsSupported    :1;
          //  TargetContentDistributionEnabled     :1;
          //  UefiSpecificationSupported           :1;
          //  VirtualMachineSupported              :1;
          //  ExtensionByte2Reserved               :3;
  },
  0,                       // SystemBiosMajorRelease
  0,                       // SystemBiosMinorRelease
  0,                       // EmbeddedControllerFirmwareMajorRelease
  0,                       // EmbeddedControllerFirmwareMinorRelease
  //{ (UINT16) ((FixedPcdGet32 (PcdFdSize) + FixedPcdGet32 (PcdFdtSize)) / 0x100000) }, // BiosSize (in MB since Bits 15:14 = 00b)
};

CHAR8 mBiosVendor[128]  = "EDK2";
CHAR8 mBiosVersion[128] = "EDK2-DEV";
CHAR8 mBiosDate[12]     = "13/05/2022";

CHAR8 *mBIOSInfoType0Strings[] = {
  mBiosVendor,              // Vendor
  mBiosVersion,             // Version
  mBiosDate,                // Release Date
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE1  System Information
************************************************************************/
SMBIOS_TABLE_TYPE1 mSysInfoType1 = {
  { EFI_SMBIOS_TYPE_SYSTEM_INFORMATION, sizeof (SMBIOS_TABLE_TYPE1), 0 },
  1,    // Manufacturer String
  2,    // ProductName String
  3,    // Version String
  4,    // SerialNumber String
  { 0x25EF0280, 0xEC82, 0x42B0, { 0x8F, 0xB6, 0x10, 0xAD, 0xCC, 0xC6, 0x7C, 0x02 } },
  SystemWakeupTypePowerSwitch,
  5,    // SKUNumber String
  6,    // Family String
};

CHAR8 mSysInfoManufName[128];
CHAR8 mSysInfoProductName[128];
CHAR8 mSysInfoVersionName[128];
CHAR8 mSysInfoSerial[sizeof (UINT64) * 2 + 1];
CHAR8 mSysInfoSKU[sizeof (UINT64) * 2 + 1];

CHAR8 *mSysInfoType1Strings[] = {
  "Rockchip Electronics",
  mSysInfoProductName,
  mSysInfoVersionName,
  mSysInfoSerial,
  mSysInfoSKU,
  "Rockchip",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE2  Board Information
************************************************************************/
SMBIOS_TABLE_TYPE2 mBoardInfoType2 = {
  { EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION, sizeof (SMBIOS_TABLE_TYPE2), 0 },
  1,    // Manufacturer String
  2,    // ProductName String
  3,    // Version String
  4,    // SerialNumber String
  5,    // AssetTag String
  {     // FeatureFlag
    1,    //  Motherboard           :1;
    0,    //  RequiresDaughterCard  :1;
    0,    //  Removable             :1;
    0,    //  Replaceable           :1;
    0,    //  HotSwappable          :1;
    0,    //  Reserved              :3;
  },
  6,                        // LocationInChassis String
  0,                        // ChassisHandle;
  BaseBoardTypeMotherBoard, // BoardType;
  0,                        // NumberOfContainedObjectHandles;
  { 0 }                     // ContainedObjectHandles[1];
};

CHAR8 mChassisAssetTag[128];

CHAR8 *mBoardInfoType2Strings[] = {
  mSysInfoManufName,
  mSysInfoProductName,
  mSysInfoVersionName,
  mSysInfoSerial,
  mChassisAssetTag,
  "Internal",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE3  Enclosure Information
************************************************************************/
SMBIOS_TABLE_TYPE3 mEnclosureInfoType3 = {
  { EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE, sizeof (SMBIOS_TABLE_TYPE3), 0 },
  1,                        // Manufacturer String
  MiscChassisEmbeddedPc,    // Type;
  2,                        // Version String
  3,                        // SerialNumber String
  4,                        // AssetTag String
  ChassisStateSafe,         // BootupState;
  ChassisStateSafe,         // PowerSupplyState;
  ChassisStateSafe,         // ThermalState;
  ChassisSecurityStatusNone,// SecurityStatus;
  { 0, 0, 0, 0 },           // OemDefined[4];
  0,    // Height;
  1,    // NumberofPowerCords;
  0,    // ContainedElementCount;
  0,    // ContainedElementRecordLength;
  { { 0 } },    // ContainedElements[1];
};
CHAR8 *mEnclosureInfoType3Strings[] = {
  mSysInfoManufName,
  mSysInfoProductName,
  mSysInfoSerial,
  mChassisAssetTag,
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE4  Processor Information
************************************************************************/
SMBIOS_TABLE_TYPE4 mProcessorInfoType4 = {
  { EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION, sizeof (SMBIOS_TABLE_TYPE4), 0},
  1,                               // Socket String
  CentralProcessor,                // ProcessorType;          ///< The enumeration value from PROCESSOR_TYPE_DATA.
  ProcessorFamilyIndicatorFamily2, // ProcessorFamily;        ///< The enumeration value from PROCESSOR_FAMILY2_DATA.
  2,                               // ProcessorManufacture String;
  {                                // ProcessorId;
    { 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 }
  },
  3,                    // ProcessorVersion String;
  {                     // Voltage;
    1,  // ProcessorVoltageCapability5V        :1;
    1,  // ProcessorVoltageCapability3_3V      :1;
    1,  // ProcessorVoltageCapability2_9V      :1;
    0,  // ProcessorVoltageCapabilityReserved  :1; ///< Bit 3, must be zero.
    0,  // ProcessorVoltageReserved            :3; ///< Bits 4-6, must be zero.
    0   // ProcessorVoltageIndicateLegacy      :1;
  },
  0,                      // ExternalClock;
  0,                      // MaxSpeed;
  0,                      // CurrentSpeed;
  0x41,                   // Status;
  ProcessorUpgradeNone,   // ProcessorUpgrade;         ///< The enumeration value from PROCESSOR_UPGRADE.
  0xFFFF,                 // L1CacheHandle;
  0xFFFF,                 // L2CacheHandle;
  0xFFFF,                 // L3CacheHandle;
  0,                      // SerialNumber;
  0,                      // AssetTag;
  0,                      // PartNumber;
  4,                      // CoreCount;
  4,                      // EnabledCoreCount;
  4,                      // ThreadCount;
  0x6C,                   // ProcessorCharacteristics; ///< The enumeration value from PROCESSOR_CHARACTERISTIC_FLAGS
      // ProcessorReserved1              :1;
      // ProcessorUnknown                :1;
      // Processor64BitCapble            :1;
      // ProcessorMultiCore              :1;
      // ProcessorHardwareThread         :1;
      // ProcessorExecuteProtection      :1;
      // ProcessorEnhancedVirtualization :1;
      // ProcessorPowerPerformanceCtrl    :1;
      // Processor128bitCapble            :1;
      // ProcessorReserved2               :7;
  ProcessorFamilyARM,     // ARM Processor Family;
  0,                      // CoreCount2;
  0,                      // EnabledCoreCount2;
  0,                      // ThreadCount2;
};

CHAR8 mCpuName[128] = "RK3588";

CHAR8 *mProcessorInfoType4Strings[] = {
  "Socket",
  "Rockchip",
  mCpuName,
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE7  Cache Information
************************************************************************/
SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1I = {
  { EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof (SMBIOS_TABLE_TYPE7), 0 },
  1,                        // SocketDesignation String
  0x380,                    // Cache Configuration
       //Cache Level        :3  (L1)
       //Cache Socketed     :1  (Not Socketed)
       //Reserved           :1
       //Location           :2  (Internal)
       //Enabled/Disabled   :1  (Enabled)
       //Operational Mode   :2  (Unknown)
       //Reserved           :6
#if (RPI_MODEL == 4)
  0x0030,                   // Maximum Size (RPi4: 48KB)
  0x0030,                   // Install Size (RPi4: 48KB)
#else
  0x0010,                   // Maximum Size (RPi3: 16KB)
  0x0010,                   // Install Size (RPi3: 16KB)
#endif
  {                         // Supported SRAM Type
    0,  //Other             :1
    0,  //Unknown           :1
    0,  //NonBurst          :1
    1,  //Burst             :1
    0,  //PiplelineBurst    :1
    1,  //Synchronous       :1
    0,  //Asynchronous      :1
    0   //Reserved          :9
  },
  {                         // Current SRAM Type
    0,  //Other             :1
    0,  //Unknown           :1
    0,  //NonBurst          :1
    1,  //Burst             :1
    0,  //PiplelineBurst    :1
    1,  //Synchronous       :1
    0,  //Asynchronous      :1
    0   //Reserved          :9
  },
  0,                        // Cache Speed unknown
  CacheErrorParity,         // Error Correction
  CacheTypeInstruction,     // System Cache Type
  CacheAssociativity2Way    // Associativity  (RPi4 L1 Instruction cache is 3-way set associative, but SMBIOS spec does not define that)
};
CHAR8  *mCacheInfoType7Strings_L1I[] = {
  "L1 Instruction",
  NULL
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1D = {
  { EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof (SMBIOS_TABLE_TYPE7), 0 },
  1,                        // SocketDesignation String
  0x180,                    // Cache Configuration
       //Cache Level        :3  (L1)
       //Cache Socketed     :1  (Not Socketed)
       //Reserved           :1
       //Location           :2  (Internal)
       //Enabled/Disabled   :1  (Enabled)
       //Operational Mode   :2  (WB)
       //Reserved           :6
#if (RPI_MODEL == 4)
  0x0020,                   // Maximum Size (RPi4: 32KB)
  0x0020,                   // Install Size (RPi4: 32KB)
#else
  0x0010,                   // Maximum Size (RPi3: 16KB)
  0x0010,                   // Install Size (RPi3: 16KB)
#endif
  {                         // Supported SRAM Type
    0,  //Other             :1
    0,  //Unknown           :1
    0,  //NonBurst          :1
    1,  //Burst             :1
    0,  //PiplelineBurst    :1
    1,  //Synchronous       :1
    0,  //Asynchronous      :1
    0   //Reserved          :9
  },
  {                         // Current SRAM Type
    0,  //Other             :1
    0,  //Unknown           :1
    0,  //NonBurst          :1
    1,  //Burst             :1
    0,  //PiplelineBurst    :1
    1,  //Synchronous       :1
    0,  //Asynchronous      :1
    0   //Reserved          :9
  },
  0,                        // Cache Speed unknown
  CacheErrorSingleBit,      // Error Correction
  CacheTypeData,            // System Cache Type
#if (RPI_MODEL == 4)
  CacheAssociativity2Way    // Associativity
#else
  CacheAssociativity4Way    // Associativity
#endif
};
CHAR8  *mCacheInfoType7Strings_L1D[] = {
  "L1 Data",
  NULL
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L2 = {
  { EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof (SMBIOS_TABLE_TYPE7), 0 },
  1,                        // SocketDesignation String
  0x0181,                   // Cache Configuration
       //Cache Level        :3  (L2)
       //Cache Socketed     :1  (Not Socketed)
       //Reserved           :1
       //Location           :2  (Internal)
       //Enabled/Disabled   :1  (Enabled)
       //Operational Mode   :2  (WB)
       //Reserved           :6
  0x0400,                   // Maximum Size (1MB)
  0x0400,                   // Install Size (1MB)
  {                         // Supported SRAM Type
    0,  //Other             :1
    0,  //Unknown           :1
    0,  //NonBurst          :1
    1,  //Burst             :1
    0,  //PiplelineBurst    :1
    1,  //Synchronous       :1
    0,  //Asynchronous      :1
    0   //Reserved          :9
  },
  {                         // Current SRAM Type
    0,  //Other             :1
    0,  //Unknown           :1
    0,  //NonBurst          :1
    1,  //Burst             :1
    0,  //PiplelineBurst    :1
    1,  //Synchronous       :1
    0,  //Asynchronous      :1
    0   //Reserved          :9
  },
  0,                        // Cache Speed unknown
  CacheErrorSingleBit,      // Error Correction Multi
  CacheTypeUnified,         // System Cache Type
  CacheAssociativity16Way   // Associativity
};
CHAR8  *mCacheInfoType7Strings_L2[] = {
  "L2",
  NULL
};
/***********************************************************************
        SMBIOS data definition  TYPE9  System Slot Information
************************************************************************/
SMBIOS_TABLE_TYPE9  mSysSlotInfoType9 = {
  { EFI_SMBIOS_TYPE_SYSTEM_SLOTS, sizeof (SMBIOS_TABLE_TYPE9), 0 },
  1,    // SlotDesignation String
  SlotTypeOther,          // SlotType;                 ///< The enumeration value from MISC_SLOT_TYPE.
  SlotDataBusWidthOther,  // SlotDataBusWidth;         ///< The enumeration value from MISC_SLOT_DATA_BUS_WIDTH.
  SlotUsageAvailable,     // CurrentUsage;             ///< The enumeration value from MISC_SLOT_USAGE.
  SlotLengthOther,        // SlotLength;               ///< The enumeration value from MISC_SLOT_LENGTH.
  0,    // SlotID;
  {    // SlotCharacteristics1;
    1,  // CharacteristicsUnknown  :1;
    0,  // Provides50Volts         :1;
    0,  // Provides33Volts         :1;
    0,  // SharedSlot              :1;
    0,  // PcCard16Supported       :1;
    0,  // CardBusSupported        :1;
    0,  // ZoomVideoSupported      :1;
    0,  // ModemRingResumeSupported:1;
  },
  {     // SlotCharacteristics2;
    0,  // PmeSignalSupported      :1;
    0,  // HotPlugDevicesSupported :1;
    0,  // SmbusSignalSupported    :1;
    0,  // Reserved                :5;  ///< Set to 0.
  },
  0xFFFF, // SegmentGroupNum;
  0xFF,   // BusNum;
  0xFF,   // DevFuncNum;
};
CHAR8 *mSysSlotInfoType9Strings[] = {
  "SD Card",
  NULL
};


/***********************************************************************
        SMBIOS data definition  TYPE 11  OEM Strings
************************************************************************/

SMBIOS_TABLE_TYPE11 mOemStringsType11 = {
  { EFI_SMBIOS_TYPE_OEM_STRINGS, sizeof (SMBIOS_TABLE_TYPE11), 0 },
  1 // StringCount
};
CHAR8 *mOemStringsType11Strings[] = {
  "https://github/tianocore/edk2-platforms",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE16  Physical Memory ArrayInformation
************************************************************************/
SMBIOS_TABLE_TYPE16 mPhyMemArrayInfoType16 = {
  { EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, sizeof (SMBIOS_TABLE_TYPE16), 0 },
  MemoryArrayLocationSystemBoard, // Location;                       ///< The enumeration value from MEMORY_ARRAY_LOCATION.
  MemoryArrayUseSystemMemory,     // Use;                            ///< The enumeration value from MEMORY_ARRAY_USE.
  MemoryErrorCorrectionUnknown,   // MemoryErrorCorrection;          ///< The enumeration value from MEMORY_ERROR_CORRECTION.
  0x00000000,                     // MaximumCapacity;
  0xFFFE,                         // MemoryErrorInformationHandle;
  1,                              // NumberOfMemoryDevices;
  0x00000000ULL,                  // ExtendedMaximumCapacity;
};
CHAR8 *mPhyMemArrayInfoType16Strings[] = {
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE17  Memory Device Information
************************************************************************/
SMBIOS_TABLE_TYPE17 mMemDevInfoType17 = {
  { EFI_SMBIOS_TYPE_MEMORY_DEVICE, sizeof (SMBIOS_TABLE_TYPE17), 0 },
  0,                    // MemoryArrayHandle; // Should match SMBIOS_TABLE_TYPE16.Handle, initialized at runtime, refer to PhyMemArrayInfoUpdateSmbiosType16()
  0xFFFE,               // MemoryErrorInformationHandle; (not provided)
  0xFFFF,               // TotalWidth; (unknown)
  0xFFFF,               // DataWidth; (unknown)
  0xFFFF,               // Size; // When bit 15 is 0: Size in MB
                        // When bit 15 is 1: Size in KB, and continues in ExtendedSize
                        // initialized at runtime, refer to PhyMemArrayInfoUpdateSmbiosType16()
  MemoryFormFactorChip, // FormFactor;                     ///< The enumeration value from MEMORY_FORM_FACTOR.
  0,                    // DeviceSet;
  1,                    // DeviceLocator String
  0,                    // BankLocator String
  MemoryTypeLpddr4,     // MemoryType;                     ///< The enumeration value from MEMORY_DEVICE_TYPE.
  {                     // TypeDetail;
    0,  // Reserved        :1;
    0,  // Other           :1;
    1,  // Unknown         :1;
    0,  // FastPaged       :1;
    0,  // StaticColumn    :1;
    0,  // PseudoStatic    :1;
    0,  // Rambus          :1;
    0,  // Synchronous     :1;
    0,  // Cmos            :1;
    0,  // Edo             :1;
    0,  // WindowDram      :1;
    0,  // CacheDram       :1;
    0,  // Nonvolatile     :1;
    0,  // Registered      :1;
    0,  // Unbuffered      :1;
    0,  // Reserved1       :1;
  },
  0,                    // Speed; (unknown)
  2,                    // Manufacturer String
  0,                    // SerialNumber String
  0,                    // AssetTag String
  0,                    // PartNumber String
  0,                    // Attributes; (unknown rank)
  0,                    // ExtendedSize; (since Size < 32GB-1)
  0,                    // ConfiguredMemoryClockSpeed; (unknown)
  0,                    // MinimumVoltage; (unknown)
  0,                    // MaximumVoltage; (unknown)
  0,                    // ConfiguredVoltage; (unknown)
  MemoryTechnologyDram, // MemoryTechnology                 ///< The enumeration value from MEMORY_DEVICE_TECHNOLOGY
  {{                    // MemoryOperatingModeCapability
    0,  // Reserved                        :1;
    0,  // Other                           :1;
    0,  // Unknown                         :1;
    1,  // VolatileMemory                  :1;
    0,  // ByteAccessiblePersistentMemory  :1;
    0,  // BlockAccessiblePersistentMemory :1;
    0   // Reserved                        :10;
  }},
  0,                    // FirwareVersion
  0,                    // ModuleManufacturerID (unknown)
  0,                    // ModuleProductID (unknown)
  0,                    // MemorySubsystemControllerManufacturerID (unknown)
  0,                    // MemorySubsystemControllerProductID (unknown)
  0,                    // NonVolatileSize
  0xFFFFFFFFFFFFFFFFULL,// VolatileSize // initialized at runtime, refer to PhyMemArrayInfoUpdateSmbiosType16()
  0,                    // CacheSize
  0,                    // LogicalSize (since MemoryType is not MemoryTypeLogicalNonVolatileDevice)
  0,                    // ExtendedSpeed,
  0                     // ExtendedConfiguredMemorySpeed
};
CHAR8 *mMemDevInfoType17Strings[] = {
  "SDRAM",
  "Micron",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE19  Memory Array Mapped Address Information
************************************************************************/
SMBIOS_TABLE_TYPE19 mMemArrMapInfoType19 = {
  { EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS, sizeof (SMBIOS_TABLE_TYPE19), 0 },
  0x00000000, // StartingAddress;
  0x00000000, // EndingAddress;
  0,          // MemoryArrayHandle; // Should match SMBIOS_TABLE_TYPE16.Handle, initialized at runtime, refer to PhyMemArrayInfoUpdateSmbiosType16()
  1,          // PartitionWidth;
  0,          // ExtendedStartingAddress;  // not used
  0,          // ExtendedEndingAddress;    // not used
};
CHAR8 *mMemArrMapInfoType19Strings[] = {
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE32  Boot Information
************************************************************************/
SMBIOS_TABLE_TYPE32 mBootInfoType32 = {
  { EFI_SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION, sizeof (SMBIOS_TABLE_TYPE32), 0 },
  { 0, 0, 0, 0, 0, 0 },         // Reserved[6];
  BootInformationStatusNoError  // BootStatus
};

CHAR8 *mBootInfoType32Strings[] = {
  NULL
};

/**

   Create SMBIOS record.

   Converts a fixed SMBIOS structure and an array of pointers to strings into
   an SMBIOS record where the strings are cat'ed on the end of the fixed record
   and terminated via a double NULL and add to SMBIOS table.

   SMBIOS_TABLE_TYPE32 gSmbiosType12 = {
   { EFI_SMBIOS_TYPE_SYSTEM_CONFIGURATION_OPTIONS, sizeof (SMBIOS_TABLE_TYPE12), 0 },
   1 // StringCount
   };

   CHAR8 *gSmbiosType12Strings[] = {
   "Not Found",
   NULL
   };

   ...

   LogSmbiosData (
   (EFI_SMBIOS_TABLE_HEADER*)&gSmbiosType12,
   gSmbiosType12Strings
   );

   @param  Template    Fixed SMBIOS structure, required.
   @param  StringPack  Array of strings to convert to an SMBIOS string pack.
   NULL is OK.
   @param  DataSmbiosHandle  The new SMBIOS record handle.
   NULL is OK.
**/

EFI_STATUS
EFIAPI
LogSmbiosData (
  IN  EFI_SMBIOS_TABLE_HEADER *Template,
  IN  CHAR8                   **StringPack,
  OUT EFI_SMBIOS_HANDLE       *DataSmbiosHandle
  )
{
  EFI_STATUS                Status;
  EFI_SMBIOS_PROTOCOL       *Smbios;
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER   *Record;
  UINTN                     Index;
  UINTN                     StringSize;
  UINTN                     Size;
  CHAR8                     *Str;

  //
  // Locate Smbios protocol.
  //
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID**)&Smbios);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Calculate the size of the fixed record and optional string pack

  Size = Template->Length;
  if (StringPack == NULL) {
    // At least a double null is required
    Size += 2;
  } else {
    for (Index = 0; StringPack[Index] != NULL; Index++) {
      StringSize = AsciiStrSize (StringPack[Index]);
      Size += StringSize;
    }
    if (StringPack[0] == NULL) {
      // At least a double null is required
      Size += 1;
    }

    // Don't forget the terminating double null
    Size += 1;
  }

  // Copy over Template
  Record = (EFI_SMBIOS_TABLE_HEADER*)AllocateZeroPool (Size);
  if (Record == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (Record, Template, Template->Length);

  // Append string pack
  Str = ((CHAR8*)Record) + Record->Length;

  for (Index = 0; StringPack[Index] != NULL; Index++) {
    StringSize = AsciiStrSize (StringPack[Index]);
    CopyMem (Str, StringPack[Index], StringSize);
    Str += StringSize;
  }

  *Str = 0;
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = Smbios->Add (
                     Smbios,
                     gImageHandle,
                     &SmbiosHandle,
                     Record
                   );

  if ((Status == EFI_SUCCESS) && (DataSmbiosHandle != NULL)) {
    *DataSmbiosHandle = SmbiosHandle;
  }

  ASSERT_EFI_ERROR (Status);
  FreePool (Record);
  return Status;
}

/***********************************************************************
        SMBIOS data update  TYPE0  BIOS Information
************************************************************************/
VOID
BIOSInfoUpdateSmbiosType0 (
  VOID
  )
{
  UINT32 EpochSeconds = 0;
  EFI_TIME Time;
  INTN   State = 0;
  INTN   i;
  INTN   Value[2];
  INTN   Year = TIME_BUILD_YEAR;
  INTN   Month = TIME_BUILD_MONTH;
  INTN   Day = TIME_BUILD_DAY;

  // Populate the Firmware major and minor.
    // The firmware revision is really an epoch time which we convert to a
    // YY.MM major.minor. This is good enough for our purpose, where this
    // revision is merely provided as a loose indicator of when the
    // VideoCore firmware was generated.
    EpochToEfiTime (EpochSeconds, &Time);
    ASSERT (Time.Year >= 2000 && Time.Year <= 2255);
    mBIOSInfoType0.EmbeddedControllerFirmwareMajorRelease = (UINT8)(Time.Year - 2000);
    mBIOSInfoType0.EmbeddedControllerFirmwareMinorRelease = Time.Month;
  // mBiosVendor and mBiosVersion, which are referenced in mBIOSInfoType0Strings,
  // are left unchanged if the following calls fail.
  UnicodeStrToAsciiStrS ((CHAR16*)PcdGetPtr (PcdFirmwareVendor),
    mBiosVendor, sizeof (mBiosVendor));
  UnicodeStrToAsciiStrS ((CHAR16*)PcdGetPtr (PcdFirmwareVersionString),
    mBiosVersion, sizeof (mBiosVersion));
  ASSERT (Year >= 0 && Year <= 9999);
  ASSERT (Month >= 1 && Month <= 12);
  ASSERT (Day >= 1 && Day <= 31);
  AsciiSPrint (mBiosDate, sizeof (mBiosDate), "%02d/%02d/%04d", Month, Day, Year);

  // Look for a "x.y" numeric string anywhere in mBiosVersion and
  // try to parse it to populate the BIOS major and minor.
  for (i = 0; (i < AsciiStrLen (mBiosVersion)) && (State < 4); i++) {
    switch (State) {
    case 0:
      if (!SMB_IS_DIGIT (mBiosVersion[i]))
        break;
      Value[0] = Value[1] = 0;
      State++;
      // Fall through
    case 1:
    case 3:
      if (SMB_IS_DIGIT (mBiosVersion[i])) {
        Value[State / 2] = (Value[State / 2] * 10) + (mBiosVersion[i] - '0');
        if (Value[State / 2] > 255) {
          while (SMB_IS_DIGIT (mBiosVersion[i + 1]))
            i++;
          // Reset our state (we may have something like "Firmware X83737.1 v1.23")
          State = 0;
        }
      } else {
        State++;
      }
      if (State != 2)
        break;
      // Fall through
    case 2:
      if ((mBiosVersion[i] == '.') && (SMB_IS_DIGIT (mBiosVersion[i + 1]))) {
        State++;
      } else {
        State = 0;
      }
      break;
    }
  }
  if ((State == 3) || (State == 4)) {
    mBIOSInfoType0.SystemBiosMajorRelease = (UINT8)Value[0];
    mBIOSInfoType0.SystemBiosMinorRelease = (UINT8)Value[1];
  }

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mBIOSInfoType0, mBIOSInfoType0Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE1  System Information
************************************************************************/
VOID
I64ToHexString (
  IN OUT CHAR8* TargetStringSz,
  IN UINT32 TargetStringSize,
  IN UINT64 Value
  )
{
  static CHAR8 ItoH[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
  UINT8 StringInx;
  INT8 NibbleInx;

  ZeroMem ((void*)TargetStringSz, TargetStringSize);

  //
  // Convert each nibble to hex string, starting from
  // the highest non-zero nibble.
  //
  StringInx = 0;
  for (NibbleInx = sizeof (UINT64) * 2; NibbleInx > 0; --NibbleInx) {
    UINT64 NibbleMask = (((UINT64)0xF) << ((NibbleInx - 1) * 4));
    UINT8 Nibble = (UINT8)((Value & NibbleMask) >> ((NibbleInx - 1) * 4));

    ASSERT (Nibble <= 0xF);

    if (StringInx < (TargetStringSize - 1)) {
      TargetStringSz[StringInx++] = ItoH[Nibble];
    } else {
      break;
    }
  }
}

VOID
SysInfoUpdateSmbiosType1 (
  VOID
  )
{
  UINT32 BoardRevision = 0;
  UINT64 BoardSerial = 0;

  AsciiStrCpyS (mSysInfoProductName, sizeof (mSysInfoProductName),"RK3588");
  AsciiStrCpyS (mSysInfoManufName, sizeof (mSysInfoManufName),"Rockchip");
  AsciiSPrint (mSysInfoVersionName, sizeof (mSysInfoVersionName), "%X", BoardRevision);
  I64ToHexString (mSysInfoSKU, sizeof (mSysInfoSKU), BoardRevision);
  I64ToHexString (mSysInfoSerial, sizeof (mSysInfoSerial), BoardSerial);
  DEBUG ((DEBUG_ERROR, "Board Serial Number: %a\n", mSysInfoSerial));
  mSysInfoType1.Uuid.Data1 = BoardRevision;
  mSysInfoType1.Uuid.Data2 = 0x0;
  mSysInfoType1.Uuid.Data3 = 0x0;
  // Swap endianness, so that the serial is more user-friendly as a UUID
  BoardSerial = SwapBytes64 (BoardSerial);
  CopyMem (mSysInfoType1.Uuid.Data4, &BoardSerial, sizeof (BoardSerial));

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mSysInfoType1, mSysInfoType1Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE2  Board Information
************************************************************************/
VOID
BoardInfoUpdateSmbiosType2 (
  VOID
  )
{

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mBoardInfoType2, mBoardInfoType2Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE3  Enclosure Information
************************************************************************/
VOID
EnclosureInfoUpdateSmbiosType3 (
  VOID
  )
{
  EFI_SMBIOS_HANDLE SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mEnclosureInfoType3, mEnclosureInfoType3Strings, &SmbiosHandle);

  // Set Type2 ChassisHandle to point to the newly added Type3 handle
  mBoardInfoType2.ChassisHandle = (UINT16) SmbiosHandle;
}

/***********************************************************************
        SMBIOS data update  TYPE4  Processor Information
************************************************************************/
VOID
ProcessorInfoUpdateSmbiosType4 (
  IN UINTN MaxCpus
  )
{
  //EFI_STATUS Status;
 // UINT32     Rate;
  UINT64     *ProcessorId;

  mProcessorInfoType4.CoreCount = (UINT8)MaxCpus;
  mProcessorInfoType4.CoreCount2 = (UINT8)MaxCpus;
  mProcessorInfoType4.EnabledCoreCount = (UINT8)MaxCpus;
  mProcessorInfoType4.EnabledCoreCount2 = (UINT8)MaxCpus;
  mProcessorInfoType4.ThreadCount = (UINT8)MaxCpus;
  mProcessorInfoType4.ThreadCount2 = (UINT8)MaxCpus;
  mProcessorInfoType4.MaxSpeed = 2100;    /*2100 MHZ*/
  mProcessorInfoType4.CurrentSpeed = 1800;/*1800 MHZ*/

  ProcessorId = (UINT64 *)&(mProcessorInfoType4.ProcessorId);
  *ProcessorId = ArmReadMidr();

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mProcessorInfoType4, mProcessorInfoType4Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE7  Cache Information
************************************************************************/
VOID
CacheInfoUpdateSmbiosType7 (
  VOID
  )
{
  EFI_SMBIOS_HANDLE SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mCacheInfoType7_L1I, mCacheInfoType7Strings_L1I, NULL);

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mCacheInfoType7_L1D, mCacheInfoType7Strings_L1D, &SmbiosHandle);
  // Set Type4 L1CacheHandle to point to the newly added L1 Data Cache
  mProcessorInfoType4.L1CacheHandle = (UINT16) SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mCacheInfoType7_L2, mCacheInfoType7Strings_L2, &SmbiosHandle);
  // Set Type4 L2CacheHandle to point to the newly added L2 Cache
  mProcessorInfoType4.L2CacheHandle = (UINT16) SmbiosHandle;
}

/***********************************************************************
        SMBIOS data update  TYPE9  System Slot Information
************************************************************************/
VOID
SysSlotInfoUpdateSmbiosType9 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mSysSlotInfoType9, mSysSlotInfoType9Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE11  OEM Strings
************************************************************************/
VOID
OemStringsUpdateSmbiosType11 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mOemStringsType11, mOemStringsType11Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE16  Physical Memory Array Information
************************************************************************/
VOID
PhyMemArrayInfoUpdateSmbiosType16 (
  VOID
  )
{
  EFI_SMBIOS_HANDLE MemArraySmbiosHandle;
  EFI_STATUS        Status = 0;
  UINT32            InstalledMB = 0;

 //
 // Update memory size fields:
 //  - Type 16 MaximumCapacity in KB
 //  - Type 17 size in MB (since bit 15 = 0)
 //  - Type 17 VolatileSize in Bytes
 //

 // The minimum RAM size used on any Raspberry Pi model is 256 MB
  mMemDevInfoType17.Size = 256;

  //Status = mFwProtocol->GetModelInstalledMB (&InstalledMB);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "Couldn't get the board memory size - defaulting to 256 MB: %r\n", Status));
  } else {
    mMemDevInfoType17.Size = InstalledMB; // Size in MB
  }

  mPhyMemArrayInfoType16.MaximumCapacity = mMemDevInfoType17.Size * 1024; // Size in KB
  mMemDevInfoType17.VolatileSize = MultU64x32 (mMemDevInfoType17.Size, 1024 * 1024);  // Size in Bytes

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mPhyMemArrayInfoType16, mPhyMemArrayInfoType16Strings, &MemArraySmbiosHandle);

  //
  // Update the memory device information and memory array map with the newly added type 16 handle
  //
  mMemDevInfoType17.MemoryArrayHandle = MemArraySmbiosHandle;
  mMemArrMapInfoType19.MemoryArrayHandle = MemArraySmbiosHandle;
}

/***********************************************************************
        SMBIOS data update  TYPE17  Memory Device Information
************************************************************************/
VOID
MemDevInfoUpdateSmbiosType17 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mMemDevInfoType17, mMemDevInfoType17Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE19  Memory Array Map Information
************************************************************************/
VOID
MemArrMapInfoUpdateSmbiosType19 (
  VOID
  )
{
  //EFI_STATUS Status;
  //UINT32 InstalledMB = 0;

  // Note: Type 19 addresses are expressed in KB, not bytes
  // The memory layout used in all known Pi SoC's starts at 0
  mMemArrMapInfoType19.StartingAddress = 0;
  mMemArrMapInfoType19.EndingAddress = 1024 * 1024;
  mMemArrMapInfoType19.EndingAddress = 8192 * 1024;
  mMemArrMapInfoType19.EndingAddress -= 1;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mMemArrMapInfoType19, mMemArrMapInfoType19Strings, NULL);
}


/***********************************************************************
        SMBIOS data update  TYPE32  Boot Information
************************************************************************/
VOID
BootInfoUpdateSmbiosType32 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER*)&mBootInfoType32, mBootInfoType32Strings, NULL);
}

/***********************************************************************
        Driver Entry
************************************************************************/
EFI_STATUS
EFIAPI
PlatformSmbiosDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  BIOSInfoUpdateSmbiosType0 ();

  SysInfoUpdateSmbiosType1 ();

  EnclosureInfoUpdateSmbiosType3 (); // Add Type 3 first to get chassis handle for use in Type 2

  BoardInfoUpdateSmbiosType2 ();

  CacheInfoUpdateSmbiosType7 (); // Add Type 7 first to get Cache handle for use in Type 4

  ProcessorInfoUpdateSmbiosType4 (4);   //One example for creating and updating

  SysSlotInfoUpdateSmbiosType9 ();

  OemStringsUpdateSmbiosType11 ();

  PhyMemArrayInfoUpdateSmbiosType16 ();

  MemDevInfoUpdateSmbiosType17 ();

  MemArrMapInfoUpdateSmbiosType19 ();

  BootInfoUpdateSmbiosType32 ();

  return EFI_SUCCESS;
}
