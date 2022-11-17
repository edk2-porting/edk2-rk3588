/** @file
  Implementation of Fsp PCH Policy Initialization.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PeiFspPolicyInitLib.h>

#include <Ppi/SiPolicy.h>

#include <CpuAccess.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <FspEas.h>

/**
  Performs FSP CPU PEI Policy initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspCpuPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  EFI_STATUS                      Status;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi;
  CPU_OVERCLOCKING_PREMEM_CONFIG  *CpuOverClockingPreMemConfig;
  CPU_CONFIG_LIB_PREMEM_CONFIG    *CpuConfigLibPreMemConfig;
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Update SiCpuPolicy Pre-Mem Start\n"));

  //
  // Locate SiPreMemPolicyPpi
  //
  SiPreMemPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuOverclockingPreMemConfigGuid, (VOID *) &CpuOverClockingPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuConfigLibPreMemConfigGuid, (VOID *) &CpuConfigLibPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  ///
  ///Test RC Policies
  ///

  ///
  ///
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Update SiCpuPolicy Pre-Mem End\n"));

  //
  // Overclocking PreMem policies
  //
  FspmUpd->FspmConfig.OcSupport             = (UINT8) CpuOverClockingPreMemConfig->OcSupport;
  FspmUpd->FspmConfig.OcLock                = (UINT8) CpuOverClockingPreMemConfig->OcLock;
  FspmUpd->FspmConfig.CoreMaxOcRatio        = (UINT8) CpuOverClockingPreMemConfig->CoreMaxOcRatio;
  FspmUpd->FspmConfig.CoreVoltageMode       = (UINT8) CpuOverClockingPreMemConfig->CoreVoltageMode;
  FspmUpd->FspmConfig.RingMinOcRatio        = (UINT8) CpuOverClockingPreMemConfig->RingMinOcRatio;
  FspmUpd->FspmConfig.RingMaxOcRatio        = (UINT8) CpuOverClockingPreMemConfig->RingMaxOcRatio;
  FspmUpd->FspmConfig.CoreVoltageOverride   = (UINT16) CpuOverClockingPreMemConfig->CoreVoltageOverride;
  FspmUpd->FspmConfig.CoreVoltageAdaptive   = (UINT16) CpuOverClockingPreMemConfig->CoreVoltageAdaptive;
  FspmUpd->FspmConfig.CoreVoltageOffset     = (UINT16) CpuOverClockingPreMemConfig->CoreVoltageOffset;
  FspmUpd->FspmConfig.CorePllVoltageOffset  = (UINT8) CpuOverClockingPreMemConfig->CorePllVoltageOffset;
  FspmUpd->FspmConfig.GtPllVoltageOffset    = (UINT8) CpuOverClockingPreMemConfig->GtPllVoltageOffset;
  FspmUpd->FspmConfig.RingPllVoltageOffset  = (UINT8) CpuOverClockingPreMemConfig->RingPllVoltageOffset;
  FspmUpd->FspmConfig.SaPllVoltageOffset    = (UINT8) CpuOverClockingPreMemConfig->SaPllVoltageOffset;
  FspmUpd->FspmConfig.McPllVoltageOffset    = (UINT8) CpuOverClockingPreMemConfig->McPllVoltageOffset;
  FspmUpd->FspmConfig.Avx2RatioOffset       = (UINT8) CpuOverClockingPreMemConfig->Avx2RatioOffset;
  FspmUpd->FspmConfig.BclkAdaptiveVoltage   = (UINT8) CpuOverClockingPreMemConfig->BclkAdaptiveVoltage;

  //
  //  Cpu Config Lib policies
  //
  FspmUpd->FspmConfig.HyperThreading            = (UINT8) CpuConfigLibPreMemConfig->HyperThreading;
  FspmUpd->FspmConfig.BootFrequency             = (UINT8) CpuConfigLibPreMemConfig->BootFrequency;
  FspmUpd->FspmConfig.ActiveCoreCount           = (UINT8) CpuConfigLibPreMemConfig->ActiveCoreCount;
  FspmUpd->FspmConfig.JtagC10PowerGateDisable   = (UINT8) CpuConfigLibPreMemConfig->JtagC10PowerGateDisable;
  FspmUpd->FspmConfig.FClkFrequency             = (UINT8) CpuConfigLibPreMemConfig->FClkFrequency;
  FspmUpd->FspmConfig.BistOnReset               = (UINT8) CpuConfigLibPreMemConfig->BistOnReset;
  FspmUpd->FspmConfig.VmxEnable                 = (UINT8) CpuConfigLibPreMemConfig->VmxEnable;
  FspmUpd->FspmConfig.CpuRatioOverride          = (UINT8) CpuConfigLibPreMemConfig->CpuRatioOverride;
  FspmUpd->FspmConfig.CpuRatio                  = (UINT8) CpuConfigLibPreMemConfig->CpuRatio;
  FspmUpd->FspmConfig.PeciSxReset               = (UINT8) CpuConfigLibPreMemConfig->PeciSxReset;
  FspmUpd->FspmConfig.PeciC10Reset              = (UINT8) CpuConfigLibPreMemConfig->PeciC10Reset;

  return EFI_SUCCESS;
}

/**
 This routine is used to search SMRAM and get SmramCpuData point.

 @retval SmramCpuData - The pointer of CPU information in SMRAM.
 @retval NULL         - Unable to find the CPU information.
 **/

