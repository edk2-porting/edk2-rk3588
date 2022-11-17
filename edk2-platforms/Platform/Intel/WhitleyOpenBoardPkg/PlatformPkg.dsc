## @file
# X64 Platform with 64-bit DXE.
#
# @copyright
# Copyright 2008 - 2021 Intel Corporation. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = $(RP_PKG)
  PLATFORM_GUID                       = D7EAF54D-C9B9-4075-89F0-71943DBCFA61
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/$(RP_PKG)
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = DEFAULT
  VPD_TOOL_GUID                       = 8C3D856A-9BE6-468E-850A-24F7A8D38E08
  FLASH_DEFINITION                    = $(RP_PKG)/PlatformPkg.fdf
  PLATFORM_SI_PACKAGE                 = ClientOneSiliconPkg
  DEFINE      PLATFORM_SI_BIN_PACKAGE = WhitleySiliconBinPkg
  PEI_ARCH                            = IA32
  DXE_ARCH                            = X64

!if $(CPUTARGET) == "CPX"
  DEFINE FSP_BIN_PKG            = CedarIslandFspBinPkg
  DEFINE IIO_INSTANCE           = Skx
!elseif $(CPUTARGET) == "ICX"
  DEFINE FSP_BIN_PKG            = WhitleyFspBinPkg
  DEFINE IIO_INSTANCE           = Icx
!else
  DEFINE IIO_INSTANCE           = UnknownCpu
!endif

  #
  # Platform On/Off features are defined here
  #
  !include $(RP_PKG)/PlatformPkgConfig.dsc

  #
  # MRC common configuration options defined here
  #
  !include $(SILICON_PKG)/MrcCommonConfig.dsc

[Packages]
  IntelFsp2WrapperPkg/IntelFsp2WrapperPkg.dec

  !include $(FSP_BIN_PKG)/DynamicExPcd.dsc
  !include $(FSP_BIN_PKG)/DynamicExPcdFvLateSilicon.dsc
  !include $(RP_PKG)/DynamicExPcd.dsc

  !include $(RP_PKG)/Uba/UbaCommon.dsc
  !include $(RP_PKG)/Uba/UbaRpBoards.dsc

  !include $(RP_PKG)/Include/Dsc/EnablePerformanceMonitoringInfrastructure.dsc

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

[DefaultStores]
  0|STANDARD
  1|MANUFACTURING


################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFeatureFlag]
  #
  # MinPlatform control flags
  #
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit     |FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit       |FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly        |FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable   |FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable             |FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable|TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable      |FALSE

  # don't degrade 64bit MMIO space to 32-bit
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciDegradeResourceForOptionRom|FALSE

  # Server doesn't support capsule update on Reset.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|FALSE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmEnableBspElection|TRUE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuHotPlugSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciBusHotplugDeviceSupport|FALSE

  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|TRUE

  gEfiCpRcPkgTokenSpaceGuid.Reserved15|TRUE

!if ($(CPUTARGET) == "ICX")
  gEfiCpRcPkgTokenSpaceGuid.PcdMemBootHealthFeatureSupported|FALSE
!endif # $(CPUTARGET) == "ICX"

  gCpuPkgTokenSpaceGuid.PcdCpuSkylakeFamilyFlag|TRUE
  gCpuPkgTokenSpaceGuid.PcdCpuIcelakeFamilyFlag|TRUE

  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmDebug|FALSE
  gCpuPkgTokenSpaceGuid.PcdCpuSelectLfpAsBspFlag|TRUE

  ## Uncomment for better boot performance
#  gPerfOptTokenSpaceGuid.PcdPreUefiLegacyEnable|FALSE
#  gPerfOptTokenSpaceGuid.PcdLocalVideoEnable|FALSE

  gPlatformTokenSpaceGuid.PcdSupportUnsignedCapsuleImage|TRUE

  ## This PCD specified whether ACPI SDT protocol is installed.
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

  ## This PCD specifies whether FPGA routine will be active
  gSocketPkgFpgaGuid.PcdSktFpgaActive|TRUE

!if $(CPU_SKX_ONLY_SUPPORT) == TRUE
  gEfiCpRcPkgTokenSpaceGuid.PerBitMargin|FALSE
  gEfiCpRcPkgTokenSpaceGuid.PcdSeparateCwlAdj|TRUE
!endif

  ## This PCD specifies whether or not to enable the High Speed UART
  gPlatformModuleTokenSpaceGuid.PcdEnableHighSpeedUart|FALSE

  gEfiMdeModulePkgTokenSpaceGuid.PcdHiiOsRuntimeSupport|FALSE

