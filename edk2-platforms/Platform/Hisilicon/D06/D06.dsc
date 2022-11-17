#
#  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
#  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
#  Copyright (c) 2018, Linaro Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
#

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = D06
  PLATFORM_GUID                  = D0D445F1-B2CA-4101-9986-1B23525CBEA6
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x0001001A
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|NOOPT|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platform/Hisilicon/$(PLATFORM_NAME)/$(PLATFORM_NAME).fdf

  #
  # Network definition
  #
  DEFINE NETWORK_TLS_ENABLE      = FALSE
  DEFINE NETWORK_VLAN_ENABLE     = FALSE
  DEFINE NETWORK_IP6_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE

!include Silicon/Hisilicon/Hisilicon.dsc.inc
!include MdePkg/MdeLibs.dsc.inc

[LibraryClasses.common]
  ArmLib|ArmPkg/Library/ArmLib/ArmBaseLib.inf
  ArmPlatformLib|Silicon/Hisilicon/Library/ArmPlatformLibHisilicon/ArmPlatformLib.inf


  I2CLib|Silicon/Hisilicon/Library/I2CLib/I2CLib.inf
  TimerLib|ArmPkg/Library/ArmArchTimerLib/ArmArchTimerLib.inf
  IpmiCmdLib|Silicon/Hisilicon/Library/IpmiCmdLib/IpmiCmdLib.inf

  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  OrderedCollectionLib|MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf

  CpldIoLib|Silicon/Hisilicon/Library/CpldIoLib/CpldIoLib.inf

  TimeBaseLib|EmbeddedPkg/Library/TimeBaseLib/TimeBaseLib.inf
  RtcHelperLib|Silicon/Hisilicon/Library/RtcHelperLib/RtcHelperLib.inf
  RealTimeClockLib|Silicon/Hisilicon/Library/M41T83RealTimeClockLib/M41T83RealTimeClockLib.inf
  OemMiscLib|Platform/Hisilicon/D06/Library/OemMiscLibD06/OemMiscLibD06.inf
  OemAddressMapLib|Platform/Hisilicon/D06/Library/OemAddressMapD06/OemAddressMapD06.inf
  PlatformSysCtrlLib|Silicon/Hisilicon/Hi1620/Library/PlatformSysCtrlLibHi1620/PlatformSysCtrlLibHi1620.inf

  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
  BmcConfigBootLib|Silicon/Hisilicon/Library/BmcConfigBootLib/BmcConfigBootLib.inf
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  PlatformBootManagerLib|Silicon/Hisilicon/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
  CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf

  # USB Requirements
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf

  LpcLib|Silicon/Hisilicon/Hi1620/Library/LpcLibHi1620/LpcLib.inf
  SerialPortLib|ArmPlatformPkg/Library/PL011SerialPortLib/PL011SerialPortLib.inf
  OemNicLib|Platform/Hisilicon/D06/Library/OemNicLib/OemNicLib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  PciPlatformLib|Silicon/Hisilicon/Hi1620/Library/Hi1620PciPlatformLib/Hi1620PciPlatformLib.inf

[LibraryClasses.common.SEC]
  ArmPlatformLib|Silicon/Hisilicon/Library/ArmPlatformLibHisilicon/ArmPlatformLibSec.inf


[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  I2CLib|Silicon/Hisilicon/Library/I2CLib/I2CLibRuntime.inf
  SerialPortLib|ArmPlatformPkg/Library/PL011SerialPortLib/PL011SerialPortLib.inf

[BuildOptions]
  GCC:*_*_AARCH64_PLATFORM_FLAGS == -I$(WORKSPACE)/Silicon/Hisilicon/Hi1620/Include

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsFeatureFlag.common]

  ## If TRUE, Graphics Output Protocol will be installed on virtual handle created by ConsplitterDxe.
  #  It could be set FALSE to save size.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport|TRUE
  gHisiTokenSpaceGuid.PcdIsItsSupported|TRUE
  gArmTokenSpaceGuid.PcdArmGicV3WithV2Legacy|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdHiiOsRuntimeSupport|FALSE
[PcdsDynamicExDefault.common.DEFAULT]
  gEfiSignedCapsulePkgTokenSpaceGuid.PcdEdkiiSystemFirmwareImageDescriptor|{0x0}|VOID*|0x100
  gEfiMdeModulePkgTokenSpaceGuid.PcdSystemFmpCapsuleImageTypeIdGuid|{0x29, 0x3d, 0x4b, 0xd3, 0x85, 0x00, 0xb3, 0x4a, 0x8b, 0xe8, 0x84, 0x18, 0x8c, 0xc5, 0x04, 0x89}
  gEfiSignedCapsulePkgTokenSpaceGuid.PcdEdkiiSystemFirmwareFileGuid|{0xcf, 0x4f, 0x2e, 0x64, 0xf7, 0x2d, 0x15, 0x44, 0x8b, 0x70, 0xa0, 0x39, 0x09, 0xc5, 0x7b, 0x55}


