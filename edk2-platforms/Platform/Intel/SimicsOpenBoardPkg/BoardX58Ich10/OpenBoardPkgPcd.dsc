## @file
#  PCD configuration build description file for the X58Ich10 board.
#
# Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Pcd Section - list of all PCD Entries used by this board.
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
  #
  gMinPlatformPkgTokenSpaceGuid.PcdBootStage|4

[PcdsFeatureFlag.common]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowerGrayOutReadOnlyMenu|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutUgaSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSupportUefiDecompress|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciBusHotplugDeviceSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|FALSE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuHotPlugSupport|FALSE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmEnableBspElection|FALSE

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
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

  ######################################
  # Silicon Configuration
  ######################################
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  gSimicsX58PkgTokenSpaceGuid.PcdSmmSmramRequire|TRUE
!endif

  ######################################
  # Advanced Feature Configuration
  ######################################
  gNetworkFeaturePkgTokenSpaceGuid.PcdNetworkFeatureEnable|TRUE
  gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosFeatureEnable|TRUE

[PcdsFeatureFlag.X64]
  ######################################
  # Edk2 Configuration
  ######################################
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackGuard|FALSE

[PcdsFixedAtBuild.common]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultCreatorId|0x4C544E49
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultCreatorRevision|0x20091013
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00000002
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize|0x10000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize|0x2000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize|0xc000
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxHardwareErrorVariableSize|0x8000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxSizeNonPopulateCapsule|0x1700000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x400
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize|0x100000
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdShadowPeimOnS3Boot|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeMemorySize|1
  gEfiMdeModulePkgTokenSpaceGuid.PcdVariableStoreSize|0xc000
  gEfiMdeModulePkgTokenSpaceGuid.PcdVpdBaseAddress|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseMemory|TRUE
!if $(TARGET) == RELEASE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|TRUE
!endif
  # DEBUG_INIT      0x00000001  // Initialization
  # DEBUG_WARN      0x00000002  // Warnings
  # DEBUG_LOAD      0x00000004  // Load events
  # DEBUG_FS        0x00000008  // EFI File system
  # DEBUG_POOL      0x00000010  // Alloc & Free (pool)
  # DEBUG_PAGE      0x00000020  // Alloc & Free (page)
  # DEBUG_INFO      0x00000040  // Informational debug messages
  # DEBUG_DISPATCH  0x00000080  // PEI/DXE/SMM Dispatchers
  # DEBUG_VARIABLE  0x00000100  // Variable
  # DEBUG_BM        0x00000400  // Boot Manager
  # DEBUG_BLKIO     0x00001000  // BlkIo Driver
  # DEBUG_NET       0x00004000  // SNP Driver
  # DEBUG_UNDI      0x00010000  // UNDI Driver
  # DEBUG_LOADFILE  0x00020000  // LoadFile
  # DEBUG_EVENT     0x00080000  // Event messages
  # DEBUG_GCD       0x00100000  // Global Coherency Database changes
  # DEBUG_CACHE     0x00200000  // Memory range cachability changes
  # DEBUG_VERBOSE   0x00400000  // Detailed debug messages that may
  #                             // significantly impact boot performance
  # DEBUG_ERROR     0x80000000  // Error
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x8000004F
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
  gEfiMdePkgTokenSpaceGuid.PcdFSBClock|133333333
  gEfiMdePkgTokenSpaceGuid.PcdMaximumGuidedExtractHandler|0x10
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x0
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
!endif
!if $(TARGET) == "RELEASE"
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x03
!else
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoBarEnableMask| 0x80
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBarRegisterOffset|0x0040
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBusNumber| 0x00
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciDeviceNumber| 0x1F
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciEnableRegisterOffset|0x0044
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciFunctionNumber| 0x00
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress|0x0400
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddressMask|0xFFFC
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiPm1TmrOffset|0x0008
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApStackSize|0x1000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuInitIpiDelayInMicroSeconds|10
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|512
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|10000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x4000

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe0BlockAddress|0x420
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe1BlockAddress|0
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1AControlBlockAddress|0x404
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1AEventBlockAddress|0x400
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1BControlBlockAddress|0
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1BEventBlockAddress|0
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm2ControlBlockAddress|0x450
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPmTimerBlockAddress|0x408
  gMinPlatformPkgTokenSpaceGuid.PcdFadtFlags|0x000004A5
  gMinPlatformPkgTokenSpaceGuid.PcdFadtIaPcBootArch|0x0003
  gMinPlatformPkgTokenSpaceGuid.PcdFadtPreferredPmProfile|0x0
  gMinPlatformPkgTokenSpaceGuid.PcdLocalApicAddress|0xFEE00000
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuCoreCount|128
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuSocketCount|4
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuThreadCount|4
  gMinPlatformPkgTokenSpaceGuid.PcdPcIoApicAddressBase|0xFEC01000

  ######################################
  # Board Configuration
  ######################################
  gBoardModulePkgTokenSpaceGuid.PcdPs2KbMsEnable|1
  gBoardModulePkgTokenSpaceGuid.PcdSuperIoPciIsaBridgeDevice|{0x00, 0x00, 0x1F, 0x00}

