/** @file

  Copyright 2018-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef CHASSIS_H__
#define CHASSIS_H__

#include <Uefi.h>

#define  NXP_LAYERSCAPE_CHASSIS3V2_DCFG_ADDRESS  0x1E00000

#define SVR_SOC_VER(svr)            (((svr) >> 8) & 0xFFFFFE)
#define SVR_MAJOR(svr)              (((svr) >> 4) & 0xf)
#define SVR_MINOR(svr)              (((svr) >> 0) & 0xf)

/**
  The Device Configuration Unit provides general purpose configuration and
  status for the device. These registers only support 32-bit accesses.
**/
#pragma pack(1)
typedef struct {
  UINT8   Reserved0[0x70 - 0x0];
  UINT32  DeviceDisableRegister1;  // Device Disable Register 1
  UINT32  DeviceDisableRegister2;  // Device Disable Register 2
  UINT32  DeviceDisableRegister3;  // Device Disable Register 3
  UINT32  DeviceDisableRegister4;  // Device Disable Register 4
  UINT32  DeviceDisableRegister5;  // Device Disable Register 5
  UINT32  DeviceDisableRegister6;  // Device Disable Register 6
  UINT32  DeviceDisableRegister7;  // Device Disable Register 7
  UINT8   Reserved1[0xa4 - 0x8c];
  UINT32  Svr;                     // System Version Register
  UINT8   Reserved2[0x100 - 0xa8];
  UINT32  RcwSr[32]; // Reset Control Word Status Register
} NXP_LAYERSCAPE_CHASSIS3V2_DEVICE_CONFIG;
#pragma pack()

#endif // CHASSIS_H__