[PcdsFixedAtBuild.common]
  gArmPlatformTokenSpaceGuid.PcdCoreCount|48

  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x2000


  # Stacks for MPCores in Normal World
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xA0E88000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x00000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x3FC00000

  # Size of the region used by UEFI in permanent memory (Reserved 64MB)
  gArmPlatformTokenSpaceGuid.PcdSystemMemoryUefiRegionSize|0x10000000

  gHisiTokenSpaceGuid.PcdSerDesFlowCtrlFlag|1

  gHisiTokenSpaceGuid.PcdSlotPerChannelNum|0x2

  ## Serial Terminal
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x94080000
  gArmPlatformTokenSpaceGuid.PcdSerialDbgRegisterBase|0x400094080000
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|115200

  gArmPlatformTokenSpaceGuid.PL011UartClkInHz|200000000

  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultDataBits|8
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultParity|1
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultStopBits|1

  gHisiTokenSpaceGuid.PcdIsMPBoot|1
  gHisiTokenSpaceGuid.PcdSocketMask|0x3
  !ifdef $(FIRMWARE_VER)
    gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L"$(FIRMWARE_VER)"
  !else
    gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L"Development build 19.02 for Hisilicon D06"
  !endif

  gHisiTokenSpaceGuid.PcdBiosVersionString|L"10.01.01T18"

  gHisiTokenSpaceGuid.PcdBiosVersionForBmc|L"19.02"

  gHisiTokenSpaceGuid.PcdSystemProductName|L"D06"
  gHisiTokenSpaceGuid.PcdSystemVersion|L"VER.A"
  gHisiTokenSpaceGuid.PcdBaseBoardProductName|L"D06"
  gHisiTokenSpaceGuid.PcdBaseBoardVersion|L"Estuary"

  gHisiTokenSpaceGuid.PcdCPUInfo|L"Hisilicon 1620"

  # TA
  gHisiTokenSpaceGuid.PcdArmPrimaryCoreTemp|0x80010000
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xAE000000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0xAE100000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x9B000000



  #
  # ARM Architectual Timer Frequency
  #
  # Set it to 0 so that the code will read frequency from register and be
  # adapted to 100M and 50M boards
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|0
  gEmbeddedTokenSpaceGuid.PcdTimerPeriod|10000


  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }
  gHisiTokenSpaceGuid.PcdSysControlBaseAddress|0x94010000
  gHisiTokenSpaceGuid.PcdMailBoxAddress|0x0000FFF8

  gHisiTokenSpaceGuid.PcdCpldBaseAddress|0x80000000
  gHisiTokenSpaceGuid.PcdSFCMEM0BaseAddress|0x204000000

  gHisiTokenSpaceGuid.PcdPeriSubctrlAddress|0x94000000

  ## 2+1
  gHisiTokenSpaceGuid.PcdPlatformDefaultPackageType|0x1

  gHisiTokenSpaceGuid.PcdTopOfLowMemory|0x40000000

  gHisiTokenSpaceGuid.PcdBottomOfHighMemory|0x1000000000

  gHisiTokenSpaceGuid.PcdNORFlashBase|0x80000000
  gHisiTokenSpaceGuid.PcdNORFlashCachableSize|0x8000000

  gHisiTokenSpaceGuid.PcdTrustedFirmwareEnable|0x1
  gHisiTokenSpaceGuid.PcdMacAddress|0xA47E0000

  # PCIe ECAM Access BaseAddress
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xD0000000
  gEmbeddedTokenSpaceGuid.PcdPrePiCpuIoSize|16

  gHisiTokenSpaceGuid.Pcdsoctype|0x1620

  # SMBIOS 3.0 only
  #  BIT0 set indicates 32-bit entry point and table are produced.<BR>
  #  BIT1 set indicates 64-bit entry point and table are produced.<BR>
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosEntryPointProvideMethod|0x2

  #
  # ACPI Table Version
  #
  #   BIT 1 - EFI_ACPI_TABLE_VERSION_1_0B.<BR>
  #   BIT 2 - EFI_ACPI_TABLE_VERSION_2_0.<BR>
  #   BIT 3 - EFI_ACPI_TABLE_VERSION_3_0.<BR>
  #   BIT 4 - EFI_ACPI_TABLE_VERSION_4_0.<BR>
  #   BIT 5 - EFI_ACPI_TABLE_VERSION_5_0.<BR>
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiExposedTableVersions|0x20

  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
  gArmTokenSpaceGuid.PcdPciIoTranslation|0x0

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]

  #
  # SEC
  #

  #
  # PEI Phase modules
  #
  ArmPlatformPkg/PrePeiCore/PrePeiCoreMPCore.inf
  MdeModulePkg/Core/Pei/PeiMain.inf
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf

  ArmPlatformPkg/PlatformPei/PlatformPeim.inf

  ArmPkg/Drivers/CpuPei/CpuPei.inf
  MdeModulePkg/Universal/ReportStatusCodeRouter/Pei/ReportStatusCodeRouterPei.inf
  MdeModulePkg/Universal/StatusCodeHandler/Pei/StatusCodeHandlerPei.inf
  MdeModulePkg/Universal/FaultTolerantWritePei/FaultTolerantWritePei.inf
  MdeModulePkg/Universal/Variable/Pei/VariablePei.inf

  Platform/Hisilicon/D06/EarlyConfigPeim/EarlyConfigPeimD06.inf
  Silicon/Hisilicon/Drivers/VersionInfoPeim/VersionInfoPeim.inf

  Platform/Hisilicon/D06/Drivers/SystemFirmwareDescriptor/SystemFirmwareDescriptor.inf
  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

  #
  # DXE
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
  }
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf


  #
  # Architectural Protocols
  #
  ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  Platform/Hisilicon/D06/Drivers/OemNicConfig2PHi1620/OemNicConfig2P.inf

  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  Silicon/Hisilicon/Drivers/FlashFvbDxe/FlashFvbDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
      BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  }
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf

  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf {
    <LibraryClasses>
      CpldIoLib|Silicon/Hisilicon/Library/CpldIoLib/CpldIoLibRuntime.inf
  }
  EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf
  EmbeddedPkg/Drivers/ConsolePrefDxe/ConsolePrefDxe.inf

  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  #
  #ACPI
  #
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  Silicon/Hisilicon/Drivers/HisiAcpiPlatformDxe/AcpiPlatformDxe.inf {
    <LibraryClasses>
    NULL|Silicon/Hisilicon/Hi1620/Hi1620OemConfigUiLib/OemConfigUiLib.inf
  }

  Silicon/Hisilicon/Hi1620/Hi1620AcpiTables/AcpiTablesHi1620.inf
  Silicon/Hisilicon/Drivers/AcpiPlatformDxe/AcpiPlatformDxe.inf

  Silicon/Hisilicon/Hi1620/Drivers/Apei/Apei.inf
  Silicon/Hisilicon/Hi1620/Pptt/Pptt.inf
  #
  # Usb Support
  #
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  #
  #network
  #
