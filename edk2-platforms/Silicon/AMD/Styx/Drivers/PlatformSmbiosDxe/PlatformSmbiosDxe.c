/** @file
  Static SMBIOS Table for ARM platform
  Derived from EmulatorPkg package

  Note SMBIOS 2.7.1 Required structures:
    BIOS Information (Type 0)
    System Information (Type 1)
    Board Information (Type 2)
    System Enclosure (Type 3)
    Processor Information (Type 4) - CPU Driver
    Cache Information (Type 7) - For cache that is external to processor
    System Slots (Type 9) - If system has slots
    Physical Memory Array (Type 16)
    Memory Device (Type 17) - For each socketed system-memory Device
    Memory Array Mapped Address (Type 19) - One per contiguous block per Physical Memroy Array
    System Boot Information (Type 32)

  Copyright (c) 2012, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2013, Linaro Ltd. All rights reserved.<BR>
  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <Guid/SmBios.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Protocol/AmdIscpDxeProtocol.h>

/*----------------------------------------------------------------------------------------
 *                                  E X T E R N S
 *----------------------------------------------------------------------------------------
 */
extern  EFI_BOOT_SERVICES       *gBS;


/*----------------------------------------------------------------------------------------
 *                                  G L O B A L S
 *----------------------------------------------------------------------------------------
 */
STATIC EFI_SMBIOS_PROTOCOL   *mSmbiosProtocol;
STATIC AMD_ISCP_DXE_PROTOCOL *mIscpDxeProtocol;
STATIC ISCP_SMBIOS_INFO      mSmbiosInfo;


/***********************************************************************
        SMBIOS data definition  TYPE0  BIOS Information
************************************************************************/
STATIC CONST SMBIOS_TABLE_TYPE0 mBIOSInfoType0 = {
  { EFI_SMBIOS_TYPE_BIOS_INFORMATION, sizeof (SMBIOS_TABLE_TYPE0), 0 },
  1,                    // Vendor String
  2,                    // BiosVersion String
  0xE000,               // BiosSegment
  3,                    // BiosReleaseDate String
  0x7F,                 // BiosSize
  {                     // BiosCharacteristics
    0,    //  Reserved                          :2;  ///< Bits 0-1.
    0,    //  Unknown                           :1;
    0,    //  BiosCharacteristicsNotSupported   :1;
    0,    //  IsaIsSupported                    :1;
    0,    //  McaIsSupported                    :1;
    0,    //  EisaIsSupported                   :1;
    0,    //  PciIsSupported                    :1;
    0,    //  PcmciaIsSupported                 :1;
    0,    //  PlugAndPlayIsSupported            :1;
    0,    //  ApmIsSupported                    :1;
    1,    //  BiosIsUpgradable                  :1;
    1,    //  BiosShadowingAllowed              :1;
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
    0x81, //  AcpiIsSupported                   :1;
          //  UsbLegacyIsSupported              :1;
          //  AgpIsSupported                    :1;
          //  I2OBootIsSupported                :1;
          //  Ls120BootIsSupported              :1;
          //  AtapiZipDriveBootIsSupported      :1;
          //  Boot1394IsSupported               :1;
          //  SmartBatteryIsSupported           :1;
                  //  BIOSCharacteristicsExtensionBytes[1]
    0x0a, //  BiosBootSpecIsSupported              :1;
          //  FunctionKeyNetworkBootIsSupported    :1;
          //  TargetContentDistributionEnabled     :1;
          //  UefiSpecificationSupported           :1;
          //  VirtualMachineSupported              :1;
          //  ExtensionByte2Reserved               :3;
  },
  0x00,                    // SystemBiosMajorRelease
  0x01,                    // SystemBiosMinorRelease
  0xFF,                    // EmbeddedControllerFirmwareMajorRelease
  0xFF,                    // EmbeddedControllerFirmwareMinorRelease
};

