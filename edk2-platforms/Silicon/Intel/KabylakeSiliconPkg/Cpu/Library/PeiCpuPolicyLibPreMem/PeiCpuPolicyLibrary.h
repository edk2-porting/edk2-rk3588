/** @file
  Header file for the PeiCpuPolicyLib library.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PEI_CPU_POLICY_LIBRARY_H_
#define _PEI_CPU_POLICY_LIBRARY_H_

#include <PiPei.h>
#include <Register/Cpuid.h>
#include <CpuAccess.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/MasterBootMode.h>
#include <Ppi/SiPolicy.h>
#include <Library/CpuPolicyLibPreMem.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>

//
// TXT configuration defines
//
#define TXT_SINIT_MEMORY_SIZE         0x50000
#define TXT_HEAP_MEMORY_SIZE          0xE0000
#define TXT_LCP_PD_BASE               0x0         ///< Platform default LCP
#define TXT_LCP_PD_SIZE               0x0         ///< Platform default LCP
#define TXT_TGA_MEMORY_SIZE           0x0         ///< Maximum 512K of TGA memory (aperture)

#endif // _PEI_CPU_POLICY_LIBRARY_H_
