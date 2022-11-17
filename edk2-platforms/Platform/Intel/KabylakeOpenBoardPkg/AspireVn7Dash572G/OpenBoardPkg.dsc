## @file
#  The main build description file for the Aspire VN7-572G board.
#
# Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##
[Defines]
  DEFINE      PLATFORM_PACKAGE                = MinPlatformPkg
  DEFINE      PLATFORM_SI_PACKAGE             = KabylakeSiliconPkg
  DEFINE      PLATFORM_SI_BIN_PACKAGE         = KabylakeSiliconBinPkg
  DEFINE      PLATFORM_BOARD_PACKAGE          = KabylakeOpenBoardPkg
  DEFINE      BOARD                           = AspireVn7Dash572G
  DEFINE      PROJECT                         = $(PLATFORM_BOARD_PACKAGE)/$(BOARD)
  DEFINE      PEI_ARCH                        = IA32
  DEFINE      DXE_ARCH                        = X64
  DEFINE      TOP_MEMORY_ADDRESS              = 0x0

  #
  # Default value for OpenBoardPkg.fdf use
  #
  DEFINE BIOS_SIZE_OPTION = SIZE_60

  #
  # Debug logging
  #
  DEFINE USE_PEI_SPI_LOGGING  = FALSE
  DEFINE USE_MEMORY_LOGGING   = FALSE
  DEFINE RELEASE_LOGGING      = ($(USE_PEI_SPI_LOGGING) || $(USE_MEMORY_LOGGING))
  DEFINE TESTING              = TRUE

  PLATFORM_NAME                               = $(PLATFORM_PACKAGE)
  PLATFORM_GUID                               = AEEEF17C-36B6-4B68-949A-1E54CB33492F
  PLATFORM_VERSION                            = 0.1
  DSC_SPECIFICATION                           = 0x00010005
  OUTPUT_DIRECTORY                            = Build/$(PROJECT)
  SUPPORTED_ARCHITECTURES                     = IA32|X64
  BUILD_TARGETS                               = DEBUG|RELEASE
  SKUID_IDENTIFIER                            = ALL
  FLASH_DEFINITION                            = $(PROJECT)/OpenBoardPkg.fdf

  FIX_LOAD_TOP_MEMORY_ADDRESS                 = 0x0

  #
  # Include PCD configuration for this board.
  #
  !include AdvancedFeaturePkg/Include/AdvancedFeaturesPcd.dsc

  !include OpenBoardPkgPcd.dsc
  !include AdvancedFeaturePkg/Include/AdvancedFeatures.dsc

[Defines]
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  #
  # For backward compatibility API mode will use KabylakeFspBinPkg.
  # KabylakeFspBinPkg only supports API mode.
  #
  DEFINE      PLATFORM_FSP_BIN_PACKAGE        = KabylakeFspBinPkg
!else
  #
  # AmberLakeFspBinPkg supports both API and Dispatch modes
  #
  DEFINE      PLATFORM_FSP_BIN_PACKAGE        = AmberLakeFspBinPkg
!endif

[PcdsDynamicExDefault.common.DEFAULT]
!if gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode == TRUE
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 0
  #
  # Include FSP DynamicEx PCD settings in Dispatch mode
  #
  !include $(PLATFORM_FSP_BIN_PACKAGE)/FspPcds.dsc

  #
  # Override some FSP consumed PCD default value to match platform requirement.
  #
  gSiPkgTokenSpaceGuid.PcdSiPciExpressBaseAddress |gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress
  gSiPkgTokenSpaceGuid.PcdSiPciExpressRegionLength|gMinPlatformPkgTokenSpaceGuid.PcdPciExpressRegionLength
!endif
!endif

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this board.
#
################################################################################
[SkuIds]
  0x00|DEFAULT                # 0|DEFAULT is reserved and always required.
  # For further details on specific SKUs (which dGPU installed), see EC page of schematics
  0x41|RayleighSLx_dGPU       # Detect the UMA board by GPIO
  0x42|NewgateSLx_dGPU

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

