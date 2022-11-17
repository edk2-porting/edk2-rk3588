/** @file
  Smbios type 0.

Copyright (c) 2018 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SmbiosBasic.h"

/**
  This function makes boot time changes to the contents of the
  BiosVendor (Type 0).

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
EFI_STATUS
EFIAPI
BiosVendorFunction(
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS            Status;
  CHAR8                 *VendorStr;
  UINTN                 VendorStrLen;
  CHAR8                 *VersionStr;
  UINTN                 VerStrLen;
  CHAR8                 *DateStr;
  UINTN                 DateStrLen;
  SMBIOS_TABLE_TYPE0    *SmbiosRecord;
  SMBIOS_TABLE_TYPE0    *PcdSmbiosRecord;
  EFI_SMBIOS_HANDLE     SmbiosHandle;
  UINTN                 StringOffset;

  PcdSmbiosRecord = PcdGetPtr (PcdSmbiosType0BiosInformation);

  VendorStr = PcdGetPtr (PcdSmbiosType0StringVendor);
  VendorStrLen = AsciiStrLen (VendorStr);
  ASSERT (VendorStrLen <= SMBIOS_STRING_MAX_LENGTH);

  VersionStr = PcdGetPtr (PcdSmbiosType0StringBiosVersion);
  VerStrLen = AsciiStrLen (VersionStr);
  ASSERT (VerStrLen <= SMBIOS_STRING_MAX_LENGTH);

  DateStr = PcdGetPtr (PcdSmbiosType0StringBiosReleaseDate);
  DateStrLen = AsciiStrLen (DateStr);
  ASSERT (DateStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocateZeroPool (sizeof (SMBIOS_TABLE_TYPE0) + VendorStrLen + 1 + VerStrLen + 1 + DateStrLen + 1 + 1);
  if (SmbiosRecord == NULL) {
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (SmbiosRecord, PcdSmbiosRecord, sizeof(SMBIOS_TABLE_TYPE0));

  SmbiosRecord->Hdr.Type = SMBIOS_TYPE_BIOS_INFORMATION;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE0);
  SmbiosRecord->Hdr.Handle = 0;

  StringOffset = SmbiosRecord->Hdr.Length;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, VendorStr, VendorStrLen);
  StringOffset += VendorStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, VersionStr, VerStrLen);
  StringOffset += VerStrLen + 1;
  CopyMem ((UINT8 *)SmbiosRecord + StringOffset, DateStr, DateStrLen);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool (SmbiosRecord);
  return Status;
}
