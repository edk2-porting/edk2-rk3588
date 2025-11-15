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
  PLATFORM_NAME                  = AIOT-3588A
  PLATFORM_VENDOR                = ShiMeta
  PLATFORM_GUID                  = cb957236-be75-4077-aa31-0c3eae75c09c
  PLATFORM_VERSION               = 0.1
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
  gRockchipTokenSpaceGuid.PcdPlatformName|"ShiMeta AIOT-3588A"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"ShiMeta AIOT"
  gRockchipTokenSpaceGuid.PcdFamilyName|"ShiMeta AIOT"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://www.shimeta.com.cn/productinfo/2669315.html"
  gRockchipTokenSpaceGuid.PcdDeviceTreeName|"aiot-rk3588a"

  # I2C slaves (bus6 + bus7)
  gRockchipTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x20, 0x22, 0x62, 0x51, 0x11 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBuses|{ 0x6, 0x6, 0x6, 0x6, 0x7 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBusesRuntimeSupport|{ TRUE, TRUE, TRUE, TRUE, TRUE }

  # RTC
  gPcf8563RealTimeClockLibTokenSpaceGuid.PcdI2cSlaveAddress|0x51
  gRockchipTokenSpaceGuid.PcdRtc8563Bus|0x6



  #
  # PCIe/SATA/USB Combo PIPE PHY support flags and default values
  # NanoPC T6 has two 2.5 GBE wired to the first two PCIE2 ports, while the third one is wired to m.2 a+e key
  #
  gRK3588TokenSpaceGuid.PcdComboPhy0Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy1Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy2Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_PCIE)
  gRK3588TokenSpaceGuid.PcdComboPhy1ModeDefault|$(COMBO_PHY_MODE_PCIE)
  gRK3588TokenSpaceGuid.PcdComboPhy2ModeDefault|$(COMBO_PHY_MODE_PCIE)

    # USB2 PHY support flags
    gRK3588TokenSpaceGuid.PcdUsb2Phy0Supported | TRUE
    gRK3588TokenSpaceGuid.PcdUsb2Phy1Supported | TRUE
    gRK3588TokenSpaceGuid.PcdUsb2Phy2Supported | TRUE

  #
  # I2S
  #
  gRK3588TokenSpaceGuid.PcdI2S0Supported|TRUE

  #
  # Display support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdDisplayConnectors|{CODE({
    VOP_OUTPUT_IF_HDMI0,
    VOP_OUTPUT_IF_HDMI1
  })}

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

