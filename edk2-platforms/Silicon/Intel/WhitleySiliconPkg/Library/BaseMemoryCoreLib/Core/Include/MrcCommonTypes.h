/** @file
  MrcCommonTypes.h

  @copyright
  Copyright 2015 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MrcCommonTypes_h_
#define _MrcCommonTypes_h_

#include "DataTypes.h"
#include <MemCommon.h>

typedef struct {
  MRC_MST       MemSsType;
  UINT32        NumDataCh;      // Total number of physical data channels in the MemSS
  UINT32        NumDataIoFubsPerCh;  // Total number of IO fubs in a data channel
  UINT32        NumDataIoFubsPerSubCh;  // Total number of IO fubs in a data sub channel
  UINT32        NumDqLanesPerCh;  // Number of active DQ lanes in a data channel (bus width)
} MRC_MSM;

typedef enum {
  MrcTtDelim = MAX_INT32
} MRC_TT;

#endif // _MrcCommonTypes_h_
