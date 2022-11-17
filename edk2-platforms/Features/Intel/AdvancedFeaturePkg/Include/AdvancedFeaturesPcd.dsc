## @file
#  DSC file for defining Pcd of advanced features.
#
#  This file is intended to be included into another package so advanced features
#  can be conditionally built by enabling the respective feature via its FeaturePCD.
#
# Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

#
# The section references the package DEC files,
# it allow a FeaturePCD to be used in a conditional statement
#
[Packages]
  MdePkg/MdePkg.dec
  AcpiDebugFeaturePkg/AcpiDebugFeaturePkg.dec
  Usb3DebugFeaturePkg/Usb3DebugFeaturePkg.dec
  NetworkFeaturePkg/NetworkFeaturePkg.dec
  IpmiFeaturePkg/IpmiFeaturePkg.dec
  S3FeaturePkg/S3FeaturePkg.dec
  SmbiosFeaturePkg/SmbiosFeaturePkg.dec
  UserAuthFeaturePkg/UserAuthFeaturePkg.dec
  LogoFeaturePkg/LogoFeaturePkg.dec
  BeepDebugFeaturePkg/BeepDebugFeaturePkg.dec
  PostCodeDebugFeaturePkg/PostCodeDebugFeaturePkg.dec

#
# The section below sets all PCDs to FALSE in this DSC file so the feature is not enabled by default.
# Board can set PCDs to TRUE in its DSC file to enable a subset of advanced features
#
[PcdsFeatureFlag]
  gAcpiDebugFeaturePkgTokenSpaceGuid.PcdAcpiDebugFeatureEnable            |FALSE
  gIpmiFeaturePkgTokenSpaceGuid.PcdIpmiFeatureEnable                      |FALSE
  gNetworkFeaturePkgTokenSpaceGuid.PcdNetworkFeatureEnable                |FALSE
  gS3FeaturePkgTokenSpaceGuid.PcdS3FeatureEnable                          |FALSE
  gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosFeatureEnable                  |FALSE
  gUsb3DebugFeaturePkgTokenSpaceGuid.PcdUsb3DebugFeatureEnable            |FALSE
  gUserAuthFeaturePkgTokenSpaceGuid.PcdUserAuthenticationFeatureEnable    |FALSE
  gLogoFeaturePkgTokenSpaceGuid.PcdLogoFeatureEnable                      |FALSE
  gLogoFeaturePkgTokenSpaceGuid.PcdJpgEnable                              |FALSE
