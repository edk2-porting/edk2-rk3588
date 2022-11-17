/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_SA_POLICY_UPDATE_H_
#define _PEI_SA_POLICY_UPDATE_H_

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
#include <SaPolicyCommon.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include "PeiPchPolicyUpdate.h"
#include <Library/PcdLib.h>
#include <CpuAccess.h>

#include <FspEas.h>
#include <FspmUpd.h>
#include <FspsUpd.h>

extern EFI_GUID gTianoLogoGuid;

/**
  Performs FSP SA PEI Policy initialization in pre-memory.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyUpdatePreMem (
  IN OUT FSPM_UPD    *FspmUpd
  );

#endif


