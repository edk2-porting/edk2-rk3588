/** @file
  Policy definition for GNA Config Block

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GNA_CONFIG_H_
#define _GNA_CONFIG_H_
#pragma pack(push, 1)

#define GNA_CONFIG_REVISION 1
/**
 GNA config block for configuring GNA.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER         Header;                   ///< Offset 0-27 Config Block Header
  /**
  Offset 28:0
  This policy enables the GNA Device (SA Device 8) if supported.
  If FALSE, all other policies in this config block will be ignored.
  <b>1=TRUE</b>;
  0=FALSE.
   **/
  UINT32                      GnaEnable : 1;
  UINT32                      RsvdBits0 : 31; ///< Offset 28:1 :Reserved for future use
} GNA_CONFIG;
#pragma pack(pop)

#endif // _GNA_CONFIG_H_
