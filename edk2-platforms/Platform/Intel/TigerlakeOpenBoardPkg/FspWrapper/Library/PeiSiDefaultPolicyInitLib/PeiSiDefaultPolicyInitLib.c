/** @file
  Instance of Fsp Policy Initialization Library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/PeiSiDefaultPolicy.h>

EFI_STATUS
EFIAPI
PeiSiDefaultPolicyInitLibConstructor (
  VOID
  )
{
  EFI_STATUS Status;
  PEI_SI_DEFAULT_POLICY_INIT_PPI *PeiSiDefaultPolicyInitPpi;

  //
  // Locate Policy init PPI to install default silicon policy
  //
  Status = PeiServicesLocatePpi (
              &gSiDefaultPolicyInitPpiGuid,
              0,
              NULL,
              (VOID **) &PeiSiDefaultPolicyInitPpi
              );
  ASSERT_EFI_ERROR (Status);
  if (PeiSiDefaultPolicyInitPpi == NULL) {
    return Status;
  }
  Status = PeiSiDefaultPolicyInitPpi->PeiPolicyInit ();
  ASSERT_EFI_ERROR (Status);

  return Status;
}
