/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobVariableLib.h>
#include <Library/HobLib.h>
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

  @return Size of variable in bytes in type UINTN.

**/
STATIC
UINTN
NameSizeOfVariable (
  IN  AUTHENTICATED_VARIABLE_HEADER   *Variable
  )
{
  if (Variable->State    == (UINT8) (-1) ||
      Variable->DataSize == (UINT32) (-1) ||
      Variable->NameSize == (UINT32) (-1) ||
      Variable->Attributes == (UINT32) (-1)) {
    return 0;
  }
  return (UINTN) Variable->NameSize;
}


/**
  This code gets the size of data of variable.

  @param  Variable  Pointer to the Variable Header.

  @return Size of variable in bytes in type UINTN.

**/
STATIC
UINTN
DataSizeOfVariable (
  IN  AUTHENTICATED_VARIABLE_HEADER   *Variable
  )
{
  if (Variable->State    == (UINT8)  (-1) ||
      Variable->DataSize == (UINT32) (-1) ||
      Variable->NameSize == (UINT32) (-1) ||
      Variable->Attributes == (UINT32) (-1)) {
    return 0;
  }
  return (UINTN) Variable->DataSize;
}

/**
  This code gets the pointer to the variable name.

  @param   Variable  Pointer to the Variable Header.

  @return  A CHAR16* pointer to Variable Name.

**/
STATIC
CHAR16 *
GetVariableNamePtr (
  IN  AUTHENTICATED_VARIABLE_HEADER   *Variable
  )
{

  return (CHAR16 *) (Variable + 1);
}


/**
  This code gets the pointer to the variable data.

  @param   Variable  Pointer to the Variable Header.

  @return  A UINT8* pointer to Variable Data.

**/
STATIC
UINT8 *
GetVariableDataPtr (
  IN  AUTHENTICATED_VARIABLE_HEADER   *Variable
  )
{
  UINTN Value;

  //
  // Be careful about pad size for alignment
  //
  Value =  (UINTN) GetVariableNamePtr (Variable);
  Value += NameSizeOfVariable (Variable);
  Value += GET_PAD_SIZE (NameSizeOfVariable (Variable));

  return (UINT8 *) Value;
}


/**
  This code gets the pointer to the next variable header.

  @param  Variable  Pointer to the Variable Header.

  @return  A AUTHENTICATED_VARIABLE_HEADER* pointer to next variable header.

**/
STATIC
AUTHENTICATED_VARIABLE_HEADER *
GetNextVariablePtr (
  IN  AUTHENTICATED_VARIABLE_HEADER   *Variable
  )
{
  UINTN Value;

  if (!IsValidVariableHeader (Variable)) {
    return NULL;
  }

  Value =  (UINTN) GetVariableDataPtr (Variable);
  Value += DataSizeOfVariable (Variable);
  Value += GET_PAD_SIZE (DataSizeOfVariable (Variable));

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
    AuthVariable = GetNextVariablePtr (AuthVariable);
  }

  //
  // Create HOB data for normal variable storage.
  // Allocate more data for header alignment. 
  //
  VarStoreSize   = VarStoreSize + HEADER_ALIGNMENT - 1;
  VarStoreHeader = (VARIABLE_STORE_HEADER *) BuildGuidHob (&gEfiVariableGuid, VarStoreSize);
  ASSERT (VarStoreHeader != NULL);
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
    AuthVariable = GetNextVariablePtr (AuthVariable);
  }

  //
  // Update Variable Storage Size
  //
  VarStoreHeader->Size = (UINT32) ((UINTN) VariablePtr - (UINTN) VarStoreHeader);

  return EFI_SUCCESS;
}

static EFI_PEI_NOTIFY_DESCRIPTOR mMemoryNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  BuildDefaultDataHobForRecoveryVariable
};

