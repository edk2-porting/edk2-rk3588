/** @file
  Header file for Platform Boards Configurations.


  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_BOARD_CONFIG_H
#define _PLATFORM_BOARD_CONFIG_H

#include <ConfigBlock.h>
#include <PchPolicyCommon.h>


#pragma pack(1)

typedef struct {
  UINT8 ClkReqNumber : 4;
  UINT8 ClkReqSupported : 1;
  UINT8 DeviceResetPadActiveHigh : 1;
  UINT32 DeviceResetPad;
} ROOT_PORT_CLK_INFO;

typedef struct {
  UINT8 Section;
  UINT8 Pin;
} EXPANDER_GPIO_CONFIG;

typedef struct {
  UINT8 Type;
  UINT8 Reserved[3];  // alignment for COMMON_GPIO_CONFIG
  union {
    UINT32 Pin;
    EXPANDER_GPIO_CONFIG Expander;
  } u;
} BOARD_GPIO_CONFIG;

// Do not change the encoding. It must correspond with PCH_PCIE_CLOCK_USAGE from PCH RC.
#define NOT_USED     0xFF
#define FREE_RUNNING 0x80
#define LAN_CLOCK    0x70
#define PCIE_PEG     0x40
#define PCIE_PCH     0x00

typedef struct {
  UINT32 ClockUsage;
  UINT32 ClkReqSupported;
} PCIE_CLOCK_CONFIG;

typedef union {
  UINT64 Blob;
  BOARD_GPIO_CONFIG  BoardGpioConfig;
  ROOT_PORT_CLK_INFO Info;
  PCIE_CLOCK_CONFIG  PcieClock;
} PCD64_BLOB;

#pragma pack()

#endif // _PLATFORM_BOARD_CONFIG_H

