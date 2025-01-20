/** @file
*
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*  Copyright (c) 2023, Shimrra Shai <shimmyshai00@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __PCA9555_DXE_H__
#define __PCA9555_DXE_H__

#include <Uefi.h>

#define PCA95XX_SIGNATURE  SIGNATURE_32 ('P', '9', '5', 'X')

#define I2C_GUID \
  { \
  0xadc1901b, 0xb83c, 0x4831, { 0x8f, 0x59, 0x70, 0x89, 0x8f, 0x26, 0x57, 0x1e } \
  }

/* NB: Only PCA9555 is supported now. This driver can/should be generalized to
 *     whole PCA95XX family.
 */
#define PCA9555_INPUT_REG_BASE      0x00
#define PCA9555_OUTPUT_REG_BASE     0x02
#define PCA9555_INVERSION_REG_BASE  0x04
#define PCA9555_CONTROL_REG_BASE    0x06

#define PCA9555_NUM_GPIO   16
#define PCA9555_NUM_BANKS  2

#define PCA95XX_BANK_SIZE  8         /* bits */
#define PCA95XX_MAX_BANK   5

typedef struct {
  UINT32    Signature;

  UINT8     I2cAddress;
  UINT8     I2cBus;
} PCA9555_CONTEXT;

/* --- Method Prototypes --- */
EFI_STATUS
EFIAPI
Pca9555Supported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
Pca9555Start (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
Pca9555Stop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  );

EFI_STATUS
EFIAPI
Pca9555DxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif // __PCA9555_DXE_H__
