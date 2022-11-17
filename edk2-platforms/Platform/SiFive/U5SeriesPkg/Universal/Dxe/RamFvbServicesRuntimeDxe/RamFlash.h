/** @file
  Ram flash device for EFI variable

  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef RAM_FLASH_H_
#define RAM_FLASH_H_

#include <Protocol/FirmwareVolumeBlock.h>

extern VOID *mFlashBase;

/**
  Read from Ram Flash

  @param[in] Lba      The starting logical block index to read from.
  @param[in] Offset   Offset into the block at which to begin reading.
  @param[in] NumBytes On input, indicates the requested read size. On
                      output, indicates the actual number of bytes read
  @param[in] Buffer   Pointer to the buffer to read into.

**/
EFI_STATUS
RamFlashRead (
  IN        EFI_LBA                              Lba,
  IN        UINTN                                Offset,
  IN        UINTN                                *NumBytes,
  IN        UINT8                                *Buffer
  );


/**
  Write to Ram Flash

  @param[in] Lba      The starting logical block index to write to.
  @param[in] Offset   Offset into the block at which to begin writing.
  @param[in] NumBytes On input, indicates the requested write size. On
                      output, indicates the actual number of bytes written
  @param[in] Buffer   Pointer to the data to write.

**/
EFI_STATUS
RamFlashWrite (
  IN        EFI_LBA                              Lba,
  IN        UINTN                                Offset,
  IN        UINTN                                *NumBytes,
  IN        UINT8                                *Buffer
  );


/**
  Erase a Ram Flash block

  @param Lba    The logical block index to erase.

**/
EFI_STATUS
RamFlashEraseBlock (
  IN   EFI_LBA      Lba
  );


/**
  Initializes Ram flash memory support

  @retval EFI_WRITE_PROTECTED   The Ram flash device is not present.
  @retval EFI_SUCCESS           The Ram flash device is supported.

**/
EFI_STATUS
RamFlashInitialize (
  VOID
  );


VOID
RamFlashConvertPointers (
  VOID
  );

#endif