STATIC CHAR8 CONST * CONST mBIOSInfoType0Strings[] = {
  "github.com/tianocore/", // Vendor String
  __TIME__,                // BiosVersion String
  __DATE__,                // BiosReleaseDate String
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE1  System Information
************************************************************************/
STATIC CONST SMBIOS_TABLE_TYPE1 mSysInfoType1 = {
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
STATIC CHAR8 CONST * CONST mSysInfoType1Strings[] = {
  "AMD",
  "Seattle",
  "1.0",
  "System Serial#",
  "System SKU#",
  "edk2",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE2  Board Information
************************************************************************/
STATIC CONST SMBIOS_TABLE_TYPE2  mBoardInfoType2 = {
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
  6,    // LocationInChassis String
  0,                        // ChassisHandle;
  BaseBoardTypeMotherBoard, // BoardType;
  0,                        // NumberOfContainedObjectHandles;
  { 0 }                     // ContainedObjectHandles[1];
};
STATIC CHAR8 CONST * CONST mBoardInfoType2Strings[] = {
  "AMD",
  "Seattle",
  "1.0",
  "Base Board Serial#",
  "Base Board Asset Tag#",
  "Part Component",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE3  Enclosure Information
************************************************************************/
STATIC CONST SMBIOS_TABLE_TYPE3  mEnclosureInfoType3 = {
  { EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE, sizeof (SMBIOS_TABLE_TYPE3), 0 },
  1,                        // Manufacturer String
  MiscChassisTypeLapTop,    // Type;
  2,                        // Version String
  3,                        // SerialNumber String
  4,                        // AssetTag String
  ChassisStateSafe,         // BootupState;
  ChassisStateSafe,         // PowerSupplyState;
  ChassisStateSafe,         // ThermalState;
  ChassisSecurityStatusNone,// SecurityStatus;
  { 0, 0, 0, 0 },           // OemDefined[4];
  0,    // Height;
  0,    // NumberofPowerCords;
  0,    // ContainedElementCount;
  0,    // ContainedElementRecordLength;
  { { 0 } },    // ContainedElements[1];
};
STATIC CHAR8 CONST * CONST mEnclosureInfoType3Strings[] = {
  "AMD",
  "1.0",
  "Chassis Board Serial#",
  "Chassis Board Asset Tag#",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE4  Processor Information
************************************************************************/
STATIC SMBIOS_TABLE_TYPE4 mProcessorInfoType4 = {
  { EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION, sizeof (SMBIOS_TABLE_TYPE4), 0},
  1,                    // Socket String
  ProcessorOther,       // ProcessorType;                                     ///< The enumeration value from PROCESSOR_TYPE_DATA.
  ProcessorFamilyIndicatorFamily2, // ProcessorFamily;        ///< The enumeration value from PROCESSOR_FAMILY2_DATA.
  2,                    // ProcessorManufacture String;
  {                     // ProcessorId;
    {  // PROCESSOR_SIGNATURE
      0, //  ProcessorSteppingId:4;
      0, //  ProcessorModel:     4;
      0, //  ProcessorFamily:    4;
      0, //  ProcessorType:      2;
      0, //  ProcessorReserved1: 2;
      0, //  ProcessorXModel:    4;
      0, //  ProcessorXFamily:   8;
      0, //  ProcessorReserved2: 4;
    },

    {  // PROCESSOR_FEATURE_FLAGS
      0, //  ProcessorFpu       :1;
      0, //  ProcessorVme       :1;
      0, //  ProcessorDe        :1;
      0, //  ProcessorPse       :1;
      0, //  ProcessorTsc       :1;
      0, //  ProcessorMsr       :1;
      0, //  ProcessorPae       :1;
      0, //  ProcessorMce       :1;
      0, //  ProcessorCx8       :1;
      0, //  ProcessorApic      :1;
      0, //  ProcessorReserved1 :1;
      0, //  ProcessorSep       :1;
      0, //  ProcessorMtrr      :1;
      0, //  ProcessorPge       :1;
      0, //  ProcessorMca       :1;
      0, //  ProcessorCmov      :1;
      0, //  ProcessorPat       :1;
      0, //  ProcessorPse36     :1;
      0, //  ProcessorPsn       :1;
      0, //  ProcessorClfsh     :1;
      0, //  ProcessorReserved2 :1;
      0, //  ProcessorDs        :1;
      0, //  ProcessorAcpi      :1;
      0, //  ProcessorMmx       :1;
      0, //  ProcessorFxsr      :1;
      0, //  ProcessorSse       :1;
      0, //  ProcessorSse2      :1;
      0, //  ProcessorSs        :1;
      0, //  ProcessorReserved3 :1;
      0, //  ProcessorTm        :1;
      0, //  ProcessorReserved4 :2;
    }
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
  ProcessorUpgradeOther,  // ProcessorUpgrade;      ///< The enumeration value from PROCESSOR_UPGRADE.
  0,                      // L1CacheHandle;
  0,                      // L2CacheHandle;
  0,                      // L3CacheHandle;
  4,                      // SerialNumber;
  5,                      // AssetTag;
  6,                      // PartNumber;
  0,                      // CoreCount;
  0,                      // EnabledCoreCount;
  0,                      // ThreadCount;
  0,                      // ProcessorCharacteristics;
  ProcessorFamilyARM,     // ARM Processor Family;
};

STATIC CHAR8 CONST * CONST mProcessorInfoType4Strings[] = {
  "Socket",
  "ARM",
  "v8",
  "1.0",
  "1.0",
  "1.0",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE7  Cache Information
************************************************************************/
STATIC SMBIOS_TABLE_TYPE7  mCacheInfoType7 = {
  { EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof (SMBIOS_TABLE_TYPE7), 0 },
  1,                        // SocketDesignation String
  0x018A,                                       // Cache Configuration
  0x00FF,                                       // Maximum Size 256k
  0x00FF,                                       // Install Size 256k
  {                         // Supported SRAM Type
        0,  //Other             :1
        0,  //Unknown           :1
        0,  //NonBurst          :1
        1,  //Burst             :1
        0,  //PiplelineBurst    :1
        1,  //Synchronous       :1
        0,  //Asynchronous      :1
        0       //Reserved          :9
  },
  {                         // Current SRAM Type
        0,  //Other             :1
        0,  //Unknown           :1
        0,  //NonBurst          :1
        1,  //Burst             :1
        0,  //PiplelineBurst    :1
        1,  //Synchronous       :1
        0,  //Asynchronous      :1
        0       //Reserved          :9
  },
  0,                                            // Cache Speed unknown
  CacheErrorMultiBit,           // Error Correction Multi
  CacheTypeUnknown,                     // System Cache Type
  CacheAssociativity2Way        // Associativity
};
STATIC CONST CHAR8 *mCacheInfoType7Strings[] = {
  "Cache1",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE9  System Slot Information
************************************************************************/
STATIC CONST SMBIOS_TABLE_TYPE9  mSysSlotInfoType9 = {
  { EFI_SMBIOS_TYPE_SYSTEM_SLOTS, sizeof (SMBIOS_TABLE_TYPE9), 0 },
  1,    // SlotDesignation String
  SlotTypeOther,          // SlotType;                 ///< The enumeration value from MISC_SLOT_TYPE.
  SlotDataBusWidthOther,  // SlotDataBusWidth;         ///< The enumeration value from MISC_SLOT_DATA_BUS_WIDTH.
  SlotUsageAvailable,    // CurrentUsage;             ///< The enumeration value from MISC_SLOT_USAGE.
  SlotLengthOther,    // SlotLength;               ///< The enumeration value from MISC_SLOT_LENGTH.
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
  0,    // SegmentGroupNum;
  0,    // BusNum;
  0,    // DevFuncNum;
};
STATIC CHAR8 CONST * CONST mSysSlotInfoType9Strings[] = {
  "SD Card",
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE16  Physical Memory ArrayInformation
************************************************************************/
STATIC SMBIOS_TABLE_TYPE16 mPhyMemArrayInfoType16 = {
  { EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, sizeof (SMBIOS_TABLE_TYPE16), 0 },
  MemoryArrayLocationSystemBoard, // Location;                       ///< The enumeration value from MEMORY_ARRAY_LOCATION.
  MemoryArrayUseSystemMemory,     // Use;                            ///< The enumeration value from MEMORY_ARRAY_USE.
  MemoryErrorCorrectionUnknown,   // MemoryErrorCorrection;          ///< The enumeration value from MEMORY_ERROR_CORRECTION.
  0x80000000,                     // MaximumCapacity;
  0xFFFE,                         // MemoryErrorInformationHandle;
  1,                              // NumberOfMemoryDevices;
  0x3fffffffffffffffULL,          // ExtendedMaximumCapacity;
};
STATIC CHAR8 CONST * CONST mPhyMemArrayInfoType16Strings[] = {
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE17  Memory Device Information
************************************************************************/
STATIC SMBIOS_TABLE_TYPE17 mMemDevInfoType17 = {
  { EFI_SMBIOS_TYPE_MEMORY_DEVICE, sizeof (SMBIOS_TABLE_TYPE17), 0 },
  0,          // MemoryArrayHandle;
  0xFFFE,     // MemoryErrorInformationHandle;
  0xFFFF,     // TotalWidth;
  0xFFFF,     // DataWidth;
  0xFFFF,     // Size;
  MemoryFormFactorUnknown, // FormFactor;                     ///< The enumeration value from MEMORY_FORM_FACTOR.
  0xff,       // DeviceSet;
  1,          // DeviceLocator String
  2,          // BankLocator String
  MemoryTypeDram,         // MemoryType;                     ///< The enumeration value from MEMORY_DEVICE_TYPE.
  {           // TypeDetail;
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
  0,          // Speed;
  3,          // Manufacturer String
  0,          // SerialNumber String
  0,          // AssetTag String
  0,          // PartNumber String
  0,          // Attributes;
  0,          // ExtendedSize;
  0,          // ConfiguredMemoryClockSpeed;
};

STATIC CHAR8 CONST *mMemDevInfoType17Strings[ 7 ] = {0};

/***********************************************************************
        SMBIOS data definition  TYPE19  Memory Array Mapped Address Information
************************************************************************/
STATIC SMBIOS_TABLE_TYPE19 mMemArrMapInfoType19 = {
  { EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS, sizeof (SMBIOS_TABLE_TYPE19), 0 },
  0x80000000, // StartingAddress;
  0xbfffffff, // EndingAddress;
  0,          // MemoryArrayHandle;
  1,          // PartitionWidth;
  0,          // ExtendedStartingAddress;
  0,          // ExtendedEndingAddress;
};
STATIC CHAR8 CONST * CONST mMemArrMapInfoType19Strings[] = {
  NULL
};

/***********************************************************************
        SMBIOS data definition  TYPE32  Boot Information
************************************************************************/
STATIC CONST SMBIOS_TABLE_TYPE32 mBootInfoType32 = {
  { EFI_SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION, sizeof (SMBIOS_TABLE_TYPE32), 0 },
  { 0, 0, 0, 0, 0, 0 },         // Reserved[6];
  BootInformationStatusNoError  // BootStatus
};

STATIC CHAR8 CONST * CONST mBootInfoType32Strings[] = {
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
  @param  StringArray Array of strings to convert to an SMBIOS string pack.
                      NULL is OK.
**/

STATIC
EFI_STATUS
EFIAPI
LogSmbiosData (
  IN  EFI_SMBIOS_TABLE_HEADER *Template,
  IN  CONST CHAR8* CONST      *StringPack
  )
{
  EFI_STATUS                Status;
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER   *Record;
  UINTN                     Index;
  UINTN                     StringSize;
  UINTN                     Size;
  CHAR8                     *Str;


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
  Record = (EFI_SMBIOS_TABLE_HEADER *)AllocateZeroPool (Size);
  if (Record == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (Record, Template, Template->Length);

  // Append string pack
  Str = ((CHAR8 *)Record) + Record->Length;
  for (Index = 0; StringPack[Index] != NULL; Index++) {
    StringSize = AsciiStrSize (StringPack[Index]);
    CopyMem (Str, StringPack[Index], StringSize);
    Str += StringSize;
  }
  *Str = 0;

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = mSmbiosProtocol->Add (
                     mSmbiosProtocol,
                     gImageHandle,
                     &SmbiosHandle,
                     Record
                     );

  ASSERT_EFI_ERROR (Status);
  FreePool (Record);
  return Status;
}

/***********************************************************************
        SMBIOS data update  TYPE0  BIOS Information
************************************************************************/
STATIC
VOID
BIOSInfoUpdateSmbiosType0 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mBIOSInfoType0, mBIOSInfoType0Strings);
}

/***********************************************************************
        SMBIOS data update  TYPE1  System Information
************************************************************************/
STATIC
VOID
SysInfoUpdateSmbiosType1 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mSysInfoType1, mSysInfoType1Strings);
}

/***********************************************************************
        SMBIOS data update  TYPE2  Board Information
************************************************************************/
STATIC
VOID
BoardInfoUpdateSmbiosType2 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mBoardInfoType2, mBoardInfoType2Strings);
}

/***********************************************************************
        SMBIOS data update  TYPE3  Enclosure Information
************************************************************************/
STATIC
VOID
EnclosureInfoUpdateSmbiosType3 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mEnclosureInfoType3, mEnclosureInfoType3Strings);
}

/***********************************************************************
        SMBIOS data update  TYPE4  Processor Information
************************************************************************/
STATIC
VOID
ProcessorInfoUpdateSmbiosType4 (
  VOID
  )
{
  ISCP_TYPE4_SMBIOS_INFO *SmbiosT4 = &mSmbiosInfo.SmbiosCpuBuffer.T4[0];

  DEBUG ((EFI_D_ERROR, "Logging SmbiosType4 from ISCP.\n"));

  mProcessorInfoType4.ProcessorType = SmbiosT4->T4ProcType;
  mProcessorInfoType4.ProcessorFamily = SmbiosT4->T4ProcFamily;
  mProcessorInfoType4.ProcessorFamily2 = SmbiosT4->T4ProcFamily2;
  mProcessorInfoType4.ProcessorCharacteristics = SmbiosT4->T4ProcCharacteristics;
  mProcessorInfoType4.MaxSpeed = SmbiosT4->T4MaxSpeed;
  mProcessorInfoType4.CurrentSpeed = SmbiosT4->T4CurrentSpeed;
  mProcessorInfoType4.CoreCount = SmbiosT4->T4CoreCount;
  mProcessorInfoType4.EnabledCoreCount = SmbiosT4->T4CoreEnabled;
  mProcessorInfoType4.ThreadCount = SmbiosT4->T4ThreadCount;
  mProcessorInfoType4.ProcessorUpgrade = SmbiosT4->T4ProcUpgrade;
  mProcessorInfoType4.Status= (UINT8)SmbiosT4->T4Status;
  mProcessorInfoType4.ExternalClock = SmbiosT4->T4ExternalClock;
  CopyMem (&mProcessorInfoType4.ProcessorId.Signature,
           &SmbiosT4->T4ProcId.ProcIDLsd, sizeof(UINT32));
  CopyMem (&mProcessorInfoType4.ProcessorId.FeatureFlags,
           &SmbiosT4->T4ProcId.ProcIDMsd, sizeof(UINT32));
  CopyMem (&mProcessorInfoType4.Voltage,
           &SmbiosT4->T4Voltage, sizeof(UINT8));

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mProcessorInfoType4, mProcessorInfoType4Strings);
}

/***********************************************************************
        SMBIOS data update  TYPE7  Cache Information
************************************************************************/
STATIC
VOID
CacheInfoUpdateSmbiosType7 (
  VOID
  )
{
  ISCP_TYPE7_SMBIOS_INFO *SmbiosT7;
  SMBIOS_TABLE_TYPE7 dstType7 = {{0}};

  DEBUG ((EFI_D_ERROR, "Logging SmbiosType7 from ISCP.\n"));

  CopyMem ((VOID *) &dstType7.Hdr, (VOID *) &mCacheInfoType7.Hdr, sizeof (SMBIOS_STRUCTURE));
  dstType7.SocketDesignation = 1;  // "L# Cache"

  // L1 cache settings
  mCacheInfoType7Strings[0] = "L1 Cache";
  SmbiosT7 = &mSmbiosInfo.SmbiosCpuBuffer.T7L1[0];
  dstType7.CacheConfiguration  = SmbiosT7->T7CacheCfg;
  dstType7.MaximumCacheSize    = SmbiosT7->T7MaxCacheSize;
  dstType7.InstalledSize       = SmbiosT7->T7InstallSize;
  CopyMem (&dstType7.SupportedSRAMType,
           &SmbiosT7->T7SupportedSramType, sizeof(UINT16));
  CopyMem (&dstType7.CurrentSRAMType,
           &SmbiosT7->T7CurrentSramType, sizeof(UINT16));
  dstType7.CacheSpeed          = SmbiosT7->T7CacheSpeed;
  dstType7.ErrorCorrectionType = SmbiosT7->T7ErrorCorrectionType;
  dstType7.SystemCacheType     = SmbiosT7->T7SystemCacheType;
  dstType7.Associativity       = SmbiosT7->T7Associativity;
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&dstType7, mCacheInfoType7Strings);

  // L2 cache settings
  mCacheInfoType7Strings[0] = "L2 Cache";
  SmbiosT7 = &mSmbiosInfo.SmbiosCpuBuffer.T7L2[0];
  dstType7.CacheConfiguration  = SmbiosT7->T7CacheCfg;
  dstType7.MaximumCacheSize    = SmbiosT7->T7MaxCacheSize;
  dstType7.InstalledSize       = SmbiosT7->T7InstallSize;
  CopyMem (&dstType7.SupportedSRAMType,
           &SmbiosT7->T7SupportedSramType, sizeof(UINT16));
  CopyMem (&dstType7.CurrentSRAMType,
           &SmbiosT7->T7CurrentSramType, sizeof(UINT16));
  dstType7.CacheSpeed          = SmbiosT7->T7CacheSpeed;
  dstType7.ErrorCorrectionType = SmbiosT7->T7ErrorCorrectionType;
  dstType7.SystemCacheType     = SmbiosT7->T7SystemCacheType;
  dstType7.Associativity       = SmbiosT7->T7Associativity;
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&dstType7, mCacheInfoType7Strings);

  // L3 cache settings
  mCacheInfoType7Strings[0] = "L3 Cache";
  SmbiosT7 = &mSmbiosInfo.SmbiosCpuBuffer.T7L3[0];
  dstType7.CacheConfiguration  = SmbiosT7->T7CacheCfg;
  dstType7.MaximumCacheSize    = SmbiosT7->T7MaxCacheSize;
  dstType7.InstalledSize       = SmbiosT7->T7InstallSize;
  CopyMem (&dstType7.SupportedSRAMType,
           &SmbiosT7->T7SupportedSramType, sizeof(UINT16));
  CopyMem (&dstType7.CurrentSRAMType,
           &SmbiosT7->T7CurrentSramType, sizeof(UINT16));
  dstType7.CacheSpeed          = SmbiosT7->T7CacheSpeed;
  dstType7.ErrorCorrectionType = SmbiosT7->T7ErrorCorrectionType;
  dstType7.SystemCacheType     = SmbiosT7->T7SystemCacheType;
  dstType7.Associativity       = SmbiosT7->T7Associativity;
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&dstType7, mCacheInfoType7Strings);
}

/***********************************************************************
        SMBIOS data update  TYPE9  System Slot Information
************************************************************************/
STATIC
VOID
SysSlotInfoUpdateSmbiosType9 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mSysSlotInfoType9, mSysSlotInfoType9Strings);
}