# @todo: Change below line to [Components.$(PEI_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed
[Components.IA32]
!include $(PLATFORM_PACKAGE)/Include/Dsc/CorePeiInclude.dsc
!include $(PLATFORM_SI_PACKAGE)/SiPkgPei.dsc

# @todo: Change below line to [Components.$(DXE_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed
[Components.X64]
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreDxeInclude.dsc
!include $(PLATFORM_SI_PACKAGE)/SiPkgDxe.dsc

#######################################
# Build Option Includes
#######################################
!include $(PLATFORM_SI_PACKAGE)/SiPkgBuildOption.dsc
!include OpenBoardPkgBuildOption.dsc

################################################################################
#
# Library Class section - list of all Library Classes needed by this board.
#
################################################################################

# TODO: Harden and tune platform by libraries
[LibraryClasses.common]
  #######################################
  # Edk2 Packages
  #######################################
  FspWrapperApiLib|IntelFsp2WrapperPkg/Library/BaseFspWrapperApiLib/BaseFspWrapperApiLib.inf
  FspWrapperApiTestLib|IntelFsp2WrapperPkg/Library/PeiFspWrapperApiTestLib/PeiFspWrapperApiTestLib.inf
  # This board will set debugging library instances; FIXME: UART2 not used
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf

  #######################################
  # Silicon Initialization Package
  #######################################
  ConfigBlockLib|IntelSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf
  SiliconInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconInitLib/PeiSiliconInitLib.inf

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  #
  # FSP API mode
  #
  SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconPolicyInitLibFsp/PeiSiliconPolicyInitLibFsp.inf
!else
  #
  # FSP Dispatch mode and non-FSP build (EDK2 build)
  #
  SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconPolicyInitLibFsp/PeiSiliconPolicyInitLibFspAml.inf
!endif

  #####################################
  # Platform Package
  #####################################
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/BoardInitLibNull/BoardInitLibNull.inf
  FspWrapperHobProcessLib|$(PLATFORM_PACKAGE)/FspWrapper/Library/PeiFspWrapperHobProcessLib/PeiFspWrapperHobProcessLib.inf
  FspWrapperPlatformLib|$(PLATFORM_PACKAGE)/FspWrapper/Library/PeiFspWrapperPlatformLib/PeiFspWrapperPlatformLib.inf
  PciHostBridgeLib|$(PLATFORM_PACKAGE)/Pci/Library/PciHostBridgeLibSimple/PciHostBridgeLibSimple.inf
  PciSegmentInfoLib|$(PLATFORM_PACKAGE)/Pci/Library/PciSegmentInfoLibSimple/PciSegmentInfoLibSimple.inf
  PeiLib|$(PLATFORM_PACKAGE)/Library/PeiLib/PeiLib.inf
  PlatformBootManagerLib|$(PLATFORM_PACKAGE)/Bds/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf
  ReportFvLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/PeiReportFvLib/PeiReportFvLib.inf
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf

  #######################################
  # Board Package
  #######################################
  EcLib|$(PLATFORM_BOARD_PACKAGE)/Library/BaseEcLib/BaseEcLib.inf
  BoardEcLib|$(PROJECT)/Library/BoardEcLib/BoardEcLib.inf
  GpioExpanderLib|$(PLATFORM_BOARD_PACKAGE)/Library/BaseGpioExpanderLib/BaseGpioExpanderLib.inf
  I2cAccessLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiI2cAccessLib/PeiI2cAccessLib.inf
  PlatformSecLib|$(PLATFORM_PACKAGE)/FspWrapper/Library/SecFspWrapperPlatformSecLib/SecFspWrapperPlatformSecLib.inf

  # Thunderbolt
