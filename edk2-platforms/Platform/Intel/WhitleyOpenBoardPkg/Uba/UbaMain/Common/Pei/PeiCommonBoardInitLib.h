/** @file
  Common Board Init.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_COMMON_BOARD_PEI_H_
#define _PEI_COMMON_BOARD_PEI_H_

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <CpuAndRevisionDefines.h>
#include <Library/IoLib.h>

#define CLOCK_GENERATOR_SETTINGS_CK505       {0x00, 0xF3, 0x0F, 0xFE, 0x98, 0x02, 0x08, 0x26, 0x7C, 0xE7, 0x0F, 0xFE, 0x08}

EFI_STATUS
InstallClockgenData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
InstallGpioPlatformData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
InstallBoardInfoData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
InstallPlatformClocksConfigData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
InstallPlatformHsioPtssTableData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
InstallIioPortBifurcationInitData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

#endif // _PEI_COMMON_BOARD_PEI_H_
