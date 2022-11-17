/** @file
  Configurations for CPU and PCH trace hub

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _TRACE_HUB_CONFIG_H_
#define _TRACE_HUB_CONFIG_H_

#include <ConfigBlock.h>

#define CPU_TRACEHUB_PREMEM_CONFIG_REVISION 1
#define PCH_TRACEHUB_PREMEM_CONFIG_REVISION 1

extern EFI_GUID gPchTraceHubPreMemConfigGuid;
extern EFI_GUID gCpuTraceHubPreMemConfigGuid;

typedef enum {
  CpuTraceHub,
  PchTraceHub
} TRACE_HUB_DEVICE;
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
  TraceBufferNone,
  TraceBuffer1M,
  TraceBuffer8M,
  TraceBuffer64M,
  TraceBuffer128M,
  TraceBuffer256M,
  TraceBuffer512M,
  TraceBufferMax
} TRACE_BUFFER_SIZE;

#pragma pack (push,1)
///
/// TRACE_HUB_CONFIG block describes TraceHub settings
///
typedef struct {
  /**
  Trace hub mode. Default is disabled.
  Target Debugger mode refers to debug tool running on target device itself and it works as a conventional PCI device;
  Host Debugger mode refers to SUT debugged via probe on host, configured as ACPI device with PCI configuration sapce hidden.
  <b>0 = Disable</b>; 1 = Target Debugger mode; 2 = Host Debugger mode
  Refer to TRACE_HUB_ENABLE_MODE
  **/
  UINT8                      EnableMode;
  /**
  Trace hub memory buffer region size policy.
  The avaliable memory size options are: 0:0MB (none), 1:1MB, <b>2:8MB</b>, 3:64MB, 4:128MB, 5:256MB, 6:512MB.
  Note : Limitation of total buffer size (CPU + PCH) is 512MB. If iTbt is enabled, the total size limits to 256 MB.
  Refer to TRACE_BUFFER_SIZE
  **/
  UINT8                      MemReg0Size;
  UINT8                      MemReg1Size;
  /**
  AET Trace. AET base address can be set to FW Base either from CPU trace hub or PCH one.
  AetEnabled must be exclusive, if AetEnabled = 1 for CPU trace hub, must AetEnabled = 0 for PCH one.
  The default is set to PCH.
  CPU Trace Hub
  <b>0 = Disabled</b>; 1 = Enabled
  PCH Trace Hub
  0 = Disabled; <b>1 = Enabled</b>
  **/
  UINT8                      AetEnabled;
} TRACE_HUB_CONFIG;

/**
  CPU Trace Hub PreMem Configuration
  Contains Trace Hub settings for CPU side tracing
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER        Header;           ///< Config Block Header
  TRACE_HUB_CONFIG           TraceHub;         ///< Trace Hub Config
} CPU_TRACE_HUB_PREMEM_CONFIG;

/**
  PCH Trace Hub PreMem Configuration
  Contains Trace Hub settings for PCH side tracing
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER        Header;           ///< Config Block Header
  TRACE_HUB_CONFIG           TraceHub;         ///< Trace Hub Config
} PCH_TRACE_HUB_PREMEM_CONFIG;

#pragma pack (pop)

#endif
