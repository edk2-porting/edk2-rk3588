/** @file
  This provides a versatile abstraction for the SiliconPolicyInitLib library interface.

  This defines the typedef necessary for PPI and protocol use.
  This defines structs for PPI and protocol production and consumption.
  There is a single GUID defining both PPI and protocol.

Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SILICON_POLICY_INIT_LIB_INTERFACE_H_
#define _SILICON_POLICY_INIT_LIB_INTERFACE_H_

#include <Library/SiliconPolicyInitLib.h>

//
// All interface declarations should refer to the SiliconPolicyInitLib for description of functionality
//

typedef
VOID *
(EFIAPI *PEI_SILICON_POLICY_INIT_PRE_MEM) (
  IN OUT VOID *Policy OPTIONAL
  );

typedef
RETURN_STATUS
(EFIAPI *PEI_SILICON_POLICY_DONE_PRE_MEM) (
  IN VOID *Policy
  );

typedef
VOID *
(EFIAPI *PEI_SILICON_POLICY_INIT_POST_MEM) (
  IN OUT VOID *Policy OPTIONAL
  );

typedef
RETURN_STATUS
(EFIAPI *PEI_SILICON_POLICY_DONE_POST_MEM) (
  IN VOID *Policy
  );

typedef
VOID *
(EFIAPI *PEI_SILICON_POLICY_INIT_LATE) (
  IN OUT VOID *Policy OPTIONAL
  );

typedef
RETURN_STATUS
(EFIAPI *PEI_SILICON_POLICY_DONE_LATE) (
  IN VOID *Policy
  );

//
// PPI structure declaration
//
typedef struct {
  PEI_SILICON_POLICY_INIT_PRE_MEM       SiliconPolicyInitPreMem;
  PEI_SILICON_POLICY_DONE_PRE_MEM       SiliconPolicyDonePreMem;
  PEI_SILICON_POLICY_INIT_POST_MEM      SiliconPolicyInitPostMem;
  PEI_SILICON_POLICY_DONE_POST_MEM      SiliconPolicyDonePostMem;
} SILICON_POLICY_INIT_LIB_PPI;

//
// Protocol structure declaration
//
typedef struct {
  PEI_SILICON_POLICY_INIT_LATE         SiliconPolicyInitLate;
  PEI_SILICON_POLICY_DONE_LATE         SiliconPolicyDoneLate;
} SILICON_POLICY_INIT_LIB_PROTOCOL;

extern EFI_GUID gSiliconPolicyInitLibInterfaceGuid;

#endif // _SILICON_POLICY_INIT_LIB_INTERFACE_H_
