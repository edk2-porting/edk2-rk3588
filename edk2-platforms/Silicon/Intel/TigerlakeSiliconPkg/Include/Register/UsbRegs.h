/** @file
  Register names for USB Host and device controller

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
#ifndef _USB_REGS_H_
#define _USB_REGS_H_

//
// XHCI PCI Config Space registers
//
#define R_XHCI_CFG_PWR_CNTL_STS             0x74
#define B_XHCI_CFG_PWR_CNTL_STS_PWR_STS     (BIT1 | BIT0)
#define V_XHCI_CFG_PWR_CNTL_STS_PWR_STS_D3  (BIT1 | BIT0)

//
// xDCI (OTG) MMIO registers
//
#define R_XDCI_MEM_GCTL                       0xC110  ///< Xdci Global Ctrl

#endif // _USB_REGS_H_

