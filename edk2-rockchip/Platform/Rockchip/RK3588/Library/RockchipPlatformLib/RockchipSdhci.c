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
#include <Library/TimerLib.h>

#define word32   *(volatile unsigned int *)(long)
#define word16   *(volatile unsigned short *)(long)

//#define GRF_BASE            0xFDC60000
#define NS_CRU_BASE         0xFD7C0000
#define CRU_CLKSEL_CON77    0x0434
#define CRU_GPLL_CON1   0x1C4
#define CRU_CPLL_CON1   0x1A4

#define EMMC_CLOCK_BASE     (NS_CRU_BASE + CRU_CLKSEL_CON77)
#define EMMC_CLOCK_SEL(a)   (((7 << 12) << 16) | (a << 12))

#define EMMC_BASE           PcdGet32 (PcdSdhciDxeBaseAddress)
#define SDHCI_HOST_CTRL2  (EMMC_BASE + 0x3E)
#define SDHCI_HOST_CTRL3  (EMMC_BASE + 0x508)
#define SDHCI_EMMC_CTRL   (EMMC_BASE + 0x52C)

#define DWCMSHC_CARD_IS_EMMC		BIT0

#define EMMC_DLL_CTRL       (EMMC_BASE + 0x800)
#define EMMC_DLL_RXCLK      (EMMC_BASE + 0x804)
#define EMMC_DLL_TXCLK      (EMMC_BASE + 0x808)
#define EMMC_DLL_STRBIN     (EMMC_BASE + 0x80C)
#define EMMC_DLL_CMDOUT     (EMMC_BASE + 0x810)
#define EMMC_DLL_STATUS0    (EMMC_BASE + 0x840)
#define EMMC_DLL_STATUS1    (EMMC_BASE + 0x844)

