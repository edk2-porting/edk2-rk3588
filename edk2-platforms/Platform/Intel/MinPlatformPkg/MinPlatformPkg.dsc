## @file
#  Platform description.
#
# Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
# Copyright (c) Microsoft Corporation.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = MinPlatformPkg
  PLATFORM_GUID                       = 8FE55D15-3ABF-4175-8169-74B87E5CD175
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/MinPlatformPkg
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = DEFAULT

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsFeatureFlag]
  # configuration
    gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
    gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|FALSE
    gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|FALSE
    gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable|FALSE
    gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable|FALSE
    gMinPlatformPkgTokenSpaceGuid.PcdSerialTerminalEnable|FALSE

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################

!include MinPlatformPkg/Include/Dsc/CoreCommonLib.dsc
!include MinPlatformPkg/Include/Dsc/CorePeiLib.dsc
!include MinPlatformPkg/Include/Dsc/CoreDxeLib.dsc

[LibraryClasses.common]
  #
  # Platform
  #
  PlatformHookLib|MdeModulePkg/Library/BasePlatformHookLibNull/BasePlatformHookLibNull.inf
  PciHostBridgeLib|MinPlatformPkg/Pci/Library/PciHostBridgeLibSimple/PciHostBridgeLibSimple.inf
  PciSegmentInfoLib|MinPlatformPkg/Pci/Library/PciSegmentInfoLibSimple/PciSegmentInfoLibSimple.inf
  PlatformBootManagerLib|MinPlatformPkg/Bds/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf
  AslUpdateLib|MinPlatformPkg/Acpi/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf

  #
  # Misc
  #
  FspWrapperApiLib|IntelFsp2WrapperPkg/Library/BaseFspWrapperApiLib/BaseFspWrapperApiLib.inf
  FspWrapperApiTestLib|IntelFsp2WrapperPkg/Library/PeiFspWrapperApiTestLib/PeiFspWrapperApiTestLib.inf
  FspWrapperHobProcessLib|MinPlatformPkg/FspWrapper/Library/PeiFspWrapperHobProcessLib/PeiFspWrapperHobProcessLib.inf
  PlatformSecLib|MinPlatformPkg/FspWrapper/Library/SecFspWrapperPlatformSecLib/SecFspWrapperPlatformSecLib.inf
  VariableReadLib|MinPlatformPkg/Library/BaseVariableReadLibNull/BaseVariableReadLibNull.inf
  FspWrapperPlatformLib|MinPlatformPkg/FspWrapper/Library/PeiFspWrapperPlatformLib/PeiFspWrapperPlatformLib.inf

  BoardInitLib|MinPlatformPkg/PlatformInit/Library/BoardInitLibNull/BoardInitLibNull.inf
  BoardAcpiTableLib|MinPlatformPkg/Acpi/Library/BoardAcpiTableLibNull/BoardAcpiTableLibNull.inf
  BoardAcpiEnableLib|MinPlatformPkg/Acpi/Library/BoardAcpiEnableLibNull/BoardAcpiEnableLibNull.inf
  SiliconPolicyInitLib|MinPlatformPkg/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
  SiliconPolicyUpdateLib|MinPlatformPkg/PlatformInit/Library/SiliconPolicyUpdateLibNull/SiliconPolicyUpdateLibNull.inf

  TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf

[LibraryClasses.common.SEC]
  TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLib/SecTestPointCheckLib.inf

[LibraryClasses.common.PEIM]
  #
  # PEI phase common
  #
  FspWrapperPlatformLib|MinPlatformPkg/FspWrapper/Library/PeiFspWrapperPlatformLib/PeiFspWrapperPlatformLib.inf
  ReportFvLib|MinPlatformPkg/PlatformInit/Library/PeiReportFvLib/PeiReportFvLib.inf
  ReportCpuHobLib|IntelSiliconPkg/Library/ReportCpuHobLib/ReportCpuHobLib.inf
  TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLib/PeiTestPointCheckLib.inf
  TestPointLib|MinPlatformPkg/Test/Library/TestPointLib/PeiTestPointLib.inf
  SetCacheMtrrLib|MinPlatformPkg/Library/SetCacheMtrrLib/SetCacheMtrrLibNull.inf

