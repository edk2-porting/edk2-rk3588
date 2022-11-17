## @file
#
# Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
# Copyright (c) 2021, American Megatrends International LLC.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

#
# TRUE is ENABLE. FALSE is DISABLE.
#

[PcdsFixedAtBuild]
  gMinPlatformPkgTokenSpaceGuid.PcdBootStage|4

[PcdsFeatureFlag]
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|FALSE

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 1
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 2
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 3
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 4
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 5
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|TRUE
!endif

  !if $(TARGET) == DEBUG
    gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable|TRUE
  !else
    gMinPlatformPkgTokenSpaceGuid.PcdSmiHandlerProfileEnable|FALSE
  !endif

  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable|TRUE

  gPlatformTokenSpaceGuid.PcdLinuxBootEnable|FALSE

!if gPlatformTokenSpaceGuid.PcdLinuxBootEnable == TRUE
  gPlatformTokenSpaceGuid.PcdFastBoot|TRUE
!else
  gPlatformTokenSpaceGuid.PcdFastBoot|FALSE
!endif

!if gPlatformTokenSpaceGuid.PcdFastBoot == TRUE
  gIpmiFeaturePkgTokenSpaceGuid.PcdIpmiFeatureEnable|FALSE
  gPlatformTokenSpaceGuid.PcdUpdateConsoleInBds|FALSE
!endif