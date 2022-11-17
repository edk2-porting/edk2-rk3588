/** @file

Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/SiliconPolicyInitLib.h>
#include <Guid/SiliconPolicyInitLibInterface.h>

VOID *
EFIAPI
SiliconPolicyInitPreMem (
  IN VOID *Policy
  )
{
  RETURN_STATUS                   Status = RETURN_SUCCESS;
  SILICON_POLICY_INIT_LIB_PPI     *SiliconPolicyInitLibPpi = NULL;

  Status = PeiServicesLocatePpi (&gSiliconPolicyInitLibInterfaceGuid, 0, NULL, &SiliconPolicyInitLibPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return NULL;
  }

  return SiliconPolicyInitLibPpi->SiliconPolicyInitPreMem (Policy);
}

RETURN_STATUS
EFIAPI
SiliconPolicyDonePreMem (
  IN VOID *Policy
  )
{
  RETURN_STATUS                   Status = RETURN_SUCCESS;
  SILICON_POLICY_INIT_LIB_PPI     *SiliconPolicyInitLibPpi = NULL;

  Status = PeiServicesLocatePpi (&gSiliconPolicyInitLibInterfaceGuid, 0, NULL, &SiliconPolicyInitLibPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return SiliconPolicyInitLibPpi->SiliconPolicyDonePreMem (Policy);
}

VOID *
EFIAPI
SiliconPolicyInitPostMem (
  IN VOID *Policy
  )
{
  RETURN_STATUS                   Status = RETURN_SUCCESS;
  SILICON_POLICY_INIT_LIB_PPI     *SiliconPolicyInitLibPpi = NULL;

  Status = PeiServicesLocatePpi (&gSiliconPolicyInitLibInterfaceGuid, 0, NULL, &SiliconPolicyInitLibPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return NULL;
  }

  return SiliconPolicyInitLibPpi->SiliconPolicyInitPostMem (Policy);
}

RETURN_STATUS
EFIAPI
SiliconPolicyDonePostMem (
  IN VOID *Policy
  )
{
  RETURN_STATUS                   Status = RETURN_SUCCESS;
  SILICON_POLICY_INIT_LIB_PPI     *SiliconPolicyInitLibPpi = NULL;

  Status = PeiServicesLocatePpi (&gSiliconPolicyInitLibInterfaceGuid, 0, NULL, &SiliconPolicyInitLibPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return SiliconPolicyInitLibPpi->SiliconPolicyDonePostMem (Policy);
}

VOID *
EFIAPI
SiliconPolicyInitLate (
  IN VOID *Policy
  )
{
  ASSERT_EFI_ERROR (RETURN_UNSUPPORTED);
  return NULL;
}

RETURN_STATUS
EFIAPI
SiliconPolicyDoneLate (
  IN VOID *Policy
  )
{
  ASSERT_EFI_ERROR (RETURN_UNSUPPORTED);
  return RETURN_SUCCESS;
}
