/** @file
  Large Variable Write Lib

  This library is used to store large data sets using the UEFI Variable Services.
  At time of writting, most UEFI Variable Services implementations do not allow
  more than 64KB of data to be stored in a single UEFI variable. This library
  will split data sets across multiple variables as needed.

  In the case where more than one variable is needed to store the data, an
  integer number will be added to the end of the variable name. This number
  will be incremented for each variable as needed to store the entire data set.

  The primary use for this library is to create binary compatible drivers
  and OpROMs which need to work both with TianoCore and other UEFI PI
  implementations. When customizing and recompiling the platform firmware image
  is possible, adjusting the value of PcdMaxVariableSize may provide a simpler
  solution to this problem.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _LARGE_VARIABLE_WRITE_LIB_H_
#define _LARGE_VARIABLE_WRITE_LIB_H_

#include <Uefi/UefiBaseType.h>

/**
  Sets the value of a large variable.

  @param[in]  VariableName       A Null-terminated string that is the name of the vendor's variable.
                                 Each VariableName is unique for each VendorGuid. VariableName must
                                 contain 1 or more characters. If VariableName is an empty string,
                                 then EFI_INVALID_PARAMETER is returned.
  @param[in]  VendorGuid         A unique identifier for the vendor.
  @param[in]  LockVariable       If TRUE, any further writes to the variable will be prevented until the next reset.
                                 Note: LockVariable must be FALSE when running in SMM or after ExitBootServices.
  @param[in]  DataSize           The size in bytes of the Data buffer. A size of zero causes the variable to be deleted.
                                 If DataSize is zero, then LockVariable must be FALSE since a variable that does not
                                 exist cannot be locked.
  @param[in]  Data               The contents for the variable.

  @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as
                                 defined by the Attributes.
  @retval EFI_INVALID_PARAMETER  An invalid combination of LockVariable, name, and GUID was supplied, or the
                                 DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER  VariableName is an empty string.
  @retval EFI_INVALID_PARAMETER  DataSize is zero and LockVariable is TRUE
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data.
  @retval EFI_OUT_OF_RESOURCES   The VariableName is longer than 1018 characters
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED    The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted.

  @retval EFI_NOT_FOUND          The variable trying to be updated or deleted was not found.

**/
EFI_STATUS
EFIAPI
SetLargeVariable (
  IN  CHAR16                       *VariableName,
  IN  EFI_GUID                     *VendorGuid,
  IN  BOOLEAN                      LockVariable,
  IN  UINTN                        DataSize,
  IN  VOID                         *Data
  );

#endif  // _LARGE_VARIABLE_WRITE_LIB_H_
