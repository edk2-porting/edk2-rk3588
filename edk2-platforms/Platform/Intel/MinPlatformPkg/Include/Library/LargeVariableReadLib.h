/** @file
  Large Variable Read Lib

  This library is used to retrieve large data sets using the UEFI Variable
  Services. At time of writing, most UEFI Variable Services implementations do
  not allow more than 64KB of data to be stored in a single UEFI variable. This
  library will split data sets across multiple variables as needed.

  In the case where more than one variable is needed to store the data, an
  integer number will be added to the end of the variable name. This number
  will be incremented for each variable as needed to retrieve the entire data
  set.

  The primary use for this library is to create binary compatible drivers
  and OpROMs which need to work both with TianoCore and other UEFI PI
  implementations. When customizing and recompiling the platform firmware image
  is possible, adjusting the value of PcdMaxVariableSize may provide a simpler
  solution to this problem.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _LARGE_VARIABLE_READ_LIB_H_
#define _LARGE_VARIABLE_READ_LIB_H_

#include <Uefi/UefiBaseType.h>

/**
  Returns the value of a large variable.

  @param[in]       VariableName  A Null-terminated string that is the name of the vendor's
                                 variable.
  @param[in]       VendorGuid    A unique identifier for the vendor.
  @param[in, out]  DataSize      On input, the size in bytes of the return Data buffer.
                                 On output the size of data returned in Data.
  @param[out]      Data          The buffer to return the contents of the variable. May be NULL
                                 with a zero DataSize in order to determine the size buffer needed.

  @retval EFI_SUCCESS            The function completed successfully.
  @retval EFI_NOT_FOUND          The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL   The DataSize is too small for the result.
  @retval EFI_INVALID_PARAMETER  VariableName is NULL.
  @retval EFI_INVALID_PARAMETER  VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER  DataSize is NULL.
  @retval EFI_INVALID_PARAMETER  The DataSize is not too small and Data is NULL.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure.

**/
EFI_STATUS
EFIAPI
GetLargeVariable (
  IN     CHAR16                      *VariableName,
  IN     EFI_GUID                    *VendorGuid,
  IN OUT UINTN                       *DataSize,
  OUT    VOID                        *Data           OPTIONAL
  );

#endif  // _LARGE_VARIABLE_READ_LIB_H_
