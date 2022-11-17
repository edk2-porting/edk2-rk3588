/** @file
 *
 *  Copyright (c) 2020, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2019-2020, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2016, Linaro Limited. All rights reserved.
 *  Copyright (c) 2011-2020, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <AsmMacroIoLibV8.h>
#include <Library/ArmLib.h>
#include <IndustryStandard/Bcm2836.h>
#include <IndustryStandard/RpiMbox.h>

    .macro  drain
    mov     x5, #RPI_MBOX_MAX_TRIES
0:  ldr     w6, [x4, #BCM2836_MBOX_STATUS_OFFSET]
    tbnz    w6, #BCM2836_MBOX_STATUS_EMPTY, 1f
    dmb     ld
    ldr     wzr, [x4, #BCM2836_MBOX_READ_OFFSET]
    subs    x5, x5, #1
    b.ne    0b
1:
    .endm

    .macro  poll, status
    mov     x5, #RPI_MBOX_MAX_TRIES
0:  ldr     w6, [x4, #BCM2836_MBOX_STATUS_OFFSET]
    tbz     w6, #\status, 1f
    dmb     ld
    subs    x5, x5, #1
    b.ne    0b
1:
    .endm

    .macro  run, command_buffer
    adr     x0, \command_buffer
    orr     x0, x0, #RPI_MBOX_VC_CHANNEL
    add     x0, x0, x1

    poll    BCM2836_MBOX_STATUS_FULL
    str     w0, [x4, #BCM2836_MBOX_WRITE_OFFSET]
    dmb     sy
    poll    BCM2836_MBOX_STATUS_EMPTY
    dmb     sy
    ldr     wzr, [x4, #BCM2836_MBOX_READ_OFFSET]
    dmb     ld
    .endm

ASM_FUNC (ArmPlatformPeiBootAction)
    mov     x1, #FixedPcdGet64 (PcdDmaDeviceOffset)
    orr     x0, x0, #RPI_MBOX_VC_CHANNEL
    // x1 holds the value of PcdDmaDeviceOffset throughout this function

    MOV32   (x4, BCM2836_MBOX_BASE_ADDRESS)

    drain

    run     .Lmeminfo_buffer

    ldr     w0, .Lmembase
    adr     x2, mSystemMemoryBase
    str     x0, [x2]

    ldr     w0, .Lmemsize
    sub     x0, x0, #1
    adr     x2, mSystemMemoryEnd
    str     x0, [x2]

    run     .Lvcinfo_buffer

    ldr     w0, .Lvcbase
    adr     x2, mVideoCoreBase
    str     x0, [x2]

    ldr     w0, .Lvcsize
    adr     x2, mVideoCoreSize
    str     x0, [x2]

    run     .Lrevinfo_buffer

    ldr     w0, .Lrevision
    adr     x2, mBoardRevision
    str     w0, [x2]

    run     .Lclkinfo_buffer

    ldr     w0, .Lfrequency
    adr     x2, _gPcd_BinaryPatch_PcdSerialClockRate
    str     w0, [x2]

    ret

    .align  4
.Lmeminfo_buffer:
    .long   .Lmeminfo_size
    .long   0x0
    .long   RPI_MBOX_GET_ARM_MEMSIZE
    .long   8                           // buf size
    .long   0                           // input len
.Lmembase:
    .long   0                           // mem base
.Lmemsize:
    .long   0                           // mem size
    .long   0                           // end tag
    .set    .Lmeminfo_size, . - .Lmeminfo_buffer

    .align  4
.Lvcinfo_buffer:
    .long   .Lvcinfo_size
    .long   0x0
    .long   RPI_MBOX_GET_VC_MEMSIZE
    .long   8                           // buf size
    .long   0                           // input len
.Lvcbase:
    .long   0                           // videocore base
.Lvcsize:
    .long   0                           // videocore size
    .long   0                           // end tag
    .set    .Lvcinfo_size, . - .Lvcinfo_buffer

    .align  4
.Lrevinfo_buffer:
    .long   .Lrevinfo_size
    .long   0x0
    .long   RPI_MBOX_GET_BOARD_REVISION
    .long   4                           // buf size
    .long   0                           // input len
.Lrevision:
    .long   0                           // revision
    .long   0                           // end tag
    .set    .Lrevinfo_size, . - .Lrevinfo_buffer

    .align  4
.Lclkinfo_buffer:
    .long   .Lclkinfo_size
    .long   0x0
    .long   RPI_MBOX_GET_CLOCK_RATE
    .long   8                           // buf size
    .long   4                           // input len
    .long   4                           // clock id: 0x04 = Core/VPU
.Lfrequency:
    .long   0                           // frequency
    .long   0                           // end tag
    .set    .Lclkinfo_size, . - .Lclkinfo_buffer

//UINTN
//ArmPlatformGetPrimaryCoreMpId (
//  VOID
//  );
ASM_FUNC (ArmPlatformGetPrimaryCoreMpId)
    MOV32  (w0, FixedPcdGet32 (PcdArmPrimaryCore))
    ret

//UINTN
//ArmPlatformIsPrimaryCore (
//  IN UINTN MpId
//  );
ASM_FUNC (ArmPlatformIsPrimaryCore)
    mov   x0, #1
    ret

//UINTN
//ArmPlatformGetCorePosition (
//  IN UINTN MpId
//  );
// With this function: CorePos = (ClusterId * 4) + CoreId
ASM_FUNC (ArmPlatformGetCorePosition)
    and   x1, x0, #ARM_CORE_MASK
    and   x0, x0, #ARM_CLUSTER_MASK
    add   x0, x1, x0, LSR #6
    ret

ASM_FUNCTION_REMOVE_IF_UNREFERENCED