[LibraryClasses.common.DXE_DRIVER]
  #
  # DXE phase common
  #
  FspWrapperPlatformLib|MinPlatformPkg/FspWrapper/Library/DxeFspWrapperPlatformLib/DxeFspWrapperPlatformLib.inf
  TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLib/DxeTestPointCheckLib.inf
  TestPointLib|MinPlatformPkg/Test/Library/TestPointLib/DxeTestPointLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  SpiFlashCommonLib|MinPlatformPkg/Flash/Library/SpiFlashCommonLibNull/SpiFlashCommonLibNull.inf
  TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLib/SmmTestPointCheckLib.inf
  TestPointLib|MinPlatformPkg/Test/Library/TestPointLib/SmmTestPointLib.inf

[LibraryClasses.common.MM_STANDALONE]
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  MemoryAllocationLib|StandaloneMmPkg/Library/StandaloneMmMemoryAllocationLib/StandaloneMmMemoryAllocationLib.inf
  MmServicesTableLib|MdePkg/Library/StandaloneMmServicesTableLib/StandaloneMmServicesTableLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  SpiFlashCommonLib|MinPlatformPkg/Flash/Library/SpiFlashCommonLibNull/SpiFlashCommonLibNull.inf
  StandaloneMmDriverEntryPoint|MdePkg/Library/StandaloneMmDriverEntryPoint/StandaloneMmDriverEntryPoint.inf
  VariableReadLib|MinPlatformPkg/Library/SmmVariableReadLib/StandaloneMmVariableReadLib.inf
  VariableWriteLib|MinPlatformPkg/Library/SmmVariableWriteLib/StandaloneMmVariableWriteLib.inf

###################################################################################################
#
# Components Section - list of the modules and components that will be processed by compilation
#                      tools and the EDK II tools to generate PE32/PE32+/Coff image files.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       Binary modules do not need to be listed in this section, as they should be
#       specified in the FDF file. For example: Shell binary (Shell_Full.efi), FAT binary (Fat.efi),
#       Logo (Logo.bmp), and etc.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
###################################################################################################

