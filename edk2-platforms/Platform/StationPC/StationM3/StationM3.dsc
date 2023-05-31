#
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#  Copyright (c) 2021-2022, Rockchip Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = StationM3
  PLATFORM_GUID                  = d080df36-45e7-11ec-9726-f42a7dcb925d
  PLATFORM_VERSION               = 0.2
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  VENDOR_DIRECTORY               = Platform/StationPC
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platform/Firefly/ROC-RK3588S-PC/ROC-RK3588S-PC.fdf

!include Platform/Firefly/ROC-RK3588S-PC/ROC-RK3588S-PC.dsc.inc

[PcdsFixedAtBuild.common]
  # SMBIOS platform config
  gRockchipTokenSpaceGuid.PcdPlatformName|"Station M3"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"Station PC"
  gRockchipTokenSpaceGuid.PcdFamilyName|"Station M"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://www.stationpc.com/product/stationm3"

  #
  # CPU Performance default values
  #
  gRK3588TokenSpaceGuid.PcdCPULClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_MAX)
  gRK3588TokenSpaceGuid.PcdCPUB01ClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_MAX)
  gRK3588TokenSpaceGuid.PcdCPUB23ClusterClockPresetDefault|$(CPU_PERF_CLUSTER_CLOCK_PRESET_MAX)
