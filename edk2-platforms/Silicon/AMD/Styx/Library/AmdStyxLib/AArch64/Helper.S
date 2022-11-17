#/**
#
#  Copyright (c) 2011-2013, ARM Limited. All rights reserved.<BR>
#  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
#**/
#/**
#  Derived from:
#   ArmPlatformPkg/ArmVExpressPkg/Library/ArmVExpressLibRTSM/RTSMHelper.S
#
#**/
#include <AsmMacroIoLibV8.h>
#include <Library/ArmLib.h>

PrimaryCoreMpid:  .word    0x0
PrimaryCoreBoot:  .word    0x0

//VOID
//ArmPlatformPeiBootAction (
//  VOID
//  );
ASM_FUNC(ArmPlatformPeiBootAction)
  ldr  w0, PrimaryCoreBoot
  cbnz w0, 1f

  // Save the primary CPU MPID
  mrs  x0, mpidr_el1
  adr  x2, PrimaryCoreMpid
  mov  w1, #1
  stp  w0, w1, [x2]
1:
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
  ldr   w1, PrimaryCoreMpid

  cmp   w0, w1
  cset  x0, eq
  ret

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

ASM_FUNCTION_REMOVE_IF_UNREFERENCED
