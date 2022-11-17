/** @file
 *
 *  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/BaseMemoryLib.h>

#include "Mmc.h"

#define MMCI0_BLOCKLEN 512
#define MMCI0_TIMEOUT  1000

STATIC
EFI_STATUS
R1TranAndReady (
  UINT32 *Response
  )
{
  if ((*Response & MMC_R0_READY_FOR_DATA) != 0 &&
      MMC_R0_CURRENTSTATE (Response) == MMC_R0_STATE_TRAN) {
    return EFI_SUCCESS;
  }

  return EFI_NOT_READY;
}

STATIC
EFI_STATUS
ValidateWrittenBlockCount (
  IN  MMC_HOST_INSTANCE *MmcHostInstance,
  IN  UINTN Count,
  OUT UINTN *TransferredBlocks
  )
{
  UINT32 R1;
  UINT8 Data[4];
  EFI_STATUS Status;
  UINT32 BlocksWritten;
  EFI_MMC_HOST_PROTOCOL *MmcHost = MmcHostInstance->MmcHost;

  if (MmcHostInstance->CardInfo.CardType == MMC_CARD ||
      MmcHostInstance->CardInfo.CardType == MMC_CARD_HIGH ||
      MmcHostInstance->CardInfo.CardType == EMMC_CARD) {
    /*
     * Not on MMC.
     */
    *TransferredBlocks = Count;
    return EFI_SUCCESS;
  }

  Status = MmcHost->SendCommand (MmcHost, MMC_CMD55,
                      MmcHostInstance->CardInfo.RCA << 16);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(%u): error: %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  Status = MmcHost->SendCommand (MmcHost, MMC_ACMD22, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(%u): error: %r\n",
      __FUNCTION__, __LINE__, Status));
    return Status;
  }

  MmcHost->ReceiveResponse (MmcHost, MMC_RESPONSE_TYPE_R1, &R1);
  Status = R1TranAndReady (&R1);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Read Data
  Status = MmcHost->ReadBlockData (MmcHost, 0, sizeof (Data),
                      (VOID*)Data);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(%u): error: %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  /*
   * Big Endian.
   */
  BlocksWritten = ((UINT32)Data[0] << 24) |
                  ((UINT32)Data[1] << 16) |
                  ((UINT32)Data[2] << 8) |
                  ((UINT32)Data[3] << 0);
  if (BlocksWritten != Count) {
    DEBUG ((DEBUG_ERROR, "%a(%u): expected %u != gotten %u\n",
      __FUNCTION__, __LINE__, Count, BlocksWritten));
    if (BlocksWritten == 0) {
      return EFI_DEVICE_ERROR;
    }
  }

  *TransferredBlocks = BlocksWritten;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
WaitUntilTran (
  IN MMC_HOST_INSTANCE *MmcHostInstance
  )
{
  INTN Timeout;
  UINT32 Response[1];
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_MMC_HOST_PROTOCOL *MmcHost = MmcHostInstance->MmcHost;

  Timeout = MMCI0_TIMEOUT;
  while (Timeout--) {
    /*
     * We expect CMD13 to timeout while card is programming,
     * because the card holds DAT0 low (busy).
     */
    Status = MmcHost->SendCommand (MmcHost, MMC_CMD13,
                        MmcHostInstance->CardInfo.RCA << 16);
    if (EFI_ERROR (Status) && Status != EFI_TIMEOUT) {
      DEBUG ((DEBUG_ERROR, "%a(%u) CMD13 failed: %r\n", __FUNCTION__, __LINE__, Status));
      break;
    }

    if (Status == EFI_SUCCESS) {
      MmcHost->ReceiveResponse (MmcHost, MMC_RESPONSE_TYPE_R1, Response);
      Status = R1TranAndReady (Response);
      if (!EFI_ERROR (Status)) {
        break;
      }
    }

    gBS->Stall (1000);
  }

  if (0 == Timeout) {
    DEBUG ((DEBUG_ERROR, "%a(%u) card is busy\n", __FUNCTION__, __LINE__));
    return EFI_NOT_READY;
  }

  return Status;
}

EFI_STATUS
MmcNotifyState (
  IN MMC_HOST_INSTANCE *MmcHostInstance,
  IN MMC_STATE State
  )
{
  MmcHostInstance->State = State;
  return MmcHostInstance->MmcHost->NotifyState (MmcHostInstance->MmcHost, State);
}

