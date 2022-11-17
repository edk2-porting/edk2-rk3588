/** @file

Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Protocol/IntelCpuPcdsSetDone.h>
#include <Setup/IioUniversalData.h>
#include <Guid/SetupVariable.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/CpuConfigLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/CpuPpmLib.h>
#include <SetupTable.h>
#include <Platform.h>
#include "PlatformHost.h"
#include <Register/Cpuid.h>

CHAR16 mCpuSocketStr[8][5] = {L"CPU0", L"CPU1", L"CPU2", L"CPU3", L"CPU4", L"CPU5", L"CPU6", L"CPU7"};
CHAR16 mCpuAssetTagStr[] = L"UNKNOWN";
IIO_UDS                     *mIioUds;

/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
CheckAndReAssignSocketId(
  VOID
  )
{
#define APICID_MASK_BIT14_8 0x7F    //current Si support programmable APICID up to 15bits
  CPU_SOCKET_ID_INFO    *pcdSktIdPtr;
  UINT32                i, IntraPackageIdBits;
  UINTN                 PcdSize;
  EFI_STATUS            Status;
  UINT32                MaxSocketCount;

  MaxSocketCount = FixedPcdGet32(PcdMaxCpuSocketCount);
  DEBUG ((EFI_D_ERROR, "::SocketCount %08x\n", MaxSocketCount));
  pcdSktIdPtr = (CPU_SOCKET_ID_INFO *)PcdGetPtr(PcdCpuSocketId);
  PcdSize = PcdGetSize (PcdCpuSocketId); //MAX_SOCKET * sizeof(CPU_SOCKET_ID_INFO);
  ASSERT(PcdSize == (MAX_SOCKET * sizeof(CPU_SOCKET_ID_INFO)));
  Status = PcdSetPtrS (PcdCpuSocketId, &PcdSize, (VOID *)pcdSktIdPtr);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) return;
  DEBUG ((EFI_D_INFO, "::SockeId Pcd at %08x, size %x\n", PcdGetPtr(PcdCpuSocketId), PcdSize));

  for(i = 0; i < MAX_SOCKET; i++) {
    if(mIioUds->PlatformData.CpuQpiInfo[i].Valid) {
      pcdSktIdPtr[i].DefaultSocketId = mIioUds->PlatformData.CpuQpiInfo[i].SocId;
      pcdSktIdPtr[i].NewSocketId     = mIioUds->PlatformData.CpuQpiInfo[i].SocId;
    } else {
      pcdSktIdPtr[i].DefaultSocketId = (UINT32)-1;   //make sure Default and New are same
      pcdSktIdPtr[i].NewSocketId     = (UINT32)-1;
    }
  }

  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, &IntraPackageIdBits, NULL, NULL, NULL);
  //assign new socketId
  for(i = 0; i < MAX_SOCKET; i++) {

    if(pcdSktIdPtr[i].DefaultSocketId == (UINT32)-1) continue;

    switch(IntraPackageIdBits) {
      case 4: //socket bit starts from bit4 of ApicId
      case 5: //socket bit starts from bit5 of ApicId
        if(MAX_SOCKET == 4) {
          pcdSktIdPtr[i].NewSocketId |= (APICID_MASK_BIT14_8 << (8 - IntraPackageIdBits));
        } else {
          //3bit in lower 8bit as skt field, to avoid ApicID= FFs, leave bit8 untouched for 8S
          pcdSktIdPtr[i].NewSocketId |= (0x7E << (8 - IntraPackageIdBits));      //leave bit8 to 0 so we don't have FFs in ApicId
        }
        break;

     case 6: //socket bit starts from bit6 of ApicId
        if(MAX_SOCKET == 4) {
          //only 2bit in lower 8bit as skt field, to avoid ApicID= FFs, leave bit8 untouched for 4S
          pcdSktIdPtr[i].NewSocketId |= (0x7E << (8 - IntraPackageIdBits));
        } else {
          //only 2bit in lower 8bit as skt field, to avoid ApicID= FFs, leave bit9 untouched for 8S
          pcdSktIdPtr[i].NewSocketId |= (0x7C << (8 - IntraPackageIdBits));
        }
        break;

     default:
        DEBUG ((EFI_D_INFO, "::Need more info to make sure we can support!!!\n"));
        break;

    } //end switch
  }
}


/**

  This is the EFI driver entry point for the CpuPolicy Driver. This
  driver is responsible for getting microcode patches from FV.

  @param ImageHandle     - Handle for the image of this driver.
  @param SystemTable     - Pointer to the EFI System Table.

  @retval EFI_SUCCESS     - Protocol installed sucessfully.

**/
EFI_STATUS
EFIAPI
PlatformCpuPolicyEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                       Status;
  SETUP_DATA                       SetupData;
  UINT32                           CpuPolicy;
  UINT32                           CpuPolicyEx1;
  EFI_HANDLE                       Handle;
  UINT32                           CsrSapmCtl = 0;
  UINT32                           CsrPerfPlimitCtl = 0;
  UINT8                            PCPSOptions = 0;
  UINT32                           AdvPwrMgtCtl;
  UINT8                            socket;
  UINT32                           *UpiInPkgCEntry = NULL;
  UINT32                           *PcieInPkgCEntry = NULL;
  UINT32                           MsrPowerCtlLow  = 0;
  UINT32                           MsrTurboPowerLimitHigh = 0;
  UINT32                           MsrTurboPowerLimitLow = 0;
  UINT32                           MsrPriPlaneCurrentCfgCtlHigh = 0;
  UINT32                           MsrPriPlaneCurrentCfgCtlLow = 0;
  UINT32                           CsrDynamicPerfPowerCtl = 0;
  UINT32                           CsrPcieIltrOvrd = 0;
  UINT32                           MsrPerfBiasConfig = 0;
  MSR_REGISTER                     *CStateLatencyCtrl = NULL;
  UINT32                           CpuFamilyModelStepping;
  UINT64                           i;
  UINT64                           *Addr;
  EFI_PPM_STRUCT                   *ppm = NULL;
  XE_STRUCT                        *XePtr = NULL;
  TURBO_RATIO_LIMIT_RATIO_CORES    *TurboRatioLimitRatioCores = NULL;
  UINT8                            PackageCStateSetting = 0;
  UINT8                            CpuCStateValue = 0;

  EFI_GUID                         UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_HOB_GUID_TYPE                *GuidHob;

  GuidHob   = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT (GuidHob != NULL);
  if(GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  mIioUds   = GET_GUID_HOB_DATA(GuidHob);

  AsmCpuid (1, &CpuFamilyModelStepping, NULL, NULL, NULL);

  Status = gBS->AllocatePool (
                        EfiBootServicesData,
                        sizeof(EFI_PPM_STRUCT),
                        (VOID **) &Addr
                        );
  if(Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_INFO, "::Failed to allocate mem for PPM Struct\n"));
    ASSERT_EFI_ERROR (Status);      //may need to create a default
  } else {
    ZeroMem(Addr, sizeof(EFI_PPM_STRUCT));
    i = (UINT32)(*(UINT64 *)(&Addr));
    ppm = (EFI_PPM_STRUCT *)(Addr);
    Status = PcdSet64S (PcdCpuPmStructAddr, i);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;
    DEBUG ((EFI_D_INFO, "::PPM mem allocate @ %x %X %X\n", i, PcdGet64(PcdCpuPmStructAddr), ppm));
    UpiInPkgCEntry = (UINT32 *)(((EFI_PPM_STRUCT *)Addr)->Cst.PkgCstEntryCriteriaMaskKti);
    PcieInPkgCEntry = (UINT32 *)(((EFI_PPM_STRUCT *)Addr)->Cst.PkgCstEntryCriteriaMaskPcie);
    XePtr = (XE_STRUCT *)(&((EFI_PPM_STRUCT *)Addr)->Xe);
    TurboRatioLimitRatioCores = (TURBO_RATIO_LIMIT_RATIO_CORES *)(&((EFI_PPM_STRUCT *)Addr)->TurboRatioLimitRatioCores);
    DEBUG ((EFI_D_INFO, ":: XE @ %X\n", (UINTN) XePtr));

    CStateLatencyCtrl = (MSR_REGISTER *)(ppm->Cst.LatencyCtrl);
    DEBUG ((EFI_D_INFO, "CStateLatencyCtrl[%X]\n", (UINTN) CStateLatencyCtrl));
  }

  //
  // Read the current system configuration variable store.
  //
  ZeroMem (&SetupData, sizeof(SETUP_DATA));
  CopyMem (&SetupData.SocketConfig.IioConfig, PcdGetPtr(PcdSocketIioConfigData), sizeof(SOCKET_IIO_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.CommonRcConfig, PcdGetPtr(PcdSocketCommonRcConfigData), sizeof(SOCKET_COMMONRC_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.CsiConfig, PcdGetPtr(PcdSocketMpLinkConfigData), sizeof(SOCKET_MP_LINK_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.MemoryConfig, PcdGetPtr(PcdSocketMemoryConfigData), sizeof(SOCKET_MEMORY_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.PowerManagementConfig, PcdGetPtr(PcdSocketPowerManagementConfigData), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.SocketProcessorCoreConfiguration, PcdGetPtr(PcdSocketProcessorCoreConfigData), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
  CopyMem (&SetupData.SystemConfig, PcdGetPtr(PcdSetupData), sizeof(SYSTEM_CONFIGURATION));
  CopyMem (&SetupData.PchRcConfig, PcdGetPtr(PcdPchRcConfigurationData), sizeof(PCH_RC_CONFIGURATION));

  {

	if (SetupData.SocketConfig.PowerManagementConfig.PackageCState == PPM_AUTO) {
        PackageCStateSetting = 3;   //POR Default = C6
    } else {
        PackageCStateSetting = SetupData.SocketConfig.PowerManagementConfig.PackageCState;
    }

    // Temporary override to prevent accidental enabling until CR dungeon approves
    if (SetupData.SocketConfig.PowerManagementConfig.PackageCState != 0) {
      DEBUG((EFI_D_ERROR, "Crystal Ridge Configuration Warning: Package c-states are not disabled\n"));
    }

    if ((SetupData.SocketConfig.PowerManagementConfig.C6Enable == PPM_AUTO) ||
          SetupData.SocketConfig.PowerManagementConfig.ProcessorAutonomousCstateEnable) {
        CpuCStateValue |= C6_ENABLE;   //POR Default = Enabled
    } else {
        CpuCStateValue |= (SetupData.SocketConfig.PowerManagementConfig.C6Enable * C6_ENABLE);
    }

    Status = PcdSet8S (PcdCpuCoreCStateValue, CpuCStateValue);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;

    //
    // If ACC enabled, then override C1E to be enabled
    //
    if (SetupData.SocketConfig.PowerManagementConfig.ProcessorAutonomousCstateEnable) {
      SetupData.SocketConfig.PowerManagementConfig.ProcessorC1eEnable = TRUE;
    }

    //
    // Verify that the value being set is within the valid range 0 to MAX_SOCKET - 1
    //
    if (SetupData.SocketConfig.SocketProcessorCoreConfiguration.BspSelection > MAX_SOCKET)
      SetupData.SocketConfig.SocketProcessorCoreConfiguration.BspSelection= 0xFF;
    Status = PcdSet8S (PcdSbspSelection, SetupData.SocketConfig.SocketProcessorCoreConfiguration.BspSelection);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;
    //
    // Map CPU setup options to PcdCpuProcessorFeatureUserConfiguration
    //
    CpuPolicy = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorHyperThreadingDisable ? 0 : PCD_CPU_HT_BIT) |
                (SetupData.SocketConfig.PowerManagementConfig.ProcessorEistEnable ? PCD_CPU_EIST_BIT : 0) |
                (SetupData.SocketConfig.PowerManagementConfig.ProcessorC1eEnable ? PCD_CPU_C1E_BIT : 0) |
                (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorVmxEnable ? PCD_CPU_VT_BIT : 0) |
                (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorSmxEnable ? PCD_CPU_LT_BIT : 0) |
                (SetupData.SocketConfig.SocketProcessorCoreConfiguration.FastStringEnable ? PCD_CPU_FAST_STRING_BIT : 0) |
                (SetupData.SocketConfig.SocketProcessorCoreConfiguration.CpuidMaxValue ? PCD_CPU_MAX_CPUID_VALUE_LIMIT_BIT : 0) |
                (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ExecuteDisableBit ? PCD_CPU_EXECUTE_DISABLE_BIT : 0) |
                (SetupData.SocketConfig.SocketProcessorCoreConfiguration.MachineCheckEnable ? PCD_CPU_MACHINE_CHECK_BIT : 0) |
                (SetupData.SocketConfig.SocketProcessorCoreConfiguration.DCUStreamerPrefetcherEnable ? PCD_CPU_DCU_PREFETCHER_BIT : 0) |
                (SetupData.SocketConfig.SocketProcessorCoreConfiguration.DCUIPPrefetcherEnable ? PCD_CPU_IP_PREFETCHER_BIT : 0) |
                PCD_CPU_MONITOR_MWAIT_BIT | //never disable Mwait
                (SetupData.SocketConfig.PowerManagementConfig.TurboMode ? PCD_CPU_TURBO_MODE_BIT : 0) |
                (SetupData.SocketConfig.PowerManagementConfig.EnableThermalMonitor ? PCD_CPU_THERMAL_MANAGEMENT_BIT : 0);

    if (SetupData.SocketConfig.PowerManagementConfig.TStateEnable && (SetupData.SocketConfig.PowerManagementConfig.OnDieThermalThrottling > 0)) {
       CpuPolicy |= (SetupData.SocketConfig.PowerManagementConfig.TStateEnable ? PCD_CPU_TSTATE_BIT : 0);
    }

    CpuPolicyEx1 = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.MlcStreamerPrefetcherEnable ? PCD_CPU_MLC_STREAMER_PREFETCHER_BIT : 0) |
                   (SetupData.SocketConfig.SocketProcessorCoreConfiguration.MlcSpatialPrefetcherEnable ? PCD_CPU_MLC_SPATIAL_PREFETCHER_BIT : 0) |
                   (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ThreeStrikeTimer ? PCD_CPU_THREE_STRIKE_COUNTER_BIT : 0) |
                   PCD_CPU_ENERGY_PERFORMANCE_BIAS_BIT |
                   (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorX2apic ? PCD_CPU_X2APIC_BIT : 0) |
                   (SetupData.SocketConfig.SocketProcessorCoreConfiguration.AesEnable ? PCD_CPU_AES_BIT : 0) |
                   (SetupData.SocketConfig.SocketProcessorCoreConfiguration.PCIeDownStreamPECIWrite ? PCD_CPU_PECI_DOWNSTREAM_WRITE_BIT : 0) |
                   PCD_CPU_C_STATE_BIT;


    PCPSOptions =  (SetupData.SocketConfig.PowerManagementConfig.ProcessorSinglePCTLEn ? PCD_CPU_PCPS_SINGLEPCTL : 0) |
                   (SetupData.SocketConfig.PowerManagementConfig.ProcessorSPD ? PCD_CPU_PCPS_SPD : 0) |
                   (SetupData.SocketConfig.PowerManagementConfig.PStateDomain ? PCD_CPU_PCPS_PSTATEDOMAIN : 0) |
                   (UINT8) SetupData.SocketConfig.PowerManagementConfig.ProcessorEistPsdFunc;

    ppm->Pst.PcpsCtrl = PCPSOptions;
    ppm->OverclockingLock = SetupData.SocketConfig.PowerManagementConfig.OverclockingLock;

    ppm->FastRaplDutyCycle = SetupData.SocketConfig.PowerManagementConfig.FastRaplDutyCycle;

    if(mIioUds->PlatformData.EVMode)
      CpuPolicy &= ~PCD_CPU_LT_BIT;

    if (SetupData.SocketConfig.PowerManagementConfig.ProcessorEistEnable) {
      Status = PcdSetBoolS (PcdCpuHwCoordination, SetupData.SocketConfig.PowerManagementConfig.ProcessorEistPsdFunc ? FALSE : TRUE);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) return Status;
    }

    Status = PcdSet16S (PcdCpuAcpiLvl2Addr, PcdGet16 (PcdPchAcpiIoPortBaseAddress) + R_ACPI_LV2);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;
    Status = PcdSet8S (PcdCpuPackageCStateLimit, PackageCStateSetting);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;

    if ((SetupData.SocketConfig.PowerManagementConfig.TStateEnable) && (SetupData.SocketConfig.PowerManagementConfig.OnDieThermalThrottling > 0)) {
      Status = PcdSet8S (PcdCpuClockModulationDutyCycle, SetupData.SocketConfig.PowerManagementConfig.OnDieThermalThrottling);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) return Status;
      ppm->TCCActivationOffset = SetupData.SocketConfig.PowerManagementConfig.TCCActivationOffset;
    }
    Status = PcdSet8S (PcdCpuDcuMode, SetupData.SocketConfig.SocketProcessorCoreConfiguration.DCUModeSelection);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;

    if((CpuFamilyModelStepping >> 4) == CPU_FAMILY_SKX) {
      Status = PcdSetBoolS (PcdCpuSmmRuntimeCtlHooks, TRUE);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) return Status;
    } else {
      Status = PcdSetBoolS (PcdCpuSmmRuntimeCtlHooks, FALSE);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) return Status;
    }
    DEBUG ((EFI_D_INFO, ":: PcdCpuSmmRuntimeCtlHooks= %x\n", PcdGetBool(PcdCpuSmmRuntimeCtlHooks)));

    if(mIioUds->PlatformData.EVMode || SetupData.SystemConfig.LmceEn) {
      Status = PcdSet8S (PcdCpuProcessorMsrLockCtrl, 0);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) return Status;
    } else {
      Status = PcdSet8S (PcdCpuProcessorMsrLockCtrl, SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorMsrLockControl);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) return Status;
    }

    Status = PcdSet64S(PcdCpuIioLlcWaysBitMask, SetupData.SocketConfig.SocketProcessorCoreConfiguration.IioLlcWaysMask);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;
    Status = PcdSet64S(PcdCpuExpandedIioLlcWaysBitMask, SetupData.SocketConfig.SocketProcessorCoreConfiguration.ExpandedIioLlcWaysMask);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;
    Status = PcdSet64S(PcdCpuRemoteWaysBitMask, SetupData.SocketConfig.SocketProcessorCoreConfiguration.RemoteWaysMask);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;
    Status = PcdSet8S(PcdPchTraceHubEn, SetupData.SocketConfig.SocketProcessorCoreConfiguration.PchTraceHubEn);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;
    Status = PcdSet64S(PcdCpuQlruCfgBitMask, ((UINT64) SetupData.SocketConfig.SocketProcessorCoreConfiguration.QlruCfgMask_Hi << 32) | (UINT64)SetupData.SocketConfig.SocketProcessorCoreConfiguration.QlruCfgMask_Lo );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;
    Status = PcdSet64S(PcdCpuRRQCountThreshold, mIioUds->PlatformData.RemoteRequestThreshold);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;

    //CSR SAPM CTL
    CsrSapmCtl = 0;

   for( socket = 0; socket < MAX_SOCKET; socket++) {
      CsrSapmCtl = (( SetupData.SocketConfig.PowerManagementConfig.Iio0PkgcClkGateDis[socket] << IIO012_PKGC_CLK_GATE_DISABLE_SHIFT) |
                   ( SetupData.SocketConfig.PowerManagementConfig.Iio1PkgcClkGateDis[socket] << (IIO012_PKGC_CLK_GATE_DISABLE_SHIFT + 1)) |
                   ( SetupData.SocketConfig.PowerManagementConfig.Iio2PkgcClkGateDis[socket] << (IIO012_PKGC_CLK_GATE_DISABLE_SHIFT + 2)) );

      CsrSapmCtl |= (( SetupData.SocketConfig.PowerManagementConfig.Kti23PkgcClkGateDis[socket] << KTI23_PKGC_CLK_GATE_DISABLE_SHIFT ) |
                 ( SetupData.SocketConfig.PowerManagementConfig.Kti01PkgcClkGateDis[socket] << KTI01_PKGC_CLK_GATE_DISABLE_SHIFT ) |
                 ( SetupData.SocketConfig.PowerManagementConfig.Kti01pllOffEna[socket] << KTI_PLL_OFF_EN_SHIFT) |
                 ( SetupData.SocketConfig.PowerManagementConfig.Kti23pllOffEna[socket] << (KTI_PLL_OFF_EN_SHIFT + 1) ) );

        CsrSapmCtl |= (( SetupData.SocketConfig.PowerManagementConfig.Mc1PkgcClkGateDis[socket] << MC1_PKGC_CLK_GATE_DISABLE_SHIFT ) |
                   ( SetupData.SocketConfig.PowerManagementConfig.Mc0PkgcClkGateDis[socket] << MC0_PKGC_CLK_GATE_DISABLE_SHIFT ) |
                   ( SetupData.SocketConfig.PowerManagementConfig.Mc0pllOffEna[socket] << MEM_PLL_OFF_EN_SHIFT) |
                   ( SetupData.SocketConfig.PowerManagementConfig.Mc1pllOffEna[socket] << (MEM_PLL_OFF_EN_SHIFT + 1) ));

      if (SetupData.SocketConfig.MemoryConfig.OppSrefEn == 1) {
        CsrSapmCtl |= ((1 << MC0_PKGC_IO_VOLTAGE_REDUCTION_DISABLE_SHIFT) | (1 << MC1_PKGC_IO_VOLTAGE_REDUCTION_DISABLE_SHIFT) |
                      (1 << MC0_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE_SHIFT) | (1 << MC1_PKGC_DIG_VOLTAGE_REDUCTION_DISABLE_SHIFT)) ;
      }

      CsrSapmCtl |= (( SetupData.SocketConfig.PowerManagementConfig.P0pllOffEna[socket] << IIO_PLL_OFF_EN_SHIFT) |
                 ( SetupData.SocketConfig.PowerManagementConfig.P1pllOffEna[socket] << (IIO_PLL_OFF_EN_SHIFT + 1) ) |
                 ( SetupData.SocketConfig.PowerManagementConfig.P2pllOffEna[socket] << (IIO_PLL_OFF_EN_SHIFT + 2) ) |
                 ( SetupData.SocketConfig.PowerManagementConfig.SetvidDecayDisable[socket] << SETVID_DECAY_DISABLE_SHIFT ) |
                 ( SetupData.SocketConfig.PowerManagementConfig.SapmCtlLock[socket] << SAPMCTL_LOCK_SHIFT) );

      ppm->SapmCtl[socket]  = CsrSapmCtl;
   }

    ppm->PmaxConfig  = (SetupData.SocketConfig.PowerManagementConfig.UsePmaxOffsetTable ? USER_PMAX_USE_OFFSET_TABLE : 0 ) |
                       SetupData.SocketConfig.PowerManagementConfig.PmaxOffset |
                      (SetupData.SocketConfig.PowerManagementConfig.PmaxSign ? USER_PMAX_NEGATIVE_BIT : 0);

    CsrPerfPlimitCtl = ( SetupData.SocketConfig.PowerManagementConfig.PerfPLmtThshld << PERF_PLIMIT_THRESHOLD_SHIFT ) |
                      ( SetupData.SocketConfig.PowerManagementConfig.PerfPLimitClipC << PERF_PLIMIT_CLIP_SHIFT ) |
                      ( SetupData.SocketConfig.PowerManagementConfig.PerfPlimitDifferential << PERF_PLIMIT_DIFFERENTIAL_SHIFT ) |
                      ( SetupData.SocketConfig.PowerManagementConfig.PerfPLimitEn << REPERF_PLIMIT_EN_SHIFT );
    ppm->PerPLimitCtl = CsrPerfPlimitCtl;

    //
    // IssConfigTdpLevelInfo Bit[23:16]: the currently active Config TDP Level
    //
    ppm->Pst.ConfigTdpLevel = (UINT8) ((mIioUds->SystemStatus.IssConfigTdpLevelInfo >> 16) & 0xFF);
    ppm->Pst.CurrentPackageTdp = (mIioUds->SystemStatus.IssConfigTdpTdpInfo[0][ppm->Pst.ConfigTdpLevel] & 0x7FFF);

    for( socket = 0; socket < MAX_SOCKET; socket++) {
        UpiInPkgCEntry[socket] = (SetupData.SocketConfig.PowerManagementConfig.Kti0In[socket] |
                                              (SetupData.SocketConfig.PowerManagementConfig.Kti1In[socket] << 1) |
                                              (SetupData.SocketConfig.PowerManagementConfig.Kti2In[socket] << 2) );

        if (SetupData.SocketConfig.PowerManagementConfig.PcieIio0In[socket]) {
          PcieInPkgCEntry[socket] |= SET_PCIEx_MASK;
        }
        if (SetupData.SocketConfig.PowerManagementConfig.PcieIio1In[socket]) {
          PcieInPkgCEntry[socket] |= (SET_PCIEx_MASK << 4);
        }
        if (SetupData.SocketConfig.PowerManagementConfig.PcieIio2In[socket]) {
          PcieInPkgCEntry[socket] |= (SET_PCIEx_MASK << 8);
        }
        if (SetupData.SocketConfig.PowerManagementConfig.PcieIio3In[socket]) {
          PcieInPkgCEntry[socket] |= (SET_PCIEx_MASK << 12);
        }
        if (SetupData.SocketConfig.PowerManagementConfig.PcieIio4In[socket]) {
          PcieInPkgCEntry[socket] |= (SET_PCIEx_MASK << 16);
        }
        if (SetupData.SocketConfig.PowerManagementConfig.PcieIio5In[socket]) {
          PcieInPkgCEntry[socket] |= (SET_PCIEx_MASK << 20);
        }

    }

    AdvPwrMgtCtl = (SetupData.SocketConfig.PowerManagementConfig.SapmctlValCtl? PCD_CPU_SAPM_CTL_VAL_CTL : 0) |
                  (SetupData.SocketConfig.PowerManagementConfig.CurrentConfig? PCD_CPU_CURRENT_CONFIG : 0) |
                  (SetupData.SocketConfig.PowerManagementConfig.BootPState? PCU_CPU_EFFICIENT_BOOT : 0) |
                  (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorMsrLockControl? CPU_MSR_LOCK : 0) |
                  (SetupData.SocketConfig.PowerManagementConfig.TurboPowerLimitCsrLock? TURBO_LIMIT_CSR_LOCK : 0);

    AdvPwrMgtCtl |= SetupData.SocketConfig.PowerManagementConfig.PkgCstEntryValCtl; //PCD_CPU_PKG_CST_ENTRY_VAL_CTL

    if (SetupData.SocketConfig.PowerManagementConfig.ProcessorEistEnable == 0) {
      AdvPwrMgtCtl |= PCU_CPU_EFFICIENT_BOOT;
    }

    if (((CpuFamilyModelStepping >> 4) == CPU_FAMILY_HSX) && SetupData.SocketConfig.PowerManagementConfig.PriPlnCurCfgValCtl) {
      AdvPwrMgtCtl |= PCD_CPU_PRI_PLN_CURR_CFG_CTL;
    }

    if ((PackageCStateSetting > 0) && SetupData.SocketConfig.PowerManagementConfig.DynamicL1) {
      AdvPwrMgtCtl |= DYNAMIC_L1_DISABLE;
    }

    if (SetupData.SocketConfig.PowerManagementConfig.SPTWorkaround) {
      AdvPwrMgtCtl |= SPT_PCH_WORKAROUND;
    }

    if (SetupData.SocketConfig.PowerManagementConfig.VccSAandVccIOdisable) {
      AdvPwrMgtCtl |= VCCSA_VCCIO_DISABLE;
    }
    ppm->AdvPwrMgtCtlFlags = AdvPwrMgtCtl;

    // MSR_POWER_CTL 0x1FC
    MsrPowerCtlLow = ( SetupData.SocketConfig.PowerManagementConfig.PkgCLatNeg << PCH_NEG_DISABLE_SHIFT ) |
                     ( SetupData.SocketConfig.PowerManagementConfig.LTRSwInput << LTR_SW_DISABLE_SHIFT ) |
                     ( SetupData.SocketConfig.PowerManagementConfig.PwrPerfTuning << PWR_PERF_TUNING_CFG_MODE_SHIFT ) |
                     ( SetupData.SocketConfig.PowerManagementConfig.PwrPerfSwitch << PWR_PERF_TUNING_ENABLE_DYN_SHIFT ) |
                     ( SetupData.SocketConfig.PowerManagementConfig.SAPMControl << PWR_PERF_TUNING_DISABLE_SAPM_SHIFT ) |
                     ( SetupData.SocketConfig.PowerManagementConfig.EETurboDisable << EE_TURBO_DISABLE_SHIFT ) |
                     ( SetupData.SocketConfig.PowerManagementConfig.ProchotLock << PROCHOT_LOCK_SHIFT ) |
                     ( SetupData.SocketConfig.PowerManagementConfig.ProcessorC1eEnable << C1E_ENABLE_SHIFT ) |
                     ( (SetupData.SocketConfig.PowerManagementConfig.EnableProcHot & 0x1) << DIS_PROCHOT_OUT_SHIFT ) |
                     ( (SetupData.SocketConfig.PowerManagementConfig.EnableProcHot & 0x2) >> 1 );

    // 5332865	BIOS needs to set bit 25 in MSR 0x1FC when enabling HWP autonomous out of band mode
    if (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 2) {   //if HWPM = OOB Mode
        MsrPowerCtlLow |= ( 1 << PWR_PERF_TUNING_CFG_MODE_SHIFT );
    }

    ppm->PowerCtl.Dwords.Low = MsrPowerCtlLow;

    ppm->ProchotRatio = SetupData.SocketConfig.PowerManagementConfig.ProchotResponseRatio;

    if ((CpuFamilyModelStepping >> 4) == CPU_FAMILY_HSX) {
      // PRIMARY_PLANE_CURRENT_CONFIG_CONTROL 0x601
      MsrPriPlaneCurrentCfgCtlHigh = ( SetupData.SocketConfig.PowerManagementConfig.Psi3Code << PSI3_CODE_SHIFT ) |
        ( SetupData.SocketConfig.PowerManagementConfig.Psi3Thshld << PSI3_THSHLD_SHIFT ) |
        ( SetupData.SocketConfig.PowerManagementConfig.Psi2Code << PSI2_CODE_SHIFT ) |
        ( SetupData.SocketConfig.PowerManagementConfig.Psi2Thshld << PSI2_THSHLD_SHIFT ) |
        ( SetupData.SocketConfig.PowerManagementConfig.Psi1Code << PSI1_CODE_SHIFT ) |
        ( SetupData.SocketConfig.PowerManagementConfig.Psi1Thshld << PSI1_THSHLD_SHIFT );
    }

    MsrPriPlaneCurrentCfgCtlLow = ( SetupData.SocketConfig.PowerManagementConfig.PpcccLock << PPCCC_LOCK_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.CurrentLimit << CURRENT_LIMIT_SHIFT );

    ppm->PP0CurrentCfg.Dwords.High = MsrPriPlaneCurrentCfgCtlHigh;
    ppm->PP0CurrentCfg.Dwords.Low = MsrPriPlaneCurrentCfgCtlLow;

    // MSR_TURBO_POWER_LIMIT 0x610
    // CSR_TURBO_POWER_LIMIT 1:30:0:0xe8
    MsrTurboPowerLimitHigh = ( SetupData.SocketConfig.PowerManagementConfig.TurboPowerLimitLock << POWER_LIMIT_LOCK_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.PowerLimit2En << POWER_LIMIT_ENABLE_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.PkgClmpLim2 << PKG_CLMP_LIM_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.PowerLimit2Power );

    MsrTurboPowerLimitLow = ( SetupData.SocketConfig.PowerManagementConfig.PowerLimit1Time << POWER_LIMIT_1_TIME_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.PowerLimit1En << POWER_LIMIT_ENABLE_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.PowerLimit1Power );

    if ((CpuFamilyModelStepping >> 4) == CPU_FAMILY_HSX) {
      MsrTurboPowerLimitLow  |= ( SetupData.SocketConfig.PowerManagementConfig.PkgClmpLim1 << PKG_CLMP_LIM_SHIFT );
      MsrTurboPowerLimitHigh |= ( SetupData.SocketConfig.PowerManagementConfig.PkgClmpLim2 << PKG_CLMP_LIM_SHIFT );
    }

    if ((CpuFamilyModelStepping >> 4) == CPU_FAMILY_SKX) {
      MsrTurboPowerLimitHigh |= ( SetupData.SocketConfig.PowerManagementConfig.PowerLimit2Time << POWER_LIMIT_1_TIME_SHIFT );
    }

    ppm->TurboPowerLimit.Dwords.Low = MsrTurboPowerLimitLow;
    ppm->TurboPowerLimit.Dwords.High = MsrTurboPowerLimitHigh;

    // DYNAMIC_PERF_POWER_CTL (CSR 1:30:2:0x64)
    CsrDynamicPerfPowerCtl = ( SetupData.SocketConfig.PowerManagementConfig.UncrPerfPlmtOvrdEn << UNCORE_PERF_PLIMIT_OVERRIDE_ENABLE_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.EetOverrideEn << EET_OVERRIDE_ENABLE_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.IoBwPlmtOvrdEn << IO_BW_PLIMIT_OVERRIDE_ENABLE_SHIFT ) |
      //( SetupData.SocketConfig.PowerManagementConfig.ImcApmOvrdEn << IMC_APM_OVERRIDE_ENABLE_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.IomApmOvrdEn << IOM_APM_OVERRIDE_ENABLE_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.QpiApmOvrdEn << QPI_APM_OVERRIDE_ENABLE_SHIFT );   //4986218: Remove both changes from 4168487

    if((CpuFamilyModelStepping >> 4) == CPU_FAMILY_HSX) {
      CsrDynamicPerfPowerCtl |= (( SetupData.SocketConfig.PowerManagementConfig.EepLOverride << EEP_L_OVERRIDE_SHIFT ) |
        ( SetupData.SocketConfig.PowerManagementConfig.EepLOverrideEn << EEP_L_OVERRIDE_ENABLE_SHIFT ) |
        ( SetupData.SocketConfig.PowerManagementConfig.ITurboOvrdEn << I_TURBO_OVERRIDE_ENABLE_SHIFT ) |
        ( SetupData.SocketConfig.PowerManagementConfig.CstDemotOvrdEN << CST_DEMOTION_OVERRIDE_ENABLE_SHIFT ) |
        ( SetupData.SocketConfig.PowerManagementConfig.TrboDemotOvrdEn << TURBO_DEMOTION_OVERRIDE_ENABLE_SHIFT ));
    }

    ppm->DynamicPerPowerCtl = CsrDynamicPerfPowerCtl;

    // CSR_PCIE_ILTR_OVRD (CSR 1:30:1:78)
    // SW_LTR_OVRD (MSR 0xa02) -- not used
    CsrPcieIltrOvrd = ( SetupData.SocketConfig.PowerManagementConfig.SnpLatVld << SNOOP_LATENCY_VLD_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.SnpLatOvrd << FORCE_SNOOP_OVRD_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.SnpLatMult << SNOOP_LATENCY_MUL_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.SnpLatVal << SNOOP_LATENCY_Value_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.NonSnpLatVld << NON_SNOOP_LATENCY_VLD_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.NonSnpLatOvrd << FORCE_NON_SNOOP_OVRD_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.NonSnpLatMult << NON_SNOOP_LATENCY_MUL_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.NonSnpLatVal << NON_SNOOP_LATENCY_Value_SHIFT );

    ppm-> PcieIltrOvrd = CsrPcieIltrOvrd;

    if((CpuFamilyModelStepping >> 4) == CPU_FAMILY_SKX) { //Need to check if programming needs to be limited only if Turbo mode is enabled.
      for(i = 0; i < 8; i++) {
        TurboRatioLimitRatioCores->RatioLimitRatio[i] = SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitRatio[i];

        TurboRatioLimitRatioCores->RatioLimitRatioMask[i] = 0xFF;
        if (SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitRatio[i] > 0) {
          TurboRatioLimitRatioCores->RatioLimitRatioMask[i] = 0;
        }

        TurboRatioLimitRatioCores->RatioLimitCoresMask[i] = 0xFF;
        TurboRatioLimitRatioCores->RatioLimitCores[i] = 0;
        if (SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitCores[i] != 0xFF) {
          TurboRatioLimitRatioCores->RatioLimitCoresMask[i] = 0;
          TurboRatioLimitRatioCores->RatioLimitCores[i] = SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitCores[i];
        }
      }
    }

    MsrPerfBiasConfig = ( SetupData.SocketConfig.PowerManagementConfig.EngAvgTimeWdw1 << AVG_TIME_Window_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.P0TtlTimeLow1 << PO_TOTAL_TIME_THSHLD_LOW_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.P0TtlTimeHigh1 << PO_TOTAL_TIME_THSHLD_HIGH_SHIFT ) |
      ( SetupData.SocketConfig.PowerManagementConfig.AltEngPerfBIAS << ALT_ENERGY_PERF_BIAS_SHIFT) |
      ( SetupData.SocketConfig.PowerManagementConfig.WorkLdConfig << WORKLD_CONFIG_SHIFT );

    ppm->PerfBiasConfig.Dwords.Low = MsrPerfBiasConfig;

    //
    //ProcessorHWPM-init as disabled.
    //
    ppm->Hwpm.HWPMNative = 0;
    ppm->Hwpm.HWPMOOB = 0;
    ppm->Hwpm.HWPMEnable = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable;
    ppm->Hwpm.AutoCState = SetupData.SocketConfig.PowerManagementConfig.ProcessorAutonomousCstateEnable;
    ppm->Hwpm.HWPMInterrupt = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMInterrupt;
    ppm->Hwpm.EPPEnable = SetupData.SocketConfig.PowerManagementConfig.ProcessorEPPEnable;
    ppm->Hwpm.EPPProfile = SetupData.SocketConfig.PowerManagementConfig.ProcessorEppProfile;

    if ((SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 1) ||
       (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 3)) {
      ppm->Hwpm.HWPMNative = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable;
    }else if (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 2){
      ppm->Hwpm.HWPMOOB = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable;
      ppm->Hwpm.HWPMInterrupt = 0;
    }else if (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 0){
      ppm->Hwpm.HWPMNative = 0;
      ppm->Hwpm.HWPMOOB = 0;
      ppm->Hwpm.HWPMInterrupt = 0;
      ppm->Hwpm.EPPEnable = 0;
    }

    ppm->Hwpm.APSrocketing = SetupData.SocketConfig.PowerManagementConfig.ProcessorAPSrocketing;
    ppm->Hwpm.Scalability = SetupData.SocketConfig.PowerManagementConfig.ProcessorScalability;
    ppm->Hwpm.OutofBandAlternateEPB = SetupData.SocketConfig.PowerManagementConfig.ProcessorOutofBandAlternateEPB;

    if(SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorX2apic && SetupData.SocketConfig.SocketProcessorCoreConfiguration.ForceX2ApicIds &&
       (CpuPolicyEx1 & PCD_CPU_X2APIC_BIT)) { //if user want to reprogram > 8bit ApicId (must be X2Apic too)
      CheckAndReAssignSocketId();
    }

    for(i = 0; i < NUM_CST_LAT_MSR; i++) {    //3 CStateLatencyCtrl CSRs
      ppm->Cst.LatencyCtrl[i].Dwords.Low = ( SetupData.SocketConfig.PowerManagementConfig.CStateLatencyCtrlValid[i] << VALID_SHIFT ) |
                             ( SetupData.SocketConfig.PowerManagementConfig.CStateLatencyCtrlMultiplier[i] << MULTIPLIER_SHIFT ) |
                             ( SetupData.SocketConfig.PowerManagementConfig.CStateLatencyCtrlValue[i] << VALUE_SHIFT );
    }

    if(SetupData.SocketConfig.PowerManagementConfig.C2C3TT) { //if option is not AUTO
      ppm->C2C3TT = (UINT32)SetupData.SocketConfig.PowerManagementConfig.C2C3TT;
    } else {
      ppm->C2C3TT = 0x10;
    }

  } //end - else

  CpuPolicy |= PCD_CPU_L3_CACHE_BIT;

  Status = PcdSet32S (PcdCpuProcessorFeatureUserConfiguration, CpuPolicy);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) return Status;
  Status = PcdSet32S (PcdCpuProcessorFeatureUserConfigurationEx1, CpuPolicyEx1);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) return Status;
  if (SetupData.SystemConfig.McBankWarmBootClearError == 1) {
     Status = PcdSetBoolS (PcdIsPowerOnReset, TRUE);
     ASSERT_EFI_ERROR (Status);
     if (EFI_ERROR(Status)) return Status;
  } else {
     Status = PcdSetBoolS (PcdIsPowerOnReset, FALSE);
     ASSERT_EFI_ERROR (Status);
     if (EFI_ERROR(Status)) return Status;
  }

  //
  // Cpu Driver could be dispatched after this protocol installed.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gIntelCpuPcdsSetDoneProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
