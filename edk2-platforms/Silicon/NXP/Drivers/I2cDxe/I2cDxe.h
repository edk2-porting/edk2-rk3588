/** I2cDxe.h
  Header defining the constant, base address amd function for I2C controller

  Copyright 2017-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef I2C_DXE_H_
#define I2C_DXE_H_

#include <Library/UefiLib.h>
#include <Uefi.h>

#include <Protocol/I2cMaster.h>
#include <Protocol/NonDiscoverableDevice.h>

#define NXP_I2C_SIGNATURE         SIGNATURE_32 ('N', 'I', '2', 'C')
#define NXP_I2C_FROM_THIS(a)      CR ((a), NXP_I2C_MASTER, \
                                    I2cMaster, NXP_I2C_SIGNATURE)

extern EFI_COMPONENT_NAME2_PROTOCOL gNxpI2cDriverComponentName2;

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH              Vendor;
  UINT64                          MmioBase;
  EFI_DEVICE_PATH_PROTOCOL        End;
} NXP_I2C_DEVICE_PATH;
#pragma pack()

typedef struct {
  UINT32                          Signature;
  EFI_I2C_MASTER_PROTOCOL         I2cMaster;
  NXP_I2C_DEVICE_PATH             DevicePath;
  NON_DISCOVERABLE_DEVICE         *Dev;
} NXP_I2C_MASTER;

EFI_STATUS
NxpI2cInit (
  IN EFI_HANDLE  DriverBindingHandle,
  IN EFI_HANDLE  ControllerHandle
  );

EFI_STATUS
NxpI2cRelease (
  IN EFI_HANDLE  DriverBindingHandle,
  IN EFI_HANDLE  ControllerHandle
  );

#endif //I2C_DXE_H_
