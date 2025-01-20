/** @file
 *
 *  Khadas MCU platform driver
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __KHADAS_MCU_DXE_H__
#define __KHADAS_MCU_DXE_H__

#include <Uefi.h>

#define KHADAS_MCU_SIGNATURE  SIGNATURE_32 ('K', 'M', 'C', 'U')

#define I2C_GUID \
  { \
  0xadc1901b, 0xb83c, 0x4831, { 0x8f, 0x59, 0x70, 0x89, 0x8f, 0x26, 0x57, 0x1e } \
  }

typedef struct {
  UINT32                 Signature;
  EFI_HANDLE             ControllerHandle;
  EFI_I2C_IO_PROTOCOL    *I2cIo;
  KHADAS_MCU_PROTOCOL    KhadasMcuProtocol;
} KHADAS_MCU_CONTEXT;

#define KHADAS_MCU_FROM_IO(a)        CR (a, KHADAS_MCU_CONTEXT, I2cIo, KHADAS_MCU_SIGNATURE)
#define KHADAS_MCU_FROM_PROTOCOL(a)  CR (a, KHADAS_MCU_CONTEXT, KhadasMcuProtocol, KHADAS_MCU_SIGNATURE)

//
// Registers & fields
// https://docs.khadas.com/products/sbc/edge2/hardware/edge2-boot-flow
//
#define MCU_CMD_FAN_STATUS_CTRL_REGv2  0x8A

EFI_STATUS
EFIAPI
KhadasMcuSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
KhadasMcuStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
KhadasMcuStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  );

#endif // __KHADAS_MCU_DXE_H__
