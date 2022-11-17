/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#include "MvSpiOrionDxe.h"

SPI_MASTER *mSpiMasterInstance;

STATIC
EFI_STATUS
SpiSetBaudRate (
  IN SPI_DEVICE *Slave,
  IN UINT32 CpuClock,
  IN UINT32 MaxFreq
  )
{
  UINT32 Spr, BestSpr, Sppr, BestSppr, ClockDivider, Match, Reg, MinBaudDiff;
  UINTN SpiRegBase = Slave->HostRegisterBaseAddress;

  MinBaudDiff = 0xFFFFFFFF;
  BestSppr = 0;

  //Spr is in range 1-15 and Sppr in range 0-8
  for (Spr = 1; Spr <= 15; Spr++) {
    for (Sppr = 0; Sppr <= 7; Sppr++) {
      ClockDivider = Spr * (1 << Sppr);

      if ((CpuClock / ClockDivider) > MaxFreq) {
        continue;
      }

      if ((CpuClock / ClockDivider) == MaxFreq) {
        BestSpr = Spr;
        BestSppr = Sppr;
        Match = 1;
        break;
        }

      if ((MaxFreq - (CpuClock / ClockDivider)) < MinBaudDiff) {
        MinBaudDiff = (MaxFreq - (CpuClock / ClockDivider));
        BestSpr = Spr;
        BestSppr = Sppr;
      }
    }

    if (Match == 1) {
      break;
    }
  }

  if (BestSpr == 0) {
    return (EFI_INVALID_PARAMETER);
  }

  Reg = MmioRead32 (SpiRegBase + SPI_CONF_REG);
  Reg &= ~(SPI_SPR_MASK | SPI_SPPR_0_MASK | SPI_SPPR_HI_MASK);
  Reg |= (BestSpr << SPI_SPR_OFFSET) |
         ((BestSppr & 0x1) << SPI_SPPR_0_OFFSET) |
         ((BestSppr >> 1) << SPI_SPPR_HI_OFFSET);
  MmioWrite32 (SpiRegBase + SPI_CONF_REG, Reg);

  return EFI_SUCCESS;
}

STATIC
VOID
SpiSetCs (
  IN SPI_DEVICE *Slave
  )
{
  UINT32 Reg;
  UINTN  SpiRegBase = Slave->HostRegisterBaseAddress;

  Reg = MmioRead32 (SpiRegBase + SPI_CTRL_REG);
  Reg &= ~SPI_CS_NUM_MASK;
  Reg |= (Slave->Cs << SPI_CS_NUM_OFFSET);
  MmioWrite32 (SpiRegBase + SPI_CTRL_REG, Reg);
}

STATIC
VOID
SpiActivateCs (
  IN SPI_DEVICE *Slave
  )
{
  UINT32 Reg;
  UINTN  SpiRegBase = Slave->HostRegisterBaseAddress;

  SpiSetCs(Slave);
  Reg = MmioRead32 (SpiRegBase + SPI_CTRL_REG);
  Reg |= SPI_CS_EN_MASK;
  MmioWrite32(SpiRegBase + SPI_CTRL_REG, Reg);
}

STATIC
VOID
SpiDeactivateCs (
  IN SPI_DEVICE *Slave
  )
{
  UINT32 Reg;
  UINTN  SpiRegBase = Slave->HostRegisterBaseAddress;

  Reg = MmioRead32 (SpiRegBase + SPI_CTRL_REG);
  Reg &= ~SPI_CS_EN_MASK;
  MmioWrite32(SpiRegBase + SPI_CTRL_REG, Reg);
}

