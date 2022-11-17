## @file
#  FDF include file that defines the main macros and sets the dependent PCDs.
#
#  Copyright (C) 2014, Red Hat, Inc.
#  Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

#
# Default flash size is 2MB.
#
# Defining FD_SIZE_2MB on the build command line can override this.
#

DEFINE BLOCK_SIZE        = 0x1000
DEFINE VARS_SIZE         = 0x3e000
DEFINE VARS_BLOCKS       = 0x3e

DEFINE FW_BASE_ADDRESS   = 0xFFE00000
DEFINE FW_SIZE           = 0x00200000
DEFINE FW_BLOCKS         = 0x200
DEFINE CODE_BASE_ADDRESS = 0xFFE80000
DEFINE CODE_SIZE         = 0x00180000
DEFINE CODE_BLOCKS       = 0x180
DEFINE FVMAIN_SIZE       = 0x0016C000
DEFINE SECFV_OFFSET      = 0x001EC000
DEFINE SECFV_SIZE        = 0x14000


SET gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFdBaseAddress     = $(FW_BASE_ADDRESS)
SET gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFirmwareFdSize    = $(FW_SIZE)
SET gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFirmwareBlockSize = $(BLOCK_SIZE)

SET gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFlashNvStorageVariableBase = $(FW_BASE_ADDRESS)
SET gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize = 0xE000

SET gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFlashNvStorageEventLogBase = gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFlashNvStorageVariableBase + gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize
SET gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFlashNvStorageEventLogSize = $(BLOCK_SIZE)

SET gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFlashNvStorageFtwWorkingBase = gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFlashNvStorageEventLogBase + gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFlashNvStorageEventLogSize
SET gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize = $(BLOCK_SIZE)

SET gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFlashNvStorageFtwSpareBase = gSimicsOpenBoardPkgTokenSpaceGuid.PcdSimicsFlashNvStorageFtwWorkingBase + gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize
SET gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize = 0x10000

SET gEfiPchTokenSpaceGuid.PcdFlashAreaBaseAddress = 0xFFE00000
SET gEfiPchTokenSpaceGuid.PcdFlashAreaSize = 0x00200000

SET gMinPlatformPkgTokenSpaceGuid.PcdFlashAreaBaseAddress = gEfiPchTokenSpaceGuid.PcdFlashAreaBaseAddress
SET gMinPlatformPkgTokenSpaceGuid.PcdFlashAreaSize = gEfiPchTokenSpaceGuid.PcdFlashAreaSize

DEFINE MEMFD_BASE_ADDRESS = 0x800000
