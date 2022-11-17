/** @file
  Header file for private PCH SMM Lib.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SMM_PCH_PRIVATE_LIB_H_
#define _SMM_PCH_PRIVATE_LIB_H_

/**
  Set InSmm.Sts bit
**/
VOID
PchSetInSmmSts (
  VOID
  );

/**
  Clear InSmm.Sts bit
**/
VOID
PchClearInSmmSts (
  VOID
  );

#endif // _SMM_PCH_PRIVATE_LIB_H_
