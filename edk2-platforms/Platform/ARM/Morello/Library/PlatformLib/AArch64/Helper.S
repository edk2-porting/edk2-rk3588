/** @file

  Copyright (c) 2021, ARM Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <AsmMacroIoLibV8.h>
#include <Library/ArmLib.h>

.text
.align 3

//
// First platform specific function to be called in the PEI phase
//
// This function is actually the first function called by the PrePi
// or PrePeiCore modules. It allows to retrieve arguments passed to
// the UEFI firmware through the CPU registers.
//
ASM_FUNC(ArmPlatformPeiBootAction)
  ret

//
// Return the core position from the value of its MpId register
//
// This function returns core position from the position 0 in the processor.
// This function might be called from assembler before any stack is set.
//
// @return   Return the core position
//
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

//
// Return the MpId of the primary core
//
// This function returns the MpId of the primary core.
// This function might be called from assembler before any stack is set.
//
// @return   Return the MpId of the primary core
//
//UINTN
//ArmPlatformGetPrimaryCoreMpId (
//  VOID
//  );
ASM_FUNC(ArmPlatformGetPrimaryCoreMpId)
  MOV32 (w0, FixedPcdGet32 (PcdArmPrimaryCore))
  ret

//
// Return a non-zero value if the callee is the primary core
//
// This function returns a non-zero value if the callee is the primary core.
// Primary core is the core responsible to initialize hardware and run UEFI.
// This function might be called from assembler before any stack is set.
//
//  @return   Return a non-zero value if the callee is the primary core.
//
//UINTN
//ArmPlatformIsPrimaryCore (
//  IN UINTN MpId
//  );
ASM_FUNC(ArmPlatformIsPrimaryCore)
  MOV32 (w1, FixedPcdGet32 (PcdArmPrimaryCoreMask))
  and   x0, x0, x1
  MOV32 (w1, FixedPcdGet32 (PcdArmPrimaryCore))
  cmp   w0, w1
  cset  x0, eq
  ret
