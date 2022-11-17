/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef IIOBIFURCATIONSLOTTABLE_H
#define IIOBIFURCATIONSLOTTABLE_H

#include "IioPlatformData.h"

#define QAT_ENABLED                0  // QAT is active-low
#define RISER_PRESENT              0
#define RISER_NOT_PRESENT          1
#define RISER_HP_EN                1
#define RISER_WINGED_IN            0
#define RISER_SLOT9_DISABLE        1

typedef struct {
  UINT8 Socket;
  UINT8 IouNumber;
  UINT8 Bifurcation;
} IIO_BIFURCATION_ENTRY;

typedef union {
  struct {
    UINT8 PresentSignal:1;
    UINT8 HPConf:1;
    UINT8 WingConf:1;
    UINT8 Slot9En:1;
  } Bits;
  UINT8 Data;
} PCIE_RISER_ID;

enum {
  Iio_PortA = 0,
  Iio_PortB = 1,
  Iio_PortC = 2,
  Iio_PortD = 3
};
typedef enum {
 Iio_Iou0 =0,
 Iio_Iou1,
 Iio_Iou2,
 Iio_Mcp0,
 Iio_Mcp1,
 Iio_IouMax
} IIO_IOUS;

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
  VPP_PORT_0 = 0,
  VPP_PORT_1,
  VPP_PORT_2,
  VPP_PORT_3
} VPP_PORT;
///
/// Platform Port/Socket assignments.
///

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

typedef struct {
  UINT8   PortIndex;
  UINT8   SlotNumber;     // 0xff if slot not implemented , Slot number if slot implemented
  BOOLEAN InterLockPresent;
  UINT8   SlotPowerLimitScale;
  UINT8   SlotPowerLimitValue;
  BOOLEAN HotPlugCapable;
  UINT8   VppPort; // 0xff if Vpp not enabled
  UINT8   VppAddress;
  BOOLEAN PcieSSDCapable;
  UINT8   PcieSSDVppPort; // 0xff if Vpp not enabled
  UINT8   PcieSSDVppAddress;
  BOOLEAN Hidden;
} IIO_SLOT_CONFIG_ENTRY;

#endif
