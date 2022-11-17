/** @file
  PCH PMC Private Library implementation for Cannon Lake PCH.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Library/SataLib.h>
#include <Library/PchInfoLib.h>
#include <Register/PchRegsPmc.h>

#include "PmcPrivateLibInternal.h"

/**
  This function disables (non-static power gating) SATA and enables ModPHY SPD gating (PCH-LP only).

  @param[in]  SataCtrlIndex     SATA controller index
**/
VOID
PmcDisableSata (
  IN UINT32     SataCtrlIndex
  )
{
  UINT32 PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  //
  // Set PWRMBASE + NST_PG_FDIS_1 [22] = 1b to disable SATA Controller in PMC
  //
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_NST_PG_FDIS_1, B_PMC_PWRM_NST_PG_FDIS_1_SATA_FDIS_PMC);

  if (PmcIsModPhySusPgSupported ()) {
    //
    // MPHY SUS PG supported on PCH-LP only:
    //
    // Set MSPDRTREQ:
    // PWRMBASE + R_PWRM_MODPHY_PM_CFG5[12] = 1 to enable ASL code trigger request for ModPHY SPD gating.
    //
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_MODPHY_PM_CFG5, B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_SATA);
  }
}

/**
  This function checks if SATA device is supported (not disabled by fuse)

  @param[in] SataCtrlIndex SATA controller index

  @retval SATA support state
**/
BOOLEAN
PmcIsSataSupported (
  UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  //
  // Get fuse info from PWRMBASE + FUSE_SS_DIS_RD_2
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_FUSE_DIS_RD_2) & B_PMC_PWRM_FUSE_DIS_RD_2_SATA_FUSE_SS_DIS) == 0);
}
