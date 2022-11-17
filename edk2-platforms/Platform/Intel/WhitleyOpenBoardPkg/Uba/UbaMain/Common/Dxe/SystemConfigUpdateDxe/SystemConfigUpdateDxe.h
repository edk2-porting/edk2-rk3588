/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UPDATE_SYSTEM_CONFIG_DXE_H_
#define _UPDATE_SYSTEM_CONFIG_DXE_H_

#include <Base.h>
#include <Uefi.h>
#include <Protocol/UbaCfgDb.h>

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>

#include <Library/UbaSystemConfigUpdateLib.h>

#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>

#endif //_UPDATE_SYSTEM_CONFIG_DXE_H_
