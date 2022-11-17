/** @file  NorFlashDxe.c

  Copyright (c) 2011 - 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NorFlashInfoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>

#include "NorFlash.h"

//
// Global variable declarations
//
STATIC NOR_FLASH_INSTANCE   **mNorFlashInstances;
STATIC UINT32               mNorFlashDeviceCount;

STATIC EFI_EVENT mNorFlashVirtualAddrChangeEvent;

EFI_STATUS
EFIAPI
NorFlashFvbInitialize (
  IN NOR_FLASH_INSTANCE* Instance
  )
{
  EFI_STATUS      Status;
  UINT32          FvbNumLba;
  EFI_BOOT_MODE   BootMode;
  UINTN           RuntimeMmioRegionSize;
  UINTN           BlockSize;

  DEBUG ((DEBUG_BLKIO,"NorFlashFvbInitialize\n"));

  BlockSize = Instance->BlockSize;

  // FirmwareVolumeHeader->FvLength is declared to have the Variable area
  // AND the FTW working area AND the FTW Spare contiguous.
  ASSERT(PcdGet32(PcdFlashNvStorageVariableBase) +
         PcdGet32(PcdFlashNvStorageVariableSize) ==
         PcdGet32(PcdFlashNvStorageFtwWorkingBase));
  ASSERT(PcdGet32(PcdFlashNvStorageFtwWorkingBase) +
         PcdGet32(PcdFlashNvStorageFtwWorkingSize) ==
         PcdGet32(PcdFlashNvStorageFtwSpareBase));

  // Check if the size of the area is at least one block size
  ASSERT((PcdGet32(PcdFlashNvStorageVariableSize) > 0) &&
         (PcdGet32(PcdFlashNvStorageVariableSize) / BlockSize > 0));
  ASSERT((PcdGet32(PcdFlashNvStorageFtwWorkingSize) > 0) &&
         (PcdGet32(PcdFlashNvStorageFtwWorkingSize) / BlockSize > 0));
  ASSERT((PcdGet32(PcdFlashNvStorageFtwSpareSize) > 0) &&
         (PcdGet32(PcdFlashNvStorageFtwSpareSize) / BlockSize > 0));

  // Ensure the Variable areas are aligned on block size boundaries
  ASSERT((PcdGet32(PcdFlashNvStorageVariableBase) % BlockSize) == 0);
  ASSERT((PcdGet32(PcdFlashNvStorageFtwWorkingBase) % BlockSize) == 0);
  ASSERT((PcdGet32(PcdFlashNvStorageFtwSpareBase) % BlockSize) == 0);


  Instance->Initialized = TRUE;
  mFlashNvStorageVariableBase = FixedPcdGet32 (PcdFlashNvStorageVariableBase);

  // Set the index of the first LBA for the FVB
  Instance->StartLba = (PcdGet32 (PcdFlashNvStorageVariableBase) -
                        Instance->RegionBaseAddress) / BlockSize;

  BootMode = GetBootModeHob ();
  if (BootMode == BOOT_WITH_DEFAULT_SETTINGS) {
    Status = EFI_INVALID_PARAMETER;
  } else {
    // Determine if there is a valid header at the beginning of the NorFlash
    Status = ValidateFvHeader (Instance);
  }

  // Install the Default FVB header if required
  if (EFI_ERROR(Status)) {
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
    if (EFI_ERROR(Status)) {
      return Status;
    }

    // Install all appropriate headers
    Status = InitializeFvAndVariableStoreHeaders (Instance);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  //
  // The driver implementing the variable read service can now be dispatched;
  // the varstore headers are in place.
  //
  Status = gBS->InstallProtocolInterface (&gImageHandle,
                  &gEdkiiNvVarStoreFormattedGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Failed to install gEdkiiNvVarStoreFormattedGuid\n",
      __FUNCTION__));
      return Status;
  }

  //
  // Declare the Non-Volatile storage as EFI_MEMORY_RUNTIME
  //
  RuntimeMmioRegionSize = Instance->Size;

  Status = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                  Instance->RegionBaseAddress, RuntimeMmioRegionSize,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  ASSERT_EFI_ERROR (Status);

  Status = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                  Instance->DeviceBaseAddress, SIZE_4KB,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  ASSERT_EFI_ERROR (Status);

  Status = gDS->SetMemorySpaceAttributes (Instance->RegionBaseAddress,
                  RuntimeMmioRegionSize, EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  ASSERT_EFI_ERROR (Status);

  Status = gDS->SetMemorySpaceAttributes (Instance->DeviceBaseAddress,
                  SIZE_4KB, EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context
**/
STATIC
VOID
EFIAPI
NorFlashVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINTN Index;

  EfiConvertPointer (0x0, (VOID**)&mFlashNvStorageVariableBase);

  for (Index = 0; Index < mNorFlashDeviceCount; Index++) {
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->HostRegisterBaseAddress);
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->DeviceBaseAddress);
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->RegionBaseAddress);

    // Convert Fvb
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->FvbProtocol.EraseBlocks);
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->FvbProtocol.GetAttributes);
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->FvbProtocol.GetBlockSize);
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->FvbProtocol.GetPhysicalAddress);
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->FvbProtocol.Read);
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->FvbProtocol.SetAttributes);
    EfiConvertPointer (0x0,
      (VOID**)&mNorFlashInstances[Index]->FvbProtocol.Write);

    if (mNorFlashInstances[Index]->ShadowBuffer != NULL) {
      EfiConvertPointer (0x0, (VOID**)&mNorFlashInstances[Index]->ShadowBuffer);
    }
  }

  return;
}

