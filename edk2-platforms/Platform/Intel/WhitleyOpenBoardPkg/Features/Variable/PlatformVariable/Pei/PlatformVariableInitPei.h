/** @file
  Platform variable initialization PEIM.

  This PEIM determines whether to load variable defaults. Ordinarily, the
  decision is based on the boot mode, but an OEM hook is provided to override
  that. The appropriate HOBs and PCDs are created to signal DXE code to update
  the variable default values.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_VARIABLE_INIT_PEI_H_
#define _PLATFORM_VARIABLE_INIT_PEI_H_

#include <PiPei.h>

#include <Library/MultiPlatSupportLib.h>

#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Ppi/CpuIo.h>

#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PlatformVariableHookLib.h>
#include <Library/PlatformSetupVariableSyncLib.h>

//
// We only have one ID for all the platforms.
//
#define BOARD_ID_DEFAULT  0

#endif  // #ifndef _PLATFORM_VARIABLE_INIT_PEI_H_
