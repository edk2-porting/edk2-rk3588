/** @file
  Interface definition details between ME and platform drivers during DXE phase.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ME_POLICY_H_
#define _ME_POLICY_H_

#include <ConfigBlock.h>

/**
  ME Policy Protocol.
  All ME Policy Protocol change history listed here.

**/
#define ME_POLICY_PROTOCOL_REVISION  1

extern EFI_GUID gDxeMePolicyGuid;

#pragma pack (push,1)

/**
  ME policy provided by platform for DXE phase
  This protocol provides an interface to get Intel ME Configuration information

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct _ME_POLICY_PROTOCOL {
  CONFIG_BLOCK_TABLE_HEADER      TableHeader;
/*
  Individual Config Block Structures are added here in memory as part of AddConfigBlock()
*/
} ME_POLICY_PROTOCOL;

#pragma pack (pop)

#endif // _ME_POLICY_H_
