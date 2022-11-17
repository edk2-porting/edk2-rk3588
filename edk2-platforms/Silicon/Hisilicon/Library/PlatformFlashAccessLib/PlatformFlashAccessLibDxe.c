/** @file
  Platform Flash Access library.

  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
  Copyright (c) 2018, Linaro Limited. All rights reserved.
  Copyright (c) 2016 - 2018, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/HisiSpiFlashProtocol.h>

STATIC EFI_PHYSICAL_ADDRESS     mInternalFdAddress;
STATIC EFI_PHYSICAL_ADDRESS     mSFCMEM0BaseAddress;

STATIC HISI_SPI_FLASH_PROTOCOL         *mSpiProtocol;

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
  UINT32               RomAddress;
  EFI_STATUS           Status;

  DEBUG ((DEBUG_INFO,
          "PerformFlashWrite - 0x%x(%x) - 0x%x\n",
          (UINTN)FlashAddress,
          (UINTN)FlashAddressType,
          Length));

  if (FlashAddressType == FlashAddressTypeAbsoluteAddress) {
    FlashAddress = FlashAddress - mInternalFdAddress;
  }

  RomAddress = (UINT32)FlashAddress + (mInternalFdAddress - mSFCMEM0BaseAddress);

  DEBUG ((DEBUG_INFO, "Erase and Write Flash Start\n"));

  Status = mSpiProtocol->EraseWrite (
                           mSpiProtocol,
                           (UINT32) RomAddress,
                           (UINT8 *)Buffer,
                           (UINT32) Length
                           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Erase and Write Status = %r \n", Status));
  }

  return Status;
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

/**
  Platform Flash Access Lib Constructor.

  @param[in]  ImageHandle       The firmware allocated handle for the EFI image.
  @param[in]  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS  Constructor returns successfully.
**/
EFI_STATUS
EFIAPI
PerformFlashAccessLibConstructor (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS  Status;

  mInternalFdAddress = (EFI_PHYSICAL_ADDRESS) PcdGet64 (PcdFdBaseAddress);

  mSFCMEM0BaseAddress = (EFI_PHYSICAL_ADDRESS) PcdGet64 (PcdSFCMEM0BaseAddress);

  DEBUG ((DEBUG_INFO,
          "PcdFlashAreaBaseAddress - 0x%x, PcdSFCMEM0BaseAddress - 0x%x \n",
          mInternalFdAddress,
          mSFCMEM0BaseAddress));

  Status = gBS->LocateProtocol (
                  &gHisiSpiFlashProtocolGuid,
                  NULL,
                  (VOID **)&mSpiProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
            "LocateProtocol gHisiSpiFlashProtocolGuid Status = %r \n",
            Status));
  }

  return Status;
}
