/** @file
  Header file for the PeiSaPolicy library.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_SA_POLICY_LIBRARY_H_
#define _PEI_SA_POLICY_LIBRARY_H_

#include <CpuRegs.h>
#include <SaPolicyCommon.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/SmbusLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CpuPlatformLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/SiConfigBlockLib.h>

/**
  SaLoadSamplePolicyPreMem - Load some policy default for reference board.

  @param[in] ConfigBlockTableAddress    The pointer for SA config blocks

**/
VOID
SaLoadSamplePolicyPreMem (
  IN VOID           *ConfigBlockTableAddress
  );
#endif // _PEI_SA_POLICY_LIBRARY_H_
