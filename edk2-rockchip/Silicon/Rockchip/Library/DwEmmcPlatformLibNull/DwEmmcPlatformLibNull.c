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

EFI_STATUS
EFIAPI
DwEmmcSetClockRate (
  IN UINTN Frequency
  )
{
  return EFI_UNSUPPORTED;
}

VOID
EFIAPI
DwEmmcSetIoMux (
  VOID
  )
{
  return;
}

DWEMMC_CARD_PRESENCE_STATE
EFIAPI
DwEmmcGetCardPresenceState (
  VOID
  )
{
  return DwEmmcCardPresenceUnsupported;
}
