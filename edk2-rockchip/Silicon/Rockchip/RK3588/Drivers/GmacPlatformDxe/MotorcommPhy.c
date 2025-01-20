/** @file
 *
 *  Copyright (c) 2022, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>

#include "EthernetPhy.h"

/* Motorcomm PHY registers */
#define EXT_REG_ADDR             0x1E
#define EXT_REG_DATA             0x1F
#define PHY_CLOCK_GATING_REG     0x0C
#define  RX_CLK_EN               BIT12
#define  TX_CLK_DELAY_SEL_SHIFT  4
#define  TX_CLK_DELAY_SEL        (0xFU << TX_CLK_DELAY_SEL_SHIFT)
#define  CLK_25M_SEL_SHIFT       1
#define  CLK_25M_SEL_MASK        (0x3U << CLK_25M_SEL_SHIFT)
#define  CLK_25M_SEL_125M        (3U << CLK_25M_SEL_SHIFT)
#define  RX_CLK_DELAY_EN         BIT0
#define PHY_SLEEP_CONTROL1_REG   0x27
#define  SLEEP_SW                BIT15
#define  PLLON_IN_SLP            BIT14

#define YTPHY_SYNCE_CFG_REG               0xA012
#define  YT8531_SCR_SYNCE_ENABLE          BIT6
#define  YT8531_SCR_CLK_FRE_SEL_125M      BIT4
#define  YT8531_SCR_CLK_SRC_SEL_SHIFT     1
#define  YT8531_SCR_CLK_SRC_SEL_PLL_125M  (0 << YT8531_SCR_CLK_SRC_SEL_SHIFT)

#define LED1_CFG_REG       0xA00D
#define LED2_CFG_REG       0xA00E
#define LED_BLINK_CFG_REG  0xA00F

STATIC
VOID
YT8531PhyInit (
  IN EFI_PHYSICAL_ADDRESS  GmacBase
  )
{
  UINT16  OldAddr;
  UINT16  Data;

  PhyRead (GmacBase, 0, EXT_REG_ADDR, &OldAddr);

  PhyWrite (GmacBase, 0, EXT_REG_ADDR, YTPHY_SYNCE_CFG_REG);
  PhyWrite (
    GmacBase,
    0,
    EXT_REG_DATA,
    YT8531_SCR_SYNCE_ENABLE |
    YT8531_SCR_CLK_FRE_SEL_125M |
    YT8531_SCR_CLK_SRC_SEL_PLL_125M
    );

  PhyWrite (GmacBase, 0, EXT_REG_ADDR, PHY_CLOCK_GATING_REG);
  PhyRead (GmacBase, 0, EXT_REG_DATA, &Data);
  Data &= ~RX_CLK_EN;
  Data &= ~CLK_25M_SEL_MASK;
  Data |= CLK_25M_SEL_125M;
  PhyWrite (GmacBase, 0, EXT_REG_DATA, Data);

  PhyWrite (GmacBase, 0, EXT_REG_ADDR, PHY_SLEEP_CONTROL1_REG);
  PhyRead (GmacBase, 0, EXT_REG_DATA, &Data);
  Data &= ~SLEEP_SW;
  PhyWrite (GmacBase, 0, EXT_REG_DATA, Data);

  PhyWrite (GmacBase, 0, EXT_REG_ADDR, LED1_CFG_REG);
  PhyWrite (GmacBase, 0, EXT_REG_DATA, 0x0670);

  PhyWrite (GmacBase, 0, EXT_REG_ADDR, LED2_CFG_REG);
  PhyWrite (GmacBase, 0, EXT_REG_DATA, 0x2070);

  PhyWrite (GmacBase, 0, EXT_REG_ADDR, LED_BLINK_CFG_REG);
  PhyWrite (GmacBase, 0, EXT_REG_DATA, 0x007e);

  PhyWrite (GmacBase, 0, EXT_REG_ADDR, OldAddr);
}

EFI_STATUS
EFIAPI
MotorcommPhyInit (
  IN EFI_PHYSICAL_ADDRESS  GmacBase,
  IN UINT32                PhyId
  )
{
  switch (PhyId) {
    case 0x4F51E91B:
      DEBUG ((DEBUG_INFO, "%a: Found Motorcomm YT8531 GbE PHY\n", __func__));
      YT8531PhyInit (GmacBase);
      break;
    default:
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
