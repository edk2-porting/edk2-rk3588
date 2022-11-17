/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>

BOOLEAN
IsDevicePathExist (
  IN EFI_DEVICE_PATH_PROTOCOL          *DevicePath
  );

CHAR16  *mLoadOptionVariableList[] = {
  L"Boot",
  L"Driver",
  L"SysPrep",
};

CHAR16  *mPlatformRecoveryOptionVariableList[] = {
  L"PlatformRecovery", // does not have PlatformRecoveryOrder
};

CHAR16  *mOsRecoveryOptionVariableList[] = {
  L"OsRecovery", // VariableGuid in OsRecoveryOrder
};

CHAR16  *mKeyOptionVariableList[] = {
  L"Key", // does not have KeyOrder
};

VOID
DumpLoadOption (
  IN CHAR16           *OptionName,
  IN EFI_LOAD_OPTION  *LoadOption,
  IN UINTN            Size
  )
{
  CHAR16                        *Str;
  CHAR16                        *Description;
  EFI_DEVICE_PATH_PROTOCOL      *FilePathList;

  if (LoadOption == NULL) {
    DEBUG ((DEBUG_INFO, "%s: <NULL>\n", OptionName));
    return ;
  }

  Description = (CHAR16 *)(LoadOption + 1);

  FilePathList = (EFI_DEVICE_PATH_PROTOCOL *)((UINTN)Description + StrSize (Description));

  DEBUG ((DEBUG_INFO, "%s: [0x%08x] \"%s\"", OptionName, LoadOption->Attributes, Description));
  Str = ConvertDevicePathToText(FilePathList, TRUE, TRUE);
  DEBUG ((DEBUG_INFO, " %s\n", Str));
  if (Str != NULL) {
    FreePool (Str);
  }
}

