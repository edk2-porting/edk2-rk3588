## @file
#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#  Copyright (c) 2021-2022, Rockchip Limited. All rights reserved.
#  Copyright (c) 2023-2025, Mario Bălănică <mariobalanica02@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  DEFINE CONFIG_NO_DEBUGLIB      = FALSE

  #
  # Shared Defines
  #   - must be kept in sync with their associated headers
  #   - values must be hexadecimal if used in byte array PCDs
  #

  #
  # Silicon/Rockchip/RK3588/Include/VarStoreData.h
  #
  DEFINE CPU_PERF_CLUSTER_CLOCK_PRESET_BOOTDEFAULT  = 0
  DEFINE CPU_PERF_CLUSTER_CLOCK_PRESET_MIN          = 1
  DEFINE CPU_PERF_CLUSTER_CLOCK_PRESET_MAX          = 2
  DEFINE CPU_PERF_CLUSTER_CLOCK_PRESET_CUSTOM       = 3

  DEFINE COMBO_PHY_MODE_UNCONNECTED   = 0
  DEFINE COMBO_PHY_MODE_PCIE          = 1
  DEFINE COMBO_PHY_MODE_SATA          = 2
  DEFINE COMBO_PHY_MODE_USB3          = 3

  DEFINE PCIE30_PHY_MODE_AGGREGATION  = 4
  DEFINE PCIE30_PHY_MODE_NANBNB       = 0
  DEFINE PCIE30_PHY_MODE_NANBBI       = 1
  DEFINE PCIE30_PHY_MODE_NABINB       = 2
  DEFINE PCIE30_PHY_MODE_NABIBI       = 3

  DEFINE COOLING_FAN_STATE_DISABLED   = 0
  DEFINE COOLING_FAN_STATE_ENABLED    = 1

  DEFINE USBDP_PHY_USB3_STATE_DISABLED  = 0
  DEFINE USBDP_PHY_USB3_STATE_ENABLED   = 1

  DEFINE CONFIG_TABLE_MODE_ACPI         =  0x00000001
  DEFINE CONFIG_TABLE_MODE_FDT          =  0x00000002
  DEFINE CONFIG_TABLE_MODE_ACPI_FDT     =  0x00000003

  DEFINE ACPI_PCIE_ECAM_COMPAT_MODE_AUTO        = 0x00000000
  DEFINE ACPI_PCIE_ECAM_COMPAT_MODE_SINGLE_DEV  = 0x00000001
  DEFINE ACPI_PCIE_ECAM_COMPAT_MODE_NXPMX6      = 0x00000002
  DEFINE ACPI_PCIE_ECAM_COMPAT_MODE_GRAVITON    = 0x00000004

  DEFINE FDT_COMPAT_MODE_UNSUPPORTED    = 0
  DEFINE FDT_COMPAT_MODE_VENDOR         = 1
  DEFINE FDT_COMPAT_MODE_MAINLINE       = 2

  DEFINE HDMI_SIGNALING_MODE_AUTO  = 0
  DEFINE HDMI_SIGNALING_MODE_DVI   = 1
  DEFINE HDMI_SIGNALING_MODE_HDMI  = 2

  #
  # Silicon/Rockchip/RK3588/Drivers/RK3588Dxe/CpuPerformance.h
  #
  DEFINE SCMI_CLK_CPUL                = 0x0
  DEFINE SCMI_CLK_CPUB01              = 0x2
  DEFINE SCMI_CLK_CPUB23              = 0x3

  #
  # Default peripheral support flags
  #
!ifndef RK_STATUS_LED_ENABLE
  DEFINE RK_STATUS_LED_ENABLE       = TRUE
!endif
!ifndef RK_NOR_FLASH_ENABLE
  DEFINE RK_NOR_FLASH_ENABLE        = TRUE
!endif
!ifndef RK_860X_REGULATOR_ENABLE
  DEFINE RK_860X_REGULATOR_ENABLE   = TRUE
!endif
!ifndef RK_SD_ENABLE
  DEFINE RK_SD_ENABLE               = TRUE
!endif
!ifndef RK_EMMC_ENABLE
  DEFINE RK_EMMC_ENABLE             = TRUE
!endif
!ifndef RK_AHCI_ENABLE
  DEFINE RK_AHCI_ENABLE             = TRUE
!endif
!ifndef RK_ANALOGIX_DP_ENABLE
  DEFINE RK_ANALOGIX_DP_ENABLE      = FALSE
