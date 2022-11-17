/** @file
  Register names for SATA controllers

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
#ifndef _SATA_REGS_H_
#define _SATA_REGS_H_

//
//  SATA Controller Common Registers
//
#define R_SATA_CFG_AHCI_BAR                 0x24
#define R_SATA_CFG_MAP                      0x90
#define N_SATA_CFG_MAP_SPD                  16
#define R_SATA_CFG_PCS                      0x94
#define B_SATA_CFG_PCS_P0P                  BIT16
#define R_SATA_CFG_SATAGC                   0x9C
#define B_SATA_CFG_SATAGC_ASSEL             (BIT2 | BIT1 | BIT0)
#define V_SATA_CFG_SATAGC_ASSEL_2K          0x0
#define V_SATA_CFG_SATAGC_ASSEL_16K         0x1
#define V_SATA_CFG_SATAGC_ASSEL_32K         0x2
#define V_SATA_CFG_SATAGC_ASSEL_64K         0x3
#define V_SATA_CFG_SATAGC_ASSEL_128K        0x4
#define V_SATA_CFG_SATAGC_ASSEL_256K        0x5
#define V_SATA_CFG_SATAGC_ASSEL_512K        0x6

#endif
