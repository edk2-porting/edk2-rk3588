/** @file
  Interface header file for the Compressed Variable library class.

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _COMPRESSED_VARIABLE_LIB_H_
#define _COMPRESSED_VARIABLE_LIB_H_

#include <Uefi.h>

/**
  Retrieve data from a HOB(s), then compress and save the data.

  @param[in]  HobGuid        GUID of the HOB to save.
  @param[in]  VariableName   Name of the variable to save as.
  @param[in]  VariableGuid   GUID of the variable to save as.

  @retval EFI_SUCCESS           The variable was saved successfully.
  @retval !EFI_SUCCESS          Failure.

**/

EFI_STATUS
EFIAPI
SaveVariableFromHob (
  IN EFI_GUID HobGuid,
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid
  );

#endif // #ifndef _COMPRESSED_VARIABLE_LIB_H_
