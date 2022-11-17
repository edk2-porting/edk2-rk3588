/** @file
  P2sb policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _P2SB_CONFIG_H_
#define _P2SB_CONFIG_H_

#define P2SB_CONFIG_REVISION 1
extern EFI_GUID gP2sbConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to P2SB device.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    <b>(Test)</b>
    The sideband MMIO register access to specific ports will be locked
    before 3rd party code execution. Currently it disables PSFx access.
    This policy unlocks the sideband MMIO space for those IPs.
    <b>0: Lock sideband access </b>; 1: Unlock sideband access.
    NOTE: Do not set this policy "SbAccessUnlock" unless its necessary.
  **/
  UINT32    SbAccessUnlock    :  1;
  UINT32    Rsvdbits          : 31;    ///< Reserved bits
} PCH_P2SB_CONFIG;

#pragma pack (pop)

#endif // _P2SB_CONFIG_H_
