/********************************************************************************
Copyright (c) 2021, Rockchip Limited. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __I2CDEMO_H__
#define __I2CDEMO_H__

#include <Uefi.h>

#define I2CDEMO_SIGNATURE          SIGNATURE_32 ('I', '2', 'C', 'D')

#define MAX_BUFFER_LENGTH 64

#define I2C_GUID \
  { \
  0xadc1901b, 0xb83c, 0x4831, { 0x8f, 0x59, 0x70, 0x89, 0x8f, 0x26, 0x57, 0x1e } \
  }

typedef struct {
  UINT32  Signature;
  EFI_HANDLE ControllerHandle;
  EFI_I2C_IO_PROTOCOL *I2cIo;
  ROCKCHIP_I2CDEMO_PROTOCOL I2cDemoProtocol;
} I2CDEMO_CONTEXT;

#define I2CDEMO_SC_FROM_IO(a) CR (a, I2CDEMO_CONTEXT, I2cIo, I2CDEMO_SIGNATURE)
#define I2CDEMO_SC_FROM_I2CDEMO(a) CR (a, I2CDEMO_CONTEXT, I2cDemoProtocol, I2CDEMO_SIGNATURE)

EFI_STATUS
EFIAPI
I2cDemoSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
I2cDemoStart (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
I2cDemoStop (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN  EFI_HANDLE ControllerHandle,
  IN  UINTN NumberOfChildren,
  IN  EFI_HANDLE *ChildHandleBuffer OPTIONAL
  );

EFI_STATUS
EFIAPI
I2cDemoRead (
  IN CONST ROCKCHIP_I2CDEMO_PROTOCOL *This,
  IN UINT8 *RegAddress,
  IN UINT16 RegAddressLength,
  IN UINT8 *Buffer,
  IN UINT16 Length
  );

EFI_STATUS
EFIAPI
I2cDemoWrite (
  IN CONST ROCKCHIP_I2CDEMO_PROTOCOL *This,
  IN UINT8 *RegAddress,
  IN UINT16 RegAddressLength,
  IN UINT8 *Buffer,
  IN UINT16 Length
  );
#endif // __I2CDEMO_H__
