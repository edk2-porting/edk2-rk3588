#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#  Copyright (c) 2021-2022, Rockchip Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = R58X
  PLATFORM_GUID                  = d080df36-45e7-11ec-9726-f42a7dcb925d
  PLATFORM_VERSION               = 0.2
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platform/Mekotronics/R58X/R58X.fdf

  DEFINE CONFIG_NO_DEBUGLIB      = TRUE

  DEFINE CP_UNCONNECTED    = 0x0
  DEFINE CP_PCIE           = 0x01
  DEFINE CP_SATA           = 0x10
  DEFINE CP_USB3           = 0x20

  #
  # Network definition
  #
  DEFINE NETWORK_SNP_ENABLE             = FALSE
  DEFINE NETWORK_IP6_ENABLE             = FALSE
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE
  DEFINE NETWORK_VLAN_ENABLE            = FALSE
!include Silicon/Rockchip/Rockchip.dsc.inc
!include MdePkg/MdeLibs.dsc.inc

!if $(ENABLE_SIMPLE_INIT)
  !include SimpleInit.inc
!endif

[LibraryClasses.common]
  ArmLib|ArmPkg/Library/ArmLib/ArmBaseLib.inf
  ArmPlatformLib|Silicon/Rockchip/RK3588/Library/PlatformLib/PlatformLib.inf
  AcpiLib|EmbeddedPkg/Library/AcpiLib/AcpiLib.inf
  CruLib|Silicon/Rockchip/Library/CruLib/CruLib.inf

  DmaLib|EmbeddedPkg/Library/NonCoherentDmaLib/NonCoherentDmaLib.inf

  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf

  CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf

  # UiApp dependencies
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf

  #RealTimeClockLib|ArmPlatformPkg/Library/PL031RealTimeClockLib/PL031RealTimeClockLib.inf
  TimeBaseLib|EmbeddedPkg/Library/TimeBaseLib/TimeBaseLib.inf

  # USB Requirements
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf

  # PCIe
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  PciHostBridgeLib|Silicon/Rockchip/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  PciExpressLib|Silicon/Rockchip/Library/PciExpressLib/PciExpressLib.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf


  # VariableRuntimeDxe Requirements
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
  TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
  VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf

  AndroidBootImgLib|edk2/EmbeddedPkg/Library/AndroidBootImgLib/AndroidBootImgLib.inf

  RockchipDisplayLib|Silicon/Rockchip/Library/DisplayLib/RockchipDisplayLib.inf

  UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
  LockBoxLib|MdeModulePkg/Library/LockBoxNullLib/LockBoxNullLib.inf

  # OTP Library
  OtpLib|Silicon/Rockchip/RK3588/Library/OtpLib/OtpLib.inf

  #
  # Custom libraries
  #
  RockchipPlatformLib|Platform/Mekotronics/R58X/Library/RockchipPlatformLib/RockchipPlatformLib.inf
  ResetSystemLib|Silicon/Rockchip/Library/ResetSystemLib/ResetSystemLib.inf
  PlatformBootManagerLib|Silicon/Rockchip/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  SerialPortLib|Silicon/Hisilicon/Library/Dw8250SerialPortLib/Dw8250SerialPortLib.inf
  GpioLib|Silicon/Rockchip/RK3588/Library/GpioLib/GpioLib.inf
  # SCMI Mailbox Transport Layer
  ArmMtlLib|Silicon/Rockchip/Library/RkMtlLib/RkMtlLib.inf

[LibraryClasses.common.SEC]
  PrePiLib|EmbeddedPkg/Library/PrePiLib/PrePiLib.inf
  ExtractGuidedSectionLib|EmbeddedPkg/Library/PrePiExtractGuidedSectionLib/PrePiExtractGuidedSectionLib.inf
  HobLib|EmbeddedPkg/Library/PrePiHobLib/PrePiHobLib.inf
  MemoryAllocationLib|EmbeddedPkg/Library/PrePiMemoryAllocationLib/PrePiMemoryAllocationLib.inf
  MemoryInitPeiLib|Silicon/Rockchip/RK3588/Library/MemoryInitPeiLib/MemoryInitPeiLib.inf
  PlatformPeiLib|ArmPlatformPkg/PlatformPei/PlatformPeiLib.inf
  PrePiHobListPointerLib|ArmPlatformPkg/Library/PrePiHobListPointerLib/PrePiHobListPointerLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  RockchipPlatformLib|Platform/Mekotronics/R58X/Library/RockchipPlatformLib/RockchipPlatformLib.inf

