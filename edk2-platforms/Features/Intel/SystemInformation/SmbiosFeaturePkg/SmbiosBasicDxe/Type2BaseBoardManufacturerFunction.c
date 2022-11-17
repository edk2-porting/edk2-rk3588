/** @file
  Smbios type 2.

Copyright (c) 2018 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SmbiosBasic.h"

/**
  This function makes boot time changes to the contents of the
  BaseBoardManufacturer (Type 2).

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
EFI_STATUS
EFIAPI
BaseBoardManufacturerFunction(
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                          Status;
  CHAR8                               *ManufacturerStr;
  CHAR8                               *ProductStr;
  CHAR8                               *VersionStr;
  CHAR8                               *SerialNumberStr;
  CHAR8                               *AssertTagStr;
  CHAR8                               *ChassisStr;
  UINTN                               ManuStrLen;
  UINTN                               ProductStrLen;
  UINTN                               VerStrLen;
  UINTN                               AssertTagStrLen;
  UINTN                               SerialNumStrLen;
  UINTN                               ChassisStrLen;
  EFI_SMBIOS_HANDLE                   SmbiosHandle;
  SMBIOS_TABLE_TYPE2                  *PcdSmbiosRecord;
  SMBIOS_TABLE_TYPE2                  *SmbiosRecord;
  UINTN                               SourceSize;
  UINTN                               TotalSize;
  UINTN                               StringOffset;

  PcdSmbiosRecord = PcdGetPtr (PcdSmbiosType2BaseBoardInformation);

  //
  // Get BoardManufacturer String.
  //
  ManufacturerStr = PcdGetPtr (PcdSmbiosType2StringManufacturer);
  ManuStrLen = AsciiStrLen (ManufacturerStr);
  ASSERT (ManuStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get Board ProductName String.
  //
  ProductStr = PcdGetPtr (PcdSmbiosType2StringProductName);
  ProductStrLen = AsciiStrLen (ProductStr);
  ASSERT (ProductStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get Board Version String.
  //
  VersionStr = PcdGetPtr (PcdSmbiosType2StringVersion);
  VerStrLen = AsciiStrLen (VersionStr);
  ASSERT (VerStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get Board Serial Number String.
  //
  SerialNumberStr = PcdGetPtr (PcdSmbiosType2StringSerialNumber);
  SerialNumStrLen = AsciiStrLen (SerialNumberStr);
  ASSERT (SerialNumStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get Board Asset Tag String.
  //
  AssertTagStr = PcdGetPtr (PcdSmbiosType2StringAssetTag);
  AssertTagStrLen = AsciiStrLen (AssertTagStr);
  ASSERT (AssertTagStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get Board Chassis Location Tag String.
  //
  ChassisStr = PcdGetPtr (PcdSmbiosType2StringLocationInChassis);
  ChassisStrLen = AsciiStrLen (ChassisStr);
  ASSERT (ChassisStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Two zeros following the last string.
  //
  SourceSize = PcdGetSize (PcdSmbiosType2BaseBoardInformation);
  TotalSize = SourceSize + ManuStrLen + 1 + ProductStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + AssertTagStrLen + 1 + ChassisStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool(TotalSize);
  if (SmbiosRecord == NULL) {
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (SmbiosRecord, PcdSmbiosRecord, SourceSize);

  SmbiosRecord->Hdr.Type = SMBIOS_TYPE_BASEBOARD_INFORMATION;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE2);
  if (PcdSmbiosRecord->NumberOfContainedObjectHandles >= 2) {
    SmbiosRecord->Hdr.Length += (PcdSmbiosRecord->NumberOfContainedObjectHandles - 1) * sizeof(PcdSmbiosRecord->ContainedObjectHandles);
  }
  ASSERT(SourceSize >= SmbiosRecord->Hdr.Length);
  SmbiosRecord->Hdr.Handle = 0;

  StringOffset = SmbiosRecord->Hdr.Length;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, ManufacturerStr, ManuStrLen);
  StringOffset += ManuStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, ProductStr, ProductStrLen);
  StringOffset += ProductStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, VersionStr, VerStrLen);
  StringOffset += VerStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, SerialNumberStr, SerialNumStrLen);
  StringOffset += SerialNumStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, AssertTagStr, AssertTagStrLen);
  StringOffset += AssertTagStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, ChassisStr, ChassisStrLen);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}
