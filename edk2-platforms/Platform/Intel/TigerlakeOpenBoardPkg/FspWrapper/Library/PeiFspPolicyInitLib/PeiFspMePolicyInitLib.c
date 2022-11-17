/** @file
  Implementation of Fsp Me Policy Initialization.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>
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
  DEBUG ((DEBUG_INFO, "PeiFspMePolicyInitPreMem\n"));

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
  DEBUG ((DEBUG_INFO, "PeiFspMePolicyInit \n"));

  return EFI_SUCCESS;
}