[BuildOptions]
  GCC:*_*_*_PLATFORM_FLAGS = -I$(WORKSPACE)/Silicon/Rockchip/RK3588/Include -I$(WORKSPACE)/Platform/Rockchip/RK3588/Include -I$(WORKSPACE)/Silicon/Rockchip/Include

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsFeatureFlag.common]
  #  If TRUE, Graphics Output Protocol will be installed on virtual handle created by ConsplitterDxe.
  #  It could be set FALSE to save size.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport|TRUE

[PcdsFixedAtBuild.common]
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|4

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L"$(FIRMWARE_VER)"

  # System Memory (1GB)
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x00000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x40000000
  
  # RK3588 CPU profile
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|1

  # SMBIOS platform config
  gRockchipTokenSpaceGuid.PcdPlatformName|"R58X"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"Mekotronics"
  gRockchipTokenSpaceGuid.PcdFamilyName|"R58"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://www.mekotronics.com/h-pd-75.html"
  gRockchipTokenSpaceGuid.PcdBoardName|"EDGE-RK3588-4D32-V1.2"
  gRockchipTokenSpaceGuid.PcdMemoryVendorName|"TBD"

  # I2C
  gRockchipTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x42, 0x43 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBuses|{ 0x0, 0x0 }
  gRockchipTokenSpaceGuid.PcdI2cControllersEnabled|{ 0x0 }
  gRockchipTokenSpaceGuid.PcdI2cClockFrequency|198000000
  gRockchipTokenSpaceGuid.PcdI2cBaudRate|100000
  gRockchipTokenSpaceGuid.PcdI2cBusCount|1
  gRockchipTokenSpaceGuid.PcdI2cDemoAddresses|{ 0x51 } #/* RTCYM8563TS 0x51@bus2 */
  gRockchipTokenSpaceGuid.PcdI2cDemoBuses|{ 0x2 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorAddresses|{ 0x42, 0x43 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorBuses|{ 0x0, 0x0 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorTags|{ 2, 3 } # SCMI_CLK_CPUB01, SCMI_CLK_CPUB23
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorMinVoltages|{ UINT32(550000), UINT32(550000) }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorMaxVoltages|{ UINT32(1050000), UINT32(1050000) }

  ## UART2 - Serial Terminal
  DEFINE SERIAL_BASE = 0xFEB50000 # UART2
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|$(SERIAL_BASE)
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|1500000
  gHisiTokenSpaceGuid.PcdSerialPortSendDelay|500000
  gHisiTokenSpaceGuid.PcdUartClkInHz|24000000

  ## SPI - SPI2 for test
  gRockchipTokenSpaceGuid.SpiTestBaseAddr|0xFEB20000
  gRockchipTokenSpaceGuid.SpiRK806BaseAddr|0xFEB20000
  ## PL031 RealTimeClock
  #gArmPlatformTokenSpaceGuid.PcdPL031RtcBase|0xF8003000

  ## NOR FLASH
  gRockchipTokenSpaceGuid.FspiBaseAddr|0xFE2B0000

  ## CRU
  gRockchipTokenSpaceGuid.CruBaseAddr|0xFD7C0000

  #gRockchipTokenSpaceGuid.PcdSpiVariableOffset|0x3C0000
  #
  # ARM General Interrupt Controller
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xfe600000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0xfe600000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0xfe680000

  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|3

  # GUID of the UI app
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE

  gEmbeddedTokenSpaceGuid.PcdMetronomeTickPeriod|1000

  #
  # DW SD card controller
  #
  gDesignWareTokenSpaceGuid.PcdDwEmmcDxeBaseAddress|0xfe2c0000
  gDesignWareTokenSpaceGuid.PcdDwEmmcDxeClockFrequencyInHz|100000000
  gDesignWareTokenSpaceGuid.PcdDwPermitObsoleteDrivers|TRUE
  gDesignWareTokenSpaceGuid.PcdDwEmmcDxeFifoDepth|256
  #
  # SDHCI controller
  #
  gRockchipTokenSpaceGuid.PcdSdhciDxeBaseAddress|0xfe2e0000

  #
  # PCIe controller
  #
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4ApbBaseAddress|0xfe150000
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4DbiBaseAddress|0xf5000000
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4CfgBaseAddress|0xf0000000
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4CfgSize|0x100000
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4IoBaseAddress|0xf0100000
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4IoSize|0x10000
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4MemBaseAddress|0xf0200000
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4MemSize|0xe00000
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4MemBaseAddress64|0x901000000 #deduct 0x1000000 ECAM space
  gRockchipTokenSpaceGuid.PcdPcieRootPort3x4MemSize64|0x3f000000

  #
  # Fastboot
  #
  gEmbeddedTokenSpaceGuid.PcdAndroidFastbootUsbVendorId|0x2207
  gEmbeddedTokenSpaceGuid.PcdAndroidFastbootUsbProductId|0x0001

  #
  # USB2 EHCI + OHCI companion controllers
  #
  gRockchipTokenSpaceGuid.PcdEhciBaseAddress|0xfc800000
  gRockchipTokenSpaceGuid.PcdNumEhciController|2
  gRockchipTokenSpaceGuid.PcdEhciSize|0x40000
  gRockchipTokenSpaceGuid.PcdOhciSize|0x40000

  #
  # DWC3 controller
  #
  gRockchipTokenSpaceGuid.PcdDwc3BaseAddresses|{ UINT32(0xfc000000), UINT32(0xfc400000), UINT32(0xfcd00000) }
  gRockchipTokenSpaceGuid.PcdDwc3Size|0x400000

  #
  # Android Loader
  #
  gRK3588TokenSpaceGuid.PcdAndroidBootDevicePath|L"\\EFI\\BOOT\\GRUBAA64.EFI"
  gRK3588TokenSpaceGuid.PcdSdBootDevicePath|L"VenHw(0D51905B-B77E-452A-A2C0-ECA0CC8D514A,00E023F70000000000)/SD(0x0)"
  gRK3588TokenSpaceGuid.PcdKernelBootArg|L"earlycon=uart8250,mmio32,0xfeb50000 root=PARTUUID=614e0000-0000 rw rootwait"
  gEmbeddedTokenSpaceGuid.PcdAndroidBootDevicePath|L"VenHw(100C2CFA-B586-4198-9B4C-1683D195B1DA)/HD(3,GPT,7A3F0000-0000-446A-8000-702F00006273,0x8000,0x20000)"
  #
  # Make VariableRuntimeDxe work at emulated non-volatile variable mode.
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvModeEnable|TRUE

  # ACPI Enable
  gRK3588TokenSpaceGuid.AcpiEnable|TRUE

  #
  # Display
  #
  gRockchipTokenSpaceGuid.PcdLcdPixelFormat|0x00000001
  gRockchipTokenSpaceGuid.PcdEdpId|0x00000000 #edp0
  #gRockchipTokenSpaceGuid.PcdEdpId|0x00000001 #edp1
  gRockchipTokenSpaceGuid.PcdHdmiId|0x00000000 #hdmi0
  #gRockchipTokenSpaceGuid.PcdHdmiId|0x00000001 #hdmi1

  #
  # ComboPhy
  #
  gRockchipTokenSpaceGuid.PcdComboPhyMode|{ $(CP_PCIE), $(CP_PCIE), $(CP_USB3) }

  #  BIT0  - Initialization message.<BR>
  #  BIT1  - Warning message.<BR>
  #  BIT2  - Load Event message.<BR>
  #  BIT3  - File System message.<BR>
  #  BIT4  - Allocate or Free Pool message.<BR>
  #  BIT5  - Allocate or Free Page message.<BR>
  #  BIT6  - Information message.<BR>
  #  BIT7  - Dispatcher message.<BR>
  #  BIT8  - Variable message.<BR>
  #  BIT10 - Boot Manager message.<BR>
  #  BIT12 - BlockIo Driver message.<BR>
  #  BIT14 - Network Driver message.<BR>
  #  BIT16 - UNDI Driver message.<BR>
  #  BIT17 - LoadFile message.<BR>
  #  BIT19 - Event message.<BR>
  #  BIT20 - Global Coherency Database changes message.<BR>
  #  BIT21 - Memory range cachability changes message.<BR>
  #  BIT22 - Detailed debug message.<BR>
  #  BIT31 - Error message.<BR>
!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0e
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000000
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0f
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x800B0507
!endif
# 0x800B05C7
# Use 0x807B55FF to enable all debug messages

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase64|0x007C0000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase64|0x007CF000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase64|0x007D0000

  #
  # Display
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|0x780
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|0x438
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|0

[PcdsDynamicHii.common.DEFAULT]
  #
  # CPU Performance
  #
  gRK3588TokenSpaceGuid.PcdCPULClusterClockPreset|L"CpuPerf_CPULClusterClockPreset"|gRK3588DxeFormSetGuid|0x0|2
  gRK3588TokenSpaceGuid.PcdCPULClusterClockCustom|L"CpuPerf_CPULClusterClockCustom"|gRK3588DxeFormSetGuid|0x0|1800
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterClockPreset|L"CpuPerf_CPUB01ClusterClockPreset"|gRK3588DxeFormSetGuid|0x0|2
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterClockCustom|L"CpuPerf_CPUB01ClusterClockCustom"|gRK3588DxeFormSetGuid|0x0|2400
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterClockPreset|L"CpuPerf_CPUB23ClusterClockPreset"|gRK3588DxeFormSetGuid|0x0|2
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterClockCustom|L"CpuPerf_CPUB23ClusterClockCustom"|gRK3588DxeFormSetGuid|0x0|2400
  gRK3588TokenSpaceGuid.PcdCPULClusterVoltageMode|L"CpuPerf_CPULClusterVoltageMode"|gRK3588DxeFormSetGuid|0x0|0
  gRK3588TokenSpaceGuid.PcdCPULClusterVoltageCustom|L"CpuPerf_CPULClusterVoltageCustom"|gRK3588DxeFormSetGuid|0x0|950000
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterVoltageMode|L"CpuPerf_CPUB01ClusterVoltageMode"|gRK3588DxeFormSetGuid|0x0|0
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterVoltageCustom|L"CpuPerf_CPUB01ClusterVoltageCustom"|gRK3588DxeFormSetGuid|0x0|1000000
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterVoltageMode|L"CpuPerf_CPUB23ClusterVoltageMode"|gRK3588DxeFormSetGuid|0x0|0
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterVoltageCustom|L"CpuPerf_CPUB23ClusterVoltageCustom"|gRK3588DxeFormSetGuid|0x0|1000000

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]
  #
  # PEI Phase modules
  #
  ArmPlatformPkg/PrePi/PeiUniCore.inf
  MdeModulePkg/Core/Pei/PeiMain.inf
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf

  #
  # DXE
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
  }

  #
  # Architectural Protocols
  #
  ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  #EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf {
  <LibraryClasses>
    RealTimeClockLib|EmbeddedPkg/Library/VirtualRealTimeClockLib/VirtualRealTimeClockLib.inf
  }
  EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf

  #PCIe
  Silicon/Rockchip/Library/PciExpressLib/PciExpressLib.inf
  Silicon/Rockchip/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  Silicon/Rockchip/Drivers/PciPlatform/PcieInitDxe.inf
  ArmPkg/Drivers/ArmPciCpuIo2Dxe/ArmPciCpuIo2Dxe.inf

  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf
  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  #MdeModulePkg/Bus/Pci/NvmExpressPei/NvmExpressPei.inf
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Bus/Pci/SataControllerDxe/SataControllerDxe.inf

  MdeModulePkg/Bus/Pci/NonDiscoverablePciDeviceDxe/NonDiscoverablePciDeviceDxe.inf
  

  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf

  ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf

  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  Silicon/Rockchip/Drivers/Vop2Dxe/Vop2Dxe.inf
  #Silicon/Rockchip/Library/DisplayLib/AnalogixDpLib.inf
  Silicon/Rockchip/Library/DisplayLib/DwHdmiQpLib.inf
  Silicon/Rockchip/Drivers/LcdGraphicsOutputDxe/LcdGraphicsOutputDxe.inf

  Platform/Mekotronics/Drivers/LogoDxe/LogoDxe.inf

  #
  # SCMI Driver
  #
  ArmPkg/Drivers/ArmScmiDxe/ArmScmiDxe.inf

  #
  # ACPI Support
  #
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  Platform/Mekotronics/R58X/AcpiTables/AcpiTables.inf

  #
  # Device tree
  #
  EmbeddedPkg/Drivers/DtPlatformDxe/DtPlatformDxe.inf {
  <LibraryClasses>
    DtPlatformDtbLoaderLib|EmbeddedPkg/Library/DxeDtPlatformDtbLoaderLibDefault/DxeDtPlatformDtbLoaderLibDefault.inf
  }

  #
  # GPIO
  #
  Platform/Rockchip/RK3588/RK3588GpioDxe/RK3588GpioDxe.inf

  #
  # Virtual Keyboard
  #
  EmbeddedPkg/Drivers/VirtualKeyboardDxe/VirtualKeyboardDxe.inf

  # I2C drivers
  Silicon/Rockchip/Drivers/I2c/I2cDxe/I2cDxe.inf
  MdeModulePkg/Bus/I2c/I2cDxe/I2cDxe.inf
  Silicon/Rockchip/Drivers/I2c/I2cDemoDxe/I2cDemoDxe.inf
  Silicon/Rockchip/Applications/I2cDemoTest/I2cDemoTest.inf
  Silicon/Rockchip/Drivers/I2c/Rk860xRegulatorDxe/Rk860xRegulatorDxe.inf

  #
  # MMC/SD
  #
  #EmbeddedPkg/Universal/MmcDxe/MmcDxe.inf
  #Silicon/Synopsys/DesignWare/Drivers/DwEmmcDxe/DwEmmcDxe.inf
  Silicon/Rockchip/Drivers/MmcDxe/MmcDxe.inf
  Silicon/Rockchip/Drivers/DwEmmcDxe/DwEmmcDxe.inf
  Silicon/Rockchip/Drivers/SdhciHostDxe/SdhciHostDxe.inf

  #
  # NOR FLASH
  #
  Silicon/Rockchip/Drivers/NorFlashDxe/NorFlashDxe.inf
  Silicon/Rockchip/Drivers/NorFlashDxe/RkFvbDxe.inf
  Silicon/Rockchip/Applications/SpiTool/SpiFlashCmd.inf

  #
  # AHCI Support
  #
  Silicon/Rockchip/Drivers/SataControllerDxe/SataControllerDxe.inf
  Silicon/Rockchip/Drivers/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf

  #
  # SPI TEST
  #
  # Silicon/Rockchip/Library/SpiLib/SpiTest.inf

  #
  # SMBIOS Support
  #
  Silicon/Rockchip/Drivers/PlatformSmbiosDxe/PlatformSmbiosDxe.inf
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  
  #
  # USB Ohci Controller
  #
  Silicon/Rockchip/Drivers/OhciDxe/OhciDxe.inf

  #
  # USB Ehci Controller
  #
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf

  #
  # USB Dwc3 Controller
  #
  Silicon/Rockchip/Drivers/UsbHcdInitDxe/UsbHcd.inf

  #
  # USB Xhci Controller
  #
  MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf

  #
  # USB Host Support
  #
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf

  #
  # USB Mass Storage Support
  #
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  #
  # USB Kb Support
  #
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf

  #
  # USB Mouse Support
  #
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf

  #
  # USB MouseAbsolutePointer Support
  #
  MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf

  #
  # USB Peripheral Support
  #
  EmbeddedPkg/Drivers/AndroidFastbootTransportUsbDxe/FastbootTransportUsbDxe.inf

  #
  # Fastboot
  #
  EmbeddedPkg/Application/AndroidFastboot/AndroidFastbootApp.inf

  #
  # Android Boot applications
  #
  EmbeddedPkg/Application/AndroidBoot/AndroidBootApp.inf

  #
  # UEFI Network Stack
  #
!include NetworkPkg/Network.dsc.inc
  #
  # AX88772 Ethernet Driver
  #
  Drivers/ASIX/Bus/Usb/UsbNetworking/Ax88772c/Ax88772c.inf

  #
  # FAT filesystem + GPT/MBR partitioning
  #
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf

  #
  # Bds
  #
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  MdeModulePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|Silicon/Rockchip/Applications/I2cDemoTest/I2cDemoTest.inf
      NULL|Silicon/Rockchip/Applications/SpiTool/SpiFlashCmd.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellAcpiViewCommandLib/UefiShellAcpiViewCommandLib.inf
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      OrderedCollectionLib|MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }
!ifdef $(INCLUDE_TFTP_COMMAND)
  ShellPkg/DynamicCommand/TftpDynamicCommand/TftpDynamicCommand.inf
!endif #$(INCLUDE_TFTP_COMMAND)

  #
  # Custom Applications and drivers
  #
  Silicon/Rockchip/Applications/MaskromReset/maskrom.inf

  # Platform drivers
  Silicon/Rockchip/RK3588/Drivers/RK3588Dxe/RK3588Dxe.inf

