/** @file
  Silicon workaround library.

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//
#include <Library/SiliconWorkaroundLib.h>
#include <Library/BaseLib.h>


/**
  Return whether the workaround is enabled.

  This function returns a boolean that determines whether the workaround is enabled
  given a workaround name.

  @param[in]      WorkaroundName         An ASCII string that represents the workaround name.
                                         This workaround name should correspond to an entry
                                         in the silicon workarounds table(s).

  @retval TRUE    The workaround is enabled.
  @retval FALSE   The workaround is not found in the table(s) and therefore disabled.

**/
BOOLEAN
EFIAPI
IsSiliconWorkaroundEnabled (
  IN CONST CHAR8    *WorkaroundName
  )
{
  return FALSE;
}
