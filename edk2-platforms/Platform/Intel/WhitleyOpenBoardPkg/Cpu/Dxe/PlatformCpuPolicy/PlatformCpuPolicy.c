/** @file
  CPU Policy Platform DXE Driver implementation.

  @copyright
  Copyright 2015 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include <Protocol/CpuPolicyProtocol.h>
#include <Protocol/PpmPolicyProtocol.h>
#include <IioUniversalData.h>
#include <Protocol/PlatformType.h>
#include <SetupTable.h>
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
#include <Platform.h>
#include <Cpu/CpuIds.h>
#include <CpuAndRevisionDefines.h>
#include <ScratchpadList.h>
#include <Register/Cpuid.h>
#include <ProcessorPpmSetup.h>
#include <Protocol/DynamicSiLibraryProtocol.h>

CHAR16 mCpuSocketStr[8][5] = {L"CPU0", L"CPU1", L"CPU2", L"CPU3", L"CPU4", L"CPU5", L"CPU6", L"CPU7"};
CHAR16 mCpuAssetTagStr[] = L"UNKNOWN";
IIO_UDS                     *mIioUds;
CPU_POLICY_CONFIGURATION    mCpuPolicyConfiguration;
PPM_POLICY_CONFIGURATION    mPpmPolicyConfiguration;

/**
  Set platform CPU data that related to SMBIOS.
**/
VOID
PlatformCpuSmbiosData (
  VOID
  )
{
  UINT32                        CpuSocketCount;
  UINTN                         Index;
  CHAR16                        **CpuSocketNames;
  CHAR16                        **CpuAssetTags;
  EFI_STATUS                    Status = EFI_SUCCESS;

  //
  // Set the count of CPU sockets on the board.
  //
  CpuSocketCount = PcdGet32(PcdOemSkuBoardSocketCount);

  Status = PcdSet32S (PcdPlatformCpuSocketCount, CpuSocketCount);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return;
  }

  CpuSocketNames = AllocatePool (CpuSocketCount * sizeof (UINTN));

  if (CpuSocketNames == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocatePool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  CpuAssetTags = AllocatePool (CpuSocketCount * sizeof (UINTN));
  if (CpuAssetTags == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocatePool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    gBS->FreePool (CpuSocketNames);
    return;
  }

  for (Index = 0; Index < CpuSocketCount; Index++) {
    CpuSocketNames[Index] = mCpuSocketStr[Index];
    CpuAssetTags[Index] = mCpuAssetTagStr;
  }

  mCpuPolicyConfiguration.PlatformCpuSocketNames = (UINT64) (UINTN) CpuSocketNames;

  mCpuPolicyConfiguration.PlatformCpuAssetTags = (UINT64) (UINTN) CpuAssetTags;
}



