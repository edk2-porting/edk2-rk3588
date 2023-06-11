/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __ROCKCHIP_I2CDEMO_H__
#define __ROCKCHIP_I2CDEMO_H__

#define ROCKCHIP_I2CDEMO_PROTOCOL_GUID { 0x71954bda, 0x60d3, 0x4ef8, { 0x8e, 0x3c, 0x0e, 0x33, 0x9f, 0x3b, 0xc2, 0x2b }}

typedef struct _ROCKCHIP_I2CDEMO_PROTOCOL ROCKCHIP_I2CDEMO_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_I2CDEMO_TRANSFER) (
  IN CONST ROCKCHIP_I2CDEMO_PROTOCOL *This,
  IN UINT8 *RegAddress,
  IN UINT16 RegAddressLength,
  IN UINT8 *Buffer,
  IN UINT16 Length
);

struct _ROCKCHIP_I2CDEMO_PROTOCOL {
  EFI_I2CDEMO_TRANSFER Read;
  EFI_I2CDEMO_TRANSFER Write;
  UINT32 Identifier;
};

extern EFI_GUID gRockchipI2cDemoProtocolGuid;
#endif
