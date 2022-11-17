/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobVariableLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Ppi/MemoryDiscovered.h>
#include "Variable.h"
#include "Fce.h"

/**

  Gets the pointer to the first variable header in given variable store area.

  @param VarStoreHeader  Pointer to the Variable Store Header.

  @return Pointer to the first variable header

**/
STATIC
AUTHENTICATED_VARIABLE_HEADER *
GetStartPointer (
  IN VARIABLE_STORE_HEADER       *VarStoreHeader
  )
{
  //
  // The end of variable store
  //
  return (AUTHENTICATED_VARIABLE_HEADER *) HEADER_ALIGN (VarStoreHeader + 1);
}


/**
  This code gets the pointer to the last variable memory pointer byte.

  @param  VarStoreHeader  Pointer to the Variable Store Header.

  @return AUTHENTICATED_VARIABLE_HEADER* pointer to last unavailable Variable Header.

**/
STATIC
AUTHENTICATED_VARIABLE_HEADER *
GetEndPointer (
  IN VARIABLE_STORE_HEADER       *VarStoreHeader
  )
{
  //
  // The end of variable store
  //
  return (AUTHENTICATED_VARIABLE_HEADER *) HEADER_ALIGN ((UINTN) VarStoreHeader + VarStoreHeader->Size);
}


/**
  This code checks if variable header is valid or not.

  @param  Variable  Pointer to the Variable Header.

  @retval TRUE      Variable header is valid.
  @retval FALSE     Variable header is not valid.

**/
STATIC
BOOLEAN
IsValidVariableHeader (
  IN  AUTHENTICATED_VARIABLE_HEADER   *Variable
  )
{
  if (Variable == NULL || Variable->StartId != VARIABLE_DATA ) {
    return FALSE;
  }

  return TRUE;
}

/**
  This code gets the size of name of variable.

  @param  Variable  Pointer to the Variable Header.
  @param  AuthFlag  Authenticated variable flag.

  @return Size of variable in bytes in type UINTN.

**/
STATIC
UINTN
NameSizeOfVariable (
  IN  AUTHENTICATED_VARIABLE_HEADER     *AuthVariable,
  IN  BOOLEAN                           AuthFlag
  )
{
  VARIABLE_HEADER *Variable;

  Variable = (VARIABLE_HEADER *) AuthVariable;
  if (AuthFlag) {
    if (AuthVariable->State == (UINT8) (-1) ||
       AuthVariable->DataSize == (UINT32) (-1) ||
       AuthVariable->NameSize == (UINT32) (-1) ||
       AuthVariable->Attributes == (UINT32) (-1)) {
      return 0;
    }
    return (UINTN) AuthVariable->NameSize;
  } else {
    if (Variable->State == (UINT8) (-1) ||
       Variable->DataSize == (UINT32) (-1) ||
       Variable->NameSize == (UINT32) (-1) ||
       Variable->Attributes == (UINT32) (-1)) {
      return 0;
    }
    return (UINTN) Variable->NameSize;
  }
}


/**
  This code gets the size of data of variable.

  @param  Variable  Pointer to the Variable Header.
  @param  AuthFlag  Authenticated variable flag.

  @return Size of variable in bytes in type UINTN.

**/
STATIC
UINTN
DataSizeOfVariable (
  IN  AUTHENTICATED_VARIABLE_HEADER     *AuthVariable,
  IN  BOOLEAN                           AuthFlag
  )
{
  VARIABLE_HEADER *Variable;

  Variable = (VARIABLE_HEADER *) AuthVariable;
  if (AuthFlag) {
    if (AuthVariable->State == (UINT8) (-1) ||
       AuthVariable->DataSize == (UINT32) (-1) ||
       AuthVariable->NameSize == (UINT32) (-1) ||
       AuthVariable->Attributes == (UINT32) (-1)) {
      return 0;
    }
    return (UINTN) AuthVariable->DataSize;
  } else {
    if (Variable->State == (UINT8) (-1) ||
       Variable->DataSize == (UINT32) (-1) ||
       Variable->NameSize == (UINT32) (-1) ||
       Variable->Attributes == (UINT32) (-1)) {
      return 0;
    }
    return (UINTN) Variable->DataSize;
  }
}

