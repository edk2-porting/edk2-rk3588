/** @file
  Lpc policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _LPC_CONFIG_H_
#define _LPC_CONFIG_H_

#define LPC_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gLpcPreMemConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to LPC.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Enhance the port 8xh decoding.
    Original LPC only decodes one byte of port 80h, with this enhancement LPC can decode word or dword of port 80h-83h.
    @note: this will occupy one LPC generic IO range register. While this is enabled, read from port 80h always return 0x00.
    0: Disable, <b>1: Enable</b>
  **/
  UINT32    EnhancePort8xhDecoding      :  1;
  UINT32    RsvdBits                    : 31;     ///< Reserved bits
} PCH_LPC_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _LPC_CONFIG_H_
