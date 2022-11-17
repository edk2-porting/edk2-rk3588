/** @file
  Policy definition for Persisted Ram (Pram) Config Block

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PRAM_PREMEM_CONFIG__H_
#define _PRAM_PREMEM_CONFIG__H_
#pragma pack(push, 1)

#define PRAM_PREMEM_CONFIG_REVISION 1

/**
 Defines Pram configuration parameters.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;    ///< Offset 0-27 Config Block Header
  /**
  Offset 28:
  Size of Pram
  If disabled, or if PcdSaOcEnable is disabled, all other policies in this config block are ignored.
  <b>0=Disable</b>,
  1=4MB,
  2=16MB,
  3=64MB
  **/
  UINT8   Pram;
  UINT8   Rsvd[3];                ///< Offset 29 Reserved for DWORD alignment
} PRAM_PREMEM_CONFIG;
#pragma pack(pop)

#endif // _PRAM_CONFIG_H_
