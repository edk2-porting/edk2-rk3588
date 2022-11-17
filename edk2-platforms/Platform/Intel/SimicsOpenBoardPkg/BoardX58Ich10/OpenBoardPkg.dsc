## @file
#  The main build description file for the X58Ich10 board.
#
# Copyright (c) 2019 - 2021, Intel Corporation. All rights reserved.<BR>
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
  DEFINE PLATFORM_PACKAGE      = MinPlatformPkg
  DEFINE BOARD_NAME            = BoardX58Ich10
  DEFINE BOARD_PKG             = SimicsOpenBoardPkg
  DEFINE SKT_PKG               = SimicsX58SktPkg
  DEFINE PCH_PKG               = SimicsIch10Pkg
  DEFINE DXE_ARCH              = X64
  DEFINE PEI_ARCH              = IA32
  DEFINE PROJECT               = $(BOARD_PKG)/$(BOARD_NAME)

  PLATFORM_NAME                  = SimicsX58
  PLATFORM_GUID                  = EE8EBB5A-CC95-412f-9987-2AF70F88B69A
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/$(PROJECT)
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = $(PROJECT)/OpenBoardPkg.fdf

  DEFINE SMM_REQUIRE             = TRUE

  #
  # PLATFORMX64_ENABLE is set to TRUE when PEI is IA32 and DXE is X64 platform
  #
  DEFINE PLATFORMX64_ENABLE             = TRUE
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE
  DEFINE NETWORK_ALLOW_HTTP_CONNECTIONS = TRUE

  !include AdvancedFeaturePkg/Include/AdvancedFeaturesPcd.dsc

  !include $(PROJECT)/OpenBoardPkgPcd.dsc
  !include AdvancedFeaturePkg/Include/AdvancedFeatures.dsc

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this board.
#
################################################################################
[SkuIds]
  0|DEFAULT

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
!include $(PCH_PKG)/IchCommonLib.dsc

#######################################
# Component Includes
#######################################
# @todo: Change below line to [Components.$(PEI_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed
[Components.IA32]
!include $(PLATFORM_PACKAGE)/Include/Dsc/CorePeiInclude.dsc
!include $(SKT_PKG)/SktPkgPei.dsc

# @todo: Change below line to [Components.$(DXE_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed
[Components.X64]
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreDxeInclude.dsc

#######################################
# Build Option Includes
#######################################
!include $(PROJECT)/OpenBoardPkgBuildOption.dsc

################################################################################
#
# Library Class section - list of all Library Classes needed by this board.
#
################################################################################

[LibraryClasses]
  #######################################
  # Edk2 Packages
  #######################################
  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf
  CpuExceptionHandlerLib|MdeModulePkg/Library/CpuExceptionHandlerLibNull/CpuExceptionHandlerLibNull.inf
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  SerialPortLib|PcAtChipsetPkg/Library/SerialIoLib/SerialIoLib.inf

  #####################################
  # Platform Package
  #####################################
  AslUpdateLib|$(PLATFORM_PACKAGE)/Acpi/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/BoardInitLibNull/BoardInitLibNull.inf
  PciSegmentInfoLib|$(PLATFORM_PACKAGE)/Pci/Library/PciSegmentInfoLibSimple/PciSegmentInfoLibSimple.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf

  #######################################
  # Board Package
  #######################################
  DxeLoadLinuxLib|$(BOARD_PKG)/Library/LoadLinuxLib/DxeLoadLinuxLib.inf
  LogoLib|$(BOARD_PKG)/Library/DxeLogoLib/DxeLogoLib.inf
  NvVarsFileLib|$(BOARD_PKG)/Library/NvVarsFileLib/NvVarsFileLib.inf
  ReportFvLib|$(BOARD_PKG)/Library/PeiReportFvLib/PeiReportFvLib.inf
  SerializeVariablesLib|$(BOARD_PKG)/Library/SerializeVariablesLib/SerializeVariablesLib.inf
  SiliconPolicyInitLib|$(BOARD_PKG)/Policy/Library/SiliconPolicyInitLib/SiliconPolicyInitLib.inf
  SiliconPolicyUpdateLib|$(BOARD_PKG)/Policy/Library/SiliconPolicyUpdateLib/SiliconPolicyUpdateLib.inf
  PlatformCmosAccessLib|BoardModulePkg/Library/PlatformCmosAccessLibNull/PlatformCmosAccessLibNull.inf
  CmosAccessLib|BoardModulePkg/Library/CmosAccessLib/CmosAccessLib.inf

[LibraryClasses.common.SEC]
  #######################################
  # Edk2 Packages
  #######################################
  ExtractGuidedSectionLib|MdePkg/Library/BaseExtractGuidedSectionLib/BaseExtractGuidedSectionLib.inf

[LibraryClasses.common.PEIM]
  #######################################
  # Edk2 Packages
  #######################################
  PeiResourcePublicationLib|MdePkg/Library/PeiResourcePublicationLib/PeiResourcePublicationLib.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf
  VmgExitLib|UefiCpuPkg/Library/VmgExitLibNull/VmgExitLibNull.inf

  #####################################
  # Silicon Package
  #####################################
  ReportCpuHobLib|IntelSiliconPkg/Library/ReportCpuHobLib/ReportCpuHobLib.inf

  #####################################
  # Platform Package
  #####################################
