/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.
Copyright (c) 2020, Arm Limited. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#include "MvSpiFlashDxe.h"

STATIC EFI_EVENT            mMvSpiFlashVirtualAddrChangeEvent;
MARVELL_SPI_MASTER_PROTOCOL *SpiMasterProtocol;
SPI_FLASH_INSTANCE  *mSpiFlashInstance;

STATIC
VOID
SpiFlashFormatAddress (
  IN      UINT32  Address,
  IN      UINT8   AddrSize,
  IN OUT  UINT8   *Cmd
  )
{
  if (AddrSize == 4) {
      Cmd[1] = Address >> 24;
      Cmd[2] = Address >> 16;
      Cmd[3] = Address >> 8;
      Cmd[4] = Address;
  } else {
      Cmd[1] = Address >> 16;
      Cmd[2] = Address >> 8;
      Cmd[3] = Address;
  }
}

STATIC
EFI_STATUS
MvSpiFlashReadCmd (
  IN  SPI_DEVICE *Slave,
  IN  UINT8 *Cmd,
  IN  UINTN CmdSize,
  OUT UINT8 *DataIn,
  IN  UINTN DataSize
  )
{
  EFI_STATUS Status;

  // Send command and gather response
  Status = SpiMasterProtocol->ReadWrite (SpiMasterProtocol, Slave, Cmd,
    CmdSize, NULL, DataIn, DataSize);

  return Status;
}

STATIC
EFI_STATUS
MvSpiFlashWriteEnableCmd (
  IN  SPI_DEVICE   *Slave
  )
{
  EFI_STATUS Status;
  UINT8 CmdEn = CMD_WRITE_ENABLE;

  // Send write_enable command
  Status = SpiMasterProtocol->Transfer (SpiMasterProtocol, Slave, 1,
    &CmdEn, NULL, SPI_TRANSFER_BEGIN | SPI_TRANSFER_END);

  return Status;
}

STATIC
EFI_STATUS
MvSpiFlashWriteCommon (
  IN SPI_DEVICE *Slave,
  IN UINT8 *Cmd,
  IN UINT32 Length,
  IN UINT8* Buffer,
  IN UINT32 BufferLength
  )
{
  UINT8 CmdStatus = CMD_READ_STATUS;
  UINT8 State;
  UINT32 Counter = 0xFFFFF;
  UINT8 PollBit = STATUS_REG_POLL_WIP;
  UINT8 CheckStatus = 0x0;

  if (Slave->Info->Flags & NOR_FLASH_WRITE_FSR) {
    CmdStatus = CMD_FLAG_STATUS;
    PollBit = STATUS_REG_POLL_PEC;
    CheckStatus = STATUS_REG_POLL_PEC;
  }

  // Send command
  MvSpiFlashWriteEnableCmd (Slave);

  // Write data
  SpiMasterProtocol->ReadWrite (SpiMasterProtocol, Slave, Cmd, Length,
    Buffer, NULL, BufferLength);

  // Poll status register
  SpiMasterProtocol->Transfer (SpiMasterProtocol, Slave, 1, &CmdStatus,
    NULL, SPI_TRANSFER_BEGIN);
  do {
    SpiMasterProtocol->Transfer (SpiMasterProtocol, Slave, 1, NULL, &State,
      0);
    Counter--;
    if ((State & PollBit) == CheckStatus)
      break;
  } while (Counter > 0);
  if (Counter == 0) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Timeout while writing to spi flash\n"));
    return EFI_DEVICE_ERROR;
  }

  // Deactivate CS
  SpiMasterProtocol->Transfer (SpiMasterProtocol, Slave, 0, NULL, NULL, SPI_TRANSFER_END);

  return EFI_SUCCESS;
}

STATIC
VOID
SpiFlashCmdBankaddrWrite (
  IN SPI_DEVICE *Slave,
  IN UINT8 BankSel
  )
{
  UINT8 Cmd = CMD_BANK_WRITE;

  /* Update bank selection command for Spansion */
  if (Slave->Info->Id[0] == NOR_FLASH_ID_SPANSION) {
    Cmd = CMD_BANKADDR_BRWR;
  }

  MvSpiFlashWriteCommon (Slave, &Cmd, 1, &BankSel, 1);
}