EFI_STATUS SdhciSetPHY(
  IN UINTN Clock
  )
{
  UINT32 Ctrl2, Ctrl;
  UINT32 status, timeout, tmp;

  word32(SDHCI_EMMC_CTRL) |= (1 << 0); /* Host Controller is an eMMC card */
  if (Clock < 100000000) {
    word32(EMMC_DLL_CTRL) = 0;
    word32(EMMC_DLL_RXCLK) = 0; /* PIO mode need set bit 29*/
    word32(EMMC_DLL_TXCLK) = 0;
    word32(EMMC_DLL_STRBIN) = (1 << 27) | (1 << 26) | (10 << 16);;
    word32(EMMC_DLL_CMDOUT) = 0;
    goto exit;
  }

  word32(EMMC_DLL_RXCLK) = 0;
  /*Reset DLL*/
  word32(EMMC_DLL_CTRL) = (0x1 << 1);
  MicroSecondDelay(5);
  word32(EMMC_DLL_CTRL)  = 0;
  /*Init DLL*/
  word32(EMMC_DLL_CTRL) = (5 << 16) | (2 << 8) | 0x1;

  /* Wait max 10 ms */
  timeout = 10000;
  while (1) {
    status = word32(EMMC_DLL_STATUS0);
    tmp = (status >> 8) & 0x3;
    if (0x1 == tmp) /*check dll lock*/
      break;

    if (timeout-- > 0)
      MicroSecondDelay(1);
    else {
      DEBUG ((DEBUG_ERROR, "%a timeout status:%x\n", __FUNCTION__, status));
      return EFI_TIMEOUT;
    }
  }

  Ctrl2 = word16(SDHCI_HOST_CTRL2) & 0x0007; /* Get Bus Speed Mode*/
  if (Clock >= 200000000 && Ctrl2 == 0x7) { /*check is HS400 mode*/
    word32(SDHCI_EMMC_CTRL) |= (1 << 8); /* CMD line is sampled using data strobe for HS400 mode */
    word32(EMMC_DLL_RXCLK) = (1 << 27);
    word32(EMMC_DLL_TXCLK) = (1 << 29) | (1 << 27) | (1 << 24) | 0x8;
    word32(EMMC_DLL_STRBIN) = (1 << 27) | (1 << 24) | 0x4;
    word32(EMMC_DLL_CMDOUT) = (1 << 29) | (1 << 28) | (1 << 27) | (1 << 24) | 0x8;

	/* set CARD_IS_EMMC bit to enable Data Strobe for HS400 */
	Ctrl = word32(SDHCI_EMMC_CTRL);
	Ctrl |= DWCMSHC_CARD_IS_EMMC;
	word32(SDHCI_EMMC_CTRL) = Ctrl;
  }
  else { /*config for HS200 mode*/
    word32(EMMC_DLL_RXCLK) = (1 << 29) | (1 << 27);
    word32(EMMC_DLL_TXCLK) = (1 << 27) | (1 << 24) | 0x1;
    word32(EMMC_DLL_STRBIN) = 0;	  //max is 16
  }

exit:
  //DEBUG ((DEBUG_ERROR, "EMMC_DLL_CTRL :0x%x\n", word32(EMMC_DLL_CTRL)));
  //DEBUG ((DEBUG_ERROR, "EMMC_DLL_RXCLK:0x%x\n", word32(EMMC_DLL_RXCLK)));
  //DEBUG ((DEBUG_ERROR, "EMMC_DLL_TXCLK:0x%x\n", word32(EMMC_DLL_TXCLK)));
  //DEBUG ((DEBUG_ERROR, "EMMC_DLL_STRBIN:0x%x\n", word32(EMMC_DLL_STRBIN)));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SdhciGetClockRate(
  IN UINTN TargetFreq,
  OUT UINTN *BaseFreq
  )
{
  UINT32 CClkEmmcSel, Div;

  if (TargetFreq == 0 || BaseFreq == NULL)
    return -1;

  //word32(NS_CRU_BASE + CRU_GPLL_CON1) = 0x70002 << 6; //GPLL:600MHz
  //word32(NS_CRU_BASE + CRU_CPLL_CON1) = 0x70002 << 6; //CPLL:750MHZ

  /*GPLL:1184MHz, CPLL:750MHZ
  2'b00: clk_gpll_mux
  2'b01: clk_cpll_mux
  2'b10: xin_osc0_func*/
  if (TargetFreq >= 200000000) {
    CClkEmmcSel = 0;
    Div = 6;
  } else if (TargetFreq >= 150000000) {
    CClkEmmcSel = 0;
    Div = 8;
  } else if (TargetFreq >= 100000000) {
    CClkEmmcSel = 0;
    Div = 12;
  } else if (TargetFreq >= 50000000) {
    CClkEmmcSel = 0;
    Div = 24;
  } else if (TargetFreq >= 24000000) {
    CClkEmmcSel = 2;
    Div = 1;
  } else { /* 375KHZ*/
    CClkEmmcSel = 2;
    Div = 64;
  }

  DEBUG((DEBUG_ERROR, "+++++SetEmmcClk: %d, %d, %d\n", TargetFreq, CClkEmmcSel, Div));

  *BaseFreq = TargetFreq;
  MmioWrite32(EMMC_CLOCK_BASE, ((( 0x3 << 14 )|( 0x3F << 8 )) << 16 ) |
                                ( CClkEmmcSel << 14 ) |
                                (( Div - 1 ) << 8));

  //DEBUG ((DEBUG_ERROR, "CRU_0X434: 0x%x\n", word32(NS_CRU_BASE + 0x0434)));
  //DEBUG ((DEBUG_ERROR, "CRU_0X438: 0x%x\n", word32(NS_CRU_BASE + 0x0438))); 
  //DEBUG ((DEBUG_ERROR, "GPLL_CON1: 0x%x\n", word32(NS_CRU_BASE + CRU_GPLL_CON1)));
  //DEBUG ((DEBUG_ERROR, "CPLL_CON1: 0x%x\n", word32(NS_CRU_BASE + CRU_CPLL_CON1)));
  SdhciSetPHY(TargetFreq);

  return EFI_SUCCESS;
}

