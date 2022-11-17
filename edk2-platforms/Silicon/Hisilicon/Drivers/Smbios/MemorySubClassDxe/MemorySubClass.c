/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#include "MemorySubClass.h"

EFI_SMBIOS_PROTOCOL             *mSmbios = NULL;
EFI_HII_HANDLE                  mHiiHandle;

UINT8                           mMaxSkt;
UINT8                           mMaxCh;

VOID
SmbiosGetManufacturer (
  IN  UINT8           MfgIdLSB,
  IN  UINT8           MfgIdMSB,
  OUT CHAR16          *Manufacturer
)
{
    UINT32                  Index = 0;

    (VOID)StrCpyS(Manufacturer, SMBIOS_STRING_MAX_LENGTH - 1, L"Unknown");
    while (JEP106[Index].MfgIdLSB != 0xFF && JEP106[Index].MfgIdMSB != 0xFF )
    {
        if (JEP106[Index].MfgIdLSB == MfgIdLSB && JEP106[Index].MfgIdMSB == MfgIdMSB)
        {
            (VOID)StrCpyS (Manufacturer, SMBIOS_STRING_MAX_LENGTH - 1, JEP106[Index].Name);
            break;
        }
        Index++;
    }
}

VOID
SmbiosGetPartNumber (
  IN GBL_INTERFACE      *pGblData,
  IN UINT8              Skt,
  IN UINT8              Ch,
  IN UINT8              Dimm,
  OUT CHAR16             *PartNumber
  )
{
    CHAR16                        StringBuffer2[SMBIOS_STRING_MAX_LENGTH];
    UINT32                        Index2;

    (VOID)StrCpyS(PartNumber, SMBIOS_STRING_MAX_LENGTH - 1, L"");
    if (pGblData->Channel[Skt][Ch].Dimm[Dimm].DramType == SPD_TYPE_DDR3)
    {
        for (Index2 = 0; Index2 < SPD_MODULE_PART; Index2++)
        {
            UnicodeSPrint (StringBuffer2, SMBIOS_STRING_MAX_LENGTH - 1, L"%c", pGblData->Channel[Skt][Ch].Dimm[Dimm].SpdModPart[Index2]);
            (VOID)StrCatS(PartNumber, SMBIOS_STRING_MAX_LENGTH - 1, StringBuffer2);
        }
    }
    else
    {
        for (Index2 = 0; Index2 < SPD_MODULE_PART_DDR4; Index2++)
        {
            UnicodeSPrint (StringBuffer2, SMBIOS_STRING_MAX_LENGTH - 1, L"%c", pGblData->Channel[Skt][Ch].Dimm[Dimm].SpdModPartDDR4[Index2]);
            (VOID)StrCatS(PartNumber, SMBIOS_STRING_MAX_LENGTH - 1, StringBuffer2);
        }
    }

    return;
}

VOID
SmbiosGetSerialNumber (
  IN GBL_INTERFACE      *pGblData,
  IN UINT8              Skt,
  IN UINT8              Ch,
  IN UINT8              Dimm,
  OUT CHAR16            *SerialNumber
  )
{
    UINT32              Temp;

    Temp = SwapBytes32 (pGblData->Channel[Skt][Ch].Dimm[Dimm].SpdSerialNum);

    UnicodeSPrint(SerialNumber, SMBIOS_STRING_MAX_LENGTH, L"0x%08x", Temp);

    return;
}

