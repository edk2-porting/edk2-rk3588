#
#  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
#  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
#  Copyright (c) 2015, Linaro Limited. All rights reserved.
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
  PLATFORM_NAME                  = D03
  PLATFORM_GUID                  = e5003abd-8809-6194-ac3d-a6a99ff52478
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platform/Hisilicon/$(PLATFORM_NAME)/$(PLATFORM_NAME).fdf

  #
  # Network definition
  #
  DEFINE NETWORK_SNP_ENABLE             = FALSE
  DEFINE NETWORK_IP6_ENABLE             = FALSE
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE
  DEFINE NETWORK_VLAN_ENABLE            = FALSE

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


!ifdef $(FDT_ENABLE)
  #FDTUpdateLib
  FdtUpdateLib|Platform/Hisilicon/D03/Library/FdtUpdateLib/FdtUpdateLib.inf
!endif #$(FDT_ENABLE)

  CpldIoLib|Silicon/Hisilicon/Library/CpldIoLib/CpldIoLib.inf

  SerdesLib|Silicon/Hisilicon/Hi1610/Library/Hi1610Serdes/Hi1610SerdesLib.inf

  TimeBaseLib|EmbeddedPkg/Library/TimeBaseLib/TimeBaseLib.inf
  RealTimeClockLib|Platform/Hisilicon/D03/Library/DS3231RealTimeClockLib/DS3231RealTimeClockLib.inf

  OemMiscLib|Platform/Hisilicon/D03/Library/OemMiscLib2P/OemMiscLib2PHi1610.inf
  OemAddressMapLib|Platform/Hisilicon/D03/Library/OemAddressMap2P/OemAddressMap2PHi1610.inf
  PlatformSysCtrlLib|Silicon/Hisilicon/Hi1610/Library/PlatformSysCtrlLibHi1610/PlatformSysCtrlLibHi1610.inf

  BmcConfigBootLib|Silicon/Hisilicon/Library/BmcConfigBootLib/BmcConfigBootLib.inf
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  PlatformBootManagerLib|Silicon/Hisilicon/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
  CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf

  # USB Requirements
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf

  LpcLib|Silicon/Hisilicon/Hi1610/Library/LpcLib/LpcLib.inf
  PlatformPciLib|Platform/Hisilicon/D03/Library/PlatformPciLib/PlatformPciLib.inf
  SerialPortLib|Silicon/Hisilicon/Hi1610/Library/Uart/LpcSerialPortLib/LpcSerialPortLib.inf
  PciHostBridgeLib|Platform/Hisilicon/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  PciSegmentLib|Silicon/Hisilicon/Hi1610/Library/Hi161xPciSegmentLib/Hi161xPciSegmentLib.inf
  PciPlatformLib|Silicon/Hisilicon/Hi1610/Library/Hi161xPciPlatformLib/Hi161xPciPlatformLib.inf

## GIC on D02/D03 is not fully ARM GIC compatible: IRQ cannot be cancelled when
## input signal is de-asserted, except for virtual timer interrupt IRQ #27.
## So we choose to use virtual timer instead of physical one as a workaround.
## This library instance is to override the original define in LibraryClasses.AARCH64 in Hisilicon.dsc.inc.
[LibraryClasses.AARCH64]
  ArmGenericTimerCounterLib|ArmPkg/Library/ArmGenericTimerVirtCounterLib/ArmGenericTimerVirtCounterLib.inf

[LibraryClasses.common.SEC]
  ArmPlatformLib|Silicon/Hisilicon/Library/ArmPlatformLibHisilicon/ArmPlatformLibSec.inf


