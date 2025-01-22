## @file
#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#  Copyright (c) 2022, Xilin Wu <wuxilin123@gmail.com>
#  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
#  Copyright (c) 2024, Yun Dou <dixyes@gmail.com>
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
  PLATFORM_NAME                  = ROCK5BPlus
  PLATFORM_VENDOR                = Radxa
  PLATFORM_GUID                  = b3a14308-f25c-4b69-9387-47e0de34e3b7
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

  # GMAC is not exposed
  DEFINE RK3588_GMAC_ENABLE = FALSE

  #
  # HYM8563 RTC support
  # I2C location configured by PCDs below.
  #
  DEFINE RK_RTC8563_ENABLE = TRUE

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
  gRockchipTokenSpaceGuid.PcdPlatformName|"ROCK 5 Model B Plus"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"Radxa"
  gRockchipTokenSpaceGuid.PcdFamilyName|"ROCK 5"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://docs.radxa.com/en/rock5/rock5b"
  gRockchipTokenSpaceGuid.PcdDeviceTreeName|"rk3588-rock-5bp"

  # I2C
  gRockchipTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x42, 0x43, 0x51, 0x11 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBuses|{ 0x0, 0x0, 0x6, 0x7 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBusesRuntimeSupport|{ FALSE, FALSE, TRUE, FALSE }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorAddresses|{ 0x42, 0x43 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorBuses|{ 0x0, 0x0 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorTags|{ $(SCMI_CLK_CPUB01), $(SCMI_CLK_CPUB23) }
  gPcf8563RealTimeClockLibTokenSpaceGuid.PcdI2cSlaveAddress|0x51
  gRockchipTokenSpaceGuid.PcdRtc8563Bus|0x6

  #
  # PCIe/SATA/USB Combo PIPE PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdPcie30PhyModeSwitchable|TRUE
  gRK3588TokenSpaceGuid.PcdPcie30PhyModeDefault|$(PCIE30_PHY_MODE_NANBNB)
  gRK3588TokenSpaceGuid.PcdPcie30x2Supported|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy0Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy1Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy2Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_PCIE)
  gRK3588TokenSpaceGuid.PcdComboPhy1ModeDefault|$(COMBO_PHY_MODE_PCIE)
  gRK3588TokenSpaceGuid.PcdComboPhy2ModeDefault|$(COMBO_PHY_MODE_USB3)

  #
  # USB/DP Combo PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdUsbDpPhy0Supported|TRUE
  gRK3588TokenSpaceGuid.PcdUsbDpPhy1Supported|TRUE
  gRK3588TokenSpaceGuid.PcdDp0LaneMux|{ 0x2, 0x3 }
  gRK3588TokenSpaceGuid.PcdDp1LaneMux|{ 0x0 }

  #
  # I2S
  #
  gRK3588TokenSpaceGuid.PcdI2S0Supported|TRUE

  #
  # On-Board fan output
  #
  gRK3588TokenSpaceGuid.PcdHasOnBoardFanOutput|TRUE

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform.
#
################################################################################
[Components.common]
  # ACPI Support
  $(PLATFORM_DIRECTORY)/AcpiTables/AcpiTables.inf

  # Device Tree Support
  $(PLATFORM_DIRECTORY)/DeviceTree/Vendor.inf

  # Splash screen logo
  $(VENDOR_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