!endif
!ifndef RK_DW_HDMI_QP_ENABLE
  DEFINE RK_DW_HDMI_QP_ENABLE       = TRUE
!endif

  #
  # RK3588-specific flags
  #
!ifndef RK3588_PCIE_ENABLE
  DEFINE RK3588_PCIE_ENABLE         = TRUE
!endif
!ifndef RK3588_GMAC_ENABLE
  DEFINE RK3588_GMAC_ENABLE         = TRUE
!endif

  #
  # Base platform description
  #
!include Silicon/Rockchip/Rockchip.dsc.inc

################################################################################
#
# Library Class section - list of all common Library Classes needed by RK3588 platforms.
#
################################################################################

[LibraryClasses.common.SEC]
  SerialPortLib|Silicon/Rockchip/Library/Dw8250SerialPortLib/Dw8250SerialPortLib.inf

[LibraryClasses.common]
  # Custom reset handler
  ResetSystemLib|Silicon/Rockchip/RK3588/Library/ResetSystemLib/ResetSystemLib.inf

  # ACPI helpers
  AcpiLib|EmbeddedPkg/Library/AcpiLib/AcpiLib.inf

  # Debug UART library
  SerialPortLib|Silicon/Rockchip/Library/Dw8250SerialPortLib/DebugDw8250SerialPortLib.inf

  # SoC and memory descriptors
  ArmPlatformLib|Silicon/Rockchip/RK3588/Library/PlatformLib/PlatformLib.inf

  # PCI Support
!if $(RK3588_PCIE_ENABLE) == TRUE
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  PciSegmentLib|Silicon/Rockchip/RK3588/Library/Rk3588PciSegmentLib/Rk3588PciSegmentLib.inf
  PciHostBridgeLib|Silicon/Rockchip/RK3588/Library/Rk3588PciHostBridgeLib/Rk3588PciHostBridgeLib.inf
  Pcie30PhyLib|Silicon/Rockchip/RK3588/Library/Pcie30PhyLib/Pcie30PhyLib.inf
!endif

  # RkSdmmcDxe dependencies
  RkSdmmcPlatformLib|Silicon/Rockchip/RK3588/Library/RkSdmmcPlatformLib/RkSdmmcPlatformLib.inf

  # DwcSdhciDxe dependencies
  DwcSdhciPlatformLib|Silicon/Rockchip/RK3588/Library/DwcSdhciPlatformLib/DwcSdhciPlatformLib.inf

  # Other SoC-specific libraries
  PlatformCruLib|Silicon/Rockchip/RK3588/Library/Rk3588CruLib/Rk3588CruLib.inf
  OtpLib|Silicon/Rockchip/RK3588/Library/OtpLib/OtpLib.inf
  GpioLib|Silicon/Rockchip/RK3588/Library/GpioLib/GpioLib.inf
  SaradcLib|Silicon/Rockchip/RK3588/Library/SaradcLib/SaradcLib.inf

[LibraryClasses.common.SEC]
  MemoryInitPeiLib|Silicon/Rockchip/RK3588/Library/MemoryInitPeiLib/MemoryInitPeiLib.inf

###################################################################################################
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process.
###################################################################################################

[BuildOptions]
  GCC:*_*_*_PLATFORM_FLAGS = -I$(WORKSPACE)/Silicon/Rockchip/RK3588/Include -I$(WORKSPACE)/Silicon/Rockchip/Include

################################################################################
#
# Pcd Section - list of all common EDK II PCD Entries used by RK3588 platforms.
#
################################################################################

