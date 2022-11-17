/** @file
  Header file for the PeiCpuPolicyInit.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_CPU_POLICY_INIT_H_
#define _PEI_CPU_POLICY_INIT_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CpuPolicyLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/PeiPolicyBoardConfigLib.h>
#include <FirwmareConfigurations.h>

/**
  This function performs CPU PEI Policy initialization in PreMem.

  @param[in, out] SiPreMemPolicyPpi  The Si Pre-Mem Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyPreMem (
  IN OUT  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  );
#endif

