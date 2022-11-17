/** @file

 Copyright (c) 2019, Linaro Ltd. All rights reserved

 SPDX-License-Identifier: BSD-2-Clause-Patent

 **/

#include <Base.h>
#include <PiDxe.h>
#include <Library/NorFlashPlatformLib.h>

#define QEMU_NOR_BLOCK_SIZE    SIZE_256KB

EFI_STATUS
NorFlashPlatformInitialization (
  VOID
  )
{
  return EFI_SUCCESS;
}

NOR_FLASH_DESCRIPTION mNorFlashDevice =
{
 FixedPcdGet64(PcdFdBaseAddress),
 FixedPcdGet64(PcdFlashNvStorageVariableBase),
 FixedPcdGet32(PcdFdSize),
 QEMU_NOR_BLOCK_SIZE
};

EFI_STATUS
NorFlashPlatformGetDevices (
  OUT NOR_FLASH_DESCRIPTION   **NorFlashDescriptions,
  OUT UINT32                  *Count
  )
{
  *NorFlashDescriptions = &mNorFlashDevice;
  *Count = 1;
  return EFI_SUCCESS;
}
