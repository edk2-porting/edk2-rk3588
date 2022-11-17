#/** @file
# Platform description.
#
# Copyright (c) 2012  - 2021, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
#
#**/

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = Vlv2TbltDevicePkg
  PLATFORM_GUID                       = 465B0A0B-7AC1-443b-8F67-7B8DEC145F90
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/Vlv2TbltDevicePkgIA32
  SUPPORTED_ARCHITECTURES             = IA32
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = DEFAULT
  VPD_TOOL_GUID                       = 8C3D856A-9BE6-468E-850A-24F7A8D38E08
  PREBUILD                            = python Vlv2TbltDevicePkg/PreBuild.py
  POSTBUILD                           = python Vlv2TbltDevicePkg/Feature/Capsule/GenerateCapsule/GenCapsuleAll.py

  DEFINE RC_BINARY_RELEASE        = TRUE
  #
  # Platform On/Off features are defined here
  #
  !include Vlv2TbltDevicePkg/PlatformPkgConfig.dsc

  DEFINE      DXE_ARCHITECTURE        = IA32

  FLASH_DEFINITION                    = Vlv2TbltDevicePkg/PlatformPkg.fdf
!if $(LFMA_ENABLE) == TRUE
  FIX_LOAD_TOP_MEMORY_ADDRESS         = 0xFFFFFFFFFFFFFFFF
  DEFINE   TOP_MEMORY_ADDRESS         = 0xFFFFFFFFFFFFFFFF
!else
  FIX_LOAD_TOP_MEMORY_ADDRESS         = 0x0
  DEFINE   TOP_MEMORY_ADDRESS         = 0x0
!endif

  DEFINE   PLATFORM_PCIEXPRESS_BASE   = 0E0000000

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
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################

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

  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
!if $(SSE2_ENABLE) == TRUE
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibSse2/BaseMemoryLibSse2.inf
!else
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
!endif
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
!if $(RC_BINARY_RELEASE) == TRUE
  PchPlatformLib|Vlv2TbltDevicePkg/Library/PchPlatformLib/PchPlatformLib.inf
!endif
  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiTianoCustomDecompressLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  UefiCpuLib|UefiCpuPkg/Library/BaseUefiCpuLib/BaseUefiCpuLib.inf
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf
  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  FlashDeviceLib|Vlv2TbltDevicePkg/Library/FlashDeviceLib/FlashDeviceLib.inf
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  #
  # Framework
  #
!if $(S3_ENABLE) == TRUE
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
!else
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
!endif
  S3IoLib|MdePkg/Library/BaseS3IoLib/BaseS3IoLib.inf
  S3PciLib|MdePkg/Library/BaseS3PciLib/BaseS3PciLib.inf

  #
  # Generic Modules
  #
!if $(USB_ENABLE) == TRUE
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
!endif
!if $(SCSI_ENABLE) == TRUE
  UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
!endif

  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
!if $(CAPSULE_ENABLE) == TRUE
  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibFmp/DxeCapsuleLib.inf
!else
  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
!endif
  FmpAuthenticationLib|MdeModulePkg/Library/FmpAuthenticationLibNull/FmpAuthenticationLibNull.inf
  IniParsingLib|SignedCapsulePkg/Library/IniParsingLib/IniParsingLib.inf
  PlatformFlashAccessLib|Vlv2TbltDevicePkg/Feature/Capsule/Library/PlatformFlashAccessLib/PlatformFlashAccessLib.inf
  MicrocodeFlashAccessLib|Vlv2TbltDevicePkg/Feature/Capsule/Library/PlatformFlashAccessLib/PlatformFlashAccessLib.inf
  DisplayUpdateProgressLib|MdeModulePkg/Library/DisplayUpdateProgressLibGraphics/DisplayUpdateProgressLibGraphics.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  SecurityManagementLib|MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf
  IoApicLib|PcAtChipsetPkg/Library/BaseIoApicLib/BaseIoApicLib.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

  #
  # CPU
  #
  MtrrLib|UefiCpuPkg/Library/MtrrLib/MtrrLib.inf
  LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/DxeMpInitLib.inf
  VmgExitLib|UefiCpuPkg/Library/VmgExitLibNull/VmgExitLibNull.inf
  MicrocodeLib|UefiCpuPkg/Library/MicrocodeLib/MicrocodeLib.inf

  #
  # ICH
  #
  SmbusLib|MdePkg/Library/BaseSmbusLibNull/BaseSmbusLibNull.inf
  SmmLib|Vlv2TbltDevicePkg/Library/PchSmmLib/PchSmmLib.inf

  #
  # Platform
  #
  TimerLib|Vlv2TbltDevicePkg/Library/IntelPchAcpiTimerLib/IntelPchAcpiTimerLib.inf
  ResetSystemLib|Vlv2TbltDevicePkg/Library/ResetSystemLib/ResetSystemLib.inf

  PlatformCmosLib|Vlv2TbltDevicePkg/Library/PlatformCmosLib/PlatformCmosLib.inf

  #
  # Misc
  #
  MonoStatusCodeLib|Vlv2TbltDevicePkg/MonoStatusCode/MonoStatusCode.inf
!if $(TARGET) == RELEASE
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
!endif

  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
