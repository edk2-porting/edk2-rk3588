/** @file
  Acer Aspire VN7-572G SMM Board ACPI Enable library

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/EcLib.h>

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardEnableAcpi (
  IN BOOLEAN  EnableSci
  )
{
  EFI_STATUS  Status;

  /* Tests at runtime show this re-enables charging and battery reporting
   * - Obtained somewhere from somewhere in vendor's SmmKbcDriver (or RtKbcDriver).
   *   Further reversing will be performed */
  Status = SendEcCommand (0xE9);  /* Vendor implements using ACPI "CMDB" register" */
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): SendEcCommand(0xE9) failed!\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Status = SendEcData (0x81);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): SendEcData(0x81) failed!\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  /* TODO: Set touchpad GPP owner to ACPI? */

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  EFI_STATUS  Status;

  /* Tests at runtime show this disables charging and battery reporting
   * - Obtained somewhere from somewhere in vendor's SmmKbcDriver (or RtKbcDriver).
   *   Further reversing will be performed */
  Status = SendEcCommand (0xE9);  /* Vendor implements using ACPI "CMDB" register" */
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): SendEcCommand(0xE9) failed!\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Status = SendEcData (0x80);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): SendEcData(0x80) failed!\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  /* TODO: Set touchpad GPP owner to GPIO? */

  return EFI_SUCCESS;
}
