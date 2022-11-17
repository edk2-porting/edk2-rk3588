/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MULTI_BOARD_ACPI_SUPPORT_LIB_H_
#define _MULTI_BOARD_ACPI_SUPPORT_LIB_H_

#include <Library/BoardAcpiTableLib.h>
#include <Library/BoardAcpiEnableLib.h>

typedef
EFI_STATUS
(EFIAPI *BOARD_ENABLE_ACPI) (
  IN BOOLEAN  EnableSci
  );

typedef
EFI_STATUS
(EFIAPI *BOARD_DISABLE_ACPI) (
  IN BOOLEAN  DisableSci
  );

typedef struct {
  BOARD_ENABLE_ACPI        BoardEnableAcpi;
  BOARD_DISABLE_ACPI       BoardDisableAcpi;
} BOARD_ACPI_ENABLE_FUNC;

typedef
EFI_STATUS
(EFIAPI *BOARD_UPDATE_ACPI_TABLE) (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  );

typedef struct {
  BOARD_UPDATE_ACPI_TABLE  BoardUpdateAcpiTable;
} BOARD_ACPI_TABLE_FUNC;

EFI_STATUS
EFIAPI
RegisterBoardAcpiEnableFunc (
  IN BOARD_ACPI_ENABLE_FUNC  *BoardAcpiEnableFunc
  );

EFI_STATUS
EFIAPI
RegisterBoardAcpiTableFunc (
  IN BOARD_ACPI_TABLE_FUNC  *BoardAcpiTableFunc
  );

#endif
