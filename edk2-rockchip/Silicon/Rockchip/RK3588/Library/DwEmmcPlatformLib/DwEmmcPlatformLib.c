/** @file
 *
 *  DwEmmcDxe platform helper library.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/DwEmmcPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/ArmScmiClockProtocol.h>

#define SCMI_CCLK_SD			9

EFI_STATUS
EFIAPI
DwEmmcSetClockRate (
  IN UINTN Frequency
  )
{
  EFI_STATUS            Status;
  SCMI_CLOCK_PROTOCOL   *ClockProtocol;
  EFI_GUID              ClockProtocolGuid = ARM_SCMI_CLOCK_PROTOCOL_GUID;

  Status = gBS->LocateProtocol (&ClockProtocolGuid,
                                NULL,
                                (VOID **) &ClockProtocol);
  ASSERT (!EFI_ERROR (Status));

  Status = ClockProtocol->RateSet (ClockProtocol, SCMI_CCLK_SD, Frequency);
  ASSERT (!EFI_ERROR (Status));

  return Status;
}

VOID
EFIAPI
DwEmmcSetIoMux (
  VOID
  )
{
  return;
}
