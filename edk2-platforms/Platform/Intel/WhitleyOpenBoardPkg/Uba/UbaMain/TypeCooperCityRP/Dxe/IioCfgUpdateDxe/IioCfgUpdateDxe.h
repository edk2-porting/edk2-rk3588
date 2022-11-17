/** @file

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IIOCFG_UPDATE_DXE_H_
#define _IIOCFG_UPDATE_DXE_H_

#include <Base.h>
#include <Uefi.h>
#include <Protocol/UbaCfgDb.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciLib.h>
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
  Iio_Iou0 =0,
  Iio_Iou1,
  Iio_Iou2,
  Iio_Mcp0,
  Iio_Mcp1,
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

static IIO_BIFURCATION_DATA_ENTRY   IioBifurcationTable[] =
{
  { Iio_Socket0, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket0, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket0, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket0, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket0, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket2, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket2, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket2, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket2, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket2, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket3, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket3, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket3, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket3, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket3, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
#if MAX_SOCKET > 4
  { Iio_Socket4, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket4, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket4, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket4, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket4, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket5, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket5, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket5, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket5, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket5, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket6, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket6, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket6, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket6, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket6, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket7, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket7, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket7, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket7, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket7, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
#endif
};

static IIO_SLOT_CONFIG_DATA_ENTRY   IioSlotTable[] = {
  // Port        |  Slot      | Inter  | Power Limit | Power Limit | Hot      | Vpp           | Vpp         | PcieSSD | PcieSSD     | PcieSSD       | Hidden
  // Index       |            | lock   | Scale       |  Value      | Plug     | Port          | Addr        | Cap     | VppPort     | VppAddr       |
  /// Socket 0: Iuo2 Connected to SLOT 9 or SSDs: read values of QAT and riser.
  ///           Iuo0 Uplink
  ///           Iuo3 Connected to SLOT 1 or 2, and might be HP. Read values of QAT and riser.
  { PORT_1A_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_0   ,  0x4C        , HIDE    }, //Oculink
  { PORT_1B_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_1   ,  0x4C        , HIDE    }, //Oculink
  { PORT_1C_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_0   ,  0x4E        , HIDE    }, //Oculink
  { PORT_1D_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_1   ,  0x4E        , HIDE    }, //Oculink
  { PORT_2A_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE }, // Uplink
  { PORT_3A_INDEX, 1          , ENABLE  , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE  , VPP_PORT_0   ,  0x40        , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  /// Socket 1: DMI no connected on all boards.
  ///           Iuo2 Slot 7 or 5 based on riser information. Might be HP.
  ///           Iuo0 Slot 5 or 7 based on riser information. Might be HP.
  ///           Iuo1, Not connected for RP.
  { SOCKET_1_INDEX +
    PORT_0_INDEX, NO_SLT_IMP  , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , HIDE    },
  { SOCKET_1_INDEX +
    PORT_1A_INDEX, 5          , ENABLE  , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  { SOCKET_1_INDEX +
    PORT_2A_INDEX, 7          , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  { SOCKET_1_INDEX +
    PORT_3A_INDEX, 10         , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , HIDE    },
  /// Socket 2: DMI Port, not connected on RP
  ///           Iuo2 Slot 6 or 8 based on riser information. Might be HP.
  ///           Iuo1 Slot 6 or 8 based on riser information. Might be HP.
  ///           Iuo0, Not connected for RP.
  { SOCKET_2_INDEX +
    PORT_0_INDEX,  NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , HIDE    },
  { SOCKET_2_INDEX +
    PORT_1A_INDEX, 6          , ENABLE  , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  { SOCKET_2_INDEX +
    PORT_3A_INDEX, 8          , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  { SOCKET_2_INDEX +
    PORT_2A_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , HIDE    },
  /// Socket 3: DMI not connected.
  ///           Iuo2 Not Connected for RP.
  ///           Iuo0 Slot 4 or 3 based on riser information. Might be HP.
  ///           Iuo1 Slot 4 or 3 based on riser information. Might be HP.
  { SOCKET_3_INDEX +
    PORT_0_INDEX , NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , HIDE    },
  { SOCKET_3_INDEX +
    PORT_2A_INDEX, 3          , ENABLE  , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  { SOCKET_3_INDEX +
    PORT_3A_INDEX, 4          , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  { SOCKET_3_INDEX +
    PORT_1A_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , HIDE    },
};

#endif   //_IIOCFG_UPDATE_DXE_H_
