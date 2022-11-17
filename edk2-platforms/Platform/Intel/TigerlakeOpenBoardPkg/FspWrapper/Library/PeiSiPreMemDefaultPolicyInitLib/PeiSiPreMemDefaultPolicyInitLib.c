/** @file
  Instance of Fsp Policy Initialization Library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/PeiPreMemSiDefaultPolicy.h>

EFI_STATUS
EFIAPI
PeiSiPreMemDefaultPolicyInitLibConstructor (
  VOID
  )
{
  EFI_STATUS Status;
  PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI *PeiPreMemSiDefaultPolicyInitPpi;

  //
  // Locate Policy init PPI to install default silicon policy
  //
  Status = PeiServicesLocatePpi (
              &gSiPreMemDefaultPolicyInitPpiGuid,
              0,
              NULL,
              (VOID **) &PeiPreMemSiDefaultPolicyInitPpi
              );
  ASSERT_EFI_ERROR (Status);
  if (PeiPreMemSiDefaultPolicyInitPpi == NULL) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "PeiPreMemSiDefaultPolicyInitPpi->PeiPreMemPolicyInit ()\n", Status));
  Status = PeiPreMemSiDefaultPolicyInitPpi->PeiPreMemPolicyInit ();
  ASSERT_EFI_ERROR (Status);

  return Status;
}
