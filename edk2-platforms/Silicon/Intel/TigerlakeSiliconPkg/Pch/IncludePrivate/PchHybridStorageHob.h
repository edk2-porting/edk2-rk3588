/** @file

  Definitions required to create HybridStorageHob

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_HYBRIDSTORAGE_HOB_
#define _PCH_HYBRIDSTORAGE_HOB_

extern EFI_GUID  gHybridStorageHobGuid;

//
//  Passes to DXE Hybrid Storage location
//
typedef struct {
  UINT32 HybridStorageLocation;
} PCH_HYBRIDSTORAGE_HOB;

#endif
