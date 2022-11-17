/** @file
  Header file for the SaPolicyInitPei PEIM.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_POLICY_INIT_PEI_H_
#define _SA_POLICY_INIT_PEI_H_

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <Ppi/SiPolicy.h>
#include <SaPolicyCommon.h>
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/PeiPolicyBoardConfigLib.h>
#include <FirwmareConfigurations.h>
#include <Library/TimerLib.h>
#include <Library/GpioLib.h>

//
// Functions
//
/**
PCIe GPIO Write

@param[in] Gpio        - GPIO Number
@param[in] Active      - GPIO Active Information; High/Low
@param[in] Level       - Write GPIO value (0/1)

**/
VOID
PcieGpioWrite(
IN       UINT32                       Gpio,
IN       BOOLEAN                      Active,
IN       BOOLEAN                      Level
);

/**
PcieCardResetWorkAround performs PCIe Card reset on root port

@param[in out] SiPreMemPolicyPpi       - SI_PREMEM_POLICY_PPI

@retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
PcieCardResetWorkAround(
IN OUT   SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi
);
#endif

