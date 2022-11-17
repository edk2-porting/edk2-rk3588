/** @file
  PCH Smbus Protocol

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_SMBUS_COMMON_LIB_H
#define _PCH_SMBUS_COMMON_LIB_H

//
// Definitions
//
#define STALL_PERIOD                10 * STALL_ONE_MICRO_SECOND     ///< 10 microseconds
#define STALL_TIME                  STALL_ONE_SECOND                ///< 1 second
#define BUS_TRIES                   3       ///< How many times to retry on Bus Errors
#define SMBUS_NUM_RESERVED          38      ///< Number of device addresses that are reserved by the SMBus spec.
#define SMBUS_ADDRESS_ARP           0xC2 >> 1
#define SMBUS_DATA_PREPARE_TO_ARP   0x01
#define SMBUS_DATA_RESET_DEVICE     0x02
#define SMBUS_DATA_GET_UDID_GENERAL 0x03
#define SMBUS_DATA_ASSIGN_ADDRESS   0x04
#define SMBUS_GET_UDID_LENGTH       17      ///< 16 byte UDID + 1 byte address
//
// Private data and functions
//

typedef
UINT8
(EFIAPI *SMBUS_IO_READ) (
  IN      UINT8                     Offset
  );

typedef
VOID
(EFIAPI *SMBUS_IO_WRITE) (
  IN      UINT8                     Offset,
  IN      UINT8                     Data
  );

typedef
BOOLEAN
(EFIAPI *SMBUS_IO_DONE) (
  IN      UINT8                     *StsReg
  );

#define PCH_SMBUS_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('p', 's', 'm', 'b')

/**
  Get SMBUS IO Base address


  @retval UINT32                  The SMBUS IO Base Address
**/
UINT32
SmbusGetIoBase (
  VOID
  );

/**
  This function provides a standard way to read PCH Smbus IO registers.

  @param[in] Offset               Register offset from Smbus base IO address.

  @retval UINT8                   Returns data read from IO.
**/
UINT8
EFIAPI
SmbusIoRead (
  IN      UINT8           Offset
  );

/**
  This function provides a standard way to write PCH Smbus IO registers.

  @param[in] Offset               Register offset from Smbus base IO address.
  @param[in] Data                 Data to write to register.

**/
VOID
EFIAPI
SmbusIoWrite (
  IN      UINT8           Offset,
  IN      UINT8           Data
  );

/**
  This function provides a standard way to check if an SMBus transaction has
  completed.

  @param[in] StsReg               Not used for input. On return, contains the
                                  value of the SMBus status register.

  @retval TRUE                    Transaction is complete
  @retval FALSE                   Otherwise.
**/
BOOLEAN
EFIAPI
IoDone (
  IN      UINT8           *StsReg
  );

/**
  Check if it's ok to use the bus.


  @retval EFI_SUCCESS             SmBus is acquired and it's safe to send commands.
  @retval EFI_TIMEOUT             SmBus is busy, it's not safe to send commands.
**/
EFI_STATUS
AcquireBus (
  VOID
  );

/**
  This function provides a standard way to execute Smbus protocols
  as defined in the SMBus Specification. The data can either be of
  the Length byte, word, or a block of data. The resulting transaction will be
  either the SMBus Slave Device accepts this transaction or this function
  returns with an error

  @param[in] SlaveAddress         Smbus Slave device the command is directed at
  @param[in] Command              Slave Device dependent
  @param[in] Operation            Which SMBus protocol will be used
  @param[in] PecCheck             Defines if Packet Error Code Checking is to be used
  @param[in, out] Length          How many bytes to read. Must be 0 <= Length <= 32 depending on Operation
                                  It will contain the actual number of bytes read/written.
  @param[in, out] Buffer          Contain the data read/written.

  @retval EFI_SUCCESS             The operation completed successfully.
  @exception EFI_UNSUPPORTED      The operation is unsupported.

  @retval EFI_INVALID_PARAMETER   Length or Buffer is NULL for any operation besides
                                  quick read or quick write.
  @retval EFI_TIMEOUT             The transaction did not complete within an internally
                                  specified timeout period, or the controller is not
                                  available for use.
  @retval EFI_DEVICE_ERROR        There was an Smbus error (NACK) during the operation.
                                  This could indicate the slave device is not present
                                  or is in a hung condition.
**/
EFI_STATUS
SmbusExec (
  IN      EFI_SMBUS_DEVICE_ADDRESS  SlaveAddress,
  IN      EFI_SMBUS_DEVICE_COMMAND  Command,
  IN      EFI_SMBUS_OPERATION       Operation,
  IN      BOOLEAN                   PecCheck,
  IN OUT  UINTN                     *Length,
  IN OUT  VOID                      *Buffer
  );

/**
  This function initializes the Smbus Registers.

**/
VOID
InitializeSmbusRegisters (
  VOID
  );
#endif
