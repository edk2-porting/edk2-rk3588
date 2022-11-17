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
#include <Library/MemoryAllocationLib.h>
#include <Guid/GlobalVariable.h>
#include <Guid/ImageAuthentication.h>

typedef struct {
  CHAR16     *Name;
  EFI_GUID   *Guid;
  UINT8      ExpectedSize;
  UINT8      ExpectedData;
} VARIABLE_LIST;

VARIABLE_LIST mUefiSecureBootVariable[] = {
  {EFI_PLATFORM_KEY_NAME,        &gEfiGlobalVariableGuid},
  {EFI_KEY_EXCHANGE_KEY_NAME,    &gEfiGlobalVariableGuid},
  {EFI_IMAGE_SECURITY_DATABASE,  &gEfiImageSecurityDatabaseGuid},
  {EFI_IMAGE_SECURITY_DATABASE1, &gEfiImageSecurityDatabaseGuid},
};

VARIABLE_LIST mUefiSecureBootModeVariable[] = {
  {EFI_SECURE_BOOT_MODE_NAME, &gEfiGlobalVariableGuid, sizeof(UINT8), 1},
  {EFI_SETUP_MODE_NAME,       &gEfiGlobalVariableGuid, sizeof(UINT8), 0},
};

EFI_STATUS
EFIAPI
TestPointCheckUefiSecureBoot (
  VOID
  )
{
  VOID        *Variable;
  UINTN       Size;
  UINTN       Index;
  EFI_STATUS  Status;
  EFI_STATUS  ReturnStatus;

  DEBUG ((DEBUG_INFO, "==== TestPointCheckUefiSecureBoot - Enter\n"));

  ReturnStatus = EFI_SUCCESS;
  for (Index = 0; Index < sizeof(mUefiSecureBootVariable)/sizeof(mUefiSecureBootVariable[0]); Index++) {
    Status = GetVariable2 (mUefiSecureBootVariable[Index].Name, mUefiSecureBootVariable[Index].Guid, &Variable, &Size);
    if(Variable == NULL) {
      return EFI_NOT_FOUND;
    }
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Variable - %S not found\n", mUefiSecureBootVariable[Index].Name));
      ReturnStatus = Status;
      TestPointLibAppendErrorString (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_BYTE5_READY_TO_BOOT_UEFI_SECURE_BOOT_ENABLED_ERROR_CODE \
          TEST_POINT_READY_TO_BOOT \
          TEST_POINT_BYTE5_READY_TO_BOOT_UEFI_SECURE_BOOT_ENABLED_ERROR_STRING
        );
    } else {
      FreePool (Variable);
    }
  }

  for (Index = 0; Index < sizeof(mUefiSecureBootModeVariable)/sizeof(mUefiSecureBootModeVariable[0]); Index++) {
    Status = GetVariable2 (mUefiSecureBootModeVariable[Index].Name, mUefiSecureBootModeVariable[Index].Guid, &Variable, &Size);
    if(Variable == NULL) {
      return EFI_NOT_FOUND;
    }
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Variable - %S not found\n", mUefiSecureBootModeVariable[Index].Name));
      ReturnStatus = Status;
      TestPointLibAppendErrorString (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_BYTE5_READY_TO_BOOT_UEFI_SECURE_BOOT_ENABLED_ERROR_CODE \
          TEST_POINT_READY_TO_BOOT \
          TEST_POINT_BYTE5_READY_TO_BOOT_UEFI_SECURE_BOOT_ENABLED_ERROR_STRING
        );
    } else {
      if ((Size != mUefiSecureBootModeVariable[Index].ExpectedSize) ||
          (*(UINT8 *)Variable != mUefiSecureBootModeVariable[Index].ExpectedData)) {
        DEBUG ((DEBUG_ERROR, "Variable - %S is not expected (0x%x)\n", mUefiSecureBootModeVariable[Index].Name, *(UINT8 *)Variable));
        ReturnStatus = EFI_SECURITY_VIOLATION;
        TestPointLibAppendErrorString (
          PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
          NULL,
          TEST_POINT_BYTE5_READY_TO_BOOT_UEFI_SECURE_BOOT_ENABLED_ERROR_CODE \
            TEST_POINT_READY_TO_BOOT \
            TEST_POINT_BYTE5_READY_TO_BOOT_UEFI_SECURE_BOOT_ENABLED_ERROR_STRING
          );
      }
      FreePool (Variable);
    }
  }

  DEBUG ((DEBUG_INFO, "==== TestPointCheckUefiSecureBoot - Exit\n"));
  return ReturnStatus;
}
