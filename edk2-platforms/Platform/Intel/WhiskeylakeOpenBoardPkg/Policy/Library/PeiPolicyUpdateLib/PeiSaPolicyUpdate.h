/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_SA_POLICY_UPDATE_H_
#define _PEI_SA_POLICY_UPDATE_H_

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
#include <PlatformBoardId.h>
#include <SaPolicyCommon.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Ppi/Wdt.h>
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include "PeiPchPolicyUpdate.h"
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <CpuAccess.h>

#define WDT_TIMEOUT 60

// BClk Frequency Limitations (in Hz)
#define BCLK_MAX                538000000
#define BCLK_100                100000000
#define BCLK_GRANULARITY        1000000
#define BCLK_100_KHZ            100000


/**
  PeiGetSectionFromFv finds the file in FV and gets file Address and Size

  @param[in] NameGuid              - File GUID
  @param[out] Address              - Pointer to the File Address
  @param[out] Size                 - Pointer to File Size

  @retval EFI_SUCCESS                Successfull in reading the section from FV
**/
EFI_STATUS
EFIAPI
PeiGetSectionFromFv (
  IN CONST  EFI_GUID        NameGuid,
  OUT VOID                  **Address,
  OUT UINT32               *Size
  );

#endif


