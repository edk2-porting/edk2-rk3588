/** @file
  Source code for the board configuration init function in Post Memory init phase.


  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <GopConfigLib.h>

//
// Null function for nothing GOP VBT update.
//
VOID
GopVbtSpecificUpdateNull (
  IN CHILD_STRUCT **ChildStructPtr
  );

//
// for CFL U DDR4
//
VOID
CflUDdr4GopVbtSpecificUpdate (
  IN CHILD_STRUCT **ChildStructPtr
  );
