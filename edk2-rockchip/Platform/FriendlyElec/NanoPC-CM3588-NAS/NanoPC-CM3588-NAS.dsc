## @file
#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#  Copyright (c) 2023, Molly Sophia <mollysophia379@gmail.com>
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
  PLATFORM_NAME                  = NanoPC-CM3588-NAS
  PLATFORM_VENDOR                = FriendlyElec
  PLATFORM_GUID                  = e5022309-24e1-46e0-9d40-dcbc7293e609
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
  gRockchipTokenSpaceGuid.PcdPlatformName|"NanoPC CM3588-NAS"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"FriendlyElec"
  gRockchipTokenSpaceGuid.PcdFamilyName|"NanoPi CM3588"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://wiki.friendlyelec.com/wiki/index.php/CM3588_NAS_Kit"
  gRockchipTokenSpaceGuid.PcdDeviceTreeName|"rk3588-nanopc-cm3588-nas"

  # I2C
  gRockchipTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x42, 0x43, 0x51 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBuses|{ 0x0, 0x0, 0x6 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBusesRuntimeSupport|{ FALSE, FALSE, TRUE }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorAddresses|{ 0x42, 0x43 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorBuses|{ 0x0, 0x0 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorTags|{ $(SCMI_CLK_CPUB01), $(SCMI_CLK_CPUB23) }
  gPcf8563RealTimeClockLibTokenSpaceGuid.PcdI2cSlaveAddress|0x51
  gRockchipTokenSpaceGuid.PcdRtc8563Bus|0x6

  #
  # PCIe/SATA/USB Combo PIPE PHY support flags and default values
  # NanoPC CM3588 has one 2.5 GBE wired to the first PCIE2 port
  #
  gRK3588TokenSpaceGuid.PcdPcie30PhyModeDefault|$(PCIE30_PHY_MODE_NABIBI)
  gRK3588TokenSpaceGuid.PcdPcie30x2Supported|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy0Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_PCIE)
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
  $(PLATFORM_DIRECTORY)/DeviceTree/Mainline.inf

  # Splash screen logo
  $(VENDOR_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
