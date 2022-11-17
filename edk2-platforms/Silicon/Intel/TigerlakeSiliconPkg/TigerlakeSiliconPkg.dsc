## @file
#  Component description file for the TigerLake silicon package DSC file.
#
#  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##
[Packages]
MdePkg/MdePkg.dec
UefiCpuPkg/UefiCpuPkg.dec
TigerlakeSiliconPkg/SiPkg.dec

[PcdsFixedAtBuild]
gSiPkgTokenSpaceGuid.PcdSmmVariableEnable            |TRUE
gSiPkgTokenSpaceGuid.PcdAtaEnable                    |FALSE

gSiPkgTokenSpaceGuid.PcdAcpiEnable                   |TRUE
gSiPkgTokenSpaceGuid.PcdSourceDebugEnable            |FALSE
gSiPkgTokenSpaceGuid.PcdPpmEnable                    |TRUE
gSiPkgTokenSpaceGuid.PcdPttEnable                    |FALSE
gSiPkgTokenSpaceGuid.PcdJhiEnable                    |FALSE
gSiPkgTokenSpaceGuid.PcdSmbiosEnable                 |TRUE
gSiPkgTokenSpaceGuid.PcdS3Enable                     |TRUE
gSiPkgTokenSpaceGuid.PcdOverclockEnable              |FALSE
gSiPkgTokenSpaceGuid.PcdCpuPowerOnConfigEnable       |FALSE
gSiPkgTokenSpaceGuid.PcdBdatEnable                   |TRUE
gSiPkgTokenSpaceGuid.PcdIgdEnable                    |TRUE
gSiPkgTokenSpaceGuid.PcdPegEnable                    |TRUE
gSiPkgTokenSpaceGuid.PcdSaDmiEnable                  |TRUE
gSiPkgTokenSpaceGuid.PcdVtdEnable                    |TRUE
gSiPkgTokenSpaceGuid.PcdBiosGuardEnable              |FALSE  #BiosGuardModule.bin
gSiPkgTokenSpaceGuid.PcdOptimizeCompilerEnable       |TRUE
gSiPkgTokenSpaceGuid.PcdPeiDisplayEnable             |TRUE
gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable         |FALSE
gSiPkgTokenSpaceGuid.PcdMrcTraceMessageSupported     |TRUE
gSiPkgTokenSpaceGuid.PcdOcWdtEnable                  |TRUE
gSiPkgTokenSpaceGuid.PcdSerialIoUartEnable           |TRUE

gSiPkgTokenSpaceGuid.PcdThcEnable                    |TRUE
gSiPkgTokenSpaceGuid.PcdPpamEnable                   |TRUE
gSiPkgTokenSpaceGuid.PcdEmbeddedEnable               |0x0
gSiPkgTokenSpaceGuid.PcdCpuPcieEnable                |TRUE
gSiPkgTokenSpaceGuid.PcdHybridStorageSupport         |TRUE

!if gSiPkgTokenSpaceGuid.PcdPpamEnable == TRUE
#
# PCD for State Save Support on DGR
# TRUE - SMM State Save region access is protected
# FALSE - SMM can have Read/Write access to SMM State Save region
#
gSiPkgTokenSpaceGuid.PcdSpsStateSaveEnable           |FALSE
#
# PCD to enable SPA Support on DGR
# Note: This PCD is mainly used for Debugging purpose. Not recommended to set for End Product.
#
gSiPkgTokenSpaceGuid.PcdSpaEnable                    |FALSE
!endif

[PcdsFixedAtBuild.common]
gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress       |0xC0000000
gSiPkgTokenSpaceGuid.PcdSiPciExpressBaseAddress         |gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress
gSiPkgTokenSpaceGuid.PcdTemporaryPciExpressRegionLength |0x10000000