!if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdTbtEnable == TRUE
  DxeTbtPolicyLib|$(PLATFORM_BOARD_PACKAGE)/Features/Tbt/Library/DxeTbtPolicyLib/DxeTbtPolicyLib.inf
  TbtCommonLib|$(PLATFORM_BOARD_PACKAGE)/Features/Tbt/Library/PeiDxeSmmTbtCommonLib/TbtCommonLib.inf
!endif

  #######################################
  # Board-specific
  #######################################
  PlatformHookLib|MdeModulePkg/Library/BasePlatformHookLibNull/BasePlatformHookLibNull.inf

# NB: MinPlatform sets a NULL DebugLib and only overrides it for DEBUG builds
# TODO: Now that all debug logging is routed through RSC, correct the defines
[LibraryClasses.IA32.SEC]
  #######################################
  # Edk2 Packages
  #######################################
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf

  #######################################
  # Platform Package
  #######################################
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/SecTestPointCheckLib.inf
  SecBoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/SecBoardInitLibNull/SecBoardInitLibNull.inf
  SiliconPolicyUpdateLib|MinPlatformPkg/PlatformInit/Library/SiliconPolicyUpdateLibNull/SiliconPolicyUpdateLibNull.inf

[LibraryClasses.common.PEI_CORE]
  #######################################
  # Edk2 Packages
  #######################################
# SPI logging requires local patch: InitializeMemoryServices() before ProcessLibraryConstructorList()
# In-memory logging may require too many services for early core debug output
!if $(RELEASE_LOGGING) == TRUE
  DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
!endif

[LibraryClasses.common.PEIM]
  #######################################
  # Edk2 Packages
  #######################################
!if $(RELEASE_LOGGING) == TRUE
  DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
!endif

  #######################################
  # Silicon Package
  #######################################
  ReportCpuHobLib|IntelSiliconPkg/Library/ReportCpuHobLib/ReportCpuHobLib.inf

  #######################################
  # Platform Package
  #######################################
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/PeiMultiBoardInitSupportLib.inf
  FspWrapperPlatformLib|$(PLATFORM_PACKAGE)/FspWrapper/Library/PeiFspWrapperPlatformLib/PeiFspWrapperPlatformLib.inf
  MultiBoardInitSupportLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/PeiMultiBoardInitSupportLib.inf
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/PeiTestPointLib.inf
!if ($(TARGET) == DEBUG || $(TESTING) == TRUE)
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/PeiTestPointCheckLib.inf
!endif
  SetCacheMtrrLib|$(PLATFORM_PACKAGE)/Library/SetCacheMtrrLib/SetCacheMtrrLibNull.inf

  #######################################
  # Board Package
  #######################################
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  #
  # FSP API mode
  #
  SiliconPolicyUpdateLib|$(PROJECT)/FspWrapper/Library/PeiSiliconPolicyUpdateLibFsp/PeiSiliconPolicyUpdateLibFsp.inf
!else
  #
  # FSP Dispatch mode and non-FSP build (EDK2 build)
  #
  SiliconPolicyUpdateLib|$(PROJECT)/Policy/Library/PeiSiliconPolicyUpdateLib/PeiSiliconPolicyUpdateLib.inf
!endif

  # Thunderbolt
!if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdTbtEnable == TRUE
  PeiDTbtInitLib|$(PLATFORM_BOARD_PACKAGE)/Features/Tbt/Library/Private/PeiDTbtInitLib/PeiDTbtInitLib.inf
  PeiTbtPolicyLib|$(PLATFORM_BOARD_PACKAGE)/Features/Tbt/Library/PeiTbtPolicyLib/PeiTbtPolicyLib.inf
!endif

[LibraryClasses.common.DXE_CORE]
  #######################################
  # Edk2 Packages
  #######################################
# In-memory logging may require too many services for early core debug output
!if $(USE_MEMORY_LOGGING) == TRUE
  DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
!endif

[LibraryClasses.common.DXE_DRIVER]
  #######################################
  # Edk2 Packages
  #######################################
!if $(USE_MEMORY_LOGGING) == TRUE
  DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
