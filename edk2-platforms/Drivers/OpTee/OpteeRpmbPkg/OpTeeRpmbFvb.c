/** @file

  FV block I/O protocol driver for RPMB eMMC accessed via OP-TEE

  Copyright (c) 2020, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Library/ArmSvcLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MmServicesTableLib.h>
#include <Library/PcdLib.h>

#include <IndustryStandard/ArmFfaSvc.h>
#include <IndustryStandard/ArmMmSvc.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/SmmFirmwareVolumeBlock.h>
#include <Guid/VariableFormat.h>

#include "OpTeeRpmbFvb.h"

// These are what OP-TEE expects in ./core/arch/arm/kernel/stmm_sp.c
// Since the FFA autodiscovery mechanism is not yet implemented we are
// hardcoding the ID values for the two operations OP-TEE currently supports
//
// mMemMgrId is used to set the page permissions after relocating the executable
// mStorageId is used to access the RPMB partition via OP-TEE
// In both cases the return value is located in x3. Once the autodiscovery mechanism
// is in place, we'll have to account for an error value in x2 as well, handling
// the autodiscovery failed scenario
STATIC CONST UINT16 mMemMgrId = 3U;
STATIC CONST UINT16 mStorageId = 4U;

STATIC MEM_INSTANCE mInstance;

/**
  Sends an SVC call to OP-TEE for reading/writing an RPMB partition

  @param[in] SvcAct      SVC ID for read/write
  @param[in] Addr        Base address of the buffer. When reading contents will be
                         copied to that buffer after reading them from the device.
                         When writing, the buffer holds the contents we want to
                         write cwtoin the device
  @param[in] NumBytes    Number of bytes to read/write
  @param[in] Offset      Offset into the RPMB file

  @retval    EFI_SUCCESS           read/write ok
  @retval    EFI_UNSUPPORTED       SVC to op-tee not supported
  @retval    EFI_INVALID_PARAMETER SVC to op-tee had an invalid param
  @retval    EFI_ACCESS_DENIED     SVC to op-tee was denied
  @retval    EFI_OUT_OF_RESOURCES  op-tee out of memory
**/
STATIC
EFI_STATUS
ReadWriteRpmb (
  IN UINTN SvcAct,
  IN UINTN Addr,
  IN UINTN NumBytes,
  IN UINTN Offset
  )
{
  ARM_SVC_ARGS  SvcArgs;
  EFI_STATUS    Status;

  ZeroMem (&SvcArgs, sizeof (SvcArgs));

  SvcArgs.Arg0 = ARM_SVC_ID_FFA_MSG_SEND_DIRECT_REQ;
  SvcArgs.Arg1 = mStorageId;
  SvcArgs.Arg2 = 0;
  SvcArgs.Arg3 = SvcAct;
  SvcArgs.Arg4 = Addr;
  SvcArgs.Arg5 = NumBytes;
  SvcArgs.Arg6 = Offset;

  ArmCallSvc (&SvcArgs);
  if (SvcArgs.Arg3) {
    DEBUG ((DEBUG_ERROR, "%a: Svc Call 0x%08x Addr: 0x%08x len: 0x%x Offset: 0x%x failed with 0x%x\n",
      __func__, SvcAct, Addr, NumBytes, Offset, SvcArgs.Arg3));
  }

  switch (SvcArgs.Arg3) {
  case ARM_SVC_SPM_RET_SUCCESS:
    Status = EFI_SUCCESS;
    break;

  case ARM_SVC_SPM_RET_NOT_SUPPORTED:
    Status = EFI_UNSUPPORTED;
    break;

  case ARM_SVC_SPM_RET_INVALID_PARAMS:
    Status = EFI_INVALID_PARAMETER;
    break;

  case ARM_SVC_SPM_RET_DENIED:
    Status = EFI_ACCESS_DENIED;
    break;

  case ARM_SVC_SPM_RET_NO_MEMORY:
    Status = EFI_OUT_OF_RESOURCES;
    break;

  default:
    Status = EFI_ACCESS_DENIED;
  }

  return Status;
}

