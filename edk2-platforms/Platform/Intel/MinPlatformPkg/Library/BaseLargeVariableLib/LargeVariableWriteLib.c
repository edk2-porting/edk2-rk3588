/** @file
  Large Variable Write Lib

  This library is used to store large data sets using the UEFI Variable Services.
  At time of writting, most UEFI Variable Services implementations do not allow
  more than 64KB of data to be stored in a single UEFI variable. This library
  will split data sets across multiple variables as needed.

  In the case where more than one variable is needed to store the data, an
  integer number will be added to the end of the variable name. This number
  will be incremented for each variable as needed to store the entire data set.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/VariableReadLib.h>
#include <Library/VariableWriteLib.h>

#include "LargeVariableCommon.h"

/**
  Returns the amount of space remaining for storage of
  non-volatile runtime accessible UEFI variables. Runtime accessible UEFI
  variables are chosen because this is a BASE LibraryClass. Accordingly, it is
  possible for a Runtime DXE driver to include this library. To simplify
  implementation we only consider the runtime accessible case.

  @retval The remaining storage space for non-volatile runtime accessible UEFI
          variables in bytes.

**/
UINT64
GetRemainingVariableStorageSpace (
  VOID
  )
{
  EFI_STATUS      Status;
  UINT64          MaximumVariableStorageSize;
  UINT64          RemainingVariableStorageSize;
  UINT64          MaximumVariableSize;

  Status                        = EFI_SUCCESS;
  MaximumVariableStorageSize    = 0;
  RemainingVariableStorageSize  = 0;
  MaximumVariableSize           = 0;

  Status = VarLibQueryVariableInfo (
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             &MaximumVariableStorageSize,
             &RemainingVariableStorageSize,
             &MaximumVariableSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetVariableSplitSize: QueryVariableInfo failed, Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return 0;
  }

  return RemainingVariableStorageSize;
}

/**
  Returns the maximum size of an individual non-volatile runtime accessible
  UEFI variable. Runtime accessible UEFI variables are chosen because this is
  a BASE LibraryClass. Accordingly, it is possible for a Runtime DXE driver to
  include this library. To simplify implementation we only consider the runtime
  accessible case.

  @param[in] VariableNameLength         Length of the variable name

  @retval The maximum size of an individual UEFI variable.

**/
UINT64
GetVariableSplitSize (
  IN  UINTN                        VariableNameLength
  )
{
  EFI_STATUS      Status;
  UINT64          MaximumVariableStorageSize;
  UINT64          RemainingVariableStorageSize;
  UINT64          VariableSplitSize;

  Status                        = EFI_SUCCESS;
  MaximumVariableStorageSize    = 0;
  RemainingVariableStorageSize  = 0;
  VariableSplitSize             = 0;

  Status = VarLibQueryVariableInfo (
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             &MaximumVariableStorageSize,
             &RemainingVariableStorageSize,
             &VariableSplitSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetVariableSplitSize: QueryVariableInfo failed, Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return 0;
  } else {
    //
    // The Variable Name counts as part of the consumed NV storage
    // space for a UEFI Variable
    //
    if ((((VariableNameLength + 1) * sizeof (CHAR16)) + MAX_VARIABLE_NAME_PAD_SIZE) >= VariableSplitSize) {
      VariableSplitSize = 0;
    } else {
      VariableSplitSize -= ((VariableNameLength + 1) * sizeof (CHAR16)) + MAX_VARIABLE_NAME_PAD_SIZE;
    }
  }

  return VariableSplitSize;
}

