//------------------------------------------------------------------------------
//
// SiFive U5 Series Timer CSR functions.
//
// Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//
//------------------------------------------------------------------------------
#include <Base.h>
#include <RiscVImpl.h>
#include <U5Clint.h>

.data

.text
.align 3

.global ASM_PFX(RiscVReadMachineTimer)
.global ASM_PFX(RiscVSetMachineTimerCmp)
.global ASM_PFX(RiscVReadMachineTimerCmp)

//
// Read machine timer CSR.
// @retval a0 : 64-bit machine timer.
//
ASM_PFX (RiscVReadMachineTimer):
    li t1, CLINT_REG_MTIME
    ld a0, (t1)
    ret

//
// Set machine timer compare CSR.
// @param a0 : UINT64
//
ASM_PFX (RiscVSetMachineTimerCmp):
    li t1, CLINT_REG_MTIMECMP0
    sd a0, (t1)
    ret

//
// Read machine timer compare CSR.
// @param a0 : UINT64
//
ASM_PFX (RiscVReadMachineTimerCmp):
    li t1, CLINT_REG_MTIMECMP0
    ld a0, (t1)
    ret
