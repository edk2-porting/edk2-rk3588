/** @file
  PCH Trace Hub policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_TRACEHUB_CONFIG_H_
#define _PCH_TRACEHUB_CONFIG_H_

#define PCH_TRACEHUB_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gPchTraceHubPreMemConfigGuid;

#pragma pack (push,1)

///
/// The PCH_TRACE_HUB_CONFIG block describes TraceHub settings for PCH.
///
typedef struct {
  CONFIG_BLOCK_HEADER   Header;          ///< Config Block Header
  UINT32  EnableMode         :  2;       ///< <b>0 = Disable</b>; 1 = Target Debugger mode; 2 = Host Debugger mode
  /**
  Pch Trace hub memory buffer region size policy.
  The avaliable memory size options are: <b>0:0MB (none)</b>, 1:1MB, 2:8MB, 3:64MB, 4:128MB, 5:256MB, 6:512MB.
  Refer to TRACE_BUFFER_SIZE in TraceHubCommon.h for supported settings.
  Note : Limitation of total buffer size (CPU + PCH) is 512MB.
  **/
  UINT32  MemReg0Size        :  8;
  UINT32  MemReg1Size        :  8;
  UINT32  RsvdBits0          : 14;       ///< Reserved bits
} PCH_TRACE_HUB_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _TRACEHUB_CONFIG_H_
