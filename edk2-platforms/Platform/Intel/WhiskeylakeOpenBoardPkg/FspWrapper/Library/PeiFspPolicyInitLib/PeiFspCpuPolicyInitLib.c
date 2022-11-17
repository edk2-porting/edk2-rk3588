/** @file
  Implementation of Fsp CPU Policy Initialization.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>

#include <Ppi/SiPolicy.h>
#include <Ppi/SecPlatformInformation2.h>

#include <CpuAccess.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PcdLib.h>
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
  ///
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Update SiCpuPolicy Pre-Mem End\n"));

  //
  // Overclocking PreMem policies
  //
  FspmUpd->FspmConfig.OcSupport               = (UINT8) CpuOverClockingPreMemConfig->OcSupport;
  FspmUpd->FspmConfig.OcLock                  = (UINT8) CpuOverClockingPreMemConfig->OcLock;
  FspmUpd->FspmConfig.CoreMaxOcRatio          = (UINT8) CpuOverClockingPreMemConfig->CoreMaxOcRatio;
  FspmUpd->FspmConfig.CoreVoltageMode         = (UINT8) CpuOverClockingPreMemConfig->CoreVoltageMode;
  FspmUpd->FspmConfig.CoreVoltageOverride     = (UINT16) CpuOverClockingPreMemConfig->CoreVoltageOverride;
  FspmUpd->FspmConfig.CoreVoltageAdaptive     = (UINT16) CpuOverClockingPreMemConfig->CoreVoltageAdaptive;
  FspmUpd->FspmConfig.CoreVoltageOffset       = (UINT16) CpuOverClockingPreMemConfig->CoreVoltageOffset;
  FspmUpd->FspmConfig.CorePllVoltageOffset    = (UINT8) CpuOverClockingPreMemConfig->CorePllVoltageOffset;
  FspmUpd->FspmConfig.RingMaxOcRatio          = (UINT8) CpuOverClockingPreMemConfig->RingMaxOcRatio;
  FspmUpd->FspmConfig.RingVoltageOverride     = (UINT16) CpuOverClockingPreMemConfig->RingVoltageOverride;
  FspmUpd->FspmConfig.RingVoltageAdaptive     = (UINT16) CpuOverClockingPreMemConfig->RingVoltageAdaptive;
  FspmUpd->FspmConfig.RingVoltageOffset       = (UINT16) CpuOverClockingPreMemConfig->RingVoltageOffset;
  FspmUpd->FspmConfig.RingPllVoltageOffset    = (UINT8) CpuOverClockingPreMemConfig->RingPllVoltageOffset;
  FspmUpd->FspmConfig.GtPllVoltageOffset      = (UINT8) CpuOverClockingPreMemConfig->GtPllVoltageOffset;
  FspmUpd->FspmConfig.RingPllVoltageOffset    = (UINT8) CpuOverClockingPreMemConfig->RingPllVoltageOffset;
  FspmUpd->FspmConfig.SaPllVoltageOffset      = (UINT8) CpuOverClockingPreMemConfig->SaPllVoltageOffset;
  FspmUpd->FspmConfig.McPllVoltageOffset      = (UINT8) CpuOverClockingPreMemConfig->McPllVoltageOffset;
  FspmUpd->FspmConfig.RingDownBin             = (UINT8) CpuOverClockingPreMemConfig->RingDownBin;
  FspmUpd->FspmConfig.RingVoltageMode         = (UINT8) CpuOverClockingPreMemConfig->RingVoltageMode;
  FspmUpd->FspmConfig.Avx2RatioOffset         = (UINT8) CpuOverClockingPreMemConfig->Avx2RatioOffset;
  FspmUpd->FspmConfig.Avx3RatioOffset         = (UINT8) CpuOverClockingPreMemConfig->Avx3RatioOffset;
  FspmUpd->FspmConfig.BclkAdaptiveVoltage     = (UINT8) CpuOverClockingPreMemConfig->BclkAdaptiveVoltage;
  FspmUpd->FspmConfig.TjMaxOffset             = (UINT8) CpuOverClockingPreMemConfig->TjMaxOffset;
  FspmUpd->FspmConfig.TvbRatioClipping        = (UINT8) CpuOverClockingPreMemConfig->TvbRatioClipping;
  FspmUpd->FspmConfig.TvbVoltageOptimization  = (UINT8) CpuOverClockingPreMemConfig->TvbVoltageOptimization;

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
  FspmUpd->FspmConfig.CpuRatio                  = (UINT8) CpuConfigLibPreMemConfig->CpuRatio;
  FspmUpd->FspmConfig.PeciSxReset               = (UINT8) CpuConfigLibPreMemConfig->PeciSxReset;
  FspmUpd->FspmConfig.PeciC10Reset              = (UINT8) CpuConfigLibPreMemConfig->PeciC10Reset;
  FspmUpd->FspmConfig.SkipMpInit                = (UINT8) CpuConfigLibPreMemConfig->SkipMpInit;
  FspmUpd->FspmConfig.DpSscMarginEnable         = (UINT8) CpuConfigLibPreMemConfig->DpSscMarginEnable;

  //
  // DisableMtrrProgram <1> Disable Mtrrs program. <0> Program Mtrrs in FSP
  //
  FspmUpd->FspmConfig.DisableMtrrProgram        = (UINT8) 0;

  return EFI_SUCCESS;
}

/**
 This routine is used to get Sec Platform Information Record2 Pointer.

 @param[in] PeiServices    Pointer to the PEI services table

 @retval GetSecPlatformInformation2 - The pointer of Sec Platform Information Record2 Pointer.
 **/