/**
  This function finds the matched default data and create GUID hob for it. 
  
  @param StoreId  Specifies the type of defaults to retrieve.
  @param SkuId    Specifies the platform board of defaults to retrieve.

  @retval EFI_SUCCESS           The matched default data is found.
  @retval EFI_NOT_FOUND         The matched default data is not found.
  @retval EFI_OUT_OF_RESOURCES  No enough resource to create HOB.

**/
EFI_STATUS
EFIAPI
CreateDefaultVariableHob (
  IN UINT16  StoreId,
  IN UINT16  SkuId
  )
{
  UINTN                      FvInstance;
  EFI_FIRMWARE_VOLUME_HEADER *FvHeader;
  EFI_FFS_FILE_HEADER        *FfsHeader;
  UINT32                     FileSize;
  EFI_COMMON_SECTION_HEADER  *Section;
  UINT32                     SectionLength;
  EFI_STATUS                 Status;
  BOOLEAN                    DefaultFileIsFound;
  DEFAULT_DATA               *DefaultData;
  DEFAULT_INFO               *DefaultInfo;
  VARIABLE_STORE_HEADER      *VarStoreHeader;
  VARIABLE_STORE_HEADER      *VarStoreHeaderHob;
  UINT8                      *VarHobPtr;
  UINT8                      *VarPtr;
  UINT32                     VarDataOffset;
  UINT32                     VarHobDataOffset;
  EFI_BOOT_MODE              BootMode;
  CONST EFI_PEI_SERVICES     **PeiServices;

  //
  // Get PeiService pointer
  //
  PeiServices = GetPeiServicesTablePointer ();

  //
  // Find the FFS file that stores all default data.
  //
  DefaultFileIsFound = FALSE;
  FvInstance         = 0;
  FfsHeader          = NULL;
  while (((*PeiServices)->FfsFindNextVolume (PeiServices, FvInstance, (VOID **)&FvHeader) == EFI_SUCCESS) &&
         (!DefaultFileIsFound)) {
    FfsHeader = NULL;
    while ((*PeiServices)->FfsFindNextFile (PeiServices, EFI_FV_FILETYPE_FREEFORM, FvHeader, (VOID **)&FfsHeader) == EFI_SUCCESS) {
      if (CompareGuid ((EFI_GUID *) FfsHeader, &gDefaultDataFileGuid)) {
        DefaultFileIsFound = TRUE;
        break;
      }
    }
    FvInstance ++;
  }

  //
  // FFS file is not found.
  //
  if (!DefaultFileIsFound) {
    return EFI_NOT_FOUND;
  }

  //
  // Find the matched default data for the input default ID and plat ID.
  //
  VarStoreHeader = NULL;
  Section  = (EFI_COMMON_SECTION_HEADER *)(FfsHeader + 1);
  FileSize = *(UINT32 *)(FfsHeader->Size) & 0x00FFFFFF;
  while (((UINTN) Section < (UINTN) FfsHeader + FileSize) && (VarStoreHeader == NULL)) {
    DefaultData = (DEFAULT_DATA *) (Section + 1);
    DefaultInfo = &(DefaultData->DefaultInfo[0]);
    while ((UINTN) DefaultInfo < (UINTN) DefaultData + DefaultData->HeaderSize) {
      if (DefaultInfo->DefaultId == StoreId && DefaultInfo->BoardId == SkuId) {
        VarStoreHeader = (VARIABLE_STORE_HEADER *) ((UINT8 *) DefaultData + DefaultData->HeaderSize);
        break;
      }
      DefaultInfo ++;
    }
    //
    // Size is 24 bits wide so mask upper 8 bits. 
    // SectionLength is adjusted it is 4 byte aligned.
    // Go to the next section
    //
    SectionLength = *(UINT32 *)Section->Size & 0x00FFFFFF;
    SectionLength = (SectionLength + 3) & (~3);
    ASSERT (SectionLength != 0);
    Section = (EFI_COMMON_SECTION_HEADER *)((UINT8 *)Section + SectionLength);
  }
  //
  // Matched default data is not found.
  //
  if (VarStoreHeader == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Create HOB to store default data so that Variable driver can use it.
  // Allocate more data for header alignment.
  //
  VarStoreHeaderHob = (VARIABLE_STORE_HEADER *) BuildGuidHob (&VarStoreHeader->Signature, VarStoreHeader->Size + HEADER_ALIGNMENT - 1);
  if (VarStoreHeaderHob == NULL) {
    //
    // No enough hob resource.
    //
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Copy variable storage header.
  //
  CopyMem (VarStoreHeaderHob, VarStoreHeader, sizeof (VARIABLE_STORE_HEADER));
  //
  // Copy variable data.
  //
  VarPtr           = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeader + 1));
  VarDataOffset    = (UINT32) ((UINTN) VarPtr - (UINTN) VarStoreHeader);
  VarHobPtr        = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeaderHob + 1));
  VarHobDataOffset = (UINT32) ((UINTN) VarHobPtr - (UINTN) VarStoreHeaderHob);
  CopyMem (VarHobPtr, VarPtr, VarStoreHeader->Size - VarDataOffset);
  //
  // Update variable size.
  //
  VarStoreHeaderHob->Size = VarStoreHeader->Size - VarDataOffset + VarHobDataOffset;

  //
  // On recovery boot mode, emulation variable driver will be used.
  // But, Emulation variable only knows normal variable data format. 
  // So, if the default variable data format is authenticated, it needs to be converted to normal data.
  //
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  if (BootMode == BOOT_IN_RECOVERY_MODE && 
      CompareGuid (&VarStoreHeader->Signature, &gEfiAuthenticatedVariableGuid)) {
    Status = (**PeiServices).NotifyPpi (PeiServices, &mMemoryNotifyList);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