STATIC
UINT8
SpiFlashBank (
  IN SPI_DEVICE *Slave,
  IN UINT32 Offset
  )
{
  UINT8 BankSel;

  BankSel = Offset / SPI_FLASH_16MB_BOUN;

  SpiFlashCmdBankaddrWrite (Slave, BankSel);

  return BankSel;
}

EFI_STATUS
MvSpiFlashErase (
  IN SPI_DEVICE *Slave,
  IN UINTN Offset,
  IN UINTN Length
  )
{
  EFI_STATUS Status;
  UINT32 EraseAddr;
  UINTN EraseSize;
  UINT8 Cmd[5];

  if (Slave->Info->Flags & NOR_FLASH_ERASE_4K) {
    Cmd[0] = CMD_ERASE_4K;
    EraseSize = SIZE_4KB;
  } else if (Slave->Info->Flags & NOR_FLASH_ERASE_32K) {
    Cmd[0] = CMD_ERASE_32K;
    EraseSize = SIZE_32KB;
  } else {
    Cmd[0] = CMD_ERASE_64K;
    EraseSize = Slave->Info->SectorSize;
  }

  // Check input parameters
  if (Offset % EraseSize || Length % EraseSize) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Either erase offset or length "
      "is not multiple of erase size\n"));
    return EFI_DEVICE_ERROR;
  }

  while (Length) {
    EraseAddr = Offset;

    SpiFlashBank (Slave, EraseAddr);

    SpiFlashFormatAddress (EraseAddr, Slave->AddrSize, Cmd);

    // Programm proper erase address
    Status = MvSpiFlashWriteCommon (Slave, Cmd, Slave->AddrSize + 1, NULL, 0);
      if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "SpiFlash: Error while programming target address\n"));
        return Status;
      }

    Offset += EraseSize;
    Length -= EraseSize;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
MvSpiFlashRead (
  IN SPI_DEVICE   *Slave,
  IN UINT32       Offset,
  IN UINTN        Length,
  IN VOID         *Buf
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8 Cmd[6];
  UINT32 ReadAddr, ReadLength, RemainLength;
  UINTN BankSel = 0;

  Cmd[0] = CMD_READ_ARRAY_FAST;

  // Sign end of address with 0 byte
  Cmd[5] = 0;

  while (Length) {
    ReadAddr = Offset;

    BankSel = SpiFlashBank (Slave, ReadAddr);

    RemainLength = (SPI_FLASH_16MB_BOUN * (BankSel + 1)) - Offset;
    if (Length < RemainLength) {
      ReadLength = Length;
    } else {
      ReadLength = RemainLength;
    }
    SpiFlashFormatAddress (ReadAddr, Slave->AddrSize, Cmd);
    // Program proper read address and read data
    Status = MvSpiFlashReadCmd (Slave, Cmd, Slave->AddrSize + 2, Buf, Length);

    Offset += ReadLength;
    Length -= ReadLength;
    Buf = (VOID*)((UINTN)Buf + ReadLength);
  }

  return Status;
}

