/** @file
 Implement the I2C port control.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _I2C_MASTER_COMMON_LIB_H_
#define _I2C_MASTER_COMMON_LIB_H_

///
/// Each I2C port instance uses an I2C_MASTER_CONTEXT structure
/// to maintain its context.
///
typedef struct {
  EFI_I2C_CONTROLLER_CAPABILITIES Capabilities;
  //
  // I2C master's mmio addresses cached to speed up operation
  //
  UINTN                           MmioAddress;
  UINTN                           ConfigAddress;
  //
  // copy of all pointers and data provided in StartRequest call
  // if transfer didn't finish in one go, those will be needed to continue it
  //
  EFI_I2C_REQUEST_PACKET          *Request;
  //
  // Internal copy of Transfer status, to be returned from StartRequest()
  //
  EFI_STATUS                      TransferStatus;
  //
  // Index (Operation:Postition in Buffer) of next operation to be performed
  // Write is for both R/W operations as both need to be put in fifo
  // Read is for Read only, for filling buffer with data retrieved from bus
  //
  UINTN                           WriteOp;
  UINTN                           WritePos;
  UINTN                           ReadOp;
  UINTN                           ReadPos;
  BOOLEAN                         TransferInProgress;
} I2C_MASTER_CONTEXT;

/**
  Prepare I2c controller for use: enable its mmio range, put in D0, get out of reset
  Verifies I2C Line SDA and SCL states

  @param[in] Context - driver context

  @retval EFI_SUCCESS         Controller prepared
  @retval EFI_DEVICE_ERROR    SCL and/or SDA lines are not pulled up
**/
EFI_STATUS
PrepareController (
  I2C_MASTER_CONTEXT  *Context
  );

/**
  Determine the state of the I2C controller

  @param[in] Context - driver context

  @retval TRUE              The I2C controller is active
  @retval FALSE             The I2C controller is idle
**/
BOOLEAN
IsHardwareActive (
  I2C_MASTER_CONTEXT  *Context
  );

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
  );

/**
  FindReadOp checks if current Operation is of Read type. If so, returns.
  If not, increases ReadOp until it finds one or goes beyond Request's OperationCount

  @param[in] Context - driver context
**/
VOID
FindReadOp (
  I2C_MASTER_CONTEXT *Context
  );

/**
  Updates ReadOperation and ReadPosition, two variables that determine
  which part of Request is being filled with data incoming from I2C reads.
  This iterates only over Read operations from a request.

  @param[in] Context - driver context
**/
VOID
UpdateReadPosition (
  I2C_MASTER_CONTEXT *Context
  );

/**
  ValidateRequest checks if Request is valid and can be started

  @param[in] Context            driver context
  @param[in] RequestPacket      content of I2C request package

  @retval EFI_SUCCESS           Request is valid and can be started
  @retval EFI_ALREADY_STARTED   The controller is busy with another transfer
  @retval EFI_BAD_BUFFER_SIZE   Transfer size too big
  @retval EFI_INVALID_PARAMETER RequestPacket is NULL, invalid Operation flags
  @retval EFI_UNSUPPORTED       10bit I2C address or "ping" operation attempted (0-byte transfer, address byte not followed by any data)
**/
EFI_STATUS
ValidateRequest (
  I2C_MASTER_CONTEXT           *Context,
  CONST EFI_I2C_REQUEST_PACKET *RequestPacket
  );

/**
  IsLastFromRequest checks if WritePos and WriteOp point to the last byte of the request

  @param[in] Context - driver context
**/
BOOLEAN
IsLastFromRequest (
  I2C_MASTER_CONTEXT *Context
  );

/**
  IsLastFromRequest checks if WritePos and WriteOp point to the first byte of an operation

  @param[in] Context - driver context

  @retval Boolean
**/
BOOLEAN
IsFirstFromOperation (
  I2C_MASTER_CONTEXT *Context
  );

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
  );

/**
  WriteFifo writes to I2c controller's transmit Fifo. Data written to Fifo could be
  - data bytes to be written to an I2C slave
  - read requests that trigger I2C bus reads
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
  );

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
  );

/**
  CheckErrors checks if there were any transfer errors.

  @param[in] Context - driver context
**/
VOID
CheckErrors (
  I2C_MASTER_CONTEXT *Context
  );

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
  );

/**
  Clean up Hw activity and errors
  Return status to Request's submitter and signal the event that tells
  it that the request is complete
  Clear up Sw context to allow new request to start

  @param[in] Context - driver context
**/
VOID
FinishTransfer (
  I2C_MASTER_CONTEXT *Context
  );

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
  );

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
  );

/**
  Reset the I2C controller

  @param[in] MmioAddress    Address of I2C controller

  @retval Status
**/
EFI_STATUS
I2cReset (
  IN UINTN     MmioAddress
  );

#endif // _I2C_MASTER_COMMON_LIB_H_