/**
  Deletes a large variable.

  @param[in]  VariableName       A Null-terminated string that is the name of the vendor's variable.
                                 Each VariableName is unique for each VendorGuid. VariableName must
                                 contain 1 or more characters. If VariableName is an empty string,
                                 then EFI_INVALID_PARAMETER is returned.
  @param[in]  VendorGuid         A unique identifier for the vendor.

  @retval EFI_SUCCESS            The firmware has successfully deleted the variable and its data as
                                 defined by the Attributes.
  @retval EFI_INVALID_PARAMETER  An invalid combination of name, and GUID was supplied.
  @retval EFI_INVALID_PARAMETER  VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES   The VariableName is longer than 1018 characters
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED    The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted.

  @retval EFI_NOT_FOUND          The variable trying to be deleted was not found.

**/
EFI_STATUS
EFIAPI
DeleteLargeVariableInternal (
  IN  CHAR16                       *VariableName,
  IN  EFI_GUID                     *VendorGuid
  )
{
  CHAR16        TempVariableName[MAX_VARIABLE_NAME_SIZE];
  EFI_STATUS    Status;
  EFI_STATUS    Status2;
  UINTN         VarDataSize;
  UINTN         Index;

  VarDataSize = 0;

  //
  // First check if a variable with the given name exists
  //
  Status = VarLibGetVariable (VariableName, VendorGuid, NULL, &VarDataSize, NULL);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_VERBOSE, "DeleteLargeVariableInternal: Deleting Single Variable\n"));
    Status = VarLibSetVariable (
                VariableName,
                VendorGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                0,
                NULL
                );
    goto Done;
  } else if (Status == EFI_NOT_FOUND) {

    //
    // Check if the first variable of a multi-variable set exists
    //
    if (StrLen (VariableName) >= (MAX_VARIABLE_NAME_SIZE - MAX_VARIABLE_SPLIT_DIGITS)) {
      DEBUG ((DEBUG_ERROR, "DeleteLargeVariableInternal: Variable name too long\n"));
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
      // The first variable exists. Delete all the variables.
      //
      DEBUG ((DEBUG_VERBOSE, "DeleteLargeVariableInternal: Multiple Variables Found\n"));
      Status = EFI_SUCCESS;
      for (Index = 0; Index < MAX_VARIABLE_SPLIT; Index++) {
        VarDataSize = 0;
        ZeroMem (TempVariableName, MAX_VARIABLE_NAME_SIZE);
        UnicodeSPrint (TempVariableName, MAX_VARIABLE_NAME_SIZE, L"%s%d", VariableName, Index);
        DEBUG ((DEBUG_INFO, "Deleting %s, Guid = %g\n", TempVariableName, VendorGuid));
        Status2 = VarLibSetVariable (
                    TempVariableName,
                    VendorGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    0,
                    NULL
                    );
        if (Status2 == EFI_NOT_FOUND) {
          DEBUG ((DEBUG_VERBOSE, "DeleteLargeVariableInternal: Deletion Complete.\n"));
          break;
        } else if (EFI_ERROR (Status2)) {
          DEBUG ((DEBUG_ERROR, "DeleteLargeVariableInternal: Error deleting variable: Status = %r\n", Status2));
          Status = Status2;
        }
      }   // End of for loop
    } else {
      Status = EFI_NOT_FOUND;
    }
  }
