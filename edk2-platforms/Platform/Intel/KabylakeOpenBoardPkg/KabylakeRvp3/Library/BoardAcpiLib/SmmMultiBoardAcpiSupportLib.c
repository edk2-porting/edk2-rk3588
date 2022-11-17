/** @file
  Kaby Lake RVP 3 SMM Multi-Board ACPI Support library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
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
KabylakeRvp3BoardEnableAcpi (
  IN BOOLEAN  EnableSci
  );

EFI_STATUS
EFIAPI
KabylakeRvp3BoardDisableAcpi (
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
KabylakeRvp3MultiBoardEnableAcpi (
  IN BOOLEAN  EnableSci
  )
{
  SiliconEnableAcpi (EnableSci);
  return KabylakeRvp3BoardEnableAcpi (EnableSci);
}

EFI_STATUS
EFIAPI
KabylakeRvp3MultiBoardDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  SiliconDisableAcpi (DisableSci);
  return KabylakeRvp3BoardDisableAcpi (DisableSci);
}

BOARD_ACPI_ENABLE_FUNC  mKabylakeRvp3BoardAcpiEnableFunc = {
  KabylakeRvp3MultiBoardEnableAcpi,
  KabylakeRvp3MultiBoardDisableAcpi,
};

EFI_STATUS
EFIAPI
SmmKabylakeRvp3MultiBoardAcpiSupportLibConstructor (
  VOID
  )
{
   if ((LibPcdGetSku () == BoardIdKabyLakeYLpddr3Rvp3) || (LibPcdGetSku () == BoardIdSkylakeRvp3)) {
     return RegisterBoardAcpiEnableFunc (&mKabylakeRvp3BoardAcpiEnableFunc);
  }
  return EFI_SUCCESS;
}