!endif

  #######################################
  # Silicon Initialization Package
  #######################################
  SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/DxeSiliconPolicyInitLib/DxeSiliconPolicyInitLib.inf

  #######################################
  # Platform Package
  #######################################
  BoardAcpiTableLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/DxeMultiBoardAcpiSupportLib.inf
  BoardInitLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/DxeMultiBoardInitSupportLib.inf
  FspWrapperPlatformLib|$(PLATFORM_PACKAGE)/FspWrapper/Library/DxeFspWrapperPlatformLib/DxeFspWrapperPlatformLib.inf
  MultiBoardAcpiSupportLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/DxeMultiBoardAcpiSupportLib.inf
  MultiBoardInitSupportLib|$(PLATFORM_PACKAGE)/PlatformInit/Library/MultiBoardInitSupportLib/DxeMultiBoardInitSupportLib.inf
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/DxeTestPointLib.inf

!if ($(TARGET) == DEBUG || $(TESTING) == TRUE)
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/DxeTestPointCheckLib.inf
!endif
  #######################################
  # Board Package
  #######################################
  BoardBdsHookLib|BoardModulePkg/Library/BoardBdsHookLib/BoardBdsHookLib.inf
  BoardBootManagerLib|BoardModulePkg/Library/BoardBootManagerLib/BoardBootManagerLib.inf

  #######################################
  # Board-specific
  #######################################
  SiliconPolicyUpdateLib|$(PROJECT)/Policy/Library/DxeSiliconPolicyUpdateLib/DxeSiliconPolicyUpdateLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  #######################################
  # Edk2 Packages
  #######################################
!if $(USE_MEMORY_LOGGING) == TRUE
  DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
!endif

  #######################################
  # Silicon Initialization Package
  #######################################
  ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/DxeRuntimeResetSystemLib/DxeRuntimeResetSystemLib.inf

[LibraryClasses.common.SMM_CORE]
  #######################################
  # Edk2 Packages
  #######################################
# In-memory logging may require too many services for early core debug output
!if $(USE_MEMORY_LOGGING) == TRUE
  DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
!endif

[LibraryClasses.common.DXE_SMM_DRIVER]
  #######################################
  # Edk2 Packages
  #######################################
!if $(USE_MEMORY_LOGGING) == TRUE
  DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
!endif

  #######################################
  # Silicon Initialization Package
  #######################################
  SpiFlashCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLib.inf

  #######################################
  # Platform Package
  #######################################
  BoardAcpiEnableLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/SmmMultiBoardAcpiSupportLib.inf
  MultiBoardAcpiSupportLib|$(PLATFORM_PACKAGE)/Acpi/Library/MultiBoardAcpiSupportLib/SmmMultiBoardAcpiSupportLib.inf
  TestPointLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointLib/SmmTestPointLib.inf
!if ($(TARGET) == DEBUG || $(TESTING) == TRUE)
  TestPointCheckLib|$(PLATFORM_PACKAGE)/Test/Library/TestPointCheckLib/SmmTestPointCheckLib.inf
!endif

# TODO: DebugLib override for UEFI_DRIVER and UEFI_APPLICATION?

# TODO: Add and improve feature support
#######################################
# PEI Components
#######################################
# @todo: Change below line to [Components.$(PEI_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed
[Components.IA32]
  #######################################
  # Edk2 Packages
  #######################################
  UefiCpuPkg/SecCore/SecCore.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }

  MdeModulePkg/Universal/StatusCodeHandler/Pei/StatusCodeHandlerPei.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!if $(USE_PEI_SPI_LOGGING) == TRUE
      SerialPortLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiSerialPortLibSpiFlash/PeiSerialPortLibSpiFlash.inf
!else
!if $(USE_MEMORY_LOGGING) == TRUE
      SerialPortLib|MdeModulePkg/Library/PeiDxeSerialPortLibMem/PeiSerialPortLibMem.inf
