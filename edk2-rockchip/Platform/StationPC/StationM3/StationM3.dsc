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
  PLATFORM_NAME                  = StationM3
  PLATFORM_VENDOR                = StationPC
  PLATFORM_GUID                  = c292d925-29ff-4327-9ed9-517c9d921840
  PLATFORM_VERSION               = 0.2
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  VENDOR_DIRECTORY               = Platform/$(PLATFORM_VENDOR)
  PLATFORM_DIRECTORY             = $(VENDOR_DIRECTORY)/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

  #
  # Platform based on ROC-RK3588S-PC board
  #
!include Platform/Firefly/ROC-RK3588S-PC/ROC-RK3588S-PC.dsc.inc

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform.
#
################################################################################

[PcdsFixedAtBuild.common]
  # SMBIOS platform config
  gRockchipTokenSpaceGuid.PcdPlatformName|"Station M3"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"Station PC"
  gRockchipTokenSpaceGuid.PcdFamilyName|"Station M"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://www.stationpc.com/product/stationm3"
  gRockchipTokenSpaceGuid.PcdDeviceTreeName|"roc-rk3588s-pc"