STATIC
VOID
SpiSetupTransfer (
  IN MARVELL_SPI_MASTER_PROTOCOL *This,
  IN SPI_DEVICE *Slave
  )
{
  SPI_MASTER *SpiMaster;
  UINT32 Reg, CoreClock, SpiMaxFreq;
  UINTN SpiRegBase;

  SpiMaster = SPI_MASTER_FROM_SPI_MASTER_PROTOCOL (This);

  // Initialize values from PCDs
  SpiRegBase  = Slave->HostRegisterBaseAddress;
  CoreClock   = Slave->CoreClock;
  SpiMaxFreq  = Slave->MaxFreq;

  EfiAcquireLock (&SpiMaster->Lock);

  Reg = MmioRead32 (SpiRegBase + SPI_CONF_REG);
  Reg |= SPI_BYTE_LENGTH;
  MmioWrite32 (SpiRegBase + SPI_CONF_REG, Reg);

  SpiSetCs(Slave);

  SpiSetBaudRate (Slave, CoreClock, SpiMaxFreq);

  Reg = MmioRead32 (SpiRegBase + SPI_CONF_REG);
  Reg &= ~(SPI_CPOL_MASK | SPI_CPHA_MASK | SPI_TXLSBF_MASK | SPI_RXLSBF_MASK);

  switch (Slave->Mode) {
  case SPI_MODE0:
    break;
  case SPI_MODE1:
    Reg |= SPI_CPHA_MASK;
    break;
  case SPI_MODE2:
    Reg |= SPI_CPOL_MASK;
    break;
  case SPI_MODE3:
    Reg |= SPI_CPOL_MASK;
    Reg |= SPI_CPHA_MASK;
    break;
  }

  MmioWrite32 (SpiRegBase + SPI_CONF_REG, Reg);

  EfiReleaseLock (&SpiMaster->Lock);
}

