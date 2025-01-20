/** @file
*
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __RK3588DXE_CPU_PERF_H__
#define __RK3588DXE_CPU_PERF_H__

#define CLUSTER_MICROVOLTS_MIN           500000
#define CLUSTER_MICROVOLTS_MAX           1500000
#define CLUSTER_MICROVOLTS_STEP          6250
#define CPUL_CLUSTER_MICROVOLTS_DEFAULT  950000
#define CPUB_CLUSTER_MICROVOLTS_DEFAULT  1000000

//
// Don't declare these in the VFR file.
//
#ifndef VFR_FILE_INCLUDE
VOID
EFIAPI
ApplyCpuClockVariables (
  VOID
  );

VOID
EFIAPI
ApplyCpuVoltageVariables (
  VOID
  );

VOID
EFIAPI
SetupCpuPerfVariables (
  VOID
  );

#endif

#endif // __RK3588DXE_CPU_PERF_H__
