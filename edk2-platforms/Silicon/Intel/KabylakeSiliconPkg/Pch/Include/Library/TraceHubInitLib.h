/** @file
  Header file for TraceHub Init Lib.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _TRACE_HUB_INIT_LIB_H_
#define _TRACE_HUB_INIT_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <PchAccess.h>

///
/// The PCH_TRACE_HUB_ENABLE_MODE describes the desired TraceHub mode of operation
///
typedef enum {
  TraceHubModeDisabled       = 0,   ///< Pch TraceHub Disabled
  TraceHubModeHostDebugger   = 2,   ///< Pch TraceHub External Debugger Present
  TraceHubModeMax
} TRACE_HUB_ENABLE_MODE;

/**
  This function performs basic initialization for TraceHub
  This routine will consume address range 0xFE0C0000 - 0xFE3FFFFF for BARs usage.
  Although controller allows access to a 64bit address resource, PEI phase is a 32bit env,
  addresses greater than 4G is not allowed by CPU address space.
  So, the addresses must be limited to below 4G and UBARs should be set to 0.
  If this routine is called by platform code, it is expected EnableMode is passed in as PchTraceHubModeDisabled,
  relying on the Intel TH debugger to enable it through the "cratchpad0 bit [24]".
  By this practice, it gives the validation team the capability to use single debug BIOS
  to validate the early trace functionality and code path that enable/disable Intel TH using BIOS policy.

  @param[in] EnableMode                 Trace Hub Enable Mode
**/
VOID
TraceHubInitialize (
  IN  UINT8                             EnableMode
 );

#endif // _TRACE_HUB_INIT_LIB_H_
