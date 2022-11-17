/** @file
  DXE PcieRp policy library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DXE_PCH_PCIERP_POLICY_LIB_H_
#define _DXE_PCH_PCIERP_POLICY_LIB_H_

#include <Protocol/PchPolicy.h>

/**
  Load DXE Config block default for Pch PCIE RP

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchPcieRpDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

/**
  Print PCIE_RP_DXE_CONFIG.

  @param[in] PchPolicy            Pointer to a PCH_POLICY_PROTOCOL
**/
VOID
PchPcieRpDxePrintConfig (
  IN PCH_POLICY_PROTOCOL    *PchPolicy
  );

/**
  Get PchPcieRp config block table size.

  @retval      Size of config block
**/
UINT16
PchPcieRpDxeGetConfigBlockTotalSize (
  VOID
  );

/**
  Add PchPcieRp ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
PchPcieRpDxeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );

#endif
