//Based on ArmPlatformPkg/Library/ArmPlatformLibNull/AArch64/ArmPlatformHelper.S
//
//  Copyright (c) 2012-2013, ARM Limited. All rights reserved.
//  Copyright (c) 2016, Marvell. All rights reserved.
//  Copyright (c) 2017, Linaro Limited. All rights reserved.
//
//  SPDX-License-Identifier: BSD-2-Clause-Patent
//
//  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
//  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED
//

#include <AsmMacroIoLib.h>
#include <Library/ArmLib.h>

#define CCU_MC_BASE                     0xF0001700
#define CCU_MC_RCR_OFFSET               0x0
#define CCU_MC_RCR_REMAP_EN             BIT0
#define CCU_MC_RCR_REMAP_SIZE(Size)     (((Size) - 1) ^ (SIZE_1MB - 1))

#define CCU_MC_RSBR_OFFSET              0x4
#define CCU_MC_RSBR_SOURCE_BASE(Base)   (((Base) >> 20) << 10)
#define CCU_MC_RTBR_OFFSET              0x8
#define CCU_MC_RTBR_TARGET_BASE(Base)   (((Base) >> 20) << 10)

ASM_FUNC(ArmPlatformPeiBootAction)
  .if   FixedPcdGet64 (PcdSystemMemoryBase) != 0
  .err  PcdSystemMemoryBase should be 0x0 on this platform!
  .endif

  bx    lr

//UINTN
//ArmPlatformGetCorePosition (
//  IN UINTN MpId
//  );
// With this function: CorePos = (ClusterId * 2) + CoreId
ASM_FUNC(ArmPlatformGetCorePosition)
  and   r1, r0, #ARM_CORE_MASK
  and   r0, r0, #ARM_CLUSTER_MASK
  add   r0, r1, r0, LSR #7
  bx    lr

//UINTN
//ArmPlatformGetPrimaryCoreMpId (
//  VOID
//  );
ASM_FUNC(ArmPlatformGetPrimaryCoreMpId)
  MOV32 (r0, FixedPcdGet32(PcdArmPrimaryCore))
  bx    lr

//UINTN
//ArmPlatformIsPrimaryCore (
//  IN UINTN MpId
//  );
ASM_FUNC(ArmPlatformIsPrimaryCore)
  MOV32 (r1, FixedPcdGet32(PcdArmPrimaryCoreMask))
  and   r0, r0, r1
  MOV32 (r1, FixedPcdGet32(PcdArmPrimaryCore))
  cmp   r0, r1
  moveq r0, #1
  movne r0, #0
  bx    lr
