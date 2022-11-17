/** @file
  Prototype of the CpuPolicy library.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _CPU_POLICY_LIB_H_
#define _CPU_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print whole CPU config blocks of SiPolicyPpi and serial out.

  @param[in] SiPolicyPpi             The SI Policy PPI instance
**/
VOID
CpuPrintPolicy (
  IN  SI_POLICY_PPI                 *SiPolicyPpi
  );

/**
  Get CPU config block table total size.

  @retval Size of CPU config block table
**/
UINT16
EFIAPI
CpuGetConfigBlockTotalSize (
  VOID
  );

/**
  CpuAddConfigBlocks add all Cpu config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CpuAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

#endif // _PEI_CPU_POLICY_LIB_H_
