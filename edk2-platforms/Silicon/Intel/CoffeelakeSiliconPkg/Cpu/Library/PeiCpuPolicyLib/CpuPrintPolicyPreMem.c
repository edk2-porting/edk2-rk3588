/** @file
  This file is PeiCpuPolicy library.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCpuPolicyLibrary.h"
#include <Library/ConfigBlockLib.h>

/**
  Print CPU_CONFIG_LIB_PREMEM_CONFIG and serial out.

  @param[in] CpuConfigLibPreMemConfig     Pointer to a CPU_CONFIG_LIB_PREMEM_CONFIG

**/
VOID
CpuConfigLibPreMemConfigPrint (
  IN CONST CPU_CONFIG_LIB_PREMEM_CONFIG        *CpuConfigLibPreMemConfig
  )
{
  CPU_GENERATION CpuGeneration;
  CpuGeneration = GetCpuGeneration();
  DEBUG ((DEBUG_INFO, "------------------ CPU Config Lib PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : HyperThreading = 0x%x\n", CpuConfigLibPreMemConfig->HyperThreading));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : BootFrequency = 0x%x\n", CpuConfigLibPreMemConfig->BootFrequency));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : ActiveCoreCount = 0x%x\n", CpuConfigLibPreMemConfig->ActiveCoreCount));
  if(CpuGeneration == EnumCflCpu){
    DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : FClkFrequency = 0x%x\n", CpuConfigLibPreMemConfig->FClkFrequency));
  }
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : JtagC10PowerGateDisable = 0x%x\n", CpuConfigLibPreMemConfig->JtagC10PowerGateDisable));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : BistOnReset = 0x%x\n", CpuConfigLibPreMemConfig->BistOnReset));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : VmxEnable = 0x%x\n", CpuConfigLibPreMemConfig->VmxEnable));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : CpuRatio = 0x%x\n", CpuConfigLibPreMemConfig->CpuRatio));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : PeciSxReset = 0x%x\n", CpuConfigLibPreMemConfig->PeciSxReset));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : PeciC10Reset = 0x%x\n", CpuConfigLibPreMemConfig->PeciC10Reset));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : SkipMpInit = 0x%x\n", CpuConfigLibPreMemConfig->SkipMpInit));
  DEBUG ((DEBUG_INFO, "CPU_CONFIG_LIB_PREMEM_CONFIG : DpSscMarginEnable = 0x%x\n", CpuConfigLibPreMemConfig->DpSscMarginEnable));
}

/**
  Print CPU_OVERCLOCKING_PREMEM_CONFIG and serial out.

  @param[in] CpuOverClockingConfig   Pointer to a CPU_OVERCLOCKING_CONFIG
**/
VOID
CpuOverClockingPreMemConfigPrint (
  IN CONST CPU_OVERCLOCKING_PREMEM_CONFIG   *CpuOverClockingPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ CPU OverClocking Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: OcSupport : 0x%X\n", CpuOverClockingPreMemConfig->OcSupport));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: OcLock : 0x%X\n", CpuOverClockingPreMemConfig->OcLock));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: CoreVoltageMode : 0x%X\n", CpuOverClockingPreMemConfig->CoreVoltageMode));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: CoreMaxOcRatio  : 0x%X\n", CpuOverClockingPreMemConfig->CoreMaxOcRatio));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: CoreVoltageOverride : 0x%X\n", CpuOverClockingPreMemConfig->CoreVoltageOverride));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: CoreVoltageAdaptive : 0x%X\n", CpuOverClockingPreMemConfig->CoreVoltageAdaptive));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: CoreVoltageOffset : 0x%X\n", CpuOverClockingPreMemConfig->CoreVoltageOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: RingVoltageMode : 0x%X\n", CpuOverClockingPreMemConfig->RingVoltageMode));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: RingVoltageOverride : 0x%X\n", CpuOverClockingPreMemConfig->RingVoltageOverride));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: RingVoltageAdaptive : 0x%X\n", CpuOverClockingPreMemConfig->RingVoltageAdaptive));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: RingVoltageOffset : 0x%X\n", CpuOverClockingPreMemConfig->RingVoltageOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: RingMaxOcRatio  : 0x%X\n", CpuOverClockingPreMemConfig->RingMaxOcRatio));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: RingDownBin     : 0x%X\n", CpuOverClockingPreMemConfig->RingDownBin));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: Avx2RatioOffset : 0x%X\n", CpuOverClockingPreMemConfig->Avx2RatioOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: Avx3RatioOffset : 0x%X\n", CpuOverClockingPreMemConfig->Avx3RatioOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: BclkAdaptiveVoltage  : 0x%X\n", CpuOverClockingPreMemConfig->BclkAdaptiveVoltage));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: CorePllVoltageOffset : 0x%X\n", CpuOverClockingPreMemConfig->CorePllVoltageOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: GtPllVoltageOffset   : 0x%X\n", CpuOverClockingPreMemConfig->GtPllVoltageOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: RingPllVoltageOffset : 0x%X\n", CpuOverClockingPreMemConfig->RingPllVoltageOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: SaPllVoltageOffset   : 0x%X\n", CpuOverClockingPreMemConfig->SaPllVoltageOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: McPllVoltageOffset   : 0x%X\n", CpuOverClockingPreMemConfig->McPllVoltageOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: TjMaxOffset          : 0x%X\n", CpuOverClockingPreMemConfig->TjMaxOffset));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: TvbRatioClipping     : 0x%X\n", CpuOverClockingPreMemConfig->TvbRatioClipping));
  DEBUG ((DEBUG_INFO, " CPU_OVERCLOCKING_PREMEM_CONFIG:: TvbVoltageOptimization : 0x%X\n", CpuOverClockingPreMemConfig->TvbVoltageOptimization));
}


/**
  Print whole CPU Config blocks of SI_PREMEM_POLICY_PPI and serial out in PreMem.

  @param[in] SiPreMemPolicyPpi The SI Pre-Mem Policy PPI instance
**/
VOID
CpuPreMemPrintPolicy (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  EFI_STATUS                      Status;
  CPU_CONFIG_LIB_PREMEM_CONFIG    *CpuConfigLibPreMemConfig;
  CPU_OVERCLOCKING_PREMEM_CONFIG  *CpuOverclockingPreMemConfig;

  DEBUG ((DEBUG_INFO, "\n------------------------ CPU - SiPreMemPolicyPpi Print Begin in PreMem -----------------\n"));

  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPreMemPolicyPpi->TableHeader.Header.Revision));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuConfigLibPreMemConfigGuid, (VOID *) &CpuConfigLibPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuOverclockingPreMemConfigGuid, (VOID *) &CpuOverclockingPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  CpuConfigLibPreMemConfigPrint(CpuConfigLibPreMemConfig);
  CpuOverClockingPreMemConfigPrint(CpuOverclockingPreMemConfig);

  DEBUG ((DEBUG_INFO, "\n------------------------ CPU - SiPreMemPolicyPpi Print End -----------------\n\n"));
DEBUG_CODE_END();
}