!if $(TPM_ENABLED) == TRUE
  TpmCommLib|SecurityPkg/Library/TpmCommLib/TpmCommLib.inf
  Tpm12CommandLib|SecurityPkg/Library/Tpm12CommandLib/Tpm12CommandLib.inf
  Tpm12DeviceLib|SecurityPkg/Library/Tpm12DeviceLibDTpm/Tpm12DeviceLibDTpm.inf
!endif

  PlatformHookLib|MdeModulePkg/Library/BasePlatformHookLibNull/BasePlatformHookLibNull.inf

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  PeCoffExtraActionLib|SourceLevelDebugPkg/Library/PeCoffExtraActionLibDebug/PeCoffExtraActionLibDebug.inf
  DebugCommunicationLib|SourceLevelDebugPkg/Library/DebugCommunicationLibSerialPort/DebugCommunicationLibSerialPort.inf
  SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
!else
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
  DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
!endif

  #
  # CryptLib
  #
!if $(TPM_ENABLED) == TRUE
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
!else
  TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
!endif

  BiosIdLib|BoardModulePkg/Library/BiosIdLib/DxeBiosIdLib.inf

  StallSmmLib|Vlv2TbltDevicePkg/Library/StallSmmLib/StallSmmLib.inf

!if $(SECURE_BOOT_ENABLE) == TRUE
  PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf
  AuthVariableLib|SecurityPkg/Library/AuthVariableLib/AuthVariableLib.inf
!else
  AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
!endif

  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf

  VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf
  VariablePolicyLib|MdeModulePkg/Library/VariablePolicyLib/VariablePolicyLib.inf
  VariablePolicyHelperLib|MdeModulePkg/Library/VariablePolicyHelperLib/VariablePolicyHelperLib.inf
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
!if $(NETWORK_TLS_ENABLE) == TRUE
  TlsLib|CryptoPkg/Library/TlsLib/TlsLib.inf
!endif
  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
  Tcg2PhysicalPresenceLib|SecurityPkg/Library/DxeTcg2PhysicalPresenceLib/DxeTcg2PhysicalPresenceLib.inf
  Tcg2PpVendorLib|SecurityPkg/Library/Tcg2PpVendorLibNull/Tcg2PpVendorLibNull.inf

  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf

!if $(RECOVERY_ENABLE)
  EdkiiSystemCapsuleLib|SignedCapsulePkg/Library/EdkiiSystemCapsuleLib/EdkiiSystemCapsuleLib.inf
!endif

[LibraryClasses.IA32.SEC]
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
!endif
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

[LibraryClasses.IA32.PEIM, LibraryClasses.IA32.PEI_CORE, LibraryClasses.IA32.SEC]
  #
  # PEI phase common
  #

  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  MultiPlatformLib|Vlv2TbltDevicePkg/Library/MultiPlatformLib/MultiPlatformLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/PeiCryptLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SecPeiCpuExceptionHandlerLib.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf

!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
  TimerLib|Vlv2TbltDevicePkg/Library/IntelPchAcpiTimerLib/IntelPchAcpiTimerLib.inf
!endif

!if $(TARGET) == RELEASE
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!else
  DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
  SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
!endif

  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxPeiLib.inf
  HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterPei.inf
!if $(SOURCE_DEBUG_ENABLE) == TRUE
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/SecPeiDebugAgentLib.inf
!endif

[LibraryClasses.IA32]
  #
  # DXE phase common
  #
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf

  Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibTcg2/Tpm2DeviceLibTcg2.inf
!if $(TPM_ENABLED) == TRUE
  TcgPhysicalPresenceLib|SecurityPkg/Library/DxeTcgPhysicalPresenceLib/DxeTcgPhysicalPresenceLib.inf
!endif

  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxDxeLib.inf
  EfiRegTableLib|Vlv2TbltDevicePkg/Library/EfiRegTableLib/EfiRegTableLib.inf
  HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterDxe.inf
  MmUnblockMemoryLib|MdePkg/Library/MmUnblockMemoryLib/MmUnblockMemoryLibNull.inf

[LibraryClasses.IA32.DXE_DRIVER]
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  TimerLib|Vlv2TbltDevicePkg/Library/IntelPchAcpiTimerLib/IntelPchAcpiTimerLib.inf
!endif

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/DxeDebugAgentLib.inf
!endif

  FlashDeviceLib|Vlv2TbltDevicePkg/Library/FlashDeviceLib/FlashDeviceLibDxe.inf

[LibraryClasses.IA32.DXE_CORE]
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  MemoryAllocationLib|MdeModulePkg/Library/DxeCoreMemoryAllocationLib/DxeCoreMemoryAllocationLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/DxeCorePerformanceLib/DxeCorePerformanceLib.inf
  TimerLib|Vlv2TbltDevicePkg/Library/IntelPchAcpiTimerLib/IntelPchAcpiTimerLib.inf
!endif

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/DxeDebugAgentLib.inf
!endif

[LibraryClasses.IA32.DXE_SMM_DRIVER]
  MmServicesTableLib|MdePkg/Library/MmServicesTableLib/MmServicesTableLib.inf
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxSmmLib.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf
  SmmCpuPlatformHookLib|UefiCpuPkg/Library/SmmCpuPlatformHookLibNull/SmmCpuPlatformHookLibNull.inf
  SmmCpuFeaturesLib|UefiCpuPkg/Library/SmmCpuFeaturesLib/SmmCpuFeaturesLib.inf

  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf
  !if $(TARGET) != RELEASE
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  !endif

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/SmmDebugAgentLib.inf
  TimerLib|Vlv2TbltDevicePkg/Library/IntelPchAcpiTimerLib/IntelPchAcpiTimerLib.inf
