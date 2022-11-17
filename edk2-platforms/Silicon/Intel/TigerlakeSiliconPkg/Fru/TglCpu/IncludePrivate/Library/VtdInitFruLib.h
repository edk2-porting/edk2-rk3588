/** @file
  Vtd Initialization Fru Library header file

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _VTD_INIT_FRU_LIB_H_
#define _VTD_INIT_FRU_LIB_H_

///
/// TCSS DMA controller RMRR buffer 4MB for each DMA controller
///
#define RMRR_TCSS_DMA_SIZE  0x400000

extern UINT16  mDevEnMap[][2];
extern UINTN   mDevEnMapSize;

#endif // _VTD_INIT_FRU_LIB_H_