EFI_STATUS
MvSpiFlashWrite (
  IN SPI_DEVICE *Slave,
  IN UINT32     Offset,
  IN UINTN      Length,
  IN VOID       *Buf
  )
{
  EFI_STATUS Status;
  UINTN ByteAddr, ChunkLength, ActualIndex, PageSize;
  UINT32 WriteAddr;
  UINT8 Cmd[5];

  PageSize = Slave->Info->PageSize;

  Cmd[0] = CMD_PAGE_PROGRAM;

  for (ActualIndex = 0; ActualIndex < Length; ActualIndex += ChunkLength) {
    WriteAddr = Offset;

    SpiFlashBank (Slave, WriteAddr);

    ByteAddr = Offset % PageSize;

    ChunkLength = MIN(Length - ActualIndex, (UINT64) (PageSize - ByteAddr));

    SpiFlashFormatAddress (WriteAddr, Slave->AddrSize, Cmd);

    // Program proper write address and write data
    Status = MvSpiFlashWriteCommon (
      Slave, Cmd, Slave->AddrSize + 1,
      (VOID*)((UINTN)Buf + ActualIndex), ChunkLength
      );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Error while programming write address\n"));
      return Status;
    }

    Offset += ChunkLength;
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvSpiFlashUpdateBlock (
  IN SPI_DEVICE *Slave,
  IN UINT32 Offset,
  IN UINTN ToUpdate,
  IN UINT8 *Buf,
  IN UINT8 *TmpBuf,
  IN UINTN EraseSize
  )
{
  EFI_STATUS Status;

  // Read backup
  Status = MvSpiFlashRead (Slave, Offset, EraseSize, TmpBuf);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Update: Error while reading old data\n"));
      return Status;
    }

  // Erase entire sector
  Status = MvSpiFlashErase (Slave, Offset, EraseSize);
  if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Update: Error while erasing block\n"));
      return Status;
    }

  // Write new data
  MvSpiFlashWrite (Slave, Offset, ToUpdate, Buf);
  if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Update: Error while writing new data\n"));
      return Status;
    }

  // Write backup
  if (ToUpdate != EraseSize) {
    Status = MvSpiFlashWrite (Slave, Offset + ToUpdate, EraseSize - ToUpdate,
      &TmpBuf[ToUpdate]);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Update: Error while writing backup\n"));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
MvSpiFlashUpdate (
  IN SPI_DEVICE *Slave,
  IN UINT32 Offset,
  IN UINTN ByteCount,
  IN UINT8 *Buf
  )
{
  EFI_STATUS Status;
  UINT64 SectorSize, ToUpdate, Scale = 1;
  UINT8 *TmpBuf, *End;

  SectorSize = Slave->Info->SectorSize;

  End = Buf + ByteCount;

  TmpBuf = (UINT8 *)AllocateZeroPool (SectorSize);
  if (TmpBuf == NULL) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Cannot allocate memory\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  if (End - Buf >= 200)
    Scale = (End - Buf) / 100;

  for (; Buf < End; Buf += ToUpdate, Offset += ToUpdate) {
    ToUpdate = MIN((UINT64)(End - Buf), SectorSize);
    Print (L"   \rUpdating, %d%%", 100 - (End - Buf) / Scale);
    Status = MvSpiFlashUpdateBlock (Slave, Offset, ToUpdate, Buf, TmpBuf, SectorSize);

    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Error while updating\n"));
      return Status;
    }
  }

  Print(L"\n");
  FreePool (TmpBuf);

  return EFI_SUCCESS;
}