EFI_STATUS
EFIAPI
MmcReset (
  IN EFI_BLOCK_IO_PROTOCOL    *This,
  IN BOOLEAN                  ExtendedVerification
  )
{
  MMC_HOST_INSTANCE       *MmcHostInstance;

  MmcHostInstance = MMC_HOST_INSTANCE_FROM_BLOCK_IO_THIS (This);

  if (MmcHostInstance->MmcHost == NULL) {
    // Nothing to do
    return EFI_SUCCESS;
  }

  // If a card is not present then clear all media settings
  if (!MmcHostInstance->MmcHost->IsCardPresent (MmcHostInstance->MmcHost)) {
    MmcHostInstance->BlockIo.Media->MediaPresent = FALSE;
    MmcHostInstance->BlockIo.Media->LastBlock    = 0;
    MmcHostInstance->BlockIo.Media->BlockSize    = 512;  // Should be zero but there is a bug in DiskIo
    MmcHostInstance->BlockIo.Media->ReadOnly     = FALSE;

    // Indicate that the driver requires initialization
    MmcHostInstance->State = MmcHwInitializationState;

    return EFI_SUCCESS;
  }

  // Implement me. Either send a CMD0 (could not work for some MMC host)
  // or just turn off/turn on power and restart Identification mode.
  return EFI_SUCCESS;
}

EFI_STATUS
MmcDetectCard (
  EFI_MMC_HOST_PROTOCOL *MmcHost
  )
{
  if (!MmcHost->IsCardPresent (MmcHost)) {
    return EFI_NO_MEDIA;
  } else {
    return EFI_SUCCESS;
  }
}

EFI_STATUS
MmcStopTransmission (
  EFI_MMC_HOST_PROTOCOL *MmcHost
  )
{
  EFI_STATUS              Status;
  UINT32                  Response[4];
  // Command 12 - Stop transmission (ends read or write)
  // Normally only needed for streaming transfers or after error.
  Status = MmcHost->SendCommand (MmcHost, MMC_CMD12, 0);
  if (!EFI_ERROR (Status)) {
    MmcHost->ReceiveResponse (MmcHost, MMC_RESPONSE_TYPE_R1b, Response);
  }
  return Status;
}

STATIC
EFI_STATUS
MmcTransferBlock (
  IN EFI_BLOCK_IO_PROTOCOL    *This,
  IN UINTN                    Cmd,
  IN UINTN                    Transfer,
  IN UINT32                   MediaId,
  IN EFI_LBA                  Lba,
  IN UINTN                    BufferSize,
  OUT VOID                    *Buffer,
  OUT UINTN                   *TransferredSize
  )
{
  EFI_STATUS              Status;
  MMC_HOST_INSTANCE       *MmcHostInstance;
  EFI_MMC_HOST_PROTOCOL   *MmcHost;
  UINTN                   CmdArg;

  MmcHostInstance = MMC_HOST_INSTANCE_FROM_BLOCK_IO_THIS (This);
  MmcHost = MmcHostInstance->MmcHost;

  //Set command argument based on the card access mode (Byte mode or Block mode)
  if ((MmcHostInstance->CardInfo.OCRData.AccessMode & MMC_OCR_ACCESS_MASK) ==
      MMC_OCR_ACCESS_SECTOR) {
    CmdArg = Lba;
  } else {
    CmdArg = Lba * This->Media->BlockSize;
  }

  Status = MmcHost->SendCommand (MmcHost, Cmd, CmdArg);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(MMC_CMD%d): Error %r\n", __func__, MMC_INDX (Cmd), Status));
    return Status;
  }

  if (Transfer == MMC_IOBLOCKS_READ) {
    Status = MmcHost->ReadBlockData (MmcHost, Lba, BufferSize, Buffer);
  } else {
    Status = MmcHost->WriteBlockData (MmcHost, Lba, BufferSize, Buffer);
    if (!EFI_ERROR (Status)) {
      Status = MmcNotifyState (MmcHostInstance, MmcProgrammingState);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a(): Error MmcProgrammingState\n", __func__));
        return Status;
      }
    }
  }

  if (EFI_ERROR (Status) ||
      BufferSize > This->Media->BlockSize) {
    /*
     * CMD12 needs to be set for multiblock (to transition from
     * RECV to PROG) or for errors.
     */
    EFI_STATUS Status2 = MmcStopTransmission (MmcHost);
    if (EFI_ERROR (Status2)) {
      DEBUG ((DEBUG_ERROR, "MmcIoBlocks(): CMD12 error on Status %r: %r\n",
        Status, Status2));
      return Status2;
    }

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_BLKIO, "%a(): Error %a Block Data and Status = %r\n",
        __func__, Transfer == MMC_IOBLOCKS_READ ? "Read" : "Write", Status));
      return Status;
    }

    ASSERT (Cmd == MMC_CMD25 || Cmd == MMC_CMD18);
  }

  //
  // For reads, should be already in TRAN. For writes, wait
  // until programming finishes.
  //
  Status = WaitUntilTran (MmcHostInstance);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "WaitUntilTran after write failed\n"));
    return Status;
  }

  Status = MmcNotifyState (MmcHostInstance, MmcTransferState);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MmcIoBlocks() : Error MmcTransferState\n"));
    return Status;
  }

  if (Transfer != MMC_IOBLOCKS_READ) {
    UINTN BlocksWritten = 0;

    Status = ValidateWrittenBlockCount (MmcHostInstance,
               BufferSize /
               This->Media->BlockSize,
               &BlocksWritten);
    *TransferredSize = BlocksWritten * This->Media->BlockSize;
  } else {
    *TransferredSize = BufferSize;
  }

  return Status;
}

