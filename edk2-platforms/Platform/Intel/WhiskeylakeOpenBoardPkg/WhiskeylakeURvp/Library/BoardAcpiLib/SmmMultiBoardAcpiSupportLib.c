/** @file
  Whiskey Lake U RVP SMM Multi-Board ACPI Support library


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
WhiskeylakeURvpBoardEnableAcpi (
  IN BOOLEAN  EnableSci
  );

EFI_STATUS
EFIAPI
WhiskeylakeURvpBoardDisableAcpi (
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
WhiskeylakeURvpMultiBoardEnableAcpi (
  IN BOOLEAN  EnableSci
  )
{
  SiliconEnableAcpi (EnableSci);
  return WhiskeylakeURvpBoardEnableAcpi (EnableSci);
}

EFI_STATUS
EFIAPI
WhiskeylakeURvpMultiBoardDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  SiliconDisableAcpi (DisableSci);
  return WhiskeylakeURvpBoardDisableAcpi (DisableSci);
}

BOARD_ACPI_ENABLE_FUNC  mWhiskeylakeURvpBoardAcpiEnableFunc = {
  WhiskeylakeURvpMultiBoardEnableAcpi,
  WhiskeylakeURvpMultiBoardDisableAcpi,
};

EFI_STATUS
EFIAPI
SmmWhiskeylakeURvpMultiBoardAcpiSupportLibConstructor (
  VOID
  )
{
  if (LibPcdGetSku () == BoardIdWhiskeyLakeRvp) {
    return RegisterBoardAcpiEnableFunc (&mWhiskeylakeURvpBoardAcpiEnableFunc);
  }
  return EFI_SUCCESS;
}