!endif
!endif
    <PcdsFixedAtBuild>
      gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|$(RELEASE_LOGGING)
      gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeMemorySize|48
  }

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  #
  # In FSP API mode the policy has to be installed before FSP Wrapper updating UPD.
  # Add policy as dependency for FSP Wrapper
  #
  IntelFsp2WrapperPkg/FspmWrapperPeim/FspmWrapperPeim.inf {
    <LibraryClasses>
      SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconPolicyInitLibDependency/PeiPreMemSiliconPolicyInitLibDependency.inf
  }
  IntelFsp2WrapperPkg/FspsWrapperPeim/FspsWrapperPeim.inf {
    <LibraryClasses>
      SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconPolicyInitLibDependency/PeiPostMemSiliconPolicyInitLibDependency.inf
  }
!else
  #
  # In FSP Dispatch mode the policy will be installed after FSP-M dispatched (only PrePolicy silicon-init executed).
  # Do not add policy dependency and let FspmWrapper report FSP-M FV to dispatcher.
  #
  IntelFsp2WrapperPkg/FspmWrapperPeim/FspmWrapperPeim.inf {
    <LibraryClasses>
      SiliconPolicyInitLib|MinPlatformPkg/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
  }
  #
  # In FSP Dispatch mode the policy will be installed after FSP-S dispatched (only PrePolicy silicon-init executed).
  # Do not add policy dependency and let FspsWrapper report FSP-S FV to dispatcher.
  #
  IntelFsp2WrapperPkg/FspsWrapperPeim/FspsWrapperPeim.inf {
    <LibraryClasses>
      SiliconPolicyInitLib|MinPlatformPkg/PlatformInit/Library/SiliconPolicyInitLibNull/SiliconPolicyInitLibNull.inf
  }
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  SecurityPkg/Tcg/Tcg2Pei/Tcg2Pei.inf {
    <LibraryClasses>
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
  }
!endif

  #######################################
  # Silicon Initialization Package
  #######################################
  IntelSiliconPkg/Feature/VTd/IntelVTdPmrPei/IntelVTdPmrPei.inf
  IntelSiliconPkg/Feature/VTd/PlatformVTdInfoSamplePei/PlatformVTdInfoSamplePei.inf

  #######################################
  # Platform Package
  #######################################
  $(PLATFORM_PACKAGE)/PlatformInit/ReportFv/ReportFvPei.inf
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPreMem.inf {
    <LibraryClasses>
      !if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdMultiBoardSupport == FALSE
        BoardInitLib|$(PROJECT)/Library/BoardInitLib/PeiBoardInitPreMemLib.inf
      !else
        NULL|$(PROJECT)/Library/BoardInitLib/PeiMultiBoardInitPreMemLib.inf
      !endif
  }

  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitPei/PlatformInitPostMem.inf {
    <LibraryClasses>
      !if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdMultiBoardSupport == FALSE
        BoardInitLib|$(PROJECT)/Library/BoardInitLib/PeiBoardInitPostMemLib.inf
      !else
        NULL|$(PROJECT)/Library/BoardInitLib/PeiMultiBoardInitPostMemLib.inf
      !endif
  }

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPreMem.inf {
    <LibraryClasses>
    #
    # Hook a library constructor to update some policy fields when policy is installed.
    #
    NULL|$(PLATFORM_BOARD_PACKAGE)/FspWrapper/Library/PeiSiliconPolicyNotifyLib/PeiPreMemSiliconPolicyNotifyLib.inf
  }
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPostMem.inf
!else
  #
  # FSP Dispatch mode will consume DefaultPolicyInit PPI produced by FSP to install a default policy PPI.
  # Similar as UPD in FSP API mode, DefaultPolicyInit PPI in Dispatch mode can generate different policy structure
  # for different FSP revisions, but they must maintain backward compatibility.
  #
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPreMem.inf {
    <LibraryClasses>
      SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconPolicyInitLib/PeiPreMemSiliconPolicyInitLib.inf
  }
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPostMem.inf {
    <LibraryClasses>
      SiliconPolicyInitLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconPolicyInitLib/PeiPostMemSiliconPolicyInitLib.inf
  }
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  $(PLATFORM_PACKAGE)/Tcg/Tcg2PlatformPei/Tcg2PlatformPei.inf
!endif

  #######################################
  # Board Package
  #######################################
  # Thunderbolt
