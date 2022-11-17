/** @file
  Platform Hook Library instances

  @copyright
  Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>

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
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GlobalNvsArea.h>

GLOBAL_REMOVE_IF_UNREFERENCED BIOS_ACPI_PARAM              *mGlobalNvsArea;

VOID
MtOlympusUpdateGlobalNvs (
  VOID
  )
{

  //
  // Allocate and initialize the NVS area for SMM and ASL communication.
  //
  mGlobalNvsArea = (VOID *)(UINTN)PcdGet64 (PcdAcpiGnvsAddress);

  //
  // Update global NVS area for ASL and SMM init code to use
  //


}

EFI_STATUS
EFIAPI
MtOlympusBoardUpdateAcpiTable (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  )
{
  if (Table->Signature == EFI_ACPI_2_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    MtOlympusUpdateGlobalNvs ();
  }

  return EFI_SUCCESS;
}