!if $(TARGET) == DEBUG
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/PeiTestPointCheckLib.inf
!endif
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/PeiTestPointLib.inf
  SetCacheMtrrLib|$(PLATFORM_PACKAGE)/Library/SetCacheMtrrLib/SetCacheMtrrLib.inf

[LibraryClasses.common.DXE_DRIVER]

  #####################################
  # Platform Package
  #####################################
  PlatformBootManagerLib|$(PLATFORM_PACKAGE)/Bds/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf

  #######################################
  # Board Package
  #######################################
  BoardBdsHookLib|$(BOARD_PKG)/Library/BoardBdsHookLib/BoardBdsHookLib.inf
  BoardBootManagerLib|$(BOARD_PKG)/Library/BoardBootManagerLib/BoardBootManagerLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  #######################################
  # Silicon Initialization Package
  #######################################
  SpiFlashCommonLib|$(PCH_PKG)/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLib.inf

#######################################
# PEI Components
#######################################
# @todo: Change below line to [Components.$(PEI_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed
[Components.IA32]
  #######################################
  # Edk2 Packages
  #######################################
  #  S3 SMM driver
  #  @todo: UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationPei.inf
  UefiCpuPkg/Universal/Acpi/S3Resume2Pei/S3Resume2Pei.inf {
    <LibraryClasses>
      LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxPeiLib.inf
  }

  #######################################
  # Silicon Initialization Package
  #######################################
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  $(SKT_PKG)/Smm/Access/SmmAccessPei.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }
!endif

  #####################################
  # Platform Package
  #####################################
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPreMem.inf {
    <LibraryClasses>
      BoardInitLib|$(PROJECT)/Library/BoardInitLib/PeiBoardInitPreMemLib.inf
  }
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPostMem.inf {
    <LibraryClasses>
      BoardInitLib|$(PROJECT)/Library/BoardInitLib/PeiBoardInitPostMemLib.inf
  }
  $(PLATFORM_PACKAGE)/PlatformInit/ReportFv/ReportFvPei.inf
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPreMem.inf
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPostMem.inf

  #######################################
  # Board Package
  #######################################
  $(BOARD_PKG)/SecCore/SecMain.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

  $(BOARD_PKG)/SimicsPei/SimicsPei.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }

#######################################
# DXE Components
#######################################
# @todo: Change below line to [Components.$(DXE_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed
[Components.X64]
  #######################################
  # Edk2 Packages
  #######################################
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
  MdeModulePkg/Bus/Isa/Ps2KeyboardDxe/Ps2KeyboardDxe.inf
  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  MdeModulePkg/Bus/Pci/SataControllerDxe/SataControllerDxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf {
    <LibraryClasses>
      PciHostBridgeLib|$(BOARD_PKG)/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  }
  MdeModulePkg/Bus/Scsi/ScsiBusDxe/ScsiBusDxe.inf
  MdeModulePkg/Bus/Scsi/ScsiDiskDxe/ScsiDiskDxe.inf
  MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
  MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf
  MdeModulePkg/Universal/Acpi/S3SaveStateDxe/S3SaveStateDxe.inf
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/EbcDxe/EbcDxe.inf
  MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  MdeModulePkg/Universal/LockBox/SmmLockBox/SmmLockBox.inf {
    <LibraryClasses>
      LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxSmmLib.inf
  }
  UefiCpuPkg/CpuS3DataDxe/CpuS3DataDxe.inf
  UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf
!endif
  UefiCpuPkg/CpuDxe/CpuDxe.inf

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

  #######################################
  # Silicon Initialization Package
  #######################################
  SimicsIch10BinPkg/UndiBinary/UndiDxe.inf
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  $(PCH_PKG)/SmmControl/RuntimeDxe/SmmControl2Dxe.inf
  $(PCH_PKG)/Spi/Smm/PchSpiSmm.inf
  $(SKT_PKG)/Smm/Access/SmmAccess2Dxe.inf
!endif

  #####################################
  # Platform Package
  #####################################
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitDxe/PlatformInitDxe.inf
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitSmm/PlatformInitSmm.inf
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  $(PLATFORM_PACKAGE)/Flash/SpiFvbService/SpiFvbServiceSmm.inf
!endif

  #######################################
  # Board Package
  #######################################
  $(BOARD_PKG)/AcpiTables/AcpiTables.inf
  $(BOARD_PKG)/AcpiTables/MinPlatformAcpiTables/AcpiPlatform.inf
  $(BOARD_PKG)/SimicsDxe/SimicsDxe.inf
  $(BOARD_PKG)/SimicsVideoDxe/SimicsVideoDxe.inf
  $(BOARD_PKG)/SmbiosPlatformDxe/SmbiosPlatformDxe.inf
  BoardModulePkg/LegacySioDxe/LegacySioDxe.inf
  BoardModulePkg/BoardBdsHookDxe/BoardBdsHookDxe.inf
