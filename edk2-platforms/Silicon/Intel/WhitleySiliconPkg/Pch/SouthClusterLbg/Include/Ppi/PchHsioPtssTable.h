/** @file
  PCH policy PPI produced by a platform driver specifying HSIO PTSS Table.
  This PPI is consumed by the PCH PEI modules. This is deprecated.

  @copyright
  Copyright 2015 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_HSIO_PTSS_TABLE_H_
#define _PCH_HSIO_PTSS_TABLE_H_

///
/// HSIO PTSS table PPI GUID.
///
extern EFI_GUID  gPchHsioPtssTablePpiGuid;

///
/// The PCH_SBI_PTSS_HSIO_TABLE block describes HSIO PTSS settings for PCH.
///
typedef struct {
  UINT8       SbPortID;
  UINT8       LaneNum;
  UINT8       PhyMode;
  UINT16      Offset;
  UINT32      Value;
  UINT32      BitMask;
} PCH_SBI_PTSS_HSIO_TABLE;

#endif // _PCH_HSIO_PTSS_TABLE_H_
