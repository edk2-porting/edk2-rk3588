/** @file

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef  _platformhost_h
#define  _platformhost_h

#include <PlatformInfoTypes.h>

//
// Maximum number of processor sockets and cores per socket supported by platform.
//
#include <MaxSocket.h>
#include <MaxCore.h>

//
// Post Package Repair
//

#define MAX_PPR_ADDR_ENTRIES           20
#define MAX_PPR_ADDR_ENTRIES_SPPR      40

#if !defined(SILENT_MODE)
#define DEBUG_CODE_BLOCK  1
#endif

#define UBIOS_GENERATION_EN               BIT22     // flag to enable DfxUbiosGeneration from Simics
#define HYBRID_SYSTEM_LEVEL_EMULATION_EN  BIT23     // flag to enable DfxHybridSystemLevelEmulation from Simics

#endif   // _platformhost_h