!endif
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SmmCpuExceptionHandlerLib.inf

!if $(TPM_ENABLED) == TRUE
  TcgPpVendorLib|SecurityPkg/Library/TcgPpVendorLibNull/TcgPpVendorLibNull.inf
!endif

[LibraryClasses.IA32.SMM_CORE]
  MemoryAllocationLib|MdeModulePkg/Library/PiSmmCoreMemoryAllocationLib/PiSmmCoreMemoryAllocationLib.inf
  SmmServicesTableLib|MdeModulePkg/Library/PiSmmCoreSmmServicesTableLib/PiSmmCoreSmmServicesTableLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
  SmmCorePlatformHookLib|MdeModulePkg/Library/SmmCorePlatformHookLibNull/SmmCorePlatformHookLibNull.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf

!if $(TPM_ENABLED) == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf
!endif

  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf

!if $(TARGET) != RELEASE
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif

[LibraryClasses.IA32.DXE_RUNTIME_DRIVER]
  ReportStatusCodeLib|MdeModulePkg/Library/RuntimeDxeReportStatusCodeLib/RuntimeDxeReportStatusCodeLib.inf
  VariablePolicyLib|MdeModulePkg/Library/VariablePolicyLib/VariablePolicyLibRuntimeDxe.inf
!if $(SECURE_BOOT_ENABLE) == TRUE || $(TPM_ENABLED) == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/RuntimeCryptLib.inf
!endif

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/DxeDebugAgentLib.inf
!endif

!if $(CAPSULE_ENABLE) == TRUE
  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibFmp/DxeRuntimeCapsuleLib.inf
!endif

[LibraryClasses.common.UEFI_DRIVER]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/DxeDebugAgentLib.inf
!endif

[LibraryClasses.IA32.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/DxeDebugAgentLib.inf
!endif

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFeatureFlag.common]
!if $(MINI_BIOS_ENABLE) == FALSE
  gPlatformModuleTokenSpaceGuid.PcdBdsDispatchAdditionalOprom|TRUE
!else
  gPlatformModuleTokenSpaceGuid.PcdBdsDispatchAdditionalOprom|FALSE
!endif
#
# If PcdDxeIplSwitchToLongMode is TRUE, DxeIpl will load a 64-bit DxeCore and switch to long mode to hand over to DxeCore.
#
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|FALSE

  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserGrayOutTextStatement|TRUE

!if $(CAPSULE_RESET_ENABLE) == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|TRUE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|FALSE
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreImageLoaderSearchTeSectionFirst|FALSE
!if $(ISA_SERIAL_STATUS_CODE_ENABLE) == TRUE
  gEfiSerialPortTokenSpaceGuid.PcdStatusCodeUseIsaSerial|TRUE
!else
  gEfiSerialPortTokenSpaceGuid.PcdStatusCodeUseIsaSerial|FALSE
!endif
!if $(USB_SERIAL_STATUS_CODE_ENABLE) == TRUE
  gEfiSerialPortTokenSpaceGuid.PcdStatusCodeUseUsbSerial|TRUE
!else
  gEfiSerialPortTokenSpaceGuid.PcdStatusCodeUseUsbSerial|FALSE
!endif
!if $(RAM_SERIAL_STATUS_CODE_ENABLE) == TRUE
  gEfiSerialPortTokenSpaceGuid.PcdStatusCodeUseRam|TRUE
!else
  gEfiSerialPortTokenSpaceGuid.PcdStatusCodeUseRam|FALSE
!endif


!if $(VARIABLE_INFO_ENABLE) == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdVariableCollectStatistics|TRUE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdVariableCollectStatistics|FALSE
!endif

!if $(SOURCE_DEBUG_ENABLE)
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmDebug|TRUE
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmBlockStartupThisAp|TRUE

[PcdsFixedAtBuild.common]
!if $(SECURE_BOOT_ENABLE) == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x22000
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x4000
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x00000800
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxHardwareErrorVariableSize|0x400
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|FALSE
!if $(S4_ENABLE) == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|TRUE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
!endif
!if $(TARGET) == RELEASE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseMemory|FALSE
!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x3
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif
!if $(PERFORMANCE_ENABLE) == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|60
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|$(TOP_MEMORY_ADDRESS)
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserSubtitleTextColor|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserFieldTextColor|0x01

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x17
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
  gEfiSourceLevelDebugPkgTokenSpaceGuid.PcdDebugLoadImageMethod|2
!endif

  #
  # Set SMM stack size to 16 KB.
  #
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x4000

  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

  #
  # Clear unused single certificate PCD
  #
  gEfiSecurityPkgTokenSpaceGuid.PcdPkcs7CertBuffer|{0}

  #
  # Lock all updatable firmware devices at End of DXE
  #
  gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceLockEventGuid|{GUID(gEfiEndOfDxeEventGroupGuid)}
#  gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceLockEventGuid|{GUID(gEfiEventReadyToBootGuid)}

  #
  # Set PcdFmpDeviceTestKeySha256Digest to {0} to disable test key detection
  #
#  gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceTestKeySha256Digest|{0}

[PcdsFixedAtBuild.IA32]
!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x3
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif

!if $(RECOVERY_ENABLE)
  gEfiMdeModulePkgTokenSpaceGuid.PcdRecoveryFileName|L"VLV2REC.Cap"
