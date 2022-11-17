/** @file
  MM driver source for several Serial Flash devices
  which are compliant with the Intel(R) Serial Flash Interface Compatibility Specification.

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  Copyright (c) Microsoft Corporation.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpiFvbServiceCommon.h"
#include <Library/MmServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/SmmFirmwareVolumeBlock.h>

/**
  The function installs EFI_FIRMWARE_VOLUME_BLOCK protocol
  for each FV in the system.

  @param[in]  FvbInstance   The pointer to a FW volume instance structure,
                            which contains the information about one FV.

  @retval     VOID

**/
VOID
InstallFvbProtocol (
  IN  EFI_FVB_INSTANCE               *FvbInstance
  )
{
  EFI_FIRMWARE_VOLUME_HEADER            *FvHeader;
  EFI_STATUS                            Status;
  EFI_HANDLE                            FvbHandle;

  ASSERT (FvbInstance != NULL);
  if (FvbInstance == NULL) {
    return;
  }

  CopyMem (&FvbInstance->FvbProtocol, &mFvbProtocolTemplate, sizeof (EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL));

  FvHeader = &FvbInstance->FvHeader;
  if (FvHeader == NULL) {
    return;
  }

  //
  // Set up the devicepath
  //
  DEBUG ((DEBUG_INFO, "FwBlockService.c: Setting up DevicePath for 0x%lx:\n", FvbInstance->FvBase));
  if (FvHeader->ExtHeaderOffset == 0) {
    //
    // FV does not contains extension header, then produce MEMMAP_DEVICE_PATH
    //
    FvbInstance->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AllocateRuntimeCopyPool (sizeof (FV_MEMMAP_DEVICE_PATH), &mFvMemmapDevicePathTemplate);
    if (FvbInstance->DevicePath == NULL) {
      DEBUG ((DEBUG_INFO, "SpiFvbServiceSmm.c: Memory allocation for MEMMAP_DEVICE_PATH failed\n"));
      return;
    }
    ((FV_MEMMAP_DEVICE_PATH *) FvbInstance->DevicePath)->MemMapDevPath.StartingAddress = FvbInstance->FvBase;
    ((FV_MEMMAP_DEVICE_PATH *) FvbInstance->DevicePath)->MemMapDevPath.EndingAddress   = FvbInstance->FvBase + FvHeader->FvLength - 1;
  } else {
    FvbInstance->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AllocateRuntimeCopyPool (sizeof (FV_PIWG_DEVICE_PATH), &mFvPIWGDevicePathTemplate);
    if (FvbInstance->DevicePath == NULL) {
      DEBUG ((DEBUG_INFO, "SpiFvbServiceSmm.c: Memory allocation for FV_PIWG_DEVICE_PATH failed\n"));
      return;
    }
    CopyGuid (
      &((FV_PIWG_DEVICE_PATH *)FvbInstance->DevicePath)->FvDevPath.FvName,
      (GUID *)(UINTN)(FvbInstance->FvBase + FvHeader->ExtHeaderOffset)
      );
  }

  //
  // LocateDevicePath fails so install a new interface and device path
  //
  FvbHandle = NULL;

  Status = gMmst->MmInstallProtocolInterface (
                    &FvbHandle,
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &(FvbInstance->FvbProtocol)
                    );
  ASSERT_EFI_ERROR (Status);

  Status = gMmst->MmInstallProtocolInterface (
                    &FvbHandle,
                    &gEfiDevicePathProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &(FvbInstance->DevicePath)
                    );
  ASSERT_EFI_ERROR (Status);
}

