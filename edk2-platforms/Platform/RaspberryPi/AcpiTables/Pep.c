/** @file
 *
 *  PEP device tables
 *
 *  Copyright (c) 2019, ARM Ltd. All rights reserved.
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "Pep.h"

PEP_PROCESSOR_TABLE_PLAT RpiProcessors = {
  1, // Version
  1, // NumberProcessors
  {  // ProcessorInfo
    { // [0]
      L"\\_SB.CPU0", // DevicePath, wchar_t[16]
      0, // FeedbackCounterCount
      0x00000000, // Flags
      0, // NumberIdleStates
      0, // NumberPerfStates
      { // IdleInfo
      },
      { // perfinfo
      }
    }
  }
};

PEP_COORDINATED_STATE_TABLE_PLAT RpiCoordinatedStates = {
  1, // Version
  1, // CoordinatedStateCount
  { // CordinatedStates[]
    { // [0]
      { // DependencyInfo
        { // [0]
          1, // ExpectedState
          0, // TargetProcessor
          0x0 | 0x2 | 0x4, // LooseDependency = FALSE, InitialState = TRUE, DependentState = TRUE
        }
      },
      SOC_STATE_TYPE, // StateType
      0x1, // Flags
      0, // Latency
      0, // BreakEvenDuration
      1, // DependencyCount
      1, // MaximumDependencySize
    }
  }
};

PEP_DEVICE_TABLE_PLAT RpiDevices = {
  1, // Version
  1, // NumberDevices
  { // DeviceInfo
    { // [1]
      L"\\_SB.I2C0", // DevicePath, wchar_t[16]
      0x1 | (1 << 3), // DStateSupportMask (D0 and D3)
      1, // NumberCompoenents
      { // DStateRequirement
        { // [0]
          PowerDeviceD3 // DState
        }
      },
      { // FStateRequirement
        { // [0]
          { // FState
            { // [0]
              0
            }
          }
        }
      }
    }
  }
};
