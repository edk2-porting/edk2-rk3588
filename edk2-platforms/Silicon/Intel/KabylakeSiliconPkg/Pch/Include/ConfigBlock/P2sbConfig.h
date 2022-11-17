/** @file
  P2sb policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _P2SB_CONFIG_H_
#define _P2SB_CONFIG_H_

#define P2SB_CONFIG_REVISION 2
extern EFI_GUID gP2sbConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to P2SB device.
  <b>Revision 1:</b>
  - Init version
  <b>Revision 2:</b>
  - Deprecate PsfUnlock and Add SbAccessUnlock to not lock SideBand register access.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    <b>(Test)</b>
    This unlock the SBI lock bit to allow SBI after post time. <b>0: Disable</b>; 1: Enable.
    NOTE: Do not set this policy "SbiUnlock" unless necessary.
  **/
  UINT32    SbiUnlock         :  1;
  UINT32    PsfUnlock         :  1; //@deprecated
  /**
    <b>(Test)</b>
    The SideBand registers will be locked before 3rd party code execution.
    This policy unlock the SideBand space. <b>0: Disable</b>; 1: Enable.
    NOTE: Do not set this policy "SbAccessUnlock" unless necessary.
  **/
  UINT32    SbAccessUnlock    :  1;
  UINT32    RsvdBits          : 29;
} PCH_P2SB_CONFIG;

#pragma pack (pop)

#endif // _P2SB_CONFIG_H_
