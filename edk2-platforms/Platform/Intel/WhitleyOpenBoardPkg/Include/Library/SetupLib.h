/** @file
  This library abstracts read/write access for basic type data those values may be
  stored into the different media.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SETUP_LIB_H_
#define _SETUP_LIB_H_

#include <SetupTable.h>

typedef struct {
  EFI_GUID        *GuidValue;
  CHAR16          *SetupName;
  UINTN           VariableSize;
} GROUP_INFO;

/**
  This function provides a means by which to retrieve a value for a given option.

  Returns the data, data type and data size specified by Guid and OptionNumber.

  @param[in]  Guid          Pointer to a 128-bit unique value that designates
                            which namespace to retrieve a value from.
  @param[in]  OptionNumber  The option number to retrieve a current value for.
  @param[out] DataType      A pointer to basic data type of the retrieved data.
                            It is optional. It could be NULL.
  @param[in, out] Data      A pointer to the buffer to be retrieved.
  @param[in, out] DataSize  The size, in bytes, of Buffer.

  @retval EFI_SUCCESS           Data is successfully reterieved.
  @retval EFI_INVALID_PARAMETER Guid is NULL or DataSize is NULL or OptionNumber is invalid.
  @retval EFI_BUFFER_TOO_SMALL  Input data buffer is not enough.
  @retval EFI_NOT_FOUND         The given option is not found.

**/
EFI_STATUS
EFIAPI
GetOptionData (
  IN  EFI_GUID        *Guid,
  IN  UINTN           OptionNumber,
  IN  OUT VOID        *Data,
  IN  OUT UINTN       DataSize
  );

/**
  This function provides a means by which to set a value for a given option number.

  Sets a buffer for the data specified by Guid and OptionNumber to the value specified by
  Data and DataSize.
  If DataSize is greater than the maximum size support by OptionNumber,
  then set DataSize to the maximum size supported by OptionNumber.

  @param[in]  Guid              Pointer to a 128-bit unique value that
                                designates which namespace to set a value from.
  @param[in]  OptionNumber      The option number to set a current value for.
  @param[in]  Data              A pointer to the buffer to set.
  @param[in, out] DataSize      The size, in bytes, of Buffer.

  @retval EFI_SUCCESS           Data is successfully updated.
  @retval EFI_INVALID_PARAMETER OptionNumber is invalid, Guid is NULL, or Data is NULL, or DataSize is NULL.
  @retval EFI_NOT_FOUND         The given option is not found.
  @retval EFI_UNSUPPORTED       Set action is not supported.
**/
EFI_STATUS
EFIAPI
SetOptionData (
  IN EFI_GUID  *Guid,
  IN UINTN     OptionNumber,
  IN VOID      *Data,
  IN UINTN     DataSize
  );

/**
  Get all data in the setup

  @retval EFI_SUCCESS           Data is committed successfully.
  @retval EFI_INVALID_PARAMETER Guid is NULL.
  @retval EFI_NOT_FOUND         Guid is not found.
  @retval EFI_DEVICE_ERROR      Data can't be committed.
**/
EFI_STATUS
EFIAPI
GetEntireConfig (
  IN OUT SETUP_DATA *SetupData
  );

/**
  Set all data in the setup

  @retval EFI_SUCCESS           Data is committed successfully.
  @retval EFI_INVALID_PARAMETER Guid is NULL.
  @retval EFI_NOT_FOUND         Guid is not found.
  @retval EFI_DEVICE_ERROR      Data can't be committed.
**/
EFI_STATUS
EFIAPI
SetEntireConfig (
  IN SETUP_DATA *SetupData
  );

/**
  Get Specified data in the setup

  @retval EFI_SUCCESS           Data is successfully reterieved.
  @retval EFI_INVALID_PARAMETER Guid or Variable is null.
  @retval EFI_NOT_FOUND         The given option is not found.
**/
EFI_STATUS
EFIAPI
GetSpecificConfigGuid (
  IN EFI_GUID            *Guid,
  IN OUT VOID            *Variable
  );

/**
  Set Specified data in the setup

  @retval EFI_SUCCESS           Data is successfully reterieved.
  @retval EFI_INVALID_PARAMETER Guid or Variable is null.
  @retval EFI_NOT_FOUND         The given option is not found.
**/
EFI_STATUS
EFIAPI
SetSpecificConfigGuid (
  IN EFI_GUID            *Guid,
  IN VOID                *Variable
  );

#endif