[PcdsFixedAtBuild]
  gEfiCpRcPkgTokenSpaceGuid.PcdRankSwitchFixOption|2

  ## MinPlatform Boot Stage Selector
  # Stage 1 - enable debug (system deadloop after debug init)
  # Stage 2 - mem init (system deadloop after mem init)
  # Stage 3 - boot to shell only
  # Stage 4 - boot to OS
  # Stage 5 - boot to OS with security boot enabled
  # Stage 6 - boot with advanced features enabled
  #
  gMinPlatformPkgTokenSpaceGuid.PcdBootStage|6

  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F                    # Enable asserts, prints, code, clear memory, and deadloops on asserts.
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0x80200047      # Built in messages:  Error, MTRR, info, load, warn, init
!if $(TARGET) == "DEBUG"
  gEfiSourceLevelDebugPkgTokenSpaceGuid.PcdDebugLoadImageMethod|0x2     # This is set to INT3 (0x2) for Simics source level debugging
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x0

  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x10000
  gEfiMdeModulePkgTokenSpaceGuid.PcdShadowPeimOnS3Boot|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x0
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdFSBClock|100000000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemId|"INTEL "
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x4449204C45544E49 # "INTEL ID"
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize|0x100000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxSizeNonPopulateCapsule|0x2100000
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosVersion|0x0302
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|140

  gCpuPkgTokenSpaceGuid.PcdCpuIEDRamSize|0x400000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|10000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|512
  gCpuPkgTokenSpaceGuid.PcdPlatformType|2
  gCpuPkgTokenSpaceGuid.PcdPlatformCpuMaxCoreFrequency|4000

  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x10000

  #PcdCpuMicrocodePatchRegionSize = PcdFlashNvStorageMicrocodeSize - (EFI_FIRMWARE_VOLUME_HEADER. HeaderLength + sizeof (EFI_FFS_FILE_HEADER))
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMicrocodePatchRegionSize|0x1FFF70

  #
  # This controls the NEM code region cached during SEC
  # It usually isn't necessary to match exactly the FV layout in the FDF file.
  # It is a performance optimization to have it match the flash region exactly
  # as then no extra reads are done to load unused flash into cache.
  #
  gCpuUncoreTokenSpaceGuid.PcdFlashSecCacheRegionBase|0xFFC00000
  gCpuUncoreTokenSpaceGuid.PcdFlashSecCacheRegionSize|0x00400000

  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamBase|0x00FE800000
  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamSize|0x0000200000

  #
  # Mode              | FSP_MODE | PcdFspModeSelection
  # ------------------|----------|--------------------
  # FSP Dispatch Mode |    1     |         0
  # FSP API Mode      |    0     |         1
  #
!if ($(FSP_MODE) == 0)
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection|1
  gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0x00070000
!else
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection|0
!endif
  gUefiCpuPkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x20000

  #
  # These will be initialized during build
  #

  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspTOffset|0x00000000
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspTSize|0x00000000
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspTBase|0x00000000

  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspMOffset|0x00000000
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspMSize|0x00000000
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspSOffset|0x00000000

  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspMBase|0x00000000
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspSSize|0x00000000
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspSBase|0x00000000

  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvMicrocodeBase|0x00000000
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvMicrocodeSize|0x00000000
  gCpPlatFlashTokenSpaceGuid.PcdFlashFvAcmRegionBase|0x00000000

  ## Specifies delay value in microseconds after sending out an INIT IPI.
  # @Prompt Configure delay value after send an INIT IPI
  gUefiCpuPkgTokenSpaceGuid.PcdCpuInitIpiDelayInMicroSeconds|10
  ## Specifies max supported number of Logical Processors.
  # @Prompt Configure max supported number of Logical Processorss
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApStackSize|0x1000

  gPlatformTokenSpaceGuid.PcdPerfPkgPchPmBaseFunctionNumber|0x2

  gPlatformTokenSpaceGuid.PcdUboDev|0x08
  gPlatformTokenSpaceGuid.PcdUboFunc|0x02
  gPlatformTokenSpaceGuid.PcdUboCpuBusNo0|0xCC

  gCpuPkgTokenSpaceGuid.PcdCpuIEDEnabled|TRUE
  gPlatformTokenSpaceGuid.PcdSupportLegacyStack|FALSE

  ## Defines the ACPI register set base address.
  #  The invalid 0xFFFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Timer IO Port Address
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress         |  0x0500

  ## Defines the PCI Bus Number of the PCI device that contains the BAR and Enable for ACPI hardware registers.
  # @Prompt ACPI Hardware PCI Bus Number
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBusNumber            |  0x00

  ## Defines the PCI Device Number of the PCI device that contains the BAR and Enable for ACPI hardware registers.
  #  The invalid 0xFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Hardware PCI Device Number
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciDeviceNumber         |  0x1F

  ## Defines the PCI Function Number of the PCI device that contains the BAR and Enable for ACPI hardware registers.
  #  The invalid 0xFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Hardware PCI Function Number
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciFunctionNumber       |  0x02

  ## Defines the PCI Register Offset of the PCI device that contains the Enable for ACPI hardware registers.
  #  The invalid 0xFFFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Hardware PCI Register Offset
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciEnableRegisterOffset |0x0044

  ## Defines the bit mask that must be set to enable the APIC hardware register BAR.
  # @Prompt ACPI Hardware PCI Bar Enable BitMask
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoBarEnableMask           |  0x80

  ## Defines the PCI Register Offset of the PCI device that contains the BAR for ACPI hardware registers.
  #  The invalid 0xFFFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Hardware PCI Bar Register Offset
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBarRegisterOffset    |0x0040

  ## Defines the offset to the 32-bit Timer Value register that resides within the ACPI BAR.
  # @Prompt Offset to 32-bit Timer register in ACPI BAR
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiPm1TmrOffset              |0x0008

!if $(CPUTARGET) == "ICX"
  #
  # ACPI PCD custom override
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultCreatorId|0x4C544E49
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultCreatorRevision|0x01000013
!endif

  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuCoreCount|28
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuSocketCount|$(MAX_SOCKET)
  gMinPlatformPkgTokenSpaceGuid.PcdWsmtProtectionFlags|0x07

  # Enable DDRT scheduler debug features for power on
  gEfiCpRcPkgTokenSpaceGuid.PcdDdrtSchedulerDebugDefault|TRUE

  # Disable Fast Warm Boot for Whitley Openboard Package
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcFastBootDefault|FALSE

!if $(CPU_SKX_ONLY_SUPPORT) == FALSE
  gCpuUncoreTokenSpaceGuid.PcdWaSerializationEn|FALSE
  gEfiCpRcPkgTokenSpaceGuid.PcdMrcCmdVrefCenteringTrainingEnable|FALSE
!endif

  gPcAtChipsetPkgTokenSpaceGuid.PcdRtcIndexRegister|0x74
  gPcAtChipsetPkgTokenSpaceGuid.PcdRtcTargetRegister|0x75

  #
  # PlatformInitPreMem
  #
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiAcpiReclaimMemorySize|0x100
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiAcpiNvsMemorySize|0xA30
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiReservedMemorySize|0x100
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtDataMemorySize|0x100
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtCodeMemorySize|0x100

  gEfiCpRcPkgTokenSpaceGuid.PcdReserved15|0

  !include $(SILICON_PKG)/Product/Whitley/SiliconPkg10nmPcds.dsc