BOOLEAN
IsDimmPresent (
  IN  GBL_INTERFACE      *pGblData,
  IN  UINT8              Skt,
  IN  UINT8              Ch,
  IN  UINT8              Dimm
)
{
    if (pGblData->Channel[Skt][Ch].Enabled == FALSE ||
        pGblData->Channel[Skt][Ch].Dimm[Dimm].Enabled == FALSE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

UINT8
SmbiosGetMemoryType (
  IN  GBL_INTERFACE      *pGblData,
  IN  UINT8              Skt,
  IN  UINT8              Ch,
  IN  UINT8              Dimm
)
{
    UINT8 MemoryType;

    if(!IsDimmPresent(pGblData, Skt, Ch, Dimm))
    {
        return MemoryTypeUnknown;
    }

    if (pGblData->Channel[Skt][Ch].Dimm[Dimm].DramType == SPD_TYPE_DDR3)
    {
        MemoryType = MemoryTypeDdr3;
    }
    else if (pGblData->Channel[Skt][Ch].Dimm[Dimm].DramType == SPD_TYPE_DDR4)
    {
        MemoryType = MemoryTypeDdr4;
    }
    else
    {
        MemoryType = MemoryTypeUnknown;
    }

    return MemoryType;
}

VOID
SmbiosGetTypeDetail (
  IN  GBL_INTERFACE         *pGblData,
  IN  UINT8                 Skt,
  IN  UINT8                 Ch,
  IN  UINT8                 Dimm,
  IN  OUT MEMORY_DEVICE_TYPE_DETAIL *TypeDetail
)
{
    if (NULL == TypeDetail)
    {
        return;
    }

    if(!IsDimmPresent(pGblData, Skt, Ch, Dimm))
    {
        TypeDetail->Unknown = 1;
        return;
    }

    switch (pGblData->Channel[Skt][Ch].Dimm[Dimm].ModuleType)
    {
        case SPD_UDIMM:
          TypeDetail->Unbuffered = 1;
          break;

        case SPD_LRDIMM:
          TypeDetail->LrDimm = 1;
          break;

        case SPD_RDIMM:
          TypeDetail->Registered = 1;
          break;

        default:
          TypeDetail->Unknown = 1;
          break;
    }
}

VOID
SmbiosGetDimmVoltageInfo (
  IN     GBL_INTERFACE         *pGblData,
  IN     UINT8                 Skt,
  IN     UINT8                 Ch,
  IN     UINT8                 Dimm,
  IN OUT SMBIOS_TABLE_TYPE17   *Type17Record

)
{
    if(!IsDimmPresent(pGblData, Skt, Ch, Dimm))
    {
        return;
    }

    if (pGblData->Channel[Skt][Ch].Dimm[Dimm].DramType == SPD_TYPE_DDR3)
    {
        Type17Record->MinimumVoltage                = 1250;
        Type17Record->MaximumVoltage                = 1500;

        switch (pGblData->Channel[Skt][Ch].Dimm[Dimm].SpdVdd)
        {
            case SPD_VDD_150:
              Type17Record->ConfiguredVoltage = 1500;
              break;

            case SPD_VDD_135:
              Type17Record->ConfiguredVoltage = 1350;
              break;

            case SPD_VDD_125:
              Type17Record->ConfiguredVoltage = 1250;
              break;

            default:
              break;
        }
    }
    else if (pGblData->Channel[Skt][Ch].Dimm[Dimm].DramType == SPD_TYPE_DDR4)
    {
        Type17Record->MinimumVoltage                = 1200;
        Type17Record->MaximumVoltage                = 2000;
        switch (pGblData->Channel[Skt][Ch].Dimm[Dimm].SpdVdd)
        {
            case SPD_VDD_120:
              Type17Record->ConfiguredVoltage = 1200;
              break;

            default:
              break;
        }
    }
}

VOID
SmbiosGetMemoryDevicesNumber (
  IN OUT UINT16 *NumberOfDevices
)
{
    UINT8       Skt, Ch, Dimm;

    for(Skt = 0; Skt < mMaxSkt; Skt++)
    {
        for(Ch = 0; Ch < mMaxCh; Ch++)
        {
            for(Dimm = 0; Dimm < OemGetDimmSlot(Skt, Ch); Dimm++)
            {
                (*NumberOfDevices)++;
            }
        }
    }
}

UINT8
SmbiosGetPartitionWidth (
)
{

    UINT8       Skt, Ch, Dimm;
    UINT8       PartitionWidth = 0;

    for(Skt = 0; Skt < mMaxSkt; Skt++)
    {
        for(Ch = 0; Ch < mMaxCh; Ch++)
        {
            for(Dimm = 0; Dimm < OemGetDimmSlot(Skt, Ch); Dimm++)
            {
                PartitionWidth++;
            }
        }
    }

    return PartitionWidth;
}

EFI_STATUS
SmbiosAddType16Table (
  IN  GBL_INTERFACE      *pGblData,
  OUT EFI_SMBIOS_HANDLE  *MemArraySmbiosHandle
  )
{
    EFI_STATUS                      Status;
    UINT64                          MemoryCapacity;
    SMBIOS_TABLE_TYPE16             *Type16Record;

    UINT16                          NumberOfMemoryDevices = 0;

    SmbiosGetMemoryDevicesNumber (&NumberOfMemoryDevices);

    MemoryCapacity = (UINT64) LShiftU64 (NumberOfMemoryDevices * MAX_DIMM_SIZE, 20); // GB to KB.

    //
    // Type 16 SMBIOS Record
    //
    Type16Record = AllocateZeroPool(sizeof(SMBIOS_TABLE_TYPE16) + 1 + 1);
    if (NULL == Type16Record)
    {
        return EFI_OUT_OF_RESOURCES;
    }

    Type16Record->Hdr.Type                          = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;
    Type16Record->Hdr.Length                        = sizeof (SMBIOS_TABLE_TYPE16);
    Type16Record->Hdr.Handle                        = 0x0;
    Type16Record->Location                          = MemoryArrayLocationSystemBoard;
    Type16Record->Use                               = MemoryArrayUseSystemMemory;
    Type16Record->MemoryErrorInformationHandle      = 0xFFFE;
    Type16Record->NumberOfMemoryDevices             = NumberOfMemoryDevices;

    if(pGblData->EccEn)
    {
        Type16Record->MemoryErrorCorrection         = MemoryErrorCorrectionSingleBitEcc;
    }
    else
    {
        Type16Record->MemoryErrorCorrection         = MemoryErrorCorrectionNone;
    }

    if (MemoryCapacity >= 0x80000000)
    {
        Type16Record->MaximumCapacity = 0x80000000;   // in KB;
        Type16Record->ExtendedMaximumCapacity = MemoryCapacity << 10;  // Extended Max capacity should be stored in bytes.
    }
    else
    {
        Type16Record->MaximumCapacity = (UINT32)MemoryCapacity;  // Max capacity should be stored in kilo bytes.
        Type16Record->ExtendedMaximumCapacity = 0;
    }

    *MemArraySmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = mSmbios->Add (mSmbios, NULL, MemArraySmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *)Type16Record);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Smbios Type16 Table Log Failed! %r \n", __FUNCTION__, __LINE__, Status));
    }

    FreePool(Type16Record);
    return Status;
}

