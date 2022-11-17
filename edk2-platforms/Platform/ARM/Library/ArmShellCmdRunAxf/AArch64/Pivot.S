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
  // Preserve ElfEntry() and its arguments
  // Since we will not be returning from this function, we can clobber
  // callee preserved register instead.
  mov   x19, x0
  mov   x20, x1
  mov   x21, x2
  mov   x22, x3
  mov   x23, x4

  bl    ArmDisableDataCache
  bl    ArmDisableMmu

  // Load ElfEntry()'s arguments into x0...x3
  mov   x0, x20
  mov   x1, x21
  mov   x2, x22
  mov   x3, x23

  // Call ElfEntry()
  blr   x19

0:wfi
  wfe
  b     0b
