/** @file

  Copyright (c) 2020, ARM Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <IndustryStandard/RpiMbox.h>
#include <Library/SerialPortLib.h>
#include <Library/PcdLib.h>
#include <Protocol/RpiFirmware.h>

EFI_STATUS
EFIAPI
DualSerialPortDxeLibConstructor (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  RASPBERRY_PI_FIRMWARE_PROTOCOL    *Firmware;
  UINT32                            ClockRate;
  EFI_STATUS                        Status;

  Status = SystemTable->BootServices->LocateProtocol (
                                        &gRaspberryPiFirmwareProtocolGuid,
                                        NULL, (VOID **)&Firmware);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Firmware->GetClockRate (RPI_MBOX_CLOCK_RATE_CORE, &ClockRate);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PatchPcdSet32 (PcdSerialClockRate, ClockRate);
  return EFI_SUCCESS;
}
