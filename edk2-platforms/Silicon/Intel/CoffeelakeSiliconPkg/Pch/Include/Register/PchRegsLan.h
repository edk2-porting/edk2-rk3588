/** @file
  Register names for PCH LAN device

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

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_LAN_H_
#define _PCH_REGS_LAN_H_

//
// Gigabit LAN Controller configuration registers (D31:F6)
//
#define PCI_DEVICE_NUMBER_PCH_LAN     31
#define PCI_FUNCTION_NUMBER_PCH_LAN   6

#define R_LAN_CFG_MBARA               0x10
#define N_LAN_CFG_MBARA_ALIGN         17
#define R_LAN_CFG_PMCS                0xCC
#define B_LAN_CFG_PMCS_PS             (BIT1 | BIT0)
#define V_LAN_CFG_PMCS_PS0            0x00
#define R_LAN_MEM_CSR_CTRL                    0
#define B_LAN_MEM_CSR_CTRL_LANPHYPC_OVERRIDE  BIT16
#define B_LAN_MEM_CSR_CTRL_LANPHYPC_VAL       BIT17
#define R_LAN_MEM_CSR_CTRL_EXT             0x0018
#define B_LAN_MEM_CSR_CTRL_EXT_LPCD        BIT2
#define B_LAN_MEM_CSR_CTRL_EXT_FORCE_SMB   BIT11
#define R_LAN_MEM_CSR_MDIC                 0x0020
#define B_LAN_MEM_CSR_MDIC_RB              BIT28
#define B_LAN_MEM_CSR_MDIC_DATA            0xFFFF
#define R_LAN_MEM_CSR_EXTCNF_CTRL          0x0F00
#define B_LAN_MEM_CSR_EXTCNF_CTRL_SWFLAG   BIT5
#define R_LAN_MEM_CSR_RAL                  0x5400
#define R_LAN_MEM_CSR_RAH                  0x5404
#define B_LAN_MEM_CSR_RAH_RAH              0x0000FFFF
#define R_LAN_MEM_CSR_WUC                  0x5800
#define B_LAN_MEM_CSR_WUC_APME             BIT0

#endif