!endif

[PcdsPatchableInModule.common]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x803805c6
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0x$(PLATFORM_PCIEXPRESS_BASE)

  #######################################################################################################
  #
  # Begin of MRC parameters
  #

  ## Memory Parameter Patchable.
  #  FALSE - MRC Parameters are fixed for MinnowBoard Max<BR>
  #  TRUE  - MRC Parameters are patchable by following PCDs<BR>
  # @Prompt Memory Parameter Patchable.
  # @ValidList 0x80000001 | 0, 1
  gVlvRefCodePkgTokenSpaceGuid.PcdMemoryParameterPatchable|FALSE

  ## Memory Down or DIMM slot.
  #  0 - DIMM<BR>
  #  1 - Memory Down<BR>
  # @Prompt Enable Memory Down
  # @ValidList 0x80000001 | 0, 1
  gVlvRefCodePkgTokenSpaceGuid.PcdEnableMemoryDown|1

  ## The speed of DRAM.
  #  0 - 800 MHz<BR>
  #  1 - 1066 MHz<BR>
  #  2 - 1333 MHz<BR>
  #  3 - 1600 MHz<BR>
  # @Prompt DRAM Speed
  # @ValidList 0x80000001 | 0, 1, 2, 3
  gVlvRefCodePkgTokenSpaceGuid.PcdDramSpeed|1

  ## DRAM Type.
  #  0 - DDR3<BR>
  #  1 - DDR3L<BR>
  #  2 - DDR3U<BR>
  #  3 - DDR3All<BR>
  #  4 - LPDDR2<BR>
  #  5 - LPDDR3<BR>
  #  6 - DDR4<BR>
  # @Prompt DRAM Type
  # @ValidList 0x80000001 | 0, 1, 2, 3, 4, 5, 6
  gVlvRefCodePkgTokenSpaceGuid.PcdDramType|1

  ## Please populate DIMM slot 0 if only one DIMM is supported.
  #  0 - Disable<BR>
  #  1 - Enable<BR>
  # @Prompt DIMM 0 Enable
  # @ValidList 0x80000001 | 0, 1
  gVlvRefCodePkgTokenSpaceGuid.PcdEnableDimm0|1

  ## DIMM 1 has to be identical to DIMM 0.
  #  0 - Disable<BR>
  #  1 - Enable<BR>
  # @Prompt DIMM 1 Enable Type
  # @ValidList 0x80000001 | 0, 1
  gVlvRefCodePkgTokenSpaceGuid.PcdEnableDimm1|0

  ## DRAM device data width.
  #  0 - x8<BR>
  #  1 - x16<BR>
  #  2 - x32<BR>
  # @Prompt DIMM_DWIDTH
  # @ValidList 0x80000001 | 0, 1, 2
  gVlvRefCodePkgTokenSpaceGuid.PcdDimmDataWidth|1

  ## DRAM device data density.
  #  0 - 1 Gbit<BR>
  #  1 - 2 Gbit<BR>
  #  2 - 4 Gbit<BR>
  #  3 - 8 Gbit<BR>
  # @Prompt DIMM_Density
  # @ValidList 0x80000001 | 0, 1, 2, 3
  gVlvRefCodePkgTokenSpaceGuid.PcdDimmDensity|2

  ## DRAM device data bus width.
  #  0 - 8 bits<BR>
  #  1 - 16 bits<BR>
  #  2 - 32 bits<BR>
  #  3 - 64 bits<BR>
  # @Prompt DIMM_BusWidth
  # @ValidList 0x80000001 | 0, 1, 2, 3
  gVlvRefCodePkgTokenSpaceGuid.PcdDimmBusWidth|3

  ## Ranks Per DIMM or Sides Per DIMM.
  #  0 - 1 Rank<BR>
  #  1 - 2 Ranks<BR>
  # @Prompt DIMM_Sides
  # @ValidList 0x80000001 | 0, 1
  gVlvRefCodePkgTokenSpaceGuid.PcdRankPerDimm|0

  ## tCL.<BR><BR>
  # @Prompt tCL
  gVlvRefCodePkgTokenSpaceGuid.PcdTcl|11

  ## tRP and tRCD in DRAM clk - 5:12.5ns, 6:15ns, etc.
  # @Prompt tRP_tRCD
  gVlvRefCodePkgTokenSpaceGuid.PcdTrpTrcd|11

  ## tWR in DRAM clk.
  # @Prompt tWR
  gVlvRefCodePkgTokenSpaceGuid.PcdTwr|12

  ## tWTR in DRAM clk.
  # @Prompt tWTR
  gVlvRefCodePkgTokenSpaceGuid.PcdTwtr|6

  ## tRRD in DRAM clk.
  # @Prompt tRRD
  gVlvRefCodePkgTokenSpaceGuid.PcdTrrd|6

  ## tRTP in DRAM clk.
  # @Prompt tRTP
  gVlvRefCodePkgTokenSpaceGuid.PcdTrtp|6

  ## tFAW in DRAM clk.
  # @Prompt tFAW
  gVlvRefCodePkgTokenSpaceGuid.PcdTfaw|32

  #
  # End of MRC parameters.
  #
  ###############################################################################################

[PcdsDynamicHii.common.DEFAULT]
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|5 # Variable: L"Timeout"
  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"
  gPlatformModuleTokenSpaceGuid.PcdBootState|L"BootState"|gPlatformModuleTokenSpaceGuid|0x0|TRUE

