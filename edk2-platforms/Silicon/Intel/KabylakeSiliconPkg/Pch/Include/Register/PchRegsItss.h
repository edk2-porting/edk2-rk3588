/** @file
  Register names for ITSS

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position
  - In general, PCH registers are denoted by "_PCH_" in register names
  - Registers / bits that are different between PCH generations are denoted by
    "_PCH_[generation_name]_" in register/bit names.
  - Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
    Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
    e.g., "_PCH_H_", "_PCH_LP_"
    Registers / bits names without _H_ or _LP_ apply for both H and LP.
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_PCH_" without [generation_name] inserted.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_REGS_ITSS_H_
#define _PCH_REGS_ITSS_H_

//
// ITSS PCRs (PID:ITSS)
//
#define R_PCH_PCR_ITSS_PIRQA_ROUT             0x3100          ///< PIRQA Routing Control register
#define R_PCH_PCR_ITSS_PIRQB_ROUT             0x3101          ///< PIRQB Routing Control register
#define R_PCH_PCR_ITSS_PIRQC_ROUT             0x3102          ///< PIRQC Routing Control register
#define R_PCH_PCR_ITSS_PIRQD_ROUT             0x3103          ///< PIRQD Routing Control register
#define R_PCH_PCR_ITSS_PIRQE_ROUT             0x3104          ///< PIRQE Routing Control register
#define R_PCH_PCR_ITSS_PIRQF_ROUT             0x3105          ///< PIRQF Routing Control register
#define R_PCH_PCR_ITSS_PIRQG_ROUT             0x3106          ///< PIRQG Routing Control register
#define R_PCH_PCR_ITSS_PIRQH_ROUT             0x3107          ///< PIRQH Routing Control register
#define B_PCH_PCR_ITSS_PIRQX_ROUT_REN         0x80            ///< Interrupt Routing Enable
#define B_PCH_PCR_ITSS_PIRQX_ROUT_IR          0x0F            ///< IRQ Routng
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_3       0x03            ///< Route PIRQx to IRQ3
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_4       0x04            ///< Route PIRQx to IRQ4
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_5       0x05            ///< Route PIRQx to IRQ5
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_6       0x06            ///< Route PIRQx to IRQ6
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_7       0x07            ///< Route PIRQx to IRQ7
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_9       0x09            ///< Route PIRQx to IRQ9
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_10      0x0A            ///< Route PIRQx to IRQ10
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_11      0x0B            ///< Route PIRQx to IRQ11
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_12      0x0C            ///< Route PIRQx to IRQ12
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_14      0x0E            ///< Route PIRQx to IRQ14
#define V_PCH_PCR_ITSS_PIRQX_ROUT_IRQ_15      0x0F            ///< Route PIRQx to IRQ15

#define R_PCH_PCR_ITSS_PIR0                   0x3140          ///< PCI Interrupt Route 0
#define R_PCH_PCR_ITSS_PIR1                   0x3142          ///< PCI Interrupt Route 1
#define R_PCH_PCR_ITSS_PIR2                   0x3144          ///< PCI Interrupt Route 2
#define R_PCH_PCR_ITSS_PIR3                   0x3146          ///< PCI Interrupt Route 3
#define R_PCH_PCR_ITSS_PIR4                   0x3148          ///< PCI Interrupt Route 4
#define R_PCH_PCR_ITSS_PIR5                   0x314A          ///< PCI Interrupt Route 5
#define R_PCH_PCR_ITSS_PIR6                   0x314C          ///< PCI Interrupt Route 6
#define R_PCH_PCR_ITSS_PIR7                   0x314E          ///< PCI Interrupt Route 7
#define R_PCH_PCR_ITSS_PIR8                   0x3150          ///< PCI Interrupt Route 8
#define R_PCH_PCR_ITSS_PIR9                   0x3152          ///< PCI Interrupt Route 9
#define R_PCH_PCR_ITSS_PIR10                  0x3154          ///< PCI Interrupt Route 10
#define R_PCH_PCR_ITSS_PIR11                  0x3156          ///< PCI Interrupt Route 11
#define R_PCH_PCR_ITSS_PIR12                  0x3158          ///< PCI Interrupt Route 12

#define R_PCH_PCR_ITSS_GIC                    0x31FC          ///< General Interrupt Control
#define B_PCH_PCR_ITSS_GIC_MAX_IRQ_24         BIT9            ///< Max IRQ entry size, 1 = 24 entry size, 0 = 120 entry size
#define B_PCH_PCR_ITSS_GIC_AME                BIT17           ///< Alternate Access Mode Enable
#define B_PCH_PCR_ITSS_GIC_SPS                BIT16           ///< Shutdown Policy Select
#define R_PCH_PCR_ITSS_IPC0                   0x3200          ///< Interrupt Polarity Control 0
#define R_PCH_PCR_ITSS_IPC1                   0x3204          ///< Interrupt Polarity Control 1
#define R_PCH_PCR_ITSS_IPC2                   0x3208          ///< Interrupt Polarity Control 2
#define R_PCH_PCR_ITSS_IPC3                   0x320C          ///< Interrupt Polarity Control 3
#define R_PCH_PCR_ITSS_ITSSPRC                0x3300          ///< ITSS Power Reduction Control
#define B_PCH_PCR_ITSS_ITSSPRC_PGCBDCGE       BIT4            ///< PGCB Dynamic Clock Gating Enable
#define B_PCH_PCR_ITSS_ITSSPRC_HPETDCGE       BIT3            ///< HPET Dynamic Clock Gating Enable
#define B_PCH_PCR_ITSS_ITSSPRC_8254CGE        BIT2            ///< 8254 Static Clock Gating Enable
#define B_PCH_PCR_ITSS_ITSSPRC_IOSFICGE       BIT1            ///< IOSF-Sideband Interface Clock Gating Enable
#define B_PCH_PCR_ITSS_ITSSPRC_ITSSCGE        BIT0            ///< ITSS Clock Gate Enable

#define R_PCH_PCR_ITSS_MMC                    0x3334          ///< Master Message Control
#define B_PCH_PCR_ITSS_MMC_MSTRMSG_EN         BIT0            ///< Master Message Enable

#endif
