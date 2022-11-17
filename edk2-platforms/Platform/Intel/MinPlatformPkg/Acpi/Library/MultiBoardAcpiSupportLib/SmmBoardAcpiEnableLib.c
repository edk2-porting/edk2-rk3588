/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BoardAcpiTableLib.h>
#include <Library/MultiBoardAcpiSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>

EFI_STATUS
EFIAPI
BoardEnableAcpi (
  IN BOOLEAN  EnableSci
  )
{
  BOARD_ACPI_ENABLE_FUNC     *BoardAcpiEnableFunc;
  EFI_STATUS                 Status;

  Status = gSmst->SmmLocateProtocol (
                    &gBoardAcpiEnableGuid,
                    NULL,
                    (VOID **)&BoardAcpiEnableFunc
                    );
  if (!EFI_ERROR(Status)) {
    if (BoardAcpiEnableFunc->BoardEnableAcpi != NULL) {
      return BoardAcpiEnableFunc->BoardEnableAcpi (EnableSci);
    }
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BoardDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  BOARD_ACPI_ENABLE_FUNC     *BoardAcpiEnableFunc;
  EFI_STATUS                 Status;

  Status = gSmst->SmmLocateProtocol (
                    &gBoardAcpiEnableGuid,
                    NULL,
                    (VOID **)&BoardAcpiEnableFunc
                    );
  if (!EFI_ERROR(Status)) {
    if (BoardAcpiEnableFunc->BoardDisableAcpi != NULL) {
      return BoardAcpiEnableFunc->BoardDisableAcpi (DisableSci);
    }
  }
  return EFI_SUCCESS;
}

