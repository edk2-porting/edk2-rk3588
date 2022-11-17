/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BoardAcpiTableLib.h>
#include <Library/MultiBoardAcpiSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
RegisterBoardAcpiTableFunc (
  IN BOARD_ACPI_TABLE_FUNC  *BoardAcpiTableFunc
  )
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gBoardAcpiTableGuid,
                  EFI_NATIVE_INTERFACE,
                  BoardAcpiTableFunc
                  );
  ASSERT_EFI_ERROR(Status);

  return EFI_SUCCESS;
}