[PcdsFixedAtBuild.IA32]
  #
  # FSP Base address PCD will be updated in FDF basing on flash map.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFsptBaseAddress|0
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmBaseAddress|0

!if ($(FSP_MODE) == 0)
  gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode|TRUE
  gIntelFsp2WrapperTokenSpaceGuid.PcdPeiMinMemSize|0x4000000
  gEfiMdePkgTokenSpaceGuid.PcdSpeculationBarrierType|0
!endif

[PcdsFixedAtBuild.X64]
  # Change PcdBootManagerMenuFile to UiApp
  ##

  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

  gPlatformModuleTokenSpaceGuid.PcdS3AcpiReservedMemorySize|0xC00000

  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmCodeAccessCheckEnable |TRUE

  #
  # AcpiPlatform
  #
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiEnableSwSmi|0xA0
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiDisableSwSmi|0xA1
  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicCount|32
  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicIdBase|0x09
  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicAddressBase|0xFEC01000
  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicInterruptBase|24

  gMinPlatformPkgTokenSpaceGuid.PcdFadtPreferredPmProfile|0x04
  gMinPlatformPkgTokenSpaceGuid.PcdFadtIaPcBootArch|0x0000
  gMinPlatformPkgTokenSpaceGuid.PcdFadtFlags|0x000004A5
  gMinPlatformPkgTokenSpaceGuid.PcdLocalApicAddress|0xFEE00000
  gMinPlatformPkgTokenSpaceGuid.PcdIoApicAddress|0xFEC00000
  gMinPlatformPkgTokenSpaceGuid.PcdIoApicId|0x08

  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1AEventBlockAddress|0x500
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1BEventBlockAddress|0
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1AControlBlockAddress|0x504
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1BControlBlockAddress|0
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm2ControlBlockAddress|0x550
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPmTimerBlockAddress|0x508
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe0BlockAddress|0x580
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe1BlockAddress|0

  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmRestrictedMemoryAccess|FALSE

  gMinPlatformPkgTokenSpaceGuid.PcdTrustedConsoleInputDevicePath|{    0x02, 0x01, 0x0C, 0x00, 0xD0, 0x41, 0x03, 0x0A,     0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x06, 0x00,    0x00, 0x1F, 0x02, 0x01, 0x0C, 0x00, 0xD0, 0x41,     0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0e,    0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC2,     0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01,    0x01, 0x03, 0x0a, 0x14, 0x00, 0x53, 0x47, 0xC1,     0xe0, 0xbe, 0xf9, 0xd2, 0x11, 0x9a, 0x0c, 0x00,    0x90, 0x27, 0x3f, 0xc1, 0x4d, 0x7F, 0x01, 0x04, 0x00, 0x03,      0x0F, 0x0B, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x03,       0x01, 0x01, 0x7F, 0xFF, 0x04, 0x00}
  gMinPlatformPkgTokenSpaceGuid.PcdTrustedConsoleOutputDevicePath|{   0x02, 0x01, 0x0C, 0x00, 0xD0, 0x41, 0x03, 0x0A,     0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x06, 0x00,    0x00, 0x1F, 0x02, 0x01, 0x0C, 0x00, 0xD0, 0x41,     0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0e,    0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC2,     0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01,    0x01, 0x03, 0x0a, 0x14, 0x00, 0x53, 0x47, 0xC1,     0xe0, 0xbe, 0xf9, 0xd2, 0x11, 0x9a, 0x0c, 0x00,    0x90, 0x27, 0x3f, 0xc1, 0x4d, 0x7F, 0x01, 0x04, 0x00, 0x02,       0x01, 0x0C, 0x00, 0xd0, 0x41, 0x03, 0x0A, 0x00,       0x00, 0x00, 0x00, 0x01, 0x01, 0x06, 0x00,  0x00,       0x02, 0x7F, 0xFF, 0x04, 0x00}
  gBoardModulePkgTokenSpaceGuid.PcdSuperIoPciIsaBridgeDevice|{0x0, 0x0, 0x1F, 0x0}
  gBoardModulePkgTokenSpaceGuid.PcdUart1Enable|0x01

[PcdsPatchableInModule]
  #
  # These debug options are patcheable so that they can be manipulated during debug (if memory is updateable)
  #

  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07         # Enable status codes for debug, progress, and errors
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000042           # Displayed messages:  Error, Info, warn

  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0x80000000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuNumberOfReservedVariableMtrrs|0

!if $(PREMEM_PAGE_ALLOC_SUPPORT) == FALSE
  gEfiCpRcPkgTokenSpaceGuid.PcdPeiTemporaryRamRcHeapSize|0x130000
!endif

[PcdsDynamicExDefault.IA32]
!if ($(FSP_MODE) == 0)
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmUpdDataAddress|0x00000000
!endif


[PcdsDynamicExHii]
  gEfiMdeModulePkgTokenSpaceGuid.PcdUse1GPageTable|L"1GPageTable"|gEfiGenericVariableGuid|0x0|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|0 # Variable: L"Timeout"
  gPlatformTokenSpaceGuid.PcdPlatformMemoryCheck|L"MemoryCheck"|gPlatformTokenSpaceGuid|0x0|0
  gCpPlatTokenSpaceGuid.PcdUefiOptimizedBoot|L"UefiOptimizedBoot"|gCpPlatTokenSpaceGuid|0x0|TRUE
  gPlatformModuleTokenSpaceGuid.PcdBootState|L"BootState"|gEfiGenericVariableGuid|0x0|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"
  gPlatformTokenSpaceGuid.PcdBootDeviceScratchPad5Changed|L"BootDeviceScratchPad"|gEfiGenericVariableGuid|0x0|FALSE

