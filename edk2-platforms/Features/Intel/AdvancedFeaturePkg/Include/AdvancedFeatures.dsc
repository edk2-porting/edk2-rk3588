## @file
#  DSC file for advanced features.
#
#  This file is intended to be included into another package so advanced features
#  can be conditionally built by enabling the respective feature via its FeaturePCD.
#
# Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

#
# Debug Advanced Features
#
!if gAcpiDebugFeaturePkgTokenSpaceGuid.PcdAcpiDebugFeatureEnable == TRUE
  !include Debugging/AcpiDebugFeaturePkg/Include/AcpiDebugFeature.dsc
!endif
!if gUsb3DebugFeaturePkgTokenSpaceGuid.PcdUsb3DebugFeatureEnable == TRUE
  !include Debugging/Usb3DebugFeaturePkg/Include/Usb3DebugFeature.dsc
!endif

#
# Network Advanced Features
#
!if gNetworkFeaturePkgTokenSpaceGuid.PcdNetworkFeatureEnable == TRUE
  !include Network/NetworkFeaturePkg/Include/NetworkFeature.dsc
!endif

#
# Out-of-Band Management Advanced Features
#
!if gIpmiFeaturePkgTokenSpaceGuid.PcdIpmiFeatureEnable == TRUE
  !include OutOfBandManagement/IpmiFeaturePkg/Include/IpmiFeature.dsc
!endif

#
# Power Management Advanced Features
#
!if gS3FeaturePkgTokenSpaceGuid.PcdS3FeatureEnable == TRUE
  !include PowerManagement/S3FeaturePkg/Include/S3Feature.dsc
!endif

#
# System Information Advanced Features
#
!if gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosFeatureEnable == TRUE
  !include SystemInformation/SmbiosFeaturePkg/Include/SmbiosFeature.dsc
!endif

#
# User Interface Advanced Features
#
!if gUserAuthFeaturePkgTokenSpaceGuid.PcdUserAuthenticationFeatureEnable == TRUE
  !include UserInterface/UserAuthFeaturePkg/Include/UserAuthFeature.dsc
!endif

!if gLogoFeaturePkgTokenSpaceGuid.PcdLogoFeatureEnable == TRUE
  !include LogoFeaturePkg/Include/LogoFeature.dsc
!endif
