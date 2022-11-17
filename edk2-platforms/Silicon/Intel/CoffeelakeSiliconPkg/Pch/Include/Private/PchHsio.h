/** @file
  Header file with all common HSIO information

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_HSIO_H_
#define _PCH_HSIO_H_

#define PCH_LANE_OWN_COMMON                      0x10
#define PCH_LANE_BDCAST                          0x11
#define PCH_HSIO_LANE_GROUP_NO                   0x09
#define PCH_HSIO_LANE_GROUP_COMMON_LANE          0x00
#define PCH_HSIO_LANE_GROUP_PCIE                 0x01
#define PCH_HSIO_LANE_GROUP_DMI                  0x02
#define PCH_HSIO_LANE_GROUP_GBE                  0x03
#define PCH_HSIO_LANE_GROUP_USB3                 0x04
#define PCH_HSIO_LANE_GROUP_SATA                 0x05
#define PCH_HSIO_LANE_GROUP_SSIC                 0x06


/**
  PCH HSIO ChipsetInit Version Information
**/
typedef struct {
  UINT16 BaseCrc;
  UINT16 SusCrc;
  UINT16 OemCrc;
  UINT8  Version;
  UINT8  Product;
  UINT8  MetalLayer : 4;
  UINT8  BaseLayer : 4;
  UINT8  OemVersion;
  UINT16 DebugMode : 1;
  UINT16 OemCrcValid : 1;
  UINT16 SusCrcValid : 1;
  UINT16 BaseCrcValid : 1;
  UINT16 Reserved : 12;
} PCH_HSIO_VER_INFO;

#define PMC_DATA_CMD_SIZE   ((12/sizeof(UINT16))-1)
#define PMC_DATA_DELAY_CMD_SIZE ((4/sizeof(UINT16))-1)

#define RECORD_OFFSET(X, Y)  ((X << 4) | Y)
/**
  PCH HSIO ChipsetInit Command Field
**/
typedef struct {
  UINT8 Command : 3;
  UINT8 Size : 5;
  UINT8 Pid;
  UINT8 OpCode; //PrivateControlWrite
  UINT8 Bar; //0
  UINT8 Fbe; //First Byte Enable  : 0x0F
  UINT8 Fid; //0
  UINT16 Offset;
  UINT32 Value;
} PCH_HSIO_CMD_FIELD;

/**
PCH HSIO Delay XRAM Data
**/
typedef struct {
  UINT8 Command : 3;
  UINT8 Size : 5;
  UINT8 DelayPeriod; //(00h = 1us, 01h = 10us, 02h = 100us, ..., 07h = 10s; others reserved)
  UINT8 DelayCount; //(0 - 255); total delay = Delay period * Delay count
  UINT8 Padding;
} PCH_HSIO_DELAY_CMD_FIELD;

typedef enum {
  Delay1us = 0x0,
  Delay10us,
  Delay100us,
  Delay1ms,
  Delay10ms,
  Delay100ms,
  Delay1s,
  Delay10s
} PCH_HSIO_DELAY;

/**
PCH PCIE PLL SSC Data
**/
#define MAX_PCIE_PLL_SSC_PERCENT  20

#include <Private/CnlPchLpHsioDx.h>

#endif //_PCH_HSIO_H_

