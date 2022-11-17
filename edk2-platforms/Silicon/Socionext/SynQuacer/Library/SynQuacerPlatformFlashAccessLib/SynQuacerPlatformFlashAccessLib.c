/** @file
  Platform flash device access library for Socionext SynQuacer

  Copyright (c) 2016, Linaro, Ltd. All rights reserved.<BR>
  Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2020, Arm Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Library/BaseMemoryLib.h>
#include <Library/BootLogoLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/FirmwareVolumeBlock.h>

/**
  Gets firmware volume block handle by given address.

  This function gets firmware volume block handle whose
  address range contains the parameter Address.

  @param[in]  Address    Address which should be contained
                         by returned FVB handle.
  @param[out] FvbHandle  Pointer to FVB handle for output.

  @retval EFI_SUCCESS    FVB handle successfully returned.
  @retval EFI_NOT_FOUND  Failed to find FVB handle by address.

**/
STATIC
EFI_STATUS
GetFvbByAddress (
  IN  EFI_PHYSICAL_ADDRESS                Address,
  IN  UINTN                               Length,
  OUT EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  **OutFvb,
  OUT EFI_LBA                             *Lba,
  OUT UINTN                               *BlockSize
  )
{
  EFI_STATUS                          Status;
  EFI_HANDLE                          *HandleBuffer;
  UINTN                               HandleCount;
  UINTN                               Index;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb;
  EFI_FVB_ATTRIBUTES_2                Attributes;
  EFI_PHYSICAL_ADDRESS                FvbBaseAddress;
  UINTN                               NumberOfBlocks;

  //
  // Locate all handles with Firmware Volume Block protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolumeBlockProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }
  //
  // Traverse all the handles, searching for the one containing parameter Address
  //
  for (Index = 0; Index < HandleCount; Index += 1) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolumeBlockProtocolGuid,
                    (VOID **) &Fvb
                    );
    if (EFI_ERROR (Status)) {
      Status = EFI_NOT_FOUND;
      break;
    }
    //
    // Checks if the address range of this handle contains parameter Address
    //
    Status = Fvb->GetPhysicalAddress (Fvb, &FvbBaseAddress);
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Check if this FVB is writable: DXE core produces FVB protocols for
    // firmware volumes as well.
    //
    Status = Fvb->GetAttributes (Fvb, &Attributes);
    if (EFI_ERROR (Status) || !(Attributes & EFI_FVB2_WRITE_STATUS)) {
      DEBUG ((DEBUG_INFO,
        "%a: ignoring read-only FVB protocol implementation\n",
        __FUNCTION__));
      Status = EFI_NOT_FOUND;
      continue;
    }

    Status = Fvb->GetBlockSize (Fvb, 0, BlockSize, &NumberOfBlocks);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: failed to get FVB blocksize - %r, ignoring\n",
        __FUNCTION__, Status));
      continue;
    }

    if ((Length % *BlockSize) != 0) {
      DEBUG ((DEBUG_INFO,
        "%a: Length 0x%lx is not a multiple of the blocksize 0x%lx, ignoring\n",
        __FUNCTION__, Length, *BlockSize));
      Status = EFI_INVALID_PARAMETER;
      continue;
    }

    if ((Address >= FvbBaseAddress) &&
        ((Address + Length) <=
         (FvbBaseAddress + (*BlockSize * NumberOfBlocks)))) {
      *OutFvb  = Fvb;
      *Lba = (Address - FvbBaseAddress) / *BlockSize;
      Status = EFI_SUCCESS;
      break;
    }

    Status = EFI_NOT_FOUND;
  }

  FreePool (HandleBuffer);
  return Status;
}

