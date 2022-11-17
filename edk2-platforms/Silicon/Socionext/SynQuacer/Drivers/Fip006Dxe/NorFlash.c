/** @file  NorFlashDxe.c

  Copyright (c) 2011 - 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NorFlashInfoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "NorFlash.h"

STATIC CONST UINT16 mFip006NullCmdSeq[] = {
  CSDC (CSDC_END, CSDC_CONT_NON_CONTINUOUS, CSDC_TRP_MBM, CSDC_DEC_DECODE),
  CSDC (CSDC_END, CSDC_CONT_NON_CONTINUOUS, CSDC_TRP_MBM, CSDC_DEC_DECODE),
  CSDC (CSDC_END, CSDC_CONT_NON_CONTINUOUS, CSDC_TRP_MBM, CSDC_DEC_DECODE),
  CSDC (CSDC_END, CSDC_CONT_NON_CONTINUOUS, CSDC_TRP_MBM, CSDC_DEC_DECODE),
  CSDC (CSDC_END, CSDC_CONT_NON_CONTINUOUS, CSDC_TRP_MBM, CSDC_DEC_DECODE),
  CSDC (CSDC_END, CSDC_CONT_NON_CONTINUOUS, CSDC_TRP_MBM, CSDC_DEC_DECODE),
  CSDC (CSDC_END, CSDC_CONT_NON_CONTINUOUS, CSDC_TRP_MBM, CSDC_DEC_DECODE),
  CSDC (CSDC_END, CSDC_CONT_NON_CONTINUOUS, CSDC_TRP_MBM, CSDC_DEC_DECODE),
};

STATIC CONST CSDC_DEFINITION mN25qCSDCDefTable[] = {
  // Identification Operations
  { SPINOR_OP_RDID,     FALSE, FALSE, FALSE, FALSE, CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  // Register Operations
  { SPINOR_OP_RDSR,     FALSE, FALSE, FALSE, FALSE, CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  { SPINOR_OP_WRSR,     FALSE, FALSE, FALSE, TRUE,  CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  { SPINOR_OP_RD_ARRAY, TRUE,  FALSE, FALSE, FALSE, CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  { SPINOR_OP_RDFSR,    FALSE, FALSE, FALSE, FALSE, CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  { SPINOR_OP_RD_NVCFG, FALSE, FALSE, FALSE, FALSE, CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  { SPINOR_OP_RD_VCR,   FALSE, FALSE, FALSE, FALSE, CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  { SPINOR_OP_RD_EVCR,  FALSE, FALSE, FALSE, FALSE, CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  // Read Operations
  { SPINOR_OP_READ_4B,  TRUE,  TRUE,  FALSE, FALSE, CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  // Write Operations
  { SPINOR_OP_PP,       TRUE,  FALSE, FALSE, TRUE,  CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
  { SPINOR_OP_PP_1_1_4, TRUE,  FALSE, FALSE, TRUE,  CS_CFG_MBM_QUAD,
                        CSDC_TRP_SINGLE },
  // Erase Operations
  { SPINOR_OP_SE,       FALSE, FALSE, FALSE, TRUE,  CS_CFG_MBM_SINGLE,
                        CSDC_TRP_SINGLE },
};

STATIC CONST NOR_FLASH_INSTANCE mNorFlashInstanceTemplate = {
  NOR_FLASH_SIGNATURE, // Signature
  NULL, // Handle ... NEED TO BE FILLED

  FALSE, // Initialized
  NULL, // Initialize

  0, // HostRegisterBaseAddress ... NEED TO BE FILLED
  0, // DeviceBaseAddress ... NEED TO BE FILLED
  0, // RegionBaseAddress ... NEED TO BE FILLED
  0, // Size ... NEED TO BE FILLED
  0, // BlockSize
  0, // LastBlock
  0, // StartLba
  0, // OffsetLba

  {
    FvbGetAttributes, // GetAttributes
    FvbSetAttributes, // SetAttributes
    FvbGetPhysicalAddress,  // GetPhysicalAddress
    FvbGetBlockSize,  // GetBlockSize
    FvbRead,  // Read
    FvbWrite, // Write
    FvbEraseBlocks, // EraseBlocks
    NULL, //ParentHandle
  }, //  FvbProtoccol;

  NULL, // ShadowBuffer
  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        {
          (UINT8)(OFFSET_OF (NOR_FLASH_DEVICE_PATH, End)),
          (UINT8)(OFFSET_OF (NOR_FLASH_DEVICE_PATH, End) >> 8)
        }
      },
      { 0x0, 0x0, 0x0, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } },
    },
    0,  // Index
    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      { sizeof (EFI_DEVICE_PATH_PROTOCOL), 0 }
    }
  }, // DevicePath
  0 // Flags
};

EFI_STATUS
NorFlashCreateInstance (
  IN UINTN                  HostRegisterBase,
  IN UINTN                  NorFlashDeviceBase,
  IN UINTN                  NorFlashRegionBase,
  IN UINTN                  NorFlashSize,
  IN UINT32                 Index,
  IN UINT32                 BlockSize,
  IN BOOLEAN                HasVarStore,
  OUT NOR_FLASH_INSTANCE**  NorFlashInstance
  )
{
  EFI_STATUS Status;
  NOR_FLASH_INSTANCE* Instance;
  NOR_FLASH_INFO *FlashInfo;
  UINT8 JedecId[3];

  ASSERT(NorFlashInstance != NULL);

  Instance = AllocateRuntimeCopyPool (sizeof mNorFlashInstanceTemplate,
                                      &mNorFlashInstanceTemplate);
  if (Instance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Instance->HostRegisterBaseAddress = HostRegisterBase;
  Instance->DeviceBaseAddress       = NorFlashDeviceBase;
  Instance->RegionBaseAddress       = NorFlashRegionBase;
  Instance->Size                    = NorFlashSize;
  Instance->BlockSize               = BlockSize;
  Instance->LastBlock               = (NorFlashSize / BlockSize) - 1;

  Instance->OffsetLba = (NorFlashRegionBase - NorFlashDeviceBase) / BlockSize;

  CopyGuid (&Instance->DevicePath.Vendor.Guid, &gEfiCallerIdGuid);
  Instance->DevicePath.Index = (UINT8)Index;

  NorFlashReset (Instance);

  NorFlashReadID (Instance, JedecId);
  Status = NorFlashGetInfo (JedecId, &FlashInfo, FALSE);
  if (EFI_ERROR (Status)) {
    goto FreeInstance;
  }

  NorFlashPrintInfo (FlashInfo);

  Instance->Flags = 0;
  if (FlashInfo->Flags & NOR_FLASH_WRITE_FSR) {
    Instance->Flags = NOR_FLASH_POLL_FSR;
  }

  Instance->ShadowBuffer = AllocateRuntimePool (BlockSize);
  if (Instance->ShadowBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FreeInstance;
  }

  if (HasVarStore) {
    Instance->Initialize = NorFlashFvbInitialize;
  }

  *NorFlashInstance = Instance;
  FreePool (FlashInfo);
  return EFI_SUCCESS;

FreeInstance:
  FreePool (Instance);
  return Status;
}

STATIC
EFI_STATUS
NorFlashSetHostCSDC (
  IN  NOR_FLASH_INSTANCE    *Instance,
  IN  BOOLEAN               ReadWrite,
  IN  CONST UINT16          CSDC[ARRAY_SIZE (mFip006NullCmdSeq)]
  )
{
  EFI_PHYSICAL_ADDRESS      Dst;
  UINTN                     Index;

  Dst = Instance->HostRegisterBaseAddress
        + (ReadWrite ? FIP006_REG_CS_WR : FIP006_REG_CS_RD);
  for (Index = 0; Index < ARRAY_SIZE (mFip006NullCmdSeq); Index++) {
    MmioWrite16 (Dst + (Index << 1), CSDC[Index]);
  }
  return EFI_SUCCESS;
}

STATIC
CONST CSDC_DEFINITION *
NorFlashGetCmdDef (
  IN  NOR_FLASH_INSTANCE    *Instance,
  IN  UINT8                 Code
  )
{
  CONST CSDC_DEFINITION *Cmd;
  UINTN                 Index;

  Cmd = NULL;
  for (Index = 0; Index <  ARRAY_SIZE (mN25qCSDCDefTable); Index++) {
    if (Code == mN25qCSDCDefTable[Index].Code) {
      Cmd = &mN25qCSDCDefTable[Index];
      break;
    }
  }
  return Cmd;
}

STATIC
EFI_STATUS
GenCSDC (
  IN  UINT8     Cmd,
  IN  BOOLEAN   AddrAccess,
  IN  BOOLEAN   AddrMode4Byte,
  IN  BOOLEAN   HighZ,
  IN  UINT8     TransferMode,
  OUT UINT16    *CmdSeq
  )
{
  UINTN         Index;

  if (!CmdSeq) {
    return EFI_INVALID_PARAMETER;
  }

  Index = 0;
  CopyMem (CmdSeq, mFip006NullCmdSeq, sizeof (mFip006NullCmdSeq));

  CmdSeq[Index++] = CSDC (Cmd, CSDC_CONT_NON_CONTINUOUS, TransferMode,
                          CSDC_DEC_LEAVE_ASIS);
  if (AddrAccess) {
    if (AddrMode4Byte) {
      CmdSeq[Index++] = CSDC (CSDC_ADDRESS_31_24, CSDC_CONT_NON_CONTINUOUS,
                              TransferMode, CSDC_DEC_DECODE);
    }
    CmdSeq[Index++] = CSDC (CSDC_ADDRESS_23_16, CSDC_CONT_NON_CONTINUOUS,
                            TransferMode, CSDC_DEC_DECODE);
    CmdSeq[Index++] = CSDC (CSDC_ADDRESS_15_8, CSDC_CONT_NON_CONTINUOUS,
                            TransferMode, CSDC_DEC_DECODE);
    CmdSeq[Index++] = CSDC (CSDC_ADDRESS_7_0, CSDC_CONT_NON_CONTINUOUS,
                            TransferMode, CSDC_DEC_DECODE);
  }
  if (HighZ) {
    CmdSeq[Index++] = CSDC (CSDC_HIGH_Z, CSDC_CONT_NON_CONTINUOUS,
                            TransferMode, CSDC_DEC_DECODE);
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
NorFlashSetHostCommand (
  IN  NOR_FLASH_INSTANCE    *Instance,
  IN  UINT8                 Code
  )
{
  CONST CSDC_DEFINITION     *Cmd;
  UINT16                    CSDC[ARRAY_SIZE (mFip006NullCmdSeq)];

  Cmd = NorFlashGetCmdDef (Instance, Code);
  if (Cmd == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  GenCSDC (
      Cmd->Code,
      Cmd->AddrAccess,
      Cmd->AddrMode4Byte,
      Cmd->HighZ,
      Cmd->CsdcTrp,
      CSDC
      );
  NorFlashSetHostCSDC (Instance, Cmd->ReadWrite, CSDC);
  return EFI_SUCCESS;
}

STATIC
UINT8
NorFlashReadStatusRegister (
  IN NOR_FLASH_INSTANCE     *Instance
  )
{
  UINT8       StatusRegister;

  NorFlashSetHostCommand (Instance, SPINOR_OP_RDSR);
  StatusRegister = MmioRead8 (Instance->RegionBaseAddress);
  NorFlashSetHostCommand (Instance, SPINOR_OP_READ_4B);
  return StatusRegister;
}

STATIC
EFI_STATUS
NorFlashWaitProgramErase (
  IN NOR_FLASH_INSTANCE     *Instance
  )
{
  BOOLEAN     SRegDone;
  BOOLEAN     FSRegDone;

  DEBUG ((DEBUG_BLKIO, "NorFlashWaitProgramErase()\n"));

  do {
    SRegDone = (NorFlashReadStatusRegister (Instance) & SPINOR_SR_WIP) == 0;
    FSRegDone = TRUE;
    if (Instance->Flags & NOR_FLASH_POLL_FSR) {
      NorFlashSetHostCommand (Instance, SPINOR_OP_RDFSR);
      FSRegDone = (MmioRead8 (Instance->RegionBaseAddress) &
                   SPINOR_FSR_READY) != 0;
    }
  } while (!SRegDone || !FSRegDone);
  NorFlashSetHostCommand (Instance, SPINOR_OP_READ_4B);
  return EFI_SUCCESS;
}

// TODO: implement lock checking
STATIC
BOOLEAN
NorFlashBlockIsLocked (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN UINTN                  BlockAddress
  )
{
  return FALSE;
}

// TODO: implement sector unlocking
STATIC
EFI_STATUS
NorFlashUnlockSingleBlock (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN UINTN                  BlockAddress
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
NorFlashUnlockSingleBlockIfNecessary (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN UINTN                  BlockAddress
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  if (NorFlashBlockIsLocked (Instance, BlockAddress) == TRUE) {
    Status = NorFlashUnlockSingleBlock (Instance, BlockAddress);
  }

  return Status;
}

STATIC
EFI_STATUS
NorFlashEnableWrite (
  IN  NOR_FLASH_INSTANCE    *Instance
  )
{
  EFI_STATUS      Status;
  UINT8           StatusRegister;
  UINTN           Retry;

  DEBUG ((DEBUG_BLKIO, "NorFlashEnableWrite()\n"));

  Status = EFI_DEVICE_ERROR;
  Retry = NOR_FLASH_ERASE_RETRY;

  NorFlashSetHostCSDC (Instance, TRUE, mFip006NullCmdSeq);
  while (Retry > 0 && EFI_ERROR (Status)) {
    MmioWrite8 (Instance->RegionBaseAddress, SPINOR_OP_WREN);
    MemoryFence ();
    StatusRegister = NorFlashReadStatusRegister (Instance);
    Status = (StatusRegister & BIT1) ? EFI_SUCCESS : EFI_DEVICE_ERROR;
    Retry--;
  }
  return Status;
}

STATIC
EFI_STATUS
NorFlashDisableWrite (
  IN  NOR_FLASH_INSTANCE    *Instance
  )
{
  EFI_STATUS      Status;
  UINT8           StatusRegister;
  UINTN           Retry;

  DEBUG ((DEBUG_BLKIO, "NorFlashDisableWrite()\n"));

  Status = EFI_DEVICE_ERROR;
  Retry = NOR_FLASH_ERASE_RETRY;

  NorFlashSetHostCSDC (Instance, TRUE, mFip006NullCmdSeq);
  while (Retry > 0 && EFI_ERROR (Status)) {
    MmioWrite8 (Instance->RegionBaseAddress, SPINOR_OP_WRDIS);
    MemoryFence ();
    StatusRegister = NorFlashReadStatusRegister (Instance);
    Status = (StatusRegister & BIT1) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    Retry--;
  }
  return Status;
}

/**
 * The following function presumes that the block has already been unlocked.
 **/
STATIC
EFI_STATUS
NorFlashEraseSingleBlock (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN UINTN                  BlockAddress
  )
{

  DEBUG ((DEBUG_BLKIO, "NorFlashEraseSingleBlock(BlockAddress=0x%08x)\n",
    BlockAddress));

  if (EFI_ERROR (NorFlashEnableWrite (Instance))) {
    return EFI_DEVICE_ERROR;
  }

  //
  // The virtual address chosen by the OS may have a different offset modulo
  // 16 MB than the physical address, so we need to subtract the region base
  // address before we can mask off a block index. Note that the relative
  // offset between device base address and region base address may have changed
  // as well, so we cannot use the device base address directly.
  //
  BlockAddress -= Instance->RegionBaseAddress;
  BlockAddress += Instance->OffsetLba * Instance->BlockSize;

  NorFlashSetHostCSDC (Instance, TRUE, mFip006NullCmdSeq);
  MmioWrite32 (Instance->DeviceBaseAddress,
               SwapBytes32 (BlockAddress & 0x00FFFFFF) | SPINOR_OP_SE);
  NorFlashWaitProgramErase (Instance);
  NorFlashSetHostCSDC (Instance, TRUE, mFip006NullCmdSeq);

  if (EFI_ERROR (NorFlashDisableWrite (Instance))) {
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

/**
 * This function unlock and erase an entire NOR Flash block.
 **/
EFI_STATUS
NorFlashUnlockAndEraseSingleBlock (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN UINTN                  BlockAddress
  )
{
  EFI_STATUS              Status;
  UINTN                   Index;
  NOR_FLASH_LOCK_CONTEXT  Lock;

  NorFlashLock (&Lock);

  Index = 0;
  // The block erase might fail a first time (SW bug ?). Retry it ...
  do {
    // Unlock the block if we have to
    Status = NorFlashUnlockSingleBlockIfNecessary (Instance, BlockAddress);
    if (EFI_ERROR (Status)) {
      break;
    }
    Status = NorFlashEraseSingleBlock (Instance, BlockAddress);
    Index++;
  } while ((Index < NOR_FLASH_ERASE_RETRY) && (Status == EFI_WRITE_PROTECTED));

  if (Index == NOR_FLASH_ERASE_RETRY) {
    DEBUG ((DEBUG_ERROR,
      "EraseSingleBlock(BlockAddress=0x%08x: Block Locked Error (try to erase %d times)\n",
      BlockAddress,Index));
  }

  NorFlashUnlock (&Lock);

  return Status;
}

STATIC
EFI_STATUS
NorFlashWriteSingleWord (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN UINTN                  WordAddress,
  IN UINT32                 WriteData
  )
{
  EFI_STATUS            Status;

  DEBUG ((DEBUG_BLKIO,
    "NorFlashWriteSingleWord(WordAddress=0x%08x, WriteData=0x%08x)\n",
    WordAddress, WriteData));

  Status = EFI_SUCCESS;

  if (EFI_ERROR (NorFlashEnableWrite (Instance))) {
    return EFI_DEVICE_ERROR;
  }
  NorFlashSetHostCommand (Instance, SPINOR_OP_PP);
  MmioWrite32 (WordAddress, WriteData);
  NorFlashWaitProgramErase (Instance);

  NorFlashDisableWrite (Instance);
  NorFlashSetHostCSDC (Instance, TRUE, mFip006NullCmdSeq);
  return Status;
}

STATIC
EFI_STATUS
NorFlashWriteFullBlock (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN EFI_LBA                Lba,
  IN UINT32                 *DataBuffer,
  IN UINT32                 BlockSizeInWords
  )
{
  EFI_STATUS              Status;
  UINTN                   WordAddress;
  UINT32                  WordIndex;
  UINTN                   BlockAddress;
  NOR_FLASH_LOCK_CONTEXT  Lock;

  Status = EFI_SUCCESS;

  // Get the physical address of the block
  BlockAddress = GET_NOR_BLOCK_ADDRESS (Instance->RegionBaseAddress, Lba,
                   BlockSizeInWords * 4);

  // Start writing from the first address at the start of the block
  WordAddress = BlockAddress;

  NorFlashLock (&Lock);

  Status = NorFlashUnlockAndEraseSingleBlock (Instance, BlockAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "WriteSingleBlock: ERROR - Failed to Unlock and Erase the single block at 0x%X\n",
      BlockAddress));
    goto EXIT;
  }

  for (WordIndex=0;
       WordIndex < BlockSizeInWords;
       WordIndex++, DataBuffer++, WordAddress += 4) {
    Status = NorFlashWriteSingleWord (Instance, WordAddress, *DataBuffer);
    if (EFI_ERROR (Status)) {
      goto EXIT;
    }
  }

EXIT:
  NorFlashUnlock (&Lock);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "NOR FLASH Programming [WriteSingleBlock] failed at address 0x%08x. Exit Status = \"%r\".\n",
      WordAddress, Status));
  }
  return Status;
}

