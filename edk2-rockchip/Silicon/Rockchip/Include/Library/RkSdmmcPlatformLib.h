/** @file
 *
 *  RkSdmmcDxe platform helper library.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

typedef enum {
  RkSdmmcCardPresenceUnsupported = 0,
  RkSdmmcCardPresent,
  RkSdmmcCardNotPresent
} RKSDMMC_CARD_PRESENCE_STATE;

EFI_STATUS
EFIAPI
RkSdmmcSetClockRate (
  IN UINTN  Frequency
  );

VOID
EFIAPI
RkSdmmcSetIoMux (
  VOID
  );

RKSDMMC_CARD_PRESENCE_STATE
EFIAPI
RkSdmmcGetCardPresenceState (
  VOID
  );
