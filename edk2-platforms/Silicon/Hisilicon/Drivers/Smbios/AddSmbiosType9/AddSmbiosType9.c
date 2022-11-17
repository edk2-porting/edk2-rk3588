/** @file
*
*  Copyright (c) 2015 - 2020, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "AddSmbiosType9.h"

extern SMBIOS_TABLE_TYPE9 gPcieSlotInfo[];
extern UINT8 OemGetPcieSlotNumber ();

VOID
EFIAPI
UpdateSmbiosType9Info (
  IN OUT SMBIOS_TABLE_TYPE9             *Type9Record
)
{
  EFI_STATUS                         Status;
  UINTN                              HandleIndex;
  EFI_HANDLE                        *HandleBuffer;
  UINTN                              HandleCount;
  EFI_PCI_IO_PROTOCOL               *PciIo;
  UINTN                              SegmentNumber;
  UINTN                              BusNumber;
  UINTN                              DeviceNumber;
  UINTN                              FunctionNumber;
  UINTN                              Index;
  REPORT_PCIEDIDVID2BMC              ReportPcieDidVid[PCIEDEVICE_REPORT_MAX];

  GetPciDidVid ((VOID *)ReportPcieDidVid);

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " Locate gEfiPciIoProtocol Failed.\n"));
    gBS->FreePool ((VOID *)HandleBuffer);
    return;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Status : %r\n", __FUNCTION__, __LINE__, Status));
      continue;
    }
    (VOID)PciIo->GetLocation (PciIo, &SegmentNumber, &BusNumber, &DeviceNumber, &FunctionNumber);
    for (Index = 0; Index < sizeof(ReportPcieDidVid) / sizeof(REPORT_PCIEDIDVID2BMC); Index++) {
      if (Type9Record->SlotID == ReportPcieDidVid[Index].Slot + 1) {
        if ((BusNumber == ReportPcieDidVid[Index].Bus) && (DeviceNumber == ReportPcieDidVid[Index].Device)) {
          DEBUG ((DEBUG_ERROR, "PCIe device plot in slot Seg %d  bdf %d %d %d\r\n",
            SegmentNumber, BusNumber, DeviceNumber, FunctionNumber));
          Type9Record->SegmentGroupNum   = SegmentNumber;
          Type9Record->BusNum            = BusNumber;
          Type9Record->DevFuncNum        = (DeviceNumber << 3) | FunctionNumber;
          Type9Record->CurrentUsage      = SlotUsageInUse;
          break;
        }
      }
    }
  }

  gBS->FreePool ((VOID *)HandleBuffer);
  return;
}

STATIC
VOID
EmptySmbiosType9 (
  EFI_SMBIOS_PROTOCOL                 *Smbios
  )
{
  EFI_STATUS                          Status;
  EFI_SMBIOS_TYPE                     SmbiosType;
  EFI_SMBIOS_HANDLE                   SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER             *Record;

  do {
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    SmbiosType = EFI_SMBIOS_TYPE_SYSTEM_SLOTS;
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, &SmbiosType, &Record, NULL);
    if (!EFI_ERROR (Status)) {
      Status = Smbios->Remove (Smbios, SmbiosHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Remove System Slot Failed. Status : %r\n",
          __FUNCTION__, __LINE__, Status));
        break;
      }
    }
  } while (SmbiosHandle != SMBIOS_HANDLE_PI_RESERVED);

  return;
}

STATIC
EFI_STATUS
AddSmbiosType9Record (
  EFI_SMBIOS_PROTOCOL                 *Smbios,
  SMBIOS_TABLE_TYPE9                  *Type9Record
  )
{
  EFI_STATUS                          Status;
  EFI_SMBIOS_HANDLE                   SmbiosHandle;
  SMBIOS_TABLE_TYPE9                  *SmbiosRecord;
  CHAR8                               *OptionalStrStart;
  CHAR16                              SlotDesignation[SMBIOS_STRING_MAX_LENGTH];
  UINTN                               SlotStrLen;

  SlotStrLen = UnicodeSPrint (
                 SlotDesignation,
                 SMBIOS_STRING_MAX_LENGTH * 2,
                 L"PCIE Slot%d",
                 Type9Record->SlotID);

  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocateZeroPool (sizeof (SMBIOS_TABLE_TYPE9) + SlotStrLen + 1 + 1);
  if (SmbiosRecord == NULL) {
    DEBUG ((DEBUG_ERROR, "AllocateZeroPool Failed for SmbiosRecord.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  (VOID)CopyMem (SmbiosRecord, Type9Record, sizeof (SMBIOS_TABLE_TYPE9));
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE9);
  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  Status = UnicodeStrToAsciiStrS (SlotDesignation, OptionalStrStart, SlotStrLen + 1);
  ASSERT_EFI_ERROR (Status);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = Smbios->Add (Smbios, NULL, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *)SmbiosRecord);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Add Smbios Type09 Failed! %r\n", Status));
  }

  FreePool (SmbiosRecord);
  return Status;
}

STATIC
VOID
HandleSmbiosType9 (
  EFI_SMBIOS_PROTOCOL                 *Smbios
  )
{
  EFI_STATUS                          Status;
  SMBIOS_TABLE_TYPE9                  *Type9Record;
  UINT8                               RecordCount;
  UINT8                               Index;

  RecordCount = OemGetPcieSlotNumber ();
  if (RecordCount == 0) {
    return;
  }

  EmptySmbiosType9 (Smbios);
  Status = EFI_SUCCESS;
  for (Index = 0; Index < RecordCount; Index++) {
    if (gPcieSlotInfo[Index].Hdr.Type != EFI_SMBIOS_TYPE_SYSTEM_SLOTS) {
      continue;
    }

    Type9Record = &gPcieSlotInfo[Index];

    UpdateSmbiosType9Info (Type9Record);

    Status = AddSmbiosType9Record (Smbios, Type9Record);
    if (EFI_ERROR (Status)) {
      break;
    }
  }

  return;
}

EFI_STATUS
EFIAPI
AddSmbiosType9Entry (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS                          Status;
  EFI_SMBIOS_PROTOCOL                 *Smbios;

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a]:[%dL] LocateProtocol Failed. Status : %r\n",
        __FUNCTION__, __LINE__, Status));
      return Status;
  }

  HandleSmbiosType9 (Smbios);

  return Status;
}

