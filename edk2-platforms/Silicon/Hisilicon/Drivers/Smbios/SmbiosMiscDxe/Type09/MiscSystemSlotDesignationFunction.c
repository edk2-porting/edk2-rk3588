/** @file
  BIOS system slot designator information boot time changes.
  SMBIOS type 9.

  Copyright (c) 2009 - 2011, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2016, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2016, Linaro Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SmbiosMisc.h"

#include <Library/OemMiscLib.h>

extern SMBIOS_TABLE_TYPE9 MiscSystemSlotDesignationPcie0Data;
extern SMBIOS_TABLE_TYPE9 MiscSystemSlotDesignationPcie1Data;
extern SMBIOS_TABLE_TYPE9 MiscSystemSlotDesignationPcie2Data;
extern SMBIOS_TABLE_TYPE9 MiscSystemSlotDesignationPcie3Data;

VOID
UpdateSlotDesignation (
  IN SMBIOS_TABLE_TYPE9 *InputData
  )
{
    EFI_STRING_ID                   TokenToUpdate;
    CHAR16                          *SlotDesignation;
    UINTN                           DesignationStrLen;

    SlotDesignation = AllocateZeroPool ((sizeof (CHAR16)) * SMBIOS_STRING_MAX_LENGTH);
    if (NULL == SlotDesignation)
    {
        return;
    }

    if ((UINTN)InputData == (UINTN)&MiscSystemSlotDesignationPcie0Data)
    {
        UnicodeSPrint(SlotDesignation, SMBIOS_STRING_MAX_LENGTH - 1, L"PCIE0");
    }
    else if ((UINTN)InputData == (UINTN)&MiscSystemSlotDesignationPcie1Data)
    {
        UnicodeSPrint(SlotDesignation, SMBIOS_STRING_MAX_LENGTH - 1, L"PCIE1");
    }
    else if ((UINTN)InputData == (UINTN)&MiscSystemSlotDesignationPcie2Data)
    {
        UnicodeSPrint(SlotDesignation, SMBIOS_STRING_MAX_LENGTH - 1, L"PCIE2");
    }
    else if ((UINTN)InputData == (UINTN)&MiscSystemSlotDesignationPcie3Data)
    {
        UnicodeSPrint(SlotDesignation, SMBIOS_STRING_MAX_LENGTH - 1, L"PCIE3");
    }

    DesignationStrLen = StrLen (SlotDesignation);

    if (DesignationStrLen > 0 )
    {
        TokenToUpdate = STRING_TOKEN (STR_MISC_SYSTEM_SLOT_DESIGNATION);
        HiiSetString (mHiiHandle, TokenToUpdate, SlotDesignation, NULL);
    }

    FreePool (SlotDesignation);
}

VOID
UpdateSlotUsage(
  IN OUT SMBIOS_TABLE_TYPE9 *InputData
  )
{
    EFI_STATUS        Status;
    SERDES_PARAM    SerdesParamA;
    SERDES_PARAM    SerdesParamB;

    Status = OemGetSerdesParam (&SerdesParamA, &SerdesParamB, 0);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] OemGetSerdesParam failed %r\n", __FUNCTION__, __LINE__, Status));
        return;
    }

    //
    // PCIE0
    //
    if (((UINTN)InputData == (UINTN)&MiscSystemSlotDesignationPcie0Data)
        && SerdesParamA.Hilink1Mode == EmHilink1Pcie0X8) {
        InputData->CurrentUsage = SlotUsageAvailable;
    }

    //
    // PCIE1
    //
    if ((UINTN)InputData == (UINTN)&MiscSystemSlotDesignationPcie1Data)
    {
        if (SerdesParamA.Hilink0Mode == EmHilink0Pcie1X4Pcie2X4) {
            InputData->SlotDataBusWidth = SlotDataBusWidth4X;
        }
    }

    //
    // PCIE2
    //
    if ((UINTN)InputData == (UINTN)&MiscSystemSlotDesignationPcie2Data)
    {
        if (SerdesParamA.Hilink0Mode == EmHilink0Pcie1X4Pcie2X4) {
            InputData->SlotDataBusWidth = SlotDataBusWidth4X;
            InputData->CurrentUsage = SlotUsageAvailable;
        } else if (SerdesParamA.Hilink2Mode == EmHilink2Pcie2X8) {
            InputData->CurrentUsage = SlotUsageAvailable;
        }
    }

    //
    // PCIE3
    //
    if (((UINTN)InputData == (UINTN)&MiscSystemSlotDesignationPcie3Data)
        && SerdesParamA.Hilink5Mode == EmHilink5Pcie3X4) {
        InputData->CurrentUsage = SlotUsageAvailable;
    }
}

/**
  This function makes boot time changes to the contents of the
  MiscSystemSlotDesignator structure (Type 9).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscSystemSlotDesignation)
{
    CHAR8                              *OptionalStrStart;
    UINTN                              SlotDesignationStrLen;
    EFI_STATUS                         Status;
    EFI_STRING                         SlotDesignation;
    EFI_STRING_ID                      TokenToGet;
    SMBIOS_TABLE_TYPE9                 *SmbiosRecord;
    EFI_SMBIOS_HANDLE                  SmbiosHandle;
    SMBIOS_TABLE_TYPE9                 *InputData = NULL;

    //
    // First check for invalid parameters.
    //
    if (RecordData == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }

    InputData = (SMBIOS_TABLE_TYPE9 *)RecordData;

    UpdateSlotUsage (InputData);

    UpdateSlotDesignation (InputData);

    TokenToGet   = STRING_TOKEN (STR_MISC_SYSTEM_SLOT_DESIGNATION);
    SlotDesignation = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
    SlotDesignationStrLen = StrLen(SlotDesignation);

    //
    // Two zeros following the last string.
    //
    SmbiosRecord = AllocateZeroPool(sizeof (SMBIOS_TABLE_TYPE9) + SlotDesignationStrLen + 1 + 1);
    if(NULL == SmbiosRecord)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    (VOID)CopyMem(SmbiosRecord, InputData, sizeof (SMBIOS_TABLE_TYPE9));

    SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE9);

    OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
    Status = UnicodeStrToAsciiStrS (SlotDesignation, OptionalStrStart, SlotDesignationStrLen + 1);
    ASSERT_EFI_ERROR (Status);
    //
    // Now we have got the full smbios record, call smbios protocol to add this record.
    //

    Status = LogSmbiosData( (UINT8*)SmbiosRecord, &SmbiosHandle);
    if(EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "[%a]:[%dL] Smbios Type09 Table Log Failed! %r \n", __FUNCTION__, __LINE__, Status));
    }

    FreePool(SmbiosRecord);

Exit:
    if(SlotDesignation != NULL)
    {
      FreePool(SlotDesignation);
    }

    return Status;
}
