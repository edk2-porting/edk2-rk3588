/** @file
  Header file for Platform Boards Configurations.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_BOARD_CONFIG_H
#define _PLATFORM_BOARD_CONFIG_H

#include <ConfigBlock.h>
#include <PchPolicyCommon.h>
#include <ConfigBlock/MemoryConfig.h>
#include <GpioConfig.h>
#include <TbtBoardInfo.h>

#define IS_ALIGNED(addr, size) (((addr) & (size - 1)) ? 0 : 1)
#define ALIGN16(size)          (IS_ALIGNED(size, 16) ? size : ((size + 16) & 0xFFF0))

#define BOARD_CONFIG_BLOCK_PEI_PREMEM_VERSION  0x00000001
#define BOARD_CONFIG_BLOCK_PEI_POSTMEM_VERSION 0x00000001
#define BOARD_CONFIG_BLOCK_DXE_VERSION 0x00000001
#define BOARD_NO_BATTERY_SUPPORT 0
#define BOARD_REAL_BATTERY_SUPPORTED BIT0
#define BOARD_VIRTUAL_BATTERY_SUPPORTED BIT1

#pragma pack(1)

typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
} BOARD_CONFIG_BLOCK;

typedef struct {
  UINT8 GpioSupport;
  UINT32 WakeGpioNo;
  UINT8 HoldRstExpanderNo;
  UINT32 HoldRstGpioNo;
  BOOLEAN HoldRstActive;
  UINT8 PwrEnableExpanderNo;
  UINT32 PwrEnableGpioNo;
  BOOLEAN PwrEnableActive;
} SWITCH_GRAPHIC_GPIO;

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

typedef enum {
  BoardGpioTypePch,
  BoardGpioTypeExpander,
  BoardGpioTypeNotSupported = 0xFF
} BOARD_GPIO_TYPE;

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

typedef union {
  UINT32        Blob;
  USB20_AFE     Info;
} PCD32_BLOB;

#ifndef IO_EXPANDER_DISABLED
#define IO_EXPANDER_DISABLED      0xFF
#endif

#define SPD_DATA_SIZE 512

#pragma pack()

#endif // _PLATFORM_BOARD_CONFIG_H