[PcdsDynamicExDefault]
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|200000
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmPhysicalPresence|TRUE
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmAutoDetection|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
  gPlatformModuleTokenSpaceGuid.PcdLtConfigLockEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdProcessorLtsxEnable|FALSE

  gCpuPkgTokenSpaceGuid.PcdCpuSmmMsrSaveStateEnable|FALSE
  gCpuPkgTokenSpaceGuid.PcdCpuSmmProtectedModeEnable|FALSE
  gCpuPkgTokenSpaceGuid.PcdCpuSmmRuntimeCtlHooks|FALSE

  gSiPkgTokenSpaceGuid.PcdWakeOnRTCS5|FALSE
  gSiPkgTokenSpaceGuid.PcdRtcWakeupTimeHour|0
  gSiPkgTokenSpaceGuid.PcdRtcWakeupTimeMinute|0
  gSiPkgTokenSpaceGuid.PcdRtcWakeupTimeSecond|0

  #Platform should change it to by code
  gSiPkgTokenSpaceGuid.PcdPchSataInitReg78Data|0xAAAA0000
  gSiPkgTokenSpaceGuid.PcdPchSataInitReg88Data|0xAA33AA22

  gEfiSecurityPkgTokenSpaceGuid.PcdUserPhysicalPresence|TRUE

  #
  # CPU features related PCDs.
  #
  gCpuPkgTokenSpaceGuid.PcdCpuEnergyPolicy
  gUefiCpuPkgTokenSpaceGuid.PcdCpuClockModulationDutyCycle
  gUefiCpuPkgTokenSpaceGuid.PcdIsPowerOnReset

  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdMrIovSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSystemPageSize|0x01

  ## Put fTPM guid here: e.g. { 0xf9c6a62f, 0xc60f, 0x4b44, { 0xa6, 0x29, 0xed, 0x3d, 0x40, 0xae, 0xfa, 0x5f } }
  ## TPM1.2 { 0x8b01e5b6, 0x4f19, 0x46e8, { 0xab, 0x93, 0x1c, 0x53, 0x67, 0x1b, 0x90, 0xcc } }
  ## TPM2.0Dtpm { 0x286bf25a, 0xc2c3, 0x408c, { 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17 } }

  #TPM2.0#
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x5a, 0xf2, 0x6b, 0x28, 0xc3, 0xc2, 0x8c, 0x40, 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17}

  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInitializationPolicy|0
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2InitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2SelfTestPolicy|0

  gCpuPkgTokenSpaceGuid.PcdCpuSmmUseDelayIndication|FALSE
  gCpuPkgTokenSpaceGuid.PcdCpuSmmUseBlockIndication|FALSE

  gPlatformTokenSpaceGuid.PcdOnboardVideoPciVendorId|0x102b
  gPlatformTokenSpaceGuid.PcdOnboardVideoPciDeviceId|0x0522

  gPlatformTokenSpaceGuid.PcdSetupMenuScanCode|0x000C
  gPlatformTokenSpaceGuid.PcdBootDeviceListScanCode|0x0011
  gPlatformTokenSpaceGuid.PcdBootMenuFile|{ 0xdc, 0x5b, 0xc2, 0xee, 0xf2, 0x67, 0x95, 0x4d, 0xb1, 0xd5, 0xf8, 0x1b, 0x20, 0x39, 0xd1, 0x1d }
  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicEnable|0x0

[PcdsDynamicExDefault.X64]
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|115200
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultDataBits|8
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultParity|1
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultStopBits|1
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|0

  #
  #  Set video to 1024x768 resolution
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1024
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|768

[PcdsDynamicExDefault]

!if $(CPUTARGET) == "CPX"
  !include $(RP_PKG)/StructurePcdCpx.dsc
!else
  !include $(RP_PKG)/StructurePcd.dsc
!endif

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################

!include MinPlatformPkg/Include/Dsc/CoreCommonLib.dsc
!include MinPlatformPkg/Include/Dsc/CorePeiLib.dsc
!include MinPlatformPkg/Include/Dsc/CoreDxeLib.dsc

[LibraryClasses]

  #
  # Simics source level debugging requires the non-null version of PeCoffExtraActionLib
  #
!if $(TARGET) == "DEBUG"
  PeCoffExtraActionLib|SourceLevelDebugPkg/Library/PeCoffExtraActionLibDebug/PeCoffExtraActionLibDebug.inf
!else
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
!endif

  #
  # Basic
  #

  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf

  #
  # Framework
  #
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  FrameBufferBltLib|MdeModulePkg/Library/FrameBufferBltLib/FrameBufferBltLib.inf

  SiliconPolicyInitLib|WhitleySiliconPkg/Library/SiliconPolicyInitLibShim/SiliconPolicyInitLibShim.inf
!if ($(FSP_MODE) == 0)
  SiliconPolicyUpdateLib|$(RP_PKG)/Library/SiliconPolicyUpdateLib/SiliconPolicyUpdateLibFsp.inf
!else
  SiliconPolicyUpdateLib|$(RP_PKG)/Library/SiliconPolicyUpdateLib/SiliconPolicyUpdateLib.inf
