/** @file
  Provide SecTemporaryRamDone function.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>

#include <Ppi/TemporaryRamDone.h>
#include <Ppi/TempRamExitPpi.h>
#include <Ppi/PlatformInitTempRamExitPpi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugAgentLib.h>
#include <Library/FspWrapperPlatformLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/PeiServicesTablePointerLib.h>

/**
This interface disables temporary memory in SEC Phase.
**/
VOID
EFIAPI
SecPlatformDisableTemporaryMemory (
  VOID
  )
{
  EFI_STATUS                        Status;
  VOID                              *TempRamExitParam;
  CONST EFI_PEI_SERVICES            **PeiServices;
  FSP_TEMP_RAM_EXIT_PPI             *TempRamExitPpi;
  PLATFORM_INIT_TEMP_RAM_EXIT_PPI   *PlatformInitTempRamExitPpi;

  DEBUG ((DEBUG_INFO, "SecPlatformDisableTemporaryMemory enter\n"));
  PeiServices = GetPeiServicesTablePointer ();
  ASSERT (PeiServices != NULL);
  if (PeiServices == NULL) {
    return;
  }
  ASSERT ((*PeiServices) != NULL);
  if ((*PeiServices) == NULL) {
    return;
  }
  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gPlatformInitTempRamExitPpiGuid,
                            0,
                            NULL,
                            (VOID **) &PlatformInitTempRamExitPpi
                            );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = PlatformInitTempRamExitPpi->PlatformInitBeforeTempRamExit ();
  ASSERT_EFI_ERROR (Status);

  if (PcdGet8 (PcdFspModeSelection) == 1) {
    //
    // FSP API mode
    //
    TempRamExitParam = UpdateTempRamExitParam ();
    Status = CallTempRamExit (TempRamExitParam);
    DEBUG ((DEBUG_INFO, "TempRamExit status: 0x%x\n", Status));
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // FSP Dispatch mode
    //
    Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gFspTempRamExitPpiGuid,
                             0,
                             NULL,
                             (VOID **) &TempRamExitPpi
                             );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return;
    }
    TempRamExitPpi->TempRamExit (NULL);
  }

  Status = PlatformInitTempRamExitPpi->PlatformInitAfterTempRamExit ();
  ASSERT_EFI_ERROR (Status);

  return ;
}