[PcdsDynamicDefault.common.DEFAULT]
  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateDataPtr|0x0
  !if $(TPM_ENABLED) == TRUE
    gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x7b, 0x3a, 0xcd, 0x72, 0xA5, 0xFE, 0x5e, 0x4f, 0x91, 0x65, 0x4d, 0xd1, 0x21, 0x87, 0xbb, 0x13}
  !endif

  ## This PCD defines the video horizontal resolution.
  #  This PCD could be set to 0 then video resolution could be at highest resolution.
  #gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|800
  ## This PCD defines the video vertical resolution.
  #  This PCD could be set to 0 then video resolution could be at highest resolution.
  #gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|600

  ## This PCD defines the Console output column and the default value is 25 according to UEFI spec.
  #  This PCD could be set to 0 then console output could be at max column and max row.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|31
  ## This PCD defines the Console output row and the default value is 80 according to UEFI spec.
  #  This PCD could be set to 0 then console output could be at max column and max row.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|100

  ## The PCD is used to specify the video horizontal resolution of text setup.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|800
  ## The PCD is used to specify the video vertical resolution of text setup.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|600
  ## The PCD is used to specify the console output column of text setup.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|100
  ## The PCD is used to specify the console output column of text setup.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|31

!if $(TPM_ENABLED) == TRUE
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmScrtmPolicy|1
!endif

[PcdsDynamicExDefault.common.DEFAULT]
  gEfiVLVTokenSpaceGuid.PcdTCSmbaIoBaseAddress|0x1040
  gEfiVLVTokenSpaceGuid.PcdEmmcManufacturerId|0
  gEfiVLVTokenSpaceGuid.PcdProductSerialNumber|0
  gEfiVLVTokenSpaceGuid.PcdMeasuredBootEnable|TRUE
  gEfiVLVTokenSpaceGuid.PcdFTPMErrorOccur|FALSE
  gEfiVLVTokenSpaceGuid.PcdFTPMErrorSkip|FALSE
  gEfiVLVTokenSpaceGuid.PcdFTPMCommand|0
  gEfiVLVTokenSpaceGuid.PcdFTPMResponse|0
  gEfiVLVTokenSpaceGuid.PcdFTPMNotRespond|FALSE
  gEfiVLVTokenSpaceGuid.PcdFTPMStatus|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateSmmDataPtr|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateDataPtr|0
  gEfiVLVTokenSpaceGuid.PcdCpuLockBoxDataAddress|0
  gEfiVLVTokenSpaceGuid.PcdCpuSmramCpuDataAddress|0
  gEfiVLVTokenSpaceGuid.PcdCpuLockBoxSize|0
  gEfiSecurityPkgTokenSpaceGuid.PcdUserPhysicalPresence|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSystemFmpCapsuleImageTypeIdGuid|{GUID("$(FMP_MINNOW_MAX_SYSTEM)")}|VOID*|0x10
!if $(RECOVERY_ENABLE)
  gEfiSignedCapsulePkgTokenSpaceGuid.PcdEdkiiSystemFirmwareImageDescriptor|{0x0}|VOID*|0x100
  gEfiSignedCapsulePkgTokenSpaceGuid.PcdEdkiiSystemFirmwareFileGuid|{GUID("AF9C9EB2-12AD-4D3E-A4D4-96F6C9966215")}|VOID*|0x10
!endif

[PcdsDynamicExVpd.common.DEFAULT]
  gEfiMdeModulePkgTokenSpaceGuid.PcdNvStoreDefaultValueBuffer|*

[PcdsDynamicExHii.common.DEFAULT.STANDARD]
  !include Vlv2TbltDevicePkg/PlatformSetupDefaults.dsc

[Components.IA32]

  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/SecCore.inf

  MdeModulePkg/Core/Pei/PeiMain.inf {
!if $(TARGET) == DEBUG
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
!endif
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046
  }

  Vlv2TbltDevicePkg/MonoStatusCode/MonoStatusCode.inf {
!if $(TARGET) == DEBUG
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
!endif
  }
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/MemoryInit.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046
  }

!if $(RC_BINARY_RELEASE) == TRUE
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/SeCUma.inf
!endif

!if $(RC_BINARY_RELEASE) == TRUE
  Vlv2TbltDevicePkg/PlatformPei/PlatformPei.inf {
  !if $(TARGET) == DEBUG
      <PcdsFixedAtBuild>
        gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
  !endif
  }
!endif

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  SourceLevelDebugPkg/DebugAgentPei/DebugAgentPei.inf{
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
      DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/SecPeiDebugAgentLib.inf
      PlatformHookLib|MdeModulePkg/Library/BasePlatformHookLibNull/BasePlatformHookLibNull.inf
      SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
    }
!endif

!if $(TPM_ENABLED) == TRUE
  SecurityPkg/Tcg/PhysicalPresencePei/PhysicalPresencePei.inf
  SecurityPkg/Tcg/TcgPei/TcgPei.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }
!endif

 Vlv2TbltDevicePkg/PlatformInitPei/PlatformInitPei.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x803805c6
    <LibraryClasses>
!if $(TARGET) != RELEASE
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }
  Vlv2TbltDevicePkg/FvInfoPei/FvInfoPei.inf

  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/VlvInitPeim.inf
!if $(PCIESC_ENABLE) == TRUE
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/PchEarlyInitPeim.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046
  }