/**
  This code gets the size of variable header.

  @param AuthFlag   Authenticated variable flag.

  @return Size of variable header in bytes in type UINTN.

**/
UINTN
GetVariableHeaderSize (
  IN  BOOLEAN       AuthFlag
  )
{
  UINTN Value;

  if (AuthFlag) {
    Value = sizeof (AUTHENTICATED_VARIABLE_HEADER);
  } else {
    Value = sizeof (VARIABLE_HEADER);
  }

  return Value;
}


/**
  This code gets the pointer to the variable name.

  @param   Variable  Pointer to the Variable Header.
  @param   AuthFlag  Authenticated variable flag.

  @return  A CHAR16* pointer to Variable Name.

**/
STATIC
CHAR16 *
GetVariableNamePtr (
  IN  AUTHENTICATED_VARIABLE_HEADER     *Variable,
  IN BOOLEAN                            AuthFlag
  )
{
  return (CHAR16 *) ((UINTN) Variable + GetVariableHeaderSize (AuthFlag));
}


/**
  This code gets the pointer to the variable guid.

  @param Variable   Pointer to the Variable Header.
  @param AuthFlag   Authenticated variable flag.

  @return A EFI_GUID* pointer to Vendor Guid.

**/
EFI_GUID *
GetVendorGuidPtr (
  IN  AUTHENTICATED_VARIABLE_HEADER     *AuthVariable,
  IN  BOOLEAN                           AuthFlag
  )
{
  VARIABLE_HEADER *Variable;

  Variable = (VARIABLE_HEADER *) AuthVariable;
  if (AuthFlag) {
    return &AuthVariable->VendorGuid;
  } else {
    return &Variable->VendorGuid;
  }
}


/**
  This code gets the pointer to the variable data.

  @param   Variable  Pointer to the Variable Header.
  @param   AuthFlag  Authenticated variable flag.

  @return  A UINT8* pointer to Variable Data.

**/
STATIC
UINT8 *
GetVariableDataPtr (
  IN  AUTHENTICATED_VARIABLE_HEADER     *Variable,
  IN  BOOLEAN                           AuthFlag
  )
{
  UINTN Value;

  //
  // Be careful about pad size for alignment
  //
  Value =  (UINTN) GetVariableNamePtr (Variable, AuthFlag);
  Value += NameSizeOfVariable (Variable, AuthFlag);
  Value += GET_PAD_SIZE (NameSizeOfVariable (Variable, AuthFlag));

  return (UINT8 *) Value;
}


/**
  This code gets the pointer to the next variable header.

  @param  Variable  Pointer to the Variable Header.
  @param  AuthFlag  Authenticated variable flag.

  @return  A AUTHENTICATED_VARIABLE_HEADER* pointer to next variable header.

**/
STATIC
AUTHENTICATED_VARIABLE_HEADER *
GetNextVariablePtr (
  IN  AUTHENTICATED_VARIABLE_HEADER     *Variable,
  IN  BOOLEAN                           AuthFlag
  )
{
  UINTN Value;

  if (!IsValidVariableHeader (Variable)) {
    return NULL;
  }

  Value =  (UINTN) GetVariableDataPtr (Variable, AuthFlag);
  Value += DataSizeOfVariable (Variable, AuthFlag);
  Value += GET_PAD_SIZE (DataSizeOfVariable (Variable, AuthFlag));

  //
  // Be careful about pad size for alignment
  //
  return (AUTHENTICATED_VARIABLE_HEADER *) HEADER_ALIGN (Value);
}