EFI_STATUS
NorFlashWriteBlocks (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN EFI_LBA                Lba,
  IN UINTN                  BufferSizeInBytes,
  IN VOID                   *Buffer
  )
{
  UINT32          *pWriteBuffer;
  EFI_STATUS      Status = EFI_SUCCESS;
  EFI_LBA         CurrentBlock;
  UINT32          BlockSizeInWords;
  UINT32          NumBlocks;
  UINT32          BlockCount;

  // The buffer must be valid
  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // We must have some bytes to read
  DEBUG ((DEBUG_BLKIO, "NorFlashWriteBlocks: BufferSizeInBytes=0x%x\n",
    BufferSizeInBytes));
  if (BufferSizeInBytes == 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  // The size of the buffer must be a multiple of the block size
  DEBUG ((DEBUG_BLKIO, "NorFlashWriteBlocks: BlockSize in bytes =0x%x\n",
    Instance->BlockSize));
  if ((BufferSizeInBytes % Instance->BlockSize) != 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  // All blocks must be within the device
  NumBlocks = ((UINT32)BufferSizeInBytes) / Instance->BlockSize ;

  DEBUG ((DEBUG_BLKIO,
    "NorFlashWriteBlocks: NumBlocks=%d, LastBlock=%ld, Lba=%ld.\n", NumBlocks,
    Instance->LastBlock, Lba));

  if ((Lba + NumBlocks) > (Instance->LastBlock + 1)) {
    DEBUG ((DEBUG_ERROR,
      "NorFlashWriteBlocks: ERROR - Write will exceed last block.\n"));
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (((UINTN)Buffer % sizeof (UINT32)) == 0);

  BlockSizeInWords = Instance->BlockSize / 4;

  // Because the target *Buffer is a pointer to VOID, we must put
  // all the data into a pointer to a proper data type, so use *ReadBuffer
  pWriteBuffer = (UINT32 *)Buffer;

  CurrentBlock = Lba;
  for (BlockCount = 0;
       BlockCount < NumBlocks;
       BlockCount++, CurrentBlock++, pWriteBuffer += BlockSizeInWords) {

    DEBUG ((DEBUG_BLKIO, "NorFlashWriteBlocks: Writing block #%d\n",
      (UINTN)CurrentBlock));

    Status = NorFlashWriteFullBlock (Instance, CurrentBlock, pWriteBuffer,
               BlockSizeInWords);

    if (EFI_ERROR (Status)) {
      break;
    }
  }

  DEBUG ((DEBUG_BLKIO, "NorFlashWriteBlocks: Exit Status = \"%r\".\n", Status));
  return Status;
}

EFI_STATUS
NorFlashReadBlocks (
  IN NOR_FLASH_INSTANCE   *Instance,
  IN EFI_LBA              Lba,
  IN UINTN                BufferSizeInBytes,
  OUT VOID                *Buffer
  )
{
  UINT32              NumBlocks;
  UINTN               StartAddress;

  DEBUG ((DEBUG_BLKIO,
    "NorFlashReadBlocks: BufferSize=0x%xB BlockSize=0x%xB LastBlock=%ld, Lba=%ld.\n",
    BufferSizeInBytes, Instance->BlockSize, Instance->LastBlock,
    Lba));

  // The buffer must be valid
  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Return if we have not any byte to read
  if (BufferSizeInBytes == 0) {
    return EFI_SUCCESS;
  }

  // The size of the buffer must be a multiple of the block size
  if ((BufferSizeInBytes % Instance->BlockSize) != 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  // All blocks must be within the device
  NumBlocks = ((UINT32)BufferSizeInBytes) / Instance->BlockSize ;

  if ((Lba + NumBlocks) > (Instance->LastBlock + 1)) {
    DEBUG ((DEBUG_ERROR,
      "NorFlashReadBlocks: ERROR - Read will exceed last block\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Get the address to start reading from
  StartAddress = GET_NOR_BLOCK_ADDRESS (Instance->RegionBaseAddress, Lba,
                                        Instance->BlockSize);

  // Put the device into Read Array mode
  NorFlashSetHostCommand (Instance, SPINOR_OP_READ_4B);
  NorFlashSetHostCSDC (Instance, TRUE, mFip006NullCmdSeq);

  // Readout the data
  CopyMem(Buffer, (UINTN *)StartAddress, BufferSizeInBytes);

  return EFI_SUCCESS;
}

EFI_STATUS
NorFlashRead (
  IN NOR_FLASH_INSTANCE   *Instance,
  IN EFI_LBA              Lba,
  IN UINTN                Offset,
  IN UINTN                BufferSizeInBytes,
  OUT VOID                *Buffer
  )
{
  UINTN  StartAddress;

  // The buffer must be valid
  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Return if we have not any byte to read
  if (BufferSizeInBytes == 0) {
    return EFI_SUCCESS;
  }

  if (((Lba * Instance->BlockSize) + Offset + BufferSizeInBytes) >
      Instance->Size) {
    DEBUG ((DEBUG_ERROR,
      "NorFlashRead: ERROR - Read will exceed device size.\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Get the address to start reading from
  StartAddress = GET_NOR_BLOCK_ADDRESS (Instance->RegionBaseAddress, Lba,
                                        Instance->BlockSize);

  // Put the device into Read Array mode
  NorFlashSetHostCommand (Instance, SPINOR_OP_READ_4B);
  NorFlashSetHostCSDC (Instance, TRUE, mFip006NullCmdSeq);

  // Readout the data
  CopyMem (Buffer, (UINTN *)(StartAddress + Offset), BufferSizeInBytes);

  return EFI_SUCCESS;
}

/*
  Write a full or portion of a block. It must not span block boundaries;
  that is, Offset + *NumBytes <= Instance->BlockSize.
*/
EFI_STATUS
NorFlashWriteSingleBlock (
  IN        NOR_FLASH_INSTANCE   *Instance,
  IN        EFI_LBA               Lba,
  IN        UINTN                 Offset,
  IN OUT    UINTN                *NumBytes,
  IN        UINT8                *Buffer
  )
{
  EFI_STATUS  TempStatus;
  UINT32      Tmp;
  UINT32      TmpBuf;
  UINT32      WordToWrite;
  UINT32      Mask;
  BOOLEAN     DoErase;
  UINTN       BytesToWrite;
  UINTN       CurOffset;
  UINTN       WordAddr;
  UINTN       BlockSize;
  UINTN       BlockAddress;
  UINTN       PrevBlockAddress;

  PrevBlockAddress = 0;

  if (!Instance->Initialized && Instance->Initialize) {
    Instance->Initialize(Instance);
  }

  DEBUG ((DEBUG_BLKIO,
    "NorFlashWriteSingleBlock(Parameters: Lba=%ld, Offset=0x%x, *NumBytes=0x%x, Buffer @ 0x%08x)\n",
    Lba, Offset, *NumBytes, Buffer));

  // Cache the block size to avoid de-referencing pointers all the time
  BlockSize = Instance->BlockSize;

  // The write must not span block boundaries.
  // We need to check each variable individually because adding two large
  // values together overflows.
  if (Offset               >= BlockSize ||
      *NumBytes            >  BlockSize ||
      (Offset + *NumBytes) >  BlockSize) {
    DEBUG ((DEBUG_ERROR,
      "NorFlashWriteSingleBlock: ERROR - EFI_BAD_BUFFER_SIZE: (Offset=0x%x + NumBytes=0x%x) > BlockSize=0x%x\n",
      Offset, *NumBytes, BlockSize ));
    return EFI_BAD_BUFFER_SIZE;
  }

  // We must have some bytes to write
  if (*NumBytes == 0) {
    DEBUG ((DEBUG_ERROR,
      "NorFlashWriteSingleBlock: ERROR - EFI_BAD_BUFFER_SIZE: (Offset=0x%x + NumBytes=0x%x) > BlockSize=0x%x\n",
      Offset, *NumBytes, BlockSize ));
    return EFI_BAD_BUFFER_SIZE;
  }

  // Pick 128bytes as a good start for word operations as opposed to erasing the
  // block and writing the data regardless if an erase is really needed.
  // It looks like most individual NV variable writes are smaller than 128bytes.
  if (*NumBytes <= 128) {
    // Check to see if we need to erase before programming the data into NOR.
    // If the destination bits are only changing from 1s to 0s we can just write.
    // After a block is erased all bits in the block is set to 1.
    // If any byte requires us to erase we just give up and rewrite all of it.
    DoErase      = FALSE;
    BytesToWrite = *NumBytes;
    CurOffset    = Offset;

    while (BytesToWrite > 0) {
      // Read full word from NOR, splice as required. A word is the smallest
      // unit we can write.
      TempStatus = NorFlashRead (Instance, Lba, CurOffset & ~(0x3), sizeof(Tmp),
                     &Tmp);
      if (EFI_ERROR (TempStatus)) {
        return EFI_DEVICE_ERROR;
      }

      // Physical address of word in NOR to write.
      WordAddr = (CurOffset & ~(0x3)) +
                 GET_NOR_BLOCK_ADDRESS (Instance->RegionBaseAddress, Lba,
                   BlockSize);

      // The word of data that is to be written.
      TmpBuf = ReadUnaligned32 ((UINT32 *)(Buffer + (*NumBytes - BytesToWrite)));

      // First do word aligned chunks.
      if ((CurOffset & 0x3) == 0) {
        if (BytesToWrite >= 4) {
          // Is the destination still in 'erased' state?
          if (~Tmp != 0) {
            // Check to see if we are only changing bits to zero.
            if ((Tmp ^ TmpBuf) & TmpBuf) {
              DoErase = TRUE;
              break;
            }
          }
          // Write this word to NOR
          WordToWrite = TmpBuf;
          CurOffset += sizeof(TmpBuf);
          BytesToWrite -= sizeof(TmpBuf);
        } else {
          // BytesToWrite < 4. Do small writes and left-overs
          Mask = ~((~0) << (BytesToWrite * 8));
          // Mask out the bytes we want.
          TmpBuf &= Mask;
          // Is the destination still in 'erased' state?
          if ((Tmp & Mask) != Mask) {
            // Check to see if we are only changing bits to zero.
            if ((Tmp ^ TmpBuf) & TmpBuf) {
              DoErase = TRUE;
              break;
            }
          }
          // Merge old and new data. Write merged word to NOR
          WordToWrite = (Tmp & ~Mask) | TmpBuf;
          CurOffset += BytesToWrite;
          BytesToWrite = 0;
        }
      } else {
        // Do multiple words, but starting unaligned.
        if (BytesToWrite > (4 - (CurOffset & 0x3))) {
          Mask = ((~0) << ((CurOffset & 0x3) * 8));
          // Mask out the bytes we want.
          TmpBuf &= Mask;
          // Is the destination still in 'erased' state?
          if ((Tmp & Mask) != Mask) {
            // Check to see if we are only changing bits to zero.
            if ((Tmp ^ TmpBuf) & TmpBuf) {
              DoErase = TRUE;
              break;
            }
          }
          // Merge old and new data. Write merged word to NOR
          WordToWrite = (Tmp & ~Mask) | TmpBuf;
          BytesToWrite -= (4 - (CurOffset & 0x3));
          CurOffset += (4 - (CurOffset & 0x3));
        } else {
          // Unaligned and fits in one word.
          Mask = (~((~0) << (BytesToWrite * 8))) << ((CurOffset & 0x3) * 8);
          // Mask out the bytes we want.
          TmpBuf = (TmpBuf << ((CurOffset & 0x3) * 8)) & Mask;
          // Is the destination still in 'erased' state?
          if ((Tmp & Mask) != Mask) {
            // Check to see if we are only changing bits to zero.
            if ((Tmp ^ TmpBuf) & TmpBuf) {
              DoErase = TRUE;
              break;
            }
          }
          // Merge old and new data. Write merged word to NOR
          WordToWrite = (Tmp & ~Mask) | TmpBuf;
          CurOffset += BytesToWrite;
          BytesToWrite = 0;
        }
      }

      //
      // Write the word to NOR.
      //

      BlockAddress = GET_NOR_BLOCK_ADDRESS (Instance->RegionBaseAddress, Lba,
        BlockSize);
      if (BlockAddress != PrevBlockAddress) {
        TempStatus = NorFlashUnlockSingleBlockIfNecessary (Instance,
                       BlockAddress);
        if (EFI_ERROR (TempStatus)) {
          return EFI_DEVICE_ERROR;
        }
        PrevBlockAddress = BlockAddress;
      }
      TempStatus = NorFlashWriteSingleWord (Instance, WordAddr, WordToWrite);
      if (EFI_ERROR (TempStatus)) {
        return EFI_DEVICE_ERROR;
      }
    }
    // Exit if we got here and could write all the data. Otherwise do the
    // Erase-Write cycle.
    if (!DoErase) {
      return EFI_SUCCESS;
    }
  }

  // Check we did get some memory. Buffer is BlockSize.
  if (Instance->ShadowBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "FvbWrite: ERROR - Buffer not ready\n"));
    return EFI_DEVICE_ERROR;
  }

  // Read NOR Flash data into shadow buffer
  TempStatus = NorFlashReadBlocks (Instance, Lba, BlockSize,
                 Instance->ShadowBuffer);
  if (EFI_ERROR (TempStatus)) {
    // Return one of the pre-approved error statuses
    return EFI_DEVICE_ERROR;
  }

  // Put the data at the appropriate location inside the buffer area
  CopyMem ((VOID*)((UINTN)Instance->ShadowBuffer + Offset), Buffer, *NumBytes);

  // Write the modified buffer back to the NorFlash
  TempStatus = NorFlashWriteBlocks (Instance, Lba, BlockSize,
                 Instance->ShadowBuffer);
  if (EFI_ERROR (TempStatus)) {
    // Return one of the pre-approved error statuses
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
NorFlashReset (
  IN  NOR_FLASH_INSTANCE *Instance
  )
{
  FIP006_CS_CFG         CsCfg;

  DEBUG ((DEBUG_BLKIO, "NorFlashReset()\n"));
  CsCfg.Raw = MmioRead32 (Instance->HostRegisterBaseAddress +
                          FIP006_REG_CS_CFG);
  CsCfg.Reg.MBM = CS_CFG_MBM_SINGLE;
  CsCfg.Reg.SRAM = CS_CFG_SRAM_RW;
  MmioWrite32 (Instance->HostRegisterBaseAddress + FIP006_REG_CS_CFG,
               CsCfg.Raw);
  NorFlashSetHostCommand (Instance, SPINOR_OP_READ_4B);
  NorFlashSetHostCSDC (Instance, TRUE, mFip006NullCmdSeq);
  return EFI_SUCCESS;
}

EFI_STATUS
NorFlashReadID (
  IN  NOR_FLASH_INSTANCE  *Instance,
  OUT UINT8               JedecId[3]
  )
{
  if (Instance == NULL || JedecId == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  NorFlashSetHostCommand (Instance, SPINOR_OP_RDID);
  JedecId[0] = MmioRead8 (Instance->DeviceBaseAddress);
  JedecId[1] = MmioRead8 (Instance->DeviceBaseAddress + 1);
  JedecId[2] = MmioRead8 (Instance->DeviceBaseAddress + 2);
  NorFlashSetHostCommand (Instance, SPINOR_OP_READ_4B);
  return EFI_SUCCESS;
}
