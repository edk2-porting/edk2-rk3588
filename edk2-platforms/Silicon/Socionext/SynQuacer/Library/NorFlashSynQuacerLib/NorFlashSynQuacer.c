/** @file

 Copyright (c) 2011-2014, ARM Ltd. All rights reserved.<BR>

 SPDX-License-Identifier: BSD-2-Clause-Patent

 **/

#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/NorFlashPlatformLib.h>

#include <Platform/MemoryMap.h>

#define FW_CODE_REGION_BASE SYNQUACER_SPI_NOR_BASE
#define FW_CODE_REGION_SIZE (FW_ENV_REGION_BASE - FW_CODE_REGION_BASE)

#define FW_ENV_REGION_BASE  FixedPcdGet32 (PcdFlashNvStorageVariableBase)
#define FW_ENV_REGION_SIZE  (FixedPcdGet32 (PcdFlashNvStorageVariableSize) + \
                             FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize) + \
                             FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize))

STATIC NOR_FLASH_DESCRIPTION mNorFlashDevices[] = {
  {
    // UEFI code region
    SYNQUACER_SPI_NOR_BASE,                             // device base
    FW_CODE_REGION_BASE,                                // region base
    FW_CODE_REGION_SIZE,                                // region size
    SIZE_64KB,                                          // block size
  },
  {
    // Environment variable region
    SYNQUACER_SPI_NOR_BASE,                             // device base
    FW_ENV_REGION_BASE,                                 // region base
    FW_ENV_REGION_SIZE,                                 // region size
    SIZE_64KB,                                          // block size
  },
};

EFI_STATUS
NorFlashPlatformInitialization (
  VOID
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
NorFlashPlatformGetDevices (
  OUT NOR_FLASH_DESCRIPTION   **NorFlashDevices,
  OUT UINT32                  *Count
  )
{
  if (NorFlashDevices == NULL ||
      Count == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Count = ARRAY_SIZE (mNorFlashDevices);
  *NorFlashDevices = mNorFlashDevices;

  return EFI_SUCCESS;
}