!endif

  SetupLib|WhitleySiliconPkg/Library/SetupLib/SetupLib.inf

  #
  # ToDo:  Can we use BaseAcpiTimerLib from MinPlatform?
  #
  TimerLib|PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf

  MultiPlatSupportLib|$(RP_PKG)/Library/MultiPlatSupportLib/MultiPlatSupportLib.inf
  ReadFfsLib|$(RP_PKG)/Library/ReadFfsLib/ReadFfsLib.inf
  PlatformSetupVariableSyncLib|$(RP_PKG)/Library/PlatformSetupVariableSyncLibNull/PlatformSetupVariableSyncLibNull.inf
  PlatformVariableHookLib |$(RP_PKG)/Library/PlatformVariableHookLibNull/PlatformVariableHookLibNull.inf

  PlatformBootManagerLib|$(PLATFORM_PKG)/Bds/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf
  SerialPortLib|$(RP_PKG)/Library/SerialPortLib/SerialPortLib.inf
  PlatformHooksLib|$(RP_PKG)/Library/PlatformHooksLib/PlatformHooksLib.inf

  CmosAccessLib|BoardModulePkg/Library/CmosAccessLib/CmosAccessLib.inf
  PlatformCmosAccessLib|$(RP_PKG)/Library/PlatformCmosAccessLib/PlatformCmosAccessLib.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf
  TpmCommLib|SecurityPkg/Library/TpmCommLib/TpmCommLib.inf

  #
  # MinPlatform uses port 80, we don't want to assume HW
  #
  PostCodeLib|MdePkg/Library/BasePostCodeLibDebug/BasePostCodeLibDebug.inf

  TcgPpVendorLib|SecurityPkg/Library/TcgPpVendorLibNull/TcgPpVendorLibNull.inf
  Tcg2PpVendorLib|SecurityPkg/Library/Tcg2PpVendorLibNull/Tcg2PpVendorLibNull.inf
  AslUpdateLib|$(PLATFORM_PKG)/Acpi/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf
  PciSegmentInfoLib|$(PLATFORM_PKG)/Pci/Library/PciSegmentInfoLibSimple/PciSegmentInfoLibSimple.inf
  PlatformOpromPolicyLib|$(RP_PKG)/Library/PlatformOpromPolicyLibNull/PlatformOpromPolicyLibNull.inf
  VmgExitLib|UefiCpuPkg/Library/VmgExitLibNull/VmgExitLibNull.inf

[LibraryClasses.Common.SEC, LibraryClasses.Common.PEI_CORE, LibraryClasses.Common.PEIM]
  FspWrapperApiLib|IntelFsp2WrapperPkg/Library/BaseFspWrapperApiLib/BaseFspWrapperApiLib.inf
  FspWrapperApiTestLib|IntelFsp2WrapperPkg/Library/PeiFspWrapperApiTestLib/PeiFspWrapperApiTestLib.inf
  FspWrapperPlatformLib|WhitleySiliconPkg/Library/FspWrapperPlatformLib/FspWrapperPlatformLib.inf
  FspWrapperHobProcessLib|WhitleyOpenBoardPkg/Library/PeiFspWrapperHobProcessLib/PeiFspWrapperHobProcessLib.inf

  FspSwitchStackLib|IntelFsp2Pkg/Library/BaseFspSwitchStackLib/BaseFspSwitchStackLib.inf
  FspCommonLib|IntelFsp2Pkg/Library/BaseFspCommonLib/BaseFspCommonLib.inf
  FspPlatformLib|IntelFsp2Pkg/Library/BaseFspPlatformLib/BaseFspPlatformLib.inf

[LibraryClasses.Common.SEC]
  #
  # SEC phase
  #
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf

  PlatformSecLib|$(RP_PKG)/Library/SecFspWrapperPlatformSecLib/SecFspWrapperPlatformSecLib.inf
  SecBoardInitLib|MinPlatformPkg/PlatformInit/Library/SecBoardInitLibNull/SecBoardInitLibNull.inf
  TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLib/SecTestPointCheckLib.inf
  VariableReadLib|MinPlatformPkg/Library/BaseVariableReadLibNull/BaseVariableReadLibNull.inf

[LibraryClasses.Common.PEI_CORE, LibraryClasses.Common.PEIM]
  #
  # ToDo:  Can we remove
  #
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/PeiCpuExceptionHandlerLib.inf

  MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf


  PeiPlatformHookLib|$(RP_PKG)/Library/PeiPlatformHookLib/PeiPlatformHooklib.inf
  PlatformClocksLib|$(RP_PKG)/Library/PlatformClocksLib/Pei/PlatformClocksLib.inf

  TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLib/PeiTestPointCheckLib.inf
  TestPointLib|MinPlatformPkg/Test/Library/TestPointLib/PeiTestPointLib.inf

  ReportFvLib|$(RP_PKG)/Library/PeiReportFvLib/PeiReportFvLib.inf

[LibraryClasses.Common.PEIM]
  #
  # Library instance consumed by MinPlatformPkg PlatformInit modules.
  #
  ReportCpuHobLib|MinPlatformPkg/PlatformInit/Library/ReportCpuHobLib/ReportCpuHobLib.inf
  SetCacheMtrrLib|$(RP_PKG)/Library/SetCacheMtrrLib/SetCacheMtrrLib.inf

[LibraryClasses.common.DXE_CORE, LibraryClasses.common.DXE_SMM_DRIVER, LibraryClasses.common.SMM_CORE, LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.DXE_RUNTIME_DRIVER, LibraryClasses.common.UEFI_DRIVER, LibraryClasses.common.UEFI_APPLICATION]
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf

  Tcg2PhysicalPresenceLib|$(RP_PKG)/Library/Tcg2PhysicalPresenceLibNull/DxeTcg2PhysicalPresenceLib.inf
  TcgPhysicalPresenceLib|SecurityPkg/Library/DxeTcgPhysicalPresenceLib/DxeTcgPhysicalPresenceLib.inf

  BiosIdLib|BoardModulePkg/Library/BiosIdLib/DxeBiosIdLib.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/DxeMpInitLib.inf

  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf

  Tpm12DeviceLib|SecurityPkg/Library/Tpm12DeviceLibDTpm/Tpm12DeviceLibDTpm.inf

  TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
  TestPointLib|MinPlatformPkg/Test/Library/TestPointLib/DxeTestPointLib.inf
  BoardBdsHookLib|BoardModulePkg/Library/BoardBdsHookLib/BoardBdsHookLib.inf
  BoardBootManagerLib|MinPlatformPkg/Bds/Library/BoardBootManagerLibNull/BoardBootManagerLibNull.inf

  CompressDxeLib|MinPlatformPkg/Library/CompressLib/CompressLib.inf

