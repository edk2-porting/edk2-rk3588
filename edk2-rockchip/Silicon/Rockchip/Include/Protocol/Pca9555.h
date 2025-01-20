/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2023, Shimrra Shai <shimmyshai00@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __PCA9555_H__
#define __PCA9555_H__

#include <Uefi/UefiBaseType.h>

#include <Protocol/EmbeddedGpio.h>

#define PCA95XX_PROTOCOL_GUID \
    { 0x7e91391b, 0xa23c, 0x4a51, { 0x9d, 0xf7, 0xf6, 0x74, 0xef, 0x1d, 0x51, 0x1b } }

typedef struct _PCA95XX_PROTOCOL PCA95XX_PROTOCOL;

struct _PCA95XX_PROTOCOL {
  EMBEDDED_GPIO    GpioProtocol;
};

extern EFI_GUID  gPca95xxProtocolGuid;

#endif // __PCA9555_H__
