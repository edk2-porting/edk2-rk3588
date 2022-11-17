/** @file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SETUP_TABLE_H_
#define _SETUP_TABLE_H_
#include <Guid/SocketVariable.h>
#include <Guid/SetupVariable.h>

#include <PchSetupVariable.h>
#include <Guid/FpgaSocketVariable.h>
#include <Guid/MemBootHealthGuid.h>

typedef struct {
  SOCKET_CONFIGURATION       SocketConfig;
  SYSTEM_CONFIGURATION       SystemConfig;
  PCH_SETUP                  PchSetup;
  MEM_BOOT_HEALTH_CONFIG     MemBootHealthConfig;
} SETUP_DATA;

#endif
