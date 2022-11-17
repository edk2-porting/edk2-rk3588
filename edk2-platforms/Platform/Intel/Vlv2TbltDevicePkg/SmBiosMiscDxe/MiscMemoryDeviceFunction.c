/*++

Copyright (c) 2006  - 2020, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  MiscMemoryDeviceFunction.c

Abstract:

   Memory Device
   Misc. subclass type 17.
   SMBIOS type 17.

--*/


#include "CommonHeader.h"
#include "MiscSubclassDriver.h"
#include <Protocol/MemInfo.h>


#define FREQ_800           0x00
#define FREQ_1066          0x01
#define FREQ_1333          0x02
#define FREQ_1600          0x03

#define MAX_SOCKETS  2
#define EfiMemoryTypeDdr3  0x18

enum {
    DDRType_DDR3 = 0,
    DDRType_DDR3L = 1,
    DDRType_DDR3U = 2,
    DDRType_DDR3All = 3,
    DDRType_LPDDR2 = 4,
    DDRType_LPDDR3 = 5,
    DDRType_DDR4 = 6
};


/**
  This function makes boot time changes to the contents of the
  MiscBiosVendor (Type 0).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
VOID
GetType16Hndl (
  IN  EFI_SMBIOS_PROTOCOL      *Smbios,
  OUT  EFI_SMBIOS_HANDLE       *Handle
  )
{
  EFI_STATUS                 Status;
  EFI_SMBIOS_TYPE            RecordType;
  EFI_SMBIOS_TABLE_HEADER    *Buffer;

  *Handle = 0;
   RecordType = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;

    Status = Smbios->GetNext (
                       Smbios,
                       Handle,
                       &RecordType,
                       &Buffer,
                       NULL
                       );
    if (!EFI_ERROR(Status)) {
        return;
      }
  *Handle = 0xFFFF;
}

MISC_SMBIOS_TABLE_FUNCTION( MiscMemoryDevice )
{
    CHAR8                           *OptionalStrStart;
    UINTN                           OptionalStrSize;
    UINTN                           MemDeviceStrLen;
    UINTN                           MemBankLocatorStrLen;
    UINTN                           MemManufacturerStrLen;
    UINTN                           MemSerialNumberStrLen;
    UINTN                           MemAssetTagStrLen;
    UINTN                           MemPartNumberStrLen;
    CHAR16                          *MemDevice;
    CHAR16                          *MemBankLocator;
    CHAR16                          *MemManufacturer;
    CHAR16                          *MemSerialNumber;
    CHAR16                          *MemAssetTag;
    CHAR16                          *MemPartNumber;
    EFI_STATUS                      Status;
    STRING_REF                      TokenToGet;
    SMBIOS_TABLE_TYPE17             *SmbiosRecord;
    SMBIOS_TABLE_TYPE19             *SmbiosRecord19;
    EFI_SMBIOS_HANDLE               SmbiosHandle;
    EFI_MEMORY_ARRAY_LINK_DATA      *ForType17InputData;
    UINT16                          DdrFreq=0;
    UINT16                          Type16Handle=0;
    MEM_INFO_PROTOCOL               *MemInfoHob;
    UINT8                           MemoryType;

    UINT8                           Dimm;
    UINT8                           NumSlots;
    UINT64                          TotalMemorySize;
    STRING_REF                      DevLocator[] = {
      STRING_TOKEN(STR_MISC_MEM_DEV_LOCATOR0), STRING_TOKEN(STR_MISC_MEM_DEV_LOCATOR1)
    };
    STRING_REF                      BankLocator[] = {
      STRING_TOKEN(STR_MISC_MEM_BANK_LOCATOR0), STRING_TOKEN(STR_MISC_MEM_BANK_LOCATOR1)
    };

    //
    // First check for invalid parameters.
    //
    if (RecordData == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    ForType17InputData        = (EFI_MEMORY_ARRAY_LINK_DATA *)RecordData;

    //
    // Get Memory size parameters for each rank from the chipset registers
    //
    Status = gBS->LocateProtocol (
                    &gMemInfoProtocolGuid,
                    NULL,
                    (void **)&MemInfoHob
                    );
    ASSERT_EFI_ERROR (Status);

    NumSlots = (UINT8)(MAX_SOCKETS);

    //
    // Memory Freq
    //
    switch (MemInfoHob->MemInfoData.ddrFreq){
        case FREQ_800:
          DdrFreq = 800;
          break;
        case FREQ_1066:
          DdrFreq = 1066;
          break;
        case FREQ_1333:
          DdrFreq = 1333;
          break;
        case FREQ_1600:
          DdrFreq = 1600;
          break;
        default:
          DdrFreq = 0;
          break;
    }

    //
    // Memory Type
    //
    switch  (MemInfoHob->MemInfoData.ddrType) {
        case DDRType_LPDDR2:
          MemoryType  = EfiMemoryTypeDdr2;
          break;
        case DDRType_DDR3:
        case DDRType_DDR3L:
        case DDRType_DDR3U:
        case DDRType_LPDDR3:
          MemoryType = EfiMemoryTypeDdr3;
          break;
        default:
          MemoryType = EfiMemoryTypeUnknown;
          break;
    }

    GetType16Hndl( Smbios, &Type16Handle);
    TotalMemorySize = 0;
    for (Dimm = 0; Dimm < NumSlots; Dimm++) {
      //
      // Memory Device Locator
      //
      TokenToGet = DevLocator[Dimm];
      MemDevice = SmbiosMiscGetString (TokenToGet);
      MemDeviceStrLen = StrLen(MemDevice);
      if (MemDeviceStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
      }

      TokenToGet = DevLocator[Dimm];
      MemDevice = SmbiosMiscGetString (TokenToGet);
      MemDeviceStrLen = StrLen(MemDevice);
      if (MemDeviceStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
      }

      //
      // Memory Bank Locator
      //
      TokenToGet = BankLocator[Dimm];
      MemBankLocator = SmbiosMiscGetString (TokenToGet);
      MemBankLocatorStrLen = StrLen(MemBankLocator);
      if (MemBankLocatorStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
      }

      //
      // Memory Manufacturer
      //
      TokenToGet = STRING_TOKEN (STR_MISC_MEM_MANUFACTURER);
      MemManufacturer = SmbiosMiscGetString (TokenToGet);
      MemManufacturerStrLen = StrLen(MemManufacturer);
      if (MemManufacturerStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
      }

      //
      // Memory Serial Number
      //
      TokenToGet = STRING_TOKEN (STR_MISC_MEM_SERIAL_NO);
      MemSerialNumber = SmbiosMiscGetString (TokenToGet);
      MemSerialNumberStrLen = StrLen(MemSerialNumber);
      if (MemSerialNumberStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
      }

      //
      // Memory Asset Tag Number
      //
      TokenToGet = STRING_TOKEN (STR_MISC_MEM_ASSET_TAG);
      MemAssetTag = SmbiosMiscGetString (TokenToGet);
      MemAssetTagStrLen = StrLen(MemAssetTag);
      if (MemAssetTagStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
      }

      //
      // Memory Part Number
      //
      TokenToGet = STRING_TOKEN (STR_MISC_MEM_PART_NUMBER);
      MemPartNumber = SmbiosMiscGetString (TokenToGet);
      MemPartNumberStrLen = StrLen(MemPartNumber);
      if (MemPartNumberStrLen > SMBIOS_STRING_MAX_LENGTH) {
        return EFI_UNSUPPORTED;
      }

      //
      // Two zeros following the last string.
      //
      OptionalStrSize = MemDeviceStrLen + 1 + MemBankLocatorStrLen + 1 + MemManufacturerStrLen + 1 + MemSerialNumberStrLen + 1 + MemAssetTagStrLen+1 + MemPartNumberStrLen + 1 + 1;
      SmbiosRecord = AllocatePool(sizeof (SMBIOS_TABLE_TYPE17) + OptionalStrSize);
      ZeroMem(SmbiosRecord, sizeof (SMBIOS_TABLE_TYPE17) +  OptionalStrSize);

      SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
      SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE17);

      //
      // Make handle chosen by smbios protocol.add automatically.
      //
      SmbiosRecord->Hdr.Handle = 0;

      //
      // Memory Array Handle will be the 3rd optional string following the formatted structure.
      //
      SmbiosRecord->MemoryArrayHandle = Type16Handle;

      //
      // Memory Size
      //
      if ((MemInfoHob->MemInfoData.dimmSize[Dimm])!=0){
        SmbiosRecord->TotalWidth = 32;
        SmbiosRecord->DataWidth = 32;
        SmbiosRecord->Size = MemInfoHob->MemInfoData.dimmSize[Dimm];
        SmbiosRecord->Speed = DdrFreq;
        SmbiosRecord->ConfiguredMemoryClockSpeed = DdrFreq;
        SmbiosRecord->FormFactor = EfiMemoryFormFactorDimm;
      }

      SmbiosRecord->DeviceSet =(UINT8) ForType17InputData->MemoryDeviceSet;
      SmbiosRecord->DeviceLocator= 1;
      SmbiosRecord->BankLocator = 2;


      SmbiosRecord->Manufacturer = 3;
      SmbiosRecord->SerialNumber= 4;
      SmbiosRecord->AssetTag= 5;
      SmbiosRecord->PartNumber= 6;
      SmbiosRecord->Attributes = (UINT8) ForType17InputData->MemoryState;
      SmbiosRecord->MemoryType = MemoryType;

      OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
      UnicodeStrToAsciiStrS (MemDevice, OptionalStrStart, OptionalStrSize);
      OptionalStrStart += (MemDeviceStrLen + 1);
      OptionalStrSize  -= (MemDeviceStrLen + 1);
      UnicodeStrToAsciiStrS (MemBankLocator, OptionalStrStart, OptionalStrSize);
      OptionalStrStart += (MemBankLocatorStrLen + 1);
      OptionalStrSize  -= (MemBankLocatorStrLen + 1);
      UnicodeStrToAsciiStrS (MemManufacturer, OptionalStrStart, OptionalStrSize);
      OptionalStrStart += (MemManufacturerStrLen + 1);
      OptionalStrSize  -= (MemManufacturerStrLen + 1);
      UnicodeStrToAsciiStrS (MemSerialNumber, OptionalStrStart, OptionalStrSize);
      OptionalStrStart += (MemSerialNumberStrLen + 1);
      OptionalStrSize  -= (MemSerialNumberStrLen + 1);
      UnicodeStrToAsciiStrS (MemAssetTag, OptionalStrStart, OptionalStrSize);
      OptionalStrStart += (MemAssetTagStrLen + 1);
      OptionalStrSize  -= (MemAssetTagStrLen + 1);
      UnicodeStrToAsciiStrS (MemPartNumber, OptionalStrStart, OptionalStrSize);

      //
      // Now we have got the full smbios record, call smbios protocol to add this record.
      //
      SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
      Status = Smbios-> Add(
                          Smbios,
                          NULL,
                          &SmbiosHandle,
                          (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                          );

      if ((SmbiosRecord->Size & BIT15) != 0) {
        //
        // Size is in KB
        //
        TotalMemorySize = TotalMemorySize + LShiftU64 (SmbiosRecord->Size, 10);
      } else {
        //
        // Size is in MB
        //
        TotalMemorySize = TotalMemorySize + LShiftU64 (SmbiosRecord->Size, 20);
      }

      FreePool(SmbiosRecord);
    }

    //
    // Allocate and zero SMBIOS TYPE 19 Record
    //
    SmbiosRecord19 = AllocateZeroPool (sizeof (SMBIOS_TABLE_TYPE19));
    ASSERT (SmbiosRecord19 != NULL);

    //
    // Fill in SMBIOS type 19 information
    //
    SmbiosRecord19->Hdr.Type = SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS;
    SmbiosRecord19->Hdr.Length = (UINT8) sizeof (SMBIOS_TABLE_TYPE19);
    SmbiosRecord19->Hdr.Handle = 0;

    SmbiosRecord19->MemoryArrayHandle       = Type16Handle;
    SmbiosRecord19->PartitionWidth          = NumSlots;
    if (TotalMemorySize <= SIZE_4TB) {
      SmbiosRecord19->StartingAddress         = 0x0;
      //
      // Convert bytes to KB
      //
      SmbiosRecord19->EndingAddress           = (UINT32)RShiftU64 (TotalMemorySize, 10) - 1;
      SmbiosRecord19->ExtendedStartingAddress = 0;
      SmbiosRecord19->ExtendedEndingAddress   = 0;
    } else {
      SmbiosRecord19->StartingAddress         = 0xffffffff;
      SmbiosRecord19->EndingAddress           = 0xffffffff;
      SmbiosRecord19->ExtendedStartingAddress = 0;
      SmbiosRecord19->ExtendedEndingAddress   = TotalMemorySize - 1;
    }

    //
    // Add SMBIOS type 19 record to SMBIOS table.
    //
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Smbios->Add(
              Smbios,
              NULL,
              &SmbiosHandle,
              (EFI_SMBIOS_TABLE_HEADER *)SmbiosRecord19
              );

    return Status;
}
