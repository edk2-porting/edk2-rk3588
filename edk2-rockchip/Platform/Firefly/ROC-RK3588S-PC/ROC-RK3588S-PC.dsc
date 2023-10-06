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
  PLATFORM_NAME                  = ROC-RK3588S-PC
  PLATFORM_VENDOR                = Firefly
  PLATFORM_GUID                  = d6741299-c347-4115-9f1a-1207fdf16ab0
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
  gRockchipTokenSpaceGuid.PcdPlatformName|"ROC-RK3588S-PC"
  gRockchipTokenSpaceGuid.PcdPlatformVendorName|"Firefly"
  gRockchipTokenSpaceGuid.PcdFamilyName|"ROC"
  gRockchipTokenSpaceGuid.PcdProductUrl|"https://en.t-firefly.com/product/industry/rocrk3588spc"
  gRockchipTokenSpaceGuid.PcdDeviceTreeName|"roc-rk3588s-pc"
