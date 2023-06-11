/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _PLATFORM_SAS_PROTOCOL_H_
#define _PLATFORM_SAS_PROTOCOL_H_

#define PLATFORM_SAS_PROTOCOL_GUID \
    { \
        0x40e9829f, 0x3a2c, 0x479a, 0x9a, 0x93, 0x45, 0x7d, 0x13, 0x50, 0x96, 0x5d \
    }

typedef struct _PLATFORM_SAS_PROTOCOL PLATFORM_SAS_PROTOCOL;

typedef
VOID
(EFIAPI * SAS_INIT) (
    IN PLATFORM_SAS_PROTOCOL   *This
);

struct _PLATFORM_SAS_PROTOCOL {
    IN UINT64        BaseAddr;
    SAS_INIT         Init;
};

#endif
