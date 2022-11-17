/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BoardAcpiEnableLib.h>
#include <Library/MultiBoardAcpiSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
BoardUpdateAcpiTable (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  )
{
  BOARD_ACPI_TABLE_FUNC    *BoardAcpiTableFunc;
  EFI_STATUS               Status;

  Status = gBS->LocateProtocol (
                  &gBoardAcpiTableGuid,
                  NULL,
                  (VOID **)&BoardAcpiTableFunc
                  );
  if (!EFI_ERROR(Status)) {
    if (BoardAcpiTableFunc->BoardUpdateAcpiTable != NULL) {
      return BoardAcpiTableFunc->BoardUpdateAcpiTable (Table, Version);
    }
  }
  return EFI_SUCCESS;
}