[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  I2CLib|Silicon/Hisilicon/Library/I2CLib/I2CLibRuntime.inf
  SerialPortLib|Silicon/Hisilicon/Hi1610/Library/Uart/LpcSerialPortLib/LpcSerialPortLib.inf

[BuildOptions]
  GCC:*_*_AARCH64_PLATFORM_FLAGS == -I$(WORKSPACE)/Silicon/Hisilicon/Hi1610/Include

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
  gEfiMdeModulePkgTokenSpaceGuid.PcdHiiOsRuntimeSupport|FALSE

[PcdsDynamicExDefault.common.DEFAULT]
  gEfiSignedCapsulePkgTokenSpaceGuid.PcdEdkiiSystemFirmwareImageDescriptor|{0x0}|VOID*|0x100
  gEfiMdeModulePkgTokenSpaceGuid.PcdSystemFmpCapsuleImageTypeIdGuid|{0x29, 0x3d, 0x4b, 0xd3, 0x85, 0x00, 0xb3, 0x4a, 0x8b, 0xe8, 0x84, 0x18, 0x8c, 0xc5, 0x04, 0x89}
  gEfiSignedCapsulePkgTokenSpaceGuid.PcdEdkiiSystemFirmwareFileGuid|{0xcf, 0x4f, 0x2e, 0x64, 0xf7, 0x2d, 0x15, 0x44, 0x8b, 0x70, 0xa0, 0x39, 0x09, 0xc5, 0x7b, 0x55}

[PcdsFixedAtBuild.common]
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmTokenSpaceGuid.PcdPciIoTranslation|0

  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x2000

  # Stacks for MPCores in Normal World
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0x81000000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0xFF00

  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x00000000


  gArmTokenSpaceGuid.PcdSystemMemorySize|0x3FC00000


  # Size of the region used by UEFI in permanent memory (Reserved 64MB)
  gArmPlatformTokenSpaceGuid.PcdSystemMemoryUefiRegionSize|0x10000000

  gHisiTokenSpaceGuid.PcdSerDesFlowCtrlFlag|1
  gHisiTokenSpaceGuid.PcdSlotPerChannelNum|0x2


  gHisiTokenSpaceGuid.PcdPcieRootBridgeMask|0x7 # bit0:HB0RB0,bit1:HB0RB1,bit2:HB0RB2,bit3:HB0RB3,bit4:HB1RB0,bit5:HB1RB1,bit6:HB1RB2,bit7:HB1RB3

  ## Serial Terminal
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x2F8
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|115200

  gHisiTokenSpaceGuid.PcdUartClkInHz|1846100

  gHisiTokenSpaceGuid.PcdSerialPortSendDelay|10000000

  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultDataBits|8
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultParity|1
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultStopBits|1


  gHisiTokenSpaceGuid.PcdM3SmmuBaseAddress|0xa0040000
  gHisiTokenSpaceGuid.PcdPcieSmmuBaseAddress|0xb0040000
  gHisiTokenSpaceGuid.PcdDsaSmmuBaseAddress|0xc0040000
  gHisiTokenSpaceGuid.PcdAlgSmmuBaseAddress|0xd0040000


  !ifdef $(FIRMWARE_VER)
    gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L"$(FIRMWARE_VER)"
  !else
    gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L"Development build 19.02 for Hisilicon D03"
  !endif

  gHisiTokenSpaceGuid.PcdBiosVersionString|L"10.01.01T18"

  gHisiTokenSpaceGuid.PcdBiosVersionForBmc|L"19.02"

  gHisiTokenSpaceGuid.PcdSystemProductName|L"D03"
  gHisiTokenSpaceGuid.PcdSystemVersion|L"Estuary"
  gHisiTokenSpaceGuid.PcdBaseBoardProductName|L"D03"
  gHisiTokenSpaceGuid.PcdBaseBoardVersion|L"Estuary"

  gHisiTokenSpaceGuid.PcdCPUInfo|L"Hi1612"

  #
  # ARM PL390 General Interrupt Controller
  #

  gArmTokenSpaceGuid.PcdGicDistributorBase|0x4D000000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0xFE000000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x4D100000

  #
  # ARM Architectual Timer Frequency
  #
  # Set it to 0 so that the code will read frequence from register and be
  # adapted to 66M and 50M boards
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|0


  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

  gHisiTokenSpaceGuid.PcdSysControlBaseAddress|0x40010000
  gHisiTokenSpaceGuid.PcdMailBoxAddress|0x0000FFF8

  gHisiTokenSpaceGuid.PcdCpldBaseAddress|0x78000000

  gHisiTokenSpaceGuid.PcdSFCCFGBaseAddress|0xA6000000
  gHisiTokenSpaceGuid.PcdSFCMEM0BaseAddress|0xA4000000


  gHisiTokenSpaceGuid.PcdPeriSubctrlAddress|0x40000000


  gHisiTokenSpaceGuid.PcdMdioSubctrlAddress|0x60000000

  gHisiTokenSpaceGuid.FdtFileAddress|0xA47C0000

  gHisiTokenSpaceGuid.PcdPlatformDefaultPackageType|0x1

  gHisiTokenSpaceGuid.PcdArmPrimaryCoreTemp|0x80010000

  gHisiTokenSpaceGuid.PcdTopOfLowMemory|0x40000000

  gHisiTokenSpaceGuid.PcdBottomOfHighMemory|0x2000000000

  gHisiTokenSpaceGuid.PcdTrustedFirmwareEnable|0x1
  gHisiTokenSpaceGuid.PcdNumaEnable|0

  gHisiTokenSpaceGuid.PcdHb1BaseAddress|0x40000000000


  gHisiTokenSpaceGuid.PcdHb0Rb0PciConfigurationSpaceBaseAddress|0xB0000000
  gHisiTokenSpaceGuid.PcdHb0Rb0PciConfigurationSpaceSize|0x8000000

  gHisiTokenSpaceGuid.PcdHb0Rb1PciConfigurationSpaceBaseAddress|0xB0000000
  gHisiTokenSpaceGuid.PcdHb0Rb1PciConfigurationSpaceSize|0x10000000

  gHisiTokenSpaceGuid.PcdHb0Rb2PciConfigurationSpaceBaseAddress|0xA0000000
  gHisiTokenSpaceGuid.PcdHb0Rb2PciConfigurationSpaceSize|0x10000000

  gHisiTokenSpaceGuid.PcdHb0Rb3PciConfigurationSpaceBaseAddress|0xAC000000
  gHisiTokenSpaceGuid.PcdHb0Rb3PciConfigurationSpaceSize|0x4000000

  gHisiTokenSpaceGuid.PciHb0Rb0Base|0xa0090000
  gHisiTokenSpaceGuid.PciHb0Rb1Base|0xa0200000
  gHisiTokenSpaceGuid.PciHb0Rb2Base|0xa00a0000
  gHisiTokenSpaceGuid.PciHb0Rb3Base|0xa00b0000

  gHisiTokenSpaceGuid.PcdHb0Rb0PciRegionBaseAddress|0xb2000000
  gHisiTokenSpaceGuid.PcdHb0Rb0PciRegionSize|0x5feffff

  gHisiTokenSpaceGuid.PcdHb0Rb1PciRegionBaseAddress|0xb8000000
  gHisiTokenSpaceGuid.PcdHb0Rb1PciRegionSize|0x5feffff

  gHisiTokenSpaceGuid.PcdHb0Rb2PciRegionBaseAddress|0xaa000000
  gHisiTokenSpaceGuid.PcdHb0Rb2PciRegionSize|0x5feffff

  gHisiTokenSpaceGuid.PcdHb0Rb0CpuMemRegionBase|0xB2000000
  gHisiTokenSpaceGuid.PcdHb0Rb1CpuMemRegionBase|0xB8000000
  gHisiTokenSpaceGuid.PcdHb0Rb2CpuMemRegionBase|0xAA000000

  gHisiTokenSpaceGuid.PcdHb0Rb0CpuIoRegionBase|0xb7ff0000
  gHisiTokenSpaceGuid.PcdHb0Rb1CpuIoRegionBase|0xbdff0000
  gHisiTokenSpaceGuid.PcdHb0Rb2CpuIoRegionBase|0xAfff0000

  gHisiTokenSpaceGuid.PcdHb0Rb0IoBase|0
  gHisiTokenSpaceGuid.PcdHb0Rb0IoSize|0xffff #64K

  gHisiTokenSpaceGuid.PcdHb0Rb1IoBase|0
  gHisiTokenSpaceGuid.PcdHb0Rb1IoSize|0xffff #64K

  gHisiTokenSpaceGuid.PcdHb0Rb2IoBase|0
  gHisiTokenSpaceGuid.PcdHb0Rb2IoSize|0xffff #64K

  gHisiTokenSpaceGuid.Pcdsoctype|0x1610

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
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  Platform/Hisilicon/D03/Drivers/Ipmi/IpmiInterfacePei/IpmiInterfacePei.inf

  ArmPlatformPkg/PlatformPei/PlatformPeim.inf

  Platform/Hisilicon/D03/MemoryInitPei/MemoryInitPeim.inf
  ArmPkg/Drivers/CpuPei/CpuPei.inf
  MdeModulePkg/Universal/ReportStatusCodeRouter/Pei/ReportStatusCodeRouterPei.inf
  MdeModulePkg/Universal/StatusCodeHandler/Pei/StatusCodeHandlerPei.inf
  MdeModulePkg/Universal/FaultTolerantWritePei/FaultTolerantWritePei.inf
  MdeModulePkg/Universal/Variable/Pei/VariablePei.inf

  Platform/Hisilicon/D03/EarlyConfigPeim/EarlyConfigPeimD03.inf
  Silicon/Hisilicon/Drivers/VersionInfoPeim/VersionInfoPeim.inf

  Platform/Hisilicon/D03/Drivers/SystemFirmwareDescriptor/SystemFirmwareDescriptor.inf

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
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

  Silicon/Hisilicon/Hi1610/Drivers/IoInitDxe/IoInitDxe.inf

  #
  # Architectural Protocols
  #
  ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  ArmPkg/Drivers/ArmPciCpuIo2Dxe/ArmPciCpuIo2Dxe.inf
  Platform/Hisilicon/D03/Drivers/OemNicConfig2PHi1610/OemNicConfig2P.inf

  Platform/Hisilicon/D03/Drivers/SFC/SfcDxeDriver.inf

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

  # Simple TextIn/TextOut for UEFI Terminal
  EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf

  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  #
  #ACPI
  #
  MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf

  Silicon/Hisilicon/Hi1610/Hi1610AcpiTables/AcpiTablesHi1610.inf
  Silicon/Hisilicon/Drivers/AcpiPlatformDxe/AcpiPlatformDxe.inf

  #
  # Usb Support
  #
  Silicon/Hisilicon/Drivers/VirtualEhciPciIo/VirtualEhciPciIo.inf
  MdeModulePkg/Bus/Pci/NonDiscoverablePciDeviceDxe/NonDiscoverablePciDeviceDxe.inf
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  Platform/Hisilicon/D03/Drivers/OhciDxe/OhciDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  Platform/Hisilicon/D03/Drivers/Ipmi/ipmiInterfaceDxe/IpmiInterfaceDxe.inf

  Silicon/Hisilicon/Drivers/SnpPlatform/SnpPlatform.inf

!include NetworkPkg/Network.dsc.inc
  Platform/Hisilicon/D03/Drivers/Sas/SasDxeDriver.inf

  SignedCapsulePkg/Universal/SystemFirmwareUpdate/SystemFirmwareReportDxe.inf
  MdeModulePkg/Universal/EsrtDxe/EsrtDxe.inf

  #
  # FAT filesystem + GPT/MBR partitioning
  #

  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf

  MdeModulePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|MdeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf
  }
  MdeModulePkg/Application/HelloWorld/HelloWorld.inf
  #
  # Bds
  #
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  Platform/Hisilicon/D03/Drivers/GetInfoFromBmc/GetInfoFromBmc.inf
  Platform/Hisilicon/D03/Drivers/TransferSmbiosInfo/TransSmbiosInfo.inf
  Platform/Hisilicon/D03/Drivers/IpmiMiscOpDxe/IpmiMiscOpDxe.inf

  Platform/Hisilicon/D03/Drivers/IpmiWatchdogDxe/IpmiWatchdogDxe.inf

  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  Silicon/Hisilicon/Drivers/Smbios/SmbiosMiscDxe/SmbiosMiscDxe.inf