!endif
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/PchInitPeim.inf


  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/PchSmbusArpDisabled.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/PchSpiPeim.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/PeiSmmAccess.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/PeiSmmControl.inf
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/CpuPeim.inf
  UefiCpuPkg/CpuIoPei/CpuIoPei.inf
  UefiCpuPkg/Universal/Acpi/S3Resume2Pei/S3Resume2Pei.inf
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationPei.inf

!if $(RECOVERY_ENABLE)
  #
  # Recovery
  #
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/IA32/PchUsb.inf
  MdeModulePkg/Bus/Pci/EhciPei/EhciPei.inf
  MdeModulePkg/Bus/Usb/UsbBusPei/UsbBusPei.inf
  MdeModulePkg/Bus/Usb/UsbBotPei/UsbBotPei.inf
  FatPkg/FatPei/FatPei.inf
  MdeModulePkg/Universal/Disk/CdExpressPei/CdExpressPei.inf
  SignedCapsulePkg/Universal/RecoveryModuleLoadPei/RecoveryModuleLoadPei.inf {
    <LibraryClasses>
      FmpAuthenticationLib|SecurityPkg/Library/FmpAuthenticationLibRsa2048Sha256/FmpAuthenticationLibRsa2048Sha256.inf
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }
!endif

!if $(CAPSULE_ENABLE) == TRUE
  MdeModulePkg/Universal/CapsulePei/CapsulePei.inf
!endif
  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf {
    <LibraryClasses>
!if $(LZMA_ENABLE) == TRUE
    NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
!endif
  }

 MdeModulePkg/Universal/Variable/Pei/VariablePei.inf
 MdeModulePkg/Universal/FaultTolerantWritePei/FaultTolerantWritePei.inf

!if $(TPM_ENABLED) == TRUE
  SecurityPkg/Tcg/Tcg2Config/Tcg2ConfigPei.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }
!endif
!if $(ACPI50_ENABLE) == TRUE
  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTablePei/FirmwarePerformancePei.inf{
    <LibraryClasses>
      TimerLib|Vlv2TbltDevicePkg/Library/IntelPchAcpiTimerLib/IntelPchAcpiTimerLib.inf
  }

!endif
!if $(PERFORMANCE_ENABLE) == TRUE
  MdeModulePkg/Universal/ReportStatusCodeRouter/Pei/ReportStatusCodeRouterPei.inf
!endif
[Components.IA32]
  #
  # EDK II Related Platform codes
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046
    <LibraryClasses>
!if $(DXE_CRC32_SECTION_ENABLE) == TRUE
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
!endif
!if $(LZMA_ENABLE) == TRUE
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
!endif
!if $(TARGET) != RELEASE
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
  }
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  UefiCpuPkg/CpuIo2Dxe/CpuIo2Dxe.inf

  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf  {
    <LibraryClasses>
!if $(TARGET) != RELEASE
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
  }

  MdeModulePkg/Universal/ReportStatusCodeRouter/Smm/ReportStatusCodeRouterSmm.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf{
    <LibraryClasses>
!if $(SECURE_BOOT_ENABLE) == TRUE
      NULL|SecurityPkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
!endif

!if $(TPM_ENABLED) == TRUE
      NULL|SecurityPkg/Library/DxeTpmMeasureBootLib/DxeTpmMeasureBootLib.inf
!endif
  }
  MdeModulePkg/Universal/Metronome/Metronome.inf

  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <LibraryClasses>
      PlatformBootManagerLib|Vlv2TbltDevicePkg/Library/DxePlatformBootManagerLib/DxePlatformBootManagerLib.inf
  }
  MdeModulePkg/Logo/LogoDxe.inf
  MdeModulePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf
  }
  MdeModulePkg/Application/BootManagerMenuApp/BootManagerMenuApp.inf

  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmmRuntimeDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmm.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
      NULL|MdeModulePkg/Library/VarCheckPolicyLib/VarCheckPolicyLib.inf
      SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
  }
  Vlv2TbltDevicePkg/FvbRuntimeDxe/FvbSmm.inf
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteSmm.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchSpiSmm.inf
!if $(SECURE_BOOT_ENABLE) == TRUE
  SecurityPkg/VariableAuthenticated/SecureBootConfigDxe/SecureBootConfigDxe.inf {
    <LibraryClasses>
      PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf
    <BuildOptions>
      #
      # Specify GUID gEfiIfrBootMaintenanceGuid, to install Secure Boot Configuration menu
      # into Boot Maintenance Manager menu
      #
      *_*_*_VFR_FLAGS   = -g b2dedc91-d59f-48d2-898a-12490c74a4e0
  }
!endif
   MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf {
    <LibraryClasses>
      FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  }

  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  PcAtChipsetPkg/PcatRealTimeClockRuntimeDxe/PcatRealTimeClockRuntimeDxe.inf
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  Vlv2TbltDevicePkg/FvbRuntimeDxe/FvbRuntimeDxe.inf

  Vlv2TbltDevicePkg/PlatformSetupDxe/PlatformSetupDxe.inf

  MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchS3SupportDxe.inf
  PcAtChipsetPkg/HpetTimerDxe/HpetTimerDxe.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/SmmControl.inf

  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchSmbusDxe.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchReset.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchInitDxe.inf{
    <PcdsPatchableInModule>
        gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0xF0000043
  }
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchInitSmm.inf

  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchSmiDispatcher.inf

