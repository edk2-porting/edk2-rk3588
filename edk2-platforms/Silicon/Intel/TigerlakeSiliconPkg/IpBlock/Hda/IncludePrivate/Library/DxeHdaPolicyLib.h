/** @file
  DXE Hda policy library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DXE_HDA_POLICY_LIB_H_
#define _DXE_HDA_POLICY_LIB_H_

#include <Protocol/PchPolicy.h>

/**
  Print HDAUDIO_DXE_CONFIG and serial out.

  @param[in] PchPolicy            Pointer to a PCH_POLICY_PROTOCOL
**/
VOID
HdaDxePrintConfig (
  IN PCH_POLICY_PROTOCOL    *PchPolicy
  );

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HdaDxeLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Get Hda config block table size.

  @retval      Size of config block
**/
UINT16
HdaDxeGetConfigBlockTotalSize (
  VOID
  );

/**
  Add Hda ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
HdaDxeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // _DXE_HDA_POLICY_LIB_H_
