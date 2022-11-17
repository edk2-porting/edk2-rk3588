/** @file  NorFlashSmm.c

  Copyright (c) 2011 - 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiMm.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/MmServicesTableLib.h>

#include "NorFlash.h"

//
// Global variable declarations
//
STATIC NOR_FLASH_INSTANCE   **mNorFlashInstances;
STATIC UINT32               mNorFlashDeviceCount;

EFI_STATUS
EFIAPI
NorFlashFvbInitialize (
  IN NOR_FLASH_INSTANCE* Instance
  )
{
  EFI_STATUS      Status;
  UINT32          FvbNumLba;
  UINTN           BlockSize;

  DEBUG ((DEBUG_BLKIO,"NorFlashFvbInitialize\n"));

  BlockSize = Instance->BlockSize;

  // FirmwareVolumeHeader->FvLength is declared to have the Variable area
  // AND the FTW working area AND the FTW Spare contiguous.
  ASSERT (PcdGet32 (PcdFlashNvStorageVariableBase) +
          PcdGet32 (PcdFlashNvStorageVariableSize) ==
          PcdGet32 (PcdFlashNvStorageFtwWorkingBase));
  ASSERT (PcdGet32 (PcdFlashNvStorageFtwWorkingBase) +
          PcdGet32 (PcdFlashNvStorageFtwWorkingSize) ==
          PcdGet32 (PcdFlashNvStorageFtwSpareBase));

  // Check if the size of the area is at least one block size
  ASSERT ((PcdGet32 (PcdFlashNvStorageVariableSize) > 0) &&
          (PcdGet32 (PcdFlashNvStorageVariableSize) / BlockSize > 0));
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwWorkingSize) > 0) &&
          (PcdGet32 (PcdFlashNvStorageFtwWorkingSize) / BlockSize > 0));
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwSpareSize) > 0) &&
          (PcdGet32 (PcdFlashNvStorageFtwSpareSize) / BlockSize > 0));

  // Ensure the Variable areas are aligned on block size boundaries
  ASSERT ((PcdGet32 (PcdFlashNvStorageVariableBase) % BlockSize) == 0);
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwWorkingBase) % BlockSize) == 0);
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwSpareBase) % BlockSize) == 0);


  Instance->Initialized = TRUE;
  mFlashNvStorageVariableBase = FixedPcdGet32 (PcdFlashNvStorageVariableBase);

  // Set the index of the first LBA for the FVB
  Instance->StartLba = (PcdGet32 (PcdFlashNvStorageVariableBase) -
                        Instance->RegionBaseAddress) / BlockSize;

  // Determine if there is a valid header at the beginning of the NorFlash
  Status = ValidateFvHeader (Instance);
  if (EFI_ERROR (Status)) {
    // There is no valid header, so time to install one.
    DEBUG ((DEBUG_INFO, "%a: The FVB Header is not valid.\n", __FUNCTION__));
    DEBUG ((DEBUG_INFO, "%a: Installing a correct one for this volume.\n",
      __FUNCTION__));

    // Erase all the NorFlash that is reserved for variable storage
    FvbNumLba = (PcdGet32(PcdFlashNvStorageVariableSize) +
                 PcdGet32(PcdFlashNvStorageFtwWorkingSize) +
                 PcdGet32(PcdFlashNvStorageFtwSpareSize)) /
                Instance->BlockSize;

    Status = FvbEraseBlocks (&Instance->FvbProtocol, (EFI_LBA)0, FvbNumLba,
               EFI_LBA_LIST_TERMINATOR);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    // Install all appropriate headers
    Status = InitializeFvAndVariableStoreHeaders (Instance);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
NorFlashInitialise (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_MM_SYSTEM_TABLE   *MmSystemTable
  )
{
  EFI_STATUS              Status;
  UINT32                  Index;
  NOR_FLASH_DESCRIPTION*  NorFlashDevices;
  BOOLEAN                 ContainVariableStorage;

  Status = NorFlashPlatformInitialization ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "NorFlashInitialise: Fail to initialize Nor Flash devices\n"));
    return Status;
  }

  // Initialize NOR flash instances
  Status = NorFlashPlatformGetDevices (&NorFlashDevices, &mNorFlashDeviceCount);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,"NorFlashInitialise: Fail to get Nor Flash devices\n"));
    return Status;
  }

  mNorFlashInstances = AllocatePool (sizeof(NOR_FLASH_INSTANCE*) *
                                     mNorFlashDeviceCount);

  for (Index = 0; Index < mNorFlashDeviceCount; Index++) {
    // Check if this NOR Flash device contain the variable storage region
    ContainVariableStorage =
        (NorFlashDevices[Index].RegionBaseAddress <=
         PcdGet32 (PcdFlashNvStorageVariableBase)) &&
        (PcdGet32 (PcdFlashNvStorageVariableBase) +
         PcdGet32 (PcdFlashNvStorageVariableSize) <=
        NorFlashDevices[Index].RegionBaseAddress + NorFlashDevices[Index].Size);

    Status = NorFlashCreateInstance (
      PcdGet32 (PcdFip006DxeRegBaseAddress),
      NorFlashDevices[Index].DeviceBaseAddress,
      NorFlashDevices[Index].RegionBaseAddress,
      NorFlashDevices[Index].Size,
      Index,
      NorFlashDevices[Index].BlockSize,
      ContainVariableStorage,
      &mNorFlashInstances[Index]
    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,
        "NorFlashInitialise: Fail to create instance for NorFlash[%d]\n",
        Index));
      continue;
    }
    Status = gMmst->MmInstallProtocolInterface (
                      &mNorFlashInstances[Index]->Handle,
                      &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &mNorFlashInstances[Index]->FvbProtocol
                      );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

VOID
EFIAPI
NorFlashLock (
  NOR_FLASH_LOCK_CONTEXT    *Context
  )
{
}

VOID
EFIAPI
NorFlashUnlock (
  NOR_FLASH_LOCK_CONTEXT    *Context
  )
{
}
