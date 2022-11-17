/** @file
  UBA KTI header file

  @copyright
  Copyright 2019 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Upi/KtiHost.h>

#ifndef _UBA_KTI_H_
#define _UBA_KTI_H_

#define PLATFORM_KTIEP_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'K', 'T', 'I')
#define PLATFORM_KTIEP_UPDATE_VERSION    01

typedef struct _PLATFORM_KTI_EPARAM_UPDATE_TABLE {
  UINT32                            Signature;
  UINT32                            Version;
  ALL_LANES_EPARAM_LINK_INFO        *AllLanesEparamTablePtr;
  UINT32                            SizeOfAllLanesEparamTable;
  PER_LANE_EPARAM_LINK_INFO         *PerLaneEparamTablePtr;
  UINT32                             SizeOfPerLaneEparamTable;
} PLATFORM_KTI_EPARAM_UPDATE_TABLE;
#endif //_UBA_KTI_H_