/**
  The GetAttributes() function retrieves the attributes and
  current settings of the block.

  @param[in] This        Indicates the EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL
                         instance.
  @param[out] Attributes Pointer to EFI_FVB_ATTRIBUTES_2 in which the
                         attributes and current settings are
                         returned. Type EFI_FVB_ATTRIBUTES_2 is defined
                         in EFI_FIRMWARE_VOLUME_HEADER.

  @retval EFI_SUCCESS    The firmware volume attributes were
                         returned.

**/
STATIC
EFI_STATUS
OpTeeRpmbFvbGetAttributes (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  OUT       EFI_FVB_ATTRIBUTES_2               *Attributes
  )
{
  *Attributes = EFI_FVB2_READ_ENABLED_CAP   | // Reads may be enabled
                EFI_FVB2_READ_STATUS        | // Reads are currently enabled
                EFI_FVB2_WRITE_STATUS       | // Writes are currently enabled
                EFI_FVB2_WRITE_ENABLED_CAP  | // Writes may be enabled
                EFI_FVB2_STICKY_WRITE       | // A block erase is required to flip bits into EFI_FVB2_ERASE_POLARITY
                EFI_FVB2_MEMORY_MAPPED      | // It is memory mapped
                EFI_FVB2_ERASE_POLARITY;      // After erasure all bits take this value (i.e. '1')

  return EFI_SUCCESS;
}

/**
  The SetAttributes() function sets configurable firmware volume
  attributes and returns the new settings of the firmware volume.

  @param[in] This         Indicates the EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL instance.
  @param[in] Attributes   On input, Attributes is a pointer to
                          EFI_FVB_ATTRIBUTES_2 that contains the
                          desired firmware volume settings. On
                          successful return, it contains the new
                          settings of the firmware volume. Type
                          EFI_FVB_ATTRIBUTES_2 is defined in
                          EFI_FIRMWARE_VOLUME_HEADER.

  @retval EFI_UNSUPPORTED Set attributes not supported

**/
STATIC
EFI_STATUS
OpTeeRpmbFvbSetAttributes (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  IN OUT    EFI_FVB_ATTRIBUTES_2               *Attributes
  )
{
  DEBUG ((DEBUG_ERROR, "FvbSetAttributes(0x%X) is not supported\n", *Attributes));
  return EFI_UNSUPPORTED;
}

/**
  The GetPhysicalAddress() function retrieves the base address of
  a memory-mapped firmware volume. This function should be called
  only for memory-mapped firmware volumes.

  @param[in]  This              Indicates the EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL instance.
  @param[out] Address           Pointer to a caller-allocated
                                EFI_PHYSICAL_ADDRESS that, on successful
                                return from GetPhysicalAddress(), contains the
                                base address of the firmware volume.

  @retval     EFI_SUCCESS       The firmware volume base address was returned.
  @retval     EFI_UNSUPPORTED   The firmware volume is not memory mapped.

**/
STATIC
EFI_STATUS
OpTeeRpmbFvbGetPhysicalAddress (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  OUT       EFI_PHYSICAL_ADDRESS               *Address
  )
{
  MEM_INSTANCE *Instance;

  Instance = INSTANCE_FROM_FVB_THIS (This);
  *Address = Instance->MemBaseAddress;

  return EFI_SUCCESS;
}

/**
  The GetBlockSize() function retrieves the size of the requested
  block. It also returns the number of additional blocks with
  the identical size. The GetBlockSize() function is used to
  retrieve the block map (see EFI_FIRMWARE_VOLUME_HEADER).


  @param[in]  This                Indicates the EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL instance.
  @param[in]  Lba                 Indicates the block for which to return the size.
  @param[out] BlockSize           Pointer to a caller-allocated UINTN in which
                                  the size of the block is returned.
  @param[out] NumberOfBlocks      Pointer to a caller-allocated UINTN in
                                  which the number of consecutive blocks,
                                  starting with Lba, is returned. All
                                  blocks in this range have a size of
                                  BlockSize.

  @retval EFI_SUCCESS             The firmware volume base address was returned.
  @retval EFI_INVALID_PARAMETER   The requested LBA is out of range.

**/
STATIC
EFI_STATUS
OpTeeRpmbFvbGetBlockSize (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  IN        EFI_LBA                            Lba,
  OUT       UINTN                              *BlockSize,
  OUT       UINTN                              *NumberOfBlocks
  )
{
  MEM_INSTANCE *Instance;

  Instance = INSTANCE_FROM_FVB_THIS (This);
  if (Lba > Instance->NBlocks) {
    return EFI_INVALID_PARAMETER;
  }

  *NumberOfBlocks = Instance->NBlocks - (UINTN)Lba;
  *BlockSize = Instance->BlockSize;

  return EFI_SUCCESS;
}

