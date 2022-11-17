/** @file
  Source code for the board configuration init function in Post Memory init phase.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <GopConfigLib.h>
//
// Null function for nothing GOP VBT update.
//
VOID
GopVbtSpecificUpdateNull(
  IN CHILD_STRUCT **ChildStructPtr
);
//
// for CFL U DDR4
//
VOID
CflUDdr4GopVbtSpecificUpdate(
  IN CHILD_STRUCT **ChildStructPtr
);
/**
  Board's PCD function hook init function for PEI post memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
BoardFunctionInitPreMem (
  IN UINT16 BoardId
  )
{

  return EFI_SUCCESS;
}


