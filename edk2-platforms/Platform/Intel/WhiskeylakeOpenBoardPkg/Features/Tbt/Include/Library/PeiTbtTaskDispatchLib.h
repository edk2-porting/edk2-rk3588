/** @file
  PEI TBT Task Dispatch library Header file


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PEI_TBT_TASK_DISPATCH_LIB_H__
#define __PEI_TBT_TASK_DISPATCH_LIB_H__
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/GpioLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Ppi/PeiTbtPolicy.h>

typedef
EFI_STATUS
(EFIAPI *TBT_TASK) (
  PEI_TBT_POLICY  *PeiTbtConfig
);

typedef enum {
  TBT_NULL,                ///< All policy flags turned off.
  TBT_NORMAL   = (1 << 0), ///< Execute TBT function on cold reset.
  TBT_S3       = (1 << 1), ///< Execute TBT function on S3 exit.
  TBT_S4       = (1 << 2), ///< Execute TBT function on S4 exit.
  TBT_ALL      = MAX_UINTN ///< Execute TBT function always.
} TBT_BOOT_MODE;

typedef struct {
  TBT_TASK      TbtTask;         ///< Ptr to function to execute, with parameter list.
  TBT_BOOT_MODE TbtBootModeFlag; ///< Call table base on TbtBootModeFlag
  CHAR8         *String;         ///< Output string describing this task.
} TBT_CALL_TABLE_ENTRY;

/**
  Covert the current EFI_BOOT_MODE to TBT_BOOT_MODE
**/
TBT_BOOT_MODE
TbtGetBootMode (
  VOID
);

/**
  TbtTaskDistpach: Dispatch the TBT tasks according to TBT_CALL_TABLE_ENTRY

  @param[in] TBT_CALL_TABLE_ENTRY   TbtCallTable

**/
VOID
TbtTaskDistpach (
  IN TBT_CALL_TABLE_ENTRY *TbtCallTable
);
#endif

