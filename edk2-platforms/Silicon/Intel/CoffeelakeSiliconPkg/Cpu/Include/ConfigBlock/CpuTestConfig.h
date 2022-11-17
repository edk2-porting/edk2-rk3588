/** @file
  CPU Test Config Block.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_TEST_CONFIG_H_
#define _CPU_TEST_CONFIG_H_

#define CPU_TEST_CONFIG_REVISION 4

extern EFI_GUID gCpuTestConfigGuid;

#pragma pack (push,1)

/**
  CPU Test Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Fixed RsvdBits incorrect value.
  <b>Revision 3</b>:
  - Added CpuWakeUpTimer
  <b>Revision 4</b>:
  - Deprecate and move DebugInterfaceEnable to CPU_CONFIG.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT32                MlcStreamerPrefetcher           : 1;     ///< Enable or Disable MLC Streamer Prefetcher; 0: Disable; <b>1: Enable</b>.
  UINT32                MlcSpatialPrefetcher            : 1;     ///< Enable or Disable MLC Spatial Prefetcher; 0: Disable; <b>1: Enable</b>.
  UINT32                MonitorMwaitEnable              : 1;     ///< Enable or Disable Monitor /MWAIT instructions; 0: Disable; <b>1: Enable</b>.
  UINT32                MachineCheckEnable              : 1;     ///< Enable or Disable initialization of machine check registers; 0: Disable; <b>1: Enable</b>.
  UINT32                DebugInterfaceEnable            : 1;     ///< @deprecated Enable or Disable processor debug features; <b>0: Disable</b>; 1: Enable.
  UINT32                DebugInterfaceLockEnable        : 1;     ///< Lock or Unlock debug interface features; 0: Disable; <b>1: Enable</b>.
  UINT32                ProcessorTraceOutputScheme      : 1;     ///< Control on Processor Trace output scheme; <b>0: Single Range Output</b>; 1: ToPA Output.
  UINT32                ProcessorTraceEnable            : 1;     ///< Enable or Disable Processor Trace feature; <b>0: Disable</b>; 1: Enable.
  UINT32                ThreeStrikeCounterDisable       : 1;     ///< Disable Three strike counter; <b>0: FALSE</b>; 1: TRUE.
  /**
    This policy should be used to enable or disable Voltage Optimization feature.
    Recommended defaults:
     Enable  - For Mobile SKUs(U/Y)
     Disable - Rest of all SKUs other than Mobile.
  **/
  UINT32                VoltageOptimization             : 1;
  UINT32                CpuWakeUpTimer                  : 1;      ///< Enable or Disable long CPU wake up timer. 0: Disabled (8s); <b>1: Enabled (180s)</b>.
  UINT32                RsvdBits                        : 21;     ///< Reserved for future use
  /**
     Base address of memory region allocated for Processor Trace.
     Processor Trace requires 2^N alignment and size in bytes per thread, from 4KB to 128MB.
     - <b>NULL: Disable</b>
  **/
  EFI_PHYSICAL_ADDRESS  ProcessorTraceMemBase;
  /**
     Length in bytes of memory region allocated for Processor Trace.
     Processor Trace requires 2^N alignment and size in bytes per thread, from 4KB to 128MB.
     - <b>0: Disable</b>
  **/
  UINT32                ProcessorTraceMemLength;
} CPU_TEST_CONFIG;

#pragma pack (pop)

#endif // _CPU_TEST_CONFIG_H_
