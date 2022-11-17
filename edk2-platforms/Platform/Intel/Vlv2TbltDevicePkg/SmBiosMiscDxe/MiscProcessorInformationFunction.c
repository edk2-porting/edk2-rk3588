/*++

Copyright (c) 2006  - 2021, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  MiscProcessorInformationFunction.c

Abstract:

  Onboard processor information boot time changes.
  SMBIOS type 4.

--*/

#include "CommonHeader.h"

#include "MiscSubclassDriver.h"

#include <Protocol/MpService.h>
#include <Library/TimerLib.h>
#include <Register/Cpuid.h>

#define EfiProcessorFamilyIntelAtomProcessor    0x2B

EFI_GUID                        mProcessorProducerGuid;


/**
  Get cache SMBIOS record handle.

  @param  Smbios        Pointer to SMBIOS protocol instance.
  @param  CacheLevel    Level of cache, starting from one.
  @param  Handle        Returned record handle.

**/

VOID
GetCacheHandle (
  IN  EFI_SMBIOS_PROTOCOL      *Smbios,
  IN  UINT8                    CacheLevel,
  OUT  EFI_SMBIOS_HANDLE       *Handle
  )
{
  UINT16                     CacheConfig;
  EFI_STATUS                 Status;
  EFI_SMBIOS_TYPE            RecordType;
  EFI_SMBIOS_TABLE_HEADER    *Buffer;

  *Handle = 0;
  RecordType = EFI_SMBIOS_TYPE_CACHE_INFORMATION;

  do {
    Status = Smbios->GetNext (
                       Smbios,
                       Handle,
                       &RecordType,
                       &Buffer,
                       NULL
                       );
    if (!EFI_ERROR(Status)) {
      CacheConfig = *(UINT16*)((UINT8*)Buffer + 5);
      if ((CacheConfig & 0x7) == (CacheLevel -1) ) {
        return;
      }
    }
  } while (!EFI_ERROR(Status));

  *Handle = 0xFFFF;
}

#define BSEL_CR_OVERCLOCK_CONTROL	0xCD
#define	FUSE_BSEL_MASK				0x03



UINT16 miFSBFrequencyTable[4] = {
  83,          	// 83.3MHz
  100,          // 100MHz
  133,          // 133MHz
  117           // 116.7MHz
};

/**
  Determine the processor core frequency

  @param None

  @retval Processor core frequency multiplied by 3


**/
UINT16
DetermineiFsbFromMsr (
  VOID
  )
{
  //
  // Determine the processor core frequency
  //
  UINT64	Temp;
  Temp = (AsmReadMsr64 (BSEL_CR_OVERCLOCK_CONTROL)) & FUSE_BSEL_MASK;
  return miFSBFrequencyTable[(UINT32)(Temp)];

}

CHAR16 *
CpuidSocVendorBrandString (
  VOID
  )
{
  UINT32  MaximumExtendedFunction;
  //
  // Array to store brand string from 3 brand string leafs with
  // 4 32-bit brand string values per leaf and an extra value to
  // null terminate the string.
  //
  UINT32  BrandString[3 * 4 + 1];
  CHAR8   *AsciiBrandString;
  CHAR16  *UnicodeBrandString;
  UINTN   Length;

  AsmCpuid (CPUID_EXTENDED_FUNCTION, &MaximumExtendedFunction, NULL, NULL, NULL);

  ZeroMem (&BrandString, sizeof (BrandString));
  if (CPUID_BRAND_STRING1 <= MaximumExtendedFunction) {
    AsmCpuid (
      CPUID_BRAND_STRING1,
      &BrandString[0],
      &BrandString[1],
      &BrandString[2],
      &BrandString[3]
      );
  }
  if (CPUID_BRAND_STRING2 <= MaximumExtendedFunction) {
    AsmCpuid (
      CPUID_BRAND_STRING2,
      &BrandString[4],
      &BrandString[5],
      &BrandString[6],
      &BrandString[7]
      );
  }
  if (CPUID_BRAND_STRING3 <= MaximumExtendedFunction) {
    AsmCpuid (
      CPUID_BRAND_STRING3,
      &BrandString[8],
      &BrandString[9],
      &BrandString[10],
      &BrandString[11]
      );
  }

  //
  // Skip spaces at the beginning of the brand string
  //
  for (AsciiBrandString = (CHAR8 *)BrandString; *AsciiBrandString == ' '; AsciiBrandString++);

  DEBUG ((DEBUG_INFO, "Processor Brand String = %a\n", AsciiBrandString));

  //
  // Convert ASCII brand string to an allocated Unicode brand string
  //
  Length = AsciiStrLen (AsciiBrandString) + 1;
  UnicodeBrandString = AllocatePool (Length * sizeof (CHAR16));
  AsciiStrToUnicodeStrS (AsciiBrandString, UnicodeBrandString, Length);

  DEBUG ((DEBUG_INFO, "Processor Unicode Brand String = %s\n", UnicodeBrandString));

  return UnicodeBrandString;
}

