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

EFI_STATUS
EFIAPI
RkSdmmcSetClockRate (
  IN UINTN  Frequency
  )
{
  return EFI_UNSUPPORTED;
}

VOID
EFIAPI
RkSdmmcSetIoMux (
  VOID
  )
{
  return;
}

RKSDMMC_CARD_PRESENCE_STATE
EFIAPI
RkSdmmcGetCardPresenceState (
  VOID
  )
{
  return RkSdmmcCardPresenceUnsupported;
}
