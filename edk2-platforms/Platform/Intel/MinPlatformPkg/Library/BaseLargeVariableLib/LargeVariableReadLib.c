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

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/VariableReadLib.h>

#include "LargeVariableCommon.h"

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
  )
{
  CHAR16        TempVariableName[MAX_VARIABLE_NAME_SIZE];
  EFI_STATUS    Status;
  UINTN         TotalSize;
  UINTN         VarDataSize;
  UINTN         Index;
  UINTN         VariableSize;
  UINTN         BytesRemaining;
  UINT8         *OffsetPtr;

  VarDataSize = 0;

  //
  // First check if a variable with the given name exists
  //
  Status = VarLibGetVariable (VariableName, VendorGuid, NULL, &VarDataSize, NULL);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    if (*DataSize >= VarDataSize) {
      if (Data == NULL) {
        Status = EFI_INVALID_PARAMETER;
        goto Done;
      }
      DEBUG ((DEBUG_VERBOSE, "GetLargeVariable: Single Variable Found\n"));
      Status = VarLibGetVariable (VariableName, VendorGuid, NULL, DataSize, Data);
      goto Done;
    } else {
      *DataSize = VarDataSize;
      Status = EFI_BUFFER_TOO_SMALL;
      goto Done;
    }

  } else if (Status == EFI_NOT_FOUND) {
    //
    // Check if the first variable of a multi-variable set exists
    //
    if (StrLen (VariableName) >= (MAX_VARIABLE_NAME_SIZE - MAX_VARIABLE_SPLIT_DIGITS)) {
      DEBUG ((DEBUG_ERROR, "GetLargeVariable: Variable name too long\n"));
      Status = EFI_OUT_OF_RESOURCES;
      goto Done;
    }

    VarDataSize = 0;
    Index       = 0;
    ZeroMem (TempVariableName, MAX_VARIABLE_NAME_SIZE);
    UnicodeSPrint (TempVariableName, MAX_VARIABLE_NAME_SIZE, L"%s%d", VariableName, Index);
    Status = VarLibGetVariable (TempVariableName, VendorGuid, NULL, &VarDataSize, NULL);

    if (Status == EFI_BUFFER_TOO_SMALL) {
      //
      // The first variable exists. Calculate the total size of all the variables.
      //
      DEBUG ((DEBUG_VERBOSE, "GetLargeVariable: Multiple Variables Found\n"));
      TotalSize = 0;
      for (Index = 0; Index < MAX_VARIABLE_SPLIT; Index++) {
        VarDataSize = 0;
        ZeroMem (TempVariableName, MAX_VARIABLE_NAME_SIZE);
        UnicodeSPrint (TempVariableName, MAX_VARIABLE_NAME_SIZE, L"%s%d", VariableName, Index);
        Status = VarLibGetVariable (TempVariableName, VendorGuid, NULL, &VarDataSize, NULL);
        if (Status != EFI_BUFFER_TOO_SMALL) {
          break;
        }
        TotalSize += VarDataSize;
      }
      DEBUG ((DEBUG_VERBOSE, "TotalSize = %d, NumVariables = %d\n", TotalSize, Index));

      //
      // Check if the user provided a large enough buffer
      //
      if (*DataSize >= TotalSize) {
        if (Data == NULL) {
          Status = EFI_INVALID_PARAMETER;
          goto Done;
        }

        //
        // Read the data from all variables
        //
        OffsetPtr       = (UINT8 *) Data;
        BytesRemaining  = *DataSize;
        for (Index = 0; Index < MAX_VARIABLE_SPLIT; Index++) {
          VarDataSize = 0;
          ZeroMem (TempVariableName, MAX_VARIABLE_NAME_SIZE);
          UnicodeSPrint (TempVariableName, MAX_VARIABLE_NAME_SIZE, L"%s%d", VariableName, Index);
          VariableSize = BytesRemaining;
          DEBUG ((DEBUG_INFO, "Reading %s, Guid = %g,", TempVariableName, VendorGuid));
          Status = VarLibGetVariable (TempVariableName, VendorGuid, NULL, &VariableSize, (VOID *) OffsetPtr);
          DEBUG ((DEBUG_INFO, " Size %d\n", VariableSize));
          if (EFI_ERROR (Status)) {
            if (Status == EFI_NOT_FOUND) {
              DEBUG ((DEBUG_VERBOSE, "No more variables found\n"));
              Status = EFI_SUCCESS;   // The end has been reached
            }
            goto Done;
          }

          if (VariableSize < BytesRemaining) {
            BytesRemaining -= VariableSize;
            OffsetPtr += VariableSize;
          } else {
            DEBUG ((DEBUG_VERBOSE, "All data has been read\n"));
            BytesRemaining = 0;
            break;
          }
        }   //End of for loop

        goto Done;
      } else {
        *DataSize = TotalSize;
        Status = EFI_BUFFER_TOO_SMALL;
        goto Done;
      }
    } else {
      Status = EFI_NOT_FOUND;
    }
  }

Done:
  DEBUG ((DEBUG_ERROR, "GetLargeVariable: Status = %r\n", Status));
  return Status;
}
