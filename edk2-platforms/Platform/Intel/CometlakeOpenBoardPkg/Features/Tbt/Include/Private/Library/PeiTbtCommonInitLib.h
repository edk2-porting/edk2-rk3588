/** @file
  PEI TBT Common Init Dispatch library Header file


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PEI_TBT_COMMON_INIT_LIB_H__
#define __PEI_TBT_COMMON_INIT_LIB_H__

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiTbtTaskDispatchLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/TbtCommonLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PmcLib.h>
#include <PlatformNvRamHookLib.h>

BOOLEAN
IsHostRouterPresentBeforeSleep(
IN  UINT8        ControllerType,
IN  UINT8        Controller
);

VOID
TbtSetSxMode(
IN    BOOLEAN                 Type,
IN    UINT8                   Bus,
IN    UINT8                   Device,
IN    UINT8                   Function,
IN    UINT8                   TbtBootOn
);

VOID
TbtClearVgaRegisters(
IN    UINTN                   Segment,
IN    UINTN                   Bus,
IN    UINTN                   Device,
IN    UINTN                   Function
);

#endif

