/** @file
  Common EC commands.

Copyright (c) 2019 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <Library/EcLib.h>

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
  )
{
  UINT8                      DataSize;
  UINT8                      DataBuffer[1];
  EFI_STATUS                 Status;

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Prepare arguments for LpcEcInterface()
  DataSize = 1;
  DataBuffer[0] = Address;

  Status = LpcEcInterface (EC_C_ACPI_READ, &DataSize, DataBuffer);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Write caller's pointer from returned data and return success
  *Data = DataBuffer[0];
  return EFI_SUCCESS;
}

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
  )
{
  UINT8                      DataSize;
  UINT8                      DataBuffer[2];

  // Prepare arguments for LpcEcInterface()
  DataSize = 2;
  DataBuffer[0] = Address;
  DataBuffer[1] = Data;

  return LpcEcInterface (EC_C_ACPI_WRITE, &DataSize, DataBuffer);
}
