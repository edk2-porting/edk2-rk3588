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
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/MemoryOverwriteControl.h>
#include <IndustryStandard/MemoryOverwriteRequestControlLock.h>

typedef struct {
  CHAR16     *Name;
  EFI_GUID   *Guid;
} VARIABLE_LIST;

VARIABLE_LIST mMorVariable[] = {
  {MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,     &gEfiMemoryOverwriteControlDataGuid},
  {MEMORY_OVERWRITE_REQUEST_CONTROL_LOCK_NAME, &gEfiMemoryOverwriteRequestControlLockGuid},
};

EFI_STATUS
EFIAPI
TestPointCheckTcgMor (
  VOID
  )
{
  VOID        *Variable;
  UINTN       Size;
  UINTN       Index;
  EFI_STATUS  Status;
  EFI_STATUS  ReturnStatus;

  DEBUG ((DEBUG_INFO, "==== TestPointCheckTcgMor - Enter\n"));

  ReturnStatus = EFI_SUCCESS;
  for (Index = 0; Index < sizeof(mMorVariable)/sizeof(mMorVariable[0]); Index++) {
    Status = GetVariable2 (mMorVariable[Index].Name, mMorVariable[Index].Guid, &Variable, &Size);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Variable - %S not found\n", mMorVariable[Index].Name));
      ReturnStatus = Status;
      TestPointLibAppendErrorString (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_BYTE5_READY_TO_BOOT_TCG_MOR_ENABLED_ERROR_CODE \
          TEST_POINT_READY_TO_BOOT \
          TEST_POINT_BYTE5_READY_TO_BOOT_TCG_MOR_ENABLED_ERROR_STRING
        );
    } else {
      FreePool (Variable);
    }
  }
  
  DEBUG ((DEBUG_INFO, "==== TestPointCheckTcgMor - Exit\n"));
  return ReturnStatus;
}