Done:
  DEBUG ((DEBUG_ERROR, "DeleteLargeVariableInternal: Status = %r\n", Status));
  return Status;
}

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
  )
{
  CHAR16        TempVariableName[MAX_VARIABLE_NAME_SIZE];
  UINT64        VariableSplitSize;
  UINT64        RemainingVariableStorage;
  EFI_STATUS    Status;
  EFI_STATUS    Status2;
  UINTN         VariableNameLength;
  UINTN         Index;
  UINTN         VariablesSaved;
  UINT8         *OffsetPtr;
  UINTN         BytesRemaining;
  UINTN         SizeToSave;

  //
  // Check input parameters.
  //
  if (VariableName == NULL || VariableName[0] == 0 || VendorGuid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DataSize != 0 && Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DataSize == 0 && LockVariable) {
    DEBUG ((DEBUG_ERROR, "SetLargeVariable: Cannot lock a variable that is being deleted\n"));
    return EFI_INVALID_PARAMETER;
  }

  VariablesSaved = 0;
  if (LockVariable && !VarLibIsVariableRequestToLockSupported ()) {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "SetLargeVariable: Variable locking is not currently supported\n"));
      goto Done;
  }

  //
  // Check if the variable is being deleted.
  //
  if (DataSize == 0) {
    DEBUG ((DEBUG_VERBOSE, "SetLargeVariable: Variable is being deleted.\n"));
    Status = DeleteLargeVariableInternal (VariableName, VendorGuid);
    goto Done;
  }

  VariableNameLength  = StrLen (VariableName);
  VariableSplitSize   = GetVariableSplitSize (VariableNameLength);
  if (DataSize <= VariableSplitSize) {

    //
    // A single variable is sufficient to store the data, only create one.
    //
    DEBUG ((DEBUG_VERBOSE, "SetLargeVariable: Saving using single variable.\n"));
    Status = VarLibSetVariable (
               VariableName,
               VendorGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               DataSize,
               Data
               );
    if (EFI_ERROR (Status)) {
      goto Done;
    }
    if (LockVariable) {
      Status = VarLibVariableRequestToLock (VariableName, VendorGuid);
    }
  } else {
    //
    // Multiple variables are needed
    //

    //
    // Check the length of the variable name is short enough to allow an integer
    // to be appended.
    //
    if (VariableNameLength >= (MAX_VARIABLE_NAME_SIZE - MAX_VARIABLE_SPLIT_DIGITS)) {
      Status = EFI_OUT_OF_RESOURCES;
      DEBUG ((DEBUG_ERROR, "SetLargeVariable: Variable name too long\n"));
      goto Done;
    }

    //
    // Check that it is possible to store the data using less than
    // MAX_VARIABLE_SPLIT variables
    //
    if ((DataSize / (VariableSplitSize - MAX_VARIABLE_SPLIT_DIGITS)) > MAX_VARIABLE_SPLIT) {
      DEBUG ((
        DEBUG_ERROR,
        "SetLargeVariable: More than %d variables are needed to store the data, which exceeds the maximum supported\n",
        MAX_VARIABLE_SPLIT
        ));
      Status = EFI_OUT_OF_RESOURCES;
      goto Done;
    }

    //
    // Check that there is enough space remaining in the UEFI Variable Services
    // Non-Volatile storage to store the data.
    //
    RemainingVariableStorage = GetRemainingVariableStorageSpace ();
    if (DataSize > RemainingVariableStorage) {
      DEBUG ((DEBUG_ERROR, "SetLargeVariable: Not enough NV storage space to store the data\n"));
      Status = EFI_OUT_OF_RESOURCES;
      goto Done;
    }

    DEBUG ((DEBUG_VERBOSE, "SetLargeVariable: Saving using multiple variables.\n"));
    OffsetPtr         = (UINT8 *) Data;
    BytesRemaining    = DataSize;
    VariablesSaved    = 0;

    //
    // Store chunks of data in UEFI variables until all data is stored
    //
    for (Index = 0; (Index < MAX_VARIABLE_SPLIT) && (BytesRemaining > 0); Index++) {
      ZeroMem (TempVariableName, MAX_VARIABLE_NAME_SIZE);
      UnicodeSPrint (TempVariableName, MAX_VARIABLE_NAME_SIZE, L"%s%d", VariableName, Index);

      SizeToSave          = 0;
      VariableNameLength  = StrLen (TempVariableName);
      VariableSplitSize   = GetVariableSplitSize (VariableNameLength);
      if (VariableSplitSize == 0) {
        DEBUG ((DEBUG_ERROR, "Unable to save variable, out of NV storage space\n"));
        Status = EFI_OUT_OF_RESOURCES;
        goto Done;
      }

      if (BytesRemaining > VariableSplitSize) {
        SizeToSave = (UINTN) VariableSplitSize;
      } else {
        SizeToSave = BytesRemaining;
      }
      DEBUG ((DEBUG_INFO, "Saving %s, Guid = %g, Size %d\n", TempVariableName, VendorGuid, SizeToSave));
      Status = VarLibSetVariable (
                TempVariableName,
                VendorGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                SizeToSave,
                (VOID *) OffsetPtr
                );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "SetLargeVariable: Error writting variable: Status = %r\n", Status));
        goto Done;
      }
      VariablesSaved  = Index;
      BytesRemaining -= SizeToSave;
      OffsetPtr += SizeToSave;
    }   // End of for loop

    //
    // If the user requested that the variables be locked, lock them now that
    // all data is saved.
    //
    if (LockVariable) {
      for (Index = 0; Index < VariablesSaved; Index++) {
        ZeroMem (TempVariableName, MAX_VARIABLE_NAME_SIZE);
        UnicodeSPrint (TempVariableName, MAX_VARIABLE_NAME_SIZE, L"%s%d", VariableName, Index);

        DEBUG ((DEBUG_INFO, "Locking %s, Guid = %g\n", TempVariableName, VendorGuid));
        Status = VarLibVariableRequestToLock (TempVariableName, VendorGuid);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "SetLargeVariable: Error locking variable: Status = %r\n", Status));
          VariablesSaved = 0;
          goto Done;
        }
      }
    }
  }

Done:
  if (EFI_ERROR (Status) && VariablesSaved > 0) {
    DEBUG ((DEBUG_ERROR, "SetLargeVariable: An error was encountered, deleting variables with partially stored data\n"));
    for (Index = 0; Index < VariablesSaved; Index++) {
      ZeroMem (TempVariableName, MAX_VARIABLE_NAME_SIZE);
      UnicodeSPrint (TempVariableName, MAX_VARIABLE_NAME_SIZE, L"%s%d", VariableName, Index);

      DEBUG ((DEBUG_INFO, "Deleting %s, Guid = %g\n", TempVariableName, VendorGuid));
      Status2 = VarLibSetVariable (
                  TempVariableName,
                  VendorGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  0,
                  NULL
                  );
    DEBUG ((DEBUG_ERROR, "SetLargeVariable: Error deleting variable: Status = %r\n", Status2));
    }
  }
  DEBUG ((DEBUG_ERROR, "SetLargeVariable: Status = %r\n", Status));
  return Status;
}