EFI_STATUS
EFIAPI
NorFlashInitialise (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_PHYSICAL_ADDRESS    HostRegisterBaseAddress;
  UINT32                  Index;
  NOR_FLASH_DESCRIPTION*  NorFlashDevices;
  BOOLEAN                 ContainVariableStorage;

  // Register HSSPI FIP006 register region
  HostRegisterBaseAddress = PcdGet32 (PcdFip006DxeRegBaseAddress);

  Status = gDS->AddMemorySpace (
      EfiGcdMemoryTypeMemoryMappedIo,
      HostRegisterBaseAddress, SIZE_4KB,
      EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
      );
  ASSERT_EFI_ERROR (Status);

  Status = gDS->SetMemorySpaceAttributes (
      HostRegisterBaseAddress, SIZE_4KB,
      EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  ASSERT_EFI_ERROR (Status);

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

  mNorFlashInstances = AllocateRuntimePool (sizeof(NOR_FLASH_INSTANCE*) *
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
      HostRegisterBaseAddress,
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
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &mNorFlashInstances[Index]->Handle,
                    &gEfiDevicePathProtocolGuid, &mNorFlashInstances[Index]->DevicePath,
                    &gEfiFirmwareVolumeBlockProtocolGuid, &mNorFlashInstances[Index]->FvbProtocol,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);
  }


  //
  // Register for the virtual address change event
  //
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_NOTIFY,
                  NorFlashVirtualNotifyEvent, NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mNorFlashVirtualAddrChangeEvent);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

VOID
EFIAPI
NorFlashLock (
  NOR_FLASH_LOCK_CONTEXT    *Context
  )
{
  if (!EfiAtRuntime ()) {
    // Raise TPL to TPL_HIGH to stop anyone from interrupting us.
    Context->OriginalTPL = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  } else {
    Context->InterruptsEnabled = SaveAndDisableInterrupts ();
  }
}

VOID
EFIAPI
NorFlashUnlock (
  NOR_FLASH_LOCK_CONTEXT    *Context
  )
{
  if (!EfiAtRuntime ()) {
    // Interruptions can resume.
    gBS->RestoreTPL (Context->OriginalTPL);
  } else if (Context->InterruptsEnabled) {
    SetInterruptState (TRUE);
  }
}
