/** @file
  This file is PeiCpuPolicy library.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCpuPolicyLibrary.h"
#include <Library/ConfigBlockLib.h>
#include <Library/PcdLib.h>

/**
  Print CPU_CONFIG and serial out.

  @param[in] CpuConfig   Pointer to a CPU_CONFIG
**/
VOID
CpuConfigPrint (
  IN CONST CPU_CONFIG   *CpuConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ CPU Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " CPU_CONFIG : AesEnable : 0x%x\n", CpuConfig->AesEnable));
  DEBUG ((DEBUG_INFO, " CPU_CONFIG : MicrocodePatchAddress : 0x%x\n", CpuConfig->MicrocodePatchAddress));
  DEBUG ((DEBUG_INFO, " CPU_CONFIG : DebugInterfaceEnable : 0x%X\n", CpuConfig->DebugInterfaceEnable));
}

/**
  Print CPU_POWER_MGMT_BASIC_CONFIG and serial out.

  @param[in] CpuPowerMgmtBasicConfig   Pointer to a CPU_POWER_MGMT_BASIC_CONFIG
**/
VOID
CpuPowerMgmtBasicConfigPrint (
  IN CONST CPU_POWER_MGMT_BASIC_CONFIG   *CpuPowerMgmtBasicConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ CPU Power Mgmt Basic Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG : OneCoreRatioLimit : 0x%X , TwoCoreRatioLimit = 0x%X , ThreeCoreRatioLimit = 0x%X , FourCoreRatioLimit = 0x%X, FiveCoreRatioLimit = 0x%X, SixCoreRatioLimit = 0x%X, SevenCoreRatioLimit = 0x%X, EightCoreRatioLimit = 0x%X \n",  CpuPowerMgmtBasicConfig->OneCoreRatioLimit, \
          CpuPowerMgmtBasicConfig->TwoCoreRatioLimit, \
          CpuPowerMgmtBasicConfig->ThreeCoreRatioLimit, \
          CpuPowerMgmtBasicConfig->FourCoreRatioLimit, \
          CpuPowerMgmtBasicConfig->FiveCoreRatioLimit, \
          CpuPowerMgmtBasicConfig->SixCoreRatioLimit, \
          CpuPowerMgmtBasicConfig->SevenCoreRatioLimit, \
          CpuPowerMgmtBasicConfig->EightCoreRatioLimit));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: Hwp : 0x%x\n", CpuPowerMgmtBasicConfig->Hwp));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: SkipSetBootPState : 0x%x\n", CpuPowerMgmtBasicConfig->SkipSetBootPState));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: HdcControl : 0x%X\n", CpuPowerMgmtBasicConfig->HdcControl));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: (Intel Turbo Boost Max Technology 3.0)EnableItbm : 0x%X\n", CpuPowerMgmtBasicConfig->EnableItbm));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: EnableItbmDriver : 0x%X\n", CpuPowerMgmtBasicConfig->EnableItbmDriver));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit2 : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit2));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: TurboPowerLimitLock : 0x%x\n", CpuPowerMgmtBasicConfig->TurboPowerLimitLock));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit3DutyCycle : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit3DutyCycle));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit3Lock : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit3Lock));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit4Lock : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit4Lock));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: TccOffsetClamp : 0x%X\n", CpuPowerMgmtBasicConfig->TccOffsetClamp));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: TccOffsetLock : 0x%X\n", CpuPowerMgmtBasicConfig->TccOffsetLock));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: TurboMode : 0x%x\n", CpuPowerMgmtBasicConfig->TurboMode));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: TccActivationOffset : 0x%X\n", CpuPowerMgmtBasicConfig->TccActivationOffset));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit1 : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit1));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit2Power : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit2Power));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit3 : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit3));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit4 : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit4));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit1Time : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit1Time));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: PowerLimit3Time : 0x%x\n", CpuPowerMgmtBasicConfig->PowerLimit3Time));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: TccOffsetTimeWindowForRatl : 0x%X\n", CpuPowerMgmtBasicConfig->TccOffsetTimeWindowForRatl));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: HwpInterruptControl : 0x%x\n", CpuPowerMgmtBasicConfig->HwpInterruptControl));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: MinRingRatioLimit : 0x%x\n", CpuPowerMgmtBasicConfig->MinRingRatioLimit));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_BASIC_CONFIG: MaxRingRatioLimit : 0x%x\n", CpuPowerMgmtBasicConfig->MaxRingRatioLimit));
}

