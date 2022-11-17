/** @file
  ACPI table pcds update.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCommonBoardInitLib.h"

#include <Library/UbaClkGenUpdateLib.h>

//
// No External clockgen, Use ICC Hybrid mode
//

EFI_STATUS
InstallClockgenData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status = EFI_SUCCESS;

  return Status;
}

