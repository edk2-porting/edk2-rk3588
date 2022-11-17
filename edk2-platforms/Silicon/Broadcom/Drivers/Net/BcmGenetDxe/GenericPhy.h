/** @file

  Copyright (c) 2020 Jared McNeill. All rights reserved.
  Copyright (c) 2020 Andrey Warkentin <andrey.warkentin@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef GENERICPHY_H__
#define GENERICPHY_H__

//
// Basic Control Register
//
#define GENERIC_PHY_BMCR                0x00
#define  GENERIC_PHY_BMCR_RESET         BIT15
#define  GENERIC_PHY_BMCR_ANE           BIT12
#define  GENERIC_PHY_BMCR_RESTART_AN    BIT9

//
// Basic Status Register
//
#define GENERIC_PHY_BMSR                0x01
#define  GENERIC_PHY_BMSR_ANEG_COMPLETE BIT5
#define  GENERIC_PHY_BMSR_LINK_STATUS   BIT2

//
// PHY Identifier I & II
//
#define GENERIC_PHY_PHYIDR1             0x02
#define GENERIC_PHY_PHYIDR2             0x03

//
// Auto-Negotiation Advertisement Register
//
#define GENERIC_PHY_ANAR                0x04
#define  GENERIC_PHY_ANAR_100BASETX_FDX BIT8
#define  GENERIC_PHY_ANAR_100BASETX     BIT7
#define  GENERIC_PHY_ANAR_10BASET_FDX   BIT6
#define  GENERIC_PHY_ANAR_10BASET       BIT5

//
// Auto-Negotiation Link Partner Ability Register
//
#define GENERIC_PHY_ANLPAR              0x05

//
// 1000Base-T Control Register
//
#define GENERIC_PHY_GBCR                0x09
#define  GENERIC_PHY_GBCR_1000BASET_FDX BIT9
#define  GENERIC_PHY_GBCR_1000BASET     BIT8

//
// 1000Base-T Status Register
//
#define GENERIC_PHY_GBSR                0x0A

typedef enum {
  PHY_SPEED_NONE  = 0,
  PHY_SPEED_10    = 10,
  PHY_SPEED_100   = 100,
  PHY_SPEED_1000  = 1000
} GENERIC_PHY_SPEED;

typedef enum {
  PHY_DUPLEX_HALF,
  PHY_DUPLEX_FULL
} GENERIC_PHY_DUPLEX;

typedef
EFI_STATUS
(EFIAPI *GENERIC_PHY_READ) (
  IN VOID                     *Priv,
  IN UINT8                    PhyAddr,
  IN UINT8                    Reg,
  OUT UINT16 *                Data
  );

typedef
EFI_STATUS
(EFIAPI *GENERIC_PHY_WRITE) (
  IN VOID                     *Priv,
  IN UINT8                    PhyAddr,
  IN UINT8                    Reg,
  IN UINT16                   Data
  );

typedef
EFI_STATUS
(EFIAPI *GENERIC_PHY_RESET_ACTION) (
  IN VOID                     *Priv
  );

typedef
VOID
(EFIAPI *GENERIC_PHY_CONFIGURE) (
  IN VOID                     *Priv,
  IN GENERIC_PHY_SPEED        Speed,
  IN GENERIC_PHY_DUPLEX       Duplex
  );

typedef struct {
  GENERIC_PHY_READ            Read;
  GENERIC_PHY_WRITE           Write;
  GENERIC_PHY_RESET_ACTION    ResetAction;
  GENERIC_PHY_CONFIGURE       Configure;
  VOID                        *PrivateData;

  UINT8                       PhyAddr;
  BOOLEAN                     LinkUp;
} GENERIC_PHY_PRIVATE_DATA;

EFI_STATUS
EFIAPI
GenericPhyInit (
  IN GENERIC_PHY_PRIVATE_DATA *Phy
  );

EFI_STATUS
EFIAPI
GenericPhyUpdateConfig (
  IN GENERIC_PHY_PRIVATE_DATA *Phy
  );

EFI_STATUS
EFIAPI
GenericPhyReset (
  IN GENERIC_PHY_PRIVATE_DATA *Phy
  );

#endif // GENERICPHY_H__
