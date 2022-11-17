/** @file
  Register names for Flash registers

  Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _FLASH_REGS_H_
#define _FLASH_REGS_H_

//
// Flash Descriptor Base Address Region (FDBAR) from Flash Region 0
//
#define R_FLASH_FDBAR_FLASH_MAP0          0x04
#define B_FLASH_FDBAR_NC                  0x00000300                    ///< Number Of Components
#define N_FLASH_FDBAR_NC                  8                             ///< Number Of Components
#define R_FLASH_FDBAR_FLASH_MAP1          0x08
#define B_FLASH_FDBAR_FPSBA               0x00FF0000                    ///< PCH Strap Base Address, [23:16] represents [11:4]
#define N_FLASH_FDBAR_FPSBA               16                            ///< PCH Strap base Address bit position
#define N_FLASH_FDBAR_FPSBA_REPR          4                             ///< PCH Strap base Address bit represents position
#define B_FLASH_FDBAR_PCHSL               0xFF000000                    ///< PCH Strap Length, [31:24] represents number of Dwords
#define N_FLASH_FDBAR_PCHSL               24                            ///< PCH Strap Length bit position
#define R_FLASH_FDBAR_FLASH_MAP2          0x0C
#define B_FLASH_FDBAR_FCPUSBA             0x00000FFC                    ///< CPU Strap Base Address [11:2]
#define N_FLASH_FDBAR_FCPUSBA             2                             ///< CPU Strap Base Address bit position
#define B_FLASH_FDBAR_CPUSL               0x00FF0000                    ///< CPU Strap Length, [23:16] represents number of Dwords
#define N_FLASH_FDBAR_CPUSL               16                            ///< CPU Strap Length bit position

//
// Flash Component Base Address (FCBA) from Flash Region 0
//
#define R_FLASH_FCBA_FLCOMP               0x00                          ///< Flash Components Register
#define B_FLASH_FLCOMP_COMP1_MASK         0xF0                          ///< Flash Component 1 Size MASK
#define N_FLASH_FLCOMP_COMP1              4                             ///< Flash Component 1 Size bit position
#define B_FLASH_FLCOMP_COMP0_MASK         0x0F                          ///< Flash Component 0 Size MASK
#define V_FLASH_FLCOMP_COMP_512KB         0x80000
//
// Descriptor Upper Map Section from Flash Region 0
//
#define R_FLASH_UMAP1                     0xEFC                         ///< Flash Upper Map 1
#define B_FLASH_UMAP1_MDTBA               0xFF000000                    ///< MIP Descriptor Table Base Address
#define N_FLASH_UMAP1_MDTBA               24                            ///< MDTBA bits position
#define N_FLASH_UMAP1_MDTBA_REPR          4                             ///< MDTBA address representation position

#endif