/***********************************************************************
        SMBIOS data update  TYPE16  Physical Memory Array Information
************************************************************************/
STATIC
VOID
PhyMemArrayInfoUpdateSmbiosType16 (
  VOID
  )
{
  ISCP_TYPE16_SMBIOS_INFO *SmbiosT16 = &mSmbiosInfo.SmbiosMemBuffer.T16;

  DEBUG ((EFI_D_ERROR, "Logging SmbiosType16 from ISCP.\n"));

  mPhyMemArrayInfoType16.Location = SmbiosT16->Location;
  mPhyMemArrayInfoType16.Use = SmbiosT16->Use;
  mPhyMemArrayInfoType16.MemoryErrorCorrection = SmbiosT16->MemoryErrorCorrection;
  mPhyMemArrayInfoType16.NumberOfMemoryDevices = SmbiosT16->NumberOfMemoryDevices;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mPhyMemArrayInfoType16, mPhyMemArrayInfoType16Strings);
}

/***********************************************************************
        SMBIOS data update  TYPE17  Memory Device Information
************************************************************************/
STATIC
VOID
MemDevInfoUpdatedstType17 (
  VOID
  )
{
  SMBIOS_TABLE_TYPE17 dstType17 = {{0}};
  ISCP_TYPE17_SMBIOS_INFO *srcType17;
  UINTN i, j, StrIndex, LastIndex;

  DEBUG ((EFI_D_ERROR, "Logging SmbiosType17 from ISCP.\n"));

  LastIndex = (sizeof(mMemDevInfoType17Strings) / sizeof (CHAR8 *)) - 1;
  for (i = 0; i < 2; ++i) {
    for (j = 0; j < 2; ++j) {
      srcType17 = &mSmbiosInfo.SmbiosMemBuffer.T17[i][j];

      CopyMem ((VOID *) &dstType17.Hdr, (VOID *) &mMemDevInfoType17.Hdr, sizeof (SMBIOS_STRUCTURE));
      dstType17.MemoryArrayHandle = srcType17->Handle;
      dstType17.TotalWidth = srcType17->TotalWidth;
      dstType17.DataWidth = srcType17->DataWidth;
      dstType17.Size = srcType17->MemorySize;
      dstType17.FormFactor = srcType17->FormFactor;
      dstType17.DeviceSet = srcType17->DeviceSet;
      dstType17.MemoryType = srcType17->MemoryType;

      CopyMem ((VOID *) &dstType17.TypeDetail, (VOID *) &mMemDevInfoType17.TypeDetail, sizeof (UINT16));
      dstType17.Speed = srcType17->Speed;
      dstType17.Attributes = srcType17->Attributes;
      dstType17.ExtendedSize = srcType17->ExtSize;
      dstType17.ConfiguredMemoryClockSpeed = srcType17->ConfigSpeed;

      // Build table of TYPE17 strings
      StrIndex = 0;

      if (AsciiStrLen ((CHAR8 *)srcType17->DeviceLocator) && StrIndex < LastIndex) {
        mMemDevInfoType17Strings[StrIndex++] = (CHAR8 *)srcType17->DeviceLocator;
        dstType17.DeviceLocator = (SMBIOS_TABLE_STRING) StrIndex;
      } else {
        dstType17.DeviceLocator = 0;
      }

      if (AsciiStrLen ((CHAR8 *)srcType17->BankLocator) && StrIndex < LastIndex) {
        mMemDevInfoType17Strings[StrIndex++] = (CHAR8 *)srcType17->BankLocator;
        dstType17.BankLocator = (SMBIOS_TABLE_STRING) StrIndex;
      } else {
        dstType17.BankLocator = 0;
      }

      if (AsciiStrLen ((CHAR8 *)srcType17->ManufacturerIdCode) && StrIndex < LastIndex) {
        mMemDevInfoType17Strings[StrIndex++] = (CHAR8 *)srcType17->ManufacturerIdCode;
        dstType17.Manufacturer = (SMBIOS_TABLE_STRING) StrIndex;
      } else {
        dstType17.Manufacturer = 0;
      }

      if (AsciiStrLen ((CHAR8 *)srcType17->SerialNumber) && StrIndex < LastIndex) {
        mMemDevInfoType17Strings[StrIndex++] = (CHAR8 *)srcType17->SerialNumber;
        dstType17.SerialNumber = (SMBIOS_TABLE_STRING) StrIndex;
      } else {
        dstType17.SerialNumber = 0;
      }

      if (AsciiStrLen ((CHAR8 *)srcType17->PartNumber) && StrIndex < LastIndex) {
        mMemDevInfoType17Strings[StrIndex++] = (CHAR8 *)srcType17->PartNumber;
        dstType17.PartNumber = (SMBIOS_TABLE_STRING) StrIndex;
      } else {
        dstType17.PartNumber = 0;
      }

      mMemDevInfoType17Strings[StrIndex] = NULL;
      LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&dstType17, mMemDevInfoType17Strings);
    }
  }
}

