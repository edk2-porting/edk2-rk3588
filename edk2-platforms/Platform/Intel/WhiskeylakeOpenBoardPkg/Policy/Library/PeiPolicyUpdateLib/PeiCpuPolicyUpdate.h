/** @file
  Header file for PEI CpuPolicyUpdate.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_CPU_POLICY_UPDATE_H_
#define _PEI_CPU_POLICY_UPDATE_H_

#include <PiPei.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/Wdt.h>
#include <Library/DebugLib.h>
#include <Library/PeiPlatformLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <PlatformBoardId.h>
#include <PchAccess.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <CpuAccess.h>
#include <Ppi/MasterBootMode.h>
#include <Library/PeiServicesLib.h>
#include "PeiPchPolicyUpdate.h"
#include <Library/CpuPlatformLib.h>

#endif

