/** @file

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef  _mem_defaults_h
#define  _mem_defaults_h

//
// SMBUS Clk Period default set by PcdMrcSmbusSpeedDefault
//
#define SMB_CLK_100K       0
#define SMB_CLK_400K       1
#define SMB_CLK_700K       2
#define SMB_CLK_1M         3

#define MAX_PARTIAL_MIRROR      4  //Maximum number of partial mirror regions that can be created

//
// Limit of channels to be tested by AdvMemTest
//
#define ADV_MT_LIST_LIMIT          8
#define ADV_MT_EMPTY_MASK          0xFFFFFFFF

#endif   // _mem_defaults_h
