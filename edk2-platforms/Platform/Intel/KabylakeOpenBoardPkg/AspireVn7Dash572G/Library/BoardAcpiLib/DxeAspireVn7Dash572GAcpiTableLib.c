/** @file
  Aspire VN7-572G Board ACPI Library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <PiDxe.h>
#include <Library/BoardAcpiTableLib.h>
#include <Library/EcLib.h>
#include <Library/PcdLib.h>
#include <Protocol/GlobalNvsArea.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_GLOBAL_NVS_AREA_PROTOCOL              mGlobalNvsArea;

VOID
AspireVn7Dash572GUpdateGlobalNvs (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT8       PowerRegister;

  //
  // Allocate and initialize the NVS area for SMM and ASL communication.
  //
  mGlobalNvsArea.Area = (VOID *)(UINTN)PcdGet64 (PcdAcpiGnvsAddress);

  //
  // Update global NVS area for ASL and SMM init code to use
  //

  //
  // Enable PowerState
  //
  Status = EcRead (0x70, &PowerRegister);
  if (EFI_ERROR (Status)) {
    PowerRegister = 0;
  }
  mGlobalNvsArea.Area->PowerState = (PowerRegister & BIT5) == BIT5;

  mGlobalNvsArea.Area->NativePCIESupport        = PcdGet8 (PcdPciExpNative);

  //
  // Enable APIC
  //
  mGlobalNvsArea.Area->ApicEnable = GLOBAL_NVS_DEVICE_ENABLE;

  //
  // Low Power S0 Idle - Enabled/Disabled
  //
  mGlobalNvsArea.Area->LowPowerS0Idle = PcdGet8 (PcdLowPowerS0Idle);

  mGlobalNvsArea.Area->Ps2MouseEnable     = PcdGet8 (PcdPs2KbMsEnable);
  mGlobalNvsArea.Area->Ps2KbMsEnable      = PcdGet8 (PcdPs2KbMsEnable);

  mGlobalNvsArea.Area->BoardId = (UINT8) LibPcdGetSku ();
}

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardUpdateAcpiTable (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  )
{
  if (Table->Signature == EFI_ACPI_2_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    AspireVn7Dash572GUpdateGlobalNvs ();
  }

  return EFI_SUCCESS;
}
