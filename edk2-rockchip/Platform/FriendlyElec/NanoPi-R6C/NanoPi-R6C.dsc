## @file
#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#  Copyright (c) 2023, Molly Sophia <mollysophia379@gmail.com>
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
#  Copyright (c) 2023, Sergey Tyuryukanov <s199p.wa1k9r@gmail.com>
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
  PLATFORM_NAME                  = NanoPi-R6C
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
  gRockchipTokenSpaceGuid.PcdPlatformName|"NanoPi R6C"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"FriendlyElec"
  gRockchipTokenSpaceGuid.PcdFamilyName|"NanoPi"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://wiki.friendlyelec.com/wiki/index.php/NanoPi_R6"

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
  gRK3588TokenSpaceGuid.PcdComboPhy2Switchable|FALSE
  gRK3588TokenSpaceGuid.PcdComboPhy0ModeDefault|$(COMBO_PHY_MODE_PCIE)
  gRK3588TokenSpaceGuid.PcdComboPhy2ModeDefault|$(COMBO_PHY_MODE_PCIE)


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