EFI_STATUS
MmcIoBlocks (
  IN EFI_BLOCK_IO_PROTOCOL    *This,
  IN UINTN                    Transfer,
  IN UINT32                   MediaId,
  IN EFI_LBA                  Lba,
  IN UINTN                    BufferSize,
  OUT VOID                    *Buffer
  )
{
  EFI_STATUS              Status;
  UINTN                   Cmd;
  MMC_HOST_INSTANCE       *MmcHostInstance;
  EFI_MMC_HOST_PROTOCOL   *MmcHost;
  UINTN                   BytesRemainingToBeTransfered;
  UINTN                   BlockCount;
  UINTN                   ConsumeSize;

  BlockCount = 1;
  MmcHostInstance = MMC_HOST_INSTANCE_FROM_BLOCK_IO_THIS (This);
  ASSERT (MmcHostInstance != NULL);
  MmcHost = MmcHostInstance->MmcHost;
  ASSERT (MmcHost);

  if (This->Media->MediaId != MediaId) {
    return EFI_MEDIA_CHANGED;
  }

  if ((MmcHost == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Check if a Card is Present
  if (!MmcHostInstance->BlockIo.Media->MediaPresent) {
    return EFI_NO_MEDIA;
  }

  if (PcdGet32 (PcdMmcDisableMulti) == 0 &&
      MMC_HOST_HAS_ISMULTIBLOCK (MmcHost) &&
      MmcHost->IsMultiBlock (MmcHost)) {
    BlockCount = (BufferSize + This->Media->BlockSize - 1) / This->Media->BlockSize;
  }

  // All blocks must be within the device
  if ((Lba + (BufferSize / This->Media->BlockSize)) > (This->Media->LastBlock + 1)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Transfer == MMC_IOBLOCKS_WRITE) && (This->Media->ReadOnly == TRUE)) {
    return EFI_WRITE_PROTECTED;
  }

  // Reading 0 Byte is valid
  if (BufferSize == 0) {
    return EFI_SUCCESS;
  }

  // The buffer size must be an exact multiple of the block size
  if ((BufferSize % This->Media->BlockSize) != 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  // Check the alignment
  if ((This->Media->IoAlign > 2) && (((UINTN)Buffer & (This->Media->IoAlign - 1)) != 0)) {
    return EFI_INVALID_PARAMETER;
  }

  BytesRemainingToBeTransfered = BufferSize;
  while (BytesRemainingToBeTransfered > 0) {
    Status = WaitUntilTran (MmcHostInstance);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "WaitUntilTran before IO failed"));
      return Status;
    }

    if (Transfer == MMC_IOBLOCKS_READ) {
      if (BlockCount == 1) {
        // Read a single block
        Cmd = MMC_CMD17;
      } else {
        // Read multiple blocks
        Cmd = MMC_CMD18;
      }
    } else {
      if (BlockCount == 1) {
        // Write a single block
        Cmd = MMC_CMD24;
      } else {
        // Write multiple blocks
        Cmd = MMC_CMD25;
      }
    }

    ConsumeSize = BlockCount * This->Media->BlockSize;
    if (BytesRemainingToBeTransfered < ConsumeSize) {
      ConsumeSize = BytesRemainingToBeTransfered;
    }

    Status = MmcTransferBlock (This, Cmd, Transfer, MediaId, Lba, ConsumeSize, Buffer, &ConsumeSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a(): Failed to transfer block and Status:%r\n", __func__, Status));
      return Status;
    }

    BytesRemainingToBeTransfered -= ConsumeSize;
    if (BytesRemainingToBeTransfered > 0) {
      Lba += BlockCount;
      Buffer = (UINT8*)Buffer + ConsumeSize;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MmcReadBlocks (
  IN EFI_BLOCK_IO_PROTOCOL    *This,
  IN UINT32                   MediaId,
  IN EFI_LBA                  Lba,
  IN UINTN                    BufferSize,
  OUT VOID                    *Buffer
  )
{
  return MmcIoBlocks (This, MMC_IOBLOCKS_READ, MediaId, Lba, BufferSize, Buffer);
}

EFI_STATUS
EFIAPI
MmcWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL    *This,
  IN UINT32                   MediaId,
  IN EFI_LBA                  Lba,
  IN UINTN                    BufferSize,
  IN VOID                     *Buffer
  )
{
  return MmcIoBlocks (This, MMC_IOBLOCKS_WRITE, MediaId, Lba, BufferSize, Buffer);
}

EFI_STATUS
EFIAPI
MmcFlushBlocks (
  IN EFI_BLOCK_IO_PROTOCOL  *This
  )
{
  return EFI_SUCCESS;
}