STATIC
SMRAM_CPU_DATA *
GetSmmCpuData(
  VOID
  )
{
  UINT32                  Address;
  SMRAM_CPU_DATA          *SmramCpuData;
  EFI_PEI_HOB_POINTERS    Hob;
  EFI_STATUS              Status;
  UINT32                  TsegBase = 0;
  UINT32                  TsegSize = 0;

  //
  // Find TSEG Base using Fsp Resource Descriptor
  //

  Status = PeiServicesGetHobList ((VOID **) &Hob.Raw);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED)
          && (Hob.ResourceDescriptor->PhysicalStart >= BASE_1MB)
          && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)
          && (CompareGuid (&Hob.ResourceDescriptor->Owner, &gFspReservedMemoryResourceHobTsegGuid))) {

        TsegBase = (UINT32)Hob.ResourceDescriptor->PhysicalStart;
        TsegSize = (UINT32)Hob.ResourceDescriptor->ResourceLength;
        DEBUG((DEBUG_INFO, "Found gFspReservedMemoryResourceHobTsegGuid\n"));
        break;
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }


  ///
  /// Search SMRAM on page alignment for the SMMNVS signature
  ///
  if(TsegBase && TsegSize ){
    for (Address = (TsegBase + TsegSize - EFI_PAGE_SIZE); Address >= TsegBase; Address -= EFI_PAGE_SIZE ) {
      SmramCpuData = (SMRAM_CPU_DATA *) (UINTN) Address;
      if (CompareGuid(&SmramCpuData->HeaderGuid, &gSmramCpuDataHeaderGuid)) {
        return SmramCpuData;
      }
    }
  }

  DEBUG((DEBUG_WARN, "SmramCpuData not found\n"));
  return NULL;
}

