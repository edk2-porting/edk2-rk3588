/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SEC_BOARD_INIT_LIB_H_
#define _SEC_BOARD_INIT_LIB_H_

#include <PiPei.h>
#include <Uefi.h>

/**
  This is stackless function in 32bit.

  return address - ESP.
  All other registers can be used.
**/
VOID
EFIAPI
BoardBeforeTempRamInit (
  VOID
  );

EFI_STATUS
EFIAPI
BoardAfterTempRamInit (
  VOID
  );

#endif
