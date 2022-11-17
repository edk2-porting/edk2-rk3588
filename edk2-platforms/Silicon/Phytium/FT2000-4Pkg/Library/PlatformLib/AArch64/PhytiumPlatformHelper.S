#
#  Copyright (c) 2011-2013, ARM Limited. All rights reserved.
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#

#include <AsmMacroIoLibV8.h>
#include <Base.h>
#include <Library/ArmLib.h>
#include <Library/PcdLib.h>
#include <AutoGen.h>

.text
.align 2

GCC_ASM_EXPORT(ArmPlatformPeiBootAction)
GCC_ASM_EXPORT(ArmPlatformIsPrimaryCore)
GCC_ASM_EXPORT(ArmPlatformGetPrimaryCoreMpId)
GCC_ASM_EXPORT(ArmPlatformGetCorePosition)

PrimaryCoreMpid:  .word    0x0


ASM_PFX(ArmPlatformPeiBootAction):
  // Save MPIDR_EL1[23:0] in a variable.
  mov   x20, x30
  bl    ASM_PFX(ArmReadMpidr)
  lsl   w0, w0, #8
  lsr   w0, w0, #8
  ldr   x1, =PrimaryCoreMpid
  str   w0, [x1]
  ret   x20

//UINTN
//ArmPlatformGetPrimaryCoreMpId (
//  VOID
//  );
ASM_PFX(ArmPlatformGetPrimaryCoreMpId):
  ldr   x0, =PrimaryCoreMpid
  ldr   w0, [x0]
  ret

//UINTN
//ArmPlatformIsPrimaryCore (
//  IN UINTN MpId
//  );
ASM_PFX(ArmPlatformIsPrimaryCore):
  mov   x20, x30
  bl    ASM_PFX(ArmReadMpidr)
  lsl   w0, w0, #8
  lsr   w0, w0, #8
  ldr   x1, =PrimaryCoreMpid
  ldr   w1, [x1]
  cmp   w0, w1
  cset  x0, eq
  ret   x20

//UINTN
//ArmPlatformGetCorePosition (
//  IN UINTN MpId
//  );
// With this function: CorePos = (ClusterId * 4) + CoreId
ASM_PFX(ArmPlatformGetCorePosition):
  and   x1, x0, #ARM_CORE_MASK
  and   x0, x0, #ARM_CLUSTER_MASK
  add   x0, x1, x0, LSR #6
  ret

ASM_FUNCTION_REMOVE_IF_UNREFERENCED
