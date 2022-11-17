/** @file
  PostCodeMap implementation.

  Copyright (c) 2010 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>

#include "PlatformStatusCodesInternal.h"

STATUS_CODE_TO_DATA_MAP mPostCodeProgressMap[] = {
  //
  // PEI
  //
  //Regular boot
  { PEI_CORE_STARTED, 0x10 },
  { PEI_CAR_CPU_INIT, 0x11 },
  { PEI_MEMORY_SPD_READ, 0x1D },
  { PEI_MEMORY_PRESENCE_DETECT, 0x1E },
  { PEI_MEMORY_TIMING, 0x1F},
  { PEI_MEMORY_CONFIGURING, 0x20 },
  { PEI_MEMORY_INIT, 0x21 },
  { PEI_MEMORY_INSTALLED, 0x31 },
  { PEI_CPU_INIT,  0x32 },
  { PEI_CPU_CACHE_INIT, 0x33 },
  { PEI_CPU_BSP_SELECT, 0x34 },
  { PEI_CPU_AP_INIT, 0x35 },
  { PEI_CPU_SMM_INIT, 0x36 },
  { PEI_MEM_NB_INIT, 0x37 },
  { PEI_MEM_SB_INIT, 0x3B },
  { PEI_DXE_IPL_STARTED, 0x4F },
  //Recovery
  { PEI_RECOVERY_AUTO, 0xF0 },
  { PEI_RECOVERY_USER, 0xF1 },
  { PEI_RECOVERY_STARTED, 0xF2 },
  { PEI_RECOVERY_CAPSULE_FOUND, 0xF3 },
  { PEI_RECOVERY_CAPSULE_LOADED, 0xF4 },
  //S3
  //{ PEI_S3_STARTED, 0xE0 },
  { PEI_S3_BOOT_SCRIPT, 0xE1 },
  //{ PEI_S3_VIDEO_REPOST, 0xE2 },
  { PEI_S3_OS_WAKE, 0xE3 },

  //
  // DXE
  //
  { DXE_CORE_STARTED, 0x1060 },
  { DXE_SBRUN_INIT, 0x1062 },
  { DXE_NB_HB_INIT, 0x1068 },
  { DXE_NB_INIT, 0x1069 },
  { DXE_NB_SMM_INIT, 0x106A },
  { DXE_SB_INIT, 0x1070 },
  { DXE_SB_SMM_INIT, 0x1071 },
  { DXE_SB_DEVICES_INIT, 0x1072 },
  { DXE_BDS_STARTED, 0x1090 },
  { DXE_PCI_BUS_BEGIN, 0x1092 },
  { DXE_PCI_BUS_HPC_INIT, 0x1093 },
  { DXE_PCI_BUS_ENUM, 0x1094 },
  { DXE_PCI_BUS_REQUEST_RESOURCES, 0x1095 },
  { DXE_PCI_BUS_ASSIGN_RESOURCES, 0x1096 },
  { DXE_CON_OUT_CONNECT, 0x1097 },
  { DXE_CON_IN_CONNECT, 0x1098 },
  { DXE_SIO_INIT, 0x1099 },
  { DXE_USB_BEGIN, 0x109A },
  { DXE_USB_RESET, 0x109B },
  { DXE_USB_DETECT, 0x109C },
  { DXE_USB_ENABLE, 0x109D },
  { DXE_IDE_BEGIN, 0x10A1 },
  { DXE_IDE_RESET, 0x10A2 },
  { DXE_IDE_DETECT, 0x10A3 },
  { DXE_IDE_ENABLE, 0x10A4 },
  { DXE_SCSI_BEGIN, 0x10A5 },
  { DXE_SCSI_RESET, 0x10A6 },
  { DXE_SCSI_DETECT, 0x10A7 },
  { DXE_SCSI_ENABLE, 0x10A8 },
  { DXE_SETUP_START, 0x10AB },
  { DXE_SETUP_INPUT_WAIT, 0x10AC },
  { DXE_READY_TO_BOOT, 0x10AD },
  { DXE_LEGACY_BOOT, 0x10AE },
  { DXE_EXIT_BOOT_SERVICES, 0x10AF },
  { RT_SET_VIRTUAL_ADDRESS_MAP_BEGIN, 0x10B0 },
  { RT_SET_VIRTUAL_ADDRESS_MAP_END, 0x10B1 },
  { DXE_LEGACY_OPROM_INIT, 0x10B2 },
  { DXE_RESET_SYSTEM, 0x10B3 },
  { DXE_USB_HOTPLUG, 0x10B4 },
  { DXE_PCI_BUS_HOTPLUG, 0x10B5 },
  {0,0}
};

STATUS_CODE_TO_DATA_MAP mPostCodeErrorMap[] = {
  //
  // PEI
  //
  //Regular boot
  { PEI_MEMORY_INVALID_TYPE, 0x50 },
  { PEI_MEMORY_INVALID_SPEED, 0x50 },
  { PEI_MEMORY_SPD_FAIL, 0x51 },
  { PEI_MEMORY_INVALID_SIZE, 0x52 },
  { PEI_MEMORY_MISMATCH, 0x52 },
  { PEI_MEMORY_NOT_DETECTED, 0x53 },
  { PEI_MEMORY_NONE_USEFUL, 0x53 },
  { PEI_MEMORY_ERROR, 0x54 },
  { PEI_MEMORY_NOT_INSTALLED, 0x55 },
  { PEI_CPU_INVALID_TYPE, 0x56 },
  { PEI_CPU_INVALID_SPEED, 0x56 },
  { PEI_CPU_MISMATCH, 0x57 },
  { PEI_CPU_SELF_TEST_FAILED, 0x58 },
  { PEI_CPU_CACHE_ERROR, 0x58 },
  { PEI_CPU_MICROCODE_UPDATE_FAILED, 0x59 },
  { PEI_CPU_NO_MICROCODE, 0x59 },
  { PEI_CPU_INTERNAL_ERROR, 0x5A },
  { PEI_CPU_ERROR, 0x5A },
  { PEI_RESET_NOT_AVAILABLE,0x5B },
  //Recovery
  { PEI_RECOVERY_PPI_NOT_FOUND, 0xF8 },
  { PEI_RECOVERY_NO_CAPSULE, 0xF9 },
  { PEI_RECOVERY_INVALID_CAPSULE, 0xFA },
  //S3 Resume
  { PEI_MEMORY_S3_RESUME_FAILED, 0xE8 },
  { PEI_S3_RESUME_PPI_NOT_FOUND, 0xE9 },
  { PEI_S3_BOOT_SCRIPT_ERROR, 0xEA },
  { PEI_S3_OS_WAKE_ERROR, 0xEB },

  //
  // DXE
  //
  { DXE_CPU_SELF_TEST_FAILED, 0x1058 },
  { DXE_NB_ERROR, 0x10D1 },
  { DXE_SB_ERROR, 0x10D2 },
  { DXE_ARCH_PROTOCOL_NOT_AVAILABLE, 0x10D3 },
  { DXE_PCI_BUS_OUT_OF_RESOURCES, 0x10D4 },
  { DXE_LEGACY_OPROM_NO_SPACE, 0x10D5 },
  { DXE_NO_CON_OUT, 0x10D6 },
  { DXE_NO_CON_IN, 0x10D7 },
  { DXE_INVALID_PASSWORD, 0x10D8 },
  { DXE_BOOT_OPTION_LOAD_ERROR, 0x10D9 },
  { DXE_BOOT_OPTION_FAILED, 0x10DA },
  { DXE_FLASH_UPDATE_FAILED, 0x10DB },
  { DXE_RESET_NOT_AVAILABLE, 0x10DC },
  {0,0}
};

STATUS_CODE_TO_DATA_MAP *mPostCodeStatusCodesMap[] = {
  //#define EFI_PROGRESS_CODE 0x00000001
  mPostCodeProgressMap,
  //#define EFI_ERROR_CODE 0x00000002
  mPostCodeErrorMap
  //#define EFI_DEBUG_CODE 0x00000003
};

/**
  Find the post code data from status code value.

  @param  Map              The map used to find in.
  @param  Value            The status code value.

  @return PostCode         0 for not found.

**/
UINT32
FindPostCodeData (
  IN STATUS_CODE_TO_DATA_MAP *Map,
  IN EFI_STATUS_CODE_VALUE   Value
  )
{
  while (Map->Value != 0) {
    if (Map->Value == Value) {
      return Map->Data;
    }
    Map++;
  }
  return 0;
}

/**
  Get PostCode from status code type and value.

  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.

  @return PostCode

**/
UINT32
EFIAPI
GetPostCodeFromStatusCode (
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value
  )
{
  UINT32 CodeTypeIndex;

  CodeTypeIndex = STATUS_CODE_TYPE (CodeType) - 1;

  if (CodeTypeIndex >= sizeof (mPostCodeStatusCodesMap) / sizeof(mPostCodeStatusCodesMap[0])) {
    return 0;
  }

  return FindPostCodeData (mPostCodeStatusCodesMap[CodeTypeIndex], Value);
}
