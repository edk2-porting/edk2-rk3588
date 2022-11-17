/** @file
  Register names for ITSS

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

#ifndef _PCH_REGS_ITSS_H_
#define _PCH_REGS_ITSS_H_

//
// ITSS PCRs (PID:ITSS)
//
#define R_ITSS_PCR_PIRQA_ROUT             0x3100          ///< PIRQA Routing Control register
#define R_ITSS_PCR_PIRQB_ROUT             0x3101          ///< PIRQB Routing Control register
#define R_ITSS_PCR_PIRQC_ROUT             0x3102          ///< PIRQC Routing Control register
#define R_ITSS_PCR_PIRQD_ROUT             0x3103          ///< PIRQD Routing Control register
#define R_ITSS_PCR_PIRQE_ROUT             0x3104          ///< PIRQE Routing Control register
#define R_ITSS_PCR_PIRQF_ROUT             0x3105          ///< PIRQF Routing Control register
#define R_ITSS_PCR_PIRQG_ROUT             0x3106          ///< PIRQG Routing Control register
#define R_ITSS_PCR_PIRQH_ROUT             0x3107          ///< PIRQH Routing Control register
#define B_ITSS_PCR_PIRQX_ROUT_REN         0x80            ///< Interrupt Routing Enable
#define B_ITSS_PCR_PIRQX_ROUT_IR          0x0F            ///< IRQ Routng
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_3       0x03            ///< Route PIRQx to IRQ3
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_4       0x04            ///< Route PIRQx to IRQ4
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_5       0x05            ///< Route PIRQx to IRQ5
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_6       0x06            ///< Route PIRQx to IRQ6
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_7       0x07            ///< Route PIRQx to IRQ7
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_9       0x09            ///< Route PIRQx to IRQ9
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_10      0x0A            ///< Route PIRQx to IRQ10
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_11      0x0B            ///< Route PIRQx to IRQ11
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_12      0x0C            ///< Route PIRQx to IRQ12
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_14      0x0E            ///< Route PIRQx to IRQ14
#define V_ITSS_PCR_PIRQX_ROUT_IRQ_15      0x0F            ///< Route PIRQx to IRQ15

#define R_ITSS_PCR_PIR0                   0x3140          ///< PCI Interrupt Route 0
#define R_ITSS_PCR_PIR1                   0x3142          ///< PCI Interrupt Route 1
#define R_ITSS_PCR_PIR2                   0x3144          ///< PCI Interrupt Route 2
#define R_ITSS_PCR_PIR3                   0x3146          ///< PCI Interrupt Route 3
#define R_ITSS_PCR_PIR4                   0x3148          ///< PCI Interrupt Route 4
#define R_ITSS_PCR_PIR5                   0x314A          ///< PCI Interrupt Route 5
#define R_ITSS_PCR_PIR6                   0x314C          ///< PCI Interrupt Route 6
#define R_ITSS_PCR_PIR7                   0x314E          ///< PCI Interrupt Route 7
#define R_ITSS_PCR_PIR8                   0x3150          ///< PCI Interrupt Route 8
#define R_ITSS_PCR_PIR9                   0x3152          ///< PCI Interrupt Route 9
#define R_ITSS_PCR_PIR10                  0x3154          ///< PCI Interrupt Route 10
#define R_ITSS_PCR_PIR11                  0x3156          ///< PCI Interrupt Route 11
#define R_ITSS_PCR_PIR12                  0x3158          ///< PCI Interrupt Route 12

#define R_ITSS_PCR_GIC                    0x31FC          ///< General Interrupt Control
#define B_ITSS_PCR_GIC_MAX_IRQ_24         BIT9            ///< Max IRQ entry size, 1 = 24 entry size, 0 = 120 entry size
#define B_ITSS_PCR_GIC_AME                BIT17           ///< Alternate Access Mode Enable
#define B_ITSS_PCR_GIC_SPS                BIT16           ///< Shutdown Policy Select
#define R_ITSS_PCR_IPC0                   0x3200          ///< Interrupt Polarity Control 0
#define R_ITSS_PCR_IPC1                   0x3204          ///< Interrupt Polarity Control 1
#define R_ITSS_PCR_IPC2                   0x3208          ///< Interrupt Polarity Control 2
#define R_ITSS_PCR_IPC3                   0x320C          ///< Interrupt Polarity Control 3
#define R_ITSS_PCR_ITSSPRC                0x3300          ///< ITSS Power Reduction Control
#define B_ITSS_PCR_ITSSPRC_PGCBDCGE       BIT4            ///< PGCB Dynamic Clock Gating Enable
#define B_ITSS_PCR_ITSSPRC_HPETDCGE       BIT3            ///< HPET Dynamic Clock Gating Enable
#define B_ITSS_PCR_ITSSPRC_8254CGE        BIT2            ///< 8254 Static Clock Gating Enable
#define B_ITSS_PCR_ITSSPRC_IOSFICGE       BIT1            ///< IOSF-Sideband Interface Clock Gating Enable
#define B_ITSS_PCR_ITSSPRC_ITSSCGE        BIT0            ///< ITSS Clock Gate Enable
#define R_ITSS_PCR_NMI                    0x3330          ///< NMI Control
#define N_ITSS_PCR_NMI_NMI2SMI_STS        3               ///< NMI2SMI Status
#define N_ITSS_PCR_NMI_NMI2SMI_EN         2               ///< NMI2SMI Enable
#define B_ITSS_PCR_NMI_NMI2SMI_EN         BIT2            ///< NMI2SMI Enable
#define B_ITSS_PCR_NMI_NMI_NOW_STS        BIT1            ///< NMI_NOW_STS
#define B_ITSS_PCR_NMI_NMI_NOW            BIT0            ///< NMI_NOW
#define R_ITSS_PCR_MMC                    0x3334          ///< Master Message Control
#define B_ITSS_PCR_MMC_MSTRMSG_EN         BIT0            ///< Master Message Enable

#endif
