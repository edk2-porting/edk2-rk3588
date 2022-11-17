/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "ProcessorSubClass.h"

EFI_HII_HANDLE                  mHiiHandle;

EFI_SMBIOS_PROTOCOL             *mSmbios;

SMBIOS_TABLE_TYPE7   mSmbiosCacheTable[] = {
    //L1 Instruction Cache
    {
        {                                               //Header
            EFI_SMBIOS_TYPE_CACHE_INFORMATION,              //Type
            sizeof(SMBIOS_TABLE_TYPE7),                     //Length
            0                                               //Handle
        },
        1,                                              //SocketDesignation
        0,                                              //CacheConfiguration
        0,                                              //MaximumCacheSize
        48,                                             //InstalledSize
        {                                               //SupportedSRAMType
            0
        },
        {                                               //CurrentSRAMType
            0
        },
        0,                                              //CacheSpeed
        CacheErrorParity,                               //ErrorCorrectionType
        CacheTypeInstruction,                           //SystemCacheType
        CacheAssociativity8Way                          //Associativity
    },

    //L1 Data Cache
    {
        {                                               //Header
            EFI_SMBIOS_TYPE_CACHE_INFORMATION,              //Type
            sizeof(SMBIOS_TABLE_TYPE7),                     //Length
            0                                               //Handle
        },
        1,                                              //SocketDesignation
        0,                                              //CacheConfiguration
        0,                                              //MaximumCacheSize
        32,                                              //InstalledSize
        {                                               //SupportedSRAMType
            0
        },
        {                                               //CurrentSRAMType
            0
        },
        0,                                              //CacheSpeed
        CacheErrorSingleBit,                            //ErrorCorrectionType
        CacheTypeData,                                  //SystemCacheType
        CacheAssociativity8Way                          //Associativity
    },

    //L2 Cache
    {
        {                                               //Header
            EFI_SMBIOS_TYPE_CACHE_INFORMATION,              //Type
            sizeof(SMBIOS_TABLE_TYPE7),                     //Length
            0                                               //Handle
        },
        1,                                              //SocketDesignation
        0,                                              //CacheConfiguration
        0,                                              //MaximumCacheSize
        4096,                                           //InstalledSize
        {                                               //SupportedSRAMType
            0
        },
        {                                               //CurrentSRAMType
            0
        },
        0,                                              //CacheSpeed
        CacheErrorSingleBit,                            //ErrorCorrectionType
        CacheTypeUnified,                               //SystemCacheType
        CacheAssociativity8Way                          //Associativity
    },

    //L3 Cache
    {
        {                                               //Header
            EFI_SMBIOS_TYPE_CACHE_INFORMATION,              //Type
            sizeof(SMBIOS_TABLE_TYPE7),                     //Length
            0                                               //Handle
        },
        1,                                              //SocketDesignation
        0,                                              //CacheConfiguration
        0,                                              //MaximumCacheSize
        16384,                                          //InstalledSize
        {                                               //SupportedSRAMType
            0
        },
        {                                               //CurrentSRAMType
            0
        },
        0,                                              //CacheSpeed
        CacheErrorSingleBit,                            //ErrorCorrectionType
        CacheTypeUnified,                               //SystemCacheType
        CacheAssociativity16Way                         //Associativity
    }
};

