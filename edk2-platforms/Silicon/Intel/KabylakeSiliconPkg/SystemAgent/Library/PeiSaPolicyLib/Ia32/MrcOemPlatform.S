## @file
#
# Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

ASM_GLOBAL ASM_PFX(SaMmioRead64)
ASM_PFX(SaMmioRead64):
   subl    $16, %esp
   movq    %mm0, (%esp)       #Save mm0 on stack
   movl    20(%esp), %edx     #edx = Address
   movq    (%edx), %mm0       #mm0 = [Address]
   movq    %mm0, 8(%esp)      #Store mm0 on Stack
   movq    (%esp), %mm0       #Restore mm0
   emms
   movl    8(%esp), %eax      #eax = [Address][31:0]
   movl    12(%esp), %edx     #edx = [Address][64:32]
   addl    $16, %esp
   ret

#-----------------------------------------------------------------------------
#
#  Section:     SaMmioWrite64
#
#  Description: Write 64 bits to the Memory Mapped I/O space.
#  Use MMX instruction for atomic access, because some MC registers have side effect.
#
#  @param[in] Address - Memory mapped I/O address.
#  @param[in] Value   - The value to write.
#
#-----------------------------------------------------------------------------

#UINT64
#SaMmioWrite64 (
#  IN UINTN Address,
#  IN UINT64 Value
#  )

ASM_GLOBAL ASM_PFX(SaMmioWrite64)
ASM_PFX(SaMmioWrite64):
   subl    $8, %esp
   movq    %mm0, (%esp)       #Save mm0 on Stack
   movl    12(%esp), %edx     #edx = Address
   movq    16(%esp), %mm0     #mm0 = Value
   movq    %mm0, (%edx)       #[Address] = Value
   movq    (%esp), %mm0       #Restore mm0
   emms
   movl     16(%esp), %eax    #eax = Value[31:0]
   movl     20(%esp), %edx    #edx = Value[64:32]
   addl     $8, %esp
   ret
