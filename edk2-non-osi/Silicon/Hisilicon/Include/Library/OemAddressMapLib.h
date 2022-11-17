/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef _OEM_ADDRESS_MAP_LIB_H_
#define _OEM_ADDRESS_MAP_LIB_H_

#include <PlatformArch.h>

typedef struct _DDRC_BASE_ID{
    UINTN  Base;
    UINTN  Id;
}DDRC_BASE_ID;

// Invalid address, will cause exception when accessed by bug code
#define ADDRESS_MAP_INVALID ((UINTN)(-1))

UINTN OemGetPoeSubBase (UINT32 NodeId);
UINTN OemGetPeriSubBase (UINT32 NodeId);
UINTN OemGetAlgSubBase (UINT32 NodeId);
UINTN OemGetCfgbusBase (UINT32 NodeId);
UINTN OemGetGicSubBase (UINT32 NodeId);
UINTN OemGetHACSubBase (UINT32 NodeId);
UINTN OemGetIOMGMTSubBase (UINT32 NodeId);
UINTN OemGetNetworkSubBase (UINT32 NodeId);
UINTN OemGetM3SubBase (UINT32 NodeId);
UINTN OemGetPCIeSubBase (UINT32 NodeId);

VOID OemAddressMapInit(VOID);

extern DDRC_BASE_ID DdrcBaseId[MAX_SOCKET][MAX_CHANNEL];

#endif

