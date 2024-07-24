## @file
#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
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
  PLATFORM_NAME                  = FydetabDuo
  PLATFORM_VENDOR                = FydeInnovations
  PLATFORM_GUID                  = de3232fb-1716-4f63-a8fe-67a623ae5297
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

  #
  # HYM8563 RTC support
  # I2C location configured by PCDs below.
  #
  DEFINE RK_RTC8563_ENABLE = TRUE

  # No HDMI output on this platform
  DEFINE RK_DW_HDMI_QP_ENABLE = FALSE

  #
  # RK3588S-based platform
  #
!include Silicon/Rockchip/RK3588/RK3588SPlatform.dsc.inc

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
  gRockchipTokenSpaceGuid.PcdPlatformName|"Fydetab Duo"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"Fyde Innovations"
  gRockchipTokenSpaceGuid.PcdFamilyName|"Fydetab"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://fydetabduo.com/"
  gRockchipTokenSpaceGuid.PcdDeviceTreeName|"rk3588s-tablet-12c-linux"

  # I2C
  gRockchipTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x42, 0x43, 0x50, 0x51, 0x11 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBuses|{ 0x0, 0x0, 0x6, 0x6, 0x7 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBusesRuntimeSupport|{ FALSE, FALSE, FALSE, TRUE, FALSE }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorAddresses|{ 0x42, 0x43 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorBuses|{ 0x0, 0x0 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorTags|{ $(SCMI_CLK_CPUB01), $(SCMI_CLK_CPUB23) }
  gPcf8563RealTimeClockLibTokenSpaceGuid.PcdI2cSlaveAddress|0x51
  gRockchipTokenSpaceGuid.PcdRtc8563Bus|0x6

  #
  # CPU Performance default values
  #
  gRK3588TokenSpaceGuid.PcdCPULClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_MAX)
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_MAX)
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_MAX)

  #
  # PCIe/SATA/USB Combo PIPE PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_PCIE)

  #
  # USB/DP Combo PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdUsbDpPhy0Supported|TRUE
  gRK3588TokenSpaceGuid.PcdDp0LaneMux|{ 0x2, 0x3 }

  #
  # I2S
  #
  gRK3588TokenSpaceGuid.PcdI2S0Supported|TRUE

  # SD card detect signal is inverted
  gRockchipTokenSpaceGuid.PcdRkSdmmcCardDetectBroken|TRUE

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform.
#
################################################################################
[Components.common]
  # ACPI Support
  $(PLATFORM_DIRECTORY)/AcpiTables/AcpiTables.inf

  # Splash screen logo
  $(PLATFORM_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