[LibraryClasses.Common.DXE_SMM_DRIVER]
  SpiFlashCommonLib|$(RP_PKG)/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLib.inf

  TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLib/SmmTestPointCheckLib.inf
  TestPointLib|MinPlatformPkg/Test/Library/TestPointLib/SmmTestPointLib.inf
  MmServicesTableLib|MdePkg/Library/MmServicesTableLib/MmServicesTableLib.inf

[LibraryClasses.Common.SMM_CORE]
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf

[LibraryClasses.Common]
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf

[Components.IA32]
  UefiCpuPkg/SecCore/SecCore.inf

  !include MinPlatformPkg/Include/Dsc/CorePeiInclude.dsc

  MdeModulePkg/Universal/PCD/Pei/Pcd.inf {
    <LibraryClasses>
      #
      # Beware of circular dependencies on PCD if you want to use another DebugLib instance.
      #
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
      NULL|$(FSP_BIN_PKG)/Library/FspPcdListLibNull/FspPcdListLibNull.inf                 # Include FSP DynamicEx PCD
      NULL|$(FSP_BIN_PKG)/Library/FspPcdListLibNull/FspPcdListLibNullFvLateSilicon.inf    # Include FvLateSilicon DynamicEx PCD
      NULL|$(FSP_BIN_PKG)/Library/FspPcdListLibNull/FspPcdListLibNullFvLateOpenBoard.inf  # Include FvLateBoard DynamicEx PCD
  }
  $(RP_PKG)/Universal/PeiExStatusCodeRouter/ExReportStatusCodeRouterPei.inf
  $(RP_PKG)/Universal/PeiExStatusCodeHandler/ExStatusCodeHandlerPei.inf
  $(RP_PKG)/Universal/PeiInterposerToSvidMap/PeiInterposerToSvidMap.inf

  $(RP_PKG)/Features/Variable/PlatformVariable/Pei/PlatformVariableInitPei.inf

  $(RP_PKG)/Platform/Pei/PlatformInfo/PlatformInfo.inf
  $(PLATFORM_PKG)/PlatformInit/PlatformInitPei/PlatformInitPreMem.inf {
    <LibraryClasses>
      TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
      BoardInitLib|$(RP_PKG)/Library/BoardInitLib/BoardInitPreMemLib.inf
  }
  $(PLATFORM_PKG)/PlatformInit/ReportFv/ReportFvPei.inf

  $(PLATFORM_PKG)/PlatformInit/SiliconPolicyPei/SiliconPolicyPeiPreMem.inf{
    <LibraryClasses>
      SiliconWorkaroundLib|WhitleySiliconPkg/Library/SiliconWorkaroundLibNull/SiliconWorkaroundLibNull.inf
  }
  $(RP_PKG)/Platform/Pei/EmulationPlatformInit/EmulationPlatformInit.inf
  $(PLATFORM_PKG)/PlatformInit/PlatformInitPei/PlatformInitPostMem.inf {
    <LibraryClasses>
      TestPointCheckLib|MinPlatformPkg/Test/Library/TestPointCheckLibNull/TestPointCheckLibNull.inf
      BoardInitLib|$(PLATFORM_PKG)/PlatformInit/Library/BoardInitLibNull/BoardInitLibNull.inf
  }

  IntelFsp2WrapperPkg/FspmWrapperPeim/FspmWrapperPeim.inf
!if ($(FSP_MODE) == 0)
  IntelFsp2WrapperPkg/FspsWrapperPeim/FspsWrapperPeim.inf
  $(RP_PKG)/Platform/Pei/DummyPchSpi/DummyPchSpi.inf
!endif

  $(RP_PKG)/BiosInfo/BiosInfo.inf

  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationPei.inf

  UefiCpuPkg/CpuMpPei/CpuMpPei.inf

  UefiCpuPkg/Universal/Acpi/S3Resume2Pei/S3Resume2Pei.inf {
    <LibraryClasses>
    !if $(PERFORMANCE_ENABLE) == TRUE
      TimerLib|UefiCpuPkg/Library/SecPeiDxeTimerLibUefiCpu/SecPeiDxeTimerLibUefiCpu.inf
    !endif
  }

