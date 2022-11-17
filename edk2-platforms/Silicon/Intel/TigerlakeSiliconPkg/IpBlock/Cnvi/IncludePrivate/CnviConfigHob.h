/** @file
  This file defines the CNVi CONFIG HOB

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CNVI_CONFIG_HOB_H_
#define _CNVI_CONFIG_HOB_H_

#include <Base.h>

extern EFI_GUID gCnviConfigHobGuid;
#pragma pack (push,1)

/**
  This HOB is used to pass CNVi related private information to DXE phase
**/
typedef struct {
  EFI_HOB_GUID_TYPE EfiHobGuidType; ///< GUID HOB type structure for gCnviConfigHobGuid
  UINT32 Mode           :  1; ///< 0: Disabled, <b>1: Auto</b>
  UINT32 BtCore         :  1; ///< 0: Disabled, <b>1: Enabled</b>
  UINT32 BtAudioOffload :  1; ///< <b>0: Disabled</b>, 1: Enabled
  UINT32 RsvdBits0      : 29; ///< Reserved bits
} CNVI_CONFIG_HOB;
#pragma pack (pop)

#endif // _CNVI_CONFIG_HOB_H_
