/** @file
 Header file for board Init function for DXE Init phase.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_BOARD_INIT_LIB_H_
#define _DXE_BOARD_INIT_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <PlatformBoardId.h>
#include <Register/PchRegs.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPcieRpLib.h>
#include <Platform.h>

EFI_STATUS
EFIAPI
BoardConfigInit (
   VOID
  );

#endif // _DXE_BOARD_INIT_LIB_H_

