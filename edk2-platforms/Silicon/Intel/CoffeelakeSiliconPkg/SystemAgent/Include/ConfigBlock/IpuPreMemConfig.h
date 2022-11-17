/** @file
  IPU policy definitions (PreMem)

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPU_CONFIG_PREMEM_H_
#define _IPU_CONFIG_PREMEM_H_

#pragma pack(push, 1)

#define IPU_PREMEM_CONFIG_REVISION 1

#define SA_IMR_IPU_CAMERA   0
#define SA_IMR_IPU_GEN      1

/**
 IPU PreMem configuration\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Offset  0-27 Config Block Header
  /**
  Offset 28:0 :
  <b>(Test)</b> It enables the SA IPU Device if supported and not fused off.
  If FALSE, all other policies in this config block will be ignored.
  <b>1=TRUE</b>;
  0=FALSE.
  **/
  UINT32    SaIpuEnable:1;
  /**
  Offset 28:1 :
  <b>(Test)</b> It configure the IPU IMR to IPU Camera or IPU Gen when IPU is enabled.
  If FALSE, all other policies in this config block will be ignored.
  <b>0=IPU Camera</b>;
  1=IPU Gen
  **/
  UINT32    SaIpuImrConfiguration:1;
  UINT32    RsvdBits0:30;                     /// Offset 28:2 :Reserved for future use.
} IPU_PREMEM_CONFIG;
#pragma pack(pop)

#endif // _IPU_PREMEM_CONFIG_H_