[PcdsFixedAtBuild.common]
  # System Memory (1GB)
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x00000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x40000000

  # RK3588 CPU profile
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|1

  # SMBIOS platform config
  gRockchipTokenSpaceGuid.PcdProcessorName|"Rockchip RK3588 variant"
  gRockchipTokenSpaceGuid.PcdPlatformName|"Generic RK3588 platform"
  gRockchipTokenSpaceGuid.PcdMemoryVendorName|"TBD"

  # I2C
  gRockchipTokenSpaceGuid.PcdI2cClockFrequency|198000000
  gRockchipTokenSpaceGuid.PcdI2cBaudRate|100000
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorMinVoltages|{ UINT32(550000), UINT32(550000) }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorMaxVoltages|{ UINT32(1050000), UINT32(1050000) }

  # UART2 - Serial Terminal
  DEFINE SERIAL_BASE = 0xFEB50000 # UART2
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|$(SERIAL_BASE)
  gRockchipTokenSpaceGuid.PcdSerialPortSendDelay|500000
  gRockchipTokenSpaceGuid.PcdUartClkInHz|24000000

  # SPI - SPI2 for test
  gRockchipTokenSpaceGuid.SpiRK806BaseAddr|0xFEB20000

  # NOR FLASH
  gRockchipTokenSpaceGuid.FspiBaseAddr|0xFE2B0000

  # CRU
  gRockchipTokenSpaceGuid.CruBaseAddr|0xFD7C0000

  #
  # ARM General Interrupt Controller
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xfe600000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0xfe600000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0xfe680000

  # GUID of the UI app
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE

  gEmbeddedTokenSpaceGuid.PcdMetronomeTickPeriod|1000

  #
  # DesignWare SD Controller (MSHC)
  #
  gRockchipTokenSpaceGuid.PcdRkSdmmcBaseAddress|0xfe2c0000

  #
  # DesignWare SD/eMMC Controller (SDHCI)
  #
  gRockchipTokenSpaceGuid.PcdDwcSdhciBaseAddress|0xfe2e0000

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
  gRockchipTokenSpaceGuid.PcdDwc3Size|0x400000

  #
  # CPU Performance default values
  #
  gRK3588TokenSpaceGuid.PcdCPULClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_MAX)
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_MAX)
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_MAX)

  #
  # Cooling Fan definitions
  #
  gRK3588TokenSpaceGuid.PcdHasOnBoardFanOutput|FALSE

  #
  # PCIe/SATA/USB Combo PIPE PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdComboPhy0Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy1Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy2Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_UNCONNECTED)
  gRK3588TokenSpaceGuid.PcdComboPhy1ModeDefault|$(COMBO_PHY_MODE_UNCONNECTED)
  gRK3588TokenSpaceGuid.PcdComboPhy2ModeDefault|$(COMBO_PHY_MODE_UNCONNECTED)

  #
  # PCI Express 3.0 support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdPcie30Supported|FALSE
  gRK3588TokenSpaceGuid.PcdPcie30x2Supported|FALSE
  gRK3588TokenSpaceGuid.PcdPcie30PhyModeSwitchable|FALSE
  gRK3588TokenSpaceGuid.PcdPcie30PhyModeDefault|$(PCIE30_PHY_MODE_AGGREGATION)

  #
  # ACPI / Device Tree support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdConfigTableModeDefault|$(CONFIG_TABLE_MODE_ACPI_FDT)
  gRK3588TokenSpaceGuid.PcdAcpiPcieEcamCompatModeDefault|$(ACPI_PCIE_ECAM_COMPAT_MODE_AUTO)
  gRK3588TokenSpaceGuid.PcdFdtCompatModeDefault|$(FDT_COMPAT_MODE_MAINLINE)
  gRK3588TokenSpaceGuid.PcdFdtForceGopDefault|FALSE
  gRK3588TokenSpaceGuid.PcdFdtSupportOverridesDefault|FALSE
  gRK3588TokenSpaceGuid.PcdFdtOverrideFixupDefault|TRUE
  gRK3588TokenSpaceGuid.PcdFdtOverrideBasePathDefault|L""
  gRK3588TokenSpaceGuid.PcdFdtOverrideOverlayPathDefault|L""

  #
  # Display support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdDisplayConnectors|{ 0x0 }
  gRK3588TokenSpaceGuid.PcdDisplayModePresetDefault|{CODE({
    DISPLAY_MODE_NATIVE
  })}
  gRK3588TokenSpaceGuid.PcdDisplayModeCustomDefault|{CODE({
    .OscFreq     = 25175,
    .HActive     = 640,
    .HFrontPorch = 16,
    .HSync       = 96,
    .HBackPorch  = 48,
    .HSyncActive = 0,
    .VActive     = 480,
    .VFrontPorch = 10,
    .VSync       = 2,
    .VBackPorch  = 33,
    .VSyncActive = 0,
  })}
  gRK3588TokenSpaceGuid.PcdDisplayForceOutputDefault|TRUE
  gRK3588TokenSpaceGuid.PcdDisplayDuplicateOutputDefault|FALSE
  gRK3588TokenSpaceGuid.PcdDisplayRotationDefault|0
  gRK3588TokenSpaceGuid.PcdHdmiSignalingModeDefault|$(HDMI_SIGNALING_MODE_AUTO)

  #
  # Network support flags and default values
  #
  gRockchipTokenSpaceGuid.PcdNetworkStackEnabledDefault|TRUE
  gRockchipTokenSpaceGuid.PcdNetworkStackIpv4EnabledDefault|TRUE
  gRockchipTokenSpaceGuid.PcdNetworkStackIpv6EnabledDefault|TRUE
  gRockchipTokenSpaceGuid.PcdNetworkStackPxeBootEnabledDefault|TRUE
  gRockchipTokenSpaceGuid.PcdNetworkStackHttpBootEnabledDefault|TRUE

