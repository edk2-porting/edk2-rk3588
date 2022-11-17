/** @file
  DXE Gpio policy library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DXE_GPIO_POLICY_LIB_H_
#define _DXE_GPIO_POLICY_LIB_H_

#include <Protocol/PchPolicy.h>

/**
  Print GPIO_DXE_CONFIG and serial out.

  @param[in] PchPolicy            Pointer to a PCH_POLICY_PROTOCOL
**/
VOID
GpioDxePrintConfig (
  IN PCH_POLICY_PROTOCOL    *PchPolicy
  );

/**
  Load DXE Config block default for GPIO

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
GpioDxeLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Gpio config block table size.

  @retval      Size of config block
**/
UINT16
GpioDxeGetConfigBlockTotalSize (
  VOID
  );

/**
  Add Gpio ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
GpioDxeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _DXE_GPIO_POLICY_LIB_H_
