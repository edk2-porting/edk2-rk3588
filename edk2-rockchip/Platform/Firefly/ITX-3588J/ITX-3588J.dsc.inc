## @file
#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
#  Copyright (c) 2023, Shimrra Shai <shimmyshai00@gmail.com>
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
  FLASH_DEFINITION               = Silicon/Rockchip/RK3588/RK3588.fdf
  RK_PLATFORM_FVMAIN_MODULES     = Platform/Firefly/ITX-3588J/ITX-3588J.Modules.fdf.inc

  #
  # HYM8563 RTC support
  # I2C location configured by PCDs below.
  #
  DEFINE RK_RTC8563_ENABLE = TRUE

  #
  # PCA9555 GPIO extender support
  # I2C location configured by PCDs below.
  #
  DEFINE RK_PCA9555_ENABLE = TRUE

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
  RockchipPlatformLib|Platform/Firefly/ITX-3588J/Library/RockchipPlatformLib/RockchipPlatformLib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform.
#
################################################################################

[PcdsFixedAtBuild.common]
  # SMBIOS platform config
  gRockchipTokenSpaceGuid.PcdPlatformName|"ITX-3588J"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"Firefly"
  gRockchipTokenSpaceGuid.PcdFamilyName|"ITX"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://en.t-firefly.com/product/industry/itx3588j"
  gRockchipTokenSpaceGuid.PcdDeviceTreeName|"itx-3588j"

  # I2C
  gRockchipTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x42, 0x43, 0x51, 0x21, 0x22 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBuses|{ 0x0, 0x0, 0x6, 0x6, 0x6 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBusesRuntimeSupport|{ FALSE, FALSE, TRUE, FALSE, FALSE }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorAddresses|{ 0x42, 0x43 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorBuses|{ 0x0, 0x0 }
  gRockchipTokenSpaceGuid.PcdRk860xRegulatorTags|{ $(SCMI_CLK_CPUB01), $(SCMI_CLK_CPUB23) }
  gPcf8563RealTimeClockLibTokenSpaceGuid.PcdI2cSlaveAddress|0x51
  gRockchipTokenSpaceGuid.PcdRtc8563Bus|0x6
  gRockchipTokenSpaceGuid.PcdPca9555Address|0x21
  gRockchipTokenSpaceGuid.PcdPca9555Bus|0x6

  #
  # PCIe/SATA/USB Combo PIPE PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdComboPhy0Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy1Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy2Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_SATA)
  gRK3588TokenSpaceGuid.PcdComboPhy1ModeDefault|$(COMBO_PHY_MODE_PCIE)
  gRK3588TokenSpaceGuid.PcdComboPhy2ModeDefault|$(COMBO_PHY_MODE_USB3)

  #
  # USB/DP Combo PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdUsbDpPhy0Supported|TRUE
  gRK3588TokenSpaceGuid.PcdUsbDpPhy1Supported|TRUE
  gRK3588TokenSpaceGuid.PcdDp1LaneMux|{ 0x2, 0x3 }

  #
  # GMAC
  #
  gRK3588TokenSpaceGuid.PcdGmac0Supported|TRUE
  gRK3588TokenSpaceGuid.PcdGmac0TxDelay|0x45
  gRK3588TokenSpaceGuid.PcdGmac1Supported|TRUE
  gRK3588TokenSpaceGuid.PcdGmac1TxDelay|0x42

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
  Platform/Firefly/ITX-3588J/AcpiTables/AcpiTables.inf

  # Device Tree Support
  Platform/Firefly/ITX-3588J/DeviceTree/Vendor.inf

  # Splash screen logo
  $(VENDOR_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
