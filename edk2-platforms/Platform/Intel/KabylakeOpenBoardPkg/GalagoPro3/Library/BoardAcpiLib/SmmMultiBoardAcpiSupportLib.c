/** @file
  SMM multi-board ACPI support functionality.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardAcpiEnableLib.h>
#include <Library/MultiBoardAcpiSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#include <PlatformBoardId.h>

EFI_STATUS
EFIAPI
GalagoPro3BoardEnableAcpi (
  IN BOOLEAN  EnableSci
  );

EFI_STATUS
EFIAPI
GalagoPro3BoardDisableAcpi (
  IN BOOLEAN  DisableSci
  );

EFI_STATUS
EFIAPI
SiliconEnableAcpi (
  IN BOOLEAN  EnableSci
  );

EFI_STATUS
EFIAPI
SiliconDisableAcpi (
  IN BOOLEAN  DisableSci
  );

EFI_STATUS
EFIAPI
GalagoPro3MultiBoardEnableAcpi (
  IN BOOLEAN  EnableSci
  )
{
  SiliconEnableAcpi (EnableSci);
  return GalagoPro3BoardEnableAcpi (EnableSci);
}

EFI_STATUS
EFIAPI
GalagoPro3MultiBoardDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  SiliconDisableAcpi (DisableSci);
  return GalagoPro3BoardDisableAcpi (DisableSci);
}

BOARD_ACPI_ENABLE_FUNC  mGalagoPro3BoardAcpiEnableFunc = {
  GalagoPro3MultiBoardEnableAcpi,
  GalagoPro3MultiBoardDisableAcpi,
};

EFI_STATUS
EFIAPI
SmmGalagoPro3MultiBoardAcpiSupportLibConstructor (
  VOID
  )
{
  if (LibPcdGetSku () == BoardIdGalagoPro3) {
    return RegisterBoardAcpiEnableFunc (&mGalagoPro3BoardAcpiEnableFunc);
  }
  return EFI_SUCCESS;
}

