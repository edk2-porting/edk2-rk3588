/** @file
 *
 *  Copyright (c) 2017, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "RockchipSdhci.h"
#include <Library/PcdLib.h>

#define GRF_BASE            0xFDC60000
#define NS_CRU_BASE         0xFDD20000
#define CRU_CLKSEL_CON28    0x0170

#define EMMC_CLOCK_BASE     (NS_CRU_BASE + CRU_CLKSEL_CON28)
#define EMMC_CLOCK_SEL(a)   (((7 << 12) << 16) | (a << 12))

#define EMMCPHY_BASE		PcdGet32 (PcdSdhciDxeBaseAddress)
#define DWCMSHC_HOST_CTRL3	(EMMC_BASE + 0x508)
#define EMMC_DLL_CTRL		(EMMC_BASE + 0x800)
#define EMMC_DLL_RXCLK		(EMMC_BASE + 0x804)
#define EMMC_DLL_TXCLK		(EMMC_BASE + 0x808)
#define EMMC_DLL_STRBIN		(EMMC_BASE + 0x80C)
#define EMMC_DLL_STATUS0	(EMMC_BASE + 0x840)
#define EMMC_DLL_STATUS1	(EMMC_BASE + 0x844)

EFI_STATUS SdhciSetPHY(
  IN UINTN Clock
  )
{
  UINT32 Tmp;

  /* Disable cmd conflict check */
  Tmp = MmioRead32(DWCMSHC_HOST_CTRL3);
  Tmp &= ~BIT0;
  MmioWrite32(EMMCPHY_BASE + DWCMSHC_HOST_CTRL3, Tmp);

  if (Clock < 100000000) {
    MmioWrite32(EMMC_DLL_CTRL, 0);
    MmioWrite32(EMMC_DLL_RXCLK, 1<<29); /* PIO mode need set bit 29*/
    MmioWrite32(EMMC_DLL_TXCLK, 0);
    MmioWrite32(EMMC_DLL_STRBIN, 0);
    return EFI_SUCCESS;
  }
  else {
    return RETURN_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SdhciGetClockRate(
  IN UINTN TargetFreq,
  OUT UINTN *BaseFreq
  )
{
  UINT32 CClkEmmcSel;

  if (TargetFreq == 0 || BaseFreq == NULL)
    return -1;
  
  if (TargetFreq >= 200000000) {
    CClkEmmcSel = 1;
  } else if (TargetFreq >= 150000000) {
    CClkEmmcSel = 2;
  } else if (TargetFreq >= 100000000) {
    CClkEmmcSel = 3;
  } else if (TargetFreq >= 50000000) {
    CClkEmmcSel = 4;
  } else if (TargetFreq >= 24000000) {
    CClkEmmcSel = 0;
  } else {
    CClkEmmcSel = 5; /* 375KHZ */
  }
  
  SdhciSetPHY(TargetFreq);
  *BaseFreq= TargetFreq;

  DEBUG ((DEBUG_BLKIO, "MMCHost: Clock: %d\n", TargetFreq));
  MmioWrite32(EMMC_CLOCK_BASE, EMMC_CLOCK_SEL(CClkEmmcSel));
  DEBUG ((DEBUG_BLKIO, "EMMC_CLOCK 0x%x: 0x%x:\n", EMMC_CLOCK_BASE, MmioRead32(EMMC_CLOCK_BASE)));
  //DEBUG ((DEBUG_BLKIO, "GPIO1B_IOMUX_H: 0x%x:\n", MmioRead32(GRF_BASE+0x0C)));
  //DEBUG ((DEBUG_BLKIO, "GPIO1C_IOMUX_L: 0x%x:\n", MmioRead32(GRF_BASE+0x10)));
  //DEBUG ((DEBUG_BLKIO, "GPIO1C_IOMUX_H: 0x%x:\n", MmioRead32(GRF_BASE+0x14)));

  //DEBUG ((DEBUG_BLKIO, "EMMC_DLL_CTRL: 0x%x:\n", MmioRead32(EMMC_DLL_CTRL)));
  //DEBUG ((DEBUG_BLKIO, "EMMC_DLL_RXCLK: 0x%x:\n", MmioRead32(EMMC_DLL_RXCLK)));
  //DEBUG ((DEBUG_BLKIO, "EMMC_DLL_TXCLK: 0x%x:\n", MmioRead32(EMMC_DLL_TXCLK)));

  return EFI_SUCCESS;
}

