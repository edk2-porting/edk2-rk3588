/** @file
  Header file for SEC PCH Lib.
  All function in this library is available for SEC

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SEC_PCH_LIB_H_
#define _SEC_PCH_LIB_H_

/**
  This function do the PCH cycle decoding initialization.
**/
VOID
EFIAPI
EarlyCycleDecoding (
  VOID
  );

#endif // _SEC_PCH_LIB_H_
