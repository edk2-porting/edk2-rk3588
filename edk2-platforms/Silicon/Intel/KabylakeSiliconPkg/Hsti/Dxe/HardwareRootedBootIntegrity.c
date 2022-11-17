/** @file
  This file contains the tests for the HardwareRootedBootIntegrity bit

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiSiliconDxe.h"

/**
  Run tests for HardwareRootedBootIntegrity bit
**/
VOID
CheckHardwareRootedBootIntegrity (
  VOID
  )
{
  if ((mFeatureImplemented[0] & HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY) == 0) {
    return;
  }

  return ;
}
