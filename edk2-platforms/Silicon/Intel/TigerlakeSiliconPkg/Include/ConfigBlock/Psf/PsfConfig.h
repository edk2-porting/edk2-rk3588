/** @file
  Primary Sideband Fabric policy.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PSF_CONFIG_H_
#define _PSF_CONFIG_H_

#define PSF_CONFIG_REVISION 1
extern EFI_GUID gPsfConfigGuid;

#pragma pack (push,1)

/**
  The PSF_CONFIG block describes the expected configuration of the Primary
  Sideband Fabric.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  /**
    Psf Tcc (Time Coordinated Computing) Enable will decrease psf transaction latency by disable
    some psf power management features. <b>0: Disable</b>; 1: Enable.
  **/
  UINT32    TccEnable                :  1;
  UINT32    RsvdBits0                : 31;       ///< Reserved bits
} PSF_CONFIG;

#pragma pack (pop)

#endif // _PSF_CONFIG_H_