[PcdsPatchableInModule]
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|1500000

[PcdsDynamicDefault.common]
  #
  # Display
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|100
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|31
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|100
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|31

[PcdsDynamicHii.common.DEFAULT]
  #
  # CPU Performance
  #
  gRK3588TokenSpaceGuid.PcdCPULClusterClockPreset|L"CpuPerf_CPULClusterClockPreset"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdCPULClusterClockPresetDefault
  gRK3588TokenSpaceGuid.PcdCPULClusterClockCustom|L"CpuPerf_CPULClusterClockCustom"|gRK3588DxeFormSetGuid|0x0|1800
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterClockPreset|L"CpuPerf_CPUB01ClusterClockPreset"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdCPUB01ClusterClockPresetDefault
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterClockCustom|L"CpuPerf_CPUB01ClusterClockCustom"|gRK3588DxeFormSetGuid|0x0|2400
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterClockPreset|L"CpuPerf_CPUB23ClusterClockPreset"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdCPUB23ClusterClockPresetDefault
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterClockCustom|L"CpuPerf_CPUB23ClusterClockCustom"|gRK3588DxeFormSetGuid|0x0|2400
  gRK3588TokenSpaceGuid.PcdCPULClusterVoltageMode|L"CpuPerf_CPULClusterVoltageMode"|gRK3588DxeFormSetGuid|0x0|0
  gRK3588TokenSpaceGuid.PcdCPULClusterVoltageCustom|L"CpuPerf_CPULClusterVoltageCustom"|gRK3588DxeFormSetGuid|0x0|950000
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterVoltageMode|L"CpuPerf_CPUB01ClusterVoltageMode"|gRK3588DxeFormSetGuid|0x0|0
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterVoltageCustom|L"CpuPerf_CPUB01ClusterVoltageCustom"|gRK3588DxeFormSetGuid|0x0|1000000
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterVoltageMode|L"CpuPerf_CPUB23ClusterVoltageMode"|gRK3588DxeFormSetGuid|0x0|0
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterVoltageCustom|L"CpuPerf_CPUB23ClusterVoltageCustom"|gRK3588DxeFormSetGuid|0x0|1000000

  #
  # PCIe/SATA/USB Combo PIPE PHY
  #
  gRK3588TokenSpaceGuid.PcdComboPhy0Mode|L"ComboPhy0Mode"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault
  gRK3588TokenSpaceGuid.PcdComboPhy1Mode|L"ComboPhy1Mode"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdComboPhy1ModeDefault
  gRK3588TokenSpaceGuid.PcdComboPhy2Mode|L"ComboPhy2Mode"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdComboPhy2ModeDefault

  #
  # PCI Express 3.0
  #
  gRK3588TokenSpaceGuid.PcdPcie30State|L"Pcie30State"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdPcie30Supported
  gRK3588TokenSpaceGuid.PcdPcie30PhyMode|L"Pcie30PhyMode"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdPcie30PhyModeDefault

  #
  # ACPI / Device Tree
  #
  gRK3588TokenSpaceGuid.PcdConfigTableMode|L"ConfigTableMode"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdConfigTableModeDefault
  gRK3588TokenSpaceGuid.PcdAcpiPcieEcamCompatMode|L"AcpiPcieEcamCompatMode"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdAcpiPcieEcamCompatModeDefault
  gRK3588TokenSpaceGuid.PcdFdtCompatMode|L"FdtCompatMode"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdFdtCompatModeDefault
  gRK3588TokenSpaceGuid.PcdFdtForceGop|L"FdtForceGop"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdFdtForceGopDefault
  gRK3588TokenSpaceGuid.PcdFdtSupportOverrides|L"FdtSupportOverrides"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdFdtSupportOverridesDefault
  gRK3588TokenSpaceGuid.PcdFdtOverrideFixup|L"FdtOverrideFixup"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdFdtOverrideFixupDefault
  gRK3588TokenSpaceGuid.PcdFdtOverrideBasePath|L"FdtOverrideBasePath"|gRK3588DxeFormSetGuid|0x0|{ 0x0 }
  gRK3588TokenSpaceGuid.PcdFdtOverrideOverlayPath|L"FdtOverrideOverlayPath"|gRK3588DxeFormSetGuid|0x0|{ 0x0 }

  #
  # Cooling Fan
  #
  gRK3588TokenSpaceGuid.PcdCoolingFanState|L"CoolingFanState"|gRK3588DxeFormSetGuid|0x0|1
  gRK3588TokenSpaceGuid.PcdCoolingFanSpeed|L"CoolingFanSpeed"|gRK3588DxeFormSetGuid|0x0|50

  #
  # USB/DP PHY
  #
  gRK3588TokenSpaceGuid.PcdUsbDpPhy0Usb3State|L"UsbDpPhy0Usb3State"|gRK3588DxeFormSetGuid|0x0|$(USBDP_PHY_USB3_STATE_ENABLED)
  gRK3588TokenSpaceGuid.PcdUsbDpPhy1Usb3State|L"UsbDpPhy1Usb3State"|gRK3588DxeFormSetGuid|0x0|$(USBDP_PHY_USB3_STATE_ENABLED)

  #
  # Display
  #
  gRK3588TokenSpaceGuid.PcdDisplayConnectorsMask|L"DisplayConnectorsMask"|gRK3588DxeFormSetGuid|0x0|0|BS
  gRK3588TokenSpaceGuid.PcdDisplayModePreset|L"DisplayModePreset"|gRK3588DxeFormSetGuid|0x0|{ 0x0 }
  gRK3588TokenSpaceGuid.PcdDisplayModeCustom|L"DisplayModeCustom"|gRK3588DxeFormSetGuid|0x0|{ 0x0 }
  gRK3588TokenSpaceGuid.PcdDisplayConnectorsPriority|L"DisplayConnectorsPriority"|gRK3588DxeFormSetGuid|0x0|{ 0x0 }
  gRK3588TokenSpaceGuid.PcdDisplayForceOutput|L"DisplayForceOutput"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdDisplayForceOutputDefault
  gRK3588TokenSpaceGuid.PcdDisplayDuplicateOutput|L"DisplayDuplicateOutput"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdDisplayDuplicateOutputDefault
  gRK3588TokenSpaceGuid.PcdDisplayRotation|L"DisplayRotation"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdDisplayRotationDefault
  gRK3588TokenSpaceGuid.PcdHdmiSignalingMode|L"HdmiSignalingMode"|gRK3588DxeFormSetGuid|0x0|gRK3588TokenSpaceGuid.PcdHdmiSignalingModeDefault