!if $(PCIESC_ENABLE) == TRUE
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchPcieSmm.inf
!endif
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchSpiRuntime.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchPolicyInitDxe.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PchBiosWriteProtect.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/SmmAccess.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PciHostBridge.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/VlvInitDxe.inf

  #
  # Performance Application; Set PERFORMANCE_ENABLE=TRUE for normal boot performance and smm performance data
  #
!if $(PERFORMANCE_ENABLE) == TRUE
  ShellPkg/DynamicCommand/DpDynamicCommand/DpDynamicCommand.inf {
    <PcdsFixedAtBuild>
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
  }
!endif

  Vlv2TbltDevicePkg/VlvPlatformInitDxe/VlvPlatformInitDxe.inf{
    <LibraryClasses>
!if $(TARGET) != RELEASE
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/Dptf.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PnpDxe.inf

!if $(TPM_ENABLED) == TRUE
  SecurityPkg/Tcg/Tcg2Config/Tcg2ConfigDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  SecurityPkg/Tcg/TcgConfigDxe/TcgConfigDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
    <BuildOptions>
      #
      # specify GUID gEfiIfrNotInTPVPageGuid, this page will not
      # be showed in TPV page.
      #
      *_*_*_VFR_FLAGS   = -g e58809f8-fbc1-48e2-883a-a30fdc4b441e
  }

  SecurityPkg/Tcg/TcgDxe/TcgDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  SecurityPkg/Tcg/TcgSmm/TcgSmm.inf
!endif
  #
  # EDK II Related Platform codes
  #
  Vlv2TbltDevicePkg/PlatformSmm/PlatformSmm.inf{
    <LibraryClasses>
    !if $(TARGET) != RELEASE
          DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
    !endif
          PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  Vlv2TbltDevicePkg/PlatformInfoDxe/PlatformInfoDxe.inf
  Vlv2TbltDevicePkg/PlatformCpuInfoDxe/PlatformCpuInfoDxe.inf
  Vlv2TbltDevicePkg/PlatformDxe/PlatformDxe.inf

  Vlv2TbltDevicePkg/PciPlatform/PciPlatform.inf
  Vlv2TbltDevicePkg/SaveMemoryConfig/SaveMemoryConfig.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PlatformCpuPolicy.inf
  Vlv2TbltDevicePkg/PpmPolicy/PpmPolicy.inf
!if $(GOP_DRIVER_ENABLE) == TRUE
  Vlv2TbltDevicePkg/PlatformGopPolicy/PlatformGopPolicy.inf

!endif


  #
  # SMM
  #
  MdeModulePkg/Core/PiSmmCore/PiSmmIpl.inf
  MdeModulePkg/Core/PiSmmCore/PiSmmCore.inf
  UefiCpuPkg/CpuDxe/CpuDxe.inf
  UefiCpuPkg/CpuS3DataDxe/CpuS3DataDxe.inf
  UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf
  UefiCpuPkg/CpuIo2Smm/CpuIo2Smm.inf
  MdeModulePkg/Universal/LockBox/SmmLockBox/SmmLockBox.inf
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationSmm.inf

  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/PowerManagement2.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/DigitalThermalSensor.inf
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/GraphicDxeInitSmm.inf

  #
  # ACPI
  #
   MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0xF0000043
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x27
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Universal/Acpi/S3SaveStateDxe/S3SaveStateDxe.inf

  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  Vlv2DeviceRefCodePkg/ValleyView2Soc/CPU/PowerManagement/AcpiTables/PowerManagementAcpiTables.inf

  Vlv2DeviceRefCodePkg/AcpiTablesPCAT/AcpiTables.inf

  Vlv2TbltDevicePkg/AcpiPlatform/AcpiPlatform.inf

  MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf

  #
  # PCI
  #
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf


  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/ISPDxe.inf


  #
  # ISA
  #
  Vlv2TbltDevicePkg/PcuSio/PcuSio.inf
  MdeModulePkg/Bus/Pci/PciSioSerialDxe/PciSioSerialDxe.inf

!if $(ACPI50_ENABLE) == TRUE
  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf {
    <LibraryClasses>
      TimerLib|Vlv2TbltDevicePkg/Library/IntelPchAcpiTimerLib/IntelPchAcpiTimerLib.inf
  }
  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableSmm/FirmwarePerformanceSmm.inf {
    <LibraryClasses>
      TimerLib|Vlv2TbltDevicePkg/Library/IntelPchAcpiTimerLib/IntelPchAcpiTimerLib.inf
  }
!endif

#
# eMMC/SD Card
#
  MdeModulePkg/Bus/Pci/SdMmcPciHcDxe/SdMmcPciHcDxe.inf
  MdeModulePkg/Bus/Sd/EmmcDxe/EmmcDxe.inf
  MdeModulePkg/Bus/Sd/SdDxe/SdDxe.inf

#
# IDE/SCSI/AHCI
#
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf
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
      OrderedCollectionLib|MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }
!if $(SATA_ENABLE) == TRUE
  Vlv2SocBinPkg/$(DXE_ARCHITECTURE)$(TARGET)/$(DXE_ARCHITECTURE)/SataController.inf
!endif
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
!if $(SCSI_ENABLE) == TRUE
  MdeModulePkg/Bus/Scsi/ScsiBusDxe/ScsiBusDxe.inf
  MdeModulePkg/Bus/Scsi/ScsiDiskDxe/ScsiDiskDxe.inf
