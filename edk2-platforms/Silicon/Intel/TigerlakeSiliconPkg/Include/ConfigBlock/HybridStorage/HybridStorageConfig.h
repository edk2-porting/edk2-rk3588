/** @file
  Hybrid Storage policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _HYBRID_STORAGE_CONFIG_H_
#define _HYBRID_STORAGE_CONFIG_H_

#include <ConfigBlock.h>

#define HYBRID_STORAGE_CONFIG_REVISION 1

extern EFI_GUID gHybridStorageConfigGuid;

#pragma pack (push,1)

/**
  The HYBRID_STORAGE_CONFIG block describes the expected configuration for Hybrid Storage device

  <b>Revision 1</b>:
  - Init version
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Hybrid Storage Mode
    <b>0: Disable</b>, 1: Enable Dynamic Configuration
  **/
  UINT8    HybridStorageMode;
  UINT8    RsvdBytes[3];
} HYBRID_STORAGE_CONFIG;

#pragma pack (pop)

#endif // _HYBRID_STORAGE_CONFIG_H_
