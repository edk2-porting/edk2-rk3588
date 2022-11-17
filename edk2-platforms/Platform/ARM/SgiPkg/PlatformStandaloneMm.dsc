## @file
#  StandaloneMM platform description file for SGI-575, RD-N1-Edge, RD-E1-Edge
#  and RD-V1 platforms.
#
#  Copyright (c) 2021, ARM Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = SgiMmStandalone
  PLATFORM_GUID                  = 503b97f6-1be9-4661-97fd-9a55bbd2680d
  PLATFORM_VERSION               = 1.0
  DSC_SPECIFICATION              = 0x0001001B
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platform/ARM/SgiPkg/PlatformStandaloneMm.fdf
  DEFINE DEBUG_MESSAGE           = TRUE

  # LzmaF86
  DEFINE COMPRESSION_TOOL_GUID   = D42AE6BD-1352-4bfb-909A-CA72A6EAE889

# include common definitions.
!include MdePkg/MdeLibs.dsc.inc
!include Platform/ARM/SgiPkg/SgiPlatformMm.dsc.inc

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFixedAtBuild]
  ## PL011 - Serial Terminal
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x7FF70000

!if $(SECURE_STORAGE_ENABLE) == TRUE
  ##Secure NOR Flash 2
  gArmSgiTokenSpaceGuid.PcdSmcCs2Base|0x10000000
  gArmSgiTokenSpaceGuid.PcdSysPeriphBase|0x1C000000
  gArmSgiTokenSpaceGuid.PcdSysPeriphSysRegBase|0x1C010000

  ##Secure Variable Storage in NOR Flash 2
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase|0x10000000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize|0x00100000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase|0x10100000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize|0x00100000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase|0x10200000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize|0x00100000
!endif
