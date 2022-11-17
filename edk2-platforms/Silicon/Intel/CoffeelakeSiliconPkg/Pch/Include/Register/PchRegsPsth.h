/** @file
  Register definition for PSTH component

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
    Three letter acronym of the generation is used .
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_H_" in component name.
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

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_PSTH_H_
#define _PCH_REGS_PSTH_H_

//
// Private chipset register (Memory space) offset definition
// The PCR register defines is used for PCR MMIO programming and PCH SBI programming as well.
//

//
// PSTH and IO Trap PCRs (PID:PSTH)
//
#define R_PSTH_PCR_PSTHCTL          0x1D00                ///< PSTH control register
#define B_PSTH_PCR_PSTHIOSFPTCGE    BIT2                  ///< PSTH IOSF primary trunk clock gating enable
#define B_PSTH_PCR_PSTHIOSFSTCGE    BIT1                  ///< PSTH IOSF sideband trunk clock gating enable
#define B_PSTH_PCR_PSTHDCGE         BIT0                  ///< PSTH dynamic clock gating enable
#define R_PSTH_PCR_TRPST            0x1E00                ///< Trap status register
#define B_PSTH_PCR_TRPST_CTSS       0x0000000F            ///< Cycle Trap SMI# Status mask
#define R_PSTH_PCR_TRPC             0x1E10                ///< Trapped cycle
#define B_PSTH_PCR_TRPC_RW          BIT24                 ///< Read/Write#: 1=Read, 0=Write
#define B_PSTH_PCR_TRPC_AHBE        0x00000000000F0000    ///< Active high byte enables
#define B_PSTH_PCR_TRPC_IOA         0x000000000000FFFC    ///< Trap cycle I/O address
#define R_PSTH_PCR_TRPD             0x1E18                ///< Trapped write data
#define B_PSTH_PCR_TRPD_IOD         0x00000000FFFFFFFF    ///< Trap cycle I/O data
#define R_PSTH_PCR_TRPREG0          0x1E80                ///< IO Tarp 0 register
#define R_PSTH_PCR_TRPREG1          0x1E88                ///< IO Tarp 1 register
#define R_PSTH_PCR_TRPREG2          0x1E90                ///< IO Tarp 2 register
#define R_PSTH_PCR_TRPREG3          0x1E98                ///< IO Tarp 3 register
#define B_PSTH_PCR_TRPREG_RWM       BIT17                 ///< 49 - 32 for 32 bit access, Read/Write mask
#define B_PSTH_PCR_TRPREG_RWIO      BIT16                 ///< 48 - 32 for 32 bit access, Read/Write#, 1=Read, 0=Write
#define N_PSTH_PCR_TRPREG_RWIO      16                    ///< 48 - 32 for 32 bit access, 16bit shift for Read/Write field
#define N_PSTH_PCR_TRPREG_BEM       36
#define B_PSTH_PCR_TRPREG_BEM       0x000000F000000000    ///< Byte enable mask
#define N_PSTH_PCR_TRPREG_BE        32
#define B_PSTH_PCR_TRPREG_BE        0x0000000F00000000    ///< Byte enable
#define B_PSTH_PCR_TRPREG_AM        0x0000000000FC0000    ///< IO Address mask
#define B_PSTH_PCR_TRPREG_AD        0x000000000000FFFC    ///< IO Address
#define B_PSTH_PCR_TRPREG_TSE       BIT0                  ///< Trap and SMI# Enable


#endif
