/** @file
  Public include file for the CPU Configuration Library

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_CONFIG_LIB_H_
#define _CPU_CONFIG_LIB_H_

#include <Protocol/MpService.h>
#include <AcpiCpuData.h>
#include <CpuDataStruct.h>


// CPU C State Settings
#define C0_ENABLE                           0x00
#define C6_ENABLE                           0x03

//
// Structure conveying socket ID configuration information.
//
typedef struct {
  UINT32                    DefaultSocketId;
  UINT32                    NewSocketId;
} CPU_SOCKET_ID_INFO;

#endif
