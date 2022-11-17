/** @file
  Trace Hub policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _TRACEHUB_CONFIG_H_
#define _TRACEHUB_CONFIG_H_

#define TRACEHUB_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gTraceHubPreMemConfigGuid;

#pragma pack (push,1)

///
/// The PCH_TRACE_HUB_CONFIG block describes TraceHub settings for PCH.
///
typedef struct {
  CONFIG_BLOCK_HEADER   Header;          ///< Config Block Header
  UINT32  EnableMode         :  2;       ///< 0 = Disable <b> 2 = Host Debugger enabled </b>
  UINT32  RsvdBits0          : 30;       ///< Reserved bits
  UINT32  MemReg0Size;                   ///< Default is <b>0 (none)</b>.
  UINT32  MemReg1Size;                   ///< Default is <b>0 (none)</b>.
} PCH_TRACE_HUB_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _TRACEHUB_CONFIG_H_
