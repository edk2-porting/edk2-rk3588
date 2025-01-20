/** @file

  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _ROCKCHIP_I2C_MASTER_PROTOCOL_H_
#define _ROCKCHIP_I2C_MASTER_PROTOCOL_H_

#include <Uefi/UefiBaseType.h>

#define ROCKCHIP_I2C_MASTER_PROTOCOL_GUID \
  { 0x7b4af789, 0x43d2, 0x44a5, { 0xa0, 0xb3, 0x3f, 0xa5, 0xde, 0xd7, 0xa1, 0x27 } }

typedef struct {
  UINT8    Bus;
} ROCKCHIP_I2C_MASTER_PROTOCOL;

extern EFI_GUID  gRockchipI2cMasterProtocolGuid;

#endif // _ROCKCHIP_I2C_MASTER_PROTOCOL_H_
