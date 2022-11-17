## @file
#
# Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Pcd Section - list of all PCD Entries defined by this board.
#
################################################################################

[PcdsFixedAtBuild.common]
  ######################################
  # Key Boot Stage and FSP configuration
  ######################################
  #
  # Please select the Boot Stage here.
  # Stage 1 - enable debug (system deadloop after debug init)
  # Stage 2 - mem init (system deadloop after mem init)
  # Stage 3 - boot to shell only
  # Stage 4 - boot to OS
  # Stage 5 - boot to OS with security boot enabled
  # Stage 6 - boot with advanced features enabled
  #
  gMinPlatformPkgTokenSpaceGuid.PcdBootStage|4

[PcdsFeatureFlag.common]
  gPlatformTokenSpaceGuid.PcdLockCsrSsidSvidRegister|FALSE
  # Server doesn't support capsle update on Reset.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmEnableBspElection|TRUE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmEnableBspElection|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuHotPlugSupport|TRUE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuHotPlugSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciBusHotplugDeviceSupport|FALSE

#S3 add
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|TRUE
#S3 add

  gEfiCpuTokenSpaceGuid.PcdCpuConroeFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuCedarMillFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuPrescottFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuNehalemFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuIvyBridgeFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSandyBridgeFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuHaswellFamilyFlag|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuSkylakeFamilyFlag|TRUE

  gEfiCpuTokenSpaceGuid.PcdCpuGateA20MDisableFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmDebug|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuSelectLfpAsBspFlag|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuSocketIdReassignmentFlag|TRUE

  ## This PCD specified whether ACPI SDT protocol is installed.
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdSerialTerminalEnable|FALSE

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 1
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 2
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 3
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 4
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 5
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|TRUE
!endif

!if $(TARGET) == DEBUG
  gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable|TRUE
!else
  gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable|FALSE
!endif

[PcdsFeatureFlag.X64]
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackGuard|FALSE

[PcdsFeatureFlag]
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowerGrayOutReadOnlyMenu|TRUE

[PcdsDynamicExDefault]
!include $(PROJECT)/StructureConfig.dsc

[PcdsFixedAtBuild.X64]
  gMinPlatformPkgTokenSpaceGuid.PcdTrustedConsoleOutputDevicePath|{0x02, 0x01, 0x0C, 0x00, 0xd0, 0x41, 0x03, 0x0A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x06, 0x00,  0x00, 0x01, 0x01, 0x01, 0x06, 0x00,  0x00, 0x01, 0x7F, 0xFF, 0x04, 0x00}

[PcdsFixedAtBuild.IA32]
  gCpuUncoreTokenSpaceGuid.PcdFlashSecCacheRegionBase|0x00FFA00000
  gCpuUncoreTokenSpaceGuid.PcdFlashSecCacheRegionSize|0x0000600000

