/** @file
  Implementation of Fsp Me Policy Initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>
#include <ConfigBlock/MePeiConfig.h>
#include <Ppi/SiPolicy.h>
#include <Library/ConfigBlockLib.h>

/**
  Performs FSP ME PEI Policy pre mem initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspMePolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  EFI_STATUS                Status;
  SI_PREMEM_POLICY_PPI      *SiPreMemPolicy;
  ME_PEI_PREMEM_CONFIG      *MePeiPreMemConfig;

  DEBUG ((DEBUG_INFO, "PeiFspMePolicyInitPreMem\n"));

  //
  // Locate gSiPreMemPolicyPpi
  //
  SiPreMemPolicy = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  FspmUpd->FspmConfig.HeciTimeouts                      = (UINT8)  MePeiPreMemConfig->HeciTimeouts;
  //
  // Test policies
  //
  FspmUpd->FspmTestConfig.DidInitStat                   = (UINT8) MePeiPreMemConfig->DidInitStat;
  FspmUpd->FspmTestConfig.DisableCpuReplacedPolling     = (UINT8) MePeiPreMemConfig->DisableCpuReplacedPolling;
  FspmUpd->FspmTestConfig.SendDidMsg                    = (UINT8) MePeiPreMemConfig->SendDidMsg;
  FspmUpd->FspmTestConfig.DisableMessageCheck           = (UINT8) MePeiPreMemConfig->DisableMessageCheck;
  FspmUpd->FspmTestConfig.SkipMbpHob                    = (UINT8) MePeiPreMemConfig->SkipMbpHob;

  FspmUpd->FspmTestConfig.HeciCommunication2            = (UINT8) MePeiPreMemConfig->HeciCommunication2;
  FspmUpd->FspmTestConfig.KtDeviceEnable                = (UINT8) MePeiPreMemConfig->KtDeviceEnable;

  FspmUpd->FspmConfig.Heci1BarAddress                   = MePeiPreMemConfig->Heci1BarAddress;
  FspmUpd->FspmConfig.Heci2BarAddress                   = MePeiPreMemConfig->Heci2BarAddress;
  FspmUpd->FspmConfig.Heci3BarAddress                   = MePeiPreMemConfig->Heci3BarAddress;

  return EFI_SUCCESS;
}

/**
  Performs FSP ME PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspMePolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
  EFI_STATUS                Status;
  SI_POLICY_PPI             *SiPolicyPpi;
  ME_PEI_CONFIG             *MePeiConfig;

  DEBUG ((DEBUG_INFO, "PeiFspMePolicyInit \n"));
  //
  // Locate gSiPolicyPpiGuid
  //
  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

  FspsUpd->FspsConfig.Heci3Enabled                  = (UINT8) MePeiConfig->Heci3Enabled;
  FspsUpd->FspsConfig.MeUnconfigOnRtcClear          = (UINT8) MePeiConfig->MeUnconfigOnRtcClear;

  //
  // Test policies
  //
  FspsUpd->FspsTestConfig.MctpBroadcastCycle        = (UINT8) MePeiConfig->MctpBroadcastCycle;
  FspsUpd->FspsTestConfig.EndOfPostMessage          = (UINT8) MePeiConfig->EndOfPostMessage;
  FspsUpd->FspsTestConfig.DisableD0I3SettingForHeci = (UINT8) MePeiConfig->DisableD0I3SettingForHeci;

  return EFI_SUCCESS;
}