EFI_STATUS
TestPointCheckLoadOption (
  IN CHAR16           *OptionName,
  IN EFI_LOAD_OPTION  *LoadOption,
  IN UINTN            Size
  )
{
  CHAR16                        *Description;
  EFI_DEVICE_PATH_PROTOCOL      *FilePathList;

  if (LoadOption == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Description = (CHAR16 *)(LoadOption + 1);

  FilePathList = (EFI_DEVICE_PATH_PROTOCOL *)((UINTN)Description + StrSize (Description));

  if (!IsDevicePathExist (FilePathList)) {
    DEBUG ((DEBUG_ERROR, "DevicePath not found!\n"));
    return EFI_INVALID_PARAMETER;
  }
  return EFI_SUCCESS;
}

VOID
DumpKeyOption (
  IN CHAR16           *OptionName,
  IN EFI_KEY_OPTION   *KeyOption,
  IN UINTN            Size
  )
{
  UINTN               Index;
  EFI_INPUT_KEY       *Keys;

  Keys = (EFI_INPUT_KEY *)(KeyOption + 1);

  DEBUG ((DEBUG_INFO, "%s: [Boot%04x] 0x%08x", OptionName, KeyOption->BootOption, KeyOption->KeyData.PackedValue));
  for (Index = 0; Index < KeyOption->KeyData.Options.InputKeyCount; Index++) {
    DEBUG ((DEBUG_INFO, " (%04x/%04x)", Keys[Index].ScanCode, Keys[Index].UnicodeChar));
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

EFI_STATUS
TestPointCheckLoadOptionVariable (
  VOID
  )
{
  UINT16      *BootOrder;
  UINTN       OrderSize;
  VOID        *Variable;
  UINTN       ListIndex;
  UINTN       Size;
  UINTN       Index;
  EFI_STATUS  Status;
  CHAR16      BootOptionName[sizeof(L"PlatformRecovery####")];
  CHAR16      BootOrderName[sizeof(L"PlatformRecoveryOrder")];
  EFI_STATUS  ReturnStatus;

  ReturnStatus = EFI_SUCCESS;
  for (ListIndex = 0; ListIndex < sizeof(mLoadOptionVariableList)/sizeof(mLoadOptionVariableList[0]); ListIndex++) {
    UnicodeSPrint (BootOrderName, sizeof(BootOrderName), L"%sOrder", mLoadOptionVariableList[ListIndex]);
    Status = GetVariable2 (BootOrderName, &gEfiGlobalVariableGuid, (VOID **)&BootOrder, &OrderSize);
    if (EFI_ERROR(Status) || (BootOrder == NULL)) {
      continue;
    }
    for (Index = 0; Index < OrderSize/sizeof(CHAR16); Index++) {
      UnicodeSPrint (BootOptionName, sizeof(BootOptionName), L"%s%04x", mLoadOptionVariableList[ListIndex], BootOrder[Index]);
      Status = GetVariable2 (BootOptionName, &gEfiGlobalVariableGuid, &Variable, &Size);
      if (!EFI_ERROR(Status)) {
        DumpLoadOption (BootOptionName, Variable, Size);
        Status = TestPointCheckLoadOption (BootOptionName, Variable, Size);
        if (EFI_ERROR(Status)) {
          TestPointLibAppendErrorString (
            PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
            NULL,
            TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_BOOT_VARIABLE_FUNCTIONAL_ERROR_CODE \
              TEST_POINT_READY_TO_BOOT \
              TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_BOOT_VARIABLE_FUNCTIONAL_ERROR_STRING
            );
          ReturnStatus = Status;
        }
      } else {
        DumpLoadOption (BootOptionName, NULL, 0);
      }
    }
  }

  return ReturnStatus;
}

EFI_STATUS
TestPointCheckPlatformRecoveryOptionVariable (
  VOID
  )
{
  VOID        *Variable;
  UINTN       ListIndex;
  UINTN       Size;
  UINTN       Index;
  EFI_STATUS  Status;
  CHAR16      PlatformRecoveryOptionName[sizeof(L"PlatformRecovery####")];
  EFI_STATUS  ReturnStatus;

  ReturnStatus = EFI_SUCCESS;
  for (ListIndex = 0; ListIndex < sizeof(mPlatformRecoveryOptionVariableList)/sizeof(mPlatformRecoveryOptionVariableList[0]); ListIndex++) {
    for (Index = 0; ; Index++) {
      UnicodeSPrint (PlatformRecoveryOptionName, sizeof(PlatformRecoveryOptionName), L"%s%04x", mPlatformRecoveryOptionVariableList[ListIndex], Index);
      Status = GetVariable2 (PlatformRecoveryOptionName, &gEfiGlobalVariableGuid, &Variable, &Size);
      if (!EFI_ERROR(Status)) {
        DumpLoadOption (PlatformRecoveryOptionName, Variable, Size);
        Status = TestPointCheckLoadOption (PlatformRecoveryOptionName, Variable, Size);
        if (EFI_ERROR(Status)) {
          ReturnStatus = Status;
          TestPointLibAppendErrorString (
            PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
            NULL,
            TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_BOOT_VARIABLE_FUNCTIONAL_ERROR_CODE \
              TEST_POINT_READY_TO_BOOT \
              TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_BOOT_VARIABLE_FUNCTIONAL_ERROR_STRING
            );
        }
      } else {
        break;
      }
    }
  }

  return ReturnStatus;
}

EFI_STATUS
TestPointCheckOsRecoveryOptionVariable (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
TestPointCheckKeyOptionVariable (
  VOID
  )
{
  VOID        *Variable;
  UINTN       ListIndex;
  UINTN       Size;
  UINTN       Index;
  EFI_STATUS  Status;
  CHAR16      KeyOptionName[sizeof(L"Key####")];

  for (ListIndex = 0; ListIndex < sizeof(mKeyOptionVariableList)/sizeof(mKeyOptionVariableList[0]); ListIndex++) {
    for (Index = 0; ; Index++) {
      UnicodeSPrint (KeyOptionName, sizeof(KeyOptionName), L"%s%04x", mKeyOptionVariableList[ListIndex], Index);
      Status = GetVariable2 (KeyOptionName, &gEfiGlobalVariableGuid, &Variable, &Size);
      if (!EFI_ERROR(Status) && (Variable != NULL)) {
        DumpKeyOption (KeyOptionName, Variable, Size);
      } else {
        break;
      }
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
TestPointCheckBootVariable (
  VOID
  )
{
  EFI_STATUS   Status;
  EFI_STATUS   ReturnStatus;

  DEBUG ((DEBUG_INFO, "==== TestPointCheckBootVariable - Enter\n"));

  ReturnStatus = EFI_SUCCESS;

  Status = TestPointCheckLoadOptionVariable ();
  if (EFI_ERROR(Status)) {
    ReturnStatus = Status;
  }
  Status = TestPointCheckPlatformRecoveryOptionVariable ();
  if (EFI_ERROR(Status)) {
    ReturnStatus = Status;
  }
  Status = TestPointCheckOsRecoveryOptionVariable ();
  if (EFI_ERROR(Status)) {
    ReturnStatus = Status;
  }
  Status = TestPointCheckKeyOptionVariable ();
  if (EFI_ERROR(Status)) {
    ReturnStatus = Status;
  }

  DEBUG ((DEBUG_INFO, "==== TestPointCheckBootVariable - Exit\n"));

  return ReturnStatus;
}