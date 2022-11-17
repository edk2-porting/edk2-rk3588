/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SYSTEM_BOARD_INFO_DXE_H_
#define _SYSTEM_BOARD_INFO_DXE_H_

#include <Base.h>
#include <Uefi.h>
#include <Protocol/UbaCfgDb.h>

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/UbaSystemBoardInfoLib.h>
#include <SystemBoard.h>

// Protocol
#include <Platform.h>
#include <Ppi/PchPolicy.h>

#include <IndustryStandard/LegacyBiosMpTable.h>
#include <UncoreCommonIncludes.h>

#endif  //_SYSTEM_BOARD_INFO_DXE_H_