/**

    Re-assign socket id when both X2APIC and ForceX2Apic are enabled.

    @param None

    @retval None

**/
VOID
CheckAndReAssignSocketId(
  VOID
  )
{
  CPU_SOCKET_ID_INFO    *pcdSktIdPtr;
  UINT32                i, IntraPackageIdBits;
  UINTN                 PcdSize;
  EFI_STATUS            Status;
  UINT32                MaxSocketCount;

  MaxSocketCount = FixedPcdGet32(PcdMaxCpuSocketCount);
  DEBUG ((DEBUG_INFO, "::SocketCount %08x\n", MaxSocketCount));
  pcdSktIdPtr = (CPU_SOCKET_ID_INFO *)PcdGetPtr(PcdCpuSocketId);
  PcdSize = LibPcdGetSize (PcdToken (PcdCpuSocketId)); //MAX_SOCKET * sizeof(CPU_SOCKET_ID_INFO);
  ASSERT(PcdSize == (MAX_SOCKET * sizeof(CPU_SOCKET_ID_INFO)));
  Status = PcdSetPtrS (PcdCpuSocketId, &PcdSize, (VOID *)pcdSktIdPtr);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return;
  }
  DEBUG ((EFI_D_INFO, "::SockeId Pcd at %08x, size %x\n", PcdGetPtr(PcdCpuSocketId), PcdSize));

  for(i = 0; i < MAX_SOCKET; i++) {
    if (mIioUds->PlatformData.CpuQpiInfo[i].Valid) {
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

    if (pcdSktIdPtr[i].DefaultSocketId == (UINT32)-1) {
      continue;
    }

    switch(IntraPackageIdBits) {
      case 4: //socket bit starts from bit4 of ApicId
      case 5: //socket bit starts from bit5 of ApicId
        if (MAX_SOCKET == 4) {
          pcdSktIdPtr[i].NewSocketId |= (APICID_MASK_BIT14_8 << (8 - IntraPackageIdBits));
        } else {
          //3bit in lower 8bit as skt field, to avoid ApicID= FFs, leave bit8 untouched for 8S
          pcdSktIdPtr[i].NewSocketId |= (0x7E << (8 - IntraPackageIdBits));      //leave bit8 to 0 so we don't have FFs in ApicId
        }
        break;

     case 6: //socket bit starts from bit6 of ApicId
        if (MAX_SOCKET == 4) {
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
  EFI_HANDLE                       Handle;
  UINT8                            socket;
  TURBO_POWRER_LIMIT               *TurboPowerLimit;
  EFI_PLATFORM_TYPE_PROTOCOL       *PlatformType;
  UINT32                           CpuFamilyModelStepping;
  BIOS_SCRATCHPAD7_STRUCT          Sp7;
  UINT64                           i;
  UINT8                            PackageCStateSetting = 0;
  UINT8                            CpuCStateValue = 0;
  EFI_GUID                         UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_HOB_GUID_TYPE                *GuidHob;
  SYSTEM_MEMORY_MAP_HOB            *SystemMemoryMap;
  CPU_VAR_DATA                     *CpuVarDataPtr = NULL;
  DYNAMIC_SI_LIBARY_PROTOCOL       *DynamicSiLibraryProtocol = NULL;

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &DynamicSiLibraryProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  CpuVarDataPtr = DynamicSiLibraryProtocol->GetCpuVarData ();

  GuidHob   = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  mIioUds   = GET_GUID_HOB_DATA(GuidHob);

  SystemMemoryMap = DynamicSiLibraryProtocol->GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    return EFI_NOT_FOUND;
  }

  SetMem (&mCpuPolicyConfiguration, sizeof (CPU_POLICY_CONFIGURATION), 0x00);
  SetMem (&mPpmPolicyConfiguration, sizeof (PPM_POLICY_CONFIGURATION), 0x00);

  AsmCpuid (1, &CpuFamilyModelStepping, NULL, NULL, NULL);

  PlatformCpuSmbiosData ();

  //
  // Read the current system configuration variable store.
  //
  ZeroMem(&SetupData, sizeof(SETUP_DATA));
  CopyMem (&SetupData.SocketConfig.IioConfig, PcdGetPtr(PcdSocketIioConfig), sizeof(SOCKET_IIO_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.CommonRcConfig, PcdGetPtr(PcdSocketCommonRcConfig), sizeof(SOCKET_COMMONRC_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.UpiConfig, PcdGetPtr(PcdSocketMpLinkConfig), sizeof(SOCKET_MP_LINK_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.MemoryConfig, PcdGetPtr(PcdSocketMemoryConfig), sizeof(SOCKET_MEMORY_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.PowerManagementConfig, PcdGetPtr(PcdSocketPowerManagementConfig), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
  CopyMem (&SetupData.SocketConfig.SocketProcessorCoreConfiguration, PcdGetPtr(PcdSocketProcessorCoreConfig), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
  CopyMem (&SetupData.SystemConfig, PcdGetPtr(PcdSetup), sizeof(SYSTEM_CONFIGURATION));
  CopyMem (&SetupData.PchSetup, PcdGetPtr(PcdPchSetup), sizeof(PCH_SETUP));

  Sp7.Data = DynamicSiLibraryProtocol->ReadScratchpad7 ();
  DEBUG ((EFI_D_INFO, "AYP Debug scratchpad7: %x Stepping %x\n", Sp7.Bits.AepDimmPresent, CpuFamilyModelStepping & 0x0f));
  if ((Sp7.Bits.AepDimmPresent == 1) && ((CpuFamilyModelStepping & 0x0f) < 0x04) && ((CpuFamilyModelStepping >> 4) == CPU_FAMILY_SKX)) {
    SetupData.SocketConfig.PowerManagementConfig.PackageCState = 0;
  }

  if (SetupData.SocketConfig.PowerManagementConfig.PackageCState == PPM_AUTO) {
      PackageCStateSetting = C6_ENABLE;  //POR Default = C6
  } else {
      PackageCStateSetting = SetupData.SocketConfig.PowerManagementConfig.PackageCState;
  }

  if (SetupData.SocketConfig.PowerManagementConfig.C6Enable == PPM_AUTO) {
      CpuCStateValue |= C6_ENABLE;   //POR Default = Enabled
  } else {
      CpuCStateValue |= (SetupData.SocketConfig.PowerManagementConfig.C6Enable * C6_ENABLE);
  }

  mCpuPolicyConfiguration.Policy.CpuCoreCStateValue = CpuCStateValue;

  //
  //  Update CpuMtoIWa from StructurePcd
  //
  if (DynamicSiLibraryProtocol->IsCpuAndRevision (CPU_ICXSP, REV_R0) || DynamicSiLibraryProtocol->IsCpuAndRevision (CPU_ICXSP, REV_L0) || DynamicSiLibraryProtocol->IsCpuAndRevision (CPU_ICXSP, REV_C0)) {
    mCpuPolicyConfiguration.Policy.CpuMtoIWa = SetupData.SocketConfig.SocketProcessorCoreConfiguration.CpuMtoIWa;
  }
  //
  // Verify that the value being set is within the valid range 0 to MAX_SOCKET - 1
  //
  if (SetupData.SocketConfig.SocketProcessorCoreConfiguration.BspSelection > MAX_SOCKET) {
    SetupData.SocketConfig.SocketProcessorCoreConfiguration.BspSelection= 0xFF;
  }
  mCpuPolicyConfiguration.SbspSelection = SetupData.SocketConfig.SocketProcessorCoreConfiguration.BspSelection;
  //
  // Map CPU setup options to mCpuPolicyConfiguration
  //
  mCpuPolicyConfiguration.Policy.CpuEistEnable           = SetupData.SocketConfig.PowerManagementConfig.ProcessorEistEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuVtEnable             = SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorVmxEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuLtEnable             = SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorSmxEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuFastStringEnable     = SetupData.SocketConfig.SocketProcessorCoreConfiguration.FastStringEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuMaxCpuidValueLimitEnable = SetupData.SocketConfig.SocketProcessorCoreConfiguration.CpuidMaxValue ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuMachineCheckEnable   = SetupData.SocketConfig.SocketProcessorCoreConfiguration.MachineCheckEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuDcuPrefetcherEnable  = SetupData.SocketConfig.SocketProcessorCoreConfiguration.DCUStreamerPrefetcherEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuIpPrefetcherEnable   = SetupData.SocketConfig.SocketProcessorCoreConfiguration.DCUIPPrefetcherEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuMonitorMwaitEnable   = SetupData.SocketConfig.PowerManagementConfig.MonitorMWait ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuTurboModeEnable      = SetupData.SocketConfig.PowerManagementConfig.TurboMode ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuThermalManagementEnable = SetupData.SocketConfig.PowerManagementConfig.EnableThermalMonitor ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuTccActivationOffset = SetupData.SocketConfig.PowerManagementConfig.TCCActivationOffset;
  mCpuPolicyConfiguration.Policy.CpuL1NextPagePrefetcherDisable  = SetupData.SocketConfig.SocketProcessorCoreConfiguration.CpuL1NextPagePrefetcherDisable ? TRUE : FALSE;

  if (SetupData.SocketConfig.PowerManagementConfig.TStateEnable && (SetupData.SocketConfig.PowerManagementConfig.OnDieThermalThrottling > 0)) {
    mCpuPolicyConfiguration.Policy.CpuTStateEnable = SetupData.SocketConfig.PowerManagementConfig.TStateEnable ? TRUE : FALSE;
  }

  mCpuPolicyConfiguration.Policy.CpuMlcStreamerPrefetecherEnable  = SetupData.SocketConfig.SocketProcessorCoreConfiguration.MlcStreamerPrefetcherEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuMlcSpatialPrefetcherEnable  = SetupData.SocketConfig.SocketProcessorCoreConfiguration.MlcSpatialPrefetcherEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuThreeStrikeCounterEnable    = SetupData.SocketConfig.SocketProcessorCoreConfiguration.ThreeStrikeTimer ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuEnergyPerformanceBiasEnable = TRUE;
  mCpuPolicyConfiguration.Policy.CpuX2ApicEnable                = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorX2apic || DynamicSiLibraryProtocol->X2ApicIdDetect (NULL)) ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuAesEnable                   = SetupData.SocketConfig.SocketProcessorCoreConfiguration.AesEnable ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuPpinControlEnable           = SetupData.SocketConfig.SocketProcessorCoreConfiguration.PpinControl ? TRUE : FALSE;
  mCpuPolicyConfiguration.Policy.CpuPeciDownstreamWriteEnable   = SetupData.SocketConfig.SocketProcessorCoreConfiguration.PCIeDownStreamPECIWrite ? TRUE : FALSE;
  if ((CpuFamilyModelStepping >> 4) == CPU_FAMILY_ICX) {
    mCpuPolicyConfiguration.Policy.CpuC1AutoDemotionEnable        = SetupData.SocketConfig.PowerManagementConfig.C1AutoDemotion ? TRUE : FALSE;
    mCpuPolicyConfiguration.Policy.CpuC1AutoUndemotionEnable      = SetupData.SocketConfig.PowerManagementConfig.C1AutoUnDemotion ? TRUE : FALSE;
  }
  mCpuPolicyConfiguration.Policy.CpuCStateEnable                = TRUE;

  mPpmPolicyConfiguration.OverclockingLock  = SetupData.SocketConfig.PowerManagementConfig.OverclockingLock;
  mPpmPolicyConfiguration.AvxSupport        = SetupData.SocketConfig.PowerManagementConfig.AvxSupport;
  mPpmPolicyConfiguration.AvxIccpLevel      = SetupData.SocketConfig.PowerManagementConfig.AvxIccpLevel;
  mPpmPolicyConfiguration.GpssTimer         = SetupData.SocketConfig.PowerManagementConfig.GpssTimer;

  mPpmPolicyConfiguration.FastRaplDutyCycle = SetupData.SocketConfig.PowerManagementConfig.FastRaplDutyCycle;

  if (mIioUds->PlatformData.EVMode) {
    mCpuPolicyConfiguration.Policy.CpuLtEnable = FALSE;
  }

  if (SetupData.SocketConfig.PowerManagementConfig.ProcessorEistEnable) {
    mCpuPolicyConfiguration.Policy.CpuHwCoordinationEnable = SetupData.SocketConfig.PowerManagementConfig.ProcessorEistPsdFunc ? FALSE : TRUE;
    mCpuPolicyConfiguration.Policy.CpuBootPState = SetupData.SocketConfig.PowerManagementConfig.BootPState;
  } else {
    mCpuPolicyConfiguration.Policy.CpuTurboModeEnable = FALSE;
  }

  mCpuPolicyConfiguration.Policy.CpuAcpiLvl2Addr = PCH_ACPI_BASE_ADDRESS + R_ACPI_LV2;
  mCpuPolicyConfiguration.Policy.CpuPackageCStateLimit = PackageCStateSetting;

  if ((SetupData.SocketConfig.PowerManagementConfig.TStateEnable) && (SetupData.SocketConfig.PowerManagementConfig.OnDieThermalThrottling > 0)) {
    mCpuPolicyConfiguration.Policy.CpuClockModulationDutyCycle = SetupData.SocketConfig.PowerManagementConfig.OnDieThermalThrottling;
  }

  Status = PcdSetBoolS (PcdCpuSmmMsrSaveStateEnable, SetupData.SocketConfig.SocketProcessorCoreConfiguration.eSmmSaveState? TRUE : FALSE);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (SetupData.SystemConfig.EmcaMsmiEn != 0) {
    Status = PcdSetBoolS (PcdCpuSmmProtectedModeEnable, TRUE);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  } else {
    Status = PcdSetBoolS (PcdCpuSmmProtectedModeEnable, FALSE);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }


  mCpuPolicyConfiguration.Policy.CpuIioLlcWaysBitMask         = SetupData.SocketConfig.SocketProcessorCoreConfiguration.IioLlcWaysMask;
  mCpuPolicyConfiguration.Policy.CpuExpandedIioLlcWaysBitMask = SetupData.SocketConfig.SocketProcessorCoreConfiguration.ExpandedIioLlcWaysMask;
  mCpuPolicyConfiguration.Policy.CpuRemoteWaysBitMask         = SetupData.SocketConfig.SocketProcessorCoreConfiguration.RemoteWaysMask;
  mCpuPolicyConfiguration.Policy.CpuRrqCountThreshold         = mIioUds->PlatformData.RemoteRequestThreshold;
  mCpuPolicyConfiguration.Policy.CpuCrashLogGprs = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.CpuCrashLogGprs > 0) ? TRUE : FALSE;

  //CSR SAPM CTL
  for( socket = 0; socket < MAX_SOCKET; socket++) {
    mPpmPolicyConfiguration.SapmCtl[socket].Iio0PkgcClkGateDis  = SetupData.SocketConfig.PowerManagementConfig.Iio0PkgcClkGateDis[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Iio1PkgcClkGateDis  = SetupData.SocketConfig.PowerManagementConfig.Iio1PkgcClkGateDis[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Iio2PkgcClkGateDis  = SetupData.SocketConfig.PowerManagementConfig.Iio2PkgcClkGateDis[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Kti01PkgcClkGateDis = SetupData.SocketConfig.PowerManagementConfig.Kti01PkgcClkGateDis[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Kti23PkgcClkGateDis = SetupData.SocketConfig.PowerManagementConfig.Kti23PkgcClkGateDis[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Mc0PkgcClkGateDis   = SetupData.SocketConfig.PowerManagementConfig.Mc0PkgcClkGateDis[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Mc1PkgcClkGateDis   = SetupData.SocketConfig.PowerManagementConfig.Mc1PkgcClkGateDis[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].P0pllOffEna         = SetupData.SocketConfig.PowerManagementConfig.P0pllOffEna[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].P1pllOffEna         = SetupData.SocketConfig.PowerManagementConfig.P1pllOffEna[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].P2pllOffEna         = SetupData.SocketConfig.PowerManagementConfig.P2pllOffEna[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Kti01pllOffEna      = SetupData.SocketConfig.PowerManagementConfig.Kti01pllOffEna[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Kti23pllOffEna      = SetupData.SocketConfig.PowerManagementConfig.Kti23pllOffEna[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Mc0pllOffEna        = SetupData.SocketConfig.PowerManagementConfig.Mc0pllOffEna[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].Mc1pllOffEna        = SetupData.SocketConfig.PowerManagementConfig.Mc1pllOffEna[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].SetvidDecayDisable  = SetupData.SocketConfig.PowerManagementConfig.SetvidDecayDisable[socket];
    mPpmPolicyConfiguration.SapmCtl[socket].SapmCtlLock         = SetupData.SocketConfig.PowerManagementConfig.SapmCtlLock[socket];

    if (SetupData.SocketConfig.MemoryConfig.OppSrefEn == 1) {
      mPpmPolicyConfiguration.SapmCtl[socket].Mc0PkgcIoVolRedDis  = 1;
      mPpmPolicyConfiguration.SapmCtl[socket].Mc1PkgcIoVolRedDis  = 1;
      mPpmPolicyConfiguration.SapmCtl[socket].Mc0PkgcDigVolRedDis = 1;
      mPpmPolicyConfiguration.SapmCtl[socket].Mc1PkgcDigVolRedDis = 1;
    }

    if (Sp7.Bits.AepDimmPresent == 1) {
      mPpmPolicyConfiguration.SapmCtl[socket].Mc0pllOffEna        = 0;
      mPpmPolicyConfiguration.SapmCtl[socket].Mc1pllOffEna        = 0;
      mPpmPolicyConfiguration.SapmCtl[socket].Mc0PkgcClkGateDis   = 1;
      mPpmPolicyConfiguration.SapmCtl[socket].Mc1PkgcClkGateDis   = 1;
    }
  }

  //
  // PMAX Detector Config
  //
  mPpmPolicyConfiguration.PmaxConfig.PmaxDetector       = SetupData.SocketConfig.PowerManagementConfig.PmaxDetector;
  mPpmPolicyConfiguration.PmaxConfig.PmaxAutoAdjustment = SetupData.SocketConfig.PowerManagementConfig.PmaxAutoAdjustment;
  mPpmPolicyConfiguration.PmaxConfig.PmaxLoadLine       = SetupData.SocketConfig.PowerManagementConfig.PmaxLoadLine;
  mPpmPolicyConfiguration.PmaxConfig.PmaxSign           = SetupData.SocketConfig.PowerManagementConfig.PmaxSign;
  mPpmPolicyConfiguration.PmaxConfig.PmaxOffset         = SetupData.SocketConfig.PowerManagementConfig.PmaxOffset;
  mPpmPolicyConfiguration.PmaxConfig.PmaxTriggerSetup   = SetupData.SocketConfig.PowerManagementConfig.PmaxTriggerSetup;

  //
  // PERF_P_LIMIT_CONTROL (CSR 1:30:2:0xE4)
  //
  mPpmPolicyConfiguration.PerPLimitCtl.PerfPLmtThshld         = SetupData.SocketConfig.PowerManagementConfig.PerfPLmtThshld;
  mPpmPolicyConfiguration.PerPLimitCtl.PerfPLimitClipC        = SetupData.SocketConfig.PowerManagementConfig.PerfPLimitClipC;
  mPpmPolicyConfiguration.PerPLimitCtl.PerfPlimitDifferential = SetupData.SocketConfig.PowerManagementConfig.PerfPlimitDifferential;
  mPpmPolicyConfiguration.PerPLimitCtl.PerfPLimitEn           = SetupData.SocketConfig.PowerManagementConfig.PerfPLimitEn;

    mPpmPolicyConfiguration.DynamicIss = SetupData.SocketConfig.PowerManagementConfig.DynamicIss;
    mPpmPolicyConfiguration.IssCapableSystem = CpuVarDataPtr->IssCapableSystem;
    mPpmPolicyConfiguration.ConfigTDPLevel = CpuVarDataPtr->IssConfigTdpCurrentLevel;
    mPpmPolicyConfiguration.ConfigTDPLock = SetupData.SocketConfig.PowerManagementConfig.ConfigTdpLock;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
      mPpmPolicyConfiguration.PmaxConfig.BasePackageTdp[socket] = CpuVarDataPtr->IssConfigTdpPower[socket][0];
      mPpmPolicyConfiguration.CurrentPackageTdp[socket] = CpuVarDataPtr->IssConfigTdpPower[socket][mPpmPolicyConfiguration.ConfigTDPLevel];

    if ((CpuFamilyModelStepping >> 4) != CPU_FAMILY_SKX) {
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaKti[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaKti[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaRlink[socket]        = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaRlink[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaFxr[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaFxr[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaMcddr[socket]        = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaMcddr[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaHbm[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaHbm[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaIio[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaIio[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaHqm[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaHqm[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaNac[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaNac[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaTip[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaTip[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnablePkgCCriteriaMdfs[socket]         = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaMdfs[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaLogicalIpType[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaLogicalIpType[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaLogicalIpTypeMcddr[socket] = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaLogicalIpTypeMcddr[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaLogicalIpTypeIio[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaLogicalIpTypeIio[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoKti[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoKti[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnableLinkInL1Kti[socket]              = SetupData.SocketConfig.PowerManagementConfig.EnableLinkInL1Kti[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoRlink[socket]    = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoRlink[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnableLinkInL1Rlink[socket]            = SetupData.SocketConfig.PowerManagementConfig.EnableLinkInL1Rlink[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoFxr[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoFxr[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgcCriteraPsMaskFxr[socket]           = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskFxr[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaAllowedPsMaskFxr[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskFxr[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoMcddr[socket]    = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoMcddr[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgcCriteriaPsOptionMcddr[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteriaPsOptionMcddr[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoHbm[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoHbm[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgcCriteriaPsOptionHbm[socket]        = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteriaPsOptionHbm[socket];

      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoIio[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoIio[socket];
      mPpmPolicyConfiguration.PkgcCriteria.EnableLinkInL1Iio[socket]              = SetupData.SocketConfig.PowerManagementConfig.EnableLinkInL1Iio[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoHqm[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoHqm[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgcCriteraPsMaskHqm[socket]           = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskHqm[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaAllowedPsMaskHqm[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskHqm[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoNac[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoNac[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgcCriteraPsMaskNac[socket]           = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskNac[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaAllowedPsMaskNac[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskNac[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoTip[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoTip[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgcCriteraPsMaskTip[socket]           = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskTip[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaAllowedPsMaskTip[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskTip[socket];
      mPpmPolicyConfiguration.PkgcCriteria.PkgCCriteriaInstanceNoMdfs[socket]     = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoMdfs[socket];
      mPpmPolicyConfiguration.PkgcCriteria.AllowLpStateMdfs[socket]               = SetupData.SocketConfig.PowerManagementConfig.AllowLpStateMdfs[socket];
    }

    if ((CpuFamilyModelStepping >> 4) == CPU_FAMILY_SKX) {
      mPpmPolicyConfiguration.PpmCst.PkgCstEntryCriteriaMaskKti[socket] = (SetupData.SocketConfig.PowerManagementConfig.Kti0In[socket] |
                               (SetupData.SocketConfig.PowerManagementConfig.Kti1In[socket] << 1) |
                               (SetupData.SocketConfig.PowerManagementConfig.Kti2In[socket] << 2) );

      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio0In[socket]) {
        mPpmPolicyConfiguration.PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= SET_PCIEx_MASK;
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio1In[socket]) {
        mPpmPolicyConfiguration.PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 4);
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio2In[socket]) {
        mPpmPolicyConfiguration.PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 8);
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio3In[socket]) {
        mPpmPolicyConfiguration.PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 12);
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio4In[socket]) {
        mPpmPolicyConfiguration.PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 16);
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio5In[socket]) {
        mPpmPolicyConfiguration.PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 20);
      }
    }
  }

  mPpmPolicyConfiguration.AdvPwrMgtCtl.SapmctlValCtl = SetupData.SocketConfig.PowerManagementConfig.SapmctlValCtl;
  mPpmPolicyConfiguration.AdvPwrMgtCtl.MsrLock       = 1;
  mPpmPolicyConfiguration.AdvPwrMgtCtl.PkgCstEntryValCtl    = SetupData.SocketConfig.PowerManagementConfig.PkgCstEntryValCtl;
  mPpmPolicyConfiguration.AdvPwrMgtCtl.CurrentConfig        = SetupData.SocketConfig.PowerManagementConfig.CurrentConfig;
  mPpmPolicyConfiguration.TurboPowerLimit.TurboLimitCsrLock = 1;
  mPpmPolicyConfiguration.AdvPwrMgtCtl.MsrPkgCstConfigControlLock = SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorMsrPkgCstConfigControlLock ? 1 : 0;

  if ((PackageCStateSetting > 0) && SetupData.SocketConfig.PowerManagementConfig.DynamicL1) {
    mPpmPolicyConfiguration.AdvPwrMgtCtl.DynamicL1Disable = 1;
  }

  if (SetupData.SocketConfig.PowerManagementConfig.VccSAandVccIOdisable) {
    mPpmPolicyConfiguration.AdvPwrMgtCtl.VccsaVccioDisable = 1;
  }

  mPpmPolicyConfiguration.AdvPwrMgtCtl.SwLtrOvrdCtl = SetupData.SocketConfig.PowerManagementConfig.SwLtrOvrdCtl;
  mPpmPolicyConfiguration.AdvPwrMgtCtl.EnableLowerLatencyMode = SetupData.SocketConfig.PowerManagementConfig.EnableLowerLatencyMode;

  if (SetupData.SocketConfig.PowerManagementConfig.ProcessorEistEnable == 0) {
    mPpmPolicyConfiguration.PowerCtl.EeTurboDisable = 1;
  }

  mPpmPolicyConfiguration.PowerCtl.PkgCLatNeg     = SetupData.SocketConfig.PowerManagementConfig.PkgCLatNeg;
  mPpmPolicyConfiguration.PowerCtl.LtrSwInput     = SetupData.SocketConfig.PowerManagementConfig.LTRSwInput;
  mPpmPolicyConfiguration.PowerCtl.PwrPerfSwitch  = SetupData.SocketConfig.PowerManagementConfig.PwrPerfSwitch;
  mPpmPolicyConfiguration.PowerCtl.SapmControl    = SetupData.SocketConfig.PowerManagementConfig.SAPMControl;
  mPpmPolicyConfiguration.PowerCtl.EeTurboDisable = SetupData.SocketConfig.PowerManagementConfig.EETurboDisable;
  mPpmPolicyConfiguration.PowerCtl.ProchotLock    = 1;
  mPpmPolicyConfiguration.PowerCtl.C1eEnable      = SetupData.SocketConfig.PowerManagementConfig.ProcessorC1eEnable;
  mPpmPolicyConfiguration.PowerCtl.SetvidDecayDisable = SetupData.SocketConfig.PowerManagementConfig.SetvidDecayDisable[0];

  if ((SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 3) && !SetupData.SocketConfig.PowerManagementConfig.PwrPerfTuning) {
    // If hwp native w/o legacy then default value is "BIOS Controls EPB". OS Control of EPB should not be allowed
    mPpmPolicyConfiguration.PowerCtl.PwrPerfTuning  = 1;
  } else if ((SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 2) && !SetupData.SocketConfig.PowerManagementConfig.PwrPerfTuning) {
    // if hwp oob mode then default value is "PECI Controls EPB". OS Control of EPB should not be allowed
    mPpmPolicyConfiguration.PowerCtl.PwrPerfTuning  = 2;
  } else {
    mPpmPolicyConfiguration.PowerCtl.PwrPerfTuning  = SetupData.SocketConfig.PowerManagementConfig.PwrPerfTuning;
  }

  //  From Setup:
  //    0: Output only
  //    1: Disable
  //    2: Bi-Directional
  //    3: Input Only
  //
  if ((SetupData.SocketConfig.PowerManagementConfig.EnableProcHot & 1) == 0) {
    mPpmPolicyConfiguration.PowerCtl.ProchotOutputDisable = 0;
  } else {
    mPpmPolicyConfiguration.PowerCtl.ProchotOutputDisable = 1;
  }

  if ((SetupData.SocketConfig.PowerManagementConfig.EnableProcHot & 2) == 0) {
    mPpmPolicyConfiguration.PowerCtl.BidirProchotEnable = 0;
  } else {
    mPpmPolicyConfiguration.PowerCtl.BidirProchotEnable = 1;
  }

  //
  // PROCHOT_RESPONSE_RATIO (CSR 1:30:2:0xB0)
  //
  mPpmPolicyConfiguration.ProchotRatio = SetupData.SocketConfig.PowerManagementConfig.ProchotResponseRatio;

  mPpmPolicyConfiguration.PpoCurrentCfg.PpcccLock = SetupData.SocketConfig.PowerManagementConfig.PpcccLock;
  mPpmPolicyConfiguration.PpoCurrentCfg.CurrentLimit = SetupData.SocketConfig.PowerManagementConfig.CurrentLimit;

  // MSR_PACKAGE_POWER_LIMIT 0x610
  // CSR_TURBO_POWER_LIMIT 1:30:0:0xe8
  TurboPowerLimit = &mPpmPolicyConfiguration.TurboPowerLimit;
  TurboPowerLimit->TurboPowerLimitLock  = SetupData.SocketConfig.PowerManagementConfig.TurboPowerLimitLock;
  TurboPowerLimit->PowerLimit1Power     = SetupData.SocketConfig.PowerManagementConfig.PowerLimit1Power;
  TurboPowerLimit->PowerLimit1Time      = SetupData.SocketConfig.PowerManagementConfig.PowerLimit1Time;
  TurboPowerLimit->PowerLimit1En        = SetupData.SocketConfig.PowerManagementConfig.PowerLimit1En;
  TurboPowerLimit->PowerLimit2Power     = SetupData.SocketConfig.PowerManagementConfig.PowerLimit2Power;
  TurboPowerLimit->PowerLimit2Time      = SetupData.SocketConfig.PowerManagementConfig.PowerLimit2Time;
  TurboPowerLimit->PowerLimit2En        = SetupData.SocketConfig.PowerManagementConfig.PowerLimit2En;
  TurboPowerLimit->PkgClmpLim1          = 1;
  TurboPowerLimit->PkgClmpLim2          = 1;

  // DYNAMIC_PERF_POWER_CTL (CSR 1:30:2:0x64)
  mPpmPolicyConfiguration.DynamicPerPowerCtl.UncrPerfPlmtOvrdEn   = SetupData.SocketConfig.PowerManagementConfig.UncrPerfPlmtOvrdEn;
  mPpmPolicyConfiguration.DynamicPerPowerCtl.EetOverrideEn        = SetupData.SocketConfig.PowerManagementConfig.EetOverrideEn;
  mPpmPolicyConfiguration.DynamicPerPowerCtl.IoBwPlmtOvrdEn       = SetupData.SocketConfig.PowerManagementConfig.IoBwPlmtOvrdEn;
  mPpmPolicyConfiguration.DynamicPerPowerCtl.IomApmOvrdEn         = SetupData.SocketConfig.PowerManagementConfig.IomApmOvrdEn;
  mPpmPolicyConfiguration.DynamicPerPowerCtl.KtiApmOvrdEn         = SetupData.SocketConfig.PowerManagementConfig.KtiApmOvrdEn;

  //
  // CSR_PCIE_ILTR_OVRD (CSR 1:30:1:0xFC)
  // SW_LTR_OVRD (MSR 0xa02) -- not used
  //
  mPpmPolicyConfiguration.PcieIltrOvrd.SnpLatVld     = SetupData.SocketConfig.PowerManagementConfig.SnpLatVld;
  mPpmPolicyConfiguration.PcieIltrOvrd.SnpLatOvrd    = SetupData.SocketConfig.PowerManagementConfig.SnpLatOvrd;
  mPpmPolicyConfiguration.PcieIltrOvrd.SnpLatMult    = SetupData.SocketConfig.PowerManagementConfig.SnpLatMult;
  mPpmPolicyConfiguration.PcieIltrOvrd.SnpLatVal     = SetupData.SocketConfig.PowerManagementConfig.SnpLatVal;
  mPpmPolicyConfiguration.PcieIltrOvrd.NonSnpLatVld  = SetupData.SocketConfig.PowerManagementConfig.NonSnpLatVld;
  mPpmPolicyConfiguration.PcieIltrOvrd.NonSnpLatOvrd = SetupData.SocketConfig.PowerManagementConfig.NonSnpLatOvrd;
  mPpmPolicyConfiguration.PcieIltrOvrd.NonSnpLatMult = SetupData.SocketConfig.PowerManagementConfig.NonSnpLatMult;
  mPpmPolicyConfiguration.PcieIltrOvrd.NonSnpLatVal  = SetupData.SocketConfig.PowerManagementConfig.NonSnpLatVal;

  for(i = 0; i < 8; i++) {
    mPpmPolicyConfiguration.TurboRatioLimit.RatioLimitRatio[i] = SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitRatio[i];

    mPpmPolicyConfiguration.TurboRatioLimit.RatioLimitRatioMask[i] = 0xFF;
    if (SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitRatio[i] > 0) {
      mPpmPolicyConfiguration.TurboRatioLimit.RatioLimitRatioMask[i] = 0;
    }

    mPpmPolicyConfiguration.TurboRatioLimit.RatioLimitCoresMask[i] = 0xFF;
    mPpmPolicyConfiguration.TurboRatioLimit.RatioLimitCores[i] = 0;
    if (SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitCores[i] != 0xFF) {
      mPpmPolicyConfiguration.TurboRatioLimit.RatioLimitCoresMask[i] = 0;
      mPpmPolicyConfiguration.TurboRatioLimit.RatioLimitCores[i] = SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitCores[i];
    }
  }

  //
  // ENERGY_PERF_BIAS_CONFIG (MSR 0xA01)
  //
  mPpmPolicyConfiguration.PerfBiasConfig.EngAvgTimeWdw1 = SetupData.SocketConfig.PowerManagementConfig.EngAvgTimeWdw1;
  mPpmPolicyConfiguration.PerfBiasConfig.P0TtlTimeLow1  = SetupData.SocketConfig.PowerManagementConfig.P0TtlTimeLow1;
  mPpmPolicyConfiguration.PerfBiasConfig.P0TtlTimeHigh1 = SetupData.SocketConfig.PowerManagementConfig.P0TtlTimeHigh1;
  mPpmPolicyConfiguration.PerfBiasConfig.AltEngPerfBIAS = SetupData.SocketConfig.PowerManagementConfig.AltEngPerfBIAS;
  mPpmPolicyConfiguration.PerfBiasConfig.WorkLdConfig   = SetupData.SocketConfig.PowerManagementConfig.WorkLdConfig;

  //
  //ProcessorHWPM-init as disabled.
  //
  mPpmPolicyConfiguration.Hwpm.HWPMNative    = 0;
  mPpmPolicyConfiguration.Hwpm.HWPMOOB       = 0;
  mPpmPolicyConfiguration.Hwpm.HWPMEnable    = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable;
  mPpmPolicyConfiguration.Hwpm.HWPMInterrupt = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMInterrupt;
  mPpmPolicyConfiguration.Hwpm.EPPEnable     = SetupData.SocketConfig.PowerManagementConfig.ProcessorEPPEnable;
  mPpmPolicyConfiguration.Hwpm.EPPProfile    = SetupData.SocketConfig.PowerManagementConfig.ProcessorEppProfile;

  if ((SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 1) ||
     (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 3)) {
    mPpmPolicyConfiguration.Hwpm.HWPMNative  = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable;
  }else if (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 2){
    mPpmPolicyConfiguration.Hwpm.HWPMOOB     = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable;
    mPpmPolicyConfiguration.Hwpm.HWPMInterrupt = 0;
  }else if (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == 0){
    mPpmPolicyConfiguration.Hwpm.HWPMNative    = 0;
    mPpmPolicyConfiguration.Hwpm.HWPMOOB       = 0;
    mPpmPolicyConfiguration.Hwpm.HWPMInterrupt = 0;
    mPpmPolicyConfiguration.Hwpm.EPPEnable     = 0;
  }

  mPpmPolicyConfiguration.Hwpm.APSrocketing          = SetupData.SocketConfig.PowerManagementConfig.ProcessorAPSrocketing;
  mPpmPolicyConfiguration.Hwpm.Scalability           = SetupData.SocketConfig.PowerManagementConfig.ProcessorScalability;
  mPpmPolicyConfiguration.Hwpm.RaplPrioritization             = SetupData.SocketConfig.PowerManagementConfig.ProcessorRaplPrioritization;
  mPpmPolicyConfiguration.Hwpm.OutofBandAlternateEPB = SetupData.SocketConfig.PowerManagementConfig.ProcessorOutofBandAlternateEPB;

  for (i = 0; i < NUM_CST_LAT_MSR; i++) {    // CStateLatencyCtrl CSRs
    mPpmPolicyConfiguration.PpmCst.LatencyCtrl[i].Valid      = SetupData.SocketConfig.PowerManagementConfig.CStateLatencyCtrlValid[i];
    mPpmPolicyConfiguration.PpmCst.LatencyCtrl[i].Multiplier = SetupData.SocketConfig.PowerManagementConfig.CStateLatencyCtrlMultiplier[i];
    mPpmPolicyConfiguration.PpmCst.LatencyCtrl[i].Value      = SetupData.SocketConfig.PowerManagementConfig.CStateLatencyCtrlValue[i];
  }

  if (SetupData.SocketConfig.PowerManagementConfig.C2C3TT) { //if option is not AUTO
    mPpmPolicyConfiguration.C2C3TT = SetupData.SocketConfig.PowerManagementConfig.C2C3TT;
  } else {
    mPpmPolicyConfiguration.C2C3TT = 0x10;
  }

  //
  // If Emulation flag set by InitializeDefaultData in MemoryQpiInit.c
  //  force X2APIC
  //
  Status = gBS->LocateProtocol (
                  &gEfiPlatformTypeProtocolGuid,
                  NULL,
                  &PlatformType
                  );
  ASSERT_EFI_ERROR (Status);

  // allocate memory for IedTrace
  if ((SetupData.SocketConfig.SocketProcessorCoreConfiguration.IedTraceSize != 0) && (PcdGet32 (PcdCpuIEDRamSize) != 0)) {
    DynamicSiLibraryProtocol->CheckAndPopulateIedTraceMemory(0x400000 << (SetupData.SocketConfig.SocketProcessorCoreConfiguration.IedTraceSize - 1), mIioUds);
  }

  //
  // Install CPU PPM Policy Protocol for platform PPM configuration
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gPpmPolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPpmPolicyConfiguration
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Install CPU Policy Protocol for platform configuration
  // Cpu Driver could be dispatched after this protocol installed.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiCpuPolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mCpuPolicyConfiguration
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
