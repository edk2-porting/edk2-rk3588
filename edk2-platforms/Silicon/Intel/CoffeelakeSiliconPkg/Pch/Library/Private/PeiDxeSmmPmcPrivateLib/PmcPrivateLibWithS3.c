/** @file
  PCH private PMC Library.
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
#include <Library/PchCycleDecodingLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PmcLib.h>
#include <Private/Library/PmcPrivateLib.h>

/**
  This function locks down PMC (DebugModeLock)
**/
VOID
PmcLockWithS3BootScript (
  VOID
  )
{

  UINT32 PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  //
  // Set PWRM_CFG[27] prior to OS.
  //
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_CFG, B_PMC_PWRM_CFG_DBG_MODE_LOCK);

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (PchPwrmBase + R_PMC_PWRM_CFG),
    1,
    (VOID *) ((UINTN) PchPwrmBase + R_PMC_PWRM_CFG)
    );

}

/**
  This S3 BootScript only function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
**/
VOID
PmcDisableCf9GlobalResetInS3BootScript (
  VOID
  )
{
  UINT32                          Data;

  UINT32                          PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  Data = MmioRead32 (PchPwrmBase + R_PMC_PWRM_ETR3);

  Data &= (UINT32) ~B_PMC_PWRM_ETR3_CF9GR;

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) PchPwrmBase +
    R_PMC_PWRM_ETR3,
    1,
    &Data
    );
}

/**
  This S3 BootScript only function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
  Global Reset configuration is locked after programming
**/
VOID
PmcDisableCf9GlobalResetWithLockInS3BootScript (
  VOID
  )
{
  UINT32                          Data;

  UINT32                          PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  Data = MmioRead32 (PchPwrmBase + R_PMC_PWRM_ETR3);

  Data &= (UINT32) ~B_PMC_PWRM_ETR3_CF9GR;
  Data |= (UINT32) B_PMC_PWRM_ETR3_CF9LOCK;

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) PchPwrmBase +
    R_PMC_PWRM_ETR3,
    1,
    &Data
    );
}

/**
  This function sets SLP_SX Stretching Policy and adds
  lock setting to S3 Boot Script
**/
VOID
PmcLockSlpSxStretchingPolicyWithS3BootScript (
  VOID
  )
{
  UINT32  PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  MmioOr8 (
    (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B + 2),
    (UINT8) ((B_PMC_PWRM_GEN_PMCON_B_SLPSX_STR_POL_LOCK) >> 16)
    );

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B + 2),
    1,
    (VOID *) ((UINTN) PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B + 2)
    );
}

/**
  This function sets SMI Lock with S3 Boot Script programming
**/
VOID
PmcLockSmiWithS3BootScript (
  VOID
  )
{
  UINT32  PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  MmioOr8 ((PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B), B_PMC_PWRM_GEN_PMCON_B_SMI_LOCK);

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B),
    1,
    (VOID *) ((UINTN) PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B)
    );
}

/**
  This function locks static power gating configuration with S3 Boot Script programming
**/
VOID
PmcLockFunctionDisableConfigWithS3BootScript (
  VOID
  )
{
  UINT32  PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  MmioOr32 (PchPwrmBase + R_PMC_PWRM_ST_PG_FDIS_PMC_1, (UINT32) (B_PMC_PWRM_ST_PG_FDIS_PMC_1_ST_FDIS_LK));

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    (UINTN) (PchPwrmBase + R_PMC_PWRM_ST_PG_FDIS_PMC_1),
    1,
    (VOID *) (UINTN) (PchPwrmBase + R_PMC_PWRM_ST_PG_FDIS_PMC_1)
    );
}

/**
  This function locks PMC Set Strap Message interface with S3 Boot Script programming
**/
VOID
PmcLockSetStrapMsgInterfaceWithS3BootScript (
  VOID
  )
{
  UINT32  PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  MmioOr32 ((UINTN) (PchPwrmBase + R_PMC_PWRM_SSML), B_PMC_PWRM_SSML_SSL);

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    (UINTN) (PchPwrmBase + R_PMC_PWRM_SSML),
    1,
    (VOID *) (UINTN) (PchPwrmBase + R_PMC_PWRM_SSML)
    );
}
