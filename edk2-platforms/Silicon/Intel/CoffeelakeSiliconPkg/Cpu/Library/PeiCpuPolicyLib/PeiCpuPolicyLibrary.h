/** @file
  Header file for the PeiCpuPolicyLib library.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_CPU_POLICY_LIBRARY_H_
#define _PEI_CPU_POLICY_LIBRARY_H_

#include <PiPei.h>
#include <CpuAccess.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/MasterBootMode.h>
#include <Ppi/SiPolicy.h>
#include <Library/CpuPolicyLib.h>
#include <IndustryStandard/SmBios.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/Cpuid.h>
#include <Library/PcdLib.h>

#define MAX_MICROCODE_PATCH_SIZE 0x20000

#endif // _PEI_CPU_POLICY_LIBRARY_H_