UINT64
MeasureTscFrequency (
  VOID
  )
{
  EFI_TPL  CurrentTpl;
  UINT64   BeginValue;
  UINT64   EndValue;
  UINT64   Frequency;

  //
  // Wait for 10000us = 10ms for the calculation
  // It needs a precise timer to calculate the ticks
  //
  CurrentTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  BeginValue = AsmReadTsc ();
  MicroSecondDelay (10000);
  EndValue   = AsmReadTsc ();
  gBS->RestoreTPL (CurrentTpl);
  Frequency = MultU64x32 (EndValue - BeginValue, 1000);
  Frequency = DivU64x32 (Frequency, 10);
  return Frequency;
}

MISC_SMBIOS_TABLE_FUNCTION (MiscProcessorInformation)
{
    CHAR8                           *OptionalStrStart;
    UINTN                           OptionalStrSize;
    EFI_STRING                      SerialNumber;
    CHAR16                          *Version=NULL;
    CHAR16                          *Manufacturer=NULL;
    CHAR16                          *Socket=NULL;
    CHAR16                          *AssetTag=NULL;
    CHAR16                          *PartNumber=NULL;
    UINTN                           SerialNumberStrLen=0;
    UINTN                           VersionStrLen=0;
    UINTN                           ManufacturerStrLen=0;
    UINTN                           SocketStrLen=0;
    UINTN                           AssetTagStrLen=0;
    UINTN                           PartNumberStrLen=0;
    UINTN                           ProcessorVoltage=(BIT7 | 9);
    UINT32                          Eax01;
    UINT32                          Ebx01;
    UINT32                          Ecx01;
    UINT32                          Edx01;
    STRING_REF                      TokenToGet;
    EFI_STATUS                      Status;
    EFI_SMBIOS_HANDLE               SmbiosHandle;
    SMBIOS_TABLE_TYPE4              *SmbiosRecord;
    EFI_CPU_DATA_RECORD             *ForType4InputData;
    UINT16                          L1CacheHandle=0;
    UINT16                          L2CacheHandle=0;
    UINT16                          L3CacheHandle=0;
    UINTN                           NumberOfEnabledProcessors=0 ;
    UINTN                           NumberOfProcessors=0;
    UINT64                          Frequency = 0;
    EFI_MP_SERVICES_PROTOCOL        *MpService;
    PROCESSOR_ID_DATA               *ProcessorId = NULL;

    //
    // First check for invalid parameters.
    //
    if (RecordData == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    ForType4InputData = (EFI_CPU_DATA_RECORD *)RecordData;

    ProcessorId = AllocateZeroPool(sizeof(PROCESSOR_ID_DATA));
    if (ProcessorId == NULL) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // Token to get for Socket Name
    //
    TokenToGet = STRING_TOKEN (STR_MISC_SOCKET_NAME);
    Socket = SmbiosMiscGetString (TokenToGet);
    SocketStrLen = StrLen(Socket);
    if (SocketStrLen > SMBIOS_STRING_MAX_LENGTH) {
         return EFI_UNSUPPORTED;
    }

    //
    // Token to get for Processor Manufacturer
    //
    TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_MAUFACTURER);
    Manufacturer = SmbiosMiscGetString (TokenToGet);
    ManufacturerStrLen = StrLen(Manufacturer);
    if (ManufacturerStrLen > SMBIOS_STRING_MAX_LENGTH) {
      return EFI_UNSUPPORTED;
    }

    //
    // Token to get for Processor Version
    //
    Version = CpuidSocVendorBrandString ();
    VersionStrLen = StrLen(Version);
    if (VersionStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
    }

    //
    // Token to get for Serial Number
    //
    TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_SERIAL_NUMBER);
    SerialNumber = SmbiosMiscGetString (TokenToGet);
    SerialNumberStrLen = StrLen(SerialNumber);
    if (SerialNumberStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
    }

    //
    // Token to get for Assert Tag Information
    //
    TokenToGet = STRING_TOKEN (STR_MISC_ASSERT_TAG_DATA);
    AssetTag = SmbiosMiscGetString (TokenToGet);
    AssetTagStrLen = StrLen(AssetTag);
    if (AssetTagStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
    }

    //
    // Token to get for part number Information
    //
    TokenToGet = STRING_TOKEN (STR_MISC_PART_NUMBER);
    PartNumber = SmbiosMiscGetString (TokenToGet);
    PartNumberStrLen = StrLen(PartNumber);
    if (PartNumberStrLen > SMBIOS_STRING_MAX_LENGTH) {
         return EFI_UNSUPPORTED;
    }

    //
    // Two zeros following the last string.
    //
    OptionalStrSize = AssetTagStrLen + 1 + SocketStrLen + 1+ ManufacturerStrLen +1 + VersionStrLen+ 1+ SerialNumberStrLen + 1 + PartNumberStrLen+ 1 + 1;
    SmbiosRecord = AllocateZeroPool(sizeof (SMBIOS_TABLE_TYPE4) + OptionalStrSize);

    SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION;
    SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE4);

    //
    // Make handle chosen by smbios protocol.add automatically.
    //
    SmbiosRecord->Hdr.Handle = 0;

    SmbiosRecord-> Socket= 1;
    SmbiosRecord -> ProcessorManufacturer = 2;
    SmbiosRecord -> ProcessorVersion = 3;
    SmbiosRecord ->SerialNumber =4;

    SmbiosRecord-> AssetTag= 5;
    SmbiosRecord-> PartNumber= 6;

    //
    // Processor Type
    //
    ForType4InputData-> VariableRecord.ProcessorType= EfiCentralProcessor;
    SmbiosRecord -> ProcessorType = ForType4InputData-> VariableRecord.ProcessorType;

    //
    // Processor Family
    //
    ForType4InputData-> VariableRecord.ProcessorFamily= EfiProcessorFamilyIntelAtomProcessor; //0x2B;;
    SmbiosRecord -> ProcessorFamily = ForType4InputData-> VariableRecord.ProcessorFamily;
    SmbiosRecord -> ExternalClock = DetermineiFsbFromMsr();

    //
    // Processor ID
    //
    AsmCpuid(0x001, &Eax01, &Ebx01, &Ecx01, &Edx01);
    ProcessorId->Signature = *(PROCESSOR_SIGNATURE *)&Eax01;
    ProcessorId->FeatureFlags = *(PROCESSOR_FEATURE_FLAGS *)&Edx01;
    SmbiosRecord -> ProcessorId = *(PROCESSOR_ID_DATA *)ProcessorId;

    //
    // Processor Voltage
    //
    ForType4InputData-> VariableRecord.ProcessorVoltage= *(EFI_PROCESSOR_VOLTAGE_DATA *)&ProcessorVoltage;
    SmbiosRecord -> Voltage = *(PROCESSOR_VOLTAGE *) &(ForType4InputData-> VariableRecord.ProcessorVoltage);

    //
    // Status
    //
    ForType4InputData-> VariableRecord.ProcessorHealthStatus= 0x41;//0x41;
    SmbiosRecord -> Status = ForType4InputData-> VariableRecord.ProcessorHealthStatus;

    //
    // Processor Upgrade
    //
    SmbiosRecord -> ProcessorUpgrade = 0x008;

    //
    // Processor Family 2
    //
    SmbiosRecord -> ProcessorFamily2 = ForType4InputData-> VariableRecord.ProcessorFamily;

    //
    // Processor speed in MHz
    //
    Frequency = MeasureTscFrequency ();
    Frequency = DivU64x32 (Frequency, 1000000);
    SmbiosRecord-> CurrentSpeed = (UINT16)Frequency;
    SmbiosRecord-> MaxSpeed = (UINT16)Frequency;

    //
    // Processor Characteristics
    //
    AsmCpuid(0x8000000, NULL, NULL, NULL, &Edx01);
    Edx01= Edx01 >> 28;
    Edx01 &= 0x01;
    SmbiosRecord-> ProcessorCharacteristics= (UINT16)Edx01;

    //
    // Processor Core Count and Enabled core count
    //
    Status = gBS->LocateProtocol (
                    &gEfiMpServiceProtocolGuid,
                    NULL,
                    (void **)&MpService
                    );
    if (!EFI_ERROR (Status)) {
    //
    // Determine the number of processors
    //
    MpService->GetNumberOfProcessors (
                 MpService,
                 &NumberOfProcessors,
                 &NumberOfEnabledProcessors
                 );
    }
    SmbiosRecord-> CoreCount= (UINT8)NumberOfProcessors;
    SmbiosRecord-> EnabledCoreCount= (UINT8)NumberOfEnabledProcessors;
    SmbiosRecord-> ThreadCount= (UINT8)NumberOfEnabledProcessors;
    SmbiosRecord-> ProcessorCharacteristics = 0x2; // Unknown

    //
    // Processor Cache Handle
    //
    GetCacheHandle( Smbios,1, &L1CacheHandle);
    GetCacheHandle( Smbios,2, &L2CacheHandle);
    GetCacheHandle( Smbios,3, &L3CacheHandle);

    //
    // Updating Cache Handle Information
    //
    SmbiosRecord->L1CacheHandle  = L1CacheHandle;
    SmbiosRecord->L2CacheHandle  = L2CacheHandle;
    SmbiosRecord->L3CacheHandle  = L3CacheHandle;

    OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
    UnicodeStrToAsciiStrS (Socket, OptionalStrStart, OptionalStrSize);
    OptionalStrStart += (SocketStrLen + 1);
    OptionalStrSize  -= (SocketStrLen + 1);
    UnicodeStrToAsciiStrS (Manufacturer, OptionalStrStart, OptionalStrSize);
    OptionalStrStart += (ManufacturerStrLen + 1);
    OptionalStrSize  -= (ManufacturerStrLen + 1);
    UnicodeStrToAsciiStrS (Version, OptionalStrStart, OptionalStrSize);
    OptionalStrStart += (VersionStrLen + 1);
    OptionalStrSize  -= (VersionStrLen + 1);
    UnicodeStrToAsciiStrS (SerialNumber, OptionalStrStart, OptionalStrSize);
    OptionalStrStart += (SerialNumberStrLen + 1);
    OptionalStrSize  -= (SerialNumberStrLen + 1);
    UnicodeStrToAsciiStrS (AssetTag, OptionalStrStart, OptionalStrSize);
    OptionalStrStart += (AssetTagStrLen + 1);
    OptionalStrSize  -= (AssetTagStrLen + 1);
    UnicodeStrToAsciiStrS (PartNumber, OptionalStrStart, OptionalStrSize);

    //
    // Now we have got the full Smbios record, call Smbios protocol to add this record.
    //
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios-> Add(
                        Smbios,
                        NULL,
                        &SmbiosHandle,
                        (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                        );
    if (EFI_ERROR (Status)) return Status;
    FreePool(SmbiosRecord);
    return Status;

}
