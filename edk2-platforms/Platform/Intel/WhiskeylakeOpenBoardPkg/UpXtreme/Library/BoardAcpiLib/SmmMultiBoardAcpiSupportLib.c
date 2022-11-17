/** @file
  SMM Multi-Board ACPI Support Library for the UpXtreme Board


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
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
UpXtremeBoardEnableAcpi (
  IN BOOLEAN  EnableSci
  );

EFI_STATUS
EFIAPI
UpXtremeBoardDisableAcpi (
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
UpXtremeMultiBoardEnableAcpi (
  IN BOOLEAN  EnableSci
  )
{
  SiliconEnableAcpi (EnableSci);
  return UpXtremeBoardEnableAcpi (EnableSci);
}

EFI_STATUS
EFIAPI
UpXtremeMultiBoardDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  SiliconDisableAcpi (DisableSci);
  return UpXtremeBoardDisableAcpi (DisableSci);
}

BOARD_ACPI_ENABLE_FUNC  mUpXtremeBoardAcpiEnableFunc = {
  UpXtremeMultiBoardEnableAcpi,
  UpXtremeMultiBoardDisableAcpi,
};

EFI_STATUS
EFIAPI
SmmUpXtremeMultiBoardAcpiSupportLibConstructor (
  VOID
  )
{
  if (LibPcdGetSku () == BoardIdUpXtreme) {
    return RegisterBoardAcpiEnableFunc (&mUpXtremeBoardAcpiEnableFunc);
  }
  return EFI_SUCCESS;
}


