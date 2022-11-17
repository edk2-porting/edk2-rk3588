/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#ifndef __MV_PHY_DXE_H__
#define __MV_PHY_DXE_H__

#define MII_BMCR                       0x00  /* Basic mode control Register */
#define MII_BMSR                       0x01  /* Basic mode status Register  */

/* BMCR */
#define BMCR_ANRESTART                 0x0200 /* 1 = Restart autonegotiation */
#define BMCR_ISOLATE                   0x0400 /* 0 = Isolate PHY */
#define BMCR_ANENABLE                  0x1000 /* 1 = Enable autonegotiation */
#define BMCR_RESET                     0x8000 /* 1 = Reset the PHY */

/* BSMR */
#define BMSR_LSTATUS                   0x0004 /* 1 = Link up */
#define BMSR_ANEGCAPABLE               0x0008 /* 1 = Able to perform auto-neg */
#define BMSR_ANEGCOMPLETE              0x0020 /* 1 = Auto-neg complete */

#define PHY_AUTONEGOTIATE_TIMEOUT      5000

/* 88E1011 PHY Status Register */
#define MIIM_88E1xxx_PHY_STATUS        0x11
#define MIIM_88E1xxx_PHYSTAT_SPEED     0xc000
#define MIIM_88E1xxx_PHYSTAT_GBIT      0x8000
#define MIIM_88E1xxx_PHYSTAT_100       0x4000
#define MIIM_88E1xxx_PHYSTAT_DUPLEX    0x2000
#define MIIM_88E1xxx_PHYSTAT_SPDDONE   0x0800
#define MIIM_88E1xxx_PHYSTAT_LINK      0x0400

/* 88E1111 Extended PHY Specific Control Register */
#define MIIM_88E1111_PHY_EXT_CR        0x14
#define MIIM_88E1111_RX_DELAY          0x80
#define MIIM_88E1111_TX_DELAY          0x02

/* 88E1111 Extended PHY Specific Status Register */
#define MIIM_88E1111_PHY_EXT_SR               0x1b
#define MIIM_88E1111_HWCFG_MODE_MASK          0xf
#define MIIM_88E1111_HWCFG_MODE_COPPER_RGMII  0xb
#define MIIM_88E1111_HWCFG_MODE_FIBER_RGMII   0x3
#define MIIM_88E1111_HWCFG_MODE_SGMII_NO_CLK  0x4
#define MIIM_88E1111_HWCFG_MODE_COPPER_RTBI   0x9
#define MIIM_88E1111_HWCFG_FIBER_COPPER_AUTO  0x8000
#define MIIM_88E1111_HWCFG_FIBER_COPPER_RES   0x2000

typedef enum {
  MV_PHY_DEVICE_1512,
  MV_PHY_DEVICE_1112,
  MV_PHY_DEVICE_ID_MAX
} MV_PHY_DEVICE_ID;

typedef
EFI_STATUS
(*MV_PHY_DEVICE_INIT) (
    IN CONST MARVELL_PHY_PROTOCOL *Snp,
    IN OUT PHY_DEVICE *PhyDev
    );

typedef struct {
  MV_PHY_DEVICE_ID DevId;
  MV_PHY_DEVICE_INIT DevInit;
} MV_PHY_DEVICE;

STATIC
EFI_STATUS
MvPhyInit1512 (
    IN CONST MARVELL_PHY_PROTOCOL *Snp,
    IN OUT PHY_DEVICE *PhyDev
    );

/**
  Initialize Marvell 88E1112 PHY.

  @param[in]      MvPhyProtocol   Marvell PHY protocol instance.
  @param[in out] *PhyDevice       PHY device structure.

**/
STATIC
EFI_STATUS
MvPhyInit1112 (
  IN CONST MARVELL_PHY_PROTOCOL  *MvPhyProtocol,
  IN OUT PHY_DEVICE              *PhyDevice
  );

#endif