/**
  The function does the necessary initialization work for
  Firmware Volume Block Driver.

**/
VOID
FvbInitialize (
  VOID
  )
{
  EFI_FVB_INSTANCE                      *FvbInstance;
  EFI_FIRMWARE_VOLUME_HEADER            *FvHeader;
  EFI_FV_BLOCK_MAP_ENTRY                *PtrBlockMapEntry;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  EFI_STATUS                            Status;
  UINTN                                 BufferSize;
  UINTN                                 Idx;
  UINT32                                MaxLbaSize;
  UINT32                                BytesWritten;
  UINTN                                 BytesErased;

  mPlatformFvBaseAddress[0].FvBase = PcdGet32(PcdFlashNvStorageVariableBase);
  mPlatformFvBaseAddress[0].FvSize = PcdGet32(PcdFlashNvStorageVariableSize);
  mPlatformFvBaseAddress[1].FvBase = PcdGet32(PcdFlashFvMicrocodeBase);
  mPlatformFvBaseAddress[1].FvSize = PcdGet32(PcdFlashFvMicrocodeSize);
  mPlatformDefaultBaseAddress[0].FvBase = PcdGet32(PcdFlashNvStorageVariableBase);
  mPlatformDefaultBaseAddress[0].FvSize = PcdGet32(PcdFlashNvStorageVariableSize);
  mPlatformDefaultBaseAddress[1].FvBase = PcdGet32(PcdFlashFvMicrocodeBase);
  mPlatformDefaultBaseAddress[1].FvSize = PcdGet32(PcdFlashFvMicrocodeSize);

  //
  // We will only continue with FVB installation if the
  // SPI is the active BIOS state
  //
  {
    //
    // Make sure all FVB are valid and/or fix if possible
    //
    for (Idx = 0;; Idx++) {
      if (mPlatformFvBaseAddress[Idx].FvSize == 0 && mPlatformFvBaseAddress[Idx].FvBase == 0) {
        break;
      }

      BaseAddress = mPlatformFvBaseAddress[Idx].FvBase;
      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BaseAddress;

      if (!IsFvHeaderValid (BaseAddress, FvHeader)) {
        BytesWritten = 0;
        BytesErased = 0;
        DEBUG ((DEBUG_ERROR, "ERROR - The FV in 0x%x is invalid!\n", FvHeader));
        Status = GetFvbInfo (BaseAddress, &FvHeader);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_WARN, "ERROR - Can't recovery FV header at 0x%x.  GetFvbInfo Status %r\n", BaseAddress, Status));
          continue;
        }
        DEBUG ((DEBUG_INFO, "Rewriting FV header at 0x%X with static data\n", BaseAddress));
        //
        // Spi erase
        //
        BytesErased = (UINTN) FvHeader->BlockMap->Length;
        Status = SpiFlashBlockErase( (UINTN) BaseAddress, &BytesErased);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_WARN, "ERROR - SpiFlashBlockErase Error  %r\n", Status));
          continue;
        }
        if (BytesErased != FvHeader->BlockMap->Length) {
          DEBUG ((DEBUG_WARN, "ERROR - BytesErased != FvHeader->BlockMap->Length\n"));
          DEBUG ((DEBUG_INFO, " BytesErased = 0x%X\n Length = 0x%X\n", BytesErased, FvHeader->BlockMap->Length));
          continue;
        }
        BytesWritten = FvHeader->HeaderLength;
        Status = SpiFlashWrite ((UINTN)BaseAddress, &BytesWritten, (UINT8*)FvHeader);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_WARN, "ERROR - SpiFlashWrite Error  %r\n", Status));
          continue;
        }
        if (BytesWritten != FvHeader->HeaderLength) {
          DEBUG ((DEBUG_WARN, "ERROR - BytesWritten != HeaderLength\n"));
          DEBUG ((DEBUG_INFO, " BytesWritten = 0x%X\n HeaderLength = 0x%X\n", BytesWritten, FvHeader->HeaderLength));
          continue;
        }
        Status = SpiFlashLock ();
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_WARN, "ERROR - SpiFlashLock Error  %r\n", Status));
          continue;
        }
        DEBUG ((DEBUG_INFO, "FV Header @ 0x%X restored with static data\n", BaseAddress));
        //
        // Clear cache for this range.
        //
        WriteBackInvalidateDataCacheRange ( (VOID *) (UINTN) BaseAddress, FvHeader->BlockMap->Length);
      }
    }

    //
    // Calculate the total size for all firmware volume block instances
    //
    BufferSize = 0;
    for (Idx = 0; ; Idx++) {
      if (mPlatformFvBaseAddress[Idx].FvSize == 0 && mPlatformFvBaseAddress[Idx].FvBase == 0) {
        break;
      }
      BaseAddress = mPlatformFvBaseAddress[Idx].FvBase;
      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BaseAddress;

      if (!IsFvHeaderValid (BaseAddress, FvHeader)) {
        DEBUG ((DEBUG_WARN, "ERROR - The FV in 0x%x is invalid!\n", FvHeader));
        continue;
      }

      BufferSize += (FvHeader->HeaderLength +
                    sizeof (EFI_FVB_INSTANCE) -
                    sizeof (EFI_FIRMWARE_VOLUME_HEADER)
                    );
    }

    mFvbModuleGlobal.FvbInstance =  (EFI_FVB_INSTANCE *) AllocateRuntimeZeroPool (BufferSize);
    if (mFvbModuleGlobal.FvbInstance == NULL) {
      ASSERT (FALSE);
      return;
    }

    MaxLbaSize      = 0;
    FvbInstance     = mFvbModuleGlobal.FvbInstance;
    mFvbModuleGlobal.NumFv   = 0;

    for (Idx = 0; ; Idx++) {
      if (mPlatformFvBaseAddress[Idx].FvSize == 0 && mPlatformFvBaseAddress[Idx].FvBase == 0) {
        break;
      }
      BaseAddress = mPlatformFvBaseAddress[Idx].FvBase;
      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BaseAddress;

      if (!IsFvHeaderValid (BaseAddress, FvHeader)) {
        DEBUG ((DEBUG_WARN, "ERROR - The FV in 0x%x is invalid!\n", FvHeader));
        continue;
      }

      FvbInstance->Signature = FVB_INSTANCE_SIGNATURE;
      CopyMem (&(FvbInstance->FvHeader), FvHeader, FvHeader->HeaderLength);

      FvHeader = &(FvbInstance->FvHeader);
      FvbInstance->FvBase = (UINTN)BaseAddress;

      //
      // Process the block map for each FV
      //
      FvbInstance->NumOfBlocks   = 0;
      for (PtrBlockMapEntry = FvHeader->BlockMap;
           PtrBlockMapEntry->NumBlocks != 0;
           PtrBlockMapEntry++) {
        //
        // Get the maximum size of a block.
        //
        if (MaxLbaSize < PtrBlockMapEntry->Length) {
          MaxLbaSize  = PtrBlockMapEntry->Length;
        }
        FvbInstance->NumOfBlocks += PtrBlockMapEntry->NumBlocks;
      }

      //
      // Add a FVB Protocol Instance
      //
      InstallFvbProtocol (FvbInstance);
      mFvbModuleGlobal.NumFv++;

      //
      // Move on to the next FvbInstance
      //
      FvbInstance = (EFI_FVB_INSTANCE *) ((UINTN)((UINT8 *)FvbInstance) +
                                            FvHeader->HeaderLength +
                                            (sizeof (EFI_FVB_INSTANCE) - sizeof (EFI_FIRMWARE_VOLUME_HEADER)));

    }
  }
}