/***********************************************************************
        SMBIOS data update  TYPE19  Memory Array Map Information
************************************************************************/
STATIC
VOID
MemArrMapInfoUpdateSmbiosType19 (
  VOID
  )
{
  ISCP_TYPE19_SMBIOS_INFO *SmbiosT19 = &mSmbiosInfo.SmbiosMemBuffer.T19;

  DEBUG ((EFI_D_ERROR, "Logging SmbiosType19 from ISCP.\n"));

  mMemArrMapInfoType19.StartingAddress = SmbiosT19->StartingAddr;
  mMemArrMapInfoType19.EndingAddress = SmbiosT19->EndingAddr;
  mMemArrMapInfoType19.MemoryArrayHandle = SmbiosT19->MemoryArrayHandle;
  mMemArrMapInfoType19.PartitionWidth = SmbiosT19->PartitionWidth;
  mMemArrMapInfoType19.ExtendedStartingAddress = SmbiosT19->ExtStartingAddr;
  mMemArrMapInfoType19.ExtendedEndingAddress = SmbiosT19->ExtEndingAddr;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mMemArrMapInfoType19, mMemArrMapInfoType19Strings);
}


/***********************************************************************
        SMBIOS data update  TYPE32  Boot Information
************************************************************************/
STATIC
VOID
BootInfoUpdateSmbiosType32 (
  VOID
  )
{
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mBootInfoType32, mBootInfoType32Strings);
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
  EFI_STATUS Status;

  DEBUG ((EFI_D_ERROR, "PlatformSmbiosDxe Loaded\n"));

  //
  // Locate Smbios protocol
  //
  Status = gBS->LocateProtocol (
               &gEfiSmbiosProtocolGuid,
               NULL,
               (VOID **)&mSmbiosProtocol
               );

  if (EFI_ERROR (Status)) {
    mSmbiosProtocol = NULL;
    DEBUG ((EFI_D_ERROR, "Failed to Locate SMBIOS Protocol"));
    return Status;
  }

  Status = gBS->LocateProtocol (
               &gAmdIscpDxeProtocolGuid,
               NULL,
               (VOID **)&mIscpDxeProtocol
               );
  if (EFI_ERROR (Status)) {
    mIscpDxeProtocol = NULL;
    DEBUG ((EFI_D_ERROR, "Failed to Locate ISCP DXE Protocol"));
    return Status;
  }

  Status = mIscpDxeProtocol-> AmdExecuteSmbiosInfoDxe (
                    mIscpDxeProtocol,
                    &mSmbiosInfo
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get SMBIOS data via ISCP"));
    return Status;
  }

  BIOSInfoUpdateSmbiosType0();

  SysInfoUpdateSmbiosType1();

  BoardInfoUpdateSmbiosType2();

  EnclosureInfoUpdateSmbiosType3();

  ProcessorInfoUpdateSmbiosType4();

  CacheInfoUpdateSmbiosType7();

  SysSlotInfoUpdateSmbiosType9();

  PhyMemArrayInfoUpdateSmbiosType16();

  MemDevInfoUpdatedstType17();

  MemArrMapInfoUpdateSmbiosType19();

  BootInfoUpdateSmbiosType32();

  return Status;
}