/**
  Reads the specified number of bytes into a buffer from the specified block.

  The Read() function reads the requested number of bytes from the
  requested block and stores them in the provided buffer.
  Implementations should be mindful that the firmware volume
  might be in the ReadDisabled state. If it is in this state,
  the Read() function must return the status code
  EFI_ACCESS_DENIED without modifying the contents of the
  buffer. The Read() function must also prevent spanning block
  boundaries. If a read is requested that would span a block
  boundary, the read must read up to the boundary but not
  beyond. The output parameter NumBytes must be set to correctly
  indicate the number of bytes actually read. The caller must be
  aware that a read may be partially completed.

  @param[in]     This         Indicates the EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL instance.
  @param[in]     Lba          The starting logical block index
  @param[in]     Offset       Offset into the block at which to begin reading.
  @param[in,out] NumBytes     Pointer to a UINTN. At entry, *NumBytes
                              contains the total size of the buffer. At
                              exit, *NumBytes contains the total number of
                              bytes read.
  @param[in,out] Buffer       Pointer to a caller-allocated buffer that will
                              be used to hold the data that is read.

  @retval EFI_SUCCESS         The firmware volume was read successfully,
                              and contents are in Buffer.
  @retval EFI_BAD_BUFFER_SIZE Read attempted across an LBA
                              boundary. On output, NumBytes
                              contains the total number of bytes
                              returned in Buffer.
  @retval EFI_ACCESS_DENIED   The firmware volume is in the
                              ReadDisabled state.
  @retval EFI_DEVICE_ERROR    The block device is not
                              functioning correctly and could
                              not be read.

**/
STATIC
EFI_STATUS
OpTeeRpmbFvbRead (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  IN        EFI_LBA                             Lba,
  IN        UINTN                               Offset,
  IN OUT    UINTN                               *NumBytes,
  IN OUT    UINT8                               *Buffer
  )
{
  EFI_STATUS   Status;
  MEM_INSTANCE *Instance;
  VOID         *Base;

  Status = EFI_SUCCESS;
  Instance = INSTANCE_FROM_FVB_THIS (This);
  if (!Instance->Initialized) {
    Status = Instance->Initialize (Instance);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Base = (VOID *)(UINTN)Instance->MemBaseAddress + (Lba * Instance->BlockSize) +
         Offset;
  // We could read the data from the RPMB instead of memory
  // The 2 copies should already be identical
  // Copy from memory image
  CopyMem (Buffer, Base, *NumBytes);

  return Status;
}

/**
  Writes the specified number of bytes from the input buffer to the block.

  The Write() function writes the specified number of bytes from
  the provided buffer to the specified block and offset. If the
  firmware volume is sticky write, the caller must ensure that
  all the bits of the specified range to write are in the
  EFI_FVB_ERASE_POLARITY state before calling the Write()
  function, or else the result will be unpredictable. This
  unpredictability arises because, for a sticky-write firmware
  volume, a write may negate a bit in the EFI_FVB_ERASE_POLARITY
  state but cannot flip it back again.  Before calling the
  Write() function,  it is recommended for the caller to first call
  the EraseBlocks() function to erase the specified block to
  write. A block erase cycle will transition bits from the
  (NOT)EFI_FVB_ERASE_POLARITY state back to the
  EFI_FVB_ERASE_POLARITY state. Implementations should be
  mindful that the firmware volume might be in the WriteDisabled
  state. If it is in this state, the Write() function must
  return the status code EFI_ACCESS_DENIED without modifying the
  contents of the firmware volume. The Write() function must
  also prevent spanning block boundaries. If a write is
  requested that spans a block boundary, the write must store up
  to the boundary but not beyond. The output parameter NumBytes
  must be set to correctly indicate the number of bytes actually
  written. The caller must be aware that a write may be
  partially completed. All writes, partial or otherwise, must be
  fully flushed to the hardware before the Write() service
  returns.

  @param[in]     This                Indicates the EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL
                                     instance.
  @param[in]     Lba                 The starting logical block index to write to.
  @param[in]     Offset              Offset into the block at which to begin writing.
  @param[in,out] NumBytes            The pointer to a UINTN. At entry, *NumBytes
                                     contains the total size of the buffer. At
                                     exit, *NumBytes contains the total number of
                                     bytes actually written.
  @param[in]     Buffer              The pointer to a caller-allocated buffer
                                     that contains the source for the write.

  @retval        EFI_SUCCESS         The firmware volume was written
                                     successfully.
  @retval        EFI_BAD_BUFFER_SIZE The write was attempted across an
                                     LBA boundary. On output, NumBytes
                                     contains the total number of bytes
                                     actually written.
  @retval        EFI_ACCESS_DENIED   The firmware volume is in the
                                     WriteDisabled state.
  @retval        EFI_DEVICE_ERROR    The block device is malfunctioning
                                     and could not be written.
**/
STATIC
EFI_STATUS
OpTeeRpmbFvbWrite (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  IN        EFI_LBA                             Lba,
  IN        UINTN                               Offset,
  IN OUT    UINTN                               *NumBytes,
  IN        UINT8                               *Buffer
  )
{
  MEM_INSTANCE *Instance;
  EFI_STATUS   Status;
  VOID         *Base;

  Instance = INSTANCE_FROM_FVB_THIS (This);
  if (!Instance->Initialized) {
    Status = Instance->Initialize (Instance);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  Base = (VOID *)(UINTN)Instance->MemBaseAddress + (Lba * Instance->BlockSize) +
         Offset;
  Status = ReadWriteRpmb (
             SP_SVC_RPMB_WRITE,
             (UINTN)Buffer,
             *NumBytes,
             (Lba * Instance->BlockSize) + Offset
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Update the memory copy
  CopyMem (Base, Buffer, *NumBytes);

  return Status;
}

/**
  Erases and initializes a firmware volume block.

  The EraseBlocks() function erases one or more blocks as denoted
  by the variable argument list. The entire parameter list of
  blocks must be verified before erasing any blocks. If a block is
  requested that does not exist within the associated firmware
  volume (it has a larger index than the last block of the
  firmware volume), the EraseBlocks() function must return the
  status code EFI_INVALID_PARAMETER without modifying the contents
  of the firmware volume. Implementations should be mindful that
  the firmware volume might be in the WriteDisabled state. If it
  is in this state, the EraseBlocks() function must return the
  status code EFI_ACCESS_DENIED without modifying the contents of
  the firmware volume. All calls to EraseBlocks() must be fully
  flushed to the hardware before the EraseBlocks() service
  returns.

  @param[in] This               Indicates the EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL
                                instance.
  @param[in] ...                The variable argument list is a list of tuples.
                                Each tuple describes a range of LBAs to erase
                                and consists of the following:
                                - An EFI_LBA that indicates the starting LBA
                                - A UINTN that indicates the number of blocks to
                                erase.

                                The list is terminated with an
                                EFI_LBA_LIST_TERMINATOR. For example, the
                                following indicates that two ranges of blocks
                                (5-7 and 10-11) are to be erased: EraseBlocks
                                (This, 5, 3, 10, 2, EFI_LBA_LIST_TERMINATOR);

  @retval EFI_SUCCESS           The erase request successfully
                                completed.
  @retval EFI_ACCESS_DENIED     The firmware volume is in the
                                WriteDisabled state.
  @retval EFI_DEVICE_ERROR      The block device is not functioning
                                correctly and could not be written.
                                The firmware device may have been
                                partially erased.
  @retval EFI_INVALID_PARAMETER One or more of the LBAs listed
                                in the variable argument list do
                                not exist in the firmware volume.

**/
STATIC
EFI_STATUS
OpTeeRpmbFvbErase (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  ...
  )
{
  MEM_INSTANCE *Instance;
  UINTN   NumBytes;
  UINTN   NumLba;
  EFI_LBA Start;
  VOID    *Base;
  VOID    *Buf;
  VA_LIST Args;
  EFI_STATUS Status;

  Instance = INSTANCE_FROM_FVB_THIS (This);

  VA_START (Args, This);
  for (Start = VA_ARG (Args, EFI_LBA);
       Start != EFI_LBA_LIST_TERMINATOR;
       Start = VA_ARG (Args, EFI_LBA)) {
    NumLba = VA_ARG (Args, UINTN);
    if (NumLba == 0 || Start + NumLba > Instance->NBlocks) {
      return EFI_INVALID_PARAMETER;
    }
    NumBytes = NumLba * Instance->BlockSize;
    Base = (VOID *)(UINTN)Instance->MemBaseAddress +
           (Start * Instance->BlockSize);
    Buf = AllocatePool (NumLba * Instance->BlockSize);
    if (Buf == NULL) {
      return EFI_DEVICE_ERROR;
    }
    SetMem64 (Buf, NumLba * Instance->BlockSize, ~0UL);
    // Write the device
    Status = ReadWriteRpmb (
               SP_SVC_RPMB_WRITE,
               (UINTN)Buf,
               NumBytes,
               Start * Instance->BlockSize
               );
    if (EFI_ERROR (Status)) {
      FreePool (Buf);
      return Status;
    }
    // Update the in memory copy
    SetMem64 (Base, NumLba * Instance->BlockSize, ~0UL);
    FreePool (Buf);
  }

  VA_END (Args);

  return EFI_SUCCESS;
}

/**
  Since we use a memory backed storage we need to restore the RPMB contents
  into memory before we register the Fvb protocol.

  @param Instance Address to copy flash contents to
**/
STATIC
VOID
ReadEntireFlash (
  IN MEM_INSTANCE *Instance
 )
{
  UINTN ReadAddr;
  UINTN StorageFtwWorkingSize;
  UINTN StorageVariableSize;
  UINTN StorageFtwSpareSize;

  StorageFtwWorkingSize = PcdGet32 (PcdFlashNvStorageFtwWorkingSize);
  StorageVariableSize   = PcdGet32 (PcdFlashNvStorageVariableSize);
  StorageFtwSpareSize   = PcdGet32 (PcdFlashNvStorageFtwSpareSize);

  ReadAddr = Instance->MemBaseAddress;
  // There's no need to check if the read failed here. The upper EDK2 layers
  // will initialize the flash correctly if the in-memory copy is wrong
  ReadWriteRpmb (
    SP_SVC_RPMB_READ,
    ReadAddr,
    StorageVariableSize + StorageFtwWorkingSize + StorageFtwSpareSize,
    0
    );
}

/**
  Validate the firmware volume header.

  @param[in]                    FwVolHeader Pointer to the firmware volume
                                header for the RPMB

  @retval EFI_SUCCESS           The firmware volume header is correct
  @retval EFI_NOT_FOUND         No header present
  @retval EFI_VOLUME_CORRUPTED  The firmware volume header CRC was invalid
                                or either one of GUID's, Signature and header
                                length was invalid
**/
STATIC
EFI_STATUS
EFIAPI
ValidateFvHeader (
  IN EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader
  )
{
  UINT16                      Checksum;
  VARIABLE_STORE_HEADER       *VariableStoreHeader;
  UINTN                       VariableStoreLength;
  UINTN                       FvLength;

  FvLength = PcdGet32 (PcdFlashNvStorageVariableSize) +
             PcdGet32 (PcdFlashNvStorageFtwWorkingSize) +
             PcdGet32 (PcdFlashNvStorageFtwSpareSize);

  // Verify the header revision, header signature, length
  // Length of FvBlock cannot be 2**64-1
  // HeaderLength cannot be an odd number
  //
  if ((FwVolHeader->Revision  != EFI_FVH_REVISION)
      || (FwVolHeader->Signature != EFI_FVH_SIGNATURE)
      || (FwVolHeader->FvLength  != FvLength)) {
    DEBUG ((DEBUG_INFO, "%a: No Firmware Volume header present\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  // Check the Firmware Volume Guid
  if (!CompareGuid (&FwVolHeader->FileSystemGuid, &gEfiSystemNvDataFvGuid)) {
    DEBUG ((DEBUG_INFO, "%a: Firmware Volume Guid non-compatible\n",
      __FUNCTION__));
    return EFI_VOLUME_CORRUPTED;
  }

  // Verify the header checksum
  Checksum = CalculateSum16 ((UINT16*)FwVolHeader, FwVolHeader->HeaderLength);
  if (Checksum != 0) {
    DEBUG ((DEBUG_INFO, "%a: FV checksum is invalid (Checksum:0x%X)\n",
      __FUNCTION__, Checksum));
    return EFI_VOLUME_CORRUPTED;
  }

  VariableStoreHeader = (VARIABLE_STORE_HEADER*)((UINTN)FwVolHeader +
                         FwVolHeader->HeaderLength);

  // Check the Variable Store Guid
  if (!CompareGuid (&VariableStoreHeader->Signature, &gEfiVariableGuid) &&
      !CompareGuid (&VariableStoreHeader->Signature, &gEfiAuthenticatedVariableGuid)) {
    DEBUG ((DEBUG_INFO, "%a: Variable Store Guid non-compatible\n", __FUNCTION__));
    return EFI_VOLUME_CORRUPTED;
  }

  VariableStoreLength = PcdGet32 (PcdFlashNvStorageVariableSize) -
                        FwVolHeader->HeaderLength;
  if (VariableStoreHeader->Size != VariableStoreLength) {
    DEBUG ((DEBUG_INFO, "%a: Variable Store Length does not match\n",
      __FUNCTION__));
    return EFI_VOLUME_CORRUPTED;
  }

  return EFI_SUCCESS;
}

/**
  Initialize Fvb and variable storage headers for the RPMB storage.

  @param[in,out]    Instance   MEM_INSTANCE pointer describing the device and
                               the Firmware Block Protocol

  @retval           EFI_SUCCESS read/write ok
  @retval           EFI_UNSUPPORTED SVC to op-tee not supported
  @retval           EFI_INVALID_PARAMETER SVC to op-tee had an invalid param
  @retval           EFI_ACCESS_DENIED SVC to op-tee was denied
  @retval           EFI_OUT_OF_RESOURCES op-tee out of memory
**/
STATIC
EFI_STATUS
InitializeFvAndVariableStoreHeaders (
  IN OUT MEM_INSTANCE *Instance
  )
{
  EFI_FIRMWARE_VOLUME_HEADER *FirmwareVolumeHeader;
  VARIABLE_STORE_HEADER      *VariableStoreHeader;
  EFI_STATUS                 Status;
  UINTN                      HeadersLength;
  VOID*                      Headers;

  HeadersLength = sizeof (EFI_FIRMWARE_VOLUME_HEADER) +
                  sizeof (EFI_FV_BLOCK_MAP_ENTRY) +
                  sizeof (VARIABLE_STORE_HEADER);
  Headers = AllocateZeroPool (HeadersLength);
  if (Headers == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // EFI_FIRMWARE_VOLUME_HEADER
  //
  FirmwareVolumeHeader = (EFI_FIRMWARE_VOLUME_HEADER*)Headers;
  CopyGuid (&FirmwareVolumeHeader->FileSystemGuid, &gEfiSystemNvDataFvGuid);
  FirmwareVolumeHeader->FvLength = PcdGet32 (PcdFlashNvStorageVariableSize) +
                                   PcdGet32 (PcdFlashNvStorageFtwWorkingSize) +
                                   PcdGet32 (PcdFlashNvStorageFtwSpareSize);
  FirmwareVolumeHeader->Signature = EFI_FVH_SIGNATURE;
  FirmwareVolumeHeader->Attributes = EFI_FVB2_READ_ENABLED_CAP |
                                     EFI_FVB2_READ_STATUS |
                                     EFI_FVB2_STICKY_WRITE |
                                     EFI_FVB2_MEMORY_MAPPED |
                                     EFI_FVB2_ERASE_POLARITY |
                                     EFI_FVB2_WRITE_STATUS |
                                     EFI_FVB2_WRITE_ENABLED_CAP;

  FirmwareVolumeHeader->HeaderLength = sizeof (EFI_FIRMWARE_VOLUME_HEADER) +
                                       sizeof (EFI_FV_BLOCK_MAP_ENTRY);
  FirmwareVolumeHeader->Revision = EFI_FVH_REVISION;
  FirmwareVolumeHeader->BlockMap[0].NumBlocks = Instance->NBlocks;
  FirmwareVolumeHeader->BlockMap[0].Length    = Instance->BlockSize;
  FirmwareVolumeHeader->BlockMap[1].NumBlocks = 0;
  FirmwareVolumeHeader->BlockMap[1].Length    = 0;
  FirmwareVolumeHeader->Checksum = CalculateCheckSum16 (
                                     (UINT16*)FirmwareVolumeHeader,
                                     FirmwareVolumeHeader->HeaderLength
                                     );

  //
  // VARIABLE_STORE_HEADER
  //
  VariableStoreHeader = (VARIABLE_STORE_HEADER*)((UINTN)Headers +
                         FirmwareVolumeHeader->HeaderLength);
  CopyGuid (&VariableStoreHeader->Signature, &gEfiAuthenticatedVariableGuid);
  VariableStoreHeader->Size = PcdGet32 (PcdFlashNvStorageVariableSize) -
                              FirmwareVolumeHeader->HeaderLength;
  VariableStoreHeader->Format = VARIABLE_STORE_FORMATTED;
  VariableStoreHeader->State = VARIABLE_STORE_HEALTHY;

  Status = ReadWriteRpmb (SP_SVC_RPMB_WRITE, (UINTN)Headers, HeadersLength, 0);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  // Install the combined header in memory
  CopyMem ((VOID*)(UINTN)Instance->MemBaseAddress, Headers, HeadersLength);

Exit:
  FreePool (Headers);
  return Status;
}

/**
  Initialize the firmware block protocol for the specified memory.

  @param[in,out] Instance                  MEM_INSTANCE pointer describing the device and the
                                           Firmware Block Protocol

  @retval        EFI_SUCCESS               Initialized successfully or already initialized
  @retval        EFI_UNSUPPORTED           SVC to op-tee not supported
  @retval        EFI_INVALID_PARAMETER     SVC to op-tee had an invalid param
  @retval        EFI_ACCESS_DENIED         SVC to op-tee was denied
  @retval        EFI_OUT_OF_RESOURCES      op-tee out of memory
**/
STATIC
EFI_STATUS
EFIAPI
FvbInitialize (
  IN OUT MEM_INSTANCE *Instance
  )
{
  EFI_FIRMWARE_VOLUME_HEADER *FwVolHeader;
  EFI_STATUS                  Status;

  if (Instance->Initialized) {
    return EFI_SUCCESS;
  }

  // FirmwareVolumeHeader->FvLength is declared to have the Variable area
  // AND the FTW working area AND the FTW Spare contiguous.
  ASSERT (
    (PcdGet64 (PcdFlashNvStorageVariableBase64) +
    PcdGet32 (PcdFlashNvStorageVariableSize)) ==
    PcdGet64 (PcdFlashNvStorageFtwWorkingBase64)
    );
  ASSERT (
    (PcdGet64 (PcdFlashNvStorageFtwWorkingBase64) +
    PcdGet32 (PcdFlashNvStorageFtwWorkingSize)) ==
    PcdGet64 (PcdFlashNvStorageFtwSpareBase64));

  // Check if the size of the area is at least one block size
  ASSERT (PcdGet32 (PcdFlashNvStorageVariableSize) / Instance->BlockSize > 0);
  ASSERT (PcdGet32 (PcdFlashNvStorageFtwWorkingSize) / Instance->BlockSize > 0);
  ASSERT (PcdGet32 (PcdFlashNvStorageFtwSpareSize) / Instance->BlockSize > 0);

  // Ensure the Variable areas are aligned on block size boundaries
  ASSERT ((PcdGet64 (PcdFlashNvStorageVariableBase64) % Instance->BlockSize) == 0);
  ASSERT ((PcdGet64 (PcdFlashNvStorageFtwWorkingBase64) % Instance->BlockSize) == 0);
  ASSERT ((PcdGet64 (PcdFlashNvStorageFtwSpareBase64) % Instance->BlockSize) == 0);

  // Read the file from disk and copy it to memory
  ReadEntireFlash (Instance);

  FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)Instance->MemBaseAddress;
  Status = ValidateFvHeader (FwVolHeader);
  if (EFI_ERROR (Status)) {
    // There is no valid header, so time to install one.
    DEBUG ((DEBUG_INFO, "%a: The FVB Header is not valid.\n", __FUNCTION__));

    // Reset memory
    SetMem64 (
      (VOID *)(UINTN)Instance->MemBaseAddress,
      Instance->NBlocks * Instance->BlockSize,
      ~0UL
      );
    DEBUG ((DEBUG_INFO, "%a: Erasing Flash.\n", __FUNCTION__));
    Status = ReadWriteRpmb (
               SP_SVC_RPMB_WRITE,
               Instance->MemBaseAddress,
               PcdGet32 (PcdFlashNvStorageVariableSize) +
               PcdGet32 (PcdFlashNvStorageFtwWorkingSize) +
               PcdGet32 (PcdFlashNvStorageFtwSpareSize),
               0
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    // Install all appropriate headers
    DEBUG ((DEBUG_INFO, "%a: Installing a correct one for this volume.\n",
      __FUNCTION__));
    Status = InitializeFvAndVariableStoreHeaders (Instance);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: Found valid FVB Header.\n", __FUNCTION__));
  }
  Instance->Initialized = TRUE;

  return Status;
}

/**
  Since the RPMB is not byte addressable we need to allocate memory
  and sync that on reads/writes. Initialize the memory and install the
  Fvb protocol.

  @param[in] ImageHandle The EFI image handle
  @param[in] SystemTable MM system table

  @retval    EFI_SUCCESS Protocol installed
  @retval    EFI_INVALID_PARAMETER Invalid Pcd values specified
  @retval    EFI_OUT_OF_RESOURCES  Can't allocate necessary memory
**/
EFI_STATUS
EFIAPI
OpTeeRpmbFvbInit (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_MM_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS   Status;
  VOID         *Addr;
  UINTN        FvLength;
  UINTN        NBlocks;

  FvLength = PcdGet32 (PcdFlashNvStorageVariableSize) +
             PcdGet32 (PcdFlashNvStorageFtwWorkingSize) +
             PcdGet32 (PcdFlashNvStorageFtwSpareSize);

  NBlocks = EFI_SIZE_TO_PAGES (ALIGN_VARIABLE (FvLength));
  Addr = AllocatePages (NBlocks);
  if (Addr == NULL) {
    ASSERT (0);
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&mInstance, sizeof (mInstance));

  mInstance.FvbProtocol.GetPhysicalAddress = OpTeeRpmbFvbGetPhysicalAddress;
  mInstance.FvbProtocol.GetAttributes      = OpTeeRpmbFvbGetAttributes;
  mInstance.FvbProtocol.SetAttributes      = OpTeeRpmbFvbSetAttributes;
  mInstance.FvbProtocol.GetBlockSize       = OpTeeRpmbFvbGetBlockSize;
  mInstance.FvbProtocol.EraseBlocks        = OpTeeRpmbFvbErase;
  mInstance.FvbProtocol.Write              = OpTeeRpmbFvbWrite;
  mInstance.FvbProtocol.Read               = OpTeeRpmbFvbRead;

  mInstance.MemBaseAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)Addr;
  mInstance.Signature      = FLASH_SIGNATURE;
  mInstance.Initialize     = FvbInitialize;
  mInstance.BlockSize      = EFI_PAGE_SIZE;
  mInstance.NBlocks        = NBlocks;

  // Update the defined PCDs related to Variable Storage
  PatchPcdSet64 (PcdFlashNvStorageVariableBase64, mInstance.MemBaseAddress);
  PatchPcdSet64 (
    PcdFlashNvStorageFtwWorkingBase64,
    mInstance.MemBaseAddress + PcdGet32 (PcdFlashNvStorageVariableSize)
    );
  PatchPcdSet64 (
    PcdFlashNvStorageFtwSpareBase64,
    mInstance.MemBaseAddress +
    PcdGet32 (PcdFlashNvStorageVariableSize) +
    PcdGet32 (PcdFlashNvStorageFtwWorkingSize)
    );

  Status = gMmst->MmInstallProtocolInterface (
                    &mInstance.Handle,
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mInstance.FvbProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a: Register OP-TEE RPMB Fvb\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "%a: Using NV store FV in-memory copy at 0x%lx\n",
    __FUNCTION__, PatchPcdGet64 (PcdFlashNvStorageVariableBase64)));

  return Status;
}
