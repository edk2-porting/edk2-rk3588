## @file
#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#  Copyright (c) 2021-2022, Rockchip Limited. All rights reserved.
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
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
  # Must match defines in Silicon/Rockchip/RK3588/Include/VarStoreData.h
  #
  DEFINE CPU_PERF_CLUSTER_CLOCK_PRESET_BOOTDEFAULT  = 0
  DEFINE CPU_PERF_CLUSTER_CLOCK_PRESET_MIN          = 1
  DEFINE CPU_PERF_CLUSTER_CLOCK_PRESET_MAX          = 2
  DEFINE CPU_PERF_CLUSTER_CLOCK_PRESET_CUSTOM       = 3

  DEFINE COMBO_PHY_MODE_UNCONNECTED   = 0
  DEFINE COMBO_PHY_MODE_PCIE          = 1
  DEFINE COMBO_PHY_MODE_SATA          = 2
  DEFINE COMBO_PHY_MODE_USB3          = 3

  #
  # Network definition
  #
  DEFINE NETWORK_SNP_ENABLE             = FALSE
  DEFINE NETWORK_IP6_ENABLE             = FALSE
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE
  DEFINE NETWORK_VLAN_ENABLE            = FALSE

  #
  # Default peripheral support flags
  #
!ifndef RK_NOR_FLASH_ENABLE
  DEFINE RK_NOR_FLASH_ENABLE        = TRUE
!endif
!ifndef RK_860X_REGULATOR_ENABLE
  DEFINE RK_860X_REGULATOR_ENABLE   = TRUE
!endif
!ifndef RK_PCIE_ENABLE
  DEFINE RK_PCIE_ENABLE             = TRUE
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

  #
  # Base platform description
  #
!include Silicon/Rockchip/Rockchip.dsc.inc
!include MdePkg/MdeLibs.dsc.inc

################################################################################
#
# Library Class section - list of all common Library Classes needed by RK3588 platforms.
#
################################################################################

[LibraryClasses.common]
  # ACPI helpers
  AcpiLib|EmbeddedPkg/Library/AcpiLib/AcpiLib.inf

  # UART library
  SerialPortLib|Silicon/Hisilicon/Library/Dw8250SerialPortLib/Dw8250SerialPortLib.inf

  # SoC and memory descriptors
  ArmPlatformLib|Silicon/Rockchip/RK3588/Library/PlatformLib/PlatformLib.inf

  # DwEmmcDxe dependencies
  DwEmmcPlatformLib|Silicon/Rockchip/RK3588/Library/DwEmmcPlatformLib/DwEmmcPlatformLib.inf

  # Other SoC-specific libraries
  OtpLib|Silicon/Rockchip/RK3588/Library/OtpLib/OtpLib.inf
  GpioLib|Silicon/Rockchip/RK3588/Library/GpioLib/GpioLib.inf

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

  # UART2 - Serial Terminal
  DEFINE SERIAL_BASE = 0xFEB50000 # UART2
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|$(SERIAL_BASE)
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|1500000
  gHisiTokenSpaceGuid.PcdSerialPortSendDelay|500000
  gHisiTokenSpaceGuid.PcdUartClkInHz|24000000

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
  gRockchipTokenSpaceGuid.PcdDwEmmcDxeBaseAddress|0xfe2c0000
  gRockchipTokenSpaceGuid.PcdDwEmmcDxeClockFrequencyInHz|100000000
  gRockchipTokenSpaceGuid.PcdDwEmmcDxeFifoDepth|256

  #
  # DesignWare SD/eMMC Controller (SDHCI)
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
  # Display
  #
  gRockchipTokenSpaceGuid.PcdLcdPixelFormat|0x00000001
  gRockchipTokenSpaceGuid.PcdEdpId|0x00000000 #edp0
  #gRockchipTokenSpaceGuid.PcdEdpId|0x00000001 #edp1
  gRockchipTokenSpaceGuid.PcdHdmiId|0x00000000 #hdmi0
  #gRockchipTokenSpaceGuid.PcdHdmiId|0x00000001 #hdmi1

  #
  # CPU Performance default values
  #
  gRK3588TokenSpaceGuid.PcdCPULClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_BOOTDEFAULT)
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_BOOTDEFAULT)
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_BOOTDEFAULT)

  #
  # PCIe/SATA/USB Combo PIPE PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdComboPhy0Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy1Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy2Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_UNCONNECTED)
  gRK3588TokenSpaceGuid.PcdComboPhy1ModeDefault|$(COMBO_PHY_MODE_UNCONNECTED)
  gRK3588TokenSpaceGuid.PcdComboPhy2ModeDefault|$(COMBO_PHY_MODE_UNCONNECTED)

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

################################################################################
#
# Components Section - list of all common EDK II Modules needed by RK3588 platforms.
#
################################################################################
[Components.common]
  # General platform manager
  Silicon/Rockchip/RK3588/Drivers/RK3588Dxe/RK3588Dxe.inf
