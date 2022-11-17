/** @file
  Register names for PCH PCI-E root port devices

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
#ifndef _PCH_REGS_PCIE_H_
#define _PCH_REGS_PCIE_H_

#define R_PCH_PCIE_CFG_CLIST                          0x40
#define R_PCH_PCIE_CFG_LCAP                           (R_PCH_PCIE_CFG_CLIST + R_PCIE_LCAP_OFFSET)
#define N_PCH_PCIE_CFG_LCAP_PN                        24
#define R_PCH_PCIE_CFG_LCTL                           (R_PCH_PCIE_CFG_CLIST + R_PCIE_LCTL_OFFSET)
#define R_PCH_PCIE_CFG_LSTS                           (R_PCH_PCIE_CFG_CLIST + R_PCIE_LSTS_OFFSET)
#define R_PCH_PCIE_CFG_SLCAP                          (R_PCH_PCIE_CFG_CLIST + R_PCIE_SLCAP_OFFSET)
#define R_PCH_PCIE_CFG_SLSTS                          (R_PCH_PCIE_CFG_CLIST + R_PCIE_SLSTS_OFFSET)

#define R_PCH_PCIE_CFG_MPC2                           0xD4
#define B_PCH_PCIE_CFG_MPC2_EOIFD                     BIT1

#define R_PCH_PCIE_CFG_MPC                            0xD8
#define S_PCH_PCIE_CFG_MPC                            4
#define B_PCH_PCIE_CFG_MPC_PMCE                       BIT31
#define B_PCH_PCIE_CFG_MPC_HPME                       BIT1
#define N_PCH_PCIE_CFG_MPC_HPME                       1

#define R_PCH_PCIE_CFG_SMSCS                          0xDC
#define S_PCH_PCIE_CFG_SMSCS                          4
#define B_PCH_PCIE_CFG_SMSCS_PMCS                     BIT31
#define N_PCH_PCIE_CFG_SMSCS_LERSMIS                  5
#define N_PCH_PCIE_CFG_SMSCS_HPLAS                    4
#define N_PCH_PCIE_CFG_SMSCS_HPPDM                    1

//CES.RE, CES.BT, CES.BD

#define R_PCH_PCIE_CFG_EX_SPEECH                      0xA30 ///< Secondary PCI Express Extended Capability Header
#define R_PCH_PCIE_CFG_EX_LCTL3                       (R_PCH_PCIE_CFG_EX_SPEECH + R_PCIE_EX_LCTL3_OFFSET)

#define R_PCH_PCIE_CFG_LTROVR                         0x400
#define B_PCH_PCIE_CFG_LTROVR_LTRNSROVR               BIT31 ///< LTR Non-Snoop Requirement Bit Override
#define B_PCH_PCIE_CFG_LTROVR_LTRSROVR                BIT15 ///< LTR Snoop Requirement Bit Override

#define R_PCH_PCIE_CFG_LTROVR2                        0x404
#define B_PCH_PCIE_CFG_LTROVR2_FORCE_OVERRIDE         BIT3 ///< LTR Force Override Enable
#define B_PCH_PCIE_CFG_LTROVR2_LOCK                   BIT2 ///< LTR Override Lock
#define B_PCH_PCIE_CFG_LTROVR2_LTRNSOVREN             BIT1 ///< LTR Non-Snoop Override Enable
#define B_PCH_PCIE_CFG_LTROVR2_LTRSOVREN              BIT0 ///< LTR Snoop Override Enable

#define R_PCH_PCIE_CFG_PCIEPMECTL                     0x420
#define B_PCH_PCIE_CFG_PCIEPMECTL_DLSULPPGE           BIT30
#define B_PCH_PCIE_CFG_PCIEPMECTL_L1LE                BIT17
#define B_PCH_PCIE_CFG_PCIEPMECTL_L1FSOE              BIT0

#define R_PCH_PCIE_CFG_EQCFG1                         0x450
#define S_PCH_PCIE_CFG_EQCFG1                         4
#define N_PCH_PCIE_CFG_EQCFG1_LERSMIE                 21

//
// PCIE PCRs (PID:SPA SPB SPC SPD SPE SPF)
//
#define R_SPX_PCR_PCD                         0                       ///< Port configuration and disable
#define B_SPX_PCR_PCD_RP1FN                   (BIT2 | BIT1 | BIT0)    ///< Port 1 Function Number
#define S_SPX_PCR_PCD_RP_FIELD                4                       ///< 4 bits for each RP FN

#endif