[PcdsDynamicDefault.common]
gSiPkgTokenSpaceGuid.PcdPciExpressRegionLength          |0x10000000
## Specifies the AP wait loop state during POST phase.
#  The value is defined as below.
#  1: Place AP in the Hlt-Loop state.
#  2: Place AP in the Mwait-Loop state.
#  3: Place AP in the Run-Loop state.
# @Prompt The AP wait loop state.
gUefiCpuPkgTokenSpaceGuid.PcdCpuApLoopMode|2
## Specifies the AP target C-state for Mwait during POST phase.
#  The default value 0 means C1 state.
#  The value is defined as below.<BR><BR>
# @Prompt The specified AP target C-state for Mwait.
gUefiCpuPkgTokenSpaceGuid.PcdCpuApTargetCstate|0

[Defines]
  PLATFORM_NAME = TigerlakeSiliconPkg
  PLATFORM_GUID = CCD38CA7-61D3-4185-9CDA-A9FDF209CB31
  PLATFORM_VERSION = 0.4
  DSC_SPECIFICATION = 0x00010005
  OUTPUT_DIRECTORY = Build/TigerlakeSiliconPkg
  SUPPORTED_ARCHITECTURES = IA32|X64
  BUILD_TARGETS = DEBUG|RELEASE
  SKUID_IDENTIFIER = DEFAULT

  DEFINE   PLATFORM_SI_PACKAGE        = TigerlakeSiliconPkg
  #
  # Definition for Build Flag
  #
  !include $(PLATFORM_SI_PACKAGE)/SiPkgBuildOption.dsc

  !include MdePkg/MdeLibs.dsc.inc

[LibraryClasses.common]
  #
  # Entry point
  #
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf

  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciSegmentLib|$(PLATFORM_SI_PACKAGE)/Library/BasePciSegmentMultiSegLibPci/BasePciSegmentMultiSegLibPci.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  PostCodeLib|MdePkg/Library/BasePostCodeLibPort80/BasePostCodeLibPort80.inf

  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf

  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
  S3IoLib|MdePkg/Library/BaseS3IoLib/BaseS3IoLib.inf
  S3PciLib|MdePkg/Library/BaseS3PciLib/BaseS3PciLib.inf

  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf

  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

  #
  # Misc
  #
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
  MtrrLib|UefiCpuPkg/Library/MtrrLib/MtrrLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf

#####################################################################################################

#
# Silicon Init Common Library
#
!include $(PLATFORM_SI_PACKAGE)/SiPkgCommonLib.dsc
ConfigBlockLib|IntelSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf

[LibraryClasses.IA32]
#
# PEI phase common
#
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  PeiGetVtdPmrAlignmentLib|IntelSiliconPkg/Library/PeiGetVtdPmrAlignmentLib/PeiGetVtdPmrAlignmentLib.inf

#####################################################################################################################################

#
# Silicon Init Pei Library
#
!include $(PLATFORM_SI_PACKAGE)/SiPkgPeiLib.dsc

[LibraryClasses.IA32.SEC]
  GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/BaseGpioHelpersLibNull/BaseGpioHelpersLibNull.inf

[LibraryClasses.X64]
 #
 # DXE phase common
 #
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf

#
# Hsti
#
  HstiLib|MdePkg/Library/DxeHstiLib/DxeHstiLib.inf

###################################################################################################
#
# Silicon Init Dxe Library
#
!include $(PLATFORM_SI_PACKAGE)/SiPkgDxeLib.dsc

[LibraryClasses.X64.PEIM]

[LibraryClasses.X64.DXE_CORE]
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

[LibraryClasses.X64.DXE_SMM_DRIVER]
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf
  SmmIoLib|MdePkg/Library/SmmIoLib/SmmIoLib.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf

[LibraryClasses.X64.SMM_CORE]

[LibraryClasses.X64.UEFI_DRIVER]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

[LibraryClasses.X64.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf

[Components.IA32]
!include $(PLATFORM_SI_PACKAGE)/SiPkgPei.dsc

[Components.X64]
!include $(PLATFORM_SI_PACKAGE)/SiPkgDxe.dsc