!if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdTbtEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/Tbt/TbtInit/Pei/PeiTbtInit.inf
!endif
  $(PLATFORM_BOARD_PACKAGE)/BiosInfo/BiosInfo.inf

#######################################
# DXE Components
#######################################
# @todo: Change below line to [Components.$(DXE_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed
[Components.X64]
# Compiled .efi but not in FV:
# - dpDynamicCommand, TestPointDumpApp
# Other apps; perhaps useful:
# - MdeModulePkg/{DumpDynPcd,*ProfileInfo,VariableInfo}, UefiCpuPkg/Cpuid
# - Also, ShellPkg/*DynamicCommand

  #######################################
  # Edk2 Packages
  #######################################
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!if $(USE_MEMORY_LOGGING) == TRUE
      SerialPortLib|MdeModulePkg/Library/PeiDxeSerialPortLibMem/DxeSerialPortLibMem.inf
!endif
    <PcdsFixedAtBuild>
      gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|$(USE_MEMORY_LOGGING)
      gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeMemorySize|512
  }
  # TODO: Still requires a little more thought
  MdeModulePkg/Universal/StatusCodeHandler/Smm/StatusCodeHandlerSmm.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!if $(USE_MEMORY_LOGGING) == TRUE
      SerialPortLib|MdeModulePkg/Library/PeiDxeSerialPortLibMem/SmmSerialPortLibMem.inf
!endif
    <PcdsFixedAtBuild>
      gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|$(USE_MEMORY_LOGGING)
      gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeMemorySize|512
  }
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf
  MdeModulePkg/Bus/Pci/SataControllerDxe/SataControllerDxe.inf
  MdeModulePkg/Universal/Console/GraphicsOutputDxe/GraphicsOutputDxe.inf
  MdeModulePkg/Bus/Isa/Ps2KeyboardDxe/Ps2KeyboardDxe.inf
  MdeModulePkg/Bus/Isa/Ps2MouseDxe/Ps2MouseDxe.inf
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <LibraryClasses>
      NULL|BoardModulePkg/Library/BdsPs2KbcLib/BdsPs2KbcLib.inf
  }
  UefiCpuPkg/CpuDxe/CpuDxe.inf {
    <LibraryClasses>
!if $(USE_MEMORY_LOGGING) == TRUE
# TODO/TEST
#      SerialPortLib|MdeModulePkg/Library/PeiDxeSerialPortLibMem/DxeSerialPortLibMem.inf
!endif
  }

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  #
  # FSP API mode
  #
  IntelFsp2WrapperPkg/FspWrapperNotifyDxe/FspWrapperNotifyDxe.inf
!endif

  ShellPkg/Application/Shell/Shell.inf {
    <PcdsFixedAtBuild>
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
    <LibraryClasses>
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork2CommandsLib/UefiShellNetwork2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellAcpiViewCommandLib/UefiShellAcpiViewCommandLib.inf
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
      ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
      ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  }

!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80080046
    <LibraryClasses>
      !if $(TARGET) == DEBUG
        DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      !endif
!if $(USE_MEMORY_LOGGING) == TRUE
# TODO/TEST
#      SerialPortLib|MdeModulePkg/Library/PeiDxeSerialPortLibMem/SmmSerialPortLibMem.inf
!endif
  }
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  SecurityPkg/Tcg/Tcg2Dxe/Tcg2Dxe.inf {
    <LibraryClasses>
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
  }