/**
 Performs FSP CPU PEI Policy post memory initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspCpuPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
  EFI_STATUS                       Status;
  SI_POLICY_PPI                    *SiPolicyPpi;
  CPU_CONFIG                       *CpuConfig;
  CPU_POWER_MGMT_BASIC_CONFIG      *CpuPowerMgmtBasicConfig;
  CPU_POWER_MGMT_CUSTOM_CONFIG     *CpuPowerMgmtCustomConfig;
  CPU_TEST_CONFIG                  *CpuTestConfig;
  CPU_POWER_MGMT_TEST_CONFIG       *CpuPowerMgmtTestConfig;
  UINTN                             Index;
  SMRAM_CPU_DATA                    *SmramCpuData;
  UINT32                            S3BspMtrrTablePointer;
  UINT32                            CpuS3ResumeDataPointer;
  EFI_BOOT_MODE                     BootMode;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Update SiCpuPolicy\n"));
  //
  // Locate gSiPolicyPpiGuid
  //
  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuConfigGuid, (VOID *) &CpuConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtCustomConfigGuid, (VOID *) &CpuPowerMgmtCustomConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuTestConfigGuid, (VOID *) &CpuTestConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *) &CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR (Status);
  //
  // Production RC Policies
  //

  CopyMem ((VOID *)&(FspsUpd->FspsConfig.CpuConfig), (VOID *)&(((CPU_CONFIG_UNION *)CpuConfig)->CpuConfigFsp.Data), sizeof (UINT32));
  FspsUpd->FspsConfig.MicrocodePatchAddress     = CpuConfig->MicrocodePatchAddress;

  FspsUpd->FspsConfig.TurboMode                 = (UINT8) CpuPowerMgmtBasicConfig->TurboMode;

  ///
  ///Test RC Policies
  ///
  FspsUpd->FspsTestConfig.MlcStreamerPrefetcher    = (UINT8) CpuTestConfig->MlcStreamerPrefetcher;
  FspsUpd->FspsTestConfig.MlcSpatialPrefetcher     = (UINT8) CpuTestConfig->MlcSpatialPrefetcher;
  FspsUpd->FspsTestConfig.MonitorMwaitEnable       = (UINT8) CpuTestConfig->MonitorMwaitEnable;
  FspsUpd->FspsTestConfig.DebugInterfaceEnable     = (UINT8) CpuTestConfig->DebugInterfaceEnable;
  FspsUpd->FspsTestConfig.DebugInterfaceLockEnable = (UINT8) CpuTestConfig->DebugInterfaceLockEnable;
  FspsUpd->FspsTestConfig.ApIdleManner             = (UINT8) CpuTestConfig->ApIdleManner;
  FspsUpd->FspsTestConfig.ApHandoffManner          = (UINT8) CpuTestConfig->ApHandoffManner;
  FspsUpd->FspsTestConfig.ProcTraceOutputScheme    = (UINT8) CpuTestConfig->ProcTraceOutputScheme;
  FspsUpd->FspsTestConfig.ProcTraceEnable          = (UINT8) CpuTestConfig->ProcTraceEnable;
  FspsUpd->FspsTestConfig.ProcTraceMemSize         = (UINT8) CpuTestConfig->ProcTraceMemSize;
  FspsUpd->FspsTestConfig.VoltageOptimization      = (UINT8) CpuTestConfig->VoltageOptimization;


  FspsUpd->FspsTestConfig.OneCoreRatioLimit          = (UINT8) CpuPowerMgmtBasicConfig->OneCoreRatioLimit;
  FspsUpd->FspsTestConfig.TwoCoreRatioLimit          = (UINT8) CpuPowerMgmtBasicConfig->TwoCoreRatioLimit;
  FspsUpd->FspsTestConfig.ThreeCoreRatioLimit        = (UINT8) CpuPowerMgmtBasicConfig->ThreeCoreRatioLimit;
  FspsUpd->FspsTestConfig.FourCoreRatioLimit         = (UINT8) CpuPowerMgmtBasicConfig->FourCoreRatioLimit;
  FspsUpd->FspsTestConfig.FiveCoreRatioLimit         = (UINT8) CpuPowerMgmtBasicConfig->FiveCoreRatioLimit;
  FspsUpd->FspsTestConfig.SixCoreRatioLimit          = (UINT8) CpuPowerMgmtBasicConfig->SixCoreRatioLimit;
  FspsUpd->FspsTestConfig.SevenCoreRatioLimit        = (UINT8) CpuPowerMgmtBasicConfig->SevenCoreRatioLimit;
  FspsUpd->FspsTestConfig.EightCoreRatioLimit        = (UINT8) CpuPowerMgmtBasicConfig->EightCoreRatioLimit;
  FspsUpd->FspsTestConfig.Hwp                        = (UINT8) CpuPowerMgmtBasicConfig->Hwp;
  FspsUpd->FspsTestConfig.HdcControl                 = (UINT8) CpuPowerMgmtBasicConfig->HdcControl;
  FspsUpd->FspsTestConfig.PowerLimit1Time            = (UINT8) CpuPowerMgmtBasicConfig->PowerLimit1Time;
  FspsUpd->FspsTestConfig.PowerLimit2                = (UINT8) CpuPowerMgmtBasicConfig->PowerLimit2;
  FspsUpd->FspsTestConfig.TurboPowerLimitLock        = (UINT8) CpuPowerMgmtBasicConfig->TurboPowerLimitLock;
  FspsUpd->FspsTestConfig.PowerLimit3Time            = (UINT8) CpuPowerMgmtBasicConfig->PowerLimit3Time;
  FspsUpd->FspsTestConfig.PowerLimit3DutyCycle       = (UINT8) CpuPowerMgmtBasicConfig->PowerLimit3DutyCycle;
  FspsUpd->FspsTestConfig.PowerLimit3Lock            = (UINT8) CpuPowerMgmtBasicConfig->PowerLimit3Lock;
  FspsUpd->FspsTestConfig.PowerLimit4Lock            = (UINT8) CpuPowerMgmtBasicConfig->PowerLimit4Lock;
  FspsUpd->FspsTestConfig.TccActivationOffset        = (UINT8) CpuPowerMgmtBasicConfig->TccActivationOffset;
  FspsUpd->FspsTestConfig.TccOffsetClamp             = (UINT8) CpuPowerMgmtBasicConfig->TccOffsetClamp;
  FspsUpd->FspsTestConfig.TccOffsetLock              = (UINT8) CpuPowerMgmtBasicConfig->TccOffsetLock;
  FspsUpd->FspsTestConfig.PowerLimit1                = (UINT32) (CpuPowerMgmtBasicConfig->PowerLimit1 * 125);
  FspsUpd->FspsTestConfig.PowerLimit2Power           = (UINT32) (CpuPowerMgmtBasicConfig->PowerLimit2Power * 125);
  FspsUpd->FspsTestConfig.PowerLimit3                = (UINT32) (CpuPowerMgmtBasicConfig->PowerLimit3 * 125);
  FspsUpd->FspsTestConfig.PowerLimit4                = (UINT32) (CpuPowerMgmtBasicConfig->PowerLimit4 * 125);
  FspsUpd->FspsTestConfig.TccOffsetTimeWindowForRatl = (UINT32) CpuPowerMgmtBasicConfig->TccOffsetTimeWindowForRatl;

  FspsUpd->FspsTestConfig.NumberOfEntries             = (UINT8) CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries;
  FspsUpd->FspsTestConfig.Custom1PowerLimit1Time      = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[0].CustomPowerLimit1Time;
  FspsUpd->FspsTestConfig.Custom2PowerLimit1Time      = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[1].CustomPowerLimit1Time;
  FspsUpd->FspsTestConfig.Custom3PowerLimit1Time      = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[2].CustomPowerLimit1Time;
  FspsUpd->FspsTestConfig.Custom1TurboActivationRatio = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[0].CustomTurboActivationRatio;
  FspsUpd->FspsTestConfig.Custom2TurboActivationRatio = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[1].CustomTurboActivationRatio;
  FspsUpd->FspsTestConfig.Custom3TurboActivationRatio = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[2].CustomTurboActivationRatio;
  FspsUpd->FspsTestConfig.ConfigTdpLock               = (UINT8) CpuPowerMgmtCustomConfig->ConfigTdpLock;
  FspsUpd->FspsTestConfig.ConfigTdpBios               = (UINT8) CpuPowerMgmtCustomConfig->ConfigTdpBios;
  FspsUpd->FspsTestConfig.MaxRatio                    = (UINT16) CpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio;
  for (Index = 0; Index < CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries; Index++) {
      FspsUpd->FspsTestConfig.StateRatio[Index]       = (UINT16) CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatio[Index];
  }
  FspsUpd->FspsTestConfig.Custom1PowerLimit1          = (UINT32) (CpuPowerMgmtCustomConfig->CustomConfigTdpTable[0].CustomPowerLimit1 * 125);
  FspsUpd->FspsTestConfig.Custom1PowerLimit2          = (UINT32) (CpuPowerMgmtCustomConfig->CustomConfigTdpTable[0].CustomPowerLimit2 * 125);
  FspsUpd->FspsTestConfig.Custom2PowerLimit1          = (UINT32) (CpuPowerMgmtCustomConfig->CustomConfigTdpTable[1].CustomPowerLimit1 * 125);
  FspsUpd->FspsTestConfig.Custom2PowerLimit2          = (UINT32) (CpuPowerMgmtCustomConfig->CustomConfigTdpTable[1].CustomPowerLimit2 * 125);
  FspsUpd->FspsTestConfig.Custom3PowerLimit1          = (UINT32) (CpuPowerMgmtCustomConfig->CustomConfigTdpTable[2].CustomPowerLimit1 * 125);
  FspsUpd->FspsTestConfig.Custom3PowerLimit2          = (UINT32) (CpuPowerMgmtCustomConfig->CustomConfigTdpTable[2].CustomPowerLimit2 * 125);
  
  FspsUpd->FspsTestConfig.Eist                          = (UINT8) CpuPowerMgmtTestConfig->Eist;
  FspsUpd->FspsTestConfig.EnergyEfficientPState         = (UINT8) CpuPowerMgmtTestConfig->EnergyEfficientPState;
  FspsUpd->FspsTestConfig.EnergyEfficientTurbo          = (UINT8) CpuPowerMgmtTestConfig->EnergyEfficientTurbo;
  FspsUpd->FspsTestConfig.TStates                       = (UINT8) CpuPowerMgmtTestConfig->TStates;
  FspsUpd->FspsTestConfig.BiProcHot                     = (UINT8) CpuPowerMgmtTestConfig->BiProcHot;
  FspsUpd->FspsTestConfig.DisableProcHotOut             = (UINT8) CpuPowerMgmtTestConfig->DisableProcHotOut;
  FspsUpd->FspsTestConfig.ProcHotResponse               = (UINT8) CpuPowerMgmtTestConfig->ProcHotResponse;
  FspsUpd->FspsTestConfig.DisableVrThermalAlert         = (UINT8) CpuPowerMgmtTestConfig->DisableVrThermalAlert;
  FspsUpd->FspsTestConfig.AutoThermalReporting          = (UINT8) CpuPowerMgmtTestConfig->AutoThermalReporting;
  FspsUpd->FspsTestConfig.ThermalMonitor                = (UINT8) CpuPowerMgmtTestConfig->ThermalMonitor;
  FspsUpd->FspsTestConfig.Cx                            = (UINT8) CpuPowerMgmtTestConfig->Cx;
  FspsUpd->FspsTestConfig.PmgCstCfgCtrlLock             = (UINT8) CpuPowerMgmtTestConfig->PmgCstCfgCtrlLock;
  FspsUpd->FspsTestConfig.C1e                           = (UINT8) CpuPowerMgmtTestConfig->C1e;
  FspsUpd->FspsTestConfig.PkgCStateDemotion             = (UINT8) CpuPowerMgmtTestConfig->PkgCStateDemotion;
  FspsUpd->FspsTestConfig.PkgCStateUnDemotion           = (UINT8) CpuPowerMgmtTestConfig->PkgCStateUnDemotion;
  FspsUpd->FspsTestConfig.CStatePreWake                 = (UINT8) CpuPowerMgmtTestConfig->CStatePreWake;
  FspsUpd->FspsTestConfig.TimedMwait                    = (UINT8) CpuPowerMgmtTestConfig->TimedMwait;
  FspsUpd->FspsTestConfig.CstCfgCtrIoMwaitRedirection   = (UINT8) CpuPowerMgmtTestConfig->CstCfgCtrIoMwaitRedirection;
  FspsUpd->FspsTestConfig.PkgCStateLimit                = (UINT8) CpuPowerMgmtTestConfig->PkgCStateLimit;
  FspsUpd->FspsTestConfig.CstateLatencyControl0TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl0TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl1TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl1TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl2TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl2TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl3TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl3TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl4TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl4TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl5TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl5TimeUnit;
  FspsUpd->FspsTestConfig.PpmIrmSetting                 = (UINT8) CpuPowerMgmtTestConfig->PpmIrmSetting;
  FspsUpd->FspsTestConfig.ProcHotLock                   = (UINT8) CpuPowerMgmtTestConfig->ProcHotLock;
  FspsUpd->FspsTestConfig.RaceToHalt                    = (UINT8) CpuPowerMgmtTestConfig->RaceToHalt;
  FspsUpd->FspsTestConfig.ConfigTdpLevel                = (UINT8) CpuPowerMgmtTestConfig->ConfigTdpLevel;
  FspsUpd->FspsTestConfig.CstateLatencyControl0Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl0Irtl;
  FspsUpd->FspsTestConfig.CstateLatencyControl1Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl1Irtl;
  FspsUpd->FspsTestConfig.CstateLatencyControl2Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl2Irtl;
  FspsUpd->FspsTestConfig.CstateLatencyControl3Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl3Irtl;
  FspsUpd->FspsTestConfig.CstateLatencyControl4Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl4Irtl;
  FspsUpd->FspsTestConfig.CstateLatencyControl5Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl5Irtl;

  Status = PeiServicesGetBootMode(&BootMode);
  if ((Status == EFI_SUCCESS) && (BootMode == BOOT_ON_S3_RESUME)) {
    SmramCpuData = GetSmmCpuData();
    if (SmramCpuData != NULL) {
      S3BspMtrrTablePointer = (UINT32) AllocateZeroPool(SmramCpuData->S3BspMtrrTableSize); //Add Null terminator
      FspsUpd->FspsConfig.CpuS3ResumeMtrrData = S3BspMtrrTablePointer;
      FspsUpd->FspsConfig.CpuS3ResumeMtrrDataSize = (UINT16) SmramCpuData->S3BspMtrrTableSize;
      //
      // Copy BSP MTRR table to UPD.
      //
      CopyMem((VOID*) S3BspMtrrTablePointer,
          (UINT8 *) SmramCpuData + SmramCpuData->S3BspMtrrTableOffset,
          SmramCpuData->S3BspMtrrTableSize);

      CpuS3ResumeDataPointer = (UINT32) AllocateZeroPool(SmramCpuData->CpuPrivateDataSize);
      FspsUpd->FspsTestConfig.CpuS3ResumeData = CpuS3ResumeDataPointer;
      FspsUpd->FspsTestConfig.CpuS3ResumeDataSize = (UINT16) SmramCpuData->CpuPrivateDataSize;
      //
      // Copy CPU_PRIVATE_DATA to UPD.
      //
      CopyMem((VOID*) CpuS3ResumeDataPointer,
          (UINT8 *) SmramCpuData + SmramCpuData->CpuPrivateDataOffset,
          SmramCpuData->CpuPrivateDataSize);
    } else {
      //@todo Check if an assert can be handled cleanly
      DEBUG((DEBUG_WARN, "Unable to initialize S3 data\n"));
    }

  }
  ///
  ///
  return EFI_SUCCESS;
}
