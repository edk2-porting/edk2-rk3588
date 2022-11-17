/** @file
  This header file provides platform specific definitions used
  by other modules for platform specific initialization.
  This is not suitable for consumption by ASL or VRF files.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//#include "CommonDefinitions.h"
#include "PchAccess.h"
#include "SaAccess.h"

//
// Need minimum of 48MB during PEI phase for IAG and some buffer for boot.
//
#define  PEI_MIN_MEMORY_SIZE               (10 * 0x800000 + 0x10000000)   // 80MB + 256MB
#define  PEI_RECOVERY_MIN_MEMORY_SIZE      (10 * 0x800000 + 0x10000000)   // 80MB + 256MB

#define FLASH_BLOCK_SIZE  0x10000

#define CPU_EXTERNAL_CLOCK_FREQ  0x64
#define CPU_FREQUENCY_MODE_100  0x64
#define FREQUENCY_RESOLUTION_3182  0xc6e
#define NDIVIDER_BASE_VALUE  0x19d
#define MDIVIDER_VALUE_13  0xd

#endif

