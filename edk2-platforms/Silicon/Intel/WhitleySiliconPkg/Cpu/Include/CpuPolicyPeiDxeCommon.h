/** @file
  Intel CPU PPM policy common structures and macros for both
  CPU late policy PPI and CPU policy protocol.

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __CPU_POLICY_PEI_DXE_COMMON_HEADER__
#define __CPU_POLICY_PEI_DXE_COMMON_HEADER__

typedef struct {
  BOOLEAN   CpuTStateEnable;
  UINT8     CpuClockModulationDutyCycle;
  BOOLEAN   CpuAesEnable;
  BOOLEAN   CpuFastStringEnable;
  BOOLEAN   CpuMaxCpuidValueLimitEnable;
  BOOLEAN   CpuMachineCheckEnable;
  BOOLEAN   CpuMonitorMwaitEnable;
  BOOLEAN   CpuVtEnable;
  BOOLEAN   CpuLtEnable;
  BOOLEAN   CpuX2ApicEnable;
  BOOLEAN   CpuEistEnable;
  BOOLEAN   CpuTurboModeEnable;
  BOOLEAN   CpuHwCoordinationEnable;
  UINT8     CpuBootPState;
  BOOLEAN   CpuPpinControlEnable;
  BOOLEAN   CpuPeciDownstreamWriteEnable;
  BOOLEAN   CpuL1NextPagePrefetcherDisable;
  BOOLEAN   CpuDcuPrefetcherEnable;
  BOOLEAN   CpuIpPrefetcherEnable;
  BOOLEAN   CpuMlcStreamerPrefetecherEnable;
  BOOLEAN   CpuMlcSpatialPrefetcherEnable;
  BOOLEAN   CpuAmpPrefetchEnable;
  BOOLEAN   CpuThreeStrikeCounterEnable;
  BOOLEAN   CpuCStateEnable;
  UINT8     CpuPackageCStateLimit;
  BOOLEAN   CpuC1AutoDemotionEnable;
  BOOLEAN   CpuC1AutoUndemotionEnable;
  UINT8     CpuCoreCStateValue;
  UINT16    CpuAcpiLvl2Addr;
  BOOLEAN   CpuThermalManagementEnable;
  UINT8     CpuTccActivationOffset;
  BOOLEAN   CpuDbpfEnable;
  BOOLEAN   CpuEnergyPerformanceBiasEnable;
  UINT32    CpuIioLlcWaysBitMask;
  UINT32    CpuExpandedIioLlcWaysBitMask;
  UINT32    CpuRemoteWaysBitMask;
  UINT32    CpuRrqCountThreshold;
  UINT8     CpuMtoIWa;
  BOOLEAN   RunCpuPpmInPei;
  BOOLEAN   AcExceptionOnSplitLockEnable;
  BOOLEAN   CpuCrashLogGprs;
} CPU_POLICY_COMMON;

#endif
