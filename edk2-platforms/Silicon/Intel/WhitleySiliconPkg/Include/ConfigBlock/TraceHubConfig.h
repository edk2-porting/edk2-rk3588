/** @file
  PCH Trace Hub policy

@copyright
  Copyright 2015 - 2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _TRACEHUB_CONFIG_H_
#define _TRACEHUB_CONFIG_H_

#define CPU_TRACEHUB_PREMEM_CONFIG_REVISION 1
#define PCH_TRACEHUB_PREMEM_CONFIG_REVISION 1

extern EFI_GUID gPchTraceHubPreMemConfigGuid;

///
/// The TRACE_HUB_ENABLE_MODE describes TraceHub mode of operation
///
typedef enum {
  TraceHubModeDisabled       = 0,
  TraceHubModeTargetDebugger = 1,
  TraceHubModeHostDebugger   = 2,
  TraceHubModeMax
} TRACE_HUB_ENABLE_MODE;

///
/// The TRACE_BUFFER_SIZE describes the desired TraceHub buffer size
///
typedef enum {
  TraceBufferNone = 0,
  TraceBuffer1M = SIZE_1MB,
  TraceBuffer8M = SIZE_8MB,
  TraceBuffer64M = SIZE_64MB,
  TraceBuffer128M = SIZE_128MB,
  TraceBuffer256M = SIZE_256MB,
  TraceBuffer512M = SIZE_512MB
} TRACE_BUFFER_SIZE;

#pragma pack (push,1)

///
/// TRACE_HUB_CONFIG block describes TraceHub settings.
///
typedef struct {
  /**
  Trace hub mode. Default is disabled.
  Target Debugger mode refers to debug tool running on target device itself and it works as a conventional PCI device;
  Host Debugger mode refers to SUT debugged via probe on host, configured as ACPI device with PCI configuration sapce hidden.
  <b>0 = Disable</b>; 1 = Target Debugger mode; 2 = Host Debugger mode
  Refer to TRACE_HUB_ENABLE_MODE
  **/
  UINT32                     EnableMode;
  /**
  Trace hub memory buffer region size policy.
  The avaliable memory size options are: <b>0:0MB (none)</b>, 1:1MB, 2:8MB, 3:64MB, 4:128MB, 5:256MB, 6:512MB.
  Refer to TRACE_BUFFER_SIZE.
  **/
  UINT32                     MemReg0Size;
  UINT32                     MemReg1Size;
} TRACE_HUB_CONFIG;

#pragma pack (pop)

#endif // _TRACEHUB_CONFIG_H_
