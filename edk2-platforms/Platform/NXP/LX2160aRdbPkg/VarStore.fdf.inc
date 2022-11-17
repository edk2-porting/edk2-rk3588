## @file
#  FDF include file with FD definition that defines an empty variable store.
#
#  Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.
#  Copyright (C) 2014, Red Hat, Inc.
#  Copyright (c) 2016, Linaro, Ltd. All rights reserved.
#  Copyright (c) 2016, Freescale Semiconductor. All rights reserved.
#  Copyright 2017-2020 NXP
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[FD.LX2160aRdbNv_EFI]
BaseAddress   = 0x20500000  #The base address of the FLASH device
Size          = 0x000C0000  #The size in bytes of the FLASH device
ErasePolarity = 1
BlockSize     = 0x1000
NumBlocks     = 0xC0

#
# Place NV Storage just above Platform Data Base
#
DEFINE NVRAM_AREA_VARIABLE_BASE                = 0x00000000
DEFINE NVRAM_AREA_VARIABLE_SIZE                = 0x00040000
DEFINE FTW_WORKING_BASE                        = $(NVRAM_AREA_VARIABLE_BASE) + $(NVRAM_AREA_VARIABLE_SIZE)
DEFINE FTW_WORKING_SIZE                        = 0x00040000
DEFINE FTW_SPARE_BASE                          = $(FTW_WORKING_BASE) + $(FTW_WORKING_SIZE)
DEFINE FTW_SPARE_SIZE                          = 0x00040000

#############################################################################
# LX2160ARDB NVRAM Area
# LX2160ARDB NVRAM Area contains: Variable + FTW Working + FTW Spare
#############################################################################


$(NVRAM_AREA_VARIABLE_BASE)|$(NVRAM_AREA_VARIABLE_SIZE)
gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase64|gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize
#NV_VARIABLE_STORE
DATA = {
  ## This is the EFI_FIRMWARE_VOLUME_HEADER
  # ZeroVector []
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  # FileSystemGuid: gEfiSystemNvDataFvGuid         =
  #   { 0xFFF12B8D, 0x7696, 0x4C8B,
  #     { 0xA9, 0x85, 0x27, 0x47, 0x07, 0x5B, 0x4F, 0x50 }}
  0x8D, 0x2B, 0xF1, 0xFF, 0x96, 0x76, 0x8B, 0x4C,
  0xA9, 0x85, 0x27, 0x47, 0x07, 0x5B, 0x4F, 0x50,
  # FvLength: Flash Size : 0x4000000
  0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
  # Signature "_FVH"       # Attributes
  0x5f, 0x46, 0x56, 0x48, 0x36, 0x0E, 0x00, 0x00,
  # HeaderLength # CheckSum # ExtHeaderOffset #Reserved #Revision
  0x48, 0x00, 0x08, 0xA6, 0x00, 0x00, 0x00, 0x02,
  # Blockmap[0]: 0x4000 Blocks * 0x1000 Bytes / Block = SIZE_64MB
  0x00, 0x40, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
  # Blockmap[1]: End
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  ## This is the VARIABLE_STORE_HEADER
  # It is compatible with SECURE_BOOT_ENABLE == FALSE as well.
  # Signature: gEfiVariableGuid =
  #   { 0xddcf3616, 0x3275, 0x4164,
  #     { 0x98, 0xb6, 0xfe, 0x85, 0x70, 0x7f, 0xfe, 0x7d }}
  0x16, 0x36, 0xcf, 0xdd, 0x75, 0x32, 0x64, 0x41,
  0x98, 0xb6, 0xfe, 0x85, 0x70, 0x7f, 0xfe, 0x7d,
  # Size: 0x40000 (gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize) -
  #         0x48 (size of EFI_FIRMWARE_VOLUME_HEADER) = 0x3ffb8
  # This can speed up the Variable Dispatch a bit.
  0xB8, 0xFF, 0x03, 0x00,
  # FORMATTED: 0x5A #HEALTHY: 0xFE #Reserved: UINT16 #Reserved1: UINT32
  0x5A, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}

$(FTW_WORKING_BASE)|$(FTW_WORKING_SIZE)
gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase64|gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize
#NV_FTW_WORKING
DATA = {
  # EFI_FAULT_TOLERANT_WORKING_BLOCK_HEADER->Signature = gEdkiiWorkingBlockSignatureGuid         =
  #  { 0x9e58292b, 0x7c68, 0x497d, { 0xa0, 0xce, 0x65,  0x0, 0xfd, 0x9f, 0x1b, 0x95 }}
  0x2b, 0x29, 0x58, 0x9e, 0x68, 0x7c, 0x7d, 0x49,
  0xa0, 0xce, 0x65,  0x0, 0xfd, 0x9f, 0x1b, 0x95,
  # Crc:UINT32            #WorkingBlockValid:1, WorkingBlockInvalid:1, Reserved
  0x5b, 0xe7, 0xc6, 0x86, 0xFE, 0xFF, 0xFF, 0xFF,
  # WriteQueueSize: UINT64
  0xE0, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00
}

$(FTW_SPARE_BASE)|$(FTW_SPARE_SIZE)
gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase64|gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize
#NV_FTW_SPARE
