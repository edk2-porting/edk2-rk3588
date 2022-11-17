/** @file
  Board-specific EC library

  Copyright (c) 2021, Baruch Binyamin Doron
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _BOARD_EC_LIB_H_
#define _BOARD_EC_LIB_H_

/**
  Reads a byte of EC RAM.

  @param[in]  Address               Address to read
  @param[out] Data                  Data received

  @retval    EFI_SUCCESS            Command success
  @retval    EFI_INVALID_PARAMETER  Data is NULL
  @retval    EFI_DEVICE_ERROR       Command error
  @retval    EFI_TIMEOUT            Command timeout
**/
EFI_STATUS
EcCmd90Read (
  IN  UINT8                  Address,
  OUT UINT8                  *Data
  );

/**
  Writes a byte of EC RAM.

  @param[in] Address           Address to write
  @param[in] Data              Data to write

  @retval    EFI_SUCCESS       Command success
  @retval    EFI_DEVICE_ERROR  Command error
  @retval    EFI_TIMEOUT       Command timeout
**/
EFI_STATUS
EcCmd91Write (
  IN  UINT8                  Address,
  IN  UINT8                  Data
  );

/**
  Query the EC status.

  @param[out] Status                EC status byte

  @retval    EFI_SUCCESS            Command success
  @retval    EFI_INVALID_PARAMETER  Data is NULL
  @retval    EFI_DEVICE_ERROR       Command error
  @retval    EFI_TIMEOUT            Command timeout
**/
EFI_STATUS
EcCmd94Query (
  OUT UINT8                  *Data
  );

/**
  Reads a byte of EC (index) RAM.

  @param[in]  Address               Address to read
  @param[out] Data                  Data received
**/
VOID
EcIdxRead (
  IN  UINT16                 Address,
  OUT UINT8                  *Data
  );

/**
  Writes a byte of EC (index) RAM.

  @param[in] Address          Address to read
  @param[in] Data             Data received
**/
VOID
EcIdxWrite (
  IN  UINT16                 Address,
  IN  UINT8                  Data
  );

/**
  Read EC analog-digital converter.
  TODO: Check if ADC is valid.

  @param[in]  Adc
  @param[out] DataBuffer
**/
VOID
ReadEcAdcConverter (
  IN  UINT8        Adc,
  OUT UINT16       *DataBuffer
  );

#endif