EFI_SEC_PLATFORM_INFORMATION_RECORD2 * GetSecPlatformInformation2(
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_SEC_PLATFORM_INFORMATION2_PPI    *SecPlatformInformation2Ppi;
  EFI_SEC_PLATFORM_INFORMATION_RECORD2 *SecPlatformInformation2 = NULL;
  UINT64                               InformationSize;
  EFI_STATUS Status;

  //
  // Get BIST information from Sec Platform Information2 Ppi firstly
  //
  Status = PeiServicesLocatePpi (
             &gEfiSecPlatformInformation2PpiGuid,   // GUID
             0,                                     // Instance
             NULL,                                  // EFI_PEI_PPI_DESCRIPTOR
             (VOID ** ) &SecPlatformInformation2Ppi // PPI
             );

  DEBUG((DEBUG_INFO, "LocatePpi SecPlatformInformationPpi2 Status - %x\n", Status));
  if (EFI_ERROR(Status)) {
    return NULL;
  }

  InformationSize = 0;

  Status = SecPlatformInformation2Ppi->PlatformInformation2 (
                                         (CONST EFI_PEI_SERVICES  **) PeiServices,
                                         &InformationSize,
                                         SecPlatformInformation2
                                         );

  ASSERT (Status == EFI_BUFFER_TOO_SMALL);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return NULL;
  }

  SecPlatformInformation2 = AllocatePool((UINTN)InformationSize);
  ASSERT (SecPlatformInformation2 != NULL);
  if (SecPlatformInformation2 == NULL) {
    return NULL;
  }

  //
  // Retrieve BIST data from SecPlatform2
  //
  Status = SecPlatformInformation2Ppi->PlatformInformation2 (
                                         (CONST EFI_PEI_SERVICES  **) PeiServices,
                                         &InformationSize,
                                         SecPlatformInformation2
                                         );
  DEBUG((DEBUG_INFO, "SecPlatformInformation2Ppi->PlatformInformation2 Status - %x\n", Status));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return SecPlatformInformation2;
}

/**
 This routine is used to get Sec Platform Information Record Pointer.

 @param[in] PeiServices    Pointer to the PEI services table

 @retval GetSecPlatformInformation2 - The pointer of Sec Platform Information Record Pointer.
 **/
