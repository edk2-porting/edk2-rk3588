/** @file

  Copyright (c) 2011 - 2019, Intel Corporaton. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

  The original software modules are licensed as follows:

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.
  Copyright (c) 2011 - 2014, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#ifndef KSZ9031_PHY_DXE_H__
#define KSZ9031_PHY_DXE_H__

typedef struct {
  UINT32 PhyAddr;
  UINT32 PhyCurrentLink;
  UINT32 PhyOldLink;
} PHY_DRIVER;


//
// PHY Registers
//
#define PHY_BASIC_CTRL                        0
#define PHY_BASIC_STATUS                      1
#define PHY_ID1                               2
#define PHY_ID2                               3
#define PHY_AUTO_NEG_ADVERT                   4
#define PHY_AUTO_NEG_LINK_ABILITY             5
#define PHY_AUTO_NEG_EXP                      6
#define PHY_1000BASE_T_CONTROL                9
#define PHY_1000BASE_T_STATUS                 10
#define PHY_MODE                              17
#define PHY_SPECIAL_MODES                     18
#define PHY_SPECIAL_CTLR                      27
#define PHY_INT_SRC                           29
#define PHY_INT_MASK                          30
#define PHY_SPECIAL_PHY_CTLR                  31

// PHY control register bits
#define PHYCTRL_COLL_TEST                     BIT7            // Collision test enable
#define PHYCTRL_DUPLEX_MODE                   BIT8            // Set Duplex Mode
#define PHYCTRL_RST_AUTO                      BIT9            // Restart Auto-Negotiation of Link abilities
#define PHYCTRL_PD                            BIT11           // Power-Down switch
#define PHYCTRL_AUTO_EN                       BIT12           // Auto-Negotiation Enable
#define PHYCTRL_SPEED_SEL                     BIT13           // Link Speed Selection
#define PHYCTRL_LOOPBK                        BIT14           // Set loopback mode
#define PHYCTRL_RESET                         BIT15           // Do a PHY reset

// PHY status register bits
#define PHYSTS_EXT_CAP                        BIT0            // Extended Capabilities Register capability
#define PHYSTS_JABBER                         BIT1            // Jabber condition detected
#define PHYSTS_LINK_STS                       BIT2            // Link Status
#define PHYSTS_AUTO_CAP                       BIT3            // Auto-Negotiation Capability
#define PHYSTS_REMOTE_FAULT                   BIT4            // Remote fault detected
#define PHYSTS_AUTO_COMP                      BIT5            // Auto-Negotiation Completed
#define PHYSTS_10BASET_HDPLX                  BIT11           // 10Mbps Half-Duplex ability
#define PHYSTS_10BASET_FDPLX                  BIT12           // 10Mbps Full-Duplex ability
#define PHYSTS_100BASETX_HDPLX                BIT13           // 100Mbps Half-Duplex ability
#define PHYSTS_100BASETX_FDPLX                BIT14           // 100Mbps Full-Duplex ability
#define PHYSTS_100BASE_T4                     BIT15           // Base T4 ability

// PHY Auto-Negotiation advertisement
#define PHYANA_SEL_MASK                       ((UINT32)0x1F)   // Link type selector
#define PHYANA_10BASET                        BIT5             // Advertise 10BASET capability
#define PHYANA_10BASETFD                      BIT6             // Advertise 10BASET Full duplex capability
#define PHYANA_100BASETX                      BIT7             // Advertise 100BASETX capability
#define PHYANA_100BASETXFD                    BIT8             // Advertise 100 BASETX Full duplex capability
#define PHYANA_PAUSE_OP_MASK                  (3 << 10)        // Advertise PAUSE frame capability
#define PHYANA_REMOTE_FAULT                   BIT13            // Remote fault detected

#define PHYLPA_SLCT                           0x001f           // Same as advertise selector
#define PHYLPA_10HALF                         0x0020           // Can do 10mbps half-duplex
#define PHYLPA_1000XFULL                      0x0020           // Can do 1000BASE-X full-duplex
#define PHYLPA_10FULL                         0x0040           // Can do 10mbps full-duplex
#define PHYLPA_1000XHALF                      0x0040           // Can do 1000BASE-X half-duplex
#define PHYLPA_100HALF                        0x0080           // Can do 100mbps half-duplex
#define PHYLPA_1000XPAUSE                     0x0080           // Can do 1000BASE-X pause
#define PHYLPA_100FULL                        0x0100           // Can do 100mbps full-duplex
#define PHYLPA_1000XPAUSE_ASYM                0x0100           // Can do 1000BASE-X pause asym
#define PHYLPA_100BASE4                       0x0200           // Can do 100mbps 4k packets
#define PHYLPA_PAUSE_CAP                      0x0400           // Can pause
#define PHYLPA_PAUSE_ASYM                     0x0800           // Can pause asymetrically
#define PHYLPA_RESV                           0x1000           // Unused
#define PHYLPA_RFAULT                         0x2000           // Link partner faulted
#define PHYLPA_LPACK                          0x4000           // Link partner acked us
#define PHYLPA_NPAGE                          0x8000           // Next page bit

#define PHYLPA_DUPLEX                         (LPA_10FULL | LPA_100FULL)
#define PHYLPA_100                            (LPA_100FULL | LPA_100HALF | LPA_100BASE4)

// 1000BASE-T Status register
#define PHYLPA_1000FULL                       0x0800           // Link partner 1000BASE-T full duplex
#define PHYLPA_1000HALF                       0x0400           // Link partner 1000BASE-T half duplex

// 1000BASE-T Control register
#define PHYADVERTISE_1000FULL                 0x0200           // Advertise 1000BASE-T full duplex
#define PHYADVERTISE_1000HALF                 0x0100           // Advertise 1000BASE-T half duplex

#define SPEED_1000                            1000
#define SPEED_100                             100
#define SPEED_10                              10

#define DUPLEX_FULL                           1
#define DUPLEX_HALF                           0

// PHY Super Special control/status
#define PHYSSCS_HCDSPEED_MASK                 (7 << 2)        // Speed indication
#define PHYSSCS_AUTODONE                      BIT12           // Auto-Negotiation Done

// Flags for PHY reset
#define PHY_RESET_PMT                         BIT0
#define PHY_RESET_BCR                         BIT1
#define PHY_RESET_CHECK_LINK                  BIT2

// Flags for auto negotiation
#define AUTO_NEGOTIATE_COLLISION_TEST         BIT0
#define AUTO_NEGOTIATE_ADVERTISE_ALL          BIT1


// Micrel KSZ9031 Extended registers
#define PHY_KSZ9031RN_CONTROL_PAD_SKEW_REG    4
#define PHY_KSZ9031RN_RX_DATA_PAD_SKEW_REG    5
#define PHY_KSZ9031RN_TX_DATA_PAD_SKEW_REG    6
#define PHY_KSZ9031RN_CLK_PAD_SKEW_REG        8

// Data operations
#define PHY_KSZ9031_MOD_DATA_NO_POST_INC      0x1
#define PHY_KSZ9031_MOD_DATA_POST_INC_RW      0x2
#define PHY_KSZ9031_MOD_DATA_POST_INC_W       0x3

#define PHY_KSZ9031RN_MMD_CTRL_REG            0x0d
#define PHY_KSZ9031RN_MMD_REGDATA_REG         0x0e

#define PHY_KSZ9031RN_CLK_SKEW_CLR_MASK       0x3FF
#define PHY_KSZ9031RN_CONTROL_SKEW_CLR_MASK   0xFF
#define PHY_KSZ9031RN_RX_DATA_SKEW_CLR_MASK   0xFF
#define PHY_KSZ9031RN_TX_DATA_SKEW_CLR_MASK   0xFF

#define PHY_KSZ9031RN_CLK_PAD_SKEW_VALUE      0x3FC
#define PHY_KSZ9031RN_CONTROL_PAD_SKEW_VALUE  0x70
#define PHY_KSZ9031RN_RX_DATA_PAD_SKEW_VALUE  0x7777
#define PHY_KSZ9031RN_TX_DATA_PAD_SKEW_VALUE  0x0


#define PHY_KSZ9031RN_DEV_ADDR                0x2

// MMD Address 0h, Auto-Negotiation FLP burst transmit timing
#define PHY_KSZ9031RN_MMD_DEV_ADDR_00         0x00
#define PHY_KSZ9031RN_MMD_D0_FLP_LO_REG       3
#define PHY_KSZ9031RN_MMD_D0_FLP_16MS_LO      0x1A80
#define PHY_KSZ9031RN_MMD_D0_FLP_HI_REG       4
#define PHY_KSZ9031RN_MMD_D0_FLP_16MS_HI      0x0006

// HPS MII
#define MII_BUSY                              (1 << 0)
#define MII_WRITE                             (1 << 1)
#define MII_CLKRANGE_60_100M                  (0x0)
#define MII_CLKRANGE_100_150M                 (0x4)
#define MII_CLKRANGE_20_35M                   (0x8)
#define MII_CLKRANGE_35_60M                   (0xC)
#define MII_CLKRANGE_150_250M                 (0x10)
#define MII_CLKRANGE_250_300M                 (0x14)

#define MIIADDRSHIFT                          (11)
#define MIIREGSHIFT                           (6)
#define MII_REGMSK                            (0x1F << 6)
#define MII_ADDRMSK                           (0x1F << 11)

// Others
#define PHY_INVALID_ID                        0xFFFF
#define LINK_UP                               1
#define LINK_DOWN                             0
#define PHY_TIMEOUT                           200000


EFI_STATUS
EFIAPI
PhyDxeInitialization (
  IN  PHY_DRIVER     *PhyDriver,
  IN  UINTN          MacBaseAddress
  );

EFI_STATUS
EFIAPI
PhyDetectDevice (
  IN  PHY_DRIVER     *PhyDriver,
  IN  UINTN          MacBaseAddress
  );

EFI_STATUS
EFIAPI
PhyConfig (
  IN  PHY_DRIVER     *PhyDriver,
  IN  UINTN          MacBaseAddress
  );

EFI_STATUS
EFIAPI
PhySoftReset (
  IN  PHY_DRIVER    *PhyDriver,
  IN  UINTN         MacBaseAddress
  );

EFI_STATUS
EFIAPI
PhyReadId (
  IN  UINT32        PhyAddr,
  IN  UINTN         MacBaseAddress
  );

VOID
EFIAPI
PhyConfigSkew (
  IN  PHY_DRIVER    *PhyDriver,
  IN  UINTN         MacBaseAddress
  );

VOID
EFIAPI
PhyDisplayConfigSkew (
  IN  PHY_DRIVER    *PhyDriver,
  IN  UINTN         MacBaseAddress
  );

VOID
EFIAPI
PhyConfigFlpBurstTiming (
  IN  PHY_DRIVER    *PhyDriver,
  IN  UINTN         MacBaseAddress
  );

VOID
EFIAPI
PhyDisplayFlpBurstTiming (
  IN  PHY_DRIVER    *PhyDriver,
  IN  UINTN         MacBaseAddress
  );

EFI_STATUS
EFIAPI
PhyAutoNego (
  IN  PHY_DRIVER    *PhyDriver,
  IN  UINTN         MacBaseAddress
  );

EFI_STATUS
EFIAPI
PhyLinkAdjustEmacConfig (
  IN  PHY_DRIVER    *PhyDriver,
  IN  UINTN         MacBaseAddress
  );

EFI_STATUS
EFIAPI
PhyCheckLinkStatus (
  IN  PHY_DRIVER    *PhyDriver,
  IN  UINTN         MacBaseAddress
  );

EFI_STATUS
EFIAPI
PhyReadCapability (
  IN  PHY_DRIVER   *PhyDriver,
  IN  UINT32       *Speed,
  IN  UINT32       *Duplex,
  IN  UINTN        MacBaseAddress
  );

VOID
EFIAPI
PhyDisplayAbility (
  IN  UINT32       Speed,
  IN  UINT32       Duplex
  );

EFI_STATUS
EFIAPI
PhyRead (
  IN  UINT32       Addr,
  IN  UINT32       Reg,
  OUT UINT32       *Data,
  IN  UINTN        MacBaseAddress
  );

EFI_STATUS
EFIAPI
PhyWrite (
  IN  UINT32       Addr,
  IN  UINT32       Reg,
  IN  UINT32       Data,
  IN  UINTN        MacBaseAddress
  );

EFI_STATUS
EFIAPI
Phy9031ExtendedWrite (
  IN  PHY_DRIVER   *PhyDriver,
  IN  UINT32       Mode,
  IN  UINT32       DevAddr,
  IN  UINT32       Regnum,
  IN  UINT16       Val,
  IN  UINTN        MacBaseAddress
  );

UINT32
EFIAPI
Phy9031ExtendedRead (
  IN  PHY_DRIVER   *PhyDriver,
  IN  UINT32       Mode,
  IN  UINT32       DevAddr,
  IN  UINT32       Regnum,
  IN  UINTN        MacBaseAddress
  );

#endif /* KSZ9031_PHY_DXE_H__ */
