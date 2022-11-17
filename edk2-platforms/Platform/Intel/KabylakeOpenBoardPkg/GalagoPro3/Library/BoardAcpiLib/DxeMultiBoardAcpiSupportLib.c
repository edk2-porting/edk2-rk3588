/** @file
  DXE multi-board ACPI table support functionality.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardAcpiTableLib.h>
#include <Library/MultiBoardAcpiSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#include <PlatformBoardId.h>

EFI_STATUS
EFIAPI
GalagoPro3BoardUpdateAcpiTable (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  );

BOARD_ACPI_TABLE_FUNC  mGalagoPro3BoardAcpiTableFunc = {
  GalagoPro3BoardUpdateAcpiTable
};

EFI_STATUS
EFIAPI
DxeGalagoPro3MultiBoardAcpiSupportLibConstructor (
  VOID
  )
{
  if (LibPcdGetSku () == BoardIdGalagoPro3) {
    return RegisterBoardAcpiTableFunc (&mGalagoPro3BoardAcpiTableFunc);
  }
  return EFI_SUCCESS;
}

