; @file
; Copyright (c) 2006 - 2019 Intel Corporation. All rights reserved. <BR>
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;

#include <Base.h>

    SECTION .text

extern ASM_PFX(SecCoreStartupWithStack)

;
; SecCore Entry Point
;
; Processor is in flat protected mode
;
; @param[in]  EAX   Initial value of the EAX register (BIST: Built-in Self Test)
; @param[in]  DI    'BP': boot-strap processor, or 'AP': application processor
; @param[in]  EBP   Pointer to the start of the Boot Firmware Volume
;
; @return     None  This routine does not return
;
global ASM_PFX(_ModuleEntryPoint)
ASM_PFX(_ModuleEntryPoint):

    ;
    ; Load temporary RAM stack based on PCDs
    ;
    %define SEC_TOP_OF_STACK (FixedPcdGet32 (PcdSimicsSecPeiTempRamBase) + \
                          FixedPcdGet32 (PcdSimicsSecPeiTempRamSize))
    mov     eax, SEC_TOP_OF_STACK
    mov     esp, eax
    nop

    ;
    ; Setup parameters and call SecCoreStartupWithStack
    ;   [esp]   return address for call
    ;   [esp+4] BootFirmwareVolumePtr
    ;   [esp+8] TopOfCurrentStack
    ;
    push    eax
    push    ebp
    call    ASM_PFX(SecCoreStartupWithStack)