EFI_STATUS
EFIAPI
BuildDefaultDataHobForRecoveryVariable (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
/*++

Routine Description:

  Convert Authenticated variable to normal variable data.

Arguments:

  PeiServices       General purpose services available to every PEIM.
  NotifyDescriptor  Notify that this module published.
  Ppi               PPI that was installed.
    
Returns:

  EFI_SUCCESS     The function completed successfully.

--*/
{
  EFI_HOB_GUID_TYPE          *GuidHob;
  VARIABLE_STORE_HEADER      *AuthVarStoreHeader;
  VARIABLE_STORE_HEADER      *VarStoreHeader;
  UINT32                     VarStoreSize;
  AUTHENTICATED_VARIABLE_HEADER *AuthStartPtr;
  AUTHENTICATED_VARIABLE_HEADER *AuthEndPtr;
  AUTHENTICATED_VARIABLE_HEADER *AuthVariable;
  VARIABLE_HEADER               *Variable;
  UINT8                         *AuthVariablePtr;
  UINT8                         *VariablePtr;
  
  GuidHob = GetFirstGuidHob (&gEfiAuthenticatedVariableGuid);
  AuthVarStoreHeader = (VARIABLE_STORE_HEADER *) GET_GUID_HOB_DATA (GuidHob);
  //
  // Go through AuthVarStore to calculate the required size for normal varstore.
  //
  VarStoreSize = sizeof (VARIABLE_STORE_HEADER);
  AuthStartPtr = GetStartPointer (AuthVarStoreHeader);
  AuthEndPtr   = GetEndPointer (AuthVarStoreHeader);
  AuthVariable = AuthStartPtr;
  while ((AuthVariable < AuthEndPtr) && IsValidVariableHeader (AuthVariable)) {
    if (AuthVariable->State == VAR_ADDED) {
      VarStoreSize = HEADER_ALIGN (VarStoreSize);
      VarStoreSize += sizeof (VARIABLE_HEADER);
      VarStoreSize += AuthVariable->NameSize + GET_PAD_SIZE (AuthVariable->NameSize);
      VarStoreSize += AuthVariable->DataSize + GET_PAD_SIZE (AuthVariable->DataSize);
    }
    AuthVariable = GetNextVariablePtr (AuthVariable, TRUE);
  }

  //
  // Create HOB data for normal variable storage.
  // Allocate more data for header alignment. 
  //
  VarStoreSize   = VarStoreSize + HEADER_ALIGNMENT - 1;
  VarStoreHeader = (VARIABLE_STORE_HEADER *) BuildGuidHob (&gEfiVariableGuid, VarStoreSize);
  ASSERT (VarStoreHeader != NULL);
  if (VarStoreHeader == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyGuid (&VarStoreHeader->Signature, &gEfiVariableGuid);
  VarStoreHeader->Format = AuthVarStoreHeader->Format;
  VarStoreHeader->State  = AuthVarStoreHeader->State;
  
  //
  // Copy variable data from AuthVarStore to NormalVarStore
  //
  AuthVariable = AuthStartPtr;
  VariablePtr  = (UINT8 *) (VarStoreHeader + 1);
  while ((AuthVariable < AuthEndPtr) && IsValidVariableHeader (AuthVariable)) {
    if (AuthVariable->State == VAR_ADDED) {
      Variable = (VARIABLE_HEADER *) HEADER_ALIGN ((UINTN) VariablePtr);
      //
      // Copy variable header
      //
      Variable->StartId    = AuthVariable->StartId;
      Variable->State      = AuthVariable->State;
      Variable->Reserved   = AuthVariable->Reserved;
      Variable->Attributes = AuthVariable->Attributes;
      Variable->NameSize   = AuthVariable->NameSize;
      Variable->DataSize   = AuthVariable->DataSize;
      CopyGuid (&Variable->VendorGuid, &AuthVariable->VendorGuid);
      //
      // Copy variable Name and Data
      //
      VariablePtr     = (UINT8 *) (Variable + 1);
      AuthVariablePtr = (UINT8 *) (AuthVariable + 1);
      CopyMem (VariablePtr, AuthVariablePtr, Variable->NameSize);
      VariablePtr     = VariablePtr + Variable->NameSize + GET_PAD_SIZE (Variable->NameSize);
      AuthVariablePtr = AuthVariablePtr + AuthVariable->NameSize + GET_PAD_SIZE (AuthVariable->NameSize);
      CopyMem (VariablePtr, AuthVariablePtr, Variable->DataSize);
      VariablePtr     = VariablePtr + Variable->DataSize + GET_PAD_SIZE (Variable->DataSize);
    }
    AuthVariable = GetNextVariablePtr (AuthVariable, TRUE);
  }
  
  //
  // Update Variable Storage Size
  //
  VarStoreHeader->Size = (UINT32) ((UINTN) VariablePtr - (UINTN) VarStoreHeader);
  
  return EFI_SUCCESS;
}

EFI_PEI_NOTIFY_DESCRIPTOR mMemoryNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  BuildDefaultDataHobForRecoveryVariable 
};

/**
  Find variable from default variable HOB.

  @param[in]  VariableName      A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in]  VendorGuid        A unique identifier for the vendor.
  @param[out] AuthFlag          Pointer to output Authenticated variable flag.

  @return Pointer to variable header, NULL if not found.

**/
AUTHENTICATED_VARIABLE_HEADER *
FindVariableFromHob (
  IN CHAR16                     *VariableName,
  IN EFI_GUID                   *VendorGuid,
  OUT BOOLEAN                   *AuthFlag
  )
{
  EFI_HOB_GUID_TYPE             *GuidHob;
  VARIABLE_STORE_HEADER         *VariableStoreHeader;
  AUTHENTICATED_VARIABLE_HEADER *StartPtr;
  AUTHENTICATED_VARIABLE_HEADER *EndPtr;
  AUTHENTICATED_VARIABLE_HEADER *CurrPtr;
  VOID                          *Point;

  VariableStoreHeader = NULL;

  GuidHob = GetFirstGuidHob (&gEfiAuthenticatedVariableGuid);
  if (GuidHob != NULL) {
    VariableStoreHeader = (VARIABLE_STORE_HEADER *) GET_GUID_HOB_DATA (GuidHob);
    *AuthFlag = TRUE;
  } else {
    GuidHob = GetFirstGuidHob (&gEfiVariableGuid);
    if (GuidHob != NULL) {
      VariableStoreHeader = (VARIABLE_STORE_HEADER *) GET_GUID_HOB_DATA (GuidHob);
      *AuthFlag = FALSE;
    }
  }
  ASSERT (VariableStoreHeader != NULL);
  if (VariableStoreHeader == NULL) {
    return NULL;
  }

  StartPtr = GetStartPointer (VariableStoreHeader);
  EndPtr   = GetEndPointer (VariableStoreHeader);
  for ( CurrPtr = StartPtr
      ; (CurrPtr < EndPtr) && IsValidVariableHeader (CurrPtr)
      ; CurrPtr = GetNextVariablePtr (CurrPtr, *AuthFlag)
      ) {
    if (CurrPtr->State == VAR_ADDED) {
      if (CompareGuid (VendorGuid, GetVendorGuidPtr (CurrPtr, *AuthFlag))) {
        Point = (VOID *) GetVariableNamePtr (CurrPtr, *AuthFlag);

        ASSERT (NameSizeOfVariable (CurrPtr, *AuthFlag) != 0);
        if (CompareMem (VariableName, Point, NameSizeOfVariable (CurrPtr, *AuthFlag)) == 0) {
          return CurrPtr;
        }
      }
    }
  }

  return NULL;
}

/**
  Get variable from default variable HOB.

  @param[in]      VariableName  A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in]      VendorGuid    A unique identifier for the vendor.
  @param[out]     Attributes    If not NULL, a pointer to the memory location to return the
                                attributes bitmask for the variable.
  @param[in, out] DataSize      On input, the size in bytes of the return Data buffer.
                                On output the size of data returned in Data.
  @param[out]     Data          The buffer to return the contents of the variable.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL  The DataSize is too small for the result.
  @retval EFI_INVALID_PARAMETER VariableName is NULL.
  @retval EFI_INVALID_PARAMETER VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER DataSize is NULL.
  @retval EFI_INVALID_PARAMETER The DataSize is not too small and Data is NULL.

**/
EFI_STATUS
EFIAPI
GetVariableFromHob (
  IN     CHAR16                 *VariableName,
  IN     EFI_GUID               *VendorGuid,
  OUT    UINT32                 *Attributes, OPTIONAL
  IN OUT UINTN                  *DataSize,
  OUT    VOID                   *Data
  )
{
  BOOLEAN                       AuthFlag;
  AUTHENTICATED_VARIABLE_HEADER *Variable;
  UINTN                         VarDataSize;

  if ((VariableName == NULL) || (VendorGuid == NULL) || (DataSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Variable = FindVariableFromHob (VariableName, VendorGuid, &AuthFlag);
  if (Variable == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Get data size
  //
  VarDataSize = DataSizeOfVariable (Variable, AuthFlag);
  if (*DataSize >= VarDataSize) {
    if (Data == NULL) {
      return EFI_INVALID_PARAMETER;
    }

    CopyMem (Data, GetVariableDataPtr (Variable, AuthFlag), VarDataSize);

    if (Attributes != NULL) {
      *Attributes = Variable->Attributes;
    }

    *DataSize = VarDataSize;
    return EFI_SUCCESS;
  } else {
    *DataSize = VarDataSize;
    return EFI_BUFFER_TOO_SMALL;
  }
}

/**
  Set variable to default variable HOB.

  @param[in] VariableName       A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in] VendorGuid         A unique identifier for the vendor.
  @param[in] Attributes         If not NULL, a pointer to the memory location to set the
                                attributes bitmask for the variable.
  @param[in] DataSize           On input, the size in bytes of the return Data buffer.
                                On output the size of data returned in Data.
  @param[in] Data               The buffer to return the contents of the variable.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_INVALID_PARAMETER VariableName is NULL.
  @retval EFI_INVALID_PARAMETER VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER Attributes is not NULL, but attributes value is 0.
  @retval EFI_INVALID_PARAMETER DataSize is not equal to the variable data size.
  @retval EFI_INVALID_PARAMETER The DataSize is equal to the variable data size, but Data is NULL.

**/
EFI_STATUS
EFIAPI
SetVariableToHob (
  IN CHAR16                     *VariableName,
  IN EFI_GUID                   *VendorGuid,
  IN UINT32                     *Attributes, OPTIONAL
  IN UINTN                      DataSize,
  IN VOID                       *Data
  )
{
  BOOLEAN                       AuthFlag;
  AUTHENTICATED_VARIABLE_HEADER *Variable;
  UINTN                         VarDataSize;

  if (VariableName == NULL || VariableName[0] == 0 || VendorGuid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Variable = FindVariableFromHob (VariableName, VendorGuid, &AuthFlag);
  if (Variable == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Get data size
  //
  VarDataSize = DataSizeOfVariable (Variable, AuthFlag);
  if (DataSize == VarDataSize) {
    if (Data == NULL) {
      return EFI_INVALID_PARAMETER;
    }

    if (Attributes != NULL) {
      if (*Attributes == 0) {
        return EFI_INVALID_PARAMETER;
      }
      Variable->Attributes = *Attributes;
    }

    CopyMem (GetVariableDataPtr (Variable, AuthFlag), Data, VarDataSize);

    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
}

/**
  This function finds the matched data and create GUID hob for it. 

  @retval EFI_SUCCESS           The matched default data is found.
  @retval EFI_NOT_FOUND         The matched default data is not found.
  @retval EFI_OUT_OF_RESOURCES  No enough resource to create HOB.

**/
EFI_STATUS
EFIAPI
CreateVariableHob (
  VOID
  )
{
  UINT16  StoreId;
  UINT16  SkuId;

  StoreId = EFI_HII_DEFAULT_CLASS_STANDARD; // BUGBUG: Should get from PCD
  SkuId = (UINT16)LibPcdGetSku ();
  return CreateDefaultVariableHob (StoreId, SkuId);
}