/** @file
  Smbios type 1.

Copyright (c) 2018 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SmbiosBasic.h"

/**
  This function makes boot time changes to the contents of the
  SystemManufacturer (Type 1).

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
EFI_STATUS
EFIAPI
SystemManufacturerFunction(
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                      Status;
  CHAR8                           *ManufacturerStr;
  CHAR8                           *ProductNameStr;
  CHAR8                           *VersionStr;
  CHAR8                           *SerialNumberStr;
  CHAR8                           *SKUNumberStr;
  CHAR8                           *FamilyStr;
  UINTN                           ManufacturerStrLen;
  UINTN                           ProductNameStrLen;
  UINTN                           VersionStrLen;
  UINTN                           SerialNumberStrLen;
  UINTN                           SKUNumberStrLen;
  UINTN                           FamilyStrLen;
  UINTN                           TableSize;
  SMBIOS_TABLE_TYPE1              *PcdSmbiosRecord;
  SMBIOS_TABLE_TYPE1              *SmbiosRecord;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  UINTN                           StringOffset;

  PcdSmbiosRecord = PcdGetPtr (PcdSmbiosType1SystemInformation);

  ManufacturerStr = PcdGetPtr (PcdSmbiosType1StringManufacturer);
  ManufacturerStrLen = AsciiStrLen (ManufacturerStr);
  ASSERT (ManufacturerStrLen <= SMBIOS_STRING_MAX_LENGTH);

  ProductNameStr = PcdGetPtr (PcdSmbiosType1StringProductName);
  ProductNameStrLen = AsciiStrLen (ProductNameStr);
  ASSERT (ProductNameStrLen <= SMBIOS_STRING_MAX_LENGTH);

  VersionStr = PcdGetPtr (PcdSmbiosType1StringVersion);
  VersionStrLen = AsciiStrLen (VersionStr);
  ASSERT (VersionStrLen <= SMBIOS_STRING_MAX_LENGTH);

  SerialNumberStr = PcdGetPtr (PcdSmbiosType1StringSerialNumber);
  SerialNumberStrLen = AsciiStrLen (SerialNumberStr);
  ASSERT (SerialNumberStrLen <= SMBIOS_STRING_MAX_LENGTH);

  SKUNumberStr = PcdGetPtr (PcdSmbiosType1StringSKUNumber);
  SKUNumberStrLen = AsciiStrLen (SKUNumberStr);
  ASSERT (SKUNumberStrLen <= SMBIOS_STRING_MAX_LENGTH);

  FamilyStr = PcdGetPtr (PcdSmbiosType1StringFamily);
  FamilyStrLen = AsciiStrLen (FamilyStr);
  ASSERT (FamilyStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Create table size based on string lengths
  //
  TableSize = sizeof (SMBIOS_TABLE_TYPE1) + ManufacturerStrLen + 1 + ProductNameStrLen + 1 + VersionStrLen + 1 + SerialNumberStrLen + 1 + SKUNumberStrLen + 1 + FamilyStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TableSize);
  if (SmbiosRecord == NULL) {
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (SmbiosRecord, PcdSmbiosRecord, sizeof(SMBIOS_TABLE_TYPE1));

  //
  // Fill in Type 1 fields
  //

  SmbiosRecord->Hdr.Type = SMBIOS_TYPE_SYSTEM_INFORMATION;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE1);
  SmbiosRecord->Hdr.Handle = 0;

  //
  // Add strings to bottom of data block
  //
  StringOffset = SmbiosRecord->Hdr.Length;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, ManufacturerStr, ManufacturerStrLen);
  StringOffset += ManufacturerStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, ProductNameStr, ProductNameStrLen);
  StringOffset += ProductNameStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, VersionStr, VersionStrLen);
  StringOffset += VersionStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, SerialNumberStr, SerialNumberStrLen);
  StringOffset += SerialNumberStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, SKUNumberStr, SKUNumberStrLen);
  StringOffset += SKUNumberStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, FamilyStr, FamilyStrLen);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}
