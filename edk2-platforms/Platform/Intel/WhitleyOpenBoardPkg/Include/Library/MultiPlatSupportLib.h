/** @file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MULTI_PLATFORM_SUPPORT_LIB_H_
#define _MULTI_PLATFORM_SUPPORT_LIB_H_

#define MAX_TEMP_BUFFER 0x5000
//#define MULTI_PLATFORM_DEBUG TRUE




/*++
Description:

  This function finds the matched default data and create GUID hob for it.

Arguments:

  DefaultId - Specifies the type of defaults to retrieve.
  BoardId   - Specifies the platform board of defaults to retrieve.

Returns:

  EFI_SUCCESS - The matched default data is found.
  EFI_NOT_FOUND - The matched default data is not found.
  EFI_OUT_OF_RESOURCES - No enough resource to create HOB.

--*/

EFI_STATUS
CreateDefaultVariableHob (
  IN UINT16  DefaultId,
  IN UINT16   BoardId
  );



/**
   Gets a vairable store header from FFS inserted by FCE

  Arguments:

    DefaultId - Specifies the type of defaults to retrieve.
    BoardId   - Specifies the platform board of defaults to retrieve.


  @return The start address of VARIABLE_STORE_HEADER *. Null if cannot find it

**/

VOID * FindDefaultHobinFfs (
  IN UINT16  DefaultId,
  IN UINT16   BoardId
  );






#endif