SMBIOS_TABLE_TYPE4   mSmbiosProcessorTable[] = {
    //CPU0
    {
        {                                               //Header
            EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION,          //Type
            sizeof(SMBIOS_TABLE_TYPE4),                     //Length
            0                                               //Handle
        },
        1,                                              //Socket
        CentralProcessor,                               //ProcessorType
        ProcessorFamilyIndicatorFamily2,                //ProcessorFamily
        2,                                              //ProcessorManufacture
        {                                               //ProcessorId
            {                                               //Signature
                0
            },
            {                                               //FeatureFlags
                0
            }
        },
        3,                                              //ProcessorVersion
        {                                               //Voltage
            0
        },
        EXTERNAL_CLOCK,                                 //ExternalClock
        CPU_MAX_SPEED,                                  //MaxSpeed
        0,                                              //CurrentSpeed
        0,                                              //Status
        ProcessorUpgradeUnknown,                        //ProcessorUpgrade
        0xFFFF,                                         //L1CacheHandle
        0xFFFF,                                         //L2CacheHandle
        0xFFFF,                                         //L3CacheHandle
        4,                                              //SerialNumber
        5,                                              //AssetTag
        6,                                              //PartNumber

        0,                                              //CoreCount
        0,                                              //EnabledCoreCount
        0,                                              //ThreadCount
        0,                                              //ProcessorCharacteristics

        ProcessorFamilyARM,                             //ProcessorFamily2

        0,                                              //CoreCount2
        0,                                              //EnabledCoreCount2
        0                                               //ThreadCount2
    },

    //CPU1
    {
        {                                               //Header
            EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION,          //Type
            sizeof(SMBIOS_TABLE_TYPE4),                     //Length
            0                                               //Handle
        },
        1,                                              //Socket
        CentralProcessor,                               //ProcessorType
        ProcessorFamilyIndicatorFamily2,                //ProcessorFamily
        2,                                              //ProcessorManufacture
        {                                               //ProcessorId
            {                                               //Signature
                0
            },
            {                                               //FeatureFlags
                0
            }
        },
        3,                                              //ProcessorVersion
        {                                               //Voltage
            0
        },
        EXTERNAL_CLOCK,                                 //ExternalClock
        CPU_MAX_SPEED,                                  //MaxSpeed
        0,                                              //CurrentSpeed
        0,                                              //Status
        ProcessorUpgradeUnknown,                        //ProcessorUpgrade
        0xFFFF,                                         //L1CacheHandle
        0xFFFF,                                         //L2CacheHandle
        0xFFFF,                                         //L3CacheHandle
        4,                                              //SerialNumber
        5,                                              //AssetTag
        6,                                              //PartNumber

        0,                                              //CoreCount
        0,                                              //EnabledCoreCount
        0,                                              //ThreadCount
        0,                                              //ProcessorCharacteristics

        ProcessorFamilyARM,                             //ProcessorFamily2

        0,                                              //CoreCount2
        0,                                              //EnabledCoreCount2
        0                                               //ThreadCount2
    }
};


UINT16
GetCpuFrequency (
  IN  UINT8 ProcessorNumber
)
{
    return (UINT16)(PlatformGetCpuFreq(ProcessorNumber)/1000/1000);
}

UINTN
GetCacheSocketStr (
  IN  UINT8     CacheLevel,
  OUT CHAR16    *CacheSocketStr
  )
{
    UINTN CacheSocketStrLen;

    if(CacheLevel == CPU_CACHE_L1_Instruction)
    {
        CacheSocketStrLen = UnicodeSPrint (CacheSocketStr, SMBIOS_STRING_MAX_LENGTH - 1, L"L%x Instruction Cache", CacheLevel + 1);
    }
    else if(CacheLevel == CPU_CACHE_L1_Data)
    {
        CacheSocketStrLen = UnicodeSPrint (CacheSocketStr, SMBIOS_STRING_MAX_LENGTH - 1, L"L%x Data Cache", CacheLevel);
    }
    else
    {
        CacheSocketStrLen = UnicodeSPrint (CacheSocketStr, SMBIOS_STRING_MAX_LENGTH - 1, L"L%x Cache", CacheLevel);
    }

    return CacheSocketStrLen;
}