[PcdsFixedAtBuild.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|TRUE
!if $(TARGET) == "RELEASE"
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x03
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif
!if $(TARGET) == RELEASE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|TRUE
!endif
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x0
#S3 modified
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x10000
  gEfiMdeModulePkgTokenSpaceGuid.PcdShadowPeimOnS3Boot|TRUE
#S3 modified

  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x0
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdFSBClock|133333333
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize|0x100000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxSizeNonPopulateCapsule|0x1700000

  gEfiCpuTokenSpaceGuid.PcdCpuIEDRamSize|0x400000
  gEfiCpuTokenSpaceGuid.PcdCpuSmmApSyncTimeout|10000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|10000
  gEfiCpuTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|512
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|512
  gEfiCpuTokenSpaceGuid.PcdPlatformType|2
  gEfiCpuTokenSpaceGuid.PcdPlatformCpuMaxCoreFrequency|4000
  gEfiCpuTokenSpaceGuid.PcdPlatformCpuMaxFsbFrequency|1066
  gEfiCpuTokenSpaceGuid.PcdCpuSmmStackSize|0x10000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x10000

  ## Specifies delay value in microseconds after sending out an INIT IPI.
  # @Prompt Configure delay value after send an INIT IPI
  gUefiCpuPkgTokenSpaceGuid.PcdCpuInitIpiDelayInMicroSeconds|10

  ## Specifies max supported number of Logical Processors.
  # @Prompt Configure max supported number of Logical Processorss
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|512
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApStackSize|0x1000
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
!endif

  gPlatformTokenSpaceGuid.PcdBusStack|0x06
  gPlatformTokenSpaceGuid.PcdUboDev|0x08
  gPlatformTokenSpaceGuid.PcdUboFunc|0x02
  gPlatformTokenSpaceGuid.PcdUboCpuBusNo0|0xCC

  gEfiCpuTokenSpaceGuid.PcdCpuIEDEnabled|TRUE

  ## Defines the ACPI register set base address.
  #  The invalid 0xFFFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Timer IO Port Address
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress         |  0x0500

  ## Defines the PCI Bus Number of the PCI device that contains the BAR and Enable for ACPI hardware registers.
  # @Prompt ACPI Hardware PCI Bus Number
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBusNumber            |  0x00

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00000002
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultCreatorId|0x4C544E49
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultCreatorRevision|0x20091013

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

  ## Defines the bit mask to retrieve ACPI IO Port Base Address
  # @Prompt ACPI IO Port Base Address Mask
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddressMask     |0xFFFC

  # Indicates the max nested level
  gEfiCpRcPkgTokenSpaceGuid.PcdMaxNestedLevel|0x00000010

  gEfiCpRcPkgTokenSpaceGuid.PcdMaxCpuSocketCount|$(MAX_SOCKET)
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuSocketCount|$(MAX_SOCKET)
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuCoreCount|28

  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE

  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiAcpiReclaimMemorySize|0x70
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiAcpiNvsMemorySize|0x80
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiReservedMemorySize|0x1470
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtDataMemorySize|0xA0
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtCodeMemorySize|0x80

  #
  # The PCDs are used to control the Windows SMM Security Mitigations Table - Protection Flags
  #
  # BIT0: If set, expresses that for all synchronous SMM entries,SMM will validate that input and output buffers lie entirely within the expected fixed memory regions.
  # BIT1: If set, expresses that for all synchronous SMM entries, SMM will validate that input and output pointers embedded within the fixed communication buffer only refer to address ranges \
  #       that lie entirely within the expected fixed memory regions.
  # BIT2: Firmware setting this bit is an indication that it will not allow reconfiguration of system resources via non-architectural mechanisms.
  # BIT3-31: Reserved
  #
  gMinPlatformPkgTokenSpaceGuid.PcdWsmtProtectionFlags|0x07

[PcdsFixedAtBuild.X64]
  gPcAtChipsetPkgTokenSpaceGuid.PcdMinimalValidYear|2015
  gPcAtChipsetPkgTokenSpaceGuid.PcdMaximalValidYear|2099
  # Change PcdBootManagerMenuFile to UiApp
##

  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

  gEfiCpuTokenSpaceGuid.PcdCpuSmmCodeAccessCheckEnable |TRUE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmCodeAccessCheckEnable |TRUE

  gMinPlatformPkgTokenSpaceGuid.PcdFadtPreferredPmProfile|0x04
  gMinPlatformPkgTokenSpaceGuid.PcdFadtIaPcBootArch|0x0000
  gMinPlatformPkgTokenSpaceGuid.PcdFadtFlags|0x000004A5

  gMinPlatformPkgTokenSpaceGuid.PcdAcpiEnableSwSmi|0xA0
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiDisableSwSmi|0xA1

  gMinPlatformPkgTokenSpaceGuid.PcdLocalApicAddress|0xFEE00000
  gMinPlatformPkgTokenSpaceGuid.PcdIoApicAddress|0xFEC00000
  gMinPlatformPkgTokenSpaceGuid.PcdIoApicId|0x08

  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicCount|32
  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicIdBase|0x09
  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicAddressBase|0xFEC01000
  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicInterruptBase|24

  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1AEventBlockAddress|0x500
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1BEventBlockAddress|0
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1AControlBlockAddress|0x504
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1BControlBlockAddress|0
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm2ControlBlockAddress|0x550
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPmTimerBlockAddress|0x508
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe0BlockAddress|0x580
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe1BlockAddress|0

[PcdsPatchableInModule.common]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000042

!if gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmiHandlerProfilePropertyMask|0x1
!endif

  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0x80000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x10000000
  gPcAtChipsetPkgTokenSpaceGuid.PcdHpetBaseAddress|0xFED00000

  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedIobase           |0x1000
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedIoLimit          |0xFFFF
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemBase          |0x90000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemLimit         |0xFBFFFFFF
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemAbove4GBBase  |0x380000000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemAbove4GBLimit |0x3803FFFFFFFF

  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|800
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|600

  gEfiCpuTokenSpaceGuid.PcdCpuSmmUseDelayIndication|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmUseBlockIndication|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmUseSmmEnableIndication|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdUse1GPageTable|TRUE

  gMinPlatformPkgTokenSpaceGuid.PcdFlashAreaBaseAddress|0xFF800000
  gMinPlatformPkgTokenSpaceGuid.PcdFlashAreaSize|0x01400000

[PcdsDynamicExDefault.common.DEFAULT]
  gEfiCpuTokenSpaceGuid.PcdCpuProcessorFeatureUserConfiguration|0x002CF6CF
  gEfiCpuTokenSpaceGuid.PcdCpuProcessorFeatureUserConfigurationEx1|0
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|30000
  gEfiCpuTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|200000
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|TRUE

  gUefiCpuPkgTokenSpaceGuid.PcdCpuS3DataAddress|0

  gEfiPchTokenSpaceGuid.PcdWakeOnRTCS5|FALSE
  gEfiPchTokenSpaceGuid.PcdRtcWakeupTimeHour|0
  gEfiPchTokenSpaceGuid.PcdRtcWakeupTimeMinute|0
  gEfiPchTokenSpaceGuid.PcdRtcWakeupTimeSecond|0

  gOemSkuTokenSpaceGuid.PcdOemSkuUplinkPortIndex|0x5

[PcdsDynamicExHii.common.DEFAULT]
!if gPlatformTokenSpaceGuid.PcdFastBoot == FALSE
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|3 # Variable: L"Timeout"
!else
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|0 # Variable: L"Timeout"
!endif
  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"


[PcdsDynamicExDefault]

  gOemSkuTokenSpaceGuid.PcdForceTo1SConfigMode|FALSE

## *** PURLEY_PPO *** - Added in 8th segment in PcdPcieMmcfgTablePtr to fix size assert in PcieAddressLib.c
##                                                 |                           MMCFG Table Header                                 |                               Segment 0                                        |                              Segment 1                                       |                               Segment 2                                        |                               Segment 3                                       |                               Segment 4                                        |                               Segment 5                                       |                               Segment 6                                       |                               Segment 7                                      |                                     Segment 8
  gEfiCpRcPkgTokenSpaceGuid.PcdPcieMmcfgTablePtr|{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  gEfiCpuTokenSpaceGuid.PcdCpuEnergyPolicy|0
  gEfiCpuTokenSpaceGuid.PcdCpuAcpiLvl2Addr|0
  gEfiCpuTokenSpaceGuid.PcdCpuPackageCStateLimit|0
  gEfiCpuTokenSpaceGuid.PcdCpuCoreCStateValue|0
  gEfiCpuTokenSpaceGuid.PcdCpuClockModulationDutyCycle|0
  gEfiCpuTokenSpaceGuid.PcdCpuHwCoordination|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuDcuMode|0
  gEfiCpuTokenSpaceGuid.PcdCpuTurboOverride|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuProcessorMsrLockCtrl|0
  gEfiCpuTokenSpaceGuid.PcdCpuIioLlcWaysBitMask|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuExpandedIioLlcWaysBitMask|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuRemoteWaysBitMask|0x0
  gEfiCpuTokenSpaceGuid.PcdPchTraceHubEn|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuQlruCfgBitMask|0x0
  gEfiCpuTokenSpaceGuid.PcdSbspSelection|0xFF
#  gEfiCpuTokenSpaceGuid.PcdCpuSocketId|{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x3,0x0,0x0,0x0,0x3,0x0,0x0,0x0}
  gEfiCpuTokenSpaceGuid.PcdCpuPmStructAddr|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuRRQCountThreshold|0x0

  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdMrIovSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSystemPageSize|0x1F

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L""|VOID*|36

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemId|{0x49, 0x4E, 0x54, 0x45, 0x4C, 0x20}
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x2046573030363253

  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicEnable|0x0

[PcdsDynamicExDefault.X64]

  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|115200
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultDataBits|8
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultParity|1
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultStopBits|1
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|0

  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|31
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|100

  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|800
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|600

  gUefiCpuPkgTokenSpaceGuid.PcdCpuHotPlugDataAddress|0