[Components.X64]
  !include WhitleyOpenBoardPkg/Include/Dsc/CoreDxeInclude.dsc

  $(RP_PKG)/Platform/Dxe/PlatformType/PlatformType.inf

  MinPlatformPkg/Test/TestPointDumpApp/TestPointDumpApp.inf

  MdeModulePkg/Universal/SectionExtractionDxe/SectionExtractionDxe.inf
  MdeModulePkg/Universal/Acpi/S3SaveStateDxe/S3SaveStateDxe.inf
  MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf

  MdeModulePkg/Universal/LockBox/SmmLockBox/SmmLockBox.inf
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationSmm.inf

  ShellPkg/Application/Shell/Shell.inf {
      <LibraryClasses>
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
      IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf

    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }

  $(RP_PKG)/Cpu/Dxe/PlatformCpuPolicy/PlatformCpuPolicy.inf
  UefiCpuPkg/CpuDxe/CpuDxe.inf
  UefiCpuPkg/CpuS3DataDxe/CpuS3DataDxe.inf

  $(RP_PKG)/Features/Pci/Dxe/PciHostBridge/PciHostBridge.inf
  $(PLATFORM_PKG)/Flash/SpiFvbService/SpiFvbServiceSmm.inf

  $(RP_PKG)/Features/Pci/Dxe/PciPlatform/PciPlatform.inf

  $(RP_PKG)/Features/AcpiVtd/AcpiVtd.inf

  $(PLATFORM_PKG)/Acpi/AcpiSmm/AcpiSmm.inf {
    <LibraryClasses>
      BoardAcpiEnableLib|$(RP_PKG)/Library/BoardAcpiLib/SmmBoardAcpiEnableLib.inf
  }

  $(PLATFORM_PKG)/PlatformInit/PlatformInitDxe/PlatformInitDxe.inf {
  <LibraryClasses>
    BoardInitLib|$(RP_PKG)/Library/BoardInitLib/BoardInitDxeLib.inf
  }
  $(RP_PKG)/Platform/Dxe/S3NvramSave/S3NvramSave.inf {
!if ($(FSP_MODE) == 0)
    <BuildOptions>
        *_*_*_CC_FLAGS  = -D FSP_API_MODE
!endif
  }

  $(PLATFORM_PKG)/FspWrapper/SaveMemoryConfig/SaveMemoryConfig.inf

  $(PLATFORM_SI_BIN_PACKAGE)/CpxMicrocode/MicrocodeUpdates.inf
  $(PLATFORM_SI_BIN_PACKAGE)/IcxMicrocode/MicrocodeUpdates.inf

  MdeModulePkg/Bus/Pci/PciSioSerialDxe/PciSioSerialDxe.inf
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  BoardModulePkg/LegacySioDxe/LegacySioDxe.inf
  BoardModulePkg/BoardBdsHookDxe/BoardBdsHookDxe.inf

  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf

  MdeModulePkg/Universal/PlatformDriOverrideDxe/PlatformDriOverrideDxe.inf

  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  MdeModulePkg/Universal/SmbiosMeasurementDxe/SmbiosMeasurementDxe.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf

  #
  # SiliconPkg code for Platform Integration are defined here
  #
!if $(CPUTARGET) == "CPX"
  DEFINE CPU_CPX_SUPPORT                     = TRUE
!else
  DEFINE CPU_CPX_SUPPORT                     = FALSE
!endif
[PcdsFixedAtBuild]
!if ($(CPU_SKX_ONLY_SUPPORT) == TRUE)
  gSiPkgTokenSpaceGuid.PcdPostedCsrAccessSupported         |FALSE
!endif
[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER, LibraryClasses.common.UEFI_APPLICATION]
  ResetSystemLib|MdeModulePkg/Library/DxeResetSystemLib/DxeResetSystemLib.inf
[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  ResetSystemLib|MdeModulePkg/Library/RuntimeResetSystemLib/RuntimeResetSystemLib.inf


###################################################################################################
#
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process. They can be
#                        applied for any modules or only those modules with the specific
#                        module style (EDK or EDKII) specified in [Components] section.
#
###################################################################################################
[BuildOptions.Common.EDKII]
# Append build options for EDK and EDKII drivers (= is Append, == is Replace)
!if $(CRB_FLAG_ENABLE) == TRUE
  DEFINE CRB_EDKII_BUILD_OPTIONS = -D CRB_FLAG
!else
  DEFINE CRB_EDKII_BUILD_OPTIONS =
!endif

!if $(DEBUG_FLAGS_ENABLE) == TRUE
  DEFINE EDKII_DEBUG_BUILD_OPTIONS = -D DEBUG_CODE_BLOCK=1 -D PLATFORM_VARIABLE_ATTRIBUTES=0x3
!else
  DEFINE EDKII_DEBUG_BUILD_OPTIONS = -D SILENT_MODE -D PLATFORM_VARIABLE_ATTRIBUTES=0x3
!endif

!if $(SPARING_SCRATCHPAD_ENABLE) == TRUE
  DEFINE SPARING_SCRATCHPAD_OPTION = -D SPARING_SCRATCHPAD_SUPPORT
!else
  DEFINE SPARING_SCRATCHPAD_OPTIONS =
!endif

!if $(SCRATCHPAD_DEBUG) == TRUE
  DEFINE SCRATCHPAD_DEBUG_OPTION = -D SCRATCHPAD_DEBUG
!else
  DEFINE SCRATCHPAD_DEBUG_OPTION =
!endif

!if $(PCH_SERVER_BIOS_ENABLE) == TRUE
  DEFINE PCH_BUILD_OPTION = -DPCH_SERVER_BIOS_FLAG=1
!else
  DEFINE PCH_BUILD_OPTION =
!endif

!if $(SERVER_BIOS_ENABLE) == TRUE
  DEFINE SERVER_BUILD_OPTION = -DSERVER_BIOS_FLAG=1
!else
  DEFINE SERVER_BUILD_OPTION =
!endif

DEFINE SC_PATH = -D SC_PATH="Pch/SouthClusterLbg"

DEFINE ME_PATH = -D ME_PATH="Me/MeSps.4"

DEFINE IE_PATH = -D IE_PATH="Ie/v1"

DEFINE NVDIMM_OPTIONS =

!if $(CPUTARGET) == "ICX"
  DEFINE CPU_TYPE_OPTIONS  = -D ICX_HOST -D A0_HOST -D B0_HOST
!elseif $(CPUTARGET) == "CPX"
  DEFINE CPU_TYPE_OPTIONS  = -D SKX_HOST -D CLX_HOST -D CPX_HOST -D A0_HOST -D B0_HOST
!endif

DEFINE MAX_SOCKET_CORE_THREAD_OPTIONS = -D MAX_SOCKET=$(MAX_SOCKET) -D MAX_CORE=$(MAX_CORE) -D MAX_THREAD=$(MAX_THREAD)

DEFINE MRC_OPTIONS = -D LRDIMM_SUPPORT -D DDRT_SUPPORT

!if $(CPU_SKX_ONLY_SUPPORT) == FALSE
  DEFINE MAX_IMC_CH_OPTIONS = -D MAX_IMC=4 -D MAX_MC_CH=2
!else
  DEFINE MAX_IMC_CH_OPTIONS = -D MAX_IMC=2 -D MAX_MC_CH=3
!endif

DEFINE MAX_SAD_RULE_OPTION = -D MAX_SAD_RULES=24 -D MAX_DRAM_CLUSTERS=1

DEFINE LT_BUILD_OPTIONS = -D LT_FLAG

DEFINE FSP_BUILD_OPTIONS = -D FSP_DISPATCH_MODE_ENABLE=1

#
# MAX_KTI_PORTS needs to be updated based on the silicon type
#
!if $(CPUTARGET) == "CPX"
  DEFINE KTI_OPTIONS = -D MAX_KTI_PORTS=6
!else
  DEFINE KTI_OPTIONS = -D MAX_KTI_PORTS=3
!endif

DEFINE IIO_STACK_OPTIONS = -D MAX_IIO_STACK=6 -D MAX_LOGIC_IIO_STACK=8

DEFINE PCH_BIOS_BUILD_OPTIONS = $(PCH_BUILD_OPTION) $(SC_PATH) $(SERVER_BUILD_OPTION)

DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS = $(CRB_EDKII_BUILD_OPTIONS) $(EDKII_DEBUG_BUILD_OPTIONS) $(PCH_BIOS_BUILD_OPTIONS) $(PCH_PKG_OPTIONS) $(MAX_SOCKET_CORE_THREAD_OPTIONS) $(MAX_IMC_CH_OPTIONS) $(MAX_SAD_RULE_OPTION) $(KTI_OPTIONS) $(IIO_STACK_OPTIONS) $(LT_BUILD_OPTIONS) $(SECURITY_OPTIONS) $(SPARING_SCRATCHPAD_OPTION) $(SCRATCHPAD_DEBUG_OPTION) $(NVDIMM_OPTIONS) -D EFI_PCI_IOV_SUPPORT -D WHEA_SUPPORT $(CPU_TYPE_OPTIONS) -D MMCFG_BASE_ADDRESS=0x80000000 -D DISABLE_NEW_DEPRECATED_INTERFACES $(MRC_OPTIONS) $(FSP_BUILD_OPTIONS)

DEFINE IE_OPTIONS = $(IE_PATH) -DIE_SUPPORT=0

!if $(LINUX_GCC_BUILD) == TRUE
  DEFINE EDK2_LINUX_BUILD_OPTIONS = -D EDK2_CTE_BUILD
!else
  DEFINE EDK2_LINUX_BUILD_OPTIONS =
!endif

DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) $(EDK2_LINUX_BUILD_OPTIONS) $(IE_OPTIONS)

