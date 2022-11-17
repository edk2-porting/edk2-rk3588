/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_LIB_H_
#define _PEI_LIB_H_

#include <PiPei.h>

/**
  Returns the status whether get the variable success. The function retrieves 
  variable  through the ReadOnlyVariable2 PPI GetVariable().

  If the *Size is 0, the returned buffer is allocated using AllocatePool().
  The caller is responsible for freeing this buffer with FreePool().

  If the *Size is non-0, this function just uses caller allocated *Value.

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
  OUT UINTN          *Size
  );

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
  );

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
  );

#endif
