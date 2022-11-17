/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef   __SOCKET_COMMONRC_CONFIG_DATA_H__
#define   __SOCKET_COMMONRC_CONFIG_DATA_H__


#include <UncoreCommonIncludes.h>
#include "SocketConfiguration.h"

extern EFI_GUID gEfiSocketCommonRcVariableGuid;
#define SOCKET_COMMONRC_CONFIGURATION_NAME L"SocketCommonRcConfig"

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
    UINT8    MirrorMode;
    UINT8    LockStep;
    UINT8    CpuStepping;
    UINT8    SystemRasType;
    UINT32   FpgaPresentBitMap;
    UINT8    IssCapable;
    UINT8    PbfCapable;
} SOCKET_COMMONRC_CONFIGURATION;
#pragma pack()

#endif