EFI_STATUS
MvSpiFlashUpdateWithProgress (
  IN SPI_DEVICE                                    *Slave,
  IN UINT32                                         Offset,
  IN UINTN                                          ByteCount,
  IN UINT8                                         *Buffer,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_STATUS Status;
  UINTN SectorSize;
  UINTN SectorNum;
  UINTN ToUpdate;
  UINTN Index;
  UINT8 *TmpBuf;

  SectorSize = Slave->Info->SectorSize;
  SectorNum = (ByteCount / SectorSize) + 1;
  ToUpdate = SectorSize;

  TmpBuf = (UINT8 *)AllocateZeroPool (SectorSize);
  if (TmpBuf == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < SectorNum; Index++) {
    if (Progress != NULL) {
      Progress (StartPercentage +
                ((Index * (EndPercentage - StartPercentage)) / SectorNum));
    }

    // In the last chunk update only an actual number of remaining bytes.
    if (Index + 1 == SectorNum) {
      ToUpdate = ByteCount % SectorSize;
    }

    Status = MvSpiFlashUpdateBlock (Slave,
               Offset + Index * SectorSize,
               ToUpdate,
               Buffer + Index * SectorSize,
               TmpBuf,
               SectorSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Error while updating\n", __FUNCTION__));
      return Status;
    }
  }
  FreePool (TmpBuf);

  if (Progress != NULL) {
    Progress (EndPercentage);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MvSpiFlashReadId (
  IN     SPI_DEVICE *SpiDev,
  IN     BOOLEAN     UseInRuntime
  )
{
  EFI_STATUS Status;
  UINT8 Id[NOR_FLASH_MAX_ID_LEN];
  UINT8 Cmd;

  Cmd = CMD_READ_ID;
  Status = SpiMasterProtocol->ReadWrite (SpiMasterProtocol,
                                SpiDev,
                                &Cmd,
                                SPI_CMD_LEN,
                                NULL,
                                Id,
                                NOR_FLASH_MAX_ID_LEN);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ReadId: Spi transfer error\n"));
    return Status;
  }

  Status = NorFlashGetInfo (Id, &SpiDev->Info, UseInRuntime);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Unrecognized JEDEC Id bytes: 0x%02x%02x%02x\n",
      __FUNCTION__,
      Id[0],
      Id[1],
      Id[2]));
    return Status;
  }

  NorFlashPrintInfo (SpiDev->Info);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MvSpiFlashInit (
  IN MARVELL_SPI_FLASH_PROTOCOL *This,
  IN SPI_DEVICE *Slave
  )
{
  EFI_STATUS Status;
  UINT8 Cmd, StatusRegister;

  if (Slave->Info->Flags & NOR_FLASH_4B_ADDR) {
    Slave->AddrSize = 4;
  } else {
    Slave->AddrSize = 3;
  }

  if (Slave->AddrSize == 4) {
    // Set 4 byte address mode
    Status = MvSpiFlashWriteEnableCmd (Slave);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Error while setting write_enable\n"));
      return Status;
    }

    Cmd = CMD_4B_ADDR_ENABLE;
    Status = SpiMasterProtocol->Transfer (SpiMasterProtocol, Slave, 1, &Cmd, NULL,
      SPI_TRANSFER_BEGIN | SPI_TRANSFER_END);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "SpiFlash: Error while setting 4B address\n"));
      return Status;
    }
  }

  // Write flash status register
  Status = MvSpiFlashWriteEnableCmd (Slave);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Error while setting write_enable\n"));
    return Status;
  }

  Cmd = CMD_WRITE_STATUS_REG;
  StatusRegister = 0x0;
  Status = SpiMasterProtocol->ReadWrite (SpiMasterProtocol, Slave, &Cmd, 1,
    &StatusRegister, NULL, 1);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Error with spi transfer\n"));
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
MvSpiFlashInitProtocol (
  IN MARVELL_SPI_FLASH_PROTOCOL *SpiFlashProtocol
  )
{

  SpiFlashProtocol->Init = MvSpiFlashInit;
  SpiFlashProtocol->ReadId = MvSpiFlashReadId;
  SpiFlashProtocol->Read = MvSpiFlashRead;
  SpiFlashProtocol->Write = MvSpiFlashWrite;
  SpiFlashProtocol->Erase = MvSpiFlashErase;
  SpiFlashProtocol->Update = MvSpiFlashUpdate;
  SpiFlashProtocol->UpdateWithProgress = MvSpiFlashUpdateWithProgress;

  return EFI_SUCCESS;
}

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context
**/
STATIC
VOID
EFIAPI
MvSpiFlashVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  //
  // Convert SpiMasterProtocol callbacks in MvSpiFlashErase and
  // MvSpiFlashWrite required by runtime variable support.
  //
  EfiConvertPointer (0x0, (VOID**)&SpiMasterProtocol->ReadWrite);
  EfiConvertPointer (0x0, (VOID**)&SpiMasterProtocol->Transfer);
  EfiConvertPointer (0x0, (VOID**)&SpiMasterProtocol);

  return;
}

EFI_STATUS
EFIAPI
MvSpiFlashEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (
    &gMarvellSpiMasterProtocolGuid,
    NULL,
    (VOID **)&SpiMasterProtocol
  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Cannot locate SPI Master protocol\n"));
    return EFI_DEVICE_ERROR;
  }

  mSpiFlashInstance = AllocateRuntimeZeroPool (sizeof (SPI_FLASH_INSTANCE));
  if (mSpiFlashInstance == NULL) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Cannot allocate memory\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  MvSpiFlashInitProtocol (&mSpiFlashInstance->SpiFlashProtocol);

  mSpiFlashInstance->Signature = SPI_FLASH_SIGNATURE;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &(mSpiFlashInstance->Handle),
                  &gMarvellSpiFlashProtocolGuid,
                  &(mSpiFlashInstance->SpiFlashProtocol),
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Cannot install SPI flash protocol\n"));
    goto ErrorInstallProto;
  }

  //
  // Register for the virtual address change event
  //
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  MvSpiFlashVirtualNotifyEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mMvSpiFlashVirtualAddrChangeEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to register VA change event\n", __FUNCTION__));
    goto ErrorCreateEvent;
  }

  return EFI_SUCCESS;

ErrorCreateEvent:
  gBS->UninstallMultipleProtocolInterfaces (&mSpiFlashInstance->Handle,
    &gMarvellSpiFlashProtocolGuid,
    NULL);

ErrorInstallProto:
  FreePool (mSpiFlashInstance);

  return EFI_SUCCESS;
}
