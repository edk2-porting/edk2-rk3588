/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/ReadOnlyVariable2.h>

/**
  Returns the status whether get the variable success. The function retrieves 
  variable  through the ReadOnlyVariable2 PPI GetVariable().  The 
  returned buffer is allocated using AllocatePool().  The caller is responsible
  for freeing this buffer with FreePool().

  If Name  is NULL, then ASSERT().
  If Guid  is NULL, then ASSERT().
  If Value is NULL, then ASSERT().

  @param[in]  Name  The pointer to a Null-terminated Unicode string.
  @param[in]  Guid  The pointer to an EFI_GUID structure
  @param[out] Value The buffer point saved the variable info.
  @param[out] Size  The buffer size of the variable.

  @return EFI_OUT_OF_RESOURCES      Allocate buffer failed.
  @return EFI_SUCCESS               Find the specified variable.
  @return Others Errors             Return errors from call to gRT->GetVariable.

**/
EFI_STATUS
EFIAPI
PeiGetVariable (
  IN CONST CHAR16    *Name,
  IN CONST EFI_GUID  *Guid,
  OUT VOID           **Value,
  OUT UINTN          *Size  OPTIONAL
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VariableSize;
  VOID                              *VariableData;

  ASSERT (Name != NULL);
  ASSERT (Guid != NULL);
  ASSERT (Value != NULL);

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&VariableServices
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return EFI_NOT_READY;
  }

  VariableSize = 0;
  VariableData = NULL;
  Status = VariableServices->GetVariable (
                               VariableServices,
                               Name,
                               Guid,
                               NULL,
                               &VariableSize,
                               VariableData
                               );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }

  if ((Size == NULL) || (*Size == 0) || (*Value == NULL)) {
    VariableData = AllocatePool (VariableSize);
    ASSERT (VariableData != NULL);
    if (VariableData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    if (*Size < VariableSize) {
      return EFI_BUFFER_TOO_SMALL;
    }
    VariableData = *Value;
  }

  Status = VariableServices->GetVariable (
                               VariableServices,
                               Name,
                               Guid,
                               NULL,
                               &VariableSize,
                               VariableData
                               );
  if (EFI_ERROR (Status)) {
    FreePool (VariableData);
    VariableData = NULL;
    VariableSize = 0;
  }

  *Value = VariableData;
  if (Size != NULL) {
    *Size = VariableSize;
  }
  return Status;
}

EFI_PEI_FILE_HANDLE
InternalGetFfsHandleFromAnyFv (
  IN CONST  EFI_GUID           *NameGuid
  )
{
  EFI_STATUS                 Status;
  UINTN                      FvInstance;
  EFI_PEI_FV_HANDLE          FvHandle;
  EFI_PEI_FILE_HANDLE        FfsHandle;

  FvInstance  = 0;
  FvHandle    = NULL;
  FfsHandle   = NULL;
  while (TRUE) {
    Status = PeiServicesFfsFindNextVolume (FvInstance, &FvHandle);
    if (EFI_ERROR(Status)) {
      break;
    }
    Status = PeiServicesFfsFindFileByName (NameGuid, FvHandle, &FfsHandle);
    if (Status == EFI_SUCCESS) {
      break;
    }
    FfsHandle = NULL;
    FvInstance ++;
  }

  return FfsHandle;
}

/**
  Finds the file in any FV and gets file Address and Size
  
  @param[in]  NameGuid             File GUID
  @param[out] Address              Pointer to the File Address
  @param[out] Size                 Pointer to File Size

  @retval EFI_SUCCESS                Successfull in reading the file from any FV
**/
EFI_STATUS
EFIAPI
PeiGetFfsFromAnyFv (
  IN CONST  EFI_GUID           *NameGuid,
  OUT       VOID               **Address,
  OUT       UINTN              *Size
  )
{
  EFI_STATUS                 Status;
  EFI_FV_FILE_INFO           FvFileInfo;
  EFI_PEI_FILE_HANDLE        FfsHandle;

  FfsHandle = InternalGetFfsHandleFromAnyFv (NameGuid);
  if (FfsHandle == NULL) {
    return EFI_NOT_FOUND;
  }
  
  //
  // Need get size
  //
  Status = PeiServicesFfsGetFileInfo (FfsHandle, &FvFileInfo);
  if (EFI_ERROR(Status)) {
    return EFI_NOT_FOUND;
  }
  *Address = FvFileInfo.Buffer;
  *Size    = FvFileInfo.BufferSize;

  return Status;
}