/**
  Print CPU_POWER_MGMT_CUSTOM_CONFIG and serial out.

  @param[in] CpuPowerMgmtCustomConfig   Pointer to a CPU_POWER_MGMT_CUSTOM_CONFIG
**/
VOID
CpuPowerMgmtCustomConfigPrint (
  IN CONST CPU_POWER_MGMT_CUSTOM_CONFIG   *CpuPowerMgmtCustomConfig
  )
{
  UINT32 Index = 0;
  DEBUG ((DEBUG_INFO, "------------------ CPU Power Mgmt Custom Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "\n CustomRatioTable... \n"));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_CUSTOM_CONFIG: VidNumber : 0x%x\n", CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_CUSTOM_CONFIG: VidCpuid : 0x%x\n", CpuPowerMgmtCustomConfig->CustomRatioTable.Cpuid));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_CUSTOM_CONFIG: VidMaxRatio : 0x%x\n", CpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio));
  for (Index = 0; Index < MAX_CUSTOM_RATIO_TABLE_ENTRIES; Index++) {
    DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_CUSTOM_CONFIG: StateRatio[%d] : 0x%x\n", Index, CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatio[Index]));
  }
  for (Index = 0; Index < MAX_16_CUSTOM_RATIO_TABLE_ENTRIES; Index++) {
    DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_CUSTOM_CONFIG: StateRatioMax16[%d] : 0x%x\n", Index, CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatioMax16[Index]));
  }
  for (Index = 0; Index < MAX_CUSTOM_CTDP_ENTRIES; Index++) {
    DEBUG (
            (DEBUG_INFO,
             " CPU_POWER_MGMT_CUSTOM_CONFIG: CustomConfigTdpTable[%d] CustomPowerLimit1 : 0x%x\n",
             Index,CpuPowerMgmtCustomConfig->CustomConfigTdpTable[Index].CustomPowerLimit1)
            );
    DEBUG (
            (DEBUG_INFO,
             " CPU_POWER_MGMT_CUSTOM_CONFIG: CustomConfigTdpTable[%d] CustomPowerLimit2 : 0x%x\n",
             Index,CpuPowerMgmtCustomConfig->CustomConfigTdpTable[Index].CustomPowerLimit2)
            );
    DEBUG (
            (DEBUG_INFO,
             " CPU_POWER_MGMT_CUSTOM_CONFIG: CustomConfigTdpTable[%d] CustomPowerLimit1Time : 0x%x\n",
             Index,CpuPowerMgmtCustomConfig->CustomConfigTdpTable[Index].CustomPowerLimit1Time)
            );
    DEBUG (
            (DEBUG_INFO,
             " CPU_POWER_MGMT_CUSTOM_CONFIG: CustomConfigTdpTable[%d] CustomTurboActivationRatio : 0x%x\n",
             Index,CpuPowerMgmtCustomConfig->CustomConfigTdpTable[Index].CustomTurboActivationRatio)
            );
  }
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_CUSTOM_CONFIG: ConfigTdpLock : 0x%x\n", CpuPowerMgmtCustomConfig->ConfigTdpLock));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_CUSTOM_CONFIG: ConfigTdpBios : 0x%x\n", CpuPowerMgmtCustomConfig->ConfigTdpBios));
}

/**
  Print CPU_TEST_CONFIG and serial out.

  @param[in] CpuTestConfig   Pointer to a CPU_TEST_CONFIG
**/
VOID
CpuTestConfigPrint (
  IN CONST CPU_TEST_CONFIG   *CpuTestConfig
  )
{
  UINT8 PcdCpuApLoopMode;

  PcdCpuApLoopMode = PcdGet8 (PcdCpuApLoopMode);

  DEBUG ((DEBUG_INFO, "------------------ CPU Test Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: MlcStreamerPrefetcher : 0x%X\n", CpuTestConfig->MlcStreamerPrefetcher));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: MlcSpatialPrefetcher : 0x%X\n", CpuTestConfig->MlcSpatialPrefetcher));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: MonitorMwaitEnable : 0x%X\n", CpuTestConfig->MonitorMwaitEnable));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: MachineCheckEnable : 0x%X\n", CpuTestConfig->MachineCheckEnable));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: DebugInterfaceLockEnable : 0x%X\n", CpuTestConfig->DebugInterfaceLockEnable));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: ProcessorTraceOutputScheme : 0x%X\n", CpuTestConfig->ProcessorTraceOutputScheme));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: ProcessorTraceEnable : 0x%X\n", CpuTestConfig->ProcessorTraceEnable));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: ProcessorTraceMemBase : 0x%llX\n", CpuTestConfig->ProcessorTraceMemBase));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: ProcessorTraceMemLength : 0x%X\n", CpuTestConfig->ProcessorTraceMemLength));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: ThreeStrikeCounterDisable : 0x%X\n", CpuTestConfig->ThreeStrikeCounterDisable));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: VoltageOptimization : 0x%X\n", CpuTestConfig->VoltageOptimization));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: CpuWakeUpTimer : 0x%X\n", CpuTestConfig->CpuWakeUpTimer));
  DEBUG ((DEBUG_INFO, " CPU_TEST_CONFIG: PcdCpuApLoopMode: 0x%X\n", PcdCpuApLoopMode));
}

