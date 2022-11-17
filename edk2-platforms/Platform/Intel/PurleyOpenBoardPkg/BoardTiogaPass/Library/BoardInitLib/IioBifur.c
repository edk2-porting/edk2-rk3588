/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2021, American Megatrends International LLC.<BR>
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
  { Iio_Socket0, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 }, // Uplink x16 
  { Iio_Socket0, Iio_Iou1, IIO_BIFURCATE_x4x4x4x4 }, // x24 upper x16
  { Iio_Socket0, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 }, // OCP-Mezz
  { Iio_Socket0, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 }, 
  { Iio_Socket0, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 }, // un-used
  { Iio_Socket1, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 }, // un-used
  { Iio_Socket1, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 }, // combine AirMax A & B to x16
  { Iio_Socket1, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
};

UINT8 mIioBifurcationTableEntries = sizeof(mIioBifurcationTable)/sizeof(IIO_BIFURCATION_ENTRY);

IIO_SLOT_CONFIG_ENTRY   mIioSlotTable[] = {
    // Port        |  Slot      | Inter      | Power Limit | Power Limit | Hot     | Vpp          | Vpp          | PcieSSD | PcieSSD     | PcieSSD       | Hidden
    // Index       |            | lock       | Scale       |  Value      | Plug    | Port         | Addr         | Cap     | VppPort     | VppAddr       |
    { PORT_1A_INDEX,  NO_SLT_IMP, DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , NOT_HIDE}, // x16 uplink[15:0]
    { PORT_2A_INDEX,  4         , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE , VPP_PORT_MAX , VPP_ADDR_MAX ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , NOT_HIDE}, // x24 upper x16
    { PORT_2B_INDEX,  9         , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE , VPP_PORT_MAX , VPP_ADDR_MAX ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , NOT_HIDE}, // x24 upper x16
    { PORT_2C_INDEX,  10        , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE , VPP_PORT_MAX , VPP_ADDR_MAX ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , NOT_HIDE}, // x24 upper x16
    { PORT_2D_INDEX,  11        , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE , VPP_PORT_MAX , VPP_ADDR_MAX ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , NOT_HIDE}, // x24 upper x16
    { PORT_3A_INDEX,  7         , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE , VPP_PORT_MAX , VPP_ADDR_MAX ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , NOT_HIDE}, // OCP Mezz
    { SOCKET_1_INDEX +
      PORT_0_INDEX,   NO_SLT_IMP, DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , HIDE    }, // CPU1 DMI unused
    { SOCKET_1_INDEX +
      PORT_1A_INDEX,  NO_SLT_IMP, DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , HIDE    }, // CPU1 Port1 unused
    { SOCKET_1_INDEX +
      PORT_2A_INDEX,  NO_SLT_IMP, DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , HIDE    }, // CPU1 Port2 unused
    { SOCKET_1_INDEX +
      PORT_3A_INDEX,  NO_SLT_IMP, DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE , VPP_PORT_0    , 0x40         ,  DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX  , HIDE}, // Airmax A & B
};

UINT8 mIioSlotTableEntries = sizeof(mIioSlotTable)/sizeof(IIO_SLOT_CONFIG_ENTRY);