/** @file
  Library functions for SetupLib.
  This library instance provides methods to access Setup option.

  @copyright
  Copyright 2019 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <SetupLibInternal.h>
#include <Library/IoLib.h>

/**
  Gets the data and size of a variable.

  Read the EFI variable (VendorGuid/Name) and return a dynamically allocated
  buffer, and the size of the buffer. If failure return NULL.

  @param  Name                     String part of EFI variable name
  @param  VendorGuid               GUID part of EFI variable name
  @param  VariableSize             Returns the size of the EFI variable that was
                                   read

  @return Dynamically allocated memory that contains a copy of the EFI variable.
          Caller is responsible freeing the buffer via FreePages.

  @retval NULL                     Variable was not read

**/
VOID *
EFIAPI
GetVariableAndSize (
  IN  CHAR16              *Name,
  IN  EFI_GUID            *Guid,
  IN  UINTN               BufferSize
  )
{
  EFI_STATUS                            Status;
  VOID                                  *Buffer = NULL;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  CONST EFI_PEI_SERVICES                **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();
  (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  Buffer = AllocatePages (EFI_SIZE_TO_PAGES (BufferSize));
  if (Buffer == NULL) {
    return NULL;
  }
  //
  // Read variable into the allocated buffer.
  //
  Status = PeiVariable->GetVariable (PeiVariable, Name, Guid, NULL, &BufferSize, Buffer);
  ASSERT_EFI_ERROR (Status);

  return Buffer;
}

/**
  Retrieves the specified group space.

  @param[in]  Guid             Pointer to a 128-bit unique value that designates which namespace
                               to set a value from.
  @reture GroupInfo            The found group space. NULL will return if not found.
**/
UINTN
InternalGetGroupInfo  (
  IN EFI_GUID   *Guid
  )
{
  UINTN          Index;

  if (Guid == NULL) {
    return MAX_ADDRESS;
  }

  //
  // Find the matched GUID space.
  //
  for (Index = 0; mSetupInfo[Index].GuidValue != NULL; Index ++) {
    if (CompareGuid (mSetupInfo[Index].GuidValue, Guid)) {
      break;
    }
  }

  //
  // No matched GUID space
  //
  if (mSetupInfo[Index].GuidValue == NULL) {
    return MAX_ADDRESS;
  }

  return Index;
}

/**
  This function provides a means by which to retrieve a value for a given option.

  Returns the data, data type and data size specified by OptionNumber and Guid.

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
  IN EFI_GUID *Guid,
  IN UINTN    OptionNumber,
  IN OUT VOID *Data,
  IN UINTN    DataSize
  )
{
  UINTN       GroupIndex;
  VOID        *Variable = NULL;

  if (Guid == NULL || DataSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  GroupIndex = InternalGetGroupInfo (Guid);
  if (GroupIndex == MAX_ADDRESS) {
    return EFI_NOT_FOUND;
  }

  Variable = GetVariableAndSize (
               mSetupInfo[GroupIndex].SetupName,
               Guid,
               mSetupInfo[GroupIndex].VariableSize
               );
  if (Variable == NULL) {
    return EFI_NOT_FOUND;
  }

  CopyMem (Data, (UINT8 *)Variable + OptionNumber, DataSize);

  return EFI_SUCCESS;
}

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
  )
{
  VOID        *Variable;
  UINTN       Index;
  UINT8       *SetupDataPtr;

  if (SetupData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SetupDataPtr = (UINT8 *)SetupData;
  ZeroMem (SetupDataPtr, sizeof (SETUP_DATA));

  for (Index = 0; mSetupInfo[Index].GuidValue != NULL; Index ++) {

    Variable = NULL;
    Variable = GetVariableAndSize (
                 mSetupInfo[Index].SetupName,
                 mSetupInfo[Index].GuidValue,
                 mSetupInfo[Index].VariableSize
                 );
    ASSERT (Variable != NULL);
    if (Variable == NULL) {
      return EFI_NOT_FOUND;
    }

    CopyMem (SetupDataPtr, Variable, mSetupInfo[Index].VariableSize);
    SetupDataPtr = SetupDataPtr + mSetupInfo[Index].VariableSize;

    FreePages (Variable, EFI_SIZE_TO_PAGES (mSetupInfo[Index].VariableSize));
  }

  return EFI_SUCCESS;
}

/**
  This function provides a means by which to retrieve a value for a given option.

  Returns the data, data type and data size specified by OptionNumber and Guid.

  @param[in]  Guid          Pointer to a 128-bit unique value that designates
                            which namespace to retrieve a value from.
  @param[in]  Variable      Pointer to data location where variable is stored.

  @retval EFI_SUCCESS           Data is successfully reterieved.
  @retval EFI_INVALID_PARAMETER Guid is NULL or DataSize is NULL or OptionNumber is invalid.
  @retval EFI_BUFFER_TOO_SMALL  Input data buffer is not enough.
  @retval EFI_NOT_FOUND         The given option is not found.

**/
EFI_STATUS
EFIAPI
GetSpecificConfigGuid (
  IN EFI_GUID            *Guid,
  OUT VOID               *Variable
  )
{
  EFI_STATUS  Status;
  UINTN       GroupIndex;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  CONST EFI_PEI_SERVICES                **PeiServices;

  if ((Guid == NULL) || (Variable == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  GroupIndex = InternalGetGroupInfo (Guid);
  if (GroupIndex == MAX_ADDRESS) {
    return EFI_NOT_FOUND;
  }
  PeiServices = GetPeiServicesTablePointer ();
  (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  Status = PeiVariable->GetVariable (PeiVariable, mSetupInfo[GroupIndex].SetupName, mSetupInfo[GroupIndex].GuidValue, NULL, &mSetupInfo[GroupIndex].VariableSize, Variable);

  return Status;
}

