/** @file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MULTI_PLATFORM_SUPPORT_H_
#define _MULTI_PLATFORM_SUPPORT_H_

#include <Uefi.h>

#include <Library/PcdLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/ReadOnlyVariable2.h>

extern EFI_GUID gDefaultDataFileGuid;
extern EFI_GUID gEfiVariableGuid;
extern EFI_GUID gEfiAuthenticatedVariableGuid;

typedef struct {
  UINT16 DefaultId;
  UINT8  BoardId;
} DEFAULT_INFO;

typedef struct {
  //
  // HeaderSize includes HeaderSize fields and DefaultInfo arrays
  //
  UINT16 HeaderSize;
  //
  // DefaultInfo arrays those have the same default setting.
  //
  DEFAULT_INFO DefaultInfo[1];
  //
  // Default data is stored as variable storage.
  // VARIABLE_STORE_HEADER  VarStorageHeader;
  //
} DEFAULT_DATA;



#endif