EFI_SEC_PLATFORM_INFORMATION_RECORD2 * GetSecPlatformInformationInfoInFormat2(
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_SEC_PLATFORM_INFORMATION_PPI     *SecPlatformInformationPpi;
  EFI_SEC_PLATFORM_INFORMATION_RECORD  *SecPlatformInformation = NULL;
  EFI_SEC_PLATFORM_INFORMATION_RECORD2 *SecPlatformInformation2;
  UINT64                               InformationSize;
  EFI_STATUS                           Status;

  //
  // Get BIST information from Sec Platform Information
  //
  Status = PeiServicesLocatePpi (
             &gEfiSecPlatformInformationPpiGuid,    // GUID
             0,                                     // Instance
             NULL,                                  // EFI_PEI_PPI_DESCRIPTOR
             (VOID ** ) &SecPlatformInformationPpi  // PPI
             );

  DEBUG((DEBUG_INFO, "LocatePpi SecPlatformInformationPpi Status - %x\n", Status));
  if (EFI_ERROR(Status)) {
    return NULL;
  }

  InformationSize = 0;
  Status = SecPlatformInformationPpi->PlatformInformation (
                                        (CONST EFI_PEI_SERVICES  **) PeiServices,
                                        &InformationSize,
                                        SecPlatformInformation
                                        );

  ASSERT (Status == EFI_BUFFER_TOO_SMALL);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return NULL;
  }

  SecPlatformInformation = AllocatePool((UINTN)InformationSize);
  ASSERT (SecPlatformInformation != NULL);
  if (SecPlatformInformation == NULL) {
    return NULL;
  }

  //
  // Retrieve BIST data from SecPlatform
  //
  Status = SecPlatformInformationPpi->PlatformInformation (
                                        (CONST EFI_PEI_SERVICES  **) PeiServices,
                                        &InformationSize,
                                        SecPlatformInformation
                                        );
  DEBUG((DEBUG_INFO, "FSP  SecPlatformInformation2Ppi->PlatformInformation Status - %x\n", Status));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  SecPlatformInformation2 = AllocatePool(sizeof (EFI_SEC_PLATFORM_INFORMATION_RECORD2));
  ASSERT (SecPlatformInformation2 != NULL);
  if (SecPlatformInformation2 == NULL) {
    return NULL;
  }

  SecPlatformInformation2->NumberOfCpus = 1;
  SecPlatformInformation2->CpuInstance[0].CpuLocation = 0;
  SecPlatformInformation2->CpuInstance[0].InfoRecord.x64HealthFlags.Uint32 = SecPlatformInformation->x64HealthFlags.Uint32;

  FreePool(SecPlatformInformation);

  return SecPlatformInformation2;
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
  EFI_STATUS                           Status;
  SI_POLICY_PPI                        *SiPolicyPpi;
  CPU_CONFIG                           *CpuConfig;
  CPU_POWER_MGMT_BASIC_CONFIG          *CpuPowerMgmtBasicConfig;
  CPU_POWER_MGMT_CUSTOM_CONFIG         *CpuPowerMgmtCustomConfig;
  CPU_TEST_CONFIG                      *CpuTestConfig;
  CPU_POWER_MGMT_TEST_CONFIG           *CpuPowerMgmtTestConfig;
  UINTN                                Index;
  EFI_SEC_PLATFORM_INFORMATION_RECORD2 *SecPlatformInformation2;
  EFI_PEI_SERVICES                     **PeiServices;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Update SiCpuPolicy\n"));
  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();
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
  ///
  ///Production RC Policies
  ///

  FspsUpd->FspsConfig.AesEnable                 = (UINT8) CpuConfig->AesEnable;
  FspsUpd->FspsConfig.DebugInterfaceEnable      = (UINT8) CpuConfig->DebugInterfaceEnable;

  FspsUpd->FspsConfig.TurboMode                 = (UINT8) CpuPowerMgmtBasicConfig->TurboMode;

  ///
  /// Test RC Policies
  ///
  FspsUpd->FspsTestConfig.MlcStreamerPrefetcher      = (UINT8) CpuTestConfig->MlcStreamerPrefetcher;
  FspsUpd->FspsTestConfig.MlcSpatialPrefetcher       = (UINT8) CpuTestConfig->MlcSpatialPrefetcher;
  FspsUpd->FspsTestConfig.MonitorMwaitEnable         = (UINT8) CpuTestConfig->MonitorMwaitEnable;
  FspsUpd->FspsTestConfig.DebugInterfaceLockEnable   = (UINT8) CpuTestConfig->DebugInterfaceLockEnable;
  FspsUpd->FspsTestConfig.ApIdleManner               = PcdGet8 (PcdCpuApLoopMode);
  FspsUpd->FspsTestConfig.ProcessorTraceOutputScheme = (UINT8) CpuTestConfig->ProcessorTraceOutputScheme;
  FspsUpd->FspsTestConfig.ProcessorTraceEnable       = (UINT8) CpuTestConfig->ProcessorTraceEnable;
  FspsUpd->FspsTestConfig.ProcessorTraceMemBase      = CpuTestConfig->ProcessorTraceMemBase;
  FspsUpd->FspsTestConfig.ProcessorTraceMemLength    = (UINT32) CpuTestConfig->ProcessorTraceMemLength;
  FspsUpd->FspsTestConfig.VoltageOptimization        = (UINT8) CpuTestConfig->VoltageOptimization;
  FspsUpd->FspsTestConfig.ThreeStrikeCounterDisable  = (UINT8) CpuTestConfig->ThreeStrikeCounterDisable;
  FspsUpd->FspsTestConfig.MachineCheckEnable         = (UINT8) CpuTestConfig->MachineCheckEnable;
  FspsUpd->FspsTestConfig.CpuWakeUpTimer             = (UINT8) CpuTestConfig->CpuWakeUpTimer;

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
  FspsUpd->FspsTestConfig.HwpInterruptControl        = (UINT8) CpuPowerMgmtBasicConfig->HwpInterruptControl;
  FspsUpd->FspsTestConfig.EnableItbm                 = (UINT8) CpuPowerMgmtBasicConfig->EnableItbm;
  FspsUpd->FspsTestConfig.EnableItbmDriver           = (UINT8) CpuPowerMgmtBasicConfig->EnableItbmDriver;
  FspsUpd->FspsTestConfig.MinRingRatioLimit          = (UINT8) CpuPowerMgmtBasicConfig->MinRingRatioLimit;
  FspsUpd->FspsTestConfig.MaxRingRatioLimit          = (UINT8) CpuPowerMgmtBasicConfig->MaxRingRatioLimit;
  FspsUpd->FspsTestConfig.NumberOfEntries             = (UINT8) CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries;
  FspsUpd->FspsTestConfig.Custom1PowerLimit1Time      = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[0].CustomPowerLimit1Time;
  FspsUpd->FspsTestConfig.Custom2PowerLimit1Time      = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[1].CustomPowerLimit1Time;
  FspsUpd->FspsTestConfig.Custom3PowerLimit1Time      = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[2].CustomPowerLimit1Time;
  FspsUpd->FspsTestConfig.Custom1TurboActivationRatio = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[0].CustomTurboActivationRatio;
  FspsUpd->FspsTestConfig.Custom2TurboActivationRatio = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[1].CustomTurboActivationRatio;
  FspsUpd->FspsTestConfig.Custom3TurboActivationRatio = (UINT8) CpuPowerMgmtCustomConfig->CustomConfigTdpTable[2].CustomTurboActivationRatio;
  FspsUpd->FspsTestConfig.ConfigTdpLock               = (UINT8) CpuPowerMgmtCustomConfig->ConfigTdpLock;
  FspsUpd->FspsTestConfig.ConfigTdpBios               = (UINT8) CpuPowerMgmtCustomConfig->ConfigTdpBios;
  FspsUpd->FspsTestConfig.MaxRatio                    = (UINT8) CpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio;
  for (Index = 0; Index < CpuPowerMgmtCustomConfig->CustomRatioTable.NumberOfEntries; Index++) {
    FspsUpd->FspsTestConfig.StateRatio[Index]         = (UINT8) CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatio[Index];
  }
  for (Index = 0; Index < MAX_16_CUSTOM_RATIO_TABLE_ENTRIES; Index++) {
    FspsUpd->FspsTestConfig.StateRatioMax16[Index]    = (UINT8) CpuPowerMgmtCustomConfig->CustomRatioTable.StateRatioMax16[Index];
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
  FspsUpd->FspsTestConfig.C1StateAutoDemotion           = (UINT8) CpuPowerMgmtTestConfig->C1AutoDemotion;
  FspsUpd->FspsTestConfig.C1StateUnDemotion             = (UINT8) CpuPowerMgmtTestConfig->C1UnDemotion;
  FspsUpd->FspsTestConfig.C3StateAutoDemotion           = (UINT8) CpuPowerMgmtTestConfig->C3AutoDemotion;
  FspsUpd->FspsTestConfig.C3StateUnDemotion             = (UINT8) CpuPowerMgmtTestConfig->C3UnDemotion;
  FspsUpd->FspsTestConfig.CstateLatencyControl0TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl0TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl0Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl0Irtl;
  FspsUpd->FspsTestConfig.PkgCStateDemotion             = (UINT8) CpuPowerMgmtTestConfig->PkgCStateDemotion;
  FspsUpd->FspsTestConfig.PkgCStateUnDemotion           = (UINT8) CpuPowerMgmtTestConfig->PkgCStateUnDemotion;
  FspsUpd->FspsTestConfig.CStatePreWake                 = (UINT8) CpuPowerMgmtTestConfig->CStatePreWake;
  FspsUpd->FspsTestConfig.TimedMwait                    = (UINT8) CpuPowerMgmtTestConfig->TimedMwait;
  FspsUpd->FspsTestConfig.CstCfgCtrIoMwaitRedirection   = (UINT8) CpuPowerMgmtTestConfig->CstCfgCtrIoMwaitRedirection;
  FspsUpd->FspsTestConfig.PkgCStateLimit                = (UINT8) CpuPowerMgmtTestConfig->PkgCStateLimit;
  FspsUpd->FspsTestConfig.CstateLatencyControl1TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl1TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl2TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl2TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl3TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl3TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl4TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl4TimeUnit;
  FspsUpd->FspsTestConfig.CstateLatencyControl5TimeUnit = (UINT8) CpuPowerMgmtTestConfig->CstateLatencyControl5TimeUnit;
  FspsUpd->FspsTestConfig.PpmIrmSetting                 = (UINT8) CpuPowerMgmtTestConfig->PpmIrmSetting;
  FspsUpd->FspsTestConfig.ProcHotLock                   = (UINT8) CpuPowerMgmtTestConfig->ProcHotLock;
  FspsUpd->FspsTestConfig.RaceToHalt                    = (UINT8) CpuPowerMgmtTestConfig->RaceToHalt;
  FspsUpd->FspsTestConfig.ConfigTdpLevel                = (UINT8) CpuPowerMgmtTestConfig->ConfigTdpLevel;
  FspsUpd->FspsTestConfig.CstateLatencyControl1Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl1Irtl;
  FspsUpd->FspsTestConfig.CstateLatencyControl2Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl2Irtl;
  FspsUpd->FspsTestConfig.CstateLatencyControl3Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl3Irtl;
  FspsUpd->FspsTestConfig.CstateLatencyControl4Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl4Irtl;
  FspsUpd->FspsTestConfig.CstateLatencyControl5Irtl     = (UINT16) CpuPowerMgmtTestConfig->CstateLatencyControl5Irtl;

  //
  // Get BIST information from Sec Platform Information
  //
  SecPlatformInformation2 = GetSecPlatformInformation2 (PeiServices);
  if (SecPlatformInformation2 == NULL) {
    SecPlatformInformation2 = GetSecPlatformInformationInfoInFormat2 (PeiServices);
  }

  ASSERT (SecPlatformInformation2 != NULL);

  if (SecPlatformInformation2 != NULL) {
    FspsUpd->FspsConfig.CpuBistData = (UINT32)SecPlatformInformation2;
    DEBUG((DEBUG_INFO, "SecPlatformInformation NumberOfCpus - %x\n", SecPlatformInformation2->NumberOfCpus));
    DEBUG ((DEBUG_INFO, "SecPlatformInformation BIST - %x\n", SecPlatformInformation2->CpuInstance[0].InfoRecord.x64HealthFlags.Uint32));
  }
  return EFI_SUCCESS;
}

