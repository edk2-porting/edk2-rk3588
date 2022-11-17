/** @file
  This file defines the PCH Info Protocol.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_INFO_H_
#define _PCH_INFO_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                       gPchInfoProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_INFO_PROTOCOL PCH_INFO_PROTOCOL;

/**
  Protocol revision number
  Any backwards compatible changes to this protocol will result in an update in the revision number
  Major changes will require publication of a new protocol

  Revision 1:  Original version
  Revision 2:  Add PcieControllerCfg6 for KBL PCH-H
**/
#define PCH_INFO_PROTOCOL_REVISION    2

/**
 This protocol is used to provide the information of PCH controller.
**/
struct _PCH_INFO_PROTOCOL {
  /**
    This member specifies the revision of the PCH Info protocol. This field is used
    to indicate backwards compatible changes to the protocol. Platform code that
    consumes this protocol must read the correct revision value to correctly interpret
    the content of the protocol fields.
  **/
  UINT8        Revision;

  UINT8        PcieControllerCfg1;
  UINT8        PcieControllerCfg2;
  UINT8        PcieControllerCfg3;
  UINT8        PcieControllerCfg4;
  UINT8        PcieControllerCfg5;
  UINT8        PcieControllerCfg6;
};

#endif
