;; @file
;  This file provides assembly 64-bit atomic reads/writes required for memory initialization.
;
; Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
;;


SECTION .TEXT

;-----------------------------------------------------------------------------
;
;  Section:     SaMmioRead64
;
;  Description: Read 64 bits from the Memory Mapped I/O space.
;  Use MMX instruction for atomic access, because some MC registers have side effect.
;
;  @param[in] Address - Memory mapped I/O address.
;
;-----------------------------------------------------------------------------

;UINT64
;SaMmioRead64 (
;  IN  UINTN Address
;  )

global ASM_PFX(SaMmioRead64)
ASM_PFX(SaMmioRead64):
   sub     esp, 16
   movq    [esp], mm0         ;Save mm0 on stack
   mov     edx,  [esp + 20]   ;edx = Address
   movq    mm0, [edx]         ;mm0 = [Address]
   movq    [esp + 8], mm0     ;Store mm0 on Stack
   movq    mm0, [esp]         ;Restore mm0
   emms
   mov     eax, [esp + 8]     ;eax = [Address][31:0]
   mov     edx, [esp + 12]    ;edx = [Address][64:32]
   add     esp, 16
   ret

;-----------------------------------------------------------------------------
;
;  Section:     SaMmioWrite64
;
;  Description: Write 64 bits to the Memory Mapped I/O space.
;  Use MMX instruction for atomic access, because some MC registers have side effect.
;
;  @param[in] Address - Memory mapped I/O address.
;  @param[in] Value   - The value to write.
;
;-----------------------------------------------------------------------------

;UINT64
;SaMmioWrite64 (
;  IN UINTN Address,
;  IN UINT64 Value
;  )

global ASM_PFX(SaMmioWrite64)
ASM_PFX(SaMmioWrite64):
   sub     esp, 8
   movq    [esp], mm0          ;Save mm0 on Stack
   mov     edx, [esp + 12]     ;edx = Address
   movq    mm0, [esp + 16]     ;mm0 = Value
   movq    [edx], mm0          ;[Address] = Value
   movq    mm0, [esp]          ;Restore mm0
   emms
   mov     eax, [esp + 16]     ;eax = Value[31:0]
   mov     edx, [esp + 20]     ;edx = Value[64:32]
   add     esp, 8
   ret

