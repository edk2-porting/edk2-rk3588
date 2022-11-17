/** @file
  Platform flash device access library for Marvell Armada 7k8k Platforms

  Copyright (c) 2018 Marvell International Ltd.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/Spi.h>
#include <Protocol/SpiFlash.h>

#define MAIN_HDR_MAGIC        0xB105B002

STATIC MARVELL_SPI_FLASH_PROTOCOL *SpiFlashProtocol;
STATIC MARVELL_SPI_MASTER_PROTOCOL *SpiMasterProtocol;

typedef struct {              // Bytes
  UINT32  Magic;              //  0-3
  UINT32  PrologSize;         //  4-7
  UINT32  PrologChecksum;     //  8-11
  UINT32  BootImageSize;      // 12-15
  UINT32  BootImageChecksum;  // 16-19
  UINT32  Reserved0;          // 20-23
  UINT32  LoadAddr;           // 24-27
  UINT32  ExecAddr;           // 28-31
  UINT8   UartConfig;         //  32
  UINT8   Baudrate;           //  33
  UINT8   ExtCount;           //  34
  UINT8   AuxFlags;           //  35
  UINT32  IoArg0;             // 36-39
  UINT32  IoArg1;             // 40-43
  UINT32  IoArg2;             // 43-47
  UINT32  IoArg3;             // 48-51
  UINT32  Reserved1;          // 52-55
  UINT32  Reserved2;          // 56-59
  UINT32  Reserved3;          // 60-63
} MV_FIRMWARE_IMAGE_HEADER;

STATIC
EFI_STATUS
SpiFlashProbe (
  IN SPI_DEVICE    *SpiFlash
  )
{
  EFI_STATUS       Status;

  // Read SPI flash ID
  Status = SpiFlashProtocol->ReadId (SpiFlash, FALSE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SpiFlashProtocol->Init (SpiFlashProtocol, SpiFlash);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot initialize flash device\n", __FUNCTION__));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CheckImageHeader (
  IN OUT VOID     *ImageBuffer
  )
{
  MV_FIRMWARE_IMAGE_HEADER *Header;
  UINT32 HeaderLength, Checksum, ChecksumBackup;

  Header = (MV_FIRMWARE_IMAGE_HEADER *)ImageBuffer;
  HeaderLength = Header->PrologSize;
  ChecksumBackup = Header->PrologChecksum;

  // Compare magic number
  if (Header->Magic != MAIN_HDR_MAGIC) {
    DEBUG ((DEBUG_ERROR,
      "%a: Bad Image magic 0x%08x != 0x%08x\n",
      __FUNCTION__,
      Header->Magic,
      MAIN_HDR_MAGIC));
    return EFI_VOLUME_CORRUPTED;
  }

  // The checksum field is discarded from calculation
  Header->PrologChecksum = 0;

  Checksum = CalculateSum32 ((UINT32 *)Header, HeaderLength);
  if (Checksum != ChecksumBackup) {
    DEBUG ((DEBUG_ERROR,
      "%a: Bad Image checksum. 0x%x != 0x%x\n",
      __FUNCTION__,
      Checksum,
      ChecksumBackup));
    return EFI_VOLUME_CORRUPTED;
  }

  // Restore checksum backup
  Header->PrologChecksum = ChecksumBackup;

  return EFI_SUCCESS;
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
  EFI_STATUS              Status;
  VOID                    *ImageBuffer;
  SPI_DEVICE              *SpiFlash = NULL;
  BOOLEAN                 BufferAligned = TRUE;

  // Verify Firmware data
  if (FlashAddressType != FlashAddressTypeAbsoluteAddress) {
    DEBUG ((DEBUG_ERROR,
      "%a: only FlashAddressTypeAbsoluteAddress supported\n",
      __FUNCTION__));

    return EFI_INVALID_PARAMETER;
  }

  if (FirmwareType != PlatformFirmwareTypeSystemFirmware) {
    DEBUG ((DEBUG_ERROR,
      "%a: only PlatformFirmwareTypeSystemFirmware supported\n",
      __FUNCTION__));

    return EFI_INVALID_PARAMETER;
  }

  // Locate SPI protocols
  Status = gBS->LocateProtocol (&gMarvellSpiFlashProtocolGuid,
                  NULL,
                  (VOID **)&SpiFlashProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot locate SpiFlash protocol\n",
      __FUNCTION__));
    return Status;
  }

  Status = gBS->LocateProtocol (&gMarvellSpiMasterProtocolGuid,
                  NULL,
                  (VOID **)&SpiMasterProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Cannot locate SpiMaster protocol\n",
      __FUNCTION__));
    return Status;
  }

  //
  // Counting checksum in the header verification requires
  // the buffer address alignment.
  // It is not guaranteed by the generic capsule handling code,
  // so use an auxiliary buffer in such case.
  //
  if (((UINTN) Buffer & 0x3) != 0) {
    ImageBuffer = AllocateCopyPool (Length, Buffer);
    if (ImageBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    BufferAligned = FALSE;
  } else {
    ImageBuffer = Buffer;
  }

  // Check image checksum and magic
  Status = CheckImageHeader (ImageBuffer);
  if (EFI_ERROR (Status)) {
    goto HeaderError;
  }

  // Setup and probe SPI flash
  SpiFlash = SpiMasterProtocol->SetupDevice (SpiMasterProtocol,
                                  SpiFlash,
                                  PcdGet32 (PcdSpiFlashCs),
                                  PcdGet32 (PcdSpiFlashMode));
  if (SpiFlash == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate SPI device!\n", __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto HeaderError;
  }

  Status = SpiFlashProbe (SpiFlash);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Error while performing SPI flash probe\n",
      __FUNCTION__));
    goto FlashProbeError;
  }

  // Update firmware image in flash
  if (Progress != NULL) {
    Status = SpiFlashProtocol->UpdateWithProgress (SpiFlash,
                                 FlashAddress,
                                 Length,
                                 (UINT8 *)ImageBuffer,
                                 Progress,
                                 StartPercentage,
                                 EndPercentage);
  } else {
    Status = SpiFlashProtocol->Update (SpiFlash,
                                 FlashAddress,
                                 Length,
                                 (UINT8 *)ImageBuffer);
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Error while performing flash update\n",
      __FUNCTION__));
    goto FlashProbeError;
  }

  DEBUG ((DEBUG_ERROR,
    "%a: Update %d bytes at offset 0x%x succeeded!\n",
    __FUNCTION__,
    Length,
    FlashAddress));

  // Release resources
  SpiMasterProtocol->FreeDevice (SpiFlash);

  if (!BufferAligned) {
    FreePool (ImageBuffer);
  }

  return EFI_SUCCESS;

FlashProbeError:
  SpiMasterProtocol->FreeDevice (SpiFlash);
HeaderError:
  if (!BufferAligned) {
    FreePool (ImageBuffer);
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
