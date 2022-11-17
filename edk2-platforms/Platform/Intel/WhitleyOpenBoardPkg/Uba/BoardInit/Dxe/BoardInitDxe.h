/** @file
  BOARD INIT DXE Driver.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _BOARD_INIT_DXE_H_
#define _BOARD_INIT_DXE_H_

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>  // For Hob

#include <Protocol/UbaCfgDb.h>
#include <Protocol/UbaMakerProtocol.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Platform.h>

#endif // _BOARD_INIT_DXE_H_
