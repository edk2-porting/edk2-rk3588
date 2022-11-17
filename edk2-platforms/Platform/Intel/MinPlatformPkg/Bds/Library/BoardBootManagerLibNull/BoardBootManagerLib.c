/** @file
  This file include board specific boot manager callbacks

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/BoardBootManagerLib.h>

/**
  This function is called each second during the boot manager waits timeout.

  @param[in] TimeoutRemain  The remaining timeout.
**/
VOID
EFIAPI
BoardBootManagerWaitCallback (
  IN UINT16          TimeoutRemain
  )
{
  return;
}

/**
  The function is called when no boot option could be launched,
  including platform recovery options and options pointing to applications
  built into firmware volumes.

**/
VOID
EFIAPI
BoardBootManagerUnableToBoot (
  VOID
  )
{
  return;
}