VOID
UpdateSmbiosCacheTable (
  IN UINT8  CacheLevel
  )
{
    UINT16                      CoreCount;
    UINT32                      TotalSize;
    UINT32                      CacheSize;
    UINT16                      MaximumCacheSize;
    UINT16                      InstalledSize;
    CACHE_CONFIGURATION         CacheConfig;
    CACHE_SRAM_TYPE_DATA        CacheSramType = {0};

    CoreCount = 16;     // Default value is 16 Core

    //
    // Set Cache Configuration
    //
    CacheConfig.Bits.Socketed                = 0;    // Not Socketed
    CacheConfig.Bits.Reserved1               = 0;    //
    CacheConfig.Bits.Location                = 0;    // Internal
    CacheConfig.Bits.Enable                  = 1;    // Enabled
    CacheConfig.Bits.Reserved2               = 0;
    if(CacheLevel == CPU_CACHE_L1_Instruction || CacheLevel == CPU_CACHE_L1_Data)
    {
        CacheConfig.Bits.Level               = 0;
        CacheConfig.Bits.OperationalMode     = 1;        // Write Back
    }
    else
    {
        CacheConfig.Bits.Level               = CacheLevel - 1;
        CacheConfig.Bits.OperationalMode     = 2;        // Varies with Memory Address
    }

    mSmbiosCacheTable[CacheLevel].CacheConfiguration = CacheConfig.Data;

    //
    // Set Cache Size
    //
    CacheSize = mSmbiosCacheTable[CacheLevel].InstalledSize;
    if (PACKAGE_16CORE != PlatformGetPackageType())  // 32 Core
    {
        CoreCount = CoreCount * 2;

        if (CacheLevel > 1)
        {
            CacheSize = CacheSize * 2;
        }
    }

    if(CacheLevel <= 1)
    {
        TotalSize = CacheSize * CoreCount;
    }
    else
    {
        TotalSize = CacheSize;
    }

    if((TotalSize >> 15) == 0)  // 1K granularity
    {
        MaximumCacheSize   = (UINT16)TotalSize;
        InstalledSize      = (UINT16)TotalSize;
    }
    else    // 64K granularity
    {
        MaximumCacheSize   = (UINT16)(TotalSize >> 6);
        InstalledSize      = (UINT16)(TotalSize >> 6);

        // Set BIT15 to 1
        MaximumCacheSize   |= BIT15;
        InstalledSize      |= BIT15;
    }

    mSmbiosCacheTable[CacheLevel].MaximumCacheSize = MaximumCacheSize;
    mSmbiosCacheTable[CacheLevel].InstalledSize = InstalledSize;

    //
    // Set SRAM Type
    //
    CacheSramType.Synchronous = 1;
    (VOID)CopyMem(&mSmbiosCacheTable[CacheLevel].SupportedSRAMType, &CacheSramType, sizeof(CACHE_SRAM_TYPE_DATA));
    (VOID)CopyMem(&mSmbiosCacheTable[CacheLevel].CurrentSRAMType, &CacheSramType, sizeof(CACHE_SRAM_TYPE_DATA));
}

/**
  Add Type 7 SMBIOS Record for Cache Information.

  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[out]   L1CacheHandle       Pointer to the handle of the L1 Cache SMBIOS record.
  @param[out]   L2CacheHandle       Pointer to the handle of the L2 Cache SMBIOS record.
  @param[out]   L3CacheHandle       Pointer to the handle of the L3 Cache SMBIOS record.

**/
EFI_STATUS
AddSmbiosCacheTypeTable (
  IN UINTN                  ProcessorNumber,
  OUT EFI_SMBIOS_HANDLE     *L1CacheHandle,
  OUT EFI_SMBIOS_HANDLE     *L2CacheHandle,
  OUT EFI_SMBIOS_HANDLE     *L3CacheHandle
  )
{
    EFI_STATUS                  Status;
    SMBIOS_TABLE_TYPE7          *Type7Record;
    EFI_SMBIOS_HANDLE           SmbiosHandle;
    UINTN                       TableSize;
    UINT8                       CacheLevel;
    CHAR8                       *OptionalStrStart;
    EFI_STRING                  CacheSocketStr;
    UINTN                       CacheSocketStrLen;
    UINTN                       StringBufferSize;

    Status = EFI_SUCCESS;

    //
    // Get Cache information
    //
    for(CacheLevel = 0; CacheLevel < MAX_CACHE_LEVEL; CacheLevel++)
    {
        Type7Record = NULL;

        if(mSmbiosCacheTable[CacheLevel].InstalledSize == 0)
        {
            continue;
        }

        //
        // Update Cache information
        //
        if (mSmbiosCacheTable[CacheLevel].MaximumCacheSize == 0)
        {
            UpdateSmbiosCacheTable (CacheLevel);
        }

        StringBufferSize = sizeof(CHAR16) * SMBIOS_STRING_MAX_LENGTH;
        CacheSocketStr = AllocateZeroPool(StringBufferSize);
        if (CacheSocketStr == NULL)
        {
            Status = EFI_OUT_OF_RESOURCES;
            goto Exit;
        }

        CacheSocketStrLen = GetCacheSocketStr (CacheLevel, CacheSocketStr);

        TableSize = sizeof(SMBIOS_TABLE_TYPE7) + CacheSocketStrLen + 1 + 1;
        Type7Record = AllocateZeroPool (TableSize);
        if (Type7Record == NULL)
        {
            Status = EFI_OUT_OF_RESOURCES;
            goto Exit;
        }

        (VOID)CopyMem(Type7Record, &mSmbiosCacheTable[CacheLevel], sizeof (SMBIOS_TABLE_TYPE7));

        OptionalStrStart = (CHAR8 *) (Type7Record + 1);
        Status = UnicodeStrToAsciiStrS (CacheSocketStr, OptionalStrStart, CacheSocketStrLen + 1);
        ASSERT_EFI_ERROR (Status);

        SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
        Status = mSmbios->Add (mSmbios, NULL, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *)Type7Record);
        if (EFI_ERROR (Status))
        {
            goto Exit;
        }

        // Config L1/L2/L3 Cache Handle
        switch(CacheLevel)
        {
            case CPU_CACHE_L1_Instruction:
            case CPU_CACHE_L1_Data:
                *L1CacheHandle = SmbiosHandle;
                break;
            case CPU_CACHE_L2:
                *L2CacheHandle = SmbiosHandle;
                break;
            case CPU_CACHE_L3:
                *L3CacheHandle = SmbiosHandle;
                break;
            default :
                break;
        }
Exit:
        if(Type7Record != NULL)
        {
            FreePool (Type7Record);
        }
        if(CacheSocketStr != NULL)
        {
            FreePool (CacheSocketStr);
            CacheSocketStr = NULL;
        }
    }

    return Status;
}

