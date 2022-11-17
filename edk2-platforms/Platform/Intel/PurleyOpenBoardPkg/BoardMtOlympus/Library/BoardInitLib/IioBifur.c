/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <IioPlatformData.h>

#include <Guid/HobList.h>
#include <Protocol/IioUds.h>
#include <Library/HobLib.h>
#include <Library/GpioLib.h>
#include <GpioPinsSklH.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <IioBifurcationSlotTable.h>


#define ENABLE            1
#define DISABLE           0
#define NO_SLT_IMP        0xFF
#define SLT_IMP           1
#define HIDE              1
#define NOT_HIDE          0
#define VPP_PORT_0        0
#define VPP_PORT_1        1
#define VPP_PORT_MAX      0xFF
#define VPP_ADDR_MAX      0xFF
#define PWR_VAL_MAX       0xFF
#define PWR_SCL_MAX       0xFF


IIO_BIFURCATION_ENTRY   mIioBifurcationTable[] =
{
  { Iio_Socket0, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },  //Slot3: skt0/Iou0 Port1A x16
  { Iio_Socket0, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },  //PCH uplink x16
  { Iio_Socket0, Iio_Iou2, IIO_BIFURCATE_x4x4x4x4 },  //Slot1: skt0/Iou2 Port3A/3B, Slot2: skt0/Iou Port3C/3D  (x8 slots)
  { Iio_Socket0, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },  //MCP x16
  { Iio_Socket0, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },  //MCP x16
  { Iio_Socket1, Iio_Iou0, IIO_BIFURCATE_xxx8xxx8 },  //Slot4: skt1/IOU0 x16 Port1A/1B, 1C/1D for 2 x8 FPGAs
  { Iio_Socket1, Iio_Iou1, IIO_BIFURCATE_xxx8x4x4 },  //OCulink x8: skt1/Iou1 Port2C/2D, M.2 slots skt1/Iou1 Port1A, 2B (x4x4)
  { Iio_Socket1, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },  //Slot5: skt1/IOU2 x16
  { Iio_Socket1, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },  //MCP
  { Iio_Socket1, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },  //MCP
};

UINT8 mIioBifurcationTableEntries = sizeof(mIioBifurcationTable)/sizeof(IIO_BIFURCATION_ENTRY);

IIO_SLOT_CONFIG_ENTRY   mIioSlotTable[] = {
  // Port        |  Slot      | Inter      | Power Limit | Power Limit | Hot     | Vpp          | Vpp          | PcieSSD | PcieSSD     | PcieSSD       | Hidden
  // Index       |            | lock       | Scale       |  Value      | Plug    | Port         | Addr         | Cap     | VppPort     | VppAddr       |
  { PORT_1A_INDEX, 3, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_0, VPP_ADDR_MAX, HIDE }, //S0Slt3
//  { PORT_1B_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_1, 0x4C, HIDE },
//  { PORT_1C_INDEX, 1, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_MAX, VPP_ADDR_MAX, NOT_HIDE },
  { PORT_2A_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, NOT_HIDE },
  // Slot 2 supports HP:  PCA9555 (CPU0) Addres 0x40, SCH (Rev 0.604) P 118 (MRL in J65)
  { PORT_3A_INDEX, 1, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, ENABLE, VPP_PORT_0, 0x40, ENABLE, VPP_PORT_0, 0x40, NOT_HIDE },
  { PORT_3B_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_1, 0x40, HIDE },
  { PORT_3C_INDEX, 2, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_0, 0x42, HIDE },
  { PORT_3D_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_1, 0x42, HIDE },
  { SOCKET_1_INDEX +
  PORT_0_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, NOT_HIDE },
  // Slot 4 supports HP: PCA9554 (CPU1) Address 0x40, SCH (Rev 0.604) P 121 (MRL in J287)
  { SOCKET_1_INDEX +
  PORT_1A_INDEX, 4, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, NOT_HIDE },  //x16
//  { SOCKET_1_INDEX +
//  PORT_1B_INDEX, NO_SLT_IMP, ENABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_1, 0x40, HIDE },
//  { SOCKET_1_INDEX +
//  PORT_1C_INDEX, NO_SLT_IMP, ENABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_0, 0x42, HIDE },
//  { SOCKET_1_INDEX +
//  PORT_1D_INDEX, NO_SLT_IMP, ENABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_1, 0x42, HIDE },
  { SOCKET_1_INDEX +
  PORT_2A_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_1, VPP_ADDR_MAX, ENABLE, VPP_PORT_0, 0x44, NOT_HIDE },  //x4
  { SOCKET_1_INDEX +
  PORT_2B_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_1, 0x44, HIDE },  //x4
  { SOCKET_1_INDEX +
  PORT_2C_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_0, 0x46, HIDE },  //x8
  { SOCKET_1_INDEX +
//  PORT_2D_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_1, 0x46, HIDE },
//  { SOCKET_1_INDEX +
  PORT_3A_INDEX, 5, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_MAX, VPP_ADDR_MAX, NOT_HIDE }, //x16
//  { SOCKET_1_INDEX +
//  PORT_3C_INDEX, 7, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, ENABLE, VPP_PORT_MAX, VPP_ADDR_MAX, NOT_HIDE },
  // Note: On Neon  City, Slot 3 is assigned to PCH's PCIE port
};

UINT8 mIioSlotTableEntries = sizeof(mIioSlotTable)/sizeof(IIO_SLOT_CONFIG_ENTRY);