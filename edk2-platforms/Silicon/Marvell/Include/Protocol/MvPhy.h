/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __MV_PHY_H__
#define __MV_PHY_H__

#define MARVELL_PHY_PROTOCOL_GUID { 0x32f48a43, 0x37e3, 0x4acf, { 0x93, 0xc4, 0x3e, 0x57, 0xa7, 0xb0, 0xfb, 0xdc }}

typedef struct _MARVELL_PHY_PROTOCOL MARVELL_PHY_PROTOCOL;

typedef enum {
  PHY_CONNECTION_RGMII,
  PHY_CONNECTION_RGMII_ID,
  PHY_CONNECTION_RGMII_TXID,
  PHY_CONNECTION_RGMII_RXID,
  PHY_CONNECTION_SGMII,
  PHY_CONNECTION_RTBI,
  PHY_CONNECTION_XAUI,
  PHY_CONNECTION_RXAUI,
  PHY_CONNECTION_SFI
} PHY_CONNECTION;

typedef enum {
  NO_SPEED,
  SPEED_10,
  SPEED_100,
  SPEED_1000,
  SPEED_2500,
  SPEED_10000
} PHY_SPEED;

typedef struct {
  UINT32          Addr;
  UINT8           MdioIndex;
  BOOLEAN         LinkUp;
  BOOLEAN         FullDuplex;
  BOOLEAN         AutoNegotiation;
  PHY_SPEED       Speed;
  PHY_CONNECTION  Connection;
} PHY_DEVICE;

/*
 * Before calling MARVELL_PHY_STATUS driver should request PHY_DEVICE structure by
 * calling MARVELL_PHY_INIT. Pointer to that needs to be provided as an argument to
 * MARVELL_PHY_STATUS.
 */
typedef
EFI_STATUS
(EFIAPI *MARVELL_PHY_STATUS) (
  IN CONST MARVELL_PHY_PROTOCOL *This,
  IN OUT PHY_DEVICE  *PhyDev
  );

/*
 * MARVELL_PHY_INIT allocates PhyDev and provides driver with pointer via **PhyDev.
 * After it becomes unnecessary, PhyDev should be freed by a driver (or it will
 * get freed at ExitBootServices).
 */
typedef
EFI_STATUS
(EFIAPI *MARVELL_PHY_INIT) (
  IN CONST MARVELL_PHY_PROTOCOL *This,
  IN UINT32 PhyAddr,
  IN PHY_CONNECTION PhyConnection,
  IN OUT PHY_DEVICE **PhyDev
  );

struct _MARVELL_PHY_PROTOCOL {
  MARVELL_PHY_STATUS Status;
  MARVELL_PHY_INIT Init;
};

extern EFI_GUID gMarvellPhyProtocolGuid;
#endif
