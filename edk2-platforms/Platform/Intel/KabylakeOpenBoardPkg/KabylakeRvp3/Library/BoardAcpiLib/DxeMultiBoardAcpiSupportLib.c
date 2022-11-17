/** @file
  Kaby Lake RVP 3 Multi-Board ACPI Support library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
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
KabylakeRvp3BoardUpdateAcpiTable (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  );

BOARD_ACPI_TABLE_FUNC  mKabylakeRvp3BoardAcpiTableFunc = {
  KabylakeRvp3BoardUpdateAcpiTable
};

EFI_STATUS
EFIAPI
DxeKabylakeRvp3MultiBoardAcpiSupportLibConstructor (
  VOID
  )
{
  if ((LibPcdGetSku () == BoardIdKabyLakeYLpddr3Rvp3) || (LibPcdGetSku () == BoardIdSkylakeRvp3)) {
    return RegisterBoardAcpiTableFunc (&mKabylakeRvp3BoardAcpiTableFunc);
  }
  return EFI_SUCCESS;
}

