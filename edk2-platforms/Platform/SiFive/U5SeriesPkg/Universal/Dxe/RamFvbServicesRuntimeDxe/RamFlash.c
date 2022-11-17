/** @file
  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#include "RamFlash.h"

VOID *mFlashBase;

STATIC UINTN       mFdBlockSize = 0;
STATIC UINTN       mFdBlockCount = 0;

STATIC
UINT8*
RamFlashPtr (
  IN        EFI_LBA                             Lba,
  IN        UINTN                               Offset
  )
{
  return mFlashBase + ((UINTN)Lba * mFdBlockSize) + Offset;
}

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
  )
{
  UINT8  *Ptr;

  //
  // Only write to the first 64k. We don't bother saving the FTW Spare
  // block into the flash memory.
  //
  if (Lba >= mFdBlockCount) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get flash address
  //
  Ptr = (UINT8*) RamFlashPtr (Lba, Offset);

  CopyMem (Buffer, Ptr, *NumBytes);

  return EFI_SUCCESS;
}


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
  IN        EFI_LBA                             Lba,
  IN        UINTN                               Offset,
  IN        UINTN                               *NumBytes,
  IN        UINT8                               *Buffer
  )
{
  UINT8  *Ptr;
  UINTN  i;

  //
  // Only write to the first 64k. We don't bother saving the FTW Spare
  // block into the flash memory.
  //
  if (Lba >= mFdBlockCount) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Program flash
  //
  Ptr = RamFlashPtr (Lba, Offset);
  for (i = 0; i < *NumBytes; i++) {
     MmioWrite8((UINTN)Ptr, Buffer[i]);
     Ptr ++;
  }

  return EFI_SUCCESS;
}


/**
  Erase a Ram Flash block

  @param Lba    The logical block index to erase.

**/
EFI_STATUS
RamFlashEraseBlock (
  IN   EFI_LBA      Lba
  )
{

  return EFI_SUCCESS;
}


/**
  Initializes Ram flash memory support

  @retval EFI_WRITE_PROTECTED   The Ram flash device is not present.
  @retval EFI_SUCCESS           The Ram flash device is supported.

**/
EFI_STATUS
RamFlashInitialize (
  VOID
  )
{
  mFlashBase = (UINT8*)(UINTN) PcdGet32 (PcdVariableFdBaseAddress);
  mFdBlockSize = PcdGet32 (PcdVariableFdBlockSize);
  ASSERT(PcdGet32 (PcdVariableFdSize) % mFdBlockSize == 0);
  mFdBlockCount = PcdGet32 (PcdVariableFdSize) / mFdBlockSize;

  return EFI_SUCCESS;
}
