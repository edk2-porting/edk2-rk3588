/** @file
 *
 *  Copyright (c) 2013-2014, ARM Limited. All rights reserved.
 *  Copyright (c) 2017, Linaro Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 */

#include <AsmMacroIoLibV8.h>
#include <Library/ArmLib.h>

.text
.align 3

GCC_ASM_EXPORT(ArmPlatformPeiBootAction)
GCC_ASM_EXPORT(ArmPlatformGetCorePosition)
GCC_ASM_EXPORT(ArmPlatformGetPrimaryCoreMpId)
GCC_ASM_EXPORT(ArmPlatformIsPrimaryCore)

//
// First platform specific function to be called in the PEI phase
//
// This function is actually the first function called by the PrePi
// or PrePeiCore modules. It allows to retrieve arguments passed to
// the UEFI firmware through the CPU registers.
//
ASM_PFX(ArmPlatformPeiBootAction):
  ret

//
// Return the core position from the value of its MpId register
//
// This function returns the core position from the position 0 in the processor.
// This function might be called from assembler before any stack is set.
//
// @return   Return the core position
//
//UINTN
//ArmPlatformGetCorePosition (
//  IN UINTN MpId
//  );
// With this function: CorePos = (ClusterId * 2) + CoreId
ASM_PFX(ArmPlatformGetCorePosition):
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
ASM_PFX(ArmPlatformGetPrimaryCoreMpId):
  mov   w0, FixedPcdGet32 (PcdArmPrimaryCore)
  ret

//
// Return a non-zero value if the callee is the primary core
//
// This function returns a non-zero value if the callee is the primary core.
// The primary core is the core responsible to initialize the hardware and run UEFI.
// This function might be called from assembler before any stack is set.
//
//  @return   Return a non-zero value if the callee is the primary core.
//
//UINTN
//ArmPlatformIsPrimaryCore (
//  IN UINTN MpId
//  );
ASM_PFX(ArmPlatformIsPrimaryCore):
  mov   w0, #1
  ret
