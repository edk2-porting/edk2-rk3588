## @file
#  The main build description file for the MtOlympus board.
#
# Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
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
  DEFINE PLATFORM_PACKAGE           = MinPlatformPkg
  DEFINE PLATFORM_SI_PACKAGE        = PurleyRefreshSiliconPkg
  DEFINE PLATFORM_SI_BIN_PACKAGE    = PurleySiliconBinPkg
  DEFINE PLATFORM_BOARD_PACKAGE     = PurleyOpenBoardPkg
  DEFINE BOARD                      = BoardMtOlympus
  DEFINE PROJECT                    = $(PLATFORM_BOARD_PACKAGE)/$(BOARD)
  DEFINE PEI_ARCH                   = IA32
  DEFINE DXE_ARCH                   = X64

  PLATFORM_NAME                       = PurleyOpenBoardPkg
  PLATFORM_GUID                       = D7EAF54D-C9B9-4075-89F0-71943DBCFA61
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/$(PROJECT)
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = DEFAULT
  FLASH_DEFINITION                    = $(PROJECT)/OpenBoardPkg.fdf

  FIX_LOAD_TOP_MEMORY_ADDRESS         = 0x0

  #
  # Platform On/Off features are defined here
  #
  !include $(PROJECT)/PlatformPkgConfig.dsc

  #
  # Include PCD configuration for this board.
  #
  !include AdvancedFeaturePkg/Include/AdvancedFeaturesPcd.dsc

  !include PlatformPkgPcd.dsc
  !include AdvancedFeaturePkg/Include/AdvancedFeatures.dsc


################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

[DefaultStores]
  0|STANDARD             # UEFI Standard default  0|STANDARD is reserved.
  1|MANUFACTURING        # UEFI Manufacturing default 1|MANUFACTURING is reserved.

################################################################################
#
# Includes section - other DSC file contents included for this board build.
#
################################################################################

#######################################
# Library Includes
#######################################
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreCommonLib.dsc
!include $(PLATFORM_PACKAGE)/Include/Dsc/CorePeiLib.dsc
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreDxeLib.dsc
!include $(PLATFORM_SI_PACKAGE)/SiPkgCommonLib.dsc
!include $(PLATFORM_SI_PACKAGE)/SiPkgPeiLib.dsc
!include $(PLATFORM_SI_PACKAGE)/SiPkgDxeLib.dsc

#######################################
# Component Includes
#######################################

[Components.$(PEI_ARCH)]
!include $(PLATFORM_PACKAGE)/Include/Dsc/CorePeiInclude.dsc

[Components.$(DXE_ARCH)]
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreDxeInclude.dsc

#######################################
# Build Option Includes
#######################################
!include $(PROJECT)/PlatformPkgBuildOption.dsc

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################

[LibraryClasses.common]
!if gPlatformTokenSpaceGuid.PcdFastBoot == FALSE
  PlatformBootManagerLib|$(PLATFORM_PACKAGE)/Bds/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf
!else
  PlatformBootManagerLib|$(PLATFORM_BOARD_PACKAGE)/Override/Platform/Intel/MinPlatformPkg/Bds/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf
!endif

  ReportFvLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/PeiReportFvLib/PeiReportFvLib.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf

  CompressLib|$(PLATFORM_PACKAGE)/Library/CompressLib/CompressLib.inf

  PciSegmentInfoLib|$(PLATFORM_PACKAGE)/Pci/Library/PciSegmentInfoLibSimple/PciSegmentInfoLibSimple.inf
  AslUpdateLib|$(PLATFORM_PACKAGE)/Acpi/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf

  #
  # Board
  #
  SiliconPolicyInitLib|$(PLATFORM_BOARD_PACKAGE)/Policy/Library/SiliconPolicyInitLib/SiliconPolicyInitLib.inf
  SiliconPolicyUpdateLib|$(PLATFORM_BOARD_PACKAGE)/Policy/Library/SiliconPolicyUpdateLib/SiliconPolicyUpdateLib.inf
  PlatformHookLib|$(PROJECT)/Library/BasePlatformHookLib/BasePlatformHookLib.inf
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/BoardInitLibNull/BoardInitLibNull.inf

[LibraryClasses.IA32]
!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/PeiTestPointCheckLib.inf
!endif
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/PeiTestPointLib.inf
  SetCacheMtrrLib|$(PLATFORM_PACKAGE)/Library/SetCacheMtrrLib/SetCacheMtrrLibNull.inf
  ReportCpuHobLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/ReportCpuHobLib/ReportCpuHobLib.inf

