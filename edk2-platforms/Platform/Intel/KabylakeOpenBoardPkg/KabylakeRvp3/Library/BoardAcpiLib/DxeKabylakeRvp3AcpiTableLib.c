/** @file
  Kaby Lake RVP 3 Board ACPI Library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
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
#include <Library/AslUpdateLib.h>
#include <Protocol/GlobalNvsArea.h>

#include <PlatformBoardId.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_GLOBAL_NVS_AREA_PROTOCOL              mGlobalNvsArea;

VOID
KabylakeRvp3UpdateGlobalNvs (
  VOID
  )
{

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
  mGlobalNvsArea.Area->PowerState = 1; // AC =1; for mobile platform, will update this value in SmmPlatform.c

  mGlobalNvsArea.Area->NativePCIESupport        = PcdGet8 (PcdPciExpNative);

  //
  // Enable APIC
  //
  mGlobalNvsArea.Area->ApicEnable = GLOBAL_NVS_DEVICE_ENABLE;

  //
  // Low Power S0 Idle - Enabled/Disabled
  //
  mGlobalNvsArea.Area->LowPowerS0Idle = PcdGet8 (PcdLowPowerS0Idle);

  mGlobalNvsArea.Area->Ps2MouseEnable     = FALSE;
  mGlobalNvsArea.Area->Ps2KbMsEnable      = PcdGet8 (PcdPs2KbMsEnable);

  mGlobalNvsArea.Area->BoardId = (UINT8) LibPcdGetSku ();
}

EFI_STATUS
EFIAPI
KabylakeRvp3BoardUpdateAcpiTable (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  )
{
  if (Table->Signature == EFI_ACPI_2_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    KabylakeRvp3UpdateGlobalNvs ();
  }

  return EFI_SUCCESS;
}

