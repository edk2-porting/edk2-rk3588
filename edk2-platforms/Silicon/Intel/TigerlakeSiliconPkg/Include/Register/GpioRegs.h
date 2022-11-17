/** @file
  Register names for PCH GPIO

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
#ifndef _GPIO_REGS_H_
#define _GPIO_REGS_H_

//
// PADCFG register is split into multiple DW registers
// S_GPIO_PCR_PADCFG refers to number of bytes used by all those registers for one pad
//
#define S_GPIO_PCR_PADCFG               0x10

//
// Pad Configuration Register DW0
//

//Pad Reset Config
#define B_GPIO_PCR_RST_CONF             (BIT31 | BIT30)
#define N_GPIO_PCR_RST_CONF             30
#define V_GPIO_PCR_RST_CONF_POW_GOOD    0x00
#define V_GPIO_PCR_RST_CONF_DEEP_RST    0x01
#define V_GPIO_PCR_RST_CONF_GPIO_RST    0x02
#define V_GPIO_PCR_RST_CONF_RESUME_RST  0x03  // Only for GPD Group

//RX Raw Override to 1
#define B_GPIO_PCR_RX_RAW1              BIT28
#define N_GPIO_PCR_RX_RAW1              28

//RX Level/Edge Configuration
#define B_GPIO_PCR_RX_LVL_EDG           (BIT26 | BIT25)
#define N_GPIO_PCR_RX_LVL_EDG           25

//RX Invert
#define B_GPIO_PCR_RXINV                BIT23
#define N_GPIO_PCR_RXINV                23

//GPIO Input Route IOxAPIC
#define B_GPIO_PCR_RX_APIC_ROUTE        BIT20

//GPIO Input Route SCI
#define B_GPIO_PCR_RX_SCI_ROUTE         BIT19

//GPIO Input Route SMI
#define B_GPIO_PCR_RX_SMI_ROUTE         BIT18

//GPIO Input Route NMI
#define B_GPIO_PCR_RX_NMI_ROUTE         BIT17
#define N_GPIO_PCR_RX_NMI_ROUTE         17

//GPIO Pad Mode
#define B_GPIO_PCR_PAD_MODE             (BIT12 | BIT11 | BIT10)
#define N_GPIO_PCR_PAD_MODE             10

//GPIO RX Disable
#define B_GPIO_PCR_RXDIS                BIT9

//GPIO TX Disable
#define B_GPIO_PCR_TXDIS                BIT8
#define N_GPIO_PCR_TXDIS                8

//GPIO RX State
#define B_GPIO_PCR_RX_STATE             BIT1
#define N_GPIO_PCR_RX_STATE             1

//GPIO TX State
#define B_GPIO_PCR_TX_STATE             BIT0
#define N_GPIO_PCR_TX_STATE             0

//Termination
#define B_GPIO_PCR_TERM                (BIT13 | BIT12 | BIT11 | BIT10)
#define N_GPIO_PCR_TERM                 10

//Interrupt number
#define B_GPIO_PCR_INTSEL               0x7F
#define N_GPIO_PCR_INTSEL               0

///
/// GPIO SMI data used for EFI_SMM_GPI_DISPATCH2_PROTOCOL
/// Below defines are to be used internally by PCH SMI dispatcher only
///
#define PCH_GPIO_NUM_SUPPORTED_GPIS       512
#define S_GPIO_PCR_GP_SMI_STS               4

///
/// Groups mapped to 2-tier General Purpose Event will all be under
/// one master GPE_111 (0x6F)
///
#define PCH_GPIO_2_TIER_MASTER_GPE_NUMBER  0x6F

#endif // _GPIO_REGS_H_
