/** @file
*
*  Copyright (c) 2017, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2017, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IpmiCmdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/GlobalVariable.h>
#include <Protocol/DevicePathToText.h>


STATIC
UINT16
EFIAPI
GetBBSTypeFromFileSysPath (
  IN CHAR16                   *UsbPathTxt,
  IN CHAR16                   *FileSysPathTxt,
  IN EFI_DEVICE_PATH_PROTOCOL *FileSysPath
  )
{
  EFI_DEVICE_PATH_PROTOCOL *Node;

  if (StrnCmp (UsbPathTxt, FileSysPathTxt, StrLen (UsbPathTxt)) == 0) {
    Node = FileSysPath;
    while (!IsDevicePathEnd (Node)) {
      if ((DevicePathType (Node) == MEDIA_DEVICE_PATH) &&
          (DevicePathSubType (Node) == MEDIA_CDROM_DP)) {
        return BBS_TYPE_CDROM;
      }
      Node = NextDevicePathNode (Node);
    }
  }

  return BBS_TYPE_UNKNOWN;
}

STATIC
UINT16
EFIAPI
GetBBSTypeFromUsbPath (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *UsbPath
  )
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        *FileSystemHandles;
  UINTN                             NumberFileSystemHandles;
  UINTN                             Index;
  EFI_DEVICE_PATH_PROTOCOL          *FileSysPath;
  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL  *DevPathToText;
  CHAR16                            *UsbPathTxt;
  CHAR16                            *FileSysPathTxt;
  UINT16                            Result;

  Status = gBS->LocateProtocol (
                  &gEfiDevicePathToTextProtocolGuid,
                  NULL,
                  (VOID **) &DevPathToText);
  ASSERT_EFI_ERROR(Status);

  Result = BBS_TYPE_UNKNOWN;
  UsbPathTxt = DevPathToText->ConvertDevicePathToText (UsbPath, TRUE, TRUE);
  if (UsbPathTxt == NULL) {
    return Result;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &NumberFileSystemHandles,
                  &FileSystemHandles
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate SimpleFileSystemProtocol error(%r)\n", Status));
    FreePool (UsbPathTxt);
    return BBS_TYPE_UNKNOWN;
  }

  for (Index = 0; Index < NumberFileSystemHandles; Index++) {
    FileSysPath = DevicePathFromHandle (FileSystemHandles[Index]);
    FileSysPathTxt = DevPathToText->ConvertDevicePathToText (FileSysPath, TRUE, TRUE);

    if (FileSysPathTxt == NULL) {
      continue;
    }

    Result = GetBBSTypeFromFileSysPath (UsbPathTxt, FileSysPathTxt, FileSysPath);
    FreePool (FileSysPathTxt);

    if (Result != BBS_TYPE_UNKNOWN) {
      break;
    }
  }

  if (NumberFileSystemHandles != 0) {
    FreePool (FileSystemHandles);
  }

  FreePool (UsbPathTxt);

  return Result;
}

STATIC
UINT16
EFIAPI
GetBBSTypeFromMessagingDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN EFI_DEVICE_PATH_PROTOCOL *Node
  )
{
  VENDOR_DEVICE_PATH       *Vendor;
  UINT16                   Result;

  Result = BBS_TYPE_UNKNOWN;

  switch (DevicePathSubType (Node)) {
  case MSG_MAC_ADDR_DP:
    Result = BBS_TYPE_EMBEDDED_NETWORK;
    break;

  case MSG_USB_DP:
    Result = GetBBSTypeFromUsbPath (DevicePath);
    if (Result == BBS_TYPE_UNKNOWN) {
      Result = BBS_TYPE_USB;
    }
    break;

  case MSG_SATA_DP:
    Result = BBS_TYPE_HARDDRIVE;
    break;

  case MSG_VENDOR_DP:
    Vendor = (VENDOR_DEVICE_PATH *) (Node);
    if (&Vendor->Guid != NULL) {
      if (CompareGuid (&Vendor->Guid, &((EFI_GUID) DEVICE_PATH_MESSAGING_SAS))) {
        Result = BBS_TYPE_HARDDRIVE;
      }
    }
    break;

  default:
    Result = BBS_TYPE_UNKNOWN;
    break;
  }

  return Result;
}

STATIC
UINT16
EFIAPI
GetBBSTypeByDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL *Node;
  UINT16                   Result;

  Result = BBS_TYPE_UNKNOWN;
  if (DevicePath == NULL) {
    return Result;
  }

  Node = DevicePath;
  while (!IsDevicePathEnd (Node)) {
    switch (DevicePathType (Node)) {
    case MEDIA_DEVICE_PATH:
      if (DevicePathSubType (Node) == MEDIA_CDROM_DP) {
        Result = BBS_TYPE_CDROM;
      }
      break;

    case MESSAGING_DEVICE_PATH:
      Result = GetBBSTypeFromMessagingDevicePath (DevicePath, Node);
      break;

    default:
      Result = BBS_TYPE_UNKNOWN;
      break;
    }

    if (Result != BBS_TYPE_UNKNOWN) {
      break;
    }

    Node = NextDevicePathNode (Node);
  }

  return Result;
}

STATIC
EFI_STATUS
EFIAPI
GetBmcBootOptionsSetting (
  OUT IPMI_GET_BOOT_OPTION *BmcBootOpt
  )
{
  EFI_STATUS   Status;

  Status = IpmiCmdGetSysBootOptions (BmcBootOpt);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get iBMC BootOpts %r!\n", Status));
    return Status;
  }

  if (BmcBootOpt->BootFlagsValid != BOOT_OPTION_BOOT_FLAG_VALID) {
    return EFI_NOT_FOUND;
  }

  if (BmcBootOpt->Persistent) {
    BmcBootOpt->BootFlagsValid = BOOT_OPTION_BOOT_FLAG_VALID;
  } else {
    BmcBootOpt->BootFlagsValid = BOOT_OPTION_BOOT_FLAG_INVALID;
  }

  Status = IpmiCmdSetSysBootOptions (BmcBootOpt);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Set iBMC BootOpts %r!\n", Status));
  }

  return Status;
}

VOID
EFIAPI
RestoreBootOrder (
  VOID
  )
{
  EFI_STATUS                Status;
  UINT16                    *BootOrder;
  UINTN                     BootOrderSize;

  GetVariable2 (
    L"BootOrderBackup",
    &gOemBootVariableGuid,
    (VOID **) &BootOrder,
    &BootOrderSize
    );
  if (BootOrder == NULL) {
    return ;
  }

  Print (L"\nRestore BootOrder(%d).\n", BootOrderSize / sizeof (UINT16));

  Status = gRT->SetVariable (
                  L"BootOrder",
                  &gEfiGlobalVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS
                  | EFI_VARIABLE_NON_VOLATILE,
                  BootOrderSize,
                  BootOrder
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SetVariable BootOrder %r!\n", Status));
  }

  Status = gRT->SetVariable (
                  L"BootOrderBackup",
                  &gOemBootVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                  0,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SetVariable BootOrderBackup %r!\n", Status));
  }

  FreePool (BootOrder);
}


STATIC
VOID
EFIAPI
RestoreBootOrderOnReadyToBoot (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  // restore BootOrder variable in normal condition.
  RestoreBootOrder ();
}

STATIC
VOID
EFIAPI
UpdateBootOrder (
  IN UINT16  *NewOrder,
  IN UINT16  *BootOrder,
  IN UINTN   BootOrderSize
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  Status = gRT->SetVariable (
                  L"BootOrderBackup",
                  &gOemBootVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                  BootOrderSize,
                  BootOrder
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Set BootOrderBackup Variable:%r!\n", Status));
    return;
  }

  Status = gRT->SetVariable (
                  L"BootOrder",
                  &gEfiGlobalVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS
                  | EFI_VARIABLE_NON_VOLATILE,
                  BootOrderSize,
                  NewOrder
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Set BootOrder Variable:%r!\n", Status));
    return;
  }

  // Register notify function to restore BootOrder variable on ReadyToBoot Event.
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  RestoreBootOrderOnReadyToBoot,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Event
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Create ready to boot event %r!\n", Status));
  }
}

STATIC
VOID
EFIAPI
SetBootOrder (
  IN UINT16 BootType
  )
{
  EFI_STATUS                   Status;
  UINT16                       *NewOrder;
  UINT16                       *RemainBoots;
  UINT16                       *BootOrder;
  UINTN                        BootOrderSize;
  EFI_BOOT_MANAGER_LOAD_OPTION Option;
  CHAR16                       OptionName[sizeof ("Boot####")];
  UINTN                        Index;
  UINTN                        SelectCnt;
  UINTN                        RemainCnt;

  GetEfiGlobalVariable2 (L"BootOrder", (VOID **) &BootOrder, &BootOrderSize);
  if (BootOrder == NULL) {
    return ;
  }

  NewOrder = AllocatePool (BootOrderSize);
  RemainBoots = AllocatePool (BootOrderSize);
  if ((NewOrder == NULL) || (RemainBoots == NULL)) {
    DEBUG ((DEBUG_ERROR, "Out of resources."));
    goto Exit;
  }

  SelectCnt = 0;
  RemainCnt = 0;

  for (Index = 0; Index < BootOrderSize / sizeof (UINT16); Index++) {
    UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04x", BootOrder[Index]);
    Status = EfiBootManagerVariableToLoadOption (OptionName, &Option);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Boot%04x is invalid option!\n", BootOrder[Index]));
      continue;
    }

    if (GetBBSTypeByDevicePath (Option.FilePath) == BootType) {
      NewOrder[SelectCnt++] = BootOrder[Index];
    } else {
      RemainBoots[RemainCnt++] = BootOrder[Index];
    }
  }

  if (SelectCnt != 0) {
    // append RemainBoots to NewOrder
    for (Index = 0; Index < RemainCnt; Index++) {
      NewOrder[SelectCnt + Index] = RemainBoots[Index];
    }

    if (CompareMem (NewOrder, BootOrder, BootOrderSize) != 0) {
      UpdateBootOrder (NewOrder, BootOrder, BootOrderSize);
    }
  }

Exit:
  FreePool (BootOrder);
  if (NewOrder != NULL) {
    FreePool (NewOrder);
  }
  if (RemainBoots != NULL) {
    FreePool (RemainBoots);
  }
}

VOID
EFIAPI
HandleBmcBootType (
  VOID
  )
{
  EFI_STATUS                Status;
  IPMI_GET_BOOT_OPTION      BmcBootOpt;
  UINT16                    BootType;

  Status = GetBmcBootOptionsSetting (&BmcBootOpt);
  if (EFI_ERROR (Status)) {
    return;
  }

  Print (L"Boot Type from BMC is %x\n", BmcBootOpt.BootDeviceSelector);

  switch (BmcBootOpt.BootDeviceSelector) {
  case ForcePxe:
    BootType = BBS_TYPE_EMBEDDED_NETWORK;
    break;

  case ForcePrimaryRemovableMedia:
    BootType = BBS_TYPE_USB;
    break;

  case ForceDefaultHardDisk:
    BootType = BBS_TYPE_HARDDRIVE;
    break;

  case ForceDefaultCD:
    BootType = BBS_TYPE_CDROM;
    break;

  default:
    return;
  }

  SetBootOrder (BootType);
}

