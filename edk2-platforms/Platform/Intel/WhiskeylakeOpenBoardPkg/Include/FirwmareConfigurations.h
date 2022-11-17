/** @file
  This header file provides definitions of firmware configuration.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _FIRMWARE_CONFIGURATION_H_
#define _FIRMWARE_CONFIGURATION_H_

typedef enum {
  FwConfigDefault = 0,
  FwConfigProduction,
  FwConfigTest,
  FwConfigMax
} FW_CONFIG;

#endif

