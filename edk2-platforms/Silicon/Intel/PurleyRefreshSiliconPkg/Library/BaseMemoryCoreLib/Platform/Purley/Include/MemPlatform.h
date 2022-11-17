/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef  _mem_platform_h
#define  _mem_platform_h

#include "DataTypes.h"

#ifdef   SERIAL_DBG_MSG
#define  MRC_TRACE  1
#endif


//
// Compatible BIOS Data Structure
//
#define BDAT_SUPPORT    0  //Memory Data Schema 4 and RMT Schema 5 of BDAT 4.0

//
// QR support
//
#define  QR_DIMM_SUPPORT 1

//
// Define to enable DIMM margin checking
//
#define  MARGIN_CHECK   1

//
// Define to enable SODIMM module support
//
#define  SODIMM_SUPPORT    1

//
// Define to enable ME UMA support
//
//#define ME_SUPPORT_FLAG   1

//
// Define to enable XMP
//
#define XMP_SUPPORT     1

// Define to enable DEBUG for NVMCTLR (LATE CMD CLK)
//#define DEBUG_LATECMDCLK      1

// Define to enable MRS Stacking
//#define MRS_STACKING    1

//
// Define to max ppr
//
#define MAX_PPR_ADDR_ENTRIES           20

//
//-------------------------------------
// DVP Platform-specific defines
//-------------------------------------
//
#ifdef   DVP_PLATFORM
#endif   // DVP_PLATFORM

//
//-------------------------------------
// CRB Platform-specific defines
//-------------------------------------
//
#ifdef   CRB_PLATFORM
#endif   // CRB_PLATFORM

#ifndef MAX_HA
#define MAX_HA              2                   // Number of Home Agents / IMCs
#endif

//SKX_TODO: I have removed NonPOR elements, I will delete this line before submit

#endif   // _mem_platform_h
