/** @file
  Header file for the PolicyInitPei Library.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _POLICY_INIT_PEI_LIB_H_
#define _POLICY_INIT_PEI_LIB_H_

/**
  Initialize Intel PEI Platform Policy

**/
VOID
EFIAPI
PeiPolicyInitPreMem (
  VOID
  );

/**
  Initialize Intel PEI Platform Policy

**/
VOID
EFIAPI
PeiPolicyInit (
  VOID
  );
#endif
