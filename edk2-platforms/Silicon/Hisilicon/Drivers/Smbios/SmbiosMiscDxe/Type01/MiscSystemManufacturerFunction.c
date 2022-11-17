/*++

Copyright (c) 2006 - 2011, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2015, Hisilicon Limited. All rights reserved.<BR>
Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  MiscSystemManufacturerFunction.c

Abstract:

  This driver parses the mMiscSubclassDataTable structure and reports
  any generated data to smbios.

Based on files under Nt32Pkg/MiscSubClassPlatformDxe/

**/

#include "SmbiosMisc.h"

/**
  This function makes boot time changes to the contents of the
  MiscSystemManufacturer (Type 1).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscSystemManufacturer)
{
    CHAR8                           *OptionalStrStart;
    UINTN                           ManuStrLen;
    UINTN                           VerStrLen;
    UINTN                           PdNameStrLen;
    UINTN                           SerialNumStrLen;
    UINTN                           SKUNumStrLen;
    UINTN                           FamilyStrLen;
    EFI_STRING                      Manufacturer;
    EFI_STRING                      ProductName;
    EFI_STRING                      Version;
    EFI_STRING                      SerialNumber;
    EFI_STRING                      SKUNumber;
    EFI_STRING                      Family;
    EFI_STRING_ID                   TokenToGet;
    EFI_SMBIOS_HANDLE               SmbiosHandle;
    SMBIOS_TABLE_TYPE1              *SmbiosRecord;
    SMBIOS_TABLE_TYPE1              *InputData;
    EFI_STATUS                      Status;
    EFI_STRING_ID                   TokenToUpdate;
    CHAR16                          *Product;
    CHAR16                          *pVersion;

    EFI_GUID                        Uuid;

    //
    // First check for invalid parameters.
    //
    if (RecordData == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    InputData = (SMBIOS_TABLE_TYPE1 *)RecordData;

    Product = (CHAR16 *) PcdGetPtr (PcdSystemProductName);
    if (StrLen(Product) > 0)
    {
        TokenToUpdate = STRING_TOKEN (STR_MISC_SYSTEM_PRODUCT_NAME);
        HiiSetString (mHiiHandle, TokenToUpdate, Product, NULL);
    }

    pVersion = (CHAR16 *) PcdGetPtr (PcdSystemVersion);
    if (StrLen(pVersion) > 0)
    {
        TokenToUpdate = STRING_TOKEN (STR_MISC_SYSTEM_VERSION);
        HiiSetString (mHiiHandle, TokenToUpdate, pVersion, NULL);
    }
    UpdateSmbiosInfo(mHiiHandle, STRING_TOKEN (STR_MISC_SYSTEM_SERIAL_NUMBER), SerialNumType01);
    UpdateSmbiosInfo(mHiiHandle, STRING_TOKEN (STR_MISC_SYSTEM_MANUFACTURER), SystemManufacturerType01);

    TokenToGet   = STRING_TOKEN (STR_MISC_SYSTEM_MANUFACTURER);
    Manufacturer = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
    ManuStrLen   = StrLen(Manufacturer);

    TokenToGet   = STRING_TOKEN (STR_MISC_SYSTEM_PRODUCT_NAME);
    ProductName  = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
    PdNameStrLen = StrLen(ProductName);

    TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_VERSION);
    Version    = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
    VerStrLen  = StrLen(Version);

    TokenToGet      = STRING_TOKEN (STR_MISC_SYSTEM_SERIAL_NUMBER);
    SerialNumber    = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
    SerialNumStrLen = StrLen(SerialNumber);

    TokenToGet   = STRING_TOKEN (STR_MISC_SYSTEM_SKU_NUMBER);
    SKUNumber    = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
    SKUNumStrLen = StrLen(SKUNumber);

    TokenToGet   = STRING_TOKEN (STR_MISC_SYSTEM_FAMILY);
    Family       = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
    FamilyStrLen = StrLen(Family);

    //
    // Two zeros following the last string.
    //
    SmbiosRecord = AllocateZeroPool(sizeof (SMBIOS_TABLE_TYPE1) + ManuStrLen      + 1
                                                                + PdNameStrLen    + 1
                                                                + VerStrLen       + 1
                                                                + SerialNumStrLen + 1
                                                                + SKUNumStrLen    + 1
                                                                + FamilyStrLen    + 1 + 1);

    if (NULL == SmbiosRecord)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    (VOID)CopyMem(SmbiosRecord, InputData, sizeof (SMBIOS_TABLE_TYPE1));

    SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE1);

    SmbiosRecord->Uuid = InputData->Uuid;
    Status = GetUuidType1 (&Uuid);
    if (!EFI_ERROR (Status))
    {
        SmbiosRecord->Uuid = Uuid;
    }

    OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
    Status = UnicodeStrToAsciiStrS (Manufacturer, OptionalStrStart, ManuStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (ProductName,  OptionalStrStart + ManuStrLen + 1, PdNameStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (Version,      OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1, VerStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (SerialNumber, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1, SerialNumStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (SKUNumber,    OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1, SKUNumStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    Status = UnicodeStrToAsciiStrS (Family,       OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + SKUNumStrLen + 1, FamilyStrLen + 1);
    ASSERT_EFI_ERROR (Status);

    //
    // Now we have got the full smbios record, call smbios protocol to add this record.
    //
    Status = LogSmbiosData( (UINT8*)SmbiosRecord, &SmbiosHandle);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Smbios Type01 Table Log Failed! %r \n", __FUNCTION__, __LINE__, Status));
    }

    FreePool(SmbiosRecord);

Exit:
    if(Manufacturer != NULL)
    {
        FreePool(Manufacturer);
    }

    if(ProductName != NULL)
    {
        FreePool(ProductName);
    }

    if(Version != NULL)
    {
        FreePool(Version);
    }

    if(SerialNumber != NULL)
    {
        FreePool(SerialNumber);
    }

    if(SKUNumber != NULL)
    {
        FreePool(SKUNumber);
    }

    if(Family != NULL)
    {
        FreePool(Family);
    }

    return Status;
}
