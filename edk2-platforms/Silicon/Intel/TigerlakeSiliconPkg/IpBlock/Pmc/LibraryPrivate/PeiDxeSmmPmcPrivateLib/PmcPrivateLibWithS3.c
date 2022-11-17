/** @file
  PCH private PMC Library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
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
#include <Library/PmcPrivateLib.h>

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
  This function sets SMI Lock with S3 Boot Script programming
**/
VOID
PmcLockSmiWithS3BootScript (
  VOID
  )
{
  UINT32  PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  MmioOr8 ((UINTN) (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B), B_PMC_PWRM_GEN_PMCON_B_SMI_LOCK);

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    (UINTN) (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B),
    1,
    (VOID *) (UINTN) (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B)
    );
}

/**
  This function sets eSPI SMI Lock
  @attention This function must be called after eSPI SMI generation has been enabled.
    This setting is required in all boot modes and before EndOfDxe.
    If set value will be restored upon S3 resume by bootscript.
**/
VOID
PmcLockEspiSmiWithS3BootScript (
  VOID
  )
{
  UINT8   Data8Or;
  UINT8   Data8And;

  Data8Or  = (UINT8) (B_PMC_PWRM_GEN_PMCON_A_ESPI_SMI_LOCK >> 8);
  Data8And = (UINT8)~((B_PMC_PWRM_GEN_PMCON_A_PWR_FLR | B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS) >> 8);

  MmioAndThenOr8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 1, Data8And, Data8Or);
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint8,
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 1,
    &Data8Or,
    &Data8And
    );
}
