/** @file
 *
 *  DwcSdhciDxe platform helper library.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/DwcSdhciPlatformLib.h>

EFI_STATUS
EFIAPI
DwcSdhciSetClockRate (
  IN UINTN  Frequency
  )
{
  return EFI_UNSUPPORTED;
}

VOID
EFIAPI
DwcSdhciSetIoMux (
  VOID
  )
{
  return;
}
