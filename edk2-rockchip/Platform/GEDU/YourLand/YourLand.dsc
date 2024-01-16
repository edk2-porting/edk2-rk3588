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
  PLATFORM_NAME                  = YourLand
  PLATFORM_VENDOR                = GEDU
  PLATFORM_GUID                  = 75AA17BE-7191-4929-900D-0AF69D4D23FC
  PLATFORM_VERSION               = 1.0
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  VENDOR_DIRECTORY               = Platform/$(PLATFORM_VENDOR)
  PLATFORM_DIRECTORY             = $(VENDOR_DIRECTORY)/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Silicon/Rockchip/RK3588/RK3588.fdf
  RK_PLATFORM_FVMAIN_MODULES     = $(PLATFORM_DIRECTORY)/$(PLATFORM_NAME).Modules.fdf.inc
  FIRMWARE_VER                   = v1.0.2-20231126-GSL
  #
  # HYM8563 RTC support
  # I2C location configured by PCDs below.
  #
  # changed to false on 2023-12-26 by Raymond
  # call back on 12-27
  #
  DEFINE RK_RTC8563_ENABLE = TRUE 

  #
  # YourLand uses 2 RK806s, no RK860 
  #
  DEFINE RK_860X_REGULATOR_ENABLE = FALSE

  #
  # the drivers in current upstream don't work well
  # we disable them and use those by old RK code
  #
  DEFINE RK_SD_ENABLE = FALSE
  DEFINE RK_EMMC_ENABLE = FALSE

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
  OrderedCollectionLib|MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
  BaseUcs2Utf8Lib|RedfishPkg/Library/BaseUcs2Utf8Lib/BaseUcs2Utf8Lib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform.
#
################################################################################

[PcdsFixedAtBuild.common]
  # SMBIOS platform config
  gRockchipTokenSpaceGuid.PcdPlatformName|"YourLand"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"GEDU"
  gRockchipTokenSpaceGuid.PcdFamilyName|"YourLand"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://www.nanocode.cn/#/yl/index"
  gRockchipTokenSpaceGuid.PcdDeviceTreeName|"yourland"

  # I2C
  gRockchipTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x51 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBuses|{ 0x2 }
  gRockchipTokenSpaceGuid.PcdI2cSlaveBusesRuntimeSupport|{ FALSE}
  # gRockchipTokenSpaceGuid.PcdRk860xRegulatorAddresses|{ 0x42, 0x43 }
  # gRockchipTokenSpaceGuid.PcdRk860xRegulatorBuses|{ 0x0, 0x0 }
  # gRockchipTokenSpaceGuid.PcdRk860xRegulatorTags|{ $(SCMI_CLK_CPUB01), $(SCMI_CLK_CPUB23) }
  gPcf8563RealTimeClockLibTokenSpaceGuid.PcdI2cSlaveAddress|0x51
  gRockchipTokenSpaceGuid.PcdRtc8563Bus|0x2

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
  gRK3588TokenSpaceGuid.PcdComboPhy1Switchable|TRUE
  gRK3588TokenSpaceGuid.PcdComboPhy2Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_PCIE)
  gRK3588TokenSpaceGuid.PcdComboPhy1ModeDefault|$(COMBO_PHY_MODE_PCIE)
  gRK3588TokenSpaceGuid.PcdComboPhy2ModeDefault|$(COMBO_PHY_MODE_PCIE)
  #
  # USB/DP Combo PHY support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdUsbDpPhy0Supported|TRUE
  gRK3588TokenSpaceGuid.PcdUsbDpPhy1Supported|TRUE
  gRK3588TokenSpaceGuid.PcdDp0LaneMux|{ 0x2, 0x3 }
  gRK3588TokenSpaceGuid.PcdDp1LaneMux|{ 0x0 }

  #
  # On-Board fan output
  #
  gRK3588TokenSpaceGuid.PcdHasOnBoardFanOutput|TRUE

  #
  # Nv mode, set it to false now to avoid reset looply, added by Raymond, but no use
  # also required to save data into spi-nor according to rk doc
  gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvModeEnable             | FALSE

#[PcdsDynamicEx]
#  gRK3588TokenSpaceGuid.PcdPlatformID|0

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  EfiResetSystemLib|Platform/RaspberryPi/Library/ResetLib/ResetLib.inf
  
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

  #
  # Multimedia Card Interface, added on 2023-11-6 when enable boot from emmc 
  #
  #EmbeddedPkg/Universal/MmcDxe/MmcDxe.inf
  #Silicon/Synopsys/DesignWare/Drivers/DwEmmcDxe/DwEmmcDxe.inf
  Silicon/Rockchip/Drivers/MmcDxe/MmcDxe.inf
  #Silicon/Rockchip/Drivers/DwEmmcDxe/DwEmmcDxe.inf
  Silicon/Rockchip/Drivers/SdhciHostDxe/SdhciHostDxe.inf

  #
  # ext4 filesystem support, added by Raymond on 2023-11-13 
  # 
  Features/Ext4Pkg/Ext4Dxe/Ext4Dxe.inf

  MdeModulePkg/Application/CapsuleApp/CapsuleApp.inf

  # shutdown via PSCI, added on 2023-12-28 
  EmbeddedPkg/ResetRuntimeDxe/ResetRuntimeDxe.inf