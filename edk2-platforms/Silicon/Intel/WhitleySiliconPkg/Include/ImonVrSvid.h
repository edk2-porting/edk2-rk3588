/** @file

  ImonVrSvid.h

  API Header for IMON VR PCD Structure

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IMON_VR_SVID_H_
#define _IMON_VR_SVID_H_

//Maximum IMON COUNT
#define MAX_IMON_COUNT 16

//End of List
#define IMON_ADDR_LIST_END 0xFF

typedef struct {
  UINT8 VrSvid[MAX_IMON_COUNT];
} VCC_IMON;

#endif //_IMON_VR_SVID_H_
