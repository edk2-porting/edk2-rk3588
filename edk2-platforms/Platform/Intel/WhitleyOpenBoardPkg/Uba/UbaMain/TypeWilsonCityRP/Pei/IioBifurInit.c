/** @file
  IIO Config Update.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiBoardInit.h"
#include <Library/UbaIioConfigLib.h>
#include <IioPlatformData.h>

typedef enum {
  Iio_Socket0 = 0,
  Iio_Socket1,
  Iio_Socket2,
  Iio_Socket3,
  Iio_Socket4,
  Iio_Socket5,
  Iio_Socket6,
  Iio_Socket7
} IIO_SOCKETS;

typedef enum {
  Iio_Iou0 = 0,
  Iio_Iou1,
  Iio_Iou2,
  Iio_Iou3,
  Iio_Iou4,
  Iio_IouMax
} IIO_IOUS;

typedef enum {
  VPP_PORT_0 = 0,
  VPP_PORT_1,
  VPP_PORT_2,
  VPP_PORT_3
} VPP_PORT;

#define ENABLE            1
#define DISABLE           0


//
//  config file  : Wilson_City_PCIe_Slot_Config_1p70.xlsx
//  config sheet : WilsonCity_ICX
//
static IIO_BIFURCATION_DATA_ENTRY_EX   IioBifurcationTable[] =
{

  { Iio_Socket0, Iio_Iou0, IIO_BIFURCATE_xxxxxx16, VPP_PORT_MAX, SMB_ADDR_MAX, SMB_ADDR_MAX, SMB_DATA_MAX  },
  { Iio_Socket0, Iio_Iou1, IIO_BIFURCATE_xxxxxx16, VPP_PORT_MAX, SMB_ADDR_MAX, SMB_ADDR_MAX, SMB_DATA_MAX  },
  { Iio_Socket0, Iio_Iou2, IIO_BIFURCATE_xxxxxx16, VPP_PORT_MAX, SMB_ADDR_MAX, SMB_ADDR_MAX, SMB_DATA_MAX  },
  { Iio_Socket0, Iio_Iou3, IIO_BIFURCATE_xxxxxx16, 0           , 0x76        , 0xE2        , 4             },
  { Iio_Socket0, Iio_Iou4, IIO_BIFURCATE_x4x4x4x4, VPP_PORT_MAX, SMB_ADDR_MAX, SMB_ADDR_MAX, SMB_DATA_MAX  },

  { Iio_Socket1, Iio_Iou0, IIO_BIFURCATE_xxxxxx16, 0           , 0x70        , 0xE2        , 4             },
  { Iio_Socket1, Iio_Iou1, IIO_BIFURCATE_xxxxxx16, 0           , 0x7C        , 0xE2        , 4             },
  { Iio_Socket1, Iio_Iou2, IIO_BIFURCATE_xxxxxx16, VPP_PORT_MAX, SMB_ADDR_MAX, SMB_ADDR_MAX, SMB_DATA_MAX  },
  { Iio_Socket1, Iio_Iou3, IIO_BIFURCATE_xxxxxx16, VPP_PORT_MAX, SMB_ADDR_MAX, SMB_ADDR_MAX, SMB_DATA_MAX  },
  { Iio_Socket1, Iio_Iou4, IIO_BIFURCATE_x4x4x4x4, 0           , 0x74        , 0xE2        , 4             }
};

static IIO_SLOT_CONFIG_DATA_ENTRY_EX   IioSlotTable[] = {
  // Port Index  | Slot       |Interlock |power       |Power        |Hotplug  |Vpp Port      |Vpp Addr      |PCIeSSD  |PCIeSSD       |PCIeSSD       |Hidden    |Common   |  SRIS   |Uplink   |Retimer  |Retimer       |Retimer       |Retimer    |Mux           |Mux           |ExtnCard |ExtnCard      |ExtnCard      |ExtnCard |ExtnCard Retimer|ExtnCard Retimer|ExtnCard |ExtnCard Hotplug|ExtnCard Hotplug|Max Retimer|
  //             |            |          |Limit Scale |Limit Value  |Cap      |              |              |Cap      |Port          |Address       |          |Clock    |         |Port     |         |Address       |Channel       |Width      |Address       |Channel       |Support  |SMBus Port    |SMBus Addr    |Retimer  |SMBus Address   |Width           |Hotplug  |Vpp Port        |Vpp Address     |           |
  {SOCKET_0_INDEX +
    PORT_1A_INDEX, 6          , DISABLE , 0           , 75          , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_1B_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_1C_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_1D_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_2A_INDEX, 7          , DISABLE , 0           , 75          , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_2B_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_2C_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_2D_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_3A_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_3B_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_3C_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_3D_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_4A_INDEX, 2          , DISABLE , 0           , 200         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x76         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_0     , 0x40           , 0x1      },
  {SOCKET_0_INDEX +
    PORT_4B_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x76         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_1     , 0x40           , 0x1      },
  {SOCKET_0_INDEX +
    PORT_4C_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x76         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_0     , 0x42           , 0x1      },
  {SOCKET_0_INDEX +
    PORT_4D_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x76         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_1     , 0x42           , 0x1      },
  {SOCKET_0_INDEX +
    PORT_5A_INDEX, 10         , DISABLE , 0           , 25          , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , ENABLE  , 0x26         , 2            , 16        , 0xe2         , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x1      },
  {SOCKET_0_INDEX +
    PORT_5B_INDEX, 11         , DISABLE , 0           , 25          , ENABLE  , VPP_PORT_1   , 0x4C         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , ENABLE  , 0x26         , 2            , 16        , 0xe2         , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x1      },
  {SOCKET_0_INDEX +
    PORT_5C_INDEX, 12         , DISABLE , 0           , 25          , ENABLE  , VPP_PORT_0   , 0x4E         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , ENABLE  , 0x26         , 2            , 16        , 0xe2         , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x1      },
  {SOCKET_0_INDEX +
    PORT_5D_INDEX, 13         , DISABLE , 0           , 25          , ENABLE  , VPP_PORT_1   , 0x4E         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , ENABLE  , 0x26         , 2            , 16        , 0xe2         , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x1      },

  {SOCKET_1_INDEX +
    PORT_1A_INDEX, 4          , ENABLE  , 0           , 25          , ENABLE  , VPP_PORT_0   , 0x4A         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x70         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_0     , 0x40           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_1B_INDEX, NO_SLT_IMP , ENABLE  , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE  , VPP_PORT_0   , 0x4A         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x70         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_1     , 0x40           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_1C_INDEX, NO_SLT_IMP , ENABLE  , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE  , VPP_PORT_0   , 0x4A         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x70         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_0     , 0x42           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_1D_INDEX, NO_SLT_IMP , ENABLE  , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE  , VPP_PORT_0   , 0x4A         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x70         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_1     , 0x42           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_2A_INDEX, 9          , DISABLE , 0           , 25          , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x7C         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_0     , 0x44           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_2B_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x7C         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_1     , 0x44           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_2C_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x7C         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_0     , 0x46           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_2D_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x7C         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_1     , 0x46           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_3A_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_1_INDEX +
    PORT_3B_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_1_INDEX +
    PORT_3C_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_1_INDEX +
    PORT_3D_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_1_INDEX +
    PORT_4A_INDEX, 8          , DISABLE , 0           , 25          , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_1_INDEX +
    PORT_4B_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_1_INDEX +
    PORT_4C_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_1_INDEX +
    PORT_4D_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , DISABLE , SMB_ADDR_MAX , SMB_DATA_MAX , NOT_EXIST , SMB_ADDR_MAX , SMB_DATA_MAX , DISABLE , VPP_PORT_MAX , SMB_ADDR_MAX , DISABLE , SMB_ADDR_MAX   , NOT_EXIST      , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_1_INDEX +
    PORT_5A_INDEX, 14         , DISABLE , 0           , 25          , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , ENABLE  , 0x20         , 2            , 16        , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x74         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_0     , 0x40           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_5B_INDEX, 15         , DISABLE , 0           , 25          , ENABLE  , VPP_PORT_1   , 0x4C         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , ENABLE  , 0x20         , 2            , 16        , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x74         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_1     , 0x40           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_5C_INDEX, 16         , DISABLE , 0           , 25          , ENABLE  , VPP_PORT_0   , 0x4E         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , ENABLE  , 0x20         , 2            , 16        , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x74         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_0     , 0x42           , 0x1      },
  {SOCKET_1_INDEX +
    PORT_5D_INDEX, 17         , DISABLE , 0           , 25          , ENABLE  , VPP_PORT_1   , 0x4E         , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , ENABLE  , 0x20         , 2            , 16        , 0xe2         , 4            , ENABLE  , VPP_PORT_0   , 0x74         , ENABLE  , SMB_ADDR_MAX   , NOT_EXIST      , ENABLE  , VPP_PORT_1     , 0x42           , 0x1      }
};

EFI_STATUS
UpdateWilsonCityRPIioConfig (
  IN  IIO_GLOBALS             *IioGlobalData
  )
{
  return EFI_SUCCESS;
}

PLATFORM_IIO_CONFIG_UPDATE_TABLE_EX  TypeWilsonCityRPIioConfigTable =
{
  PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE,
  PLATFORM_IIO_CONFIG_UPDATE_VERSION_2,

  IioBifurcationTable,
  sizeof(IioBifurcationTable),
  UpdateWilsonCityRPIioConfig,
  IioSlotTable,
  sizeof(IioSlotTable)
};

/**
  Entry point function for the PEIM

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    If we installed our PPI

**/
EFI_STATUS
TypeWilsonCityRPIioPortBifurcationInit (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                         Status;
  EFI_HOB_GUID_TYPE                  *GuidHob;
  EFI_PLATFORM_INFO                  *PlatformInfo;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE_EX   *PlatformIioInfoPtr;
  UINTN                              PlatformIioInfoSize;


  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);

  //
  // This is config for ICX
  //
  PlatformIioInfoPtr = &TypeWilsonCityRPIioConfigTable;
  PlatformIioInfoSize = sizeof(TypeWilsonCityRPIioConfigTable);

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformIioConfigDataGuid,
                                 PlatformIioInfoPtr,
                                 PlatformIioInfoSize
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformIioConfigDataGuid_1,
                                 PlatformIioInfoPtr,
                                 PlatformIioInfoSize
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformIioConfigDataGuid_2,
                                 PlatformIioInfoPtr,
                                 PlatformIioInfoSize
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformIioConfigDataGuid_3,
                                 PlatformIioInfoPtr,
                                 PlatformIioInfoSize
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}
