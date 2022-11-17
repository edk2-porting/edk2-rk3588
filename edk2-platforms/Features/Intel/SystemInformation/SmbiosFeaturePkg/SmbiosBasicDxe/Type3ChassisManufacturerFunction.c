/** @file
  Smbios type 3.

Copyright (c) 2018 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SmbiosBasic.h"

/**
  This function makes boot time changes to the contents of the
  ChassisManufacturer (Type 3).

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
EFI_STATUS
EFIAPI
ChassisManufacturerFunction(
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  UINTN                           ManuStrLen;
  UINTN                           VerStrLen;
  UINTN                           AssertTagStrLen;
  UINTN                           SerialNumStrLen;
  UINTN                           SKUNumberStrLen;
  EFI_STATUS                      Status;
  CHAR8                           *ManufacturerStr;
  CHAR8                           *VersionStr;
  CHAR8                           *SerialNumberStr;
  CHAR8                           *AssertTagStr;
  CHAR8                           *SKUNumberStr;
  SMBIOS_TABLE_STRING             *SKUNumberPtr;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  SMBIOS_TABLE_TYPE3              *SmbiosRecord;
  SMBIOS_TABLE_TYPE3              *PcdSmbiosRecord;
  UINTN                           SourceSize;
  UINTN                           TotalSize;
  UINTN                           StringOffset;

  PcdSmbiosRecord = PcdGetPtr (PcdSmbiosType3SystemEnclosureChassis);

  //
  // Get ChassisManufacturer String.
  //
  ManufacturerStr = PcdGetPtr (PcdSmbiosType3StringManufacturer);
  ManuStrLen = AsciiStrLen (ManufacturerStr);
  ASSERT (ManuStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get ChassisVersion String.
  //
  VersionStr = PcdGetPtr (PcdSmbiosType3StringVersion);
  VerStrLen = AsciiStrLen (VersionStr);
  ASSERT (VerStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get ChassisSerialNumber String.
  //
  SerialNumberStr = PcdGetPtr (PcdSmbiosType3StringSerialNumber);
  SerialNumStrLen = AsciiStrLen (SerialNumberStr);
  ASSERT (SerialNumStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get ChassisAssetTag String.
  //
  AssertTagStr = PcdGetPtr (PcdSmbiosType3StringAssetTag);
  AssertTagStrLen = AsciiStrLen (AssertTagStr);
  ASSERT (AssertTagStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get ChassisSKUNumber String.
  //
  SKUNumberStr = PcdGetPtr (PcdSmbiosType3StringSKUNumber);
  SKUNumberStrLen = AsciiStrLen (SKUNumberStr);
  ASSERT (SKUNumberStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Two zeros following the last string.
  //
  SourceSize = PcdGetSize(PcdSmbiosType3SystemEnclosureChassis);
  TotalSize = SourceSize + sizeof(SMBIOS_TABLE_STRING) + ManuStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + AssertTagStrLen + 1 + SKUNumberStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool(TotalSize);
  if (SmbiosRecord == NULL) {
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (SmbiosRecord, PcdSmbiosRecord, SourceSize);

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE;
  SmbiosRecord->Hdr.Length = OFFSET_OF (SMBIOS_TABLE_TYPE3, ContainedElements) + sizeof(SMBIOS_TABLE_STRING);
  if (PcdSmbiosRecord->ContainedElementCount >= 1) {
    SmbiosRecord->Hdr.Length += PcdSmbiosRecord->ContainedElementCount * PcdSmbiosRecord->ContainedElementRecordLength;
  }
  SmbiosRecord->Hdr.Handle = 0;

  if ((PcdSmbiosRecord->ContainedElementCount == 0) || (SourceSize < (UINTN)SmbiosRecord + SmbiosRecord->Hdr.Length)) {
    SKUNumberPtr = (SMBIOS_TABLE_STRING *)((UINTN)SmbiosRecord + SmbiosRecord->Hdr.Length - sizeof(SMBIOS_TABLE_STRING));
    *SKUNumberPtr = 5;
  }

  StringOffset = SmbiosRecord->Hdr.Length;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, ManufacturerStr, ManuStrLen);
  StringOffset += ManuStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, VersionStr, VerStrLen);
  StringOffset += VerStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, SerialNumberStr, SerialNumStrLen);
  StringOffset += SerialNumStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, AssertTagStr, AssertTagStrLen);
  StringOffset += AssertTagStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, SKUNumberStr, SKUNumberStrLen);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}
