/** @file
  PCH private PEI PMC Library for all PCH generations.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PmcLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchEspiLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Register/PchRegsPmc.h>
#include <Register/PchRegsClk.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLan.h>
#include "PmcPrivateLibInternal.h"

/**
  Check if PCH PM Timer enabled based on platform policy

  @retval TRUE       PCH PM Timer is enabled.
  @retval FALSE      PCH PM Timer is disabled.
**/
BOOLEAN
PmcIsPchPmTimerEnabled (
  VOID
  )
{
  BOOLEAN           PchPmTimerEnabled;
  EFI_STATUS        Status;
  SI_POLICY_PPI     *SiPolicy;
  PCH_PM_CONFIG     *PmConfig;

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **)&SiPolicy
             );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);

  PchPmTimerEnabled = TRUE;
  if (!PmConfig->EnableTcoTimer) {
    PchPmTimerEnabled = FALSE;
  }

  DEBUG ((DEBUG_INFO, "PmcIsPchPmTimerEnabled () = %x\n", PchPmTimerEnabled));

  return PchPmTimerEnabled;
}

/**
  Lock down PMC settings

  @param[in] SiPolicy   The SI Policy PPI instance
**/
VOID
PmcLockSettings (
  IN  SI_POLICY_PPI     *SiPolicy
  )
{

  UINT32         PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  ///
  /// Set PWRMBASE Offset 0x1048 [24]
  ///
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_ETR3, BIT24);

  ///
  /// PM_SYNC_LOCK
  /// Set PWRMBASE Offset 0x18C8 [15]
  ///
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_PMSYNC_MISC_CFG, B_PMC_PWRM_PMSYNC_PM_SYNC_LOCK);
}