/**
  Check whether a buffer has any data in it, i.e., bytes with value != 0xff

  @param[in] Buffer       Address of the buffer
  @param[in] Length       Size of the buffer

  @retval TRUE            A non-0xff byte was found
  @retval FALSE           Buffer has 0xff bytes only

**/
STATIC
BOOLEAN
BufferHasData (
  IN  VOID      *Buffer,
  IN  UINTN     Length
  )
{
  UINT8   *Data;
  UINTN   Index;

  Data = Buffer;
  for (Index = 0; Index < Length; Index++) {
    if (Data[Index] != 0xff) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Perform flash write operation with progress indicator.  The start and end
  completion percentage values are passed into this function.  If the requested
  flash write operation is broken up, then completion percentage between the
  start and end values may be passed to the provided Progress function.  The
  caller of this function is required to call the Progress function for the
  start and end completion percentage values.  This allows the Progress,
  StartPercentage, and EndPercentage parameters to be ignored if the requested
  flash write operation can not be broken up

  @param[in] FirmwareType      The type of firmware.
  @param[in] FlashAddress      The address of flash device to be accessed.
  @param[in] FlashAddressType  The type of flash device address.
  @param[in] Buffer            The pointer to the data buffer.
  @param[in] Length            The length of data buffer in bytes.
  @param[in] Progress          A function used report the progress of the
                               firmware update.  This is an optional parameter
                               that may be NULL.
  @param[in] StartPercentage   The start completion percentage value that may
                               be used to report progress during the flash
                               write operation.
  @param[in] EndPercentage     The end completion percentage value that may
                               be used to report progress during the flash
                               write operation.

  @retval EFI_SUCCESS           The operation returns successfully.
  @retval EFI_WRITE_PROTECTED   The flash device is read only.
  @retval EFI_UNSUPPORTED       The flash device access is unsupported.
  @retval EFI_INVALID_PARAMETER The input parameter is not valid.
**/
EFI_STATUS
EFIAPI
PerformFlashWriteWithProgress (
  IN PLATFORM_FIRMWARE_TYPE                         FirmwareType,
  IN EFI_PHYSICAL_ADDRESS                           FlashAddress,
  IN FLASH_ADDRESS_TYPE                             FlashAddressType,
  IN VOID                                           *Buffer,
  IN UINTN                                          Length,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb;
  EFI_STATUS                          Status;
  UINTN                               BlockSize;
  EFI_LBA                             Lba;
  UINTN                               NumBytes;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL_UNION Black;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL_UNION White;
  UINTN                               Resolution;
  UINTN                               CurrentProgress;
  BOOLEAN                             HaveBootGraphics;

  Black.Raw = 0x00000000;
  White.Raw = 0x00FFFFFF;

  Status = BootLogoEnableLogo ();
  HaveBootGraphics = !EFI_ERROR (Status);

  if (FlashAddressType != FlashAddressTypeAbsoluteAddress) {
    DEBUG ((DEBUG_ERROR, "%a: only FlashAddressTypeAbsoluteAddress supported\n",
      __FUNCTION__));

    return EFI_INVALID_PARAMETER;
  }

  if (FirmwareType != PlatformFirmwareTypeSystemFirmware) {
    DEBUG ((DEBUG_ERROR,
      "%a: only PlatformFirmwareTypeSystemFirmware supported\n",
      __FUNCTION__));

    return EFI_INVALID_PARAMETER;
  }

  //
  // Locate the gEfiFirmwareVolumeBlockProtocolGuid implementation
  // that covers the system firmware
  //
  Fvb = NULL;
  Status = GetFvbByAddress (FlashAddress, Length, &Fvb, &Lba, &BlockSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: failed to locate FVB handle for address 0x%llx - %r\n",
      __FUNCTION__, FlashAddress, Status));
    return Status;
  }

  //
  // Remap the region as device rather than uncached.
  //
  Status = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                  FlashAddress, Length, EFI_MEMORY_UC);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: gDS->AddMemorySpace () failed - %r\n",
      __FUNCTION__, Status));
  }

  Status = gDS->SetMemorySpaceAttributes (FlashAddress, Length, EFI_MEMORY_UC);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: gDS->SetMemorySpaceAttributes () failed - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  if (HaveBootGraphics) {
    Resolution = (BlockSize * 100) / Length + 1;
    CurrentProgress = 0;

    Status = BootLogoUpdateProgress (White.Pixel, Black.Pixel,
               L"Updating firmware - please wait", Black.Pixel, 100, 0);
  } else {
    Print (L"Updating firmware - please wait ");
  }

  //
  // Erase the region
  //
  DEBUG ((DEBUG_INFO, "%a: erasing 0x%llx bytes at address %llx (LBA 0x%lx)\n",
    __FUNCTION__, Length, FlashAddress, Lba));

  Status = Fvb->EraseBlocks (Fvb, Lba, Length / BlockSize,
                  EFI_LBA_LIST_TERMINATOR);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Fvb->EraseBlocks () failed - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  while (Length > 0) {
    //
    // Write the new data
    //
    DEBUG ((DEBUG_INFO, "%a: writing 0x%llx bytes at LBA 0x%lx\n",
      __FUNCTION__, BlockSize, Lba));

    NumBytes = BlockSize;
    if (BufferHasData (Buffer, NumBytes)) {
      Status = Fvb->Write (Fvb, Lba, 0, &NumBytes, Buffer);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR,
          "%a: write of LBA 0x%lx failed - %r (NumBytes == 0x%lx)\n",
          __FUNCTION__, Lba, Status, NumBytes));
      }
    }

    if (HaveBootGraphics) {
      Status = BootLogoUpdateProgress (White.Pixel, Black.Pixel,
                L"Updating firmware - please wait", White.Pixel,
                CurrentProgress + Resolution, CurrentProgress);
      CurrentProgress += Resolution;
    } else {
      Print (L".");
    }

    Buffer = (VOID*)((UINTN)Buffer + BlockSize);
    Length -= BlockSize;
    Lba++;
  }
  if (!HaveBootGraphics) {
    Print (L"\n");
  }

  return EFI_SUCCESS;
}

/**
  Perform flash write operation.

  @param[in] FirmwareType      The type of firmware.
  @param[in] FlashAddress      The address of flash device to be accessed.
  @param[in] FlashAddressType  The type of flash device address.
  @param[in] Buffer            The pointer to the data buffer.
  @param[in] Length            The length of data buffer in bytes.

  @retval EFI_SUCCESS           The operation returns successfully.
  @retval EFI_WRITE_PROTECTED   The flash device is read only.
  @retval EFI_UNSUPPORTED       The flash device access is unsupported.
  @retval EFI_INVALID_PARAMETER The input parameter is not valid.
**/
EFI_STATUS
EFIAPI
PerformFlashWrite (
  IN PLATFORM_FIRMWARE_TYPE       FirmwareType,
  IN EFI_PHYSICAL_ADDRESS         FlashAddress,
  IN FLASH_ADDRESS_TYPE           FlashAddressType,
  IN VOID                         *Buffer,
  IN UINTN                        Length
  )
{
  return PerformFlashWriteWithProgress (
           FirmwareType,
           FlashAddress,
           FlashAddressType,
           Buffer,
           Length,
           NULL,
           0,
           0
           );
}
