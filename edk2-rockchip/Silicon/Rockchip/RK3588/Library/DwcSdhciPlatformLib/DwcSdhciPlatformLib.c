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
#include <Library/IoLib.h>
#include <Library/RockchipPlatformLib.h>

#define NS_CRU_BASE       0xFD7C0000
#define CRU_CLKSEL_CON77  0x0434

#define EMMC_CLOCK_BASE  (NS_CRU_BASE + CRU_CLKSEL_CON77)

EFI_STATUS
EFIAPI
DwcSdhciSetClockRate (
  IN UINTN  Frequency
  )
{
  UINT32  CClkEmmcSel, Div;

  if (Frequency >= 200000000) {
    CClkEmmcSel = 0;
    Div         = 6;
  } else if (Frequency >= 150000000) {
    CClkEmmcSel = 0;
    Div         = 8;
  } else if (Frequency >= 100000000) {
    CClkEmmcSel = 0;
    Div         = 12;
  } else if (Frequency >= 50000000) {
    CClkEmmcSel = 0;
    Div         = 24;
  } else if (Frequency >= 24000000) {
    CClkEmmcSel = 2;
    Div         = 1;
  } else {
    /* 375KHZ*/
    CClkEmmcSel = 2;
    Div         = 64;
  }

  MmioWrite32 (
    EMMC_CLOCK_BASE,
    (((0x3 << 14)|(0x3F << 8)) << 16) |
    (CClkEmmcSel << 14) |
    ((Div - 1) << 8)
    );

  return EFI_SUCCESS;
}

VOID
EFIAPI
DwcSdhciSetIoMux (
  VOID
  )
{
  SdhciEmmcIoMux ();
}
