//
//  Copyright (c) 2020, ARM Limited. All rights reserved.
//
//  SPDX-License-Identifier: BSD-2-Clause-Patent
//

#include <AsmMacroIoLibV8.h>

// VOID
// RunAxfPivot (
//   IN  ELF_ENTRYPOINT  ElfEntry
//   IN  UINTN           Arg0,
//   IN  UINTN           Arg1,
//   IN  UINTN           Arg2,
//   IN  UINTN           Arg3
//   );
ASM_FUNC(RunAxfPivot)
  // Preserve ElfEntry() and its arguments without using the stack.
  // Since we will not be returning from this function, we can clobber
  // callee preserved register instead.
  mov   r4, r0
  mov   r5, r1
  mov   r6, r2
  mov   r7, r3
  pop   {r8}

  bl    ArmDisableDataCache
  bl    ArmDisableMmu

  // Load ElfEntry()'s arguments into x0...x3
  mov   r0, r5
  mov   r1, r6
  mov   r2, r7
  mov   r3, r8

  // Call ElfEntry()
  blx   r4

0:wfi
  wfe
  b     0b
