/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_SMI_HANDLER_H_
#define _TBT_SMI_HANDLER_H_

#include <Library/TbtCommonLib.h>
#include <Library/IoLib.h>
#include <IndustryStandard/Pci.h>

#ifdef PROGRESS_CODE
#undef PROGRESS_CODE
#endif

#define MAX_TBT_DEPTH         6

#define P2P_BRIDGE            (((PCI_CLASS_BRIDGE) << 8) | (PCI_CLASS_BRIDGE_P2P))

#define BAR_ALIGN(v, a)       ((((v) - 1) | (a)) + 1)

#define CMD_BUS_MASTER        BIT2
#define CMD_BM_IO             (CMD_BUS_MASTER | BIT0)
#define CMD_BM_MEM            (CMD_BUS_MASTER | BIT1)
#define CMD_BM_MEM_IO         (CMD_BUS_MASTER | BIT1 | BIT0)

#define DEF_CACHE_LINE_SIZE   0x20
#define DEF_RES_IO_PER_DEV    4
#define DEF_RES_MEM_PER_DEV   32
#define DEF_RES_PMEM_PER_DEV  32

#define DOCK_BUSSES           8

#define DISBL_IO_REG1C        0x01F1
#define DISBL_MEM32_REG20     0x0000FFF0
#define DISBL_PMEM_REG24      0x0001FFF1

#define count(x)              (sizeof (x) / sizeof ((x)[0]))

#define PCIE_CAP_ID_SSID_SSVID 0x0D
#define INVALID_PCI_DEVICE    0xFFFFFFFF
#define PCI_TBT_VESC_REG2     0x510

typedef struct _PortInfo {
  UINT8   IoBase;
  UINT8   IoLimit;
  UINT16  MemBase;
  UINT16  MemLimit;
  UINT64  PMemBase64;
  UINT64  PMemLimit64;
  UINT8   BusNumLimit;
  UINT8   ConfedEP;
} PORT_INFO;

typedef struct _MEM_REGS {
  UINT32  Base;
  UINT32  Limit;
} MEM_REGS;

typedef struct _PMEM_REGS {
  UINT64  Base64;
  UINT64  Limit64;
} PMEM_REGS;

typedef struct _IO_REGS {
  UINT16  Base;
  UINT16  Limit;
} IO_REGS;

typedef struct _BRDG_RES_CONFIG {
  UINT8   Cmd;
  UINT8   Cls;
  UINT8   IoBase;
  UINT8   IoLimit;
  UINT16  MemBase;
  UINT16  MemLimit;
  UINT64  PMemBase64;
  UINT64  PMemLimit64;
} BRDG_RES_CONFIG;

typedef struct _BRDG_CONFIG {
  DEV_ID          DevId;
  UINT8           PBus;
  UINT8           SBus;
  UINT8           SubBus;
  BOOLEAN         IsDSBridge;
  BRDG_RES_CONFIG Res;
} BRDG_CONFIG;

enum {
  HR_US_PORT,
  HR_DS_PORT0,
  HR_DS_PORT3,
  HR_DS_PORT4,
  HR_DS_PORT5,
  HR_DS_PORT6,
  MAX_CFG_PORTS
};

enum {
  HR_DS_PORT1   = HR_DS_PORT3
};

//
// Alpine Ridge
//
enum {
  AR_DS_PORT1 = HR_DS_PORT3,
  AR_DS_PORT2,
  AR_DS_PORT3,
  AR_DS_PORT4
};

typedef struct _HR_CONFIG {
  UINT16  DeviceId;
  UINT8   HRBus;
  UINT8   MinDSNumber;
  UINT8   MaxDSNumber;
  UINT8   BridgeLoops;
} HR_CONFIG;

STATIC const BRDG_RES_CONFIG  NOT_IN_USE_BRIDGE = {
  CMD_BUS_MASTER,
  0,
  DISBL_IO_REG1C & 0xFF,
  DISBL_IO_REG1C >> 8,
  DISBL_MEM32_REG20 & 0xFFFF,
  DISBL_MEM32_REG20 >> 16,
  DISBL_PMEM_REG24 & 0xFFFF,
  DISBL_PMEM_REG24 >> 16
};

typedef union _BRDG_CIO_MAP_REG {
  UINT32  AB_REG;
  struct {
    UINT32  NumOfDSPorts : 5;
    UINT32  CioPortMap : 27;
  } Bits;
} BRDG_CIO_MAP_REG;

//
// Functions
//
VOID
ThunderboltCallback (
  IN UINT8 Type
  );

VOID
TbtDisablePCIDevicesAndBridges (
  IN UINT8 Type
  );

VOID
EndOfThunderboltCallback(
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
);

VOID
ConfigureTbtAspm(
  IN UINT8       Type,
  IN UINT16      Aspm
);

UINT8
PcieFindCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  );

#endif

