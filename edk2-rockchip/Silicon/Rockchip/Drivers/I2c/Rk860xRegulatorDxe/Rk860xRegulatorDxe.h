/** @file
*
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __RK860X_REGULATOR_DXE_H__
#define __RK860X_REGULATOR_DXE_H__

#include <Uefi.h>

#define RK860X_REGULATOR_SIGNATURE  SIGNATURE_32 ('R', 'K', '8', '6')

#define I2C_GUID \
  { \
  0xadc1901b, 0xb83c, 0x4831, { 0x8f, 0x59, 0x70, 0x89, 0x8f, 0x26, 0x57, 0x1e } \
  }

typedef struct {
  UINT8     VolReg;
  UINT8     SleepReg;
  UINT8     EnReg;
  UINT8     SleepEnReg;
  UINT8     VolMask;
  UINT16    NumVoltages;
  UINT32    VselMin;
  UINT32    VselStep;
} RK860X_REGULATOR_CONFIG;

typedef struct {
  UINT32                       Signature;
  EFI_HANDLE                   ControllerHandle;
  EFI_I2C_IO_PROTOCOL          *I2cIo;
  RK860X_REGULATOR_PROTOCOL    Rk860xRegulatorProtocol;
  RK860X_REGULATOR_CONFIG      Config;
} RK860X_REGULATOR_CONTEXT;

#define RK860X_REGULATOR_SC_FROM_IO(a)        CR (a, RK860X_REGULATOR_CONTEXT, I2cIo, RK860X_REGULATOR_SIGNATURE)
#define RK860X_REGULATOR_SC_FROM_PROTOCOL(a)  CR (a, RK860X_REGULATOR_CONTEXT, Rk860xRegulatorProtocol, RK860X_REGULATOR_SIGNATURE)

/* Registers & fields */
#define RK860X_VSEL0_A  0x00
#define RK860X_VSEL1_A  0x01
#define RK860X_VSEL0_B  0x06
#define RK860X_VSEL1_B  0x07

#define  RK860X_VSEL_BUCK_EN      BIT7
#define  RK860X_VSEL_A_NSEL_MASK  0x3F
#define  RK860X_VSEL_B_NSEL_MASK  0xff

#define RK860X_ID1             0x03
#define  RK860X_DIE_ID_MASK    0x0F
#define  RK860X_CHIP_ID_00_01  8
#define  RK860X_CHIP_ID_02_03  10

#define RK860X_NVOLTAGES_64   64
#define RK860X_NVOLTAGES_160  160

#define RK860X_MAX_VOLTAGE  1500000

EFI_STATUS
EFIAPI
Rk860xRegulatorSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
Rk860xRegulatorStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

EFI_STATUS
EFIAPI
Rk860xRegulatorStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                  ControllerHandle,
  IN  UINTN                       NumberOfChildren,
  IN  EFI_HANDLE                  *ChildHandleBuffer OPTIONAL
  );

EFI_STATUS
EFIAPI
Rk860xRegulatorGetVoltage (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  OUT UINT32                          *Voltage,
  IN BOOLEAN                          Suspend
  );

EFI_STATUS
EFIAPI
Rk860xRegulatorSetVoltage (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  IN UINT32                           Voltage,
  IN BOOLEAN                          Suspend
  );

EFI_STATUS
EFIAPI
Rk860xRegulatorGetEnable (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  OUT BOOLEAN                         *Enable,
  IN BOOLEAN                          Suspend
  );

EFI_STATUS
EFIAPI
Rk860xRegulatorSetEnable (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  IN BOOLEAN                          Enable,
  IN BOOLEAN                          Suspend
  );

#endif // __RK860X_REGULATOR_DXE_H__