/**
  Add Type 4 SMBIOS Record for Processor Information.

  @param[in]    ProcessorNumber     Processor number of specified processor.

**/
EFI_STATUS
AddSmbiosProcessorTypeTable (
  IN UINTN                  ProcessorNumber
  )
{
    EFI_STATUS                  Status;
    SMBIOS_TABLE_TYPE4          *Type4Record;
    EFI_SMBIOS_HANDLE           SmbiosHandle;
    EFI_SMBIOS_HANDLE           L1CacheHandle;
    EFI_SMBIOS_HANDLE           L2CacheHandle;
    EFI_SMBIOS_HANDLE           L3CacheHandle;

    CHAR8                       *OptionalStrStart;
    EFI_STRING_ID               ProcessorManu;
    EFI_STRING_ID               ProcessorVersion;
    EFI_STRING_ID               SerialNumber;
    EFI_STRING_ID               AssetTag;
    EFI_STRING_ID               PartNumber;
    EFI_STRING                  ProcessorSocketStr;
    EFI_STRING                  ProcessorManuStr;
    EFI_STRING                  ProcessorVersionStr;
    EFI_STRING                  SerialNumberStr;
    EFI_STRING                  AssetTagStr;
    EFI_STRING                  PartNumberStr;
    UINTN                       ProcessorSocketStrLen;
    UINTN                       ProcessorManuStrLen;
    UINTN                       ProcessorVersionStrLen;
    UINTN                       SerialNumberStrLen;
    UINTN                       AssetTagStrLen;
    UINTN                       PartNumberStrLen;
    UINTN                       StringBufferSize;
    UINTN                       TotalSize;

    UINT8                       Voltage;
    UINT16                      CoreCount;
    UINT16                      CoreEnabled;
    UINT16                      ThreadCount;
    UINT16                      CurrentSpeed;
    PROCESSOR_STATUS_DATA       ProcessorStatus = {{0}};
    PROCESSOR_CHARACTERISTICS_DATA  ProcessorCharacteristics = {{0}};

    CHAR16                      *CpuVersion;
    EFI_STRING_ID               TokenToUpdate;

    UINT64                      *ProcessorId;
    Type4Record         = NULL;
    ProcessorManuStr    = NULL;
    ProcessorVersionStr = NULL;
    SerialNumberStr     = NULL;
    AssetTagStr         = NULL;
    PartNumberStr       = NULL;

    if(OemIsSocketPresent(ProcessorNumber))  //CPU is present
    {
        Voltage         = BIT7 | 9;          // 0.9V

        Status = AddSmbiosCacheTypeTable (ProcessorNumber, &L1CacheHandle, &L2CacheHandle, &L3CacheHandle);
        if(EFI_ERROR(Status))
        {
            return Status;
        }

        CurrentSpeed        = GetCpuFrequency(ProcessorNumber);

        CoreCount       = PlatformGetCoreCount();
        CoreEnabled     = CoreCount;
        ThreadCount     = CoreCount;

        CpuVersion = (CHAR16 *) PcdGetPtr (PcdCPUInfo);
        if (StrLen(CpuVersion) > 0)
        {
            TokenToUpdate = STRING_TOKEN (STR_PROCESSOR_VERSION);
            HiiSetString (mHiiHandle, TokenToUpdate, CpuVersion, NULL);
        }

        ProcessorManu       = STRING_TOKEN (STR_PROCESSOR_MANUFACTURE);
        ProcessorVersion    = STRING_TOKEN (STR_PROCESSOR_VERSION);
        SerialNumber        = STRING_TOKEN (STR_PROCESSOR_SERIAL_NUMBER);
        AssetTag            = STRING_TOKEN (STR_PROCESSOR_ASSET_TAG);
        PartNumber          = STRING_TOKEN (STR_PROCESSOR_PART_NUMBER);

        // Processor Status
        ProcessorStatus.Bits.CpuStatus           = 1;        // CPU Enabled
        ProcessorStatus.Bits.Reserved1           = 0;
        ProcessorStatus.Bits.SocketPopulated     = 1;        // CPU Socket Populated
        ProcessorStatus.Bits.Reserved2           = 0;

        // Processor Characteristics
        ProcessorCharacteristics.Bits.Reserved       = 0;
        ProcessorCharacteristics.Bits.Capable64Bit   = 1;        // 64-bit Capable
        ProcessorCharacteristics.Bits.Unknown        = 0;
        ProcessorCharacteristics.Bits.EnhancedVirtualization     = 1;
        ProcessorCharacteristics.Bits.HardwareThread = 0;
        ProcessorCharacteristics.Bits.MultiCore      = 1;
        ProcessorCharacteristics.Bits.ExecuteProtection          = 1;
        ProcessorCharacteristics.Bits.PowerPerformanceControl    = 1;
        ProcessorCharacteristics.Bits.Reserved2      = 0;
    }
    else
    {
        Voltage             = 0;
        CurrentSpeed        = 0;
        CoreCount           = 0;
        CoreEnabled         = 0;
        ThreadCount         = 0;
        L1CacheHandle       = 0xFFFF;
        L2CacheHandle       = 0xFFFF;
        L3CacheHandle       = 0xFFFF;

        ProcessorManu       = STRING_TOKEN (STR_PROCESSOR_UNKNOWN);
        ProcessorVersion    = STRING_TOKEN (STR_PROCESSOR_UNKNOWN);
        SerialNumber        = STRING_TOKEN (STR_PROCESSOR_UNKNOWN);
        AssetTag            = STRING_TOKEN (STR_PROCESSOR_UNKNOWN);
        PartNumber          = STRING_TOKEN (STR_PROCESSOR_UNKNOWN);
    }

    // Processor Socket Designation
    StringBufferSize = sizeof(CHAR16) * SMBIOS_STRING_MAX_LENGTH;
    ProcessorSocketStr = AllocateZeroPool(StringBufferSize);
    if (ProcessorSocketStr == NULL)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    ProcessorSocketStrLen = UnicodeSPrint (ProcessorSocketStr, StringBufferSize, L"CPU%02d", ProcessorNumber + 1);

    // Processor Manufacture
    ProcessorManuStr = HiiGetPackageString (&gEfiCallerIdGuid, ProcessorManu, NULL);
    ProcessorManuStrLen = StrLen (ProcessorManuStr);

    // Processor Version
    ProcessorVersionStr = HiiGetPackageString (&gEfiCallerIdGuid, ProcessorVersion, NULL);
    ProcessorVersionStrLen = StrLen (ProcessorVersionStr);

    // Serial Number
    SerialNumberStr = HiiGetPackageString (&gEfiCallerIdGuid, SerialNumber, NULL);
    SerialNumberStrLen = StrLen (SerialNumberStr);

    // Asset Tag
    AssetTagStr = HiiGetPackageString (&gEfiCallerIdGuid, AssetTag, NULL);
    AssetTagStrLen = StrLen (AssetTagStr);

    // Part Number
    PartNumberStr = HiiGetPackageString (&gEfiCallerIdGuid, PartNumber, NULL);
    PartNumberStrLen = StrLen (PartNumberStr);

    TotalSize = sizeof (SMBIOS_TABLE_TYPE4) + ProcessorSocketStrLen + 1 + ProcessorManuStrLen + 1 + ProcessorVersionStrLen + 1 + SerialNumberStrLen + 1 + AssetTagStrLen + 1 + PartNumberStrLen + 1 + 1;
    Type4Record = AllocateZeroPool (TotalSize);
    if (Type4Record == NULL)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    (VOID)CopyMem(Type4Record, &mSmbiosProcessorTable[ProcessorNumber], sizeof (SMBIOS_TABLE_TYPE4));

    *(UINT8 *) &Type4Record->Voltage        = Voltage;
    Type4Record->CurrentSpeed               = CurrentSpeed;
    Type4Record->Status                     = ProcessorStatus.Data;
    Type4Record->L1CacheHandle              = L1CacheHandle;
    Type4Record->L2CacheHandle              = L2CacheHandle;
    Type4Record->L3CacheHandle              = L3CacheHandle;
    Type4Record->CoreCount                  = CoreCount;
    Type4Record->EnabledCoreCount           = CoreEnabled;
    Type4Record->ThreadCount                = ThreadCount;
    Type4Record->ProcessorCharacteristics   = ProcessorCharacteristics.Data;

    Type4Record->ExternalClock              = (UINT16)(ArmReadCntFrq() / 1000 / 1000);
    ProcessorId = (UINT64 *)&(Type4Record->ProcessorId);
    *ProcessorId = ArmReadMidr();

    OptionalStrStart = (CHAR8 *) (Type4Record + 1);
    Status = UnicodeStrToAsciiStrS (ProcessorSocketStr, OptionalStrStart, ProcessorSocketStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (ProcessorManuStr, OptionalStrStart + ProcessorSocketStrLen + 1, ProcessorManuStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (ProcessorVersionStr, OptionalStrStart + ProcessorSocketStrLen + 1 + ProcessorManuStrLen + 1, ProcessorVersionStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (SerialNumberStr, OptionalStrStart + ProcessorSocketStrLen + 1 + ProcessorManuStrLen + 1 + ProcessorVersionStrLen + 1, SerialNumberStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (AssetTagStr, OptionalStrStart + ProcessorSocketStrLen + 1 + ProcessorManuStrLen + 1 + ProcessorVersionStrLen + 1 + SerialNumberStrLen + 1, AssetTagStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (PartNumberStr, OptionalStrStart + ProcessorSocketStrLen + 1 + ProcessorManuStrLen + 1 + ProcessorVersionStrLen + 1 + SerialNumberStrLen + 1 + AssetTagStrLen + 1, PartNumberStrLen + 1);
    ASSERT_EFI_ERROR (Status);

    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = mSmbios->Add (mSmbios, NULL, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *)Type4Record);
    if (EFI_ERROR (Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Smbios Type04 Table Log Failed! %r \n", __FUNCTION__, __LINE__, Status));
    }
    FreePool (Type4Record);

Exit:
    if(ProcessorSocketStr != NULL)
    {
        FreePool (ProcessorSocketStr);
    }
    if(ProcessorManuStr != NULL)
    {
        FreePool (ProcessorManuStr);
    }
    if(ProcessorVersionStr != NULL)
    {
        FreePool (ProcessorVersionStr);
    }
    if(SerialNumberStr != NULL)
    {
        FreePool (SerialNumberStr);
    }
    if(AssetTagStr != NULL)
    {
        FreePool (AssetTagStr);
    }
    if(PartNumberStr != NULL)
    {
        FreePool (PartNumberStr);
    }

    return Status;
}

/**
  Standard EFI driver point.  This driver locates the ProcessorConfigurationData Variable,
  if it exists, add the related SMBIOS tables by PI SMBIOS protocol.

  @param  ImageHandle     Handle for the image of this driver
  @param  SystemTable     Pointer to the EFI System Table

  @retval  EFI_SUCCESS    The data was successfully stored.

**/
EFI_STATUS
EFIAPI
ProcessorSubClassEntryPoint(
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
    EFI_STATUS                      Status;
    UINT32                          SocketIndex;

    //
    // Locate dependent protocols
    //
    Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID**)&mSmbios);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "Could not locate SMBIOS protocol.  %r\n", Status));
        return Status;
    }

    //
    // Add our default strings to the HII database. They will be modified later.
    //
    mHiiHandle = HiiAddPackages (
                &gEfiCallerIdGuid,
                NULL,
                ProcessorSubClassStrings,
                NULL,
                NULL
                );
    if (mHiiHandle == NULL)
    {
        return EFI_OUT_OF_RESOURCES;
    }

    //
    // Add SMBIOS tables for populated sockets.
    //
    for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++)
    {
        if((SocketIndex == 1) && !OemIsMpBoot())
        {
            break;
        }
        Status = AddSmbiosProcessorTypeTable (SocketIndex);
        if(EFI_ERROR(Status))
        {
            DEBUG((EFI_D_ERROR, "Add Processor Type Table Failed!  %r.\n", Status));
            return Status;
        }
    }

    return Status;
}