!ifdef $(FDT_ENABLE)
  Silicon/Hisilicon/Drivers/UpdateFdtDxe/UpdateFdtDxe.inf {
    <LibraryClasses>
      BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  }
!endif #$(FDT_ENABLE)

  #PCIe Support
  Silicon/Hisilicon/Hi1610/Drivers/PcieInit1610/PcieInitDxe.inf {
    <LibraryClasses>
      NULL|Platform/Hisilicon/D03/Library/PlatformPciLib/PlatformPciLib.inf
  }
  Silicon/Hisilicon/Drivers/PciPlatform/PciPlatform.inf {
    <LibraryClasses>
      NULL|Platform/Hisilicon/D03/Library/PlatformPciLib/PlatformPciLib.inf
  }
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf {
    <LibraryClasses>
      NULL|Platform/Hisilicon/D03/Library/PlatformPciLib/PlatformPciLib.inf
  }

  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

  Platform/Hisilicon/D03/Drivers/ReportPciePlugDidVidToBmc/ReportPciePlugDidVidToBmc.inf
  Silicon/Hisilicon/Drivers/Smbios/AddSmbiosType9/AddSmbiosType9.inf
  Platform/Hisilicon/Drivers/Sm750Dxe/UefiSmi.inf

  Silicon/Hisilicon/Drivers/SasPlatform/SasPlatform.inf
  Silicon/Hisilicon/Drivers/Smbios/MemorySubClassDxe/MemorySubClassDxe.inf


  Silicon/Hisilicon/Drivers/Smbios/ProcessorSubClassDxe/ProcessorSubClassDxe.inf

  #
  # Memory test
  #
  MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf

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
      OrderedCollectionLib|MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
!ifdef $(INCLUDE_DP)
      NULL|ShellPkg/Library/UefiDpLib/UefiDpLib.inf
!endif #$(INCLUDE_DP)

    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }
!ifdef $(INCLUDE_TFTP_COMMAND)
  ShellPkg/DynamicCommand/TftpDynamicCommand/TftpDynamicCommand.inf {
    <PcdsFixedAtBuild>
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
  }
!endif #$(INCLUDE_TFTP_COMMAND)
