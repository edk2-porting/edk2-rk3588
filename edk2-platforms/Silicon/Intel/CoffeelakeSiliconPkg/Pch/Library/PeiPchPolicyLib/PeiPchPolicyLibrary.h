/** @file
  Header file for the PeiPchPolicy library.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_PCH_POLICY_LIBRARY_H_
#define _PEI_PCH_POLICY_LIBRARY_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchPolicyLib.h>

/**
  Adds interrupt configuration for device

  @param[in/out] InterruptConfig         Pointer to interrupt config
**/
VOID
LoadDeviceInterruptConfig (
  IN OUT  PCH_INTERRUPT_CONFIG  *InterruptConfig
  );

#endif // _PEI_PCH_POLICY_LIBRARY_H_
