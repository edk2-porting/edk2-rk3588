/** @file
 *
 *  DwEmmcDxe platform helper library.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

typedef enum {
  DwEmmcCardPresenceUnsupported = 0,
  DwEmmcCardPresent,
  DwEmmcCardNotPresent
} DWEMMC_CARD_PRESENCE_STATE;

EFI_STATUS
EFIAPI
DwEmmcSetClockRate (
  IN UINTN Frequency
  );

VOID
EFIAPI
DwEmmcSetIoMux (
  VOID
  );

DWEMMC_CARD_PRESENCE_STATE
EFIAPI
DwEmmcGetCardPresenceState (
  VOID
  );