[LibraryClasses.X64]
  BoardAcpiTableLib|$(PROJECT)/Library/BoardAcpiLib/DxeBoardAcpiTableLib.inf
!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/DxeTestPointCheckLib.inf
!endif
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/DxeTestPointLib.inf
  BoardBootManagerLib|BoardModulePkg/Library/BoardBootManagerLib/BoardBootManagerLib.inf

[LibraryClasses.X64.DXE_SMM_DRIVER]
!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/SmmTestPointCheckLib.inf
!endif
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/SmmTestPointLib.inf

[Components.IA32]

  $(PLATFORM_SI_BIN_PACKAGE)/FV/FvTempMemorySilicon/$(TARGET)/FvTempMemorySilicon.inf
  $(PLATFORM_SI_BIN_PACKAGE)/FV/FvPreMemorySilicon/$(TARGET)/FvPreMemorySilicon.inf
  $(PLATFORM_SI_BIN_PACKAGE)/FV/FvPostMemorySilicon/$(TARGET)/FvPostMemorySilicon.inf

  $(PLATFORM_BOARD_PACKAGE)/Policy/SystemBoard/SystemBoardPei.inf

  $(PLATFORM_PACKAGE)/PlatformInit/ReportFv/ReportFvPei.inf
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPreMem.inf {
    <LibraryClasses>
      BoardInitLib|$(PROJECT)/Library/BoardInitLib/PeiBoardInitPreMemLib.inf
  }
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPostMem.inf {
    <LibraryClasses>
      BoardInitLib|$(PROJECT)/Library/BoardInitLib/PeiBoardInitPostMemLib.inf
  }
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPreMem.inf
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPostMem.inf

[Components.X64]

  $(PLATFORM_SI_BIN_PACKAGE)/FV/FvLateSilicon/$(TARGET)/FvLateSilicon.inf
  $(PLATFORM_SI_BIN_PACKAGE)/Microcode/Microcode.inf

  $(PLATFORM_BOARD_PACKAGE)/Policy/IioUdsDataDxe/IioUdsDataDxe.inf
  $(PLATFORM_BOARD_PACKAGE)/Policy/PlatformCpuPolicy/PlatformCpuPolicy.inf
  $(PLATFORM_BOARD_PACKAGE)/Pci/PciPlatform/PciPlatform.inf
  $(PLATFORM_BOARD_PACKAGE)/Acpi/BoardAcpiDxe/BoardAcpiDxe.inf {
    <BuildOptions>
        MSFT:*_*_*_ASLCC_FLAGS = /D PURLEY_FLAG /D PCH_SPT
        GCC:*_*_*_ASLCC_FLAGS  = -D PURLEY_FLAG -D PCH_SPT
  }

# This is for prebuild only. No need to include in final FDF.
  $(PLATFORM_BOARD_PACKAGE)/Acpi/BoardAcpiDxe/Dsdt.inf {
    <BuildOptions>
        MSFT:*_*_*_ASLCC_FLAGS = /D PURLEY_FLAG /D PCH_SPT
        GCC:*_*_*_ASLCC_FLAGS  = -D PURLEY_FLAG -D PCH_SPT
  }

  $(PLATFORM_PACKAGE)/Flash/SpiFvbService/SpiFvbServiceSmm.inf
  $(PLATFORM_PACKAGE)/Acpi/AcpiTables/AcpiPlatform.inf
  $(PLATFORM_PACKAGE)/Acpi/AcpiSmm/AcpiSmm.inf {
    <LibraryClasses>
      BoardAcpiEnableLib|$(PROJECT)/Library/BoardAcpiLib/SmmBoardAcpiEnableLib.inf
  }

  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitDxe/PlatformInitDxe.inf
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitSmm/PlatformInitSmm.inf

  $(PLATFORM_BOARD_PACKAGE)/Policy/S3NvramSave/S3NvramSave.inf

  #
  # Shell
  #
  ShellPkg/Application/Shell/Shell.inf {
   <PcdsFixedAtBuild>
     gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
   <LibraryClasses>
     NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
     NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
     NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
     NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
     NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
     NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
     NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
     NULL|ShellPkg/Library/UefiShellNetwork2CommandsLib/UefiShellNetwork2CommandsLib.inf
     ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
     HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
     BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
     ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
     ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  }

  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
