/** @file
  Interface definition details between System Agent and platform drivers during DXE phase.

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _SA_POLICY_H_
#define _SA_POLICY_H_

#include <SaAccess.h>
#include <ConfigBlock.h>
#include <ConfigBlock/GraphicsDxeConfig.h>
#include <ConfigBlock/MiscDxeConfig.h>

///
/// Extern the GUID for protocol users.
///
extern EFI_GUID gSaPolicyProtocolGuid;
extern EFI_GUID gGraphicsDxeConfigGuid;

/**
  Don't change the original SA_POLICY_PROTOCOL_REVISION macro, external
  modules maybe have consumed this macro in their source code.  Directly
  update the SA_POLICY_PROTOCOL_REVISION version number may cause those
  external modules to auto mark themselves wrong version info.
  Always create new version macro for new Policy protocol interface.
**/
#define SA_POLICY_PROTOCOL_REVISION  1

/**
  SA DXE Policy

 The SA_POLICY_PROTOCOL producer drvier is recommended to
 set all the SA_POLICY_PROTOCOL size buffer zero before init any member parameter,
 this clear step can make sure no random value for those unknow new version parameters.

 Make sure to update the Revision if any change to the protocol, including the existing
 internal structure definations.\n
  Note: Here revision will be bumped up when adding/removing any config block under this structure.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;    ///< Offset 0-31
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} SA_POLICY_PROTOCOL;

#endif