EFI_STATUS
EFIAPI
MvSpiTransfer (
  IN MARVELL_SPI_MASTER_PROTOCOL *This,
  IN SPI_DEVICE *Slave,
  IN UINTN DataByteCount,
  IN VOID *DataOut,
  IN VOID *DataIn,
  IN UINTN Flag
  )
{
  SPI_MASTER *SpiMaster;
  UINT64  Length;
  UINT32  Iterator, Reg;
  UINT8   *DataOutPtr = (UINT8 *)DataOut;
  UINT8   *DataInPtr  = (UINT8 *)DataIn;
  UINT8   DataToSend  = 0;
  UINTN   SpiRegBase;

  SpiMaster = SPI_MASTER_FROM_SPI_MASTER_PROTOCOL (This);

  SpiRegBase = Slave->HostRegisterBaseAddress;

  Length = 8 * DataByteCount;

  if (!EfiAtRuntime ()) {
    EfiAcquireLock (&SpiMaster->Lock);
  }

  if (Flag & SPI_TRANSFER_BEGIN) {
    SpiActivateCs (Slave);
  }

  // Set 8-bit mode
  Reg = MmioRead32 (SpiRegBase + SPI_CONF_REG);
  Reg &= ~SPI_BYTE_LENGTH;
  MmioWrite32 (SpiRegBase + SPI_CONF_REG, Reg);

  while (Length > 0) {
    if (DataOut != NULL) {
      DataToSend = *DataOutPtr & 0xFF;
    }
    // Transmit Data
    MmioWrite32 (SpiRegBase + SPI_INT_CAUSE_REG, 0x0);
    MmioWrite32 (SpiRegBase + SPI_DATA_OUT_REG, DataToSend);
    // Wait for memory ready
    for (Iterator = 0; Iterator < SPI_TIMEOUT; Iterator++) {
      if (MmioRead32 (SpiRegBase + SPI_INT_CAUSE_REG)) {
        if (DataInPtr != NULL) {
          *DataInPtr = MmioRead32 (SpiRegBase + SPI_DATA_IN_REG);
          DataInPtr++;
        }
        if (DataOutPtr != NULL) {
          DataOutPtr++;
        }
        Length -= 8;
        break;
      }
    }

    if (Iterator >= SPI_TIMEOUT) {
      DEBUG ((DEBUG_ERROR, "%a: Timeout\n", __FUNCTION__));
      return EFI_TIMEOUT;
    }
  }

  if (Flag & SPI_TRANSFER_END) {
    SpiDeactivateCs (Slave);
  }

  if (!EfiAtRuntime ()) {
    EfiReleaseLock (&SpiMaster->Lock);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MvSpiReadWrite (
  IN  MARVELL_SPI_MASTER_PROTOCOL *This,
  IN  SPI_DEVICE *Slave,
  IN  UINT8 *Cmd,
  IN  UINTN CmdSize,
  IN  UINT8 *DataOut,
  OUT UINT8 *DataIn,
  IN  UINTN DataSize
  )
{
  EFI_STATUS Status;

  Status = MvSpiTransfer (This, Slave, CmdSize, Cmd, NULL, SPI_TRANSFER_BEGIN);
  if (EFI_ERROR (Status)) {
    Print (L"Spi Transfer Error\n");
    return EFI_DEVICE_ERROR;
  }

  Status = MvSpiTransfer (This, Slave, DataSize, DataOut, DataIn, SPI_TRANSFER_END);
  if (EFI_ERROR (Status)) {
    Print (L"Spi Transfer Error\n");
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MvSpiInit (
  IN MARVELL_SPI_MASTER_PROTOCOL * This
  )
{

  return EFI_SUCCESS;
}

SPI_DEVICE *
EFIAPI
MvSpiSetupSlave (
  IN MARVELL_SPI_MASTER_PROTOCOL *This,
  IN SPI_DEVICE *Slave,
  IN UINTN Cs,
  IN SPI_MODE Mode
  )
{
  if (!Slave) {
    Slave = AllocateZeroPool (sizeof(SPI_DEVICE));
    if (Slave == NULL) {
      DEBUG((DEBUG_ERROR, "Cannot allocate memory\n"));
      return NULL;
    }

    Slave->Cs   = Cs;
    Slave->Mode = Mode;
  }

  Slave->HostRegisterBaseAddress = PcdGet32 (PcdSpiRegBase);
  Slave->CoreClock = PcdGet32 (PcdSpiClockFrequency);
  Slave->MaxFreq = PcdGet32 (PcdSpiMaxFrequency);

  SpiSetupTransfer (This, Slave);

  return Slave;
}

EFI_STATUS
EFIAPI
MvSpiFreeSlave (
  IN SPI_DEVICE *Slave
  )
{
  FreePool (Slave);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MvSpiConfigRuntime (
  IN SPI_DEVICE *Slave
  )
{
  EFI_STATUS Status;
  UINTN AlignedAddress;

  //
  // Host register base may be not aligned to the page size,
  // which is not accepted when setting memory space attributes.
  // Add one aligned page of memory space which covers the host
  // controller registers.
  //
  AlignedAddress = Slave->HostRegisterBaseAddress & ~(SIZE_4KB - 1);

  Status = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                  AlignedAddress,
                  SIZE_4KB,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to add memory space\n", __FUNCTION__));
    return Status;
  }

  Status = gDS->SetMemorySpaceAttributes (AlignedAddress,
                  SIZE_4KB,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to set memory attributes\n", __FUNCTION__));
    gDS->RemoveMemorySpace (AlignedAddress, SIZE_4KB);
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
SpiMasterInitProtocol (
  IN MARVELL_SPI_MASTER_PROTOCOL *SpiMasterProtocol
  )
{

  SpiMasterProtocol->Init        = MvSpiInit;
  SpiMasterProtocol->SetupDevice = MvSpiSetupSlave;
  SpiMasterProtocol->FreeDevice  = MvSpiFreeSlave;
  SpiMasterProtocol->Transfer    = MvSpiTransfer;
  SpiMasterProtocol->ReadWrite   = MvSpiReadWrite;
  SpiMasterProtocol->ConfigRuntime = MvSpiConfigRuntime;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MvSpiOrionEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;

  mSpiMasterInstance = AllocateRuntimeZeroPool (sizeof (SPI_MASTER));
  if (mSpiMasterInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  EfiInitializeLock (&mSpiMasterInstance->Lock, TPL_NOTIFY);

  SpiMasterInitProtocol (&mSpiMasterInstance->SpiMasterProtocol);

  mSpiMasterInstance->Signature = SPI_MASTER_SIGNATURE;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &(mSpiMasterInstance->Handle),
                  &gMarvellSpiMasterProtocolGuid,
                  &(mSpiMasterInstance->SpiMasterProtocol),
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    FreePool (mSpiMasterInstance);
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}
