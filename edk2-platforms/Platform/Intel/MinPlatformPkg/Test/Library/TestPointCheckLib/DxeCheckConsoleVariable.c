/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>

BOOLEAN
IsDevicePathExist (
  IN EFI_DEVICE_PATH_PROTOCOL          *DevicePathTarget
  );

CHAR16  *mConsoleVariableList[] = {
  L"ConIn",
  L"ConInDev",
  L"ConOut",
  L"ConOutDev",
  L"ErrOut",
  L"ErrOutDev",
};

VOID
DumpDevicePath (
  IN CHAR16           *Name,
  IN EFI_DEVICE_PATH  *DevicePath,
  IN UINTN            Size
  )
{
  CHAR16 *Str;

  Str = ConvertDevicePathToText(DevicePath, TRUE, TRUE);
  DEBUG ((DEBUG_INFO, "%s: %s\n", Name, Str));
  if (Str != NULL) {
    FreePool (Str);
  }
}

EFI_STATUS
TestPointCheckConsoleVariable (
  VOID
  )
{
  VOID        *Variable;
  UINTN       Size;
  UINTN       Index;
  EFI_STATUS  Status;
  BOOLEAN     Result;

  Result = TRUE;
  DEBUG ((DEBUG_INFO, "==== TestPointCheckConsoleVariable - Enter\n"));
  for (Index = 0; Index < sizeof(mConsoleVariableList)/sizeof(mConsoleVariableList[0]); Index++) {
    Status = GetVariable2 (mConsoleVariableList[Index], &gEfiGlobalVariableGuid, &Variable, &Size);
    if (!EFI_ERROR(Status)) {
      DumpDevicePath (mConsoleVariableList[Index], Variable, Size);
      if (!IsDevicePathExist (Variable)) {
        DEBUG ((DEBUG_ERROR, "DevicePath not found!\n"));
        Result = FALSE;
        TestPointLibAppendErrorString (
          PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
          NULL,
          TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_CONSOLE_VARIABLE_FUNCTIONAL_ERROR_CODE \
            TEST_POINT_READY_TO_BOOT \
            TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_CONSOLE_VARIABLE_FUNCTIONAL_ERROR_STRING
          );
      }
    }
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckConsoleVariable - Exit\n"));

  if (Result) {
    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
}