EFI_STATUS
SmbiosAddType19Table (
  IN GBL_INTERFACE      *pGblData,
  IN EFI_SMBIOS_HANDLE  MemArraySmbiosHandle
  )
{
    EFI_STATUS                      Status;
    UINT32                          MemInfoTotalMem;
    UINT64                          TotalMemorySize;
    EFI_SMBIOS_HANDLE               MemArrayMappedAddrSmbiosHandle;
    SMBIOS_TABLE_TYPE19             *Type19Record;

    MemInfoTotalMem = pGblData->MemSize;  // In MB

    if (MemInfoTotalMem == 0)
    {
        return EFI_NOT_FOUND;
    }

    TotalMemorySize = (UINT64) LShiftU64 (MemInfoTotalMem, 10);  // MB to KB.

    //
    // Type 19 SMBIOS Record
    //
    Type19Record = AllocateZeroPool(sizeof(SMBIOS_TABLE_TYPE19) + 1 + 1);
    if (NULL == Type19Record)
    {
        return EFI_OUT_OF_RESOURCES;
    }

    Type19Record->Hdr.Type                          = EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS;
    Type19Record->Hdr.Length                        = sizeof(SMBIOS_TABLE_TYPE19);
    Type19Record->Hdr.Handle                        = 0x0;
    Type19Record->StartingAddress                   = 0x0;
    Type19Record->EndingAddress                     = (UINT32) (TotalMemorySize - 1); // in KB;
    Type19Record->MemoryArrayHandle                 = MemArraySmbiosHandle;
    Type19Record->PartitionWidth                    = SmbiosGetPartitionWidth ();
    Type19Record->ExtendedStartingAddress           = 0x0;
    Type19Record->ExtendedEndingAddress             = 0x0;

    MemArrayMappedAddrSmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = mSmbios->Add (mSmbios, NULL, &MemArrayMappedAddrSmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *)Type19Record);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Smbios Type19 Table Log Failed! %r \n", __FUNCTION__, __LINE__, Status));
    }

    FreePool(Type19Record);
    return Status;
}


