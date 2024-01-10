/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef _BASE_VARIABLE_H_
#define _BASE_VARIABLE_H_

#include <Uefi.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiMultiPhase.h>

#include <Guid/VariableFormat.h>
#include <Guid/VariableIndexTable.h>
#include <Guid/SystemNvDataGuid.h>
#include <Guid/FaultTolerantWrite.h>

#include <Library/BaseMemoryLib.h>
#include <Library/BaseVariableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

typedef enum {
  VariableStoreTypeHob,
  VariableStoreTypeNv,
  VariableStoreTypeMax
} VARIABLE_STORE_TYPE;

typedef struct {
  VARIABLE_STORE_HEADER                   *VariableStoreHeader;
  VARIABLE_INDEX_TABLE                    *IndexTable;
  //
  // If it is not NULL, it means there may be an inconsecutive variable whose
  // partial content is still in NV storage, but another partial content is backed up
  // in spare block.
  //
  FAULT_TOLERANT_WRITE_LAST_WRITE_DATA    *FtwLastWriteData;
  BOOLEAN                                 AuthFlag;
} VARIABLE_STORE_INFO;

#endif // _BASE_VARIABLE_H_