DEFINE ME_OPTIONS = -DSPS_VERSION=4 $(ME_PATH)

DEFINE ASPEED_ENABLE_BUILD_OPTIONS = -D ASPEED_ENABLE -D ESPI_ENABLE

DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) $(ME_OPTIONS) $(ASPEED_ENABLE_BUILD_OPTIONS)

  MSFT:*_*_*_CC_FLAGS= $(EDKII_DSC_FEATURE_BUILD_OPTIONS) /wd4819
  GCC:*_*_*_CC_FLAGS= $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_VFRPP_FLAGS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_APP_FLAGS   = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_PP_FLAGS    = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_ASLPP_FLAGS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_ASLCC_FLAGS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)


#
# Enable source level debugging for RELEASE build
#
!if $(TARGET) == "RELEASE"
  DEFINE EDKII_RELEASE_SRCDBG_ASM_BUILD_OPTIONS   =
  DEFINE EDKII_RELEASE_SRCDBG_CC_BUILD_OPTIONS    =
  DEFINE EDKII_RELEASE_SRCDBG_DLINK_BUILD_OPTIONS =

  MSFT:*_*_*_ASM_FLAGS   = $(EDKII_RELEASE_SRCDBG_ASM_BUILD_OPTIONS) /Zi
  MSFT:*_*_*_CC_FLAGS    = $(EDKII_RELEASE_SRCDBG_CC_BUILD_OPTIONS) /Z7
  MSFT:*_*_*_DLINK_FLAGS = $(EDKII_RELEASE_SRCDBG_DLINK_BUILD_OPTIONS) /DEBUG
  GCC:*_*_*_ASM_FLAGS    = $(EDKII_RELEASE_SRCDBG_ASM_BUILD_OPTIONS)
  GCC:*_*_*_CC_FLAGS     = $(EDKII_RELEASE_SRCDBG_CC_BUILD_OPTIONS)
  GCC:*_*_*_DLINK_FLAGS  = $(EDKII_RELEASE_SRCDBG_DLINK_BUILD_OPTIONS)
!endif

#
# Override ASL Compiler parameters in tools_def.template.
#
  MSFT:*_*_*_ASL_PATH == $(WORKSPACE)/../FDBin/Tools/IaslCompiler/6.3/iasl.exe
  GCC:*_*_*_ASL_PATH == $(WORKSPACE)/../FDBin/Tools/IaslCompiler/6.3/iasl
  *_*_*_ASL_FLAGS == -vr -we -oi
#
# Override the VFR compile flags to speed the build time
#

*_*_*_VFR_FLAGS                     ==  -n

#
# add to the build options for DXE/SMM drivers to remove the log message:
# !!!!!!!!  InsertImageRecord - Section Alignment(0x20) is not 4K  !!!!!!!!
#
[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER, BuildOptions.common.EDKII.DXE_SMM_DRIVER, BuildOptions.common.EDKII.SMM_CORE]
   MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096

[BuildOptions]
  GCC:*_GCC5_*_CC_FLAGS = -Wno-overflow -Wno-discarded-qualifiers -Wno-unused-variable -Wno-unused-but-set-variable -Wno-incompatible-pointer-types -mabi=ms
  GCC:*_GCC5_IA32_DLINK_FLAGS = -z common-page-size=0x20 -z muldefs
  GCC:*_GCC5_X64_DLINK_FLAGS  = -z common-page-size=0x20 -z muldefs
  MSFT:*_*_*_CC_FLAGS = /FAsc
