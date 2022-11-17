/** @file
  Integrated Error Handler policy.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IEH_CONFIG_H_
#define _IEH_CONFIG_H_

#define IEH_MODE_BYPASS 0
#define IEH_MODE_ENABLE 1

#define IEH_CONFIG_REVISION 1
extern EFI_GUID gIehConfigGuid;

#pragma pack (push,1)

/**
  The IEH_CONFIG block describes the expected configuration of the PCH
  Integrated Error Handler.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  /**
    IEH mode <b>0: Bypass Mode</b>; 1: Enable
  **/
  UINT32    Mode            :  1;
  UINT32    RsvdBits0       : 31;       ///< Reserved bits
} IEH_CONFIG;

#pragma pack (pop)

#endif // _IEH_CONFIG_H_
