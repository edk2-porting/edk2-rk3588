/** @file
  Data format for Emulation

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef   __EMULATION_DFX_VARIABLE_H__
#define   __EMULATION_DFX_VARIABLE_H__

#pragma pack(1)
typedef struct {
  UINT8    DfxUbiosGeneration;
  UINT8    DfxHybridSystemLevelEmulation;
  UINT8    DfxPmMsrTrace;
} EMULATION_DFX_CONFIGURATION;
#pragma pack()

extern EFI_GUID gEmulationDfxVariableGuid;
#define EMULATION_DFX_CONFIGURATION_NAME L"EmulationDfxConfig"

#endif