!endif
#
# Console
#
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf

  #
  # USB
  #
!if $(USB_ENABLE) == TRUE
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  MdeModulePkg/Bus/Pci/UhciDxe/UhciDxe.inf
  MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

!endif

  #
  # SMBIOS
  #
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  Vlv2TbltDevicePkg/SmBiosMiscDxe/SmBiosMiscDxe.inf

  #
  # CPU/FW Microde
  #
  Vlv2SocBinPkg/Microcode/MicrocodeUpdates.inf {
    <BuildOptions>
      *_*_*_GENFW_FLAGS = -a 0x800 -p 0xFF
  }


  #
  # Network Stacks
  #
!include NetworkPkg/Network.dsc.inc

!if $(CAPSULE_ENABLE) || $(MICOCODE_CAPSULE_ENABLE)
  MdeModulePkg/Universal/EsrtFmpDxe/EsrtFmpDxe.inf
  MdeModulePkg/Application/CapsuleApp/CapsuleApp.inf
!endif

!if $(CAPSULE_ENABLE)
  !include Vlv2TbltDevicePkg/FmpMinnowMaxSystem.dsc
  !include Vlv2TbltDevicePkg/FmpGreenSampleDevice.dsc
  !include Vlv2TbltDevicePkg/FmpBlueSampleDevice.dsc
  !include Vlv2TbltDevicePkg/FmpRedSampleDevice.dsc
!endif

!if $(MICOCODE_CAPSULE_ENABLE)
  IntelSiliconPkg/Feature/Capsule/MicrocodeUpdateDxe/MicrocodeUpdateDxe.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
  }
!endif

[BuildOptions]
#
# Define Build Options both for EDK and EDKII drivers.
#
GCC:*_*_*_CC_FLAGS = -Wno-missing-braces

#
# Define token for different Platform
#
!if $(SOURCE_DEBUG_ENABLE) == TRUE
  MSFT:*_*_X64_GENFW_FLAGS  = --keepexceptiontable
  GCC:*_*_X64_GENFW_FLAGS   = --keepexceptiontable
  INTEL:*_*_X64_GENFW_FLAGS = --keepexceptiontable
!endif

#
# Force PE/COFF sections to be aligned at 4KB boundaries to support page level
# protection of DXE_RUNTIME_DRIVER modules
#
[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
  MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000
  XCODE:*_*_*_DLINK_FLAGS = -segalign 0x1000

#
# Force PE/COFF sections to be aligned at 4KB boundaries to support page level
# protection of DXE_SMM_DRIVER/SMM_CORE modules
#
[BuildOptions.common.EDKII.DXE_SMM_DRIVER, BuildOptions.common.EDKII.SMM_CORE]
  MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000
  XCODE:*_*_*_DLINK_FLAGS = -segalign 0x1000

[BuildOptions.Common.EDKII]
!if $(CLKGEN_CONFIG_EXTRA_ENABLE) == TRUE
  MSFT:*_*_*_CC_FLAGS    = /DCLKGEN_CONFIG_EXTRA=1
  MSFT:*_*_*_VFRPP_FLAGS = /DCLKGEN_CONFIG_EXTRA=1
  MSFT:*_*_*_APP_FLAGS   = /DCLKGEN_CONFIG_EXTRA=1
  MSFT:*_*_*_PP_FLAGS    = /DCLKGEN_CONFIG_EXTRA=1
  MSFT:*_*_*_ASLPP_FLAGS = /DCLKGEN_CONFIG_EXTRA=1
  GCC:*_*_*_CC_FLAGS     = -DCLKGEN_CONFIG_EXTRA=1
  GCC:*_*_*_VFRPP_FLAGS  = -DCLKGEN_CONFIG_EXTRA=1
  GCC:*_*_*_APP_FLAGS    = -DCLKGEN_CONFIG_EXTRA=1
  GCC:*_*_*_PP_FLAGS     = -DCLKGEN_CONFIG_EXTRA=1
  GCC:*_*_*_ASLPP_FLAGS  = -DCLKGEN_CONFIG_EXTRA=1
!endif

!if $(PCIESC_ENABLE) == TRUE
  MSFT:*_*_*_CC_FLAGS    = /DPCIESC_SUPPORT=1
  MSFT:*_*_*_VFRPP_FLAGS = /DPCIESC_SUPPORT=1
  MSFT:*_*_*_APP_FLAGS   = /DPCIESC_SUPPORT=1
  MSFT:*_*_*_PP_FLAGS    = /DPCIESC_SUPPORT=1
  MSFT:*_*_*_ASLPP_FLAGS = /DPCIESC_SUPPORT=1
  GCC:*_*_*_CC_FLAGS     = -DPCIESC_SUPPORT=1
  GCC:*_*_*_VFRPP_FLAGS  = -DPCIESC_SUPPORT=1
  GCC:*_*_*_APP_FLAGS    = -DPCIESC_SUPPORT=1
  GCC:*_*_*_PP_FLAGS     = -DPCIESC_SUPPORT=1
  GCC:*_*_*_ASLPP_FLAGS  = -DPCIESC_SUPPORT=1
!endif

!if $(SOURCE_DEBUG_ENABLE) == TRUE
  !if $(TARGET) == DEBUG
    MSFT:*_*_X64_CC_FLAGS = /Od /Oy-
  !endif
!endif
