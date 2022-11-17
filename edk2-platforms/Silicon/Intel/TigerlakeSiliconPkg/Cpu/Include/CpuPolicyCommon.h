/** @file
  CPU Policy Structure definition which will contain several config blocks during runtime.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_POLICY_COMMON_H_
#define _CPU_POLICY_COMMON_H_

#include <ConfigBlock.h>
#include <ConfigBlock/CpuConfig.h>
#include <ConfigBlock/CpuPidTestConfig.h>
#include <ConfigBlock/CpuPowerMgmtBasicConfig.h>
#include <ConfigBlock/CpuPowerMgmtCustomConfig.h>
#include <ConfigBlock/CpuPowerMgmtPsysConfig.h>
#include <ConfigBlock/CpuPowerMgmtTestConfig.h>
#include <ConfigBlock/VoltageRegulator/CpuPowerMgmtVrConfig.h>
#include <ConfigBlock/CpuTestConfig.h>
#include <ConfigBlock/CpuSecurityPreMemConfig.h>
#include <ConfigBlock/CpuConfigLibPreMemConfig.h>
#include <OverclockingConfig.h>

#endif // _CPU_POLICY_COMMON_H_
