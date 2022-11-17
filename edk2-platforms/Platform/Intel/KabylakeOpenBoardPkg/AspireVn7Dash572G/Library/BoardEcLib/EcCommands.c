/** @file
  Board-specific EC commands.

  Copyright (c) 2021, Baruch Binyamin Doron
  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/EcLib.h>
#include <Library/IoLib.h>

/* Notes:
 * - ACPI "CMDB": Writing to this offset is equivalent to sending commands.
 *   The CMDx bytes contain the command parameters.
 *
 * TODO - Implement:
 *   - Commands: 0x58, 0xE1 and 0xE2
 *     - 0x51, 0x52: EC flash write?
 *   - ACPI CMDB: 0x63 and 0x64, 0xC7
 *     - 0x0B: Flash write (Boolean argument? Set in offset 0x0B?)
 *
 * Reversing vendor's protocols:
 * - Only read and write are used.
 * - Query, ACPI "CMDB" processing and command 58 are unused.
 * - Equivalent KbcPeim is an unused PPI.
 *
 * NB: Also look for potential EC library
 */

#define EC_INDEX_IO_PORT            0x1200
#define EC_INDEX_IO_HIGH_ADDR_PORT  EC_INDEX_IO_PORT+1
#define EC_INDEX_IO_LOW_ADDR_PORT   EC_INDEX_IO_PORT+2
#define EC_INDEX_IO_DATA_PORT       EC_INDEX_IO_PORT+3

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
  )
{
  EFI_STATUS                 Status;

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SendEcCommand (0x90);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a(): SendEcCommand(0x90) failed!\n", __FUNCTION__));
    return Status;
  }

  Status = SendEcData (Address);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a(): SendEcData(Address) failed!\n", __FUNCTION__));
    return Status;
  }

  Status = ReceiveEcData (Data);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a(): ReceiveEcData(Data) failed!\n", __FUNCTION__));
    return Status;
  }
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                 Status;

  Status = SendEcCommand (0x91);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a(): SendEcCommand(0x91) failed!\n", __FUNCTION__));
    return Status;
  }

  Status = SendEcData (Address);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a(): SendEcData(Address) failed!\n", __FUNCTION__));
    return Status;
  }

  Status = SendEcData (Data);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a(): SendEcData(Data) failed!\n", __FUNCTION__));
    return Status;
  }
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                 Status;

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SendEcCommand (0x94);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a(): SendEcCommand(0x94) failed!\n", __FUNCTION__));
    return Status;
  }

  Status = ReceiveEcData (Data);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a(): ReceiveEcData(Data) failed!\n", __FUNCTION__));
    return Status;
  }
  return EFI_SUCCESS;
}

/**
  Reads a byte of EC (index) RAM.

  @param[in]  Address               Address to read
  @param[out] Data                  Data received
**/
VOID
EcIdxRead (
  IN  UINT16                 Address,
  OUT UINT8                  *Data
  )
{
  if (Data == NULL) {
    return;
  }

  IoWrite8 (EC_INDEX_IO_HIGH_ADDR_PORT, (UINT8) (Address >> 8));
  IoWrite8 (EC_INDEX_IO_LOW_ADDR_PORT, (UINT8) Address);
  *Data = IoRead8 (EC_INDEX_IO_DATA_PORT);
}

/**
  Writes a byte of EC (index) RAM.

  @param[in] Address          Address to read
  @param[in] Data             Data received
**/
VOID
EcIdxWrite (
  IN  UINT16                 Address,
  IN  UINT8                  Data
  )
{
  IoWrite8 (EC_INDEX_IO_HIGH_ADDR_PORT, (UINT8) (Address >> 8));
  IoWrite8 (EC_INDEX_IO_LOW_ADDR_PORT, (UINT8) Address);
  IoWrite8 (EC_INDEX_IO_DATA_PORT, Data);
}

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
  )
{
  UINT8            AdcConvertersEnabled;  // Contains some ADCs and some DACs
  UINT8            IdxData;

  if (DataBuffer == NULL) {
    return;
  }

  // Backup enabled ADCs
  EcIdxRead (0xff15, &AdcConvertersEnabled);  // ADDAEN

  // Enable desired ADC in bitmask (not enabled by EC FW, not used by vendor FW)
  EcIdxWrite (0xff15, AdcConvertersEnabled | ((1 << Adc) & 0xf));  // ADDAEN

  // Sample the desired ADC in binary field; OR the start bit
  EcIdxWrite (0xff18, ((Adc << 1) & 0xf) | 1);  // ADCTRL

  // Read the desired ADC
  EcIdxRead (0xff19, &IdxData);  // ADCDAT
  *DataBuffer = (IdxData << 2);
  // Lower 2-bits of 10-bit ADC are in high bits of next register
  EcIdxRead (0xff1a, &IdxData);  // ECIF
  *DataBuffer |= ((IdxData & 0xc0) >> 6);

  // Restore enabled ADCs
  EcIdxWrite (0xff15, AdcConvertersEnabled);  // ADDAEN
}
