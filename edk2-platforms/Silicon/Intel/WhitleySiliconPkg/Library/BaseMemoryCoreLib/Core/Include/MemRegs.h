/** @file
  MemRegs.h

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _memregs_h
#define _memregs_h

//
// NVM DIMM Reg Structs
//

typedef union {
  struct {
    UINT8 module_type     : 4;
    UINT8 module_revision : 4;
  } Bits;
  UINT8 Data;
} SPD_AEP_MOD_REVISION_STRUCT;

#endif // _memregs_h