/**
  Get Section buffer pointer by SectionType and SectionInstance.

  @param[in]   SectionBuffer     The buffer of section
  @param[in]   SectionBufferSize The size of SectionBuffer in bytes
  @param[in]   SectionType       The SectionType of Section to be found
  @param[in]   SectionInstance   The Instance of Section to be found
  @param[out]  OutSectionBuffer  The section found, including SECTION_HEADER
  @param[out]  OutSectionSize    The size of section found, including SECTION_HEADER
  
  @retval EFI_SUCCESS                Successfull in reading the section from FV
**/
EFI_STATUS
InternalGetSectionByType (
  IN VOID                  *SectionBuffer,
  IN UINTN                 SectionBufferSize,
  IN EFI_SECTION_TYPE      SectionType,
  IN UINTN                 SectionInstance,
  OUT VOID                 **OutSectionBuffer,
  OUT UINTN                *OutSectionSize
  )
{
  EFI_COMMON_SECTION_HEADER             *SectionHeader;
  UINTN                                 SectionSize;
  UINTN                                 Instance;

  //
  // Find Section
  //
  SectionHeader = SectionBuffer;

  Instance = 0;
  while ((UINTN)SectionHeader < (UINTN)SectionBuffer + SectionBufferSize) {
    if (IS_SECTION2(SectionHeader)) {
      SectionSize = SECTION2_SIZE(SectionHeader);
    } else {
      SectionSize = SECTION_SIZE(SectionHeader);
    }

    if (SectionHeader->Type == SectionType) {
      if (Instance == SectionInstance) {
        *OutSectionBuffer = (UINT8 *)SectionHeader;
        *OutSectionSize = SectionSize;
        return EFI_SUCCESS;
      } else {
        Instance++;
      }
    } else {
      //
      // Skip other section type
      //
    }

    //
    // Next Section
    //
    SectionHeader = (EFI_COMMON_SECTION_HEADER *)((UINTN)SectionHeader + ALIGN_VALUE(SectionSize, 4));
  }

  return EFI_NOT_FOUND;
}

/**
  Finds the section in any FV and gets section Address and Size

  @param[in]  NameGuid             File GUID
  @param[in]  SectionType          The SectionType of Section to be found
  @param[in]  SectionInstance      The Instance of Section to be found
  @param[out] Address              Pointer to the section Address
  @param[out] Size                 Pointer to section Size

  @retval EFI_SUCCESS                Successfull in reading the section from any FV
**/
EFI_STATUS
EFIAPI
PeiGetSectionFromAnyFv  (
  IN CONST  EFI_GUID           *NameGuid,
  IN        EFI_SECTION_TYPE   SectionType,
  IN        UINTN              SectionInstance,
  OUT       VOID               **Address,
  OUT       UINTN              *Size
  )
{
  EFI_STATUS                 Status;
  EFI_COMMON_SECTION_HEADER  *Section;
  VOID                       *FileBuffer;
  UINTN                      FileBufferSize;
  
  Status = PeiGetFfsFromAnyFv (NameGuid, &FileBuffer, &FileBufferSize);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = InternalGetSectionByType (FileBuffer, FileBufferSize, SectionType, SectionInstance, Address, Size);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Section = *Address;
  if (IS_SECTION2(Section)) {
    ASSERT(SECTION2_SIZE(Section) > 0x00FFFFFF);
    *Size = SECTION2_SIZE(Section) - sizeof (EFI_COMMON_SECTION_HEADER2);
    *Address = (UINT8 *)*Address + sizeof (EFI_COMMON_SECTION_HEADER2);
  } else {
    *Size = SECTION_SIZE(Section) - sizeof (EFI_COMMON_SECTION_HEADER);
    *Address = (UINT8 *)*Address + sizeof (EFI_COMMON_SECTION_HEADER);
  }
  
  return EFI_SUCCESS;
}
