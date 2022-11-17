/** @file
  Platform Stage System Agent initialization.


  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiSaPolicyUpdate.h"
#include <CpuRegs.h>
#include <Register/Cpuid.h>
#include <Library/CpuPlatformLib.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Library/HobLib.h>
#include <Platform.h>
#include <PlatformBoardConfig.h>
#include <Library/SiPolicyLib.h>

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/GpioLib.h>
#include <Library/ConfigBlockLib.h>

///
/// Memory Reserved should be between 125% to 150% of the Current required memory
/// otherwise BdsMisc.c would do a reset to make it 125% to avoid s4 resume issues.
///
GLOBAL_REMOVE_IF_UNREFERENCED STATIC EFI_MEMORY_TYPE_INFORMATION mDefaultMemoryTypeInformation[] = {
  { EfiACPIReclaimMemory,   FixedPcdGet32 (PcdPlatformEfiAcpiReclaimMemorySize) },  // ASL
  { EfiACPIMemoryNVS,       FixedPcdGet32 (PcdPlatformEfiAcpiNvsMemorySize) },      // ACPI NVS (including S3 related)
  { EfiReservedMemoryType,  FixedPcdGet32 (PcdPlatformEfiReservedMemorySize) },     // BIOS Reserved (including S3 related)
  { EfiRuntimeServicesData, FixedPcdGet32 (PcdPlatformEfiRtDataMemorySize) },       // Runtime Service Data
  { EfiRuntimeServicesCode, FixedPcdGet32 (PcdPlatformEfiRtCodeMemorySize) },       // Runtime Service Code
  { EfiMaxMemoryType, 0 }
};

/**
  UpdatePeiSaPolicyPreMem performs SA PEI Policy initialization

  @param[in out] SiPreMemPolicyPpi - SI_PREMEM_POLICY PPI

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
UpdatePeiSaPolicyPreMem (
  IN OUT   SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  SA_MISC_PEI_PREMEM_CONFIG       *MiscPeiPreMemConfig = NULL;
  MEMORY_CONFIG_NO_CRC            *MemConfigNoCrc = NULL;
  SA_MEMORY_RCOMP                 *RcompData;
  WDT_PPI                         *gWdtPei;
  UINT8                           Index;
  UINTN                           DataSize;
  EFI_MEMORY_TYPE_INFORMATION     MemoryData[EfiMaxMemoryType + 1];
  EFI_BOOT_MODE                   BootMode;
  UINT32                          TraceHubTotalMemSize;
  GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig = NULL;
  MEMORY_CONFIGURATION            *MemConfig = NULL;
  PCIE_PEI_PREMEM_CONFIG          *PciePeiPreMemConfig = NULL;
  SWITCHABLE_GRAPHICS_CONFIG      *SgGpioData = NULL;
  IPU_PREMEM_CONFIG               *IpuPreMemPolicy = NULL;
  OVERCLOCKING_PREMEM_CONFIG      *OcPreMemConfig = NULL;
  VTD_CONFIG                      *Vtd = NULL;
  UINT32                          ProcessorTraceTotalMemSize;

  TraceHubTotalMemSize = 0;
  ProcessorTraceTotalMemSize = 0;

  DEBUG((DEBUG_INFO, "Entering Get Config Block function call from UpdatePeiSaPolicyPreMem\n"));

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &GtPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gSaPciePeiPreMemConfigGuid, (VOID *) &PciePeiPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gSwitchableGraphicsConfigGuid, (VOID *) &SgGpioData);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gIpuPreMemConfigGuid, (VOID *) &IpuPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gSaOverclockingPreMemConfigGuid, (VOID *) &OcPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *)&Vtd);
  ASSERT_EFI_ERROR(Status);

  RcompData = MemConfigNoCrc->RcompData;

  //
  // Locate WDT_PPI (ICC WDT PPI)
  //
  gWdtPei = NULL;
  Status = PeiServicesLocatePpi(
             &gWdtPpiGuid,
             0,
             NULL,
             (VOID **) &gWdtPei
             );

  Status = PeiServicesGetBootMode(&BootMode);
  ASSERT_EFI_ERROR(Status);

  MiscPeiPreMemConfig->S3DataPtr = NULL;
  MiscPeiPreMemConfig->UserBd = 0; // It's a CRB mobile board by default (btCRBMB)

  PcdSetBoolS (PcdMobileDramPresent, (BOOLEAN) (MemConfig->MobilePlatform));
  MiscPeiPreMemConfig->SpdAddressTable[0] = PcdGet8 (PcdMrcSpdAddressTable0);
  MiscPeiPreMemConfig->SpdAddressTable[1] = PcdGet8 (PcdMrcSpdAddressTable1);
  MiscPeiPreMemConfig->SpdAddressTable[2] = PcdGet8 (PcdMrcSpdAddressTable2);
  MiscPeiPreMemConfig->SpdAddressTable[3] = PcdGet8 (PcdMrcSpdAddressTable3);
  MemConfig->CaVrefConfig                 = PcdGet8 (PcdMrcCaVrefConfig);
  MemConfig->DualDimmPerChannelBoardType  = PcdGetBool (PcdDualDimmPerChannelBoardType);
  if (PcdGet32 (PcdMrcRcompResistor)) {
    CopyMem((VOID *)RcompData->RcompResistor, (VOID *) (UINTN) PcdGet32 (PcdMrcRcompResistor), sizeof (RcompData->RcompResistor));
  }
  if (PcdGet32 (PcdMrcRcompTarget)) {
    CopyMem((VOID *)RcompData->RcompTarget, (VOID *) (UINTN) PcdGet32 (PcdMrcRcompTarget), sizeof (RcompData->RcompTarget));
  }
  if (PcdGet32 (PcdMrcDqByteMap)) {
    CopyMem((VOID *)MemConfigNoCrc->DqByteMap, (VOID *) (UINTN) PcdGet32 (PcdMrcDqByteMap), sizeof (UINT8)* SA_MC_MAX_CHANNELS * SA_MRC_ITERATION_MAX * 2);
  }
  if (PcdGet32 (PcdMrcDqsMapCpu2Dram)) {
    CopyMem((VOID *)MemConfigNoCrc->DqsMap, (VOID *) (UINTN) PcdGet32 (PcdMrcDqsMapCpu2Dram), sizeof (UINT8)* SA_MC_MAX_CHANNELS * SA_MC_MAX_BYTES_NO_ECC);
  }
  if (PcdGetBool (PcdMrcDqPinsInterleavedControl)) {
    MemConfig->DqPinsInterleaved = PcdGetBool (PcdMrcDqPinsInterleaved);
  }
  if (PcdGet32 (PcdMrcSpdData)) {
    CopyMem((VOID *)MemConfigNoCrc->SpdData->SpdData[0][0], (VOID *) (UINTN) PcdGet32 (PcdMrcSpdData), SPD_DATA_SIZE);
    CopyMem((VOID *)MemConfigNoCrc->SpdData->SpdData[1][0], (VOID *) (UINTN) PcdGet32 (PcdMrcSpdData), SPD_DATA_SIZE);
  }

  MiscPeiPreMemConfig->MchBar   = (UINTN) PcdGet64 (PcdMchBaseAddress);
  MiscPeiPreMemConfig->DmiBar   = (UINTN) PcdGet64 (PcdDmiBaseAddress);
  MiscPeiPreMemConfig->EpBar    = (UINTN) PcdGet64 (PcdEpBaseAddress);
  MiscPeiPreMemConfig->EdramBar = (UINTN) PcdGet64 (PcdEdramBaseAddress);
  MiscPeiPreMemConfig->SmbusBar = PcdGet16(PcdSmbusBaseAddress);
  MiscPeiPreMemConfig->TsegSize = PcdGet32(PcdTsegSize);
  MiscPeiPreMemConfig->UserBd   = PcdGet8 (PcdSaMiscUserBd);
  MiscPeiPreMemConfig->MmioSizeAdjustment = PcdGet16 (PcdSaMiscMmioSizeAdjustment);
  if (PcdGetBool (PcdPegGpioResetControl)) {
    PciePeiPreMemConfig->PegGpioData.GpioSupport = PcdGetBool (PcdPegGpioResetSupoort);
  } else {

  }
  PciePeiPreMemConfig->PegGpioData.SaPeg0ResetGpio.GpioPad = PcdGet32 (PcdPeg0ResetGpioPad);
  PciePeiPreMemConfig->PegGpioData.SaPeg0ResetGpio.Active  = PcdGetBool (PcdPeg0ResetGpioActive);

  PciePeiPreMemConfig->PegGpioData.SaPeg3ResetGpio.GpioPad = PcdGet32 (PcdPeg3ResetGpioPad);
  PciePeiPreMemConfig->PegGpioData.SaPeg3ResetGpio.Active  = PcdGetBool (PcdPeg3ResetGpioActive);

  MemConfig->CkeRankMapping = 0xAA;
  ///
  /// Initialize the VTD Configuration
  ///
  Vtd->VtdDisable = 0;

  MemConfig->RMT = 1;
  MemConfig->UserPowerWeightsEn = 0;
  MemConfig->RaplLim2WindY = 0x0A;
  MemConfig->ExitOnFailure = 1;

  MemConfigNoCrc->PlatformMemorySize = PEI_MIN_MEMORY_SIZE + TraceHubTotalMemSize + ProcessorTraceTotalMemSize;
  DataSize = sizeof (mDefaultMemoryTypeInformation);
  CopyMem(MemoryData, mDefaultMemoryTypeInformation, DataSize);

  if (BootMode != BOOT_IN_RECOVERY_MODE) {
    for (Index = 0; Index < DataSize / sizeof (EFI_MEMORY_TYPE_INFORMATION); Index++) {
      MemConfigNoCrc->PlatformMemorySize += MemoryData[Index].NumberOfPages * EFI_PAGE_SIZE;
    }

    OcPreMemConfig->GtMaxOcRatio = 0;
    OcPreMemConfig->GtVoltageMode = 0;
    OcPreMemConfig->GtVoltageOverride = 0;
    OcPreMemConfig->GtExtraTurboVoltage = 0;
    OcPreMemConfig->GtVoltageOffset = 0;
    OcPreMemConfig->SaVoltageOffset = 0;
    OcPreMemConfig->GtusMaxOcRatio = 0;
    OcPreMemConfig->GtusVoltageMode = 0;
    OcPreMemConfig->GtusVoltageOverride = 0;
    OcPreMemConfig->GtusExtraTurboVoltage = 0;
    OcPreMemConfig->GtusVoltageOffset = 0;

    ///
    /// Build the GUID'd HOB for DXE
    ///
    BuildGuidDataHob (
      &gEfiMemoryTypeInformationGuid,
      MemoryData,
      DataSize
      );
  }

  return EFI_SUCCESS;
}


