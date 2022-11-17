/** @file
 *
 *  PEP device defines
 *
 *  Copyright (c) 2019, ARM Ltd. All rights reserved.
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

/*
 * Note: Not everything is here. At least SOC_STATE_TYPE is missing.
 */

#ifndef _RPI_PEP_H_INCLUDED_
#define _RPI_PEP_H_INCLUDED_

#include <IndustryStandard/Acpi.h>

#define PEP_MAX_DEPENDENCIES_PER_STATE 16
#define MAX_PROCESSOR_PATH_LENGTH 16
#define MAX_DEVICE_PATH_LENGTH 32
#define MAX_COMPONENT_COUNT 8
#define P_NUMBER_PROCESSORS 1
#define P_NUMBER_IDLE_STATES 1
#define P_NUMBER_PERF_STATES 0
#define P_NUMBER_DEVICES 1
#define P_NUMBER_COORDINATED_STATS 1

typedef struct _PEP_PROCESSOR_IDLE_STATE_INFO {
  UINT32 Ulong;
  UINT32 Latency;
  UINT32 BreakEvenDuration;
} PEP_PROCESSOR_IDLE_STATE_INFO, *PEP_PROCESSOR_IDLE_STATE_INFO;

typedef struct _PEP_PROCESSOR_IDLE_INFO_PLAT {
  //
  // Processor idle states.
  //
  PEP_PROCESSOR_IDLE_STATE_INFO IdleStates[P_NUMBER_IDLE_STATES];
} PEP_PROCESSOR_IDLE_INFO_PLAT, *PPEP_PROCESSOR_IDLE_INFO_PLAT;

typedef struct COORDINATED_DEPENDENCY_INFO {
  UINT32 ExpectedState;
  UINT32 TargetProcessor;
  UINT32 Ulong;
} COORDINATED_DEPENDENCY_INFO, *PCOORDINATED_DEPENDENCY_INFO;

typedef struct {
  COORDINATED_DEPENDENCY_INFO DependencyInfo[PEP_MAX_DEPENDENCIES_PER_STATE];
  UINT32 StateType;
  UINT32 Ulong;
  UINT32 Latency;
  UINT32 BreakEvenDuration;
  UINT32 DependencyCount;
  UINT32 MaximumDependencySize;
} COORDINATED_STATE_INFO;

typedef struct {
  UINT32 Unused;
} PEP_PROCESSOR_PERF_INFO;

typedef struct {
  UINT32 FState[MAX_COMPONENT_COUNT];
} COORDINATED_FSTATE_REQUIREMENT;

typedef struct {
  UINT32 DState;
} COORDINATED_DSTATE_REQUIREMENT;

//
// Top level device table
// *N.B. The exact length of the structure is determined by the NumberIdleStates/NumberPerfStates variables.
//

typedef struct _PEP_PROCESSOR_INFO_PLAT {
  WCHAR DevicePath[MAX_PROCESSOR_PATH_LENGTH]; // Null-terminated ACPI name
  ULONG FeedbackCounterCount;
  ULONG Flags;

  //
  // We are putting the idle/perf state count here (instead
  // of the PEP_PROCESSOR_xxx_INFO structure for the ease of parsing.
  //
  ULONG NumberIdleStates;
  ULONG NumberPerfStates;

  PEP_PROCESSOR_IDLE_INFO_PLAT IdleInfo;
  PEP_PROCESSOR_PERF_INFO PerfInfo;
} PEP_PROCESSOR_INFO_PLAT, *PPEP_PROCESSOR_INFO_PLAT;

typedef struct _PEP_PROCESSOR_TABLE_PLAT {
  UINT32 Version;
  UINT32 NumberProcessors;
  PEP_PROCESSOR_INFO_PLAT ProcessorInfo[P_NUMBER_PROCESSORS];
} PEP_PROCESSOR_TABLE_PLAT;

typedef struct _PEP_COORDINATED_STATE_TABLE_PLAT {
  ULONG Version;
  ULONG CoordinatedStateCount;
  COORDINATED_STATE_INFO CoordinatedStates[P_NUMBER_COORDINATED_STATS];
} PEP_COORDINATED_STATE_TABLE_PLAT, *PPEP_COORDINATED_STATE_TABLE_PLAT;

typedef struct _PEP_DEVICE_INFO_PLAT {
  WCHAR DevicePath[MAX_DEVICE_PATH_LENGTH]; // Null-terminated ACPI name
  ULONG DStateSupportMask;
  ULONG NumberComponents;

  COORDINATED_DSTATE_REQUIREMENT DStateRequirement[P_NUMBER_COORDINATED_STATS];
  COORDINATED_FSTATE_REQUIREMENT FStateRequirement[P_NUMBER_COORDINATED_STATS];
} PEP_DEVICE_INFO_PLAT, *PPEP_DEVICE_INFO_PLAT;

typedef struct _PEP_DEVICE_TABLE_PLAT {
  ULONG Version;
  ULONG NumberDevices;
  PEP_DEVICE_INFO_PLAT DeviceInfo[P_NUMBER_DEVICES];
} PEP_DEVICE_TABLE_PLAT, *PPEP_DEVICE_TABLE_PLAT;

#endif // _RPI_PEP_H_INCLUDED_
