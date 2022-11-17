/** @file
  Platform flash device access library for AMD Styx

  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>
  Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/AmdIscpDxeProtocol.h>

STATIC CONST UINT64 mFlashMaxSize = FixedPcdGet64 (PcdFdSize);

STATIC CONST UINTN mBlockSize = SIZE_64KB;

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
  EFI_STATUS                      Status;
  AMD_ISCP_DXE_PROTOCOL           *IscpDxeProtocol;

  if (FlashAddressType != FlashAddressTypeRelativeAddress) {
    DEBUG ((DEBUG_ERROR, "%a: only FlashAddressTypeRelativeAddress supported\n",
      __FUNCTION__));

    return EFI_INVALID_PARAMETER;
  }

  if (FirmwareType != PlatformFirmwareTypeSystemFirmware) {
    DEBUG ((DEBUG_ERROR,
      "%a: only PlatformFirmwareTypeSystemFirmware supported\n",
      __FUNCTION__));

    return EFI_INVALID_PARAMETER;
  }

  if ((FlashAddress % mBlockSize) != 0 || (Length % mBlockSize) != 0) {
    DEBUG ((DEBUG_ERROR,
      "%a:region [0x%lx, 0x%lx) is not a multiple of the blocksize 0x%lx\n",
      __FUNCTION__, FlashAddress, Length, mBlockSize));
    return EFI_INVALID_PARAMETER;
  }

  if ((FlashAddress + Length) > mFlashMaxSize) {
    DEBUG ((DEBUG_ERROR,
      "%a: updated region [0x%lx, 0x%lx) outside of FV region [0x0, 0x%lx)\n",
      __FUNCTION__, FlashAddress, FlashAddress + Length, mFlashMaxSize));
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gAmdIscpDxeProtocolGuid, NULL,
                  (VOID **)&IscpDxeProtocol);
  ASSERT_EFI_ERROR (Status);

  while (Length > 0) {
    //
    // Erase the block
    //
    DEBUG ((DEBUG_INFO, "%a: erasing 0x%llx bytes at address 0x%llx\n",
      __FUNCTION__, mBlockSize, FlashAddress));

    Status = IscpDxeProtocol->AmdExecuteEraseFvBlockDxe (IscpDxeProtocol,
                                FlashAddress, mBlockSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: AmdExecuteEraseFvBlockDxe () failed - %r\n",
        __FUNCTION__, Status));
    }

    //
    // Write the new data
    //
    DEBUG ((DEBUG_INFO, "%a: writing 0x%llx bytes at at address 0x%llx\\n",
      __FUNCTION__, mBlockSize, FlashAddress));

    Status = IscpDxeProtocol->AmdExecuteUpdateFvBlockDxe (IscpDxeProtocol,
                                FlashAddress, Buffer, mBlockSize);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,
        "%a: write of block address 0x%lx failed - %r\n",
        __FUNCTION__, FlashAddress, Status));
    }

    FlashAddress += mBlockSize;
    Buffer += mBlockSize;
    Length -= mBlockSize;
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
