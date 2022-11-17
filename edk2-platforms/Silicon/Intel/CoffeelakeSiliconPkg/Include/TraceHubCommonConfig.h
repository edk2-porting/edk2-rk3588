/** @file
 Common configurations for CPU and PCH trace hub

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TRACE_HUB_COMMON_CONFIG_H_
#define _TRACE_HUB_COMMON_CONFIG_H_

///
/// The TRACE_HUB_ENABLE_MODE describes the desired TraceHub mode of operation
///
typedef enum {
  TraceHubModeDisabled       = 0,       ///< TraceHub Disabled
  TraceHubModeTargetDebugger = 1,       ///< TraceHub Target Debugger mode, debug on target device itself, config to PCI mode
  TraceHubModeHostDebugger   = 2,       ///< TraceHub Host Debugger mode, debugged by host with cable attached, config to ACPI mode
  TraceHubModeMax
} TRACE_HUB_ENABLE_MODE;


#endif
