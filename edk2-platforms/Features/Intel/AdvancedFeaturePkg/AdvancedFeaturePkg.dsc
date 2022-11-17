## @file
#  Advanced Feature Package build description file.
#
# Advanced features allow a board package to be extended with advanced functionality.
#
# This package should not contain any specific advanced features. The package has two responsibilities:
# 1. Present a consolidated and simplified view of all available advanced features to board packages.
# 2. Provide a simple, single package build for all available advanced features.
#
# Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = AdvancedFeaturePkg
  PLATFORM_GUID                       = 5E4C05BC-C5F0-4843-BAE1-3AAFE269DB8F
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/AdvancedFeaturePkg
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = DEFAULT
  PEI_ARCH                            = IA32
  DXE_ARCH                            = X64

[Packages]
  MdePkg/MdePkg.dec
  MinPlatformPkg/MinPlatformPkg.dec

!include AdvancedFeaturePkg/Include/AdvancedFeaturesPcd.dsc


################################################################################
#
# Advanced Feature Enable section - all advanced features are enabling for the
#                                   AdvancedFeaturePkg build.
#
################################################################################
[PcdsFeatureFlag]
  gAcpiDebugFeaturePkgTokenSpaceGuid.PcdAcpiDebugFeatureEnable            |TRUE
  gIpmiFeaturePkgTokenSpaceGuid.PcdIpmiFeatureEnable                      |TRUE
  gNetworkFeaturePkgTokenSpaceGuid.PcdNetworkFeatureEnable                |TRUE
  gS3FeaturePkgTokenSpaceGuid.PcdS3FeatureEnable                          |TRUE
  gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosFeatureEnable                  |TRUE
  gUsb3DebugFeaturePkgTokenSpaceGuid.PcdUsb3DebugFeatureEnable            |TRUE
  gUserAuthFeaturePkgTokenSpaceGuid.PcdUserAuthenticationFeatureEnable    |TRUE
  gLogoFeaturePkgTokenSpaceGuid.PcdLogoFeatureEnable                      |TRUE
  gLogoFeaturePkgTokenSpaceGuid.PcdJpgEnable                              |FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable                |FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable                   |FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable                      |FALSE

#
# Include common library
#
!include MinPlatformPkg/Include/Dsc/CoreCommonLib.dsc
!include MinPlatformPkg/Include/Dsc/CorePeiLib.dsc
!include MinPlatformPkg/Include/Dsc/CoreDxeLib.dsc

#
# This package builds all advanced features.
#
!include Include/AdvancedFeatures.dsc
