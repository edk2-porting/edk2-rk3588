/** @file
  Tiger Lake U RVP SMM Board ACPI Enable library


  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardAcpiTableLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <PlatformBoardId.h>

/**
  Enable Board Acpi

  @param[in]  EnableSci     Enable SCI if EnableSci parameters is True.

  @retval     EFI_SUCCESS   The function always return successfully.
**/
EFI_STATUS
EFIAPI
TglBoardEnableAcpi (
  IN BOOLEAN  EnableSci
  )
{
  // enable additional board register
  return EFI_SUCCESS;
}

/**
  Disable Board Acpi

  @param[in]  DisableSci    Disable SCI if DisableSci parameters is True.

  @retval     EFI_SUCCESS   The function always return successfully.
**/
EFI_STATUS
EFIAPI
TglBoardDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  // enable additional board register
  return EFI_SUCCESS;
}
