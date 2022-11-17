/** @file
 Implement the I2C controller driver.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Base.h>
#include <IndustryStandard/Pci.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiLib.h>
#include <PchAccess.h>
#include <Library/PchSerialIoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/I2cMaster.h>
#include <Pi/PiI2c.h>
#include <Library/DevicePathLib.h>
#include <Library/I2cMasterCommonLib.h>

/**
  Prepare I2c controller for use: enable its mmio range, put in D0, get out of reset

  @param[in] Context - driver context
**/
VOID
PrepareController (
  I2C_MASTER_CONTEXT  *Context
  )
{
  MmioOr32 (Context->ConfigAddress + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  MmioAnd32 (Context->ConfigAddress + R_PCH_SERIAL_IO_PME_CTRL_STS, (UINT32) (~B_PCH_SERIAL_IO_PME_CTRL_STS_PWR_ST));
  MmioOr32 (Context->MmioAddress + R_PCH_SERIAL_IO_PPR_RESETS,
    B_PCH_SERIAL_IO_PPR_RESETS_FUNC | B_PCH_SERIAL_IO_PPR_RESETS_APB | B_PCH_SERIAL_IO_PPR_RESETS_IDMA);
}

/**
  Determine the state of the I2C controller

  @param[in] Context - driver context

  @retval TRUE              The I2C controller is active
  @retval FALSE             The I2C controller is idle
**/
BOOLEAN
IsHardwareActive (
  I2C_MASTER_CONTEXT  *Context
  )
{
  return (MmioRead32 (Context->MmioAddress + R_IC_STATUS) & B_IC_STATUS_ACTIVITY );
}

/**
  Updates WriteOperation and WritePosition, two variables that determine
  which part of Request is being committed to I2C bus.
  This iterates over both Read and Write operations from a request, because
  things that need to be written to WriteFifo are both I2c bus writes
  and I2c bus reads (the command to perform bus read needs to be put into Write Fifo)

  @param[in] Context - driver context
**/
VOID
UpdateWritePosition (
  I2C_MASTER_CONTEXT *Context
  )
{
  if ( Context->WritePos == Context->Request->Operation[Context->WriteOp].LengthInBytes - 1) {
    Context->WritePos = 0;
    Context->WriteOp ++;
  } else {
    Context->WritePos ++;
  }
}

/*
  FindReadOp checks if current Operation is of Read type. If so, returns.
  If not, increases ReadOp until it finds one or goes beyond Request's OperationCount

  @param[in] Context - driver context
*/
VOID
FindReadOp (
  I2C_MASTER_CONTEXT *Context
  )
{
  while (Context->ReadOp < Context->Request->OperationCount &&
         !(Context->Request->Operation[Context->ReadOp].Flags & I2C_FLAG_READ)) {
    Context->ReadOp++;
  }
}

/**
  Updates ReadOperation and ReadPosition, two variables that determine
  which part of Request is being filled with data incoming from I2C reads.
  This iterates only over Read operations from a request.

  @param[in] Context - driver context
**/
VOID
UpdateReadPosition (
  I2C_MASTER_CONTEXT *Context
  )
{
  if ( Context->ReadPos == Context->Request->Operation[Context->ReadOp].LengthInBytes - 1) {
    Context->ReadPos = 0;
    Context->ReadOp ++;
    FindReadOp (Context);
  } else {
    Context->ReadPos ++;
  }
}

/**
  ValidateRequest checks if Request is valid and can be started

  @param[in] Context - driver context
  @param[in] RequestPacket

  @retval EFI_SUCCESS           Request is valid and can be started
  @retval EFI_ALREADY_STARTED   The controller is busy with another transfer
  @retval EFI_BAD_BUFFER_SIZE   Transfer size too big
  @retval EFI_INVALID_PARAMETER RequestPacket is NULL, invalid Operation flags
  @retval EFI_UNSUPPORTED       "ping" operation attempted (0-byte transfer, address byte not followed by any data)
**/
EFI_STATUS
ValidateRequest (
  I2C_MASTER_CONTEXT           *Context,
  CONST EFI_I2C_REQUEST_PACKET *RequestPacket
  )
{
  UINTN TotalSize;
  UINTN Operation;
  UINTN OperationSize;

  if (Context->TransferInProgress) {
    return EFI_ALREADY_STARTED;
  }
  if (RequestPacket == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (RequestPacket->OperationCount == 0) {
    // 0-size operations ("pings") not supported
    return EFI_UNSUPPORTED;
  }

  TotalSize = 0;

  for (Operation=0; Operation < RequestPacket->OperationCount; Operation++) {
    // unsupported flags?
    if ((RequestPacket->Operation[Operation].Flags & (~I2C_FLAG_READ)) != 0) {
      return EFI_INVALID_PARAMETER;
    }
    OperationSize = RequestPacket->Operation[Operation].LengthInBytes;
    // 0-size operations ("pings") not supported
    if (OperationSize == 0) {
      return EFI_UNSUPPORTED;
    }
    TotalSize += OperationSize;
    // read operation too big?
    if (RequestPacket->Operation[Operation].Flags & I2C_FLAG_READ) {
      if (OperationSize > Context->Capabilities.MaximumReceiveBytes) {
        return EFI_BAD_BUFFER_SIZE;
      }
      // write operation too big?
    } else {
      if (OperationSize > Context->Capabilities.MaximumTransmitBytes) {
        return EFI_BAD_BUFFER_SIZE;
      }
    }
  }
  // total request too big?
  if (TotalSize > Context->Capabilities.MaximumTotalBytes) {
    return EFI_BAD_BUFFER_SIZE;
  }

  return EFI_SUCCESS;
}

/**
  IsLastFromRequest checks if WritePos and WriteOp point to the last byte of the request

  @param[in] Context - driver context

  @retval Boolean
**/
BOOLEAN
IsLastFromRequest (
  I2C_MASTER_CONTEXT *Context
  )
{
  if ((Context->WriteOp == Context->Request->OperationCount - 1 ) &&
      (Context->WritePos == Context->Request->Operation[Context->WriteOp].LengthInBytes - 1)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  IsFirstFromOperation checks if WritePos and WriteOp point to the first byte of an operation

  @param[in] Context - driver context

  @retval Boolean
**/
BOOLEAN
IsFirstFromOperation (
  I2C_MASTER_CONTEXT *Context
  )
{
  if ((Context->WriteOp != 0) && (Context->WritePos == 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  InitializeTransfer checks if HW is ready to accept new transfer.
  If so, sets up slave address

  @param[in] Context - driver context

  @retval Status
**/
EFI_STATUS
InitializeTransfer (
  I2C_MASTER_CONTEXT           *Context,
  UINTN                        SlaveAddress,
  CONST EFI_I2C_REQUEST_PACKET *RequestPacket
  )
{
  UINT32 Attempts = 10000;
  UINT32 Address;

  Context->Request = (EFI_I2C_REQUEST_PACKET*) RequestPacket;
  Context->TransferStatus = EFI_SUCCESS;
  Context->WriteOp = 0;
  Context->WritePos = 0;
  Context->ReadOp = 0;
  FindReadOp (Context);
  Context->ReadPos = 0;

  if (MmioRead32 (Context->MmioAddress + R_IC_ENABLE) != 0) {
    DEBUG (( DEBUG_ERROR, "Address change was attempted while a transfer was underway!\n"));
    return EFI_DEVICE_ERROR;
  }

  Address = SlaveAddress & 0xFF;
  if (SlaveAddress & I2C_ADDRESSING_10_BIT
      ) {
    Address |= B_IC_TAR_10BITADDR_MASTER;
  }
  MmioWrite32 (Context->MmioAddress + R_IC_TAR, Address);
  MmioWrite32 (Context->MmioAddress + R_IC_ENABLE, B_IC_EN );
  //clear errors
  MmioRead32 (Context->MmioAddress + R_IC_CLR_TX_ABRT);
  MmioRead32 (Context->MmioAddress + R_IC_CLR_INTR);

  while ( !(MmioRead32 (Context->MmioAddress + R_IC_ENABLE_STATUS ) & B_IC_EN)) {
    //should never happen, but just to make sure BIOS doesn't hang in infinite loop...
    MicroSecondDelay (1);
    if (--Attempts == 0) {
      MmioWrite32 (Context->MmioAddress + R_IC_ENABLE, 0 );
      return EFI_DEVICE_ERROR;
    }
  }
  Context->TransferInProgress = TRUE;
  return EFI_SUCCESS;
}

/**
  WriteFifo writes to I2c controller's transmit Fifo. Data written to Fifo could be
  - data bytes to be written to an i2c slave
  - read requests that trigger i2c bus reads
  First transfer from each operation adds Restart bit which triggers Restart condition on bus
  Last transfer from the whole Request adds Stop bit which triggers Stop condtion on bus
  Driver keeps track of which parts of Request were already committed to hardware using
  pointer consisting of WritePosition and WriteOperation variables. This pointer is updated
  every time data byte/read request is committed to FIFO
  WriteFifo executes while there's anything more to write and the write fifo isn't full

  @param[in] Context - driver context
**/
VOID
WriteFifo (
  I2C_MASTER_CONTEXT *Context
  )
{
  UINT32 Data;
  while ( MmioRead32 (Context->MmioAddress + R_IC_STATUS ) & B_IC_STATUS_TFNF) {
    if (Context->WriteOp >= Context->Request->OperationCount ) {
      return; // request complete, nothing more to write
    }

    if (Context->Request->Operation[Context->WriteOp].Flags & I2C_FLAG_READ) {
      Data = B_IC_CMD_READ;
    } else {
      Data = Context->Request->Operation[Context->WriteOp].Buffer[Context->WritePos];
    }
    if (IsLastFromRequest (Context)) {
      Data |= B_IC_CMD_STOP;
    }
    if (IsFirstFromOperation (Context)) {
      Data |= B_IC_CMD_RESTART;
    }
    MmioWrite32 (Context->MmioAddress + R_IC_DATA_CMD, Data);
    UpdateWritePosition (Context);
  }
}

/**
  ReadFifo reads from I2c controller's receive Fifo. It contains data retrieved
  from slave device as a result of executing read transfers, which were
  triggered by putting read requests into Write Fifo. Retrieved data is copied into buffers
  pointed to by Request structure.
  Driver keeps track where to copy incoming data using pointer consisting of
  ReadPosition and ReadOperation variables. This pointer is updated
  every time data was retrieved from hardware
  ReadFifo executes while there's data available and receive buffers were not filled

  @param[in] Context - driver context
**/
VOID
ReadFifo (
  I2C_MASTER_CONTEXT *Context
  )
{
  while ( MmioRead32 ( Context->MmioAddress + R_IC_STATUS ) & B_IC_STATUS_RFNE) {
    if ( Context->ReadOp >= Context->Request->OperationCount ) {
      return;
    }
    Context->Request->Operation[Context->ReadOp].Buffer[Context->ReadPos] = (0xFF & MmioRead32 (Context->MmioAddress + R_IC_DATA_CMD));
    UpdateReadPosition (Context);
  }
}

/**
  CheckErrors checks if there were any transfer errors.

  @param[in] Context - driver context
**/
VOID
CheckErrors (
  I2C_MASTER_CONTEXT *Context
  )
{
  if (!(MmioRead32 (Context->MmioAddress + R_IC_INTR_STAT) & B_IC_INTR_TX_ABRT)) {
    return;
  }
  if (MmioRead32 (Context->MmioAddress + R_IC_TX_ABRT_SOURCE) & B_IC_TX_ABRT_7B_ADDR_NACK) {
    Context->TransferStatus = EFI_NO_RESPONSE;
  } else {
    Context->TransferStatus = EFI_DEVICE_ERROR;
  }
  DEBUG (( DEBUG_INFO, "I2c CheckErrors: %08x\n", MmioRead32 (Context->MmioAddress + R_IC_TX_ABRT_SOURCE)));
}

/**
  Transfer is finished when all requested operations were placed in fifo,
  all read requests were filled and hardware is inactive
  The last part is necessary for write-only transfers where after
  placing all writes in fifo sw needs to wait until they flush down the bus

  @param[in] Context - driver context

  @retval Boolean
**/
BOOLEAN
IsTransferFinished (
  I2C_MASTER_CONTEXT *Context
  )
{
  if (( Context->WriteOp >= Context->Request->OperationCount ) &&
      ( Context->ReadOp >= Context->Request->OperationCount ) &&
      !IsHardwareActive (Context)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/*
  Clean up Hw activity and errors
  Return status to Request's submitter and signal the event that tells
  it that the request is complete
  Clear up Sw context to allow new request to start

  @param[in] Context - driver context
*/
VOID
FinishTransfer (
  I2C_MASTER_CONTEXT *Context
  )
{
  UINT32 Attempts = 10000;

  MmioWrite32 ( Context->MmioAddress + R_IC_ENABLE, 0 );

  while ( MmioRead32 ( Context->MmioAddress + R_IC_ENABLE_STATUS ) & B_IC_EN ) {
    //try for 1ms; should take no more than 100us even at slowest bus speed, but just to make sure BIOS doesn't hang in infinite loop
    MicroSecondDelay ( 1 );
    if (--Attempts == 0) {
      ASSERT (FALSE);
      break;
    }
  }

  //
  //clear errors
  //
  MmioRead32 (Context->MmioAddress + R_IC_CLR_TX_ABRT);
  MmioRead32 (Context->MmioAddress + R_IC_CLR_INTR);

  //
  //clean up context data
  //
  Context->TransferInProgress = FALSE;
}

/**
  PerformTransfer. For synchronous transfer this function is called in a loop
  and for asynchronous transfers, as a timer callback. It writes data and/or
  read requests to hadrware, copies read data to destination buffers. When
  transfer completes, it cleans up Sw context and Hw registers in preparation
  for new transfer

  @param[in] Context - driver context
**/
VOID
PerformTransfer (
  IN I2C_MASTER_CONTEXT *Context
  )
{
  if (!Context->TransferInProgress) {
    return;
  }
  if (EFI_ERROR (Context->TransferStatus) || IsTransferFinished (Context)) {
    FinishTransfer (Context);
    return;
  }
  WriteFifo (Context);
  ReadFifo (Context);
  CheckErrors (Context);
}


/**
  Set the I2C controller bus clock frequency.

  This routine must be called at or below TPL_NOTIFY.

  The software and controller do a best case effort of using the specified
  frequency for the I2C bus.  If the frequency does not match exactly then
  the controller will use lower frequency for the I2C to avoid exceeding
  the operating conditions for any of the I2C devices on the bus.
  For example if 400 KHz was specified and the controller's divide network
  only supports 402 KHz or 398 KHz then the controller would be set to 398
  KHz.

  @param[in] MmioAddress    Address of I2C controller
  @param[in] BusClockHertz  New I2C bus clock frequency in Hertz

  @retval EFI_SUCCESS       The bus frequency was set successfully.
  @retval EFI_UNSUPPORTED   The controller does not support this frequency.
**/

EFI_STATUS
FrequencySet (
  IN UINTN     MmioAddress,
  IN OUT UINTN *BusClockHertz
  )
{
  UINT32             Speed;

  if ( *BusClockHertz < 100*1000) {
    //can't run that slowly
    return EFI_UNSUPPORTED;
  } else if ( *BusClockHertz < 400*1000) {
    //for any request in range [100kHz, 400kHz), set 100kHz
    *BusClockHertz = 100000;
    Speed = V_IC_SPEED_STANDARD;
    MmioWrite32 ( MmioAddress + R_IC_SDA_HOLD, 0x001C001C);
    MmioWrite32 ( MmioAddress + R_IC_SS_SCL_HCNT, 528);
    MmioWrite32 ( MmioAddress + R_IC_SS_SCL_LCNT, 640);
  } else if ( *BusClockHertz < 1000*1000) {
    //for any request in range [400kHz, 1MHz), set 400kHz
    *BusClockHertz = 400000;
    Speed = V_IC_SPEED_FAST;
    MmioWrite32 ( MmioAddress + R_IC_SDA_HOLD, 0x001C001C);
    MmioWrite32 ( MmioAddress + R_IC_FS_SCL_HCNT, 128);
    MmioWrite32 ( MmioAddress + R_IC_FS_SCL_LCNT, 160);
  } else {
    //for any request in range [1MHz, +inf), set 1MHz. This silicon doesn't support 3.4MHz mode.
    *BusClockHertz = 1000000;
    Speed = V_IC_SPEED_FAST;
    MmioWrite32 ( MmioAddress + R_IC_SDA_HOLD, 0x00280028);
    MmioWrite32 ( MmioAddress + R_IC_FS_SCL_HCNT, 30);
    MmioWrite32 ( MmioAddress + R_IC_FS_SCL_LCNT, 80);
  }

  MmioWrite32 ( MmioAddress + R_IC_CON, Speed | B_IC_RESTART_EN | B_IC_SLAVE_DISABLE | B_IC_MASTER_MODE);

  return EFI_SUCCESS;
}


/**
  Reset the I2C controller

  @param[in] MmioAddress    Address of I2C controller

  @retval Status
**/
EFI_STATUS
I2cReset (
  IN UINTN     MmioAddress
  )
{
  MmioAnd32 (
    MmioAddress + R_PCH_SERIAL_IO_PPR_RESETS,
    (UINT32) ~(B_PCH_SERIAL_IO_PPR_RESETS_FUNC | B_PCH_SERIAL_IO_PPR_RESETS_APB | B_PCH_SERIAL_IO_PPR_RESETS_IDMA)
    );
  MmioRead32 (MmioAddress + R_PCH_SERIAL_IO_PPR_RESETS);
  MmioOr32 (
    MmioAddress + R_PCH_SERIAL_IO_PPR_RESETS,
    B_PCH_SERIAL_IO_PPR_RESETS_FUNC | B_PCH_SERIAL_IO_PPR_RESETS_APB | B_PCH_SERIAL_IO_PPR_RESETS_IDMA
    );
  MmioRead32 (MmioAddress + R_PCH_SERIAL_IO_PPR_RESETS);

  return EFI_SUCCESS;
}


