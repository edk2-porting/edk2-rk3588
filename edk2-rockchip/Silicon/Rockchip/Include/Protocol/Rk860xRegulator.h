/** @file
*
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __RK860X_REGULATOR_H__
#define __RK860X_REGULATOR_H__

#define RK860X_REGULATOR_PROTOCOL_GUID  { 0xb9ef9018, 0x2096, 0x4bf5, { 0x94, 0xb2, 0x70, 0x32, 0xb9, 0xa9, 0xa4, 0x29 }}

typedef struct _RK860X_REGULATOR_PROTOCOL RK860X_REGULATOR_PROTOCOL;

typedef struct {
  UINT32    Min;
  UINT32    Max;
} VOLTAGE_RANGE;

typedef
EFI_STATUS
(EFIAPI *EFI_RK860X_REGULATOR_GET_VOLTAGE)(
  IN CONST RK860X_REGULATOR_PROTOCOL *This,
  OUT UINT32 *Voltage,
  IN BOOLEAN Suspend
  );

typedef
EFI_STATUS
(EFIAPI *EFI_RK860X_REGULATOR_SET_VOLTAGE)(
  IN CONST RK860X_REGULATOR_PROTOCOL *This,
  IN UINT32 Voltage,
  IN BOOLEAN Suspend
  );

typedef
EFI_STATUS
(EFIAPI *EFI_RK860X_REGULATOR_GET_ENABLE)(
  IN CONST RK860X_REGULATOR_PROTOCOL *This,
  OUT BOOLEAN *Enable,
  IN BOOLEAN Suspend
  );

typedef
EFI_STATUS
(EFIAPI *EFI_RK860X_REGULATOR_SET_ENABLE)(
  IN CONST RK860X_REGULATOR_PROTOCOL *This,
  IN BOOLEAN Enable,
  IN BOOLEAN Suspend
  );

typedef
EFI_STATUS
(EFIAPI *EFI_RK860X_REGULATOR_GET_VOLTAGE_LIMIT)(
  IN CONST RK860X_REGULATOR_PROTOCOL *This,
  OUT UINT32 *Voltage,
  IN BOOLEAN PlatformPreferredLimit
  );

struct _RK860X_REGULATOR_PROTOCOL {
  EFI_RK860X_REGULATOR_GET_VOLTAGE    GetVoltage;
  EFI_RK860X_REGULATOR_SET_VOLTAGE    SetVoltage;
  EFI_RK860X_REGULATOR_GET_ENABLE     GetEnable;
  EFI_RK860X_REGULATOR_SET_ENABLE     SetEnable;
  VOLTAGE_RANGE                       SupportedVoltageRange;
  VOLTAGE_RANGE                       PreferredVoltageRange;
  UINT8                               Tag;
  UINT32                              Identifier;
};

extern EFI_GUID  gRk860xRegulatorProtocolGuid;

#endif // __RK860X_REGULATOR_H__
