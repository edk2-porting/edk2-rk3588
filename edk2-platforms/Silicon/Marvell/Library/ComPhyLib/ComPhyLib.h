/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __COMPHY_H__
#define __COMPHY_H__

#include <Uefi.h>
#include <Library/ArmLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MvComPhyLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/BoardDesc.h>

#include "ComPhySipSvc.h"

#define MAX_LANE_OPTIONS          10

/***** Parsing PCD *****/
#define GET_LANE_TYPE(id)         PcdGetPtr(PcdChip##id##ComPhyTypes)
#define GET_LANE_SPEED(id)        PcdGetPtr(PcdChip##id##ComPhySpeeds)
#define GET_LANE_INV(id)          PcdGetPtr(PcdChip##id##ComPhyInvFlags)

#define GetComPhyPcd(lane_struct, id) {                      \
  lane_struct[id].Type = (UINT8 *)GET_LANE_TYPE(id);         \
  lane_struct[id].SpeedValue = (UINT8 *)GET_LANE_SPEED(id);  \
  lane_struct[id].InvFlag = (UINT8 *)GET_LANE_SPEED(id);     \
}

#define COMPHY_TYPE_UNCONNECTED                      0
#define COMPHY_TYPE_PCIE0                            1
#define COMPHY_TYPE_PCIE1                            2
#define COMPHY_TYPE_PCIE2                            3
#define COMPHY_TYPE_PCIE3                            4
#define COMPHY_TYPE_SATA0                            5
#define COMPHY_TYPE_SATA1                            6
#define COMPHY_TYPE_SATA2                            7
#define COMPHY_TYPE_SATA3                            8
#define COMPHY_TYPE_SGMII0                           9
#define COMPHY_TYPE_SGMII1                           10
#define COMPHY_TYPE_SGMII2                           11
#define COMPHY_TYPE_SGMII3                           12
#define COMPHY_TYPE_QSGMII                           13
#define COMPHY_TYPE_USB3_HOST0                       14
#define COMPHY_TYPE_USB3_HOST1                       15
#define COMPHY_TYPE_USB3_DEVICE                      16
#define COMPHY_TYPE_XAUI0                            17
#define COMPHY_TYPE_XAUI1                            18
#define COMPHY_TYPE_XAUI2                            19
#define COMPHY_TYPE_XAUI3                            20
#define COMPHY_TYPE_RXAUI0                           21
#define COMPHY_TYPE_RXAUI1                           22
#define COMPHY_TYPE_SFI                              23
#define COMPHY_TYPE_MAX                              24
#define COMPHY_TYPE_INVALID                          0xff

/***** SATA registers *****/
#define SATA3_VENDOR_ADDRESS                      0xA0
#define SATA3_VENDOR_ADDR_OFSSET                  0
#define SATA3_VENDOR_ADDR_MASK                    (0xFFFFFFFF << SATA3_VENDOR_ADDR_OFSSET)
#define SATA3_VENDOR_DATA                         0xA4

#define SATA_CONTROL_REG                          0x0
#define SATA3_CTRL_SATA0_PD_OFFSET                6
#define SATA3_CTRL_SATA0_PD_MASK                  (1 << SATA3_CTRL_SATA0_PD_OFFSET)
#define SATA3_CTRL_SATA1_PD_OFFSET                14
#define SATA3_CTRL_SATA1_PD_MASK                  (1 << SATA3_CTRL_SATA1_PD_OFFSET)
#define SATA3_CTRL_SATA1_ENABLE_OFFSET            22
#define SATA3_CTRL_SATA1_ENABLE_MASK              (1 << SATA3_CTRL_SATA1_ENABLE_OFFSET)
#define SATA3_CTRL_SATA_SSU_OFFSET                23
#define SATA3_CTRL_SATA_SSU_MASK                  (1 << SATA3_CTRL_SATA_SSU_OFFSET)

#define SATA_MBUS_SIZE_SELECT_REG                 0x4
#define SATA_MBUS_REGRET_EN_OFFSET                7
#define SATA_MBUS_REGRET_EN_MASK                  (0x1 << SATA_MBUS_REGRET_EN_OFFSET)

/***************************/

typedef struct _CHIP_COMPHY_CONFIG CHIP_COMPHY_CONFIG;

typedef struct {
  UINT32 Type;
  UINT32 MuxValue;
} COMPHY_MUX_OPTIONS;

typedef struct {
  UINT8 Type;
  UINT8 Speed;
  UINT8 Invert;
} COMPHY_MAP;

typedef struct {
  UINT8 *Type;
  UINT8 *SpeedValue;
  UINT8 *InvFlag;
} PCD_LANE_MAP;

typedef
VOID
(*COMPHY_CHIP_INIT) (
  IN CHIP_COMPHY_CONFIG *PtrChipCfg
  );

struct _CHIP_COMPHY_CONFIG {
  MV_COMPHY_CHIP_TYPE ChipType;
  COMPHY_MAP MapData[MAX_LANE_OPTIONS];
  EFI_PHYSICAL_ADDRESS ComPhyBaseAddr;
  EFI_PHYSICAL_ADDRESS Hpipe3BaseAddr;
  COMPHY_CHIP_INIT Init;
  UINT32 LanesCount;
  UINT32 MuxBitCount;
  UINT8 ChipId;
};

VOID
ComPhyCp110Init (
  IN CHIP_COMPHY_CONFIG * First
  );

VOID
RegSet (
  IN EFI_PHYSICAL_ADDRESS Addr,
  IN UINT32 Data,
  IN UINT32 Mask
  );

VOID
RegSetSilent (
  IN EFI_PHYSICAL_ADDRESS Addr,
  IN UINT32 Data,
  IN UINT32 Mask
  );

#endif // __COMPHY_H__
