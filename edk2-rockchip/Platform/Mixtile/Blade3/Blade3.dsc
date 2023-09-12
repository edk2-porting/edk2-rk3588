## @file
#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
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
  PLATFORM_NAME                  = Blade3
  PLATFORM_VENDOR                = Mixtile
  PLATFORM_GUID                  = 6058aec7-9db7-4fdf-a2ce-19f58631071d
  PLATFORM_VERSION               = 0.2
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  VENDOR_DIRECTORY               = Platform/$(PLATFORM_VENDOR)
  PLATFORM_DIRECTORY             = $(VENDOR_DIRECTORY)/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Silicon/Rockchip/RK3588/RK3588.fdf
  RK_PLATFORM_FVMAIN_MODULES     = $(PLATFORM_DIRECTORY)/$(PLATFORM_NAME).Modules.fdf.inc

  # No status LED on this platform.
  DEFINE RK_STATUS_LED_ENABLE = FALSE

  #
  # RK3588-based platform
  #
!include Silicon/Rockchip/RK3588/RK3588Platform.dsc.inc

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################

[LibraryClasses.common]
  RockchipPlatformLib|$(PLATFORM_DIRECTORY)/Library/RockchipPlatformLib/RockchipPlatformLib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform.
#
################################################################################

[PcdsFixedAtBuild.common]
  # SMBIOS platform config
  gRockchipTokenSpaceGuid.PcdPlatformName|"Blade 3"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"Mixtile"
  gRockchipTokenSpaceGuid.PcdFamilyName|"Blade"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://www.mixtile.com/blade-3/"
  gRockchipTokenSpaceGuid.PcdDeviceTreeName|"rk3588-blade3-v101-linux.dtb"

  # I2C
  gRockchipTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x42, 0x43 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBuses|{ 0x0, 0x0 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBusesRuntimeSupport|{ FALSE, FALSE }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorAddresses|{ 0x42, 0x43 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorBuses|{ 0x0, 0x0 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorTags|{ $(SCMI_CLK_CPUB01), $(SCMI_CLK_CPUB23) }

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
  gRK3588TokenSpaceGuid.PcdComboPhy2Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_SATA)
  gRK3588TokenSpaceGuid.PcdComboPhy1ModeDefault|$(COMBO_PHY_MODE_PCIE)
  gRK3588TokenSpaceGuid.PcdComboPhy2ModeDefault|$(COMBO_PHY_MODE_PCIE)

  #
  # USB/DP Combo PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdUsbDpPhy0Supported|TRUE
  gRK3588TokenSpaceGuid.PcdUsbDpPhy1Supported|TRUE
  gRK3588TokenSpaceGuid.PcdDp0LaneMux|{ 0x2, 0x3 }
  gRK3588TokenSpaceGuid.PcdDp1LaneMux|{ 0x2, 0x3 }

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform.
#
################################################################################
[Components.common]
  # ACPI Support
  $(PLATFORM_DIRECTORY)/AcpiTables/AcpiTables.inf

  # Splash screen logo
  $(VENDOR_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
