/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

STATIC
VOID
EFIAPI
RuntimeResetPlatformSpecific (
  IN CONST GUID    *ResetSubtype,
  IN CONST CHAR16  *ResetString
  )
{
  UINTN  StringSize;
  UINTN  ResetDataSize = 0;
  VOID   *ResetData    = NULL;

  if (ResetSubtype == NULL) {
    ASSERT (FALSE);
    goto Exit;
  }

  if (ResetString == NULL) {
    ResetString = L"";
  }

  StringSize    = StrSize (ResetString);
  ResetDataSize = StringSize + sizeof (GUID);

  ResetData = AllocateZeroPool (ResetDataSize);
  if (ResetData == NULL) {
    ResetDataSize = 0;
    goto Exit;
  }

  CopyMem (ResetData, ResetString, StringSize);
  CopyGuid ((GUID *)((UINT8 *)ResetData + StringSize), ResetSubtype);

Exit:
  gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, ResetDataSize, ResetData);

  if (ResetData != NULL) {
    FreePool (ResetData);
  }
}

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  RuntimeResetPlatformSpecific (&gRockchipResetTypeMaskromGuid, L"MASKROM");

  return EFI_SUCCESS;
}
