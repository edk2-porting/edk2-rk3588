/** @file
 *
 *  RkSdmmcDxe platform helper library.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/RkSdmmcPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/GpioLib.h>
#include <Library/RockchipPlatformLib.h>
#include <Protocol/ArmScmiClockProtocol.h>

#define SCMI_CCLK_SD			9

EFI_STATUS
EFIAPI
RkSdmmcSetClockRate (
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
RkSdmmcSetIoMux (
  VOID
  )
{
  GpioPinSetDirection (0, GPIO_PIN_PA4, GPIO_PIN_INPUT);
  SdmmcIoMux ();
}

RKSDMMC_CARD_PRESENCE_STATE
EFIAPI
RkSdmmcGetCardPresenceState (
  VOID
  )
{
  return GpioPinReadActual (0, GPIO_PIN_PA4) ? RkSdmmcCardNotPresent
                                             : RkSdmmcCardPresent;
}