!include NetworkPkg/Network.dsc.inc

  MdeModulePkg/Bus/Pci/SataControllerDxe/SataControllerDxe.inf
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
  SignedCapsulePkg/Universal/SystemFirmwareUpdate/SystemFirmwareReportDxe.inf
  MdeModulePkg/Universal/EsrtDxe/EsrtDxe.inf
  #
  # FAT filesystem + GPT/MBR partitioning
  #

  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  MdeModulePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|MdeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf
  }
  #
  # Bds
  #
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  Silicon/Hisilicon/Drivers/Smbios/SmbiosMiscDxe/SmbiosMiscDxe.inf
  Silicon/Hisilicon/Drivers/Smbios/MemorySubClassDxe/MemorySubClassDxe.inf
  Silicon/Hisilicon/Drivers/Smbios/ProcessorSubClassDxe/ProcessorSubClassDxe.inf
  Silicon/Hisilicon/Drivers/Smbios/AddSmbiosType9/AddSmbiosType9.inf

  #PCIe Support
  Silicon/Hisilicon/Drivers/PciPlatform/PciPlatform.inf
  ArmPkg/Drivers/ArmPciCpuIo2Dxe/ArmPciCpuIo2Dxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf {
    <LibraryClasses>
      PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
      PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
      PciHostBridgeLib|Platform/Hisilicon/D06/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  }

  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf

  #
  # Memory test
  #
  MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf
  Silicon/Hisilicon/Hi1620/Drivers/Pl011DebugSerialPortInitDxe/Pl011DebugSerialPortInitDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/DriverHealthManagerDxe/DriverHealthManagerDxe.inf
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  SignedCapsulePkg/Universal/SystemFirmwareUpdate/SystemFirmwareUpdateDxe.inf {
    <LibraryClasses>
      FmpAuthenticationLib|SecurityPkg/Library/FmpAuthenticationLibPkcs7/FmpAuthenticationLibPkcs7.inf
  }

  MdeModulePkg/Application/CapsuleApp/CapsuleApp.inf

  #
  # UEFI application (Shell Embedded Boot Loader)
  #
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
!if $(NETWORK_IP6_ENABLE) == TRUE
      NULL|ShellPkg/Library/UefiShellNetwork2CommandsLib/UefiShellNetwork2CommandsLib.inf
!endif

!if $(INCLUDE_DP) == TRUE
      NULL|ShellPkg/Library/UefiDpLib/UefiDpLib.inf
!endif #$(INCLUDE_DP)

    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }
!if $(INCLUDE_TFTP_COMMAND) == TRUE
  ShellPkg/DynamicCommand/TftpDynamicCommand/TftpDynamicCommand.inf {
    <PcdsFixedAtBuild>
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
  }
!endif #$(INCLUDE_TFTP_COMMAND)

