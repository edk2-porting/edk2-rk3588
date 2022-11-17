/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

// Definition Flag:
//                  1. KTI_HW_PLATFORM   -> run with real hardware or SoftIVT
//                  2. KTI_SW_SIMULATION -> run with KTIRC Simulation
//                  3. IA32              -> run with IA32 mode


#ifndef _KTI_HOST_H_
#define _KTI_HOST_H_
#ifdef _MSC_VER
#pragma warning (disable: 4127 4214 4100)     // disable C4127: constant conditional expression
#endif
#include "DataTypes.h"
#include "PlatformHost.h"
#include "KtiSi.h"
#include "KtiDisc.h"

#pragma pack(1)

typedef INT32  KTI_STATUS;
#ifndef NULL
#define NULL  0
#endif
#define CONST       const
#define STATIC      static
#define VOID        void
#define VOLATILE    volatile
#define KTI_SUCCESS 0
#define KTI_REBOOT  1
#define KTI_SNC_CHANGED 2
#define KTI_IGNORE  3
#define KTI_FAILURE -1

//
// Warning log
//
#define MAX_WARNING_LOGS      16

typedef enum {
  NORMAL_OPERATION = 0,
  RECOVERY_OPERATION
} SNC_COLDRESET_REGISTER_OPERATION_TYPE;

typedef enum {
  KTI_GROUP = 0,
} GROUP_TYPE;

/*********************************************************
                KTIRC Host Structure Related
*********************************************************/

typedef enum {
  KTI_LINK0 =  0x0,
  KTI_LINK1,
  KTI_LINK2
} KTI_LOGIC_LINK;

typedef enum {
  FULL_SPEED = 0,
  HALF_SPEED
} KTI_LINK_SPEED_TYPE;


//
// Definitions to be used in Eparam tables:
//
typedef enum {
  PER_LANES_TXEQ_ENABLED   = 0,         // each lane use different TXEQ value
  ALL_LANES_TXEQ_ENABLED                // all lanes use same TXEQ value
} LANE_TXEQ_TYPE;

//
// Number of Clusters.
//
typedef enum {
  CLUSTER_MODE_1,
  CLUSTER_MODE_2,
} CLUSTER_MODE;

typedef enum {
  LCC = 0, // 10c
  MCC,     // 14c
  HCC,     // 22c
  XCC,     // 28c
  MAX_CHOP_TYPES
} PHYSICAL_CHOP;


//
// PHY settings that are system dependent.   Need 1 of these for each socket/link/freq.
//

typedef struct {
  UINT8  SocketID;
  UINT8  AllLanesUseSameTxeq;
  UINT8  Freq;
  UINT32 Link;
  UINT32 TXEQL[20];
  UINT32 CTLEPEAK[5];
} PER_LANE_EPARAM_LINK_INFO;

//
// This is for full speed mode, all lanes have the same TXEQ setting
//
typedef struct {
  UINT8  SocketID;
  UINT8  Freq;
  UINT32 Link;
  UINT32 AllLanesTXEQ;
  UINT8  CTLEPEAK;
} ALL_LANES_EPARAM_LINK_INFO;

#define ADAPTIVE_CTLE 0x3f
#define PER_LANE_ADAPTIVE_CTLE 0X3f3f3f3f

typedef enum {
  TYPE_UBOX = 0,
  TYPE_UBOX_IIO,
  TYPE_MCP,
  TYPE_FPGA,
  TYPE_DISABLED,              // This item must be prior to stack specific disable types
  TYPE_UBOX_IIO_DIS,
  TYPE_MCP_DIS,
  TYPE_FPGA_DIS,
  TYPE_NONE
} STACK_TYPE;

#pragma pack()

#endif // _KTI_HOST_H_
