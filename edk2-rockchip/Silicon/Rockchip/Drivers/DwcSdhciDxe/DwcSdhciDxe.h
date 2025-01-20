/** @file
 *
 *  Synopsys DesignWare Cores SDHCI eMMC driver
 *
 *  Copyright (c) 2022, Patrick Wildt <patrick@blueri.se>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __DWCSDHCIDXE_H__
#define __DWCSDHCIDXE_H__

#define DWC_SDHCI_BASE  PcdGet32 (PcdDwcSdhciBaseAddress)

#define SD_MMC_HC_CLOCK_CTRL  (DWC_SDHCI_BASE + 0x2C)
#define SD_MMC_HC_HOST_CTRL2  (DWC_SDHCI_BASE + 0x3E)

// eMMC Registers
#define EMMC_HOST_CTRL3   (DWC_SDHCI_BASE + 0x508)
#define EMMC_EMMC_CTRL    (DWC_SDHCI_BASE + 0x52C)
#define EMMC_DLL_CTRL     (DWC_SDHCI_BASE + 0x800)
#define EMMC_DLL_RXCLK    (DWC_SDHCI_BASE + 0x804)
#define EMMC_DLL_TXCLK    (DWC_SDHCI_BASE + 0x808)
#define EMMC_DLL_STRBIN   (DWC_SDHCI_BASE + 0x80C)
#define EMMC_DLL_CMDOUT   (DWC_SDHCI_BASE + 0x810)
#define EMMC_DLL_STATUS0  (DWC_SDHCI_BASE + 0x840)
#define EMMC_DLL_STATUS1  (DWC_SDHCI_BASE + 0x844)

#define CLOCK_CTRL_SDCLK_ENABLE  BIT2

#define HOST_CTRL2_HS400  (BIT2 | BIT1 | BIT0)

#define EMMC_CTRL_CARD_IS_EMMC  BIT0

#define EMMC_DLL_CTRL_SRST                 BIT1
#define EMMC_DLL_CTRL_START                BIT0
#define EMMC_DLL_CTRL_START_POINT_DEFAULT  (5 << 16)
#define EMMC_DLL_CTRL_INCREMENT_DEFAULT    (2 << 8)

#define EMMC_DLL_NO_INVERTER     BIT29
#define EMMC_DLL_DLYENA          BIT27
#define EMMC_DLL_TAPNUM_FROM_SW  BIT24

#define EMMC_DLL_TXCLK_TAPNUM_DEFAULT     (0x10 << 0)
#define EMMC_DLL_TXCLK_TAPNUM_90_DEGREES  0x9

#define EMMC_DLL_STRBIN_TAPNUM_DEFAULT     (0x3 << 0)
#define EMMC_DLL_STRBIN_DELAY_NUM_SEL      BIT26
#define EMMC_DLL_STRBIN_DELAY_NUM_OFFSET   16
#define EMMC_DLL_STRBIN_DELAY_NUM_DEFAULT  0x10

#define EMMC_DLL_CMDOUT_TAPNUM_90_DEGREES  0x8
#define EMMC_DLL_CMDOUT_SRC_CLK_NEG        BIT28
#define EMMC_DLL_CMDOUT_EN_SRC_CLK_NEG     BIT29

#define EMMC_DLL_STATUS0_DLL_LOCK     BIT8
#define EMMC_DLL_STATUS0_DLL_TIMEOUT  BIT9

typedef struct {
  UINT32    TimeoutFreq   : 6; // bit 0:5
  UINT32    Reserved      : 1; // bit 6
  UINT32    TimeoutUnit   : 1; // bit 7
  UINT32    BaseClkFreq   : 8; // bit 8:15
  UINT32    MaxBlkLen     : 2; // bit 16:17
  UINT32    BusWidth8     : 1; // bit 18
  UINT32    Adma2         : 1; // bit 19
  UINT32    Reserved2     : 1; // bit 20
  UINT32    HighSpeed     : 1; // bit 21
  UINT32    Sdma          : 1; // bit 22
  UINT32    SuspRes       : 1; // bit 23
  UINT32    Voltage33     : 1; // bit 24
  UINT32    Voltage30     : 1; // bit 25
  UINT32    Voltage18     : 1; // bit 26
  UINT32    SysBus64V4    : 1; // bit 27
  UINT32    SysBus64V3    : 1; // bit 28
  UINT32    AsyncInt      : 1; // bit 29
  UINT32    SlotType      : 2; // bit 30:31
  UINT32    Sdr50         : 1; // bit 32
  UINT32    Sdr104        : 1; // bit 33
  UINT32    Ddr50         : 1; // bit 34
  UINT32    Reserved3     : 1; // bit 35
  UINT32    DriverTypeA   : 1; // bit 36
  UINT32    DriverTypeC   : 1; // bit 37
  UINT32    DriverTypeD   : 1; // bit 38
  UINT32    DriverType4   : 1; // bit 39
  UINT32    TimerCount    : 4; // bit 40:43
  UINT32    Reserved4     : 1; // bit 44
  UINT32    TuningSDR50   : 1; // bit 45
  UINT32    RetuningMod   : 2; // bit 46:47
  UINT32    ClkMultiplier : 8; // bit 48:55
  UINT32    Reserved5     : 7; // bit 56:62
  UINT32    Hs400         : 1; // bit 63
} SD_MMC_HC_SLOT_CAP;

#endif // __DWCSDHCIDXE_H__