/**
  Print CPU_PID_TEST_CONFIG and serial out.

  @param[in] CpuPidTestConfig   Pointer to a CPU_PID_TEST_CONFIG
**/
VOID
CpuPidTestConfigPrint (
  IN CONST CPU_PID_TEST_CONFIG   *CpuPidTestConfig
  )
{
  UINT32 Index = 0;
  DEBUG ((DEBUG_INFO, "------------------ CPU PID Test Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : PidTuning : 0x%X\n", Index,  CpuPidTestConfig->PidTuning));
  if ( CpuPidTestConfig->PidTuning == 1) {
    for (Index = PID_DOMAIN_KP; Index <= PID_DOMAIN_KD; Index++) {
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : Ratl[%X] : 0x%X\n", Index,  CpuPidTestConfig->Ratl[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : VrTdcVr0[%X] : 0x%X\n", Index,  CpuPidTestConfig->VrTdcVr0[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : VrTdcVr1[%X] : 0x%X\n", Index,  CpuPidTestConfig->VrTdcVr1[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : VrTdcVr2[%X] : 0x%X\n", Index,  CpuPidTestConfig->VrTdcVr2[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : VrTdcVr3[%X] : 0x%X\n", Index,  CpuPidTestConfig->VrTdcVr3[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : PbmPsysPl1Msr[%X] : 0x%X\n", Index,  CpuPidTestConfig->PbmPsysPl1Msr[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : PbmPsysPl1MmioPcs[%X] : 0x%X\n", Index,  CpuPidTestConfig->PbmPsysPl1MmioPcs[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : PbmPsysPl2Msr[%X] : 0x%X\n", Index,  CpuPidTestConfig->PbmPsysPl2Msr[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : PbmPsysPl2MmioPcs[%X] : 0x%X\n", Index,  CpuPidTestConfig->PbmPsysPl2MmioPcs[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : PbmPkgPl1Msr[%X] : 0x%X\n", Index,  CpuPidTestConfig->PbmPkgPl1Msr[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : PbmPkgPl1MmioPcs[%X] : 0x%X\n", Index,  CpuPidTestConfig->PbmPkgPl1MmioPcs[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : PbmPkgPl2Msr[%X] : 0x%X\n", Index,  CpuPidTestConfig->PbmPkgPl2Msr[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : PbmPkgPl2MmioPcs[%X] : 0x%X\n", Index,  CpuPidTestConfig->PbmPkgPl2MmioPcs[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : DdrPl1Msr[%X] : 0x%X\n", Index,  CpuPidTestConfig->DdrPl1Msr[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : DdrPl1MmioPcs[%X] : 0x%X\n", Index,  CpuPidTestConfig->DdrPl1MmioPcs[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : DdrPl2Msr[%X] : 0x%X\n", Index,  CpuPidTestConfig->DdrPl2Msr[Index]));
        DEBUG ((DEBUG_INFO, " CPU_PID_TEST_CONFIG : DdrPl2MmioPcs[%X] : 0x%X\n", Index,  CpuPidTestConfig->DdrPl2MmioPcs[Index]));
    }
  }
}

/**
  Print CPU_POWER_MGMT_TEST_CONFIG and serial out.

  @param[in] CpuPowerMgmtTestConfig   Pointer to a CPU_POWER_MGMT_TEST_CONFIG
**/
VOID
CpuPowerMgmtTestConfigPrint (
  IN CONST CPU_POWER_MGMT_TEST_CONFIG   *CpuPowerMgmtTestConfig
  )
{
  CPU_GENERATION CpuGeneration;
  CpuGeneration = GetCpuGeneration();
  DEBUG ((DEBUG_INFO, "------------------ CPU Power Mgmt Test Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: Eist : 0x%x\n", CpuPowerMgmtTestConfig->Eist));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: EnergyEfficientPState : 0x%x\n", CpuPowerMgmtTestConfig->EnergyEfficientPState));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: EnergyEfficientTurbo : 0x%x\n", CpuPowerMgmtTestConfig->EnergyEfficientTurbo));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: TStates : 0x%x\n", CpuPowerMgmtTestConfig->TStates));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: BiProcHot : 0x%x\n", CpuPowerMgmtTestConfig->BiProcHot));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: DisableProcHotOut : 0x%x\n", CpuPowerMgmtTestConfig->DisableProcHotOut));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: ProcHotResponse : 0x%x\n", CpuPowerMgmtTestConfig->ProcHotResponse));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: DisableVrThermalAlert : 0x%x\n", CpuPowerMgmtTestConfig->DisableVrThermalAlert));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: AutoThermalReporting : 0x%x\n", CpuPowerMgmtTestConfig->AutoThermalReporting));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: ThermalMonitor : 0x%x\n", CpuPowerMgmtTestConfig->ThermalMonitor));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: Cx : 0x%x\n", CpuPowerMgmtTestConfig->Cx));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: PmgCstCfgCtrlLock : 0x%x\n", CpuPowerMgmtTestConfig->PmgCstCfgCtrlLock));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: C1e : 0x%x\n", CpuPowerMgmtTestConfig->C1e));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: C1Autodemotion : 0x%x\n", CpuPowerMgmtTestConfig->C1AutoDemotion));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: C1Undemotion : 0x%x\n", CpuPowerMgmtTestConfig->C1UnDemotion));
  if(CpuGeneration == EnumCflCpu){
    DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: C3AutoDemotion : 0x%x\n", CpuPowerMgmtTestConfig->C3AutoDemotion));
    DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: C3UnDemotion : 0x%x\n", CpuPowerMgmtTestConfig->C3UnDemotion));
  }
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: PkgCState Demotion : 0x%x\n", CpuPowerMgmtTestConfig->PkgCStateDemotion));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: PkgCstateUndemotion : 0x%x\n", CpuPowerMgmtTestConfig->PkgCStateUnDemotion));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CStatePreWake : 0x%x\n", CpuPowerMgmtTestConfig->CStatePreWake));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: TimedMwait : 0x%x\n", CpuPowerMgmtTestConfig->TimedMwait));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstCfgCtrIoMwaitRedirection : 0x%x\n", CpuPowerMgmtTestConfig->CstCfgCtrIoMwaitRedirection));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: ProcHotLock : 0x%x\n", CpuPowerMgmtTestConfig->ProcHotLock));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: ConfigTdpLevel : 0x%x\n", CpuPowerMgmtTestConfig->ConfigTdpLevel));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: RaceToHalt  : 0x%x\n",  CpuPowerMgmtTestConfig->RaceToHalt));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl0Irtl : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl0Irtl));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl0TimeUnit : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl0TimeUnit));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl1Irtl : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl1Irtl));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl2Irtl : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl2Irtl));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl3Irtl : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl3Irtl));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl4Irtl : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl4Irtl));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl5Irtl : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl5Irtl));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: PkgCStateLimit : 0x%x\n", CpuPowerMgmtTestConfig->PkgCStateLimit));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl1TimeUnit : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl1TimeUnit));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl2TimeUnit : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl2TimeUnit));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl3TimeUnit : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl3TimeUnit));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl4TimeUnit : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl4TimeUnit));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CstateLatencyControl5TimeUnit : 0x%x\n", CpuPowerMgmtTestConfig->CstateLatencyControl5TimeUnit));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_TEST_CONFIG: CustomPowerUnit : 0x%x\n", CpuPowerMgmtTestConfig->CustomPowerUnit));
  DEBUG ((DEBUG_INFO, " PpmIrmSetting : 0x%x\n", CpuPowerMgmtTestConfig->PpmIrmSetting));
}
/**
  Print whole CPU config blocks of SI_POLICY_PPI and serial out in PostMem.

  @param[in] SiPolicyPpi The SI Policy PPI instance
**/
VOID
CpuPrintPolicy (
  IN  SI_POLICY_PPI       *SiPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  EFI_STATUS                       Status;
  CPU_CONFIG                       *CpuConfig;
  CPU_POWER_MGMT_BASIC_CONFIG      *CpuPowerMgmtBasicConfig;
  CPU_POWER_MGMT_CUSTOM_CONFIG     *CpuPowerMgmtCustomConfig;
  CPU_TEST_CONFIG                  *CpuTestConfig;
  CPU_PID_TEST_CONFIG              *CpuPidTestConfig;
  CPU_POWER_MGMT_TEST_CONFIG       *CpuPowerMgmtTestConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuConfigGuid, (VOID *) &CpuConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtCustomConfigGuid, (VOID *) &CpuPowerMgmtCustomConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuTestConfigGuid, (VOID *) &CpuTestConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPidTestConfigGuid, (VOID *) &CpuPidTestConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *) &CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n ------------------------ SiCpuPolicy Print Begin in PostMem----------------- \n"));
  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPolicyPpi->TableHeader.Header.Revision));

  CpuConfigPrint(CpuConfig);
  CpuPowerMgmtBasicConfigPrint(CpuPowerMgmtBasicConfig);
  CpuPowerMgmtCustomConfigPrint(CpuPowerMgmtCustomConfig);
  CpuTestConfigPrint(CpuTestConfig);
  CpuPidTestConfigPrint(CpuPidTestConfig);
  CpuPowerMgmtTestConfigPrint(CpuPowerMgmtTestConfig);
  DEBUG ((DEBUG_INFO, "\n ------------------------ SiCpuPolicy Print End in PostMem ----------------- \n\n"));
DEBUG_CODE_END();
}

