/** @file
  Prototype of the PeiTbtPolicyLib library.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_TBT_POLICY_LIB_H_
#define _PEI_TBT_POLICY_LIB_H_

/**
  Install Tbt Policy

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
InstallPeiTbtPolicy (
  VOID
  );

/**
  Update PEI TBT Policy Callback
**/
VOID
EFIAPI
UpdatePeiTbtPolicyCallback (
  VOID
  );

/**
  Print PEI TBT Policy
**/
VOID
EFIAPI
TbtPrintPeiPolicyConfig (
  VOID
  );
#endif // _DXE_TBT_POLICY_LIB_H_