[Components]

  MinPlatformPkg/Library/PeiLib/PeiLib.inf
  MinPlatformPkg/Library/PeiHobVariableLibFce/PeiHobVariableLibFce.inf
  MinPlatformPkg/Library/PeiHobVariableLibFce/PeiHobVariableLibFceOptSize.inf

  MinPlatformPkg/Acpi/AcpiTables/AcpiPlatform.inf
  MinPlatformPkg/Acpi/AcpiSmm/AcpiSmm.inf
  MinPlatformPkg/Acpi/AcpiSmm/AcpiStandaloneMm.inf
  MinPlatformPkg/Acpi/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf
  MinPlatformPkg/Acpi/Library/BoardAcpiEnableLibNull/BoardAcpiEnableLibNull.inf
  MinPlatformPkg/Acpi/Library/BoardAcpiTableLibNull/BoardAcpiTableLibNull.inf
  MinPlatformPkg/Acpi/Library/MultiBoardAcpiSupportLib/DxeMultiBoardAcpiSupportLib.inf
  MinPlatformPkg/Acpi/Library/MultiBoardAcpiSupportLib/SmmMultiBoardAcpiSupportLib.inf
  MinPlatformPkg/Acpi/MinDsdt/MinDsdt.inf

  MinPlatformPkg/Bds/Library/BoardBootManagerLibNull/BoardBootManagerLibNull.inf
  MinPlatformPkg/Bds/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf

  MinPlatformPkg/Flash/SpiFvbService/SpiFvbServiceSmm.inf
  MinPlatformPkg/Flash/SpiFvbService/SpiFvbServiceStandaloneMm.inf
  MinPlatformPkg/Flash/Library/SpiFlashCommonLibNull/SpiFlashCommonLibNull.inf

  MinPlatformPkg/FspWrapper/SaveMemoryConfig/SaveMemoryConfig.inf
  MinPlatformPkg/FspWrapper/Library/PeiFspWrapperHobProcessLib/PeiFspWrapperHobProcessLib.inf
  MinPlatformPkg/FspWrapper/Library/SecFspWrapperPlatformSecLib/SecFspWrapperPlatformSecLib.inf
  MinPlatformPkg/FspWrapper/Library/PeiFspWrapperPlatformLib/PeiFspWrapperPlatformLib.inf
  MinPlatformPkg/FspWrapper/Library/DxeFspWrapperPlatformLib/DxeFspWrapperPlatformLib.inf

  MinPlatformPkg/Library/CompressLib/CompressLib.inf
  MinPlatformPkg/Library/SetCacheMtrrLib/SetCacheMtrrLib.inf
  MinPlatformPkg/Library/SetCacheMtrrLib/SetCacheMtrrLibNull.inf
  MinPlatformPkg/Library/SerialPortTerminalLib/SerialPortTerminalLib.inf

  MinPlatformPkg/Hsti/HstiIbvPlatformDxe/HstiIbvPlatformDxe.inf

  MinPlatformPkg/Pci/Library/PciHostBridgeLibSimple/PciHostBridgeLibSimple.inf
  MinPlatformPkg/Pci/Library/PciSegmentInfoLibSimple/PciSegmentInfoLibSimple.inf

  MinPlatformPkg/PlatformInit/ReportFv/ReportFvPei.inf
  MinPlatformPkg/PlatformInit/PlatformInitPei/PlatformInitPreMem.inf
  MinPlatformPkg/PlatformInit/PlatformInitPei/PlatformInitPostMem.inf
  MinPlatformPkg/PlatformInit/PlatformInitDxe/PlatformInitDxe.inf
  MinPlatformPkg/PlatformInit/PlatformInitSmm/PlatformInitSmm.inf
  MinPlatformPkg/PlatformInit/Library/SecBoardInitLibNull/SecBoardInitLibNull.inf
  MinPlatformPkg/PlatformInit/Library/BoardInitLibNull/BoardInitLibNull.inf
  MinPlatformPkg/PlatformInit/Library/MultiBoardInitSupportLib/PeiMultiBoardInitSupportLib.inf
  MinPlatformPkg/PlatformInit/Library/MultiBoardInitSupportLib/DxeMultiBoardInitSupportLib.inf
  MinPlatformPkg/PlatformInit/Library/PeiReportFvLib/PeiReportFvLib.inf
  MinPlatformPkg/PlatformInit/Library/ReportCpuHobLib/ReportCpuHobLib.inf
  MinPlatformPkg/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPreMem.inf
  MinPlatformPkg/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPostMem.inf
  MinPlatformPkg/PlatformInit/SiliconPolicyDxe/SiliconPolicyDxe.inf
  MinPlatformPkg/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
  MinPlatformPkg/PlatformInit/Library/SiliconPolicyUpdateLibNull/SiliconPolicyUpdateLibNull.inf

  MinPlatformPkg/Services/StallServicePei/StallServicePei.inf

  MinPlatformPkg/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
  MinPlatformPkg/Test/Library/TestPointCheckLib/SecTestPointCheckLib.inf
  MinPlatformPkg/Test/Library/TestPointCheckLib/PeiTestPointCheckLib.inf
  MinPlatformPkg/Test/Library/TestPointCheckLib/DxeTestPointCheckLib.inf
  MinPlatformPkg/Test/Library/TestPointCheckLib/SmmTestPointCheckLib.inf
  MinPlatformPkg/Test/Library/TestPointLib/DxeTestPointLib.inf
  MinPlatformPkg/Test/Library/TestPointLib/PeiTestPointLib.inf
  MinPlatformPkg/Test/Library/TestPointLib/SmmTestPointLib.inf
  MinPlatformPkg/Test/TestPointStubDxe/TestPointStubDxe.inf
  MinPlatformPkg/Test/TestPointDumpApp/TestPointDumpApp.inf

  MinPlatformPkg/Tcg/Library/PeiDxeTpmPlatformHierarchyLib/PeiDxeTpmPlatformHierarchyLib.inf
  MinPlatformPkg/Tcg/Tcg2PlatformPei/Tcg2PlatformPei.inf
  MinPlatformPkg/Tcg/Tcg2PlatformDxe/Tcg2PlatformDxe.inf

  MinPlatformPkg/Library/BaseVariableReadLibNull/BaseVariableReadLibNull.inf
  MinPlatformPkg/Library/SmmVariableReadLib/StandaloneMmVariableReadLib.inf
  MinPlatformPkg/Library/SmmVariableWriteLib/StandaloneMmVariableWriteLib.inf

[BuildOptions]
  *_*_*_CC_FLAGS = -D DISABLE_NEW_DEPRECATED_INTERFACES
