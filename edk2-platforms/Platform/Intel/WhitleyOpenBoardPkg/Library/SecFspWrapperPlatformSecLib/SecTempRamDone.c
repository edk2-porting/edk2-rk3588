/** @file
  Sample to provide SecTemporaryRamDone function.

  @copyright
  Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>

#include <Ppi/TemporaryRamDone.h>
#include <Ppi/PlatformInitTempRamExitPpi.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/FspWrapperPlatformLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <Guid/FspHeaderFile.h>

#include <Register/ArchitecturalMsr.h>

#define MSR_NEM 0x000002E0

/**
This interface disables temporary memory in SEC Phase.
This is for dispatch mode use.  We should properly produce the FSP_TEMP_RAM_EXIT_PPI and then call
that instead, but the FSP does not produce that PPI
**/
VOID
EFIAPI
SecPlatformDisableTemporaryMemoryDispatchHack (
  VOID
  )
{
  UINT64  MsrValue;
  UINT64  MtrrDefaultType;
  MSR_IA32_MTRR_DEF_TYPE_REGISTER   DefType;

  //
  // Force and INVD.
  //
  AsmInvd ();

  //
  // Disable MTRRs.
  //
  DefType.Uint64 = AsmReadMsr64 (MSR_IA32_MTRR_DEF_TYPE);
  MtrrDefaultType = DefType.Uint64;
  DefType.Bits.E = 0;
  AsmWriteMsr64 (MSR_IA32_MTRR_DEF_TYPE, DefType.Uint64);

  //
  // Force and INVD to prevent MCA error.
  //
  AsmInvd ();

  //
  // Clear NEM Run and NEM Setup bits individually.
  //
  MsrValue = AsmReadMsr64 (MSR_NEM);
  MsrValue &= ~((UINT64) BIT1);
  AsmWriteMsr64 (MSR_NEM, MsrValue);
  MsrValue &= ~((UINT64) BIT0);
  AsmWriteMsr64 (MSR_NEM, MsrValue);

  //
  // Restore MTRR default setting
  //
  AsmWriteMsr64 (MSR_IA32_MTRR_DEF_TYPE, MtrrDefaultType);
}

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
  PLATFORM_INIT_TEMP_RAM_EXIT_PPI   *PlatformInitTempRamExitPpi;

  DEBUG ((DEBUG_INFO, "SecPlatformDisableTemporaryMemory enter\n"));
  PeiServices = GetPeiServicesTablePointer ();
  ASSERT (PeiServices != NULL);
  if (PeiServices == NULL) {
    return ;
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
    SecPlatformDisableTemporaryMemoryDispatchHack ();
  }

  Status = PlatformInitTempRamExitPpi->PlatformInitAfterTempRamExit ();
  ASSERT_EFI_ERROR(Status);

  return ;
}
