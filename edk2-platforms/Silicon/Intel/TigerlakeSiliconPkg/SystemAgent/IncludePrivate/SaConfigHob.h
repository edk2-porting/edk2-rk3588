/** @file
  The GUID definition for SaConfigHob

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SA_CONFIG_HOB_H_
#define _SA_CONFIG_HOB_H_

#include <Register/SaRegsHostBridge.h>
#include <Base.h>
#include "CpuPcieInfo.h"
#include <Library/PcdLib.h>

extern EFI_GUID gSaConfigHobGuid;
#define SA_VTD_ENGINE_NUMBER        3

#pragma pack (push,1)
///
/// DPR Directory Types
///
typedef enum {
  EnumDprDirectoryTxt   = 0,
} DPR_DIRECTORY_ELEMENT;

#define DPR_DIRECTORY_TYPE_TXT          0x01      ///< DPR directory type - TXT
#define DPR_DIRECTORY_TYPE_BIOSGUARD    0x02      ///< DPR directory type - BIOS Guard
#define DPR_DIRECTORY_MAX               1         ///< DPR Maximum Size

///
/// DPR directory entry definition
///
typedef struct {
  UINT8   Type;          ///< DPR Directory Type
  UINT8   Size;          ///< DPR Size in MB
  UINT32  PhysBase;      ///< Must be 4K aligned (bits 11..0 must be clear)
  UINT16  Reserved;      ///< Must be 0
} DPR_DIRECTORY_ENTRY;

///
/// System Agent Config Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                           ///< GUID Hob type structure for gSaConfigHobGuid
  DPR_DIRECTORY_ENTRY      DprDirectory[DPR_DIRECTORY_MAX];          ///< DPR directory entry definition
  UINT8                    ApertureSize;                             ///< Aperture size value
  BOOLEAN                  CridEnable;                               ///< This field inidicates if CRID is enabled or disabled (to support Intel(R) SIPP)
} SA_CONFIG_HOB;
#pragma pack (pop)
#endif
