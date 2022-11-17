/** @file
  The GUID definition for Host Bridge Data Hob

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _HOST_BRIDGE_DATA_HOB_H_
#define _HOST_BRIDGE_DATA_HOB_H_

#include <Base.h>

extern EFI_GUID gHostBridgeDataHobGuid;
#pragma pack (push,1)

///
/// Host Bridge Data Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                       ///< GUID Hob type structure for gSaDataHobGuid
  UINT8                    EnableAbove4GBMmio;                   ///< 0=Disable above 4GB MMIO resource support, 1=Enable above 4GB MMIO resource support
  BOOLEAN                  SkipPamLock;                          ///< 0=All PAM registers will be locked in System Agent code, 1=Do not lock PAM registers in System Agent code.
  UINT8                    Rsvd1[2];                             ///< Reserved for future use
} HOST_BRIDGE_DATA_HOB;
#pragma pack (pop)
#endif
