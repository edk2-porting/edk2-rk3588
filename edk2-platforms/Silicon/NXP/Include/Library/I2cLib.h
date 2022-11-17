/** @file
  I2c Lib to control I2c controller.

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef I2C_LIB_H__
#define I2C_LIB_H__

#include <Uefi.h>
#include <Pi/PiI2c.h>

/**
  software reset of the entire I2C module.
  The module is reset and disabled.
  Status register fields (IBSR) are cleared.

  @param[in] Base       Base Address of I2c controller's registers

  @return  EFI_SUCCESS  successfuly reset the I2c module
**/
EFI_STATUS
I2cReset (
  IN UINTN  Base
  );

/**
  Configure I2c bus to operate at a given speed

  @param[in] Base         Base Address of I2c controller's registers
  @param[in] I2cBusClock  Input clock to I2c controller
  @param[in] Speed        speed to be configured for I2c bus

  @return  EFI_SUCCESS  successfuly setup the i2c bus
**/
EFI_STATUS
I2cInitialize (
  IN UINTN  Base,
  IN UINT64 I2cBusClock,
  IN UINT64 Speed
  );

/**
  Transfer data to/from I2c slave device

  @param[in] Base           Base Address of I2c controller's registers
  @param[in] SlaveAddress   Slave Address from which data is to be read
  @param[in] RequestPacket  Pointer to an EFI_I2C_REQUEST_PACKET structure
                            describing the I2C transaction

  @return  EFI_SUCCESS       successfuly transfer the data
  @return  EFI_DEVICE_ERROR  There was an error while transferring data through
                             I2c bus
  @return  EFI_NO_RESPONSE   There was no Ack from i2c device
  @return  EFI_TIMEOUT       I2c Bus is busy
  @return  EFI_NOT_READY     I2c Bus Arbitration lost
**/
EFI_STATUS
I2cBusXfer (
  IN UINTN                  Base,
  IN UINT32                 SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET *RequestPacket
  );

/**
  Read a register from I2c slave device. This API is wrapper around I2cBusXfer

  @param[in]  Base                   Base Address of I2c controller's registers
  @param[in]  SlaveAddress           Slave Address from which register value is
                                     to be read
  @param[in]  RegAddress             Register Address in Slave's memory map
  @param[in]  RegAddressWidthInBytes Number of bytes in RegAddress to send to
                                     I2c Slave for simple reads without any
                                     register, make this value = 0
                                     (RegAddress is don't care in that case)
  @param[out] RegValue               Value to be read from I2c slave's regiser
  @param[in]  RegValueNumBytes       Number of bytes to read from I2c slave
                                     register

  @return  EFI_SUCCESS       successfuly read the registers
  @return  EFI_DEVICE_ERROR  There was an error while transferring data through
                             I2c bus
  @return  EFI_NO_RESPONSE   There was no Ack from i2c device
  @return  EFI_TIMEOUT       I2c Bus is busy
  @return  EFI_NOT_READY     I2c Bus Arbitration lost
**/
EFI_STATUS
I2cBusReadReg (
  IN  UINTN   Base,
  IN  UINT32  SlaveAddress,
  IN  UINT64  RegAddress,
  IN  UINT8   RegAddressWidthInBytes,
  OUT UINT8   *RegValue,
  IN  UINT32  RegValueNumBytes
  );

#endif // I2C_LIB_H__
