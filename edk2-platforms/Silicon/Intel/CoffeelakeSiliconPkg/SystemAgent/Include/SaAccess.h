/** @file
  Macros to simplify and abstract the interface to PCI configuration.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SAACCESS_H_
#define _SAACCESS_H_

#include "SaRegs.h"
#include "SaCommonDefinitions.h"

///
/// SystemAgent Base Address definition
///
#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND  1
#endif
#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND  1000
#endif

//
// SA Segement Number
//
#define SA_SEG_NUM         0x00

#define V_SA_DEVICE_ID_INVALID 0xFFFF


///
/// The value before AutoConfig match the setting of PCI Express Base Specification 1.1, please be careful for adding new feature
///
typedef enum {
  PcieAspmDisabled,
  PcieAspmL0s,
  PcieAspmL1,
  PcieAspmL0sL1,
  PcieAspmAutoConfig,
  PcieAspmMax
} SA_PCIE_ASPM_CONFIG;

///
/// SgMode settings
///
typedef enum {
  SgModeDisabled = 0,
  SgModeReserved,
  SgModeMuxless,
  SgModeDgpu,
  SgModeMax
} SG_MODE;

//
// Macros that judge which type a device ID belongs to
//
#define IS_SA_DEVICE_ID_MOBILE(DeviceId) \
    ( \
      (DeviceId == V_SA_DEVICE_ID_KBL_MB_ULT_1) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_ULT_1) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_ULT_2) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_ULT_3) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_ULT_4) \
    )

///
/// Device IDs that are Desktop specific B0:D0:F0
///
#define IS_SA_DEVICE_ID_DESKTOP(DeviceId) \
    ( \
      (DeviceId == V_SA_DEVICE_ID_KBL_DT_2) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_DT_1) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_DT_2) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_DT_3) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_DT_4) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_WS_1) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_WS_2) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_WS_3) \
    )

///
/// Device IDS that are Server specific B0:D0:F0
///
#define IS_SA_DEVICE_ID_SERVER(DeviceId) \
    ( \
      (DeviceId == V_SA_DEVICE_ID_KBL_SVR_2) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_SVR_1) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_SVR_2) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_SVR_3) \
    )

///
/// Device IDs that are Halo specific B0:D0:F0
///
#define IS_SA_DEVICE_ID_HALO(DeviceId) \
    ( \
      (DeviceId == V_SA_DEVICE_ID_KBL_HALO_2) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_HALO_1) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_HALO_2) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_HALO_3) || \
      (DeviceId == V_SA_DEVICE_ID_CFL_HALO_IOT_1) \
    )

#endif
