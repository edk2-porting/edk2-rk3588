/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SETUP_TABLE_H_
#define _SETUP_TABLE_H_

#include <Guid/SocketVariable.h>
#include <Guid/SetupVariable.h>
#include <Guid/PchRcVariable.h>

typedef struct {
  SOCKET_CONFIGURATION       SocketConfig;
  SYSTEM_CONFIGURATION       SystemConfig;
  PCH_RC_CONFIGURATION       PchRcConfig;
} SETUP_DATA;

#endif
