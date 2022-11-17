/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/TestPointCheckLib.h>
#include <Library/DebugLib.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>

VOID
TestPointDumpCpuInfo (
  VOID
  )
{
  UINT32  RegEax;
  
  DEBUG ((DEBUG_INFO, "==== TestPointDumpCpuInfo - Enter\n"));

  DEBUG((DEBUG_INFO, "CPU info\n"));
  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  DEBUG((DEBUG_INFO, "  CPUID = 0x%08x\n", RegEax));

  DEBUG((DEBUG_INFO, "  Microcode ID (0x%08x)  = 0x%016lx\n", MSR_IA32_BIOS_SIGN_ID, AsmReadMsr64 (MSR_IA32_BIOS_SIGN_ID)));
  DEBUG((DEBUG_INFO, "  Platform ID (0x%08x)   = 0x%016lx\n", MSR_IA32_PLATFORM_ID, AsmReadMsr64 (MSR_IA32_PLATFORM_ID)));

  DEBUG ((DEBUG_INFO, "==== TestPointDumpCpuInfo - Exit\n"));
}
