/** @file
  DMI policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _DMI_CONFIG_H_
#define _DMI_CONFIG_H_

#define DMI_CONFIG_REVISION 2
extern EFI_GUID gDmiConfigGuid;


#pragma pack (push,1)


/**
  The PCH_DMI_CONFIG block describes the expected configuration of the PCH for DMI.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add LegacyIoLowLatency support.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    0: Disable; <b>1: Enable</b> ASPM on PCH side of the DMI Link.
    While DmiAspm is enabled, DMI ASPM will be set to Intel recommended value.
  **/
  UINT32     DmiAspm            :  1;
  UINT32     PwrOptEnable       :  1;   ///< <b>0: Disable</b>; 1: Enable DMI Power Optimizer on PCH side.
  /**
    Set to enable low latenc of legacy IO.
    Some systems require lower IO latency irrespective of power.
    This is a tradeoff between power and IO latency.
    @note: Once this is enabled, DmiAspm is forced to disabled and so do Pcie DmiAspm in SystemAgent.
    <b>0:Disable</b>, 1:Enable
  **/
  UINT32     LegacyIoLowLatency :  1;
  UINT32     Rsvdbits           : 29;   ///< Reserved bits
} PCH_DMI_CONFIG;

#pragma pack (pop)

#endif // _DMI_CONFIG_H_