!endif

  #######################################
  # Silicon Initialization Package
  #######################################
  IntelSiliconPkg/Feature/VTd/IntelVTdDxe/IntelVTdDxe.inf
  $(PLATFORM_SI_BIN_PACKAGE)/Microcode/MicrocodeUpdates.inf

  #######################################
  # Platform Package
  #######################################
  $(PLATFORM_PACKAGE)/FspWrapper/SaveMemoryConfig/SaveMemoryConfig.inf
  $(PLATFORM_PACKAGE)/Hsti/HstiIbvPlatformDxe/HstiIbvPlatformDxe.inf
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitDxe/PlatformInitDxe.inf {
    <LibraryClasses>
      !if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdMultiBoardSupport == FALSE
        BoardInitLib|$(PROJECT)/Library/BoardInitLib/DxeBoardInitLib.inf
      !else
        NULL|$(PROJECT)/Library/BoardInitLib/DxeMultiBoardInitLib.inf
      !endif
  }
  $(PLATFORM_PACKAGE)/PlatformInit/SiliconPolicyDxe/SiliconPolicyDxe.inf
  $(PLATFORM_PACKAGE)/Test/TestPointDumpApp/TestPointDumpApp.inf
  $(PLATFORM_PACKAGE)/Test/TestPointStubDxe/TestPointStubDxe.inf

!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  $(PLATFORM_PACKAGE)/Tcg/Tcg2PlatformDxe/Tcg2PlatformDxe.inf
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE

  $(PLATFORM_PACKAGE)/Flash/SpiFvbService/SpiFvbServiceSmm.inf
  $(PLATFORM_PACKAGE)/PlatformInit/PlatformInitSmm/PlatformInitSmm.inf

  $(PLATFORM_PACKAGE)/Acpi/AcpiSmm/AcpiSmm.inf {
    <LibraryClasses>
      !if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdMultiBoardSupport == FALSE
        BoardAcpiEnableLib|$(PROJECT)/Library/BoardAcpiLib/SmmBoardAcpiEnableLib.inf
      !else
        NULL|$(PROJECT)/Library/BoardAcpiLib/SmmMultiBoardAcpiSupportLib.inf
      !endif
  }

  $(PLATFORM_PACKAGE)/Acpi/AcpiTables/AcpiPlatform.inf {
    <LibraryClasses>
      !if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdMultiBoardSupport == FALSE
        BoardAcpiTableLib|$(PROJECT)/Library/BoardAcpiLib/DxeBoardAcpiTableLib.inf
      !else
        NULL|$(PROJECT)/Library/BoardAcpiLib/DxeMultiBoardAcpiSupportLib.inf
      !endif
  }

!endif

  #######################################
  # Board Package
  #######################################
  # Thunderbolt
!if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdTbtEnable == TRUE
  $(PLATFORM_BOARD_PACKAGE)/Features/Tbt/TbtInit/Smm/TbtSmm.inf
  $(PLATFORM_BOARD_PACKAGE)/Features/Tbt/TbtInit/Dxe/TbtDxe.inf
  $(PLATFORM_BOARD_PACKAGE)/Features/PciHotPlug/PciHotPlug.inf
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  $(PLATFORM_BOARD_PACKAGE)/Acpi/BoardAcpiDxe/BoardAcpiDxe.inf {
    <LibraryClasses>
      !if gKabylakeOpenBoardPkgTokenSpaceGuid.PcdMultiBoardSupport == FALSE
        BoardAcpiTableLib|$(PROJECT)/Library/BoardAcpiLib/DxeBoardAcpiTableLib.inf
      !else
        NULL|$(PROJECT)/Library/BoardAcpiLib/DxeMultiBoardAcpiSupportLib.inf
      !endif
  }
  $(PROJECT)/Acpi/BoardAcpiTables.inf
!endif
  BoardModulePkg/LegacySioDxe/LegacySioDxe.inf
  BoardModulePkg/BoardBdsHookDxe/BoardBdsHookDxe.inf
