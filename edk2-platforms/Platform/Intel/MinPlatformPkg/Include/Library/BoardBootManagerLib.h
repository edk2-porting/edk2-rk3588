/** @file
  Board Boot Manager library definition. A platform can implement
  instances to support platform-specific behavior.

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#ifndef __BOARD_BOOT_MANAGER_LIB_H_
#define __BOARD_BOOT_MANAGER_LIB_H_
#include <Library/UefiBootManagerLib.h>


/**
  This function is called each second during boot manager waits the timeout.

  @param[in] TimeoutRemain  The remaining timeout.
**/
VOID
EFIAPI
BoardBootManagerWaitCallback (
  IN UINT16          TimeoutRemain
  );

/**
  The function is called when no boot option could be launched,
  including platform recovery options and options pointing to applications
  built into firmware volumes.

**/
VOID
EFIAPI
BoardBootManagerUnableToBoot (
  VOID
  );

#endif
