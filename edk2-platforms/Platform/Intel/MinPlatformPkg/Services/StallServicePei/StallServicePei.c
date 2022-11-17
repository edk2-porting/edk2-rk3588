/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Ppi/Stall.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/TimerLib.h>
#include <Library/PeiServicesLib.h>

#define PEI_STALL_RESOLUTION   1

/**
  The Stall() function provides a blocking stall for at least the number
  of microseconds stipulated in the final argument of the API.

  @param  PeiServices    An indirect pointer to the PEI Services Table
                         published by the PEI Foundation.
  @param  This           Pointer to the local data for the interface.
  @param  Microseconds   Number of microseconds for which to stall.

  @retval EFI_SUCCESS    The service provided at least the required delay.

**/
EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN CONST EFI_PEI_STALL_PPI  *This,
  IN UINTN                    Microseconds
  );

EFI_PEI_STALL_PPI mStallPpi = {
  PEI_STALL_RESOLUTION,
  Stall
};

EFI_PEI_PPI_DESCRIPTOR  mPeiInstallStallPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiStallPpiGuid,
  &mStallPpi
};

EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN CONST EFI_PEI_STALL_PPI  *This,
  IN UINTN                    Microseconds
  )
{
  MicroSecondDelay (Microseconds);

  return EFI_SUCCESS;
}

/**
  This function will install the EFI_PEI_STALL_PPI.

  @param  FileHandle            Handle of the file being invoked.
  @param  PeiServices           Pointer to PEI Services table.

  @retval EFI_SUCCESS           The EFI_PEI_STALL_PPI was installed successfully.
  @retval EFI_INVALID_PARAMETER The PpiList pointer is NULL.
  @retval EFI_INVALID_PARAMETER The PEI PPI descriptor in the list does not have the
                                EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the PPI database.

**/
EFI_STATUS
EFIAPI
StallServiceEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS   Status;

  Status = PeiServicesInstallPpi (&mPeiInstallStallPpi);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