EFI_STATUS
SmbiosAddType17Table (
  IN GBL_INTERFACE      *pGblData,
  IN UINT8              Skt,
  IN UINT8              Ch,
  IN UINT8              Dimm,
  IN EFI_SMBIOS_HANDLE  MemArraySmbiosHandle
  )
{
    EFI_STATUS                      Status;
    SMBIOS_TABLE_TYPE17             *Type17Record;
    EFI_SMBIOS_HANDLE               MemDevSmbiosHandle;
    UINTN                           TableSize;

    UINTN                           StringBufferSize;
    EFI_STRING                      StringBuffer;
    UINT16                          MemInfoMemFreq;
    UINT16                          MemoryTotalWidth;
    UINT16                          MemoryDataWidth;
    UINT16                          MemoryDeviceSize;
    UINT16                          MemorySpeed;
    UINT8                           Attributes;
    UINT32                          MemoryDeviceExtendSize;
    UINT16                          CfgMemorySpeed;

    CHAR8                           *OptionalStrStart;
    UINTN                           DeviceLocatorStrLen;
    UINTN                           BankLocatorStrLen;
    UINTN                           ManufactureStrLen;
    UINTN                           SerialNumberStrLen;
    UINTN                           AssertTagStrLen;
    UINTN                           PartNumberStrLen;
    EFI_STRING                      DeviceLocatorStr;
    EFI_STRING                      BankLocatorStr;
    EFI_STRING                      ManufactureStr;
    EFI_STRING                      SerialNumberStr;
    EFI_STRING                      AssertTagStr;
    EFI_STRING                      PartNumberStr;
    EFI_STRING_ID                   DeviceLocator;

    Type17Record        = NULL;
    DeviceLocatorStr    = NULL;
    BankLocatorStr      = NULL;
    ManufactureStr      = NULL;
    SerialNumberStr     = NULL;
    AssertTagStr        = NULL;
    PartNumberStr       = NULL;

    MemoryTotalWidth    = 0;
    MemoryDataWidth     = 0;
    MemoryDeviceSize    = 0;
    MemoryDeviceExtendSize = 0;
    MemorySpeed         = 0;
    Attributes          = 0;
    CfgMemorySpeed      = 0;

    //
    // Allocate Buffers
    //
    StringBufferSize = (sizeof (CHAR16)) * SMBIOS_STRING_MAX_LENGTH;
    StringBuffer = AllocateZeroPool (StringBufferSize);
    if(NULL == StringBuffer)
    {
        return EFI_OUT_OF_RESOURCES;
    }


    //
    // Manufacture
    //
    ManufactureStr = AllocateZeroPool (StringBufferSize);
    if(NULL == ManufactureStr)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto FREE_STR_BUF;
    }
    UnicodeSPrint(ManufactureStr, SMBIOS_STRING_MAX_LENGTH - 1, L"NO DIMM");

    //
    // SerialNumber
    //
    SerialNumberStr = AllocateZeroPool (StringBufferSize);
    if(NULL == SerialNumberStr)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto FREE_STR_MAN;
    }
    UnicodeSPrint(SerialNumberStr, SMBIOS_STRING_MAX_LENGTH - 1, L"NO DIMM");

    //
    // AssetTag
    //
    AssertTagStr = AllocateZeroPool (StringBufferSize);
    if(NULL == AssertTagStr)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto FREE_STR_SN;
    }
    UnicodeSPrint(AssertTagStr, SMBIOS_STRING_MAX_LENGTH - 1, L"NO DIMM");

    //
    // PartNumber
    //
    PartNumberStr = AllocateZeroPool (StringBufferSize);
    if(NULL == PartNumberStr)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto FREE_STR_AST;
    }
    UnicodeSPrint(PartNumberStr, SMBIOS_STRING_MAX_LENGTH - 1, L"NO DIMM");


    if(IsDimmPresent(pGblData, Skt, Ch, Dimm))
    {
        MemoryDataWidth = pGblData->Channel[Skt][Ch].Dimm[Dimm].PrimaryBusWidth;
        MemoryTotalWidth = MemoryDataWidth + pGblData->Channel[Skt][Ch].Dimm[Dimm].ExtensionBusWidth;

        MemoryDeviceSize = pGblData->Channel[Skt][Ch].Dimm[Dimm].DimmSize;  //in MB
        MemoryDeviceExtendSize = 0;

        if (MemoryDeviceSize >= 0x7fff)
        {
            MemoryDeviceExtendSize = MemoryDeviceSize;  // in MB
            MemoryDeviceSize = 0x7fff;                    // max value
        }

        MemInfoMemFreq = pGblData->Freq;
        MemorySpeed = pGblData->Channel[Skt][Ch].Dimm[Dimm].DimmSpeed;
        Attributes = pGblData->Channel[Skt][Ch].Dimm[Dimm].RankNum;
        CfgMemorySpeed = MemInfoMemFreq;

        //
        // Manufacturer
        //
        SmbiosGetManufacturer (pGblData->Channel[Skt][Ch].Dimm[Dimm].SpdMMfgId & 0xFF,
                                        pGblData->Channel[Skt][Ch].Dimm[Dimm].SpdMMfgId >> 8,
                                        ManufactureStr
                                        );

        //
        // SerialNumber
        //
        SmbiosGetSerialNumber(pGblData, Skt, Ch, Dimm, SerialNumberStr);

        //
        // AssetTag
        //
        UnicodeSPrint(AssertTagStr, SMBIOS_STRING_MAX_LENGTH - 1, L"Unknown");

        //
        // PartNumber
        //
        SmbiosGetPartNumber(pGblData, Skt, Ch, Dimm, PartNumberStr);
    }

    //
    // DeviceLocator
    //
    DeviceLocatorStr = AllocateZeroPool ((sizeof (CHAR16)) * SMBIOS_STRING_MAX_LENGTH);
    if(NULL == DeviceLocatorStr)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto FREE_STR_PN;
    }
    DeviceLocator = gDimmToDevLocator[Skt][Ch][Dimm];
    if (DeviceLocator != 0xFFFF)
    {
        UnicodeSPrint(DeviceLocatorStr, SMBIOS_STRING_MAX_LENGTH, L"DIMM%x%x%x ", Skt, Ch, Dimm);
        StringBuffer = HiiGetPackageString (&gEfiCallerIdGuid, DeviceLocator, NULL);
        (VOID)StrCatS(DeviceLocatorStr, SMBIOS_STRING_MAX_LENGTH, StringBuffer);
    }
    else
    {
    UnicodeSPrint(DeviceLocatorStr, SMBIOS_STRING_MAX_LENGTH, L"DIMM%x%x%x", Skt, Ch, Dimm);
    }
    DeviceLocatorStrLen = StrLen (DeviceLocatorStr);

    //
    // BankLocator
    //
    BankLocatorStr = AllocateZeroPool ((sizeof (CHAR16)) * SMBIOS_STRING_MAX_LENGTH);
    if(NULL == BankLocatorStr)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto FREE_STR_DEV;
    }
    UnicodeSPrint(BankLocatorStr, SMBIOS_STRING_MAX_LENGTH, L"SOCKET %x CHANNEL %x DIMM %x", Skt, Ch, Dimm);
    BankLocatorStrLen = StrLen (BankLocatorStr);

    ManufactureStrLen = StrLen (ManufactureStr);
    SerialNumberStrLen = StrLen (SerialNumberStr);
    AssertTagStrLen = StrLen (AssertTagStr);
    PartNumberStrLen = StrLen (PartNumberStr);

    //
    // Report Type 17 SMBIOS Record
    //
    TableSize = sizeof(SMBIOS_TABLE_TYPE17) + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1 + ManufactureStrLen + 1 + SerialNumberStrLen + 1 + AssertTagStrLen + 1 + PartNumberStrLen + 1 + 1;
    Type17Record = AllocateZeroPool (TableSize);
    if(NULL == Type17Record)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto FREE_BL;
    }

    Type17Record->Hdr.Type                      = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
    Type17Record->Hdr.Length                    = sizeof (SMBIOS_TABLE_TYPE17);
    Type17Record->Hdr.Handle                    = 0;
    Type17Record->MemoryArrayHandle             = MemArraySmbiosHandle;
    Type17Record->MemoryErrorInformationHandle  = 0xFFFE;
    Type17Record->TotalWidth                    = MemoryTotalWidth;
    Type17Record->DataWidth                     = MemoryDataWidth;
    Type17Record->Size                          = MemoryDeviceSize;           // in MB
    Type17Record->FormFactor                    = MemoryFormFactorDimm;
    Type17Record->DeviceLocator                 = 1;
    Type17Record->BankLocator                   = 2;
    Type17Record->MemoryType                    = SmbiosGetMemoryType (pGblData, Skt, Ch, Dimm);

    Type17Record->TypeDetail.Synchronous    = 1;

    SmbiosGetTypeDetail (pGblData, Skt, Ch, Dimm, &(Type17Record->TypeDetail));

    Type17Record->Speed                         = MemorySpeed; // in MHZ
    Type17Record->Manufacturer                  = 3;
    Type17Record->SerialNumber                  = 4;
    Type17Record->AssetTag                      = 5;
    Type17Record->PartNumber                    = 6;
    Type17Record->Attributes                    = Attributes;
    Type17Record->ExtendedSize                  = MemoryDeviceExtendSize;
    Type17Record->ConfiguredMemoryClockSpeed    = CfgMemorySpeed;
    //
    // Add for smbios 2.8.0
    //
    SmbiosGetDimmVoltageInfo (pGblData, Skt, Ch, Dimm, Type17Record);

    OptionalStrStart = (CHAR8 *) (Type17Record + 1);
    Status = UnicodeStrToAsciiStrS (DeviceLocatorStr, OptionalStrStart, DeviceLocatorStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (BankLocatorStr, OptionalStrStart + DeviceLocatorStrLen + 1, BankLocatorStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (ManufactureStr, OptionalStrStart + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1, ManufactureStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (SerialNumberStr, OptionalStrStart + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1 + ManufactureStrLen + 1, SerialNumberStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (AssertTagStr, OptionalStrStart + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1 + ManufactureStrLen + 1 + SerialNumberStrLen + 1, AssertTagStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (PartNumberStr, OptionalStrStart + DeviceLocatorStrLen + 1 + BankLocatorStrLen + 1 + ManufactureStrLen + 1 + SerialNumberStrLen + 1 + AssertTagStrLen + 1, PartNumberStrLen + 1);
    ASSERT_EFI_ERROR (Status);

    MemDevSmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = mSmbios->Add (mSmbios, NULL, &MemDevSmbiosHandle, (EFI_SMBIOS_TABLE_HEADER*) Type17Record);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Smbios Type17 Table Log Failed! %r \n", __FUNCTION__, __LINE__, Status));
    }

    FreePool (Type17Record);

FREE_BL:
    FreePool (BankLocatorStr);

FREE_STR_DEV:
    FreePool (DeviceLocatorStr);

FREE_STR_PN:
    FreePool (PartNumberStr);

FREE_STR_AST:
    FreePool (AssertTagStr);

FREE_STR_SN:
    FreePool (SerialNumberStr);

FREE_STR_MAN:
    FreePool (ManufactureStr);

FREE_STR_BUF:
    FreePool (StringBuffer);

    return Status;
}


/**
  Standard EFI driver point.  This driver locates the MemoryConfigurationData Variable,
  if it exists, add the related SMBIOS tables by PI SMBIOS protocol.

  @param  ImageHandle     Handle for the image of this driver
  @param  SystemTable     Pointer to the EFI System Table

  @retval  EFI_SUCCESS    The data was successfully stored.

**/
EFI_STATUS
EFIAPI
MemorySubClassEntryPoint(
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
    EFI_STATUS                      Status;
    EFI_SMBIOS_PROTOCOL             *Smbios;
    EFI_HOB_GUID_TYPE               *GuidHob;
    GBL_INTERFACE                   *pGblData;
    EFI_SMBIOS_HANDLE               MemArraySmbiosHandle;
    UINT8                           Skt, Ch, Dimm;

    GuidHob = GetFirstGuidHob(&gHisiEfiMemoryMapGuid);
    if(NULL == GuidHob)
    {
        DEBUG((EFI_D_ERROR, "Could not get MemoryMap Guid hob.  %r\n"));
        return EFI_NOT_FOUND;
    }
    pGblData = (GBL_INTERFACE*) GET_GUID_HOB_DATA(GuidHob);

    //
    // Locate dependent protocols
    //
    Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID**)&Smbios);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "Could not locate SMBIOS protocol.  %r\n", Status));
        return Status;
    }
    mSmbios = Smbios;

    //
    // Add our default strings to the HII database. They will be modified later.
    //
    mHiiHandle = OemGetPackages();
    if(NULL == mHiiHandle)
    {
        return EFI_OUT_OF_RESOURCES;
    }

    mMaxSkt  = OemGetSocketNumber();
    mMaxCh   = OemGetDdrChannel();
    // Get DIMM slot number on Socket 0 Channel 0
    // TODO: Assume all channels have same slot number

    Status = SmbiosAddType16Table (pGblData, &MemArraySmbiosHandle);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "Smbios Add Type16 Table Failed.  %r\n", Status));
        return Status;
    }

    Status = SmbiosAddType19Table (pGblData, MemArraySmbiosHandle);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "Smbios Add Type19 Table Failed.  %r\n", Status));
        return Status;
    }

    for(Skt = 0; Skt < mMaxSkt; Skt++)
    {
        for(Ch = 0; Ch < mMaxCh; Ch++)
        {
            for(Dimm = 0; Dimm < OemGetDimmSlot(Skt, Ch); Dimm++)
            {
                Status = SmbiosAddType17Table (pGblData, Skt, Ch, Dimm, MemArraySmbiosHandle);
                if(EFI_ERROR(Status))
                {
                    DEBUG((EFI_D_ERROR, "Smbios Add Type17 Table Failed.  %r\n", Status));
                }
            }
        }
    }

    return Status;
}
