/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>

#include "SysHost.h"

extern EFI_GUID gEfiMemoryConfigDataHobGuid;
extern EFI_GUID gEfiMemoryConfigDataGuid;

#define MAX_HOB_ENTRY_SIZE  60*1024

EFI_STATUS
EFIAPI
S3NvramSaveEntry (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  );

/**
  Saves the FSP Non-Volatile Storage HOB to the UEFI Variable Services

  @param      None

  @retval     EFI_SUCCESS  The FSP Non-Volatile Storage HOB was successfully saved.
  @retval     EFI_ERROR    The FSP Non-Volatile Storage HOB was not successfully saved.
**/
EFI_STATUS
SaveFspNonVolatileStorageHob (
  VOID
  );
