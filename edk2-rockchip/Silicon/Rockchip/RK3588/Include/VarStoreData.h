/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __VARSTORE_DATA_H__
#define __VARSTORE_DATA_H__

#define CPU_PERF_CLUSTER_CLOCK_PRESET_BOOTDEFAULT   0
#define CPU_PERF_CLUSTER_CLOCK_PRESET_MIN           1
#define CPU_PERF_CLUSTER_CLOCK_PRESET_MAX           2
#define CPU_PERF_CLUSTER_CLOCK_PRESET_CUSTOM        3
typedef struct {
  UINT32 Preset;
} CPU_PERF_CLUSTER_CLOCK_PRESET_VARSTORE_DATA;

typedef struct {
  UINT32 Mhz;
} CPU_PERF_CLUSTER_CLOCK_CUSTOM_VARSTORE_DATA;

#define CPU_PERF_CLUSTER_VOLTAGE_MODE_AUTO          0
#define CPU_PERF_CLUSTER_VOLTAGE_MODE_CUSTOM        1
typedef struct {
  UINT32 Mode;
} CPU_PERF_CLUSTER_VOLTAGE_MODE_VARSTORE_DATA;

typedef struct {
  UINT32 Microvolts;
} CPU_PERF_CLUSTER_VOLTAGE_CUSTOM_VARSTORE_DATA;

#define COMBO_PHY_MODE_UNCONNECTED                  0
#define COMBO_PHY_MODE_PCIE                         1
#define COMBO_PHY_MODE_SATA                         2
#define COMBO_PHY_MODE_USB3                         3
typedef struct {
  UINT32 Mode;
} COMBO_PHY_MODE_VARSTORE_DATA;

#define PCIE30_STATE_DISABLED                       0
#define PCIE30_STATE_ENABLED                        1
typedef struct {
  UINT32 State;
} PCIE30_STATE_VARSTORE_DATA;

#define ACPI_USB2_STATE_DISABLED                    0
#define ACPI_USB2_STATE_ENABLED                     1
typedef struct {
  UINT32 State;
} ACPI_USB2_STATE_VARSTORE_DATA;

#define COOLING_FAN_STATE_DISABLED                    0
#define COOLING_FAN_STATE_ENABLED                     1
typedef struct {
  UINT32 State;
} COOLING_FAN_STATE_VARSTORE_DATA;

typedef struct {
  UINT32 Percentage;
} COOLING_FAN_SPEED_VARSTORE_DATA;

#define USBDP_PHY_USB3_STATE_DISABLED               0
#define USBDP_PHY_USB3_STATE_ENABLED                1
typedef struct {
  UINT32 State;
} USBDP_PHY_USB3_STATE_VARSTORE_DATA;

#endif // __VARSTORE_DATA_H__
