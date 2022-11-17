/** @file
*
*  Copyright (c) 2013-2014, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <AsmMacroIoLibV8.h>
#include <Library/ArmLib.h>

//UINTN
//ArmPlatformGetCorePosition (
//  IN UINTN MpId
//  );
// With this function: CorePos = (ClusterId * 2) + CoreId
ASM_FUNC(ArmPlatformGetCorePosition)
  and   x1, x0, #ARM_CORE_MASK
  and   x0, x0, #ARM_CLUSTER_MASK
  add   x0, x1, x0, LSR #7
  ret

//UINTN
//ArmPlatformGetPrimaryCoreMpId (
//  VOID
//  );
ASM_FUNC(ArmPlatformGetPrimaryCoreMpId)
  ldr   w0, PrimaryCoreMpid
  ret

//UINTN
//ArmPlatformIsPrimaryCore (
//  IN UINTN MpId
//  );
ASM_FUNC(ArmPlatformIsPrimaryCore)
  MOV32 (w1, FixedPcdGet32 (PcdArmPrimaryCoreMask))
  and   x0, x0, x1

  ldr   w1, PrimaryCoreMpid

  cmp   w0, w1
  cset  x0, eq
  ret

ASM_FUNC(ArmPlatformPeiBootAction)
  // The trusted firmware passes the primary CPU MPID through x0 register.
  // Save it in a variable.
  adr  x1, PrimaryCoreMpid
  str  w0, [x1]
  ret

PrimaryCoreMpid:  .word    0x0
