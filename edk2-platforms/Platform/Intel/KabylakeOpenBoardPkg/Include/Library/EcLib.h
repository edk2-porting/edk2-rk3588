/** @file
  EC library functions and definitions.

  This library provides basic EC interface.

  There may be different libraries for different environments (PEI, BS, RT, SMM).
  Make sure you meet the requirements for the library (protocol dependencies, use
  restrictions, etc).

Copyright (c) 2019 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _BASE_EC_LIB_H_
#define _BASE_EC_LIB_H_

//
// Include files
//
#include <Base.h>
#include <Uefi.h>
#include <EcCommands.h>

//
// Function declarations
//
/**
  Send a command to the Keyboard System Controller.

  @param[in] Command      Command byte to send

  @retval    EFI_SUCCESS  Command success
  @retval    EFI_TIMEOUT  Command timeout
  @retval    Other        Command failed
**/
EFI_STATUS
SendEcCommand (
  IN UINT8   Command
  );

/**
  Sends data to Keyboard System Controller.

  @param[in] Data          Data byte to send

  @retval    EFI_SUCCESS   Success
  @retval    EFI_TIMEOUT   Timeout
  @retval    Other         Failed
**/
EFI_STATUS
SendEcData (
  IN UINT8   Data
  );

/**
  Receives data from Keyboard System Controller.

  @param[out] Data          Data byte received

  @retval     EFI_SUCCESS   Read success
  @retval     EFI_TIMEOUT   Read timeout
  @retval     Other         Read failed
**/
EFI_STATUS
ReceiveEcData (
  OUT UINT8   *Data
  );

/**
  Receives status from Keyboard System Controller.

  @param[out] Status       Status byte to receive

  @retval     EFI_SUCCESS  Success
  @retval     Other        Failed
**/
EFI_STATUS
ReceiveEcStatus (
  OUT UINT8   *EcStatus
  );

/**
  Send data to EC through LPC interface.

  @param[in]      Command     Command value to send to the EC
  @param[in][out] DataSize    Size of data to send to the EC.
                              If the command retuned data - size of buffer returned by the EC.
                              Be aware of the DataSize must euqal to size of DataBuffer and cannot smaller
                              than number of send data or number of receive data, whichever is the grater.
  @param[in][out] DataBuffer  Pointer to the data buffer including data to be sent to the EC.
                              If the command returned data - pointer to the buffer including the data.
                              The buffer size should be the max of receive and transmit data.

  @retval         EFI_SUCCESS Success
  @retval         Other       Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                       EFI_BUFFER_TOO_SMALL, etc.
**/
EFI_STATUS
LpcEcInterface (
  IN UINT8                     Command,
  IN OUT UINT8                 *DataSize,
  IN OUT UINT8                 *DataBuffer
  );

/**
  Read a byte of EC RAM.

  @param[in]  Address          Address to read
  @param[out] Data             Data received

  @retval    EFI_SUCCESS            Command success
  @retval    EFI_INVALID_PARAMETER  Data is NULL
  @retval    EFI_DEVICE_ERROR       Command error
  @retval    EFI_TIMEOUT            Command timeout
**/
EFI_STATUS
EcRead (
  IN  UINT8                  Address,
  OUT UINT8                  *Data
  );

/**
  Write a byte of EC RAM.

  @param[in] Address           Address to write
  @param[in] Data              Data to write

  @retval    EFI_SUCCESS       Command success
  @retval    EFI_DEVICE_ERROR  Command error
  @retval    EFI_TIMEOUT       Command timeout
**/
EFI_STATUS
EcWrite (
  IN  UINT8                  Address,
  IN  UINT8                  Data
  );

#endif
