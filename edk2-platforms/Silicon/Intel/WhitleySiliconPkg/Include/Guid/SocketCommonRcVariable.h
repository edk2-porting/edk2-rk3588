/** @file
  Data format for Universal Data Structure

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef   __SOCKET_COMMONRC_CONFIG_DATA_H__
#define   __SOCKET_COMMONRC_CONFIG_DATA_H__


#include <UncoreCommonIncludes.h>

extern EFI_GUID gEfiSocketCommonRcVariableGuid;
#define SOCKET_COMMONRC_CONFIGURATION_NAME L"SocketCommonRcConfig"

#define UMABASEDCLUSTERING_DISABLED   0
#define UMABASEDCLUSTERING_HEMISPHERE 2
#define UMABASEDCLUSTERING_QUADRANT   4

#pragma pack(1)
typedef struct {
    //
    //  Common Section of RC
    //
    UINT32   MmiohBase;
    UINT16   MmiohSize;
    UINT8    MmcfgBase;
    UINT8    MmcfgSize;
    UINT8    IsocEn;
    UINT8    NumaEn;
    UINT8    UmaBasedClustering;
    UINT8    MirrorMode;
    UINT8    CpuType;
    UINT8    CpuChop;
    UINT8    X2ApicForceEn;
    UINT8    SystemRasType;
    UINT8    NumCpus;
    UINT8    ReservedS6;
    UINT8    ReservedS7[MAX_B2P_MAILBOX_GROUPS];
    UINT8    OcCap;
    UINT8    IssMaxLevel;
    UINT8    DcpmmEnable;
    UINT8    ReservedS8;
    UINT8    HbmSku;
    UINT8    PbfCapableSystem;
    UINT8    DcuRtlWaEn;
    UINT8    SstCpCapableSystem;
    UINT8    VirtualNumaEnable;
} SOCKET_COMMONRC_CONFIGURATION;
#pragma pack()

#endif