################################################################################
#
# Components Section - list of all common EDK II Modules needed by RK3588 platforms.
#
################################################################################
[Components.common]
  # General platform manager
  Silicon/Rockchip/RK3588/Drivers/RK3588Dxe/RK3588Dxe.inf

  #
  # PCI Support
  #
!if $(RK3588_PCIE_ENABLE) == TRUE
  ArmPkg/Drivers/ArmPciCpuIo2Dxe/ArmPciCpuIo2Dxe.inf
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf
  EmbeddedPkg/Drivers/NonCoherentIoMmuDxe/NonCoherentIoMmuDxe.inf
  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
!endif

  #
  # USB/DP PHY Driver
  #
  Silicon/Rockchip/RK3588/Drivers/UsbDpPhyDxe/UsbDpPhyDxe.inf

  #
  # Rockchip GMAC Support
  #
!if $(RK3588_GMAC_ENABLE) == TRUE
  Silicon/Rockchip/RK3588/Drivers/GmacPlatformDxe/GmacPlatformDxe.inf
  Silicon/Synopsys/DesignWare/Drivers/DwcEqosSnpDxe/DwcEqosSnpDxe.inf {
    <PcdsFixedAtBuild>
      # 32-bit DMA limit
      gEmbeddedTokenSpaceGuid.PcdDmaDeviceOffset|0x00000000
      gEmbeddedTokenSpaceGuid.PcdDmaDeviceLimit|0xffffffff
  }
!endif

  #
  # ACPI Support
  #
  Silicon/Rockchip/RK3588/Drivers/AcpiPlatformDxe/AcpiPlatformDxe.inf

  #
  # Device Tree Support
  #
  Silicon/Rockchip/RK3588/Drivers/FdtPlatformDxe/FdtPlatformDxe.inf
