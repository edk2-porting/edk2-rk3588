/** @file
  Interface definition details between Pch and platform drivers during DXE phase.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_POLICY_H_
#define _PCH_POLICY_H_

#include <ConfigBlock.h>
#include <Private/PchConfigHob.h>
#include <Library/HobLib.h>

extern EFI_GUID gPchPolicyProtocolGuid;

#define PCH_POLICY_PROTOCOL_REVISION  1


/**
  PCH DXE Policy

  The PCH_POLICY_PROTOCOL producer drvier is recommended to
  set all the PCH_POLICY_PROTOCOL size buffer zero before init any member parameter,
  this clear step can make sure no random value for those unknown new version parameters.

  Make sure to update the Revision if any change to the protocol, including the existing
  internal structure definations.\n
  Note: Here revision will be bumped up when adding/removing any config block under this structure.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} PCH_POLICY_PROTOCOL;

#endif
