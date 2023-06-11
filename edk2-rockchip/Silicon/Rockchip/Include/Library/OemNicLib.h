/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#ifndef _OEM_NIC_LIB_H_
#define _OEM_NIC_LIB_H_

#define ETH_MAX_PORT          8
#define ETH_DEBUG_PORT0       6
#define ETH_DEBUG_PORT1       7

#define ETH_SPEED_10M     6
#define ETH_SPEED_100M    7
#define ETH_SPEED_1000M   8
#define ETH_SPEED_10KM    9
#define ETH_HALF_DUPLEX   0
#define ETH_FULL_DUPLEX   1

#define ETH_GDD_ID                          0x001378e0
#define ETH_PHY_BCM5241_ID                  0x0143bc30
#define ETH_PHY_MVL88E1145_ID               0x01410cd0
#define ETH_PHY_MVL88E1119_ID               0x01410e80
#define ETH_PHY_MVL88E1512_ID               0x01410dd0
#define ETH_PHY_MVL88E1543_ID               0x01410ea0
#define ETH_PHY_NLP3142_ID                  0x00000412

#define ETH_INVALID                         0xffffffff

typedef struct {
  UINT32 Valid;
  UINT32 Speed;
  UINT32 Duplex;
  UINT32 PhyId;
  UINT32 PhyAddr;
} ETH_PRODUCT_DESC;

BOOLEAN OemIsInitEth (UINT32 Port);
UINT32 OemEthFindFirstSP ();
ETH_PRODUCT_DESC *OemEthInit (UINT32 port);
UINT32 GetCpu1FiberType (UINT8 *Fiber1Type, UINT8 *Fiber2Type);
UINT32 GetCpu2FiberType (UINT8 *Fiber1Type, UINT8 *Fiber2Type, UINT8 *Fiber100Ge);
EFI_STATUS EFIAPI OemGetMac (IN OUT EFI_MAC_ADDRESS *Mac, IN UINTN Port);
EFI_STATUS EFIAPI OemSetMac (IN EFI_MAC_ADDRESS *Mac, IN UINTN Port);

#endif
