/** @file
  Policy details for miscellaneous configuration in System Agent

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SA_MISC_PEI_CONFIG_H_
#define _SA_MISC_PEI_CONFIG_H_

#pragma pack(push, 1)

#define SA_MISC_PEI_CONFIG_REVISION 1

/**
  This configuration block is to configure SA Miscellaneous variables during PEI Post-Mem.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
} SA_MISC_PEI_CONFIG;
#pragma pack(pop)

#endif // _SA_MISC_PEI_CONFIG_H_