[PcdsFixedAtBuild.X64]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }
  gPcAtChipsetPkgTokenSpaceGuid.Pcd8259LegacyModeMask|0x0eB8
  gPcAtChipsetPkgTokenSpaceGuid.PcdMinimalValidYear|2015
  gPcAtChipsetPkgTokenSpaceGuid.PcdMaximalValidYear|2099
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmCodeAccessCheckEnable |TRUE

 [PcdsPatchableInModule.common]
  ######################################
  # Edk2 Configuration
  ######################################
!if gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmiHandlerProfilePropertyMask|0x1
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdUse1GPageTable|TRUE
  gPcAtChipsetPkgTokenSpaceGuid.PcdHpetBaseAddress|0xFED00000

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvMicrocodeBase|0x0
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvMicrocodeSize|0x0

[PcdsDynamicDefault]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiS3Enable|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvStoreReserved|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase64|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1024
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|600

  ######################################
  # Board Configuration
  ######################################
  gSimicsOpenBoardPkgTokenSpaceGuid.PcdPciIoBase|0x0
  gSimicsOpenBoardPkgTokenSpaceGuid.PcdPciIoSize|0x0
  gSimicsOpenBoardPkgTokenSpaceGuid.PcdPciMmio32Base|0x0
  gSimicsOpenBoardPkgTokenSpaceGuid.PcdPciMmio32Size|0x0
  gSimicsOpenBoardPkgTokenSpaceGuid.PcdPciMmio64Base|0x0
  gSimicsOpenBoardPkgTokenSpaceGuid.PcdPciMmio64Size|0x800000000
  gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsX58HostBridgePciDevId|0

  ######################################
  # Advanced Feature Configuration
  ######################################
  gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosType0StringBiosReleaseDate|"2019-08-09"
  gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosType0StringBiosVersion|"Ver.1.0.0"
  gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosType1StringProductName|"QSP UEFI BIOS"
  gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosType2StringProductName|"QSP UEFI BIOS"

[PcdsDynamicExDefault]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemId|{0x49, 0x4E, 0x54, 0x45, 0x4C, 0x20}
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x2046573030363253
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L""|VOID*|36
  gEfiMdeModulePkgTokenSpaceGuid.PcdMrIovSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSystemPageSize|0x1F
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|30000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuS3DataAddress|0

[PcdsDynamicExDefault.X64]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|100
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|31
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|800
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|600
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|0
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|115200
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultDataBits|8
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultParity|1
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultStopBits|1
  gUefiCpuPkgTokenSpaceGuid.PcdCpuHotPlugDataAddress|0

[PcdsDynamicExHii.common.DEFAULT]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|50 # Variable: L"Timeout"
  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"
