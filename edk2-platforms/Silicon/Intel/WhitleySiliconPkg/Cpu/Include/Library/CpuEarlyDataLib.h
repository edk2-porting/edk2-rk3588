/** @file
  Interface of CPU early data library.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __CPU_EARLY_DATA_LIB_H__
#define __CPU_EARLY_DATA_LIB_H__

#include <CpuDataStruct.h>

typedef struct {
  UINT64  FusedCores[MAX_SOCKET];                         // Fused Core Mask in the package 64bits
  UINT64  ActiveCores[MAX_SOCKET];                        // Active Core Mask in the package 64bits
  UINT8   MaxCoreToBusRatio[MAX_SOCKET];                  // Package Max Non-turbo Ratio (per socket)
  UINT8   MinCoreToBusRatio[MAX_SOCKET];                  // Package Maximum Efficiency Ratio (per socket)
  UINT32  PackageBspApicID[MAX_SOCKET];
  UINT8   IssCapableSystem;                               // 1 = All sockets config TDP / ISS capable
  UINT8   ConfigTdpCapableSystem;                         // 1 = All sockets config TDP capable
  UINT8   IssConfigTdpMaxLevel;                           // B2P CONFIG_TDP_GET_LEVELS_INFO
  UINT8   IssConfigTdpCurrentLevel;                       // B2P CONFIG_TDP_GET_LEVELS_INFO
  UINT8   IssConfigTdpRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_TDP_INFO
  UINT16  IssConfigTdpPower[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_TDP_INFO
  UINT32  IssConfigTdpPowerMinMax[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];  // B2P CONFIG_TDP_GET_POWER_INFO
  UINT8   IssConfigTdpTjmax[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_ICCP_TJMAX_INFO
  UINT8   IssConfigTdpCoreCount[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];    // B2P CONFIG_TDP_GET_CORE_MASK
  UINT64  IssConfigTdpEnabledCoreMask[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];   // B2P CONFIG_TDP_GET_CORE_MASK
  UINT8   PbfCapableSystem;                                         // 1 = All sockets PBF Capable
  UINT8   PbfCapable[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];           // B2P GET_CONFIG_TDP_CONTROL PBF_SUPPORT Bit[1]
  UINT64  PbfP1HiCoreMap[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];     // B2P PBF_GET_CORE_MASK_INFO Bits
  UINT8   PbfP1HighRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];       // B2P PBF_GET_P1HI_P1LO_INFO P1_HI
  UINT8   PbfP1LowRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P PBF_GET_P1HI_P1LO_INFO P1_LO
  UINT32  SstCpSystemStatus;
  UINT8   UncoreP0Ratio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_RATIO_INFO
  UINT8   UncorePnRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_RATIO_INFO
} CPU_VAR_DATA;

#endif
