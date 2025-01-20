/** @file
 *
 *  DwcSdhciDxe platform helper library.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

EFI_STATUS
EFIAPI
DwcSdhciSetClockRate (
  IN UINTN  Frequency
  );

VOID
EFIAPI
DwcSdhciSetIoMux (
  VOID
  );
