/** @file
  Register names for PCIE standard register

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCIE_REGS_H_
#define _PCIE_REGS_H_

#include <IndustryStandard/Pci30.h>

//
// PCI type 0 Header
//
#define R_PCI_BCC_OFFSET                          0x0B

//
// PCI type 1 Header
//
#define R_PCI_BRIDGE_BNUM                         0x18 ///< Bus Number Register
#define B_PCI_BRIDGE_BNUM_SBBN                    0x00FF0000 ///< Subordinate Bus Number
#define B_PCI_BRIDGE_BNUM_SCBN                    0x0000FF00 ///< Secondary Bus Number

//
// PCI Express Capability List Register (CAPID:10h)
//
#define R_PCIE_XCAP_OFFSET                        0x02 ///< PCI Express Capabilities Register (Offset 02h)
#define B_PCIE_XCAP_DT                            (BIT7 | BIT6 | BIT5 | BIT4) ///< Device/Port Type
#define N_PCIE_XCAP_DT                            4

#define R_PCIE_DCAP_OFFSET                        0x04 ///< Device Capabilities Register (Offset 04h)
#define B_PCIE_DCAP_RBER                          BIT15 ///< Role-Based Error Reporting
#define B_PCIE_DCAP_E1AL                          (BIT11 | BIT10 | BIT9) ///< Endpoint L1 Acceptable Latency
#define N_PCIE_DCAP_E1AL                          9
#define B_PCIE_DCAP_E0AL                          (BIT8 | BIT7 | BIT6) ///< Endpoint L0s Acceptable Latency
#define N_PCIE_DCAP_E0AL                          6
#define B_PCIE_DCAP_MPS                           (BIT2 | BIT1 | BIT0) ///< Max_Payload_Size Supported

#define R_PCIE_DCTL_OFFSET                        0x08 ///< Device Control Register (Offset 08h)
#define B_PCIE_DCTL_MPS                           (BIT7 | BIT6 | BIT5) ///< Max_Payload_Size
#define N_PCIE_DCTL_MPS                           5

#define R_PCIE_LCAP_OFFSET                        0x0C ///< Link Capabilities Register (Offset 0Ch)
#define B_PCIE_LCAP_CPM                           BIT18 ///< Clock Power Management
#define B_PCIE_LCAP_EL1                           (BIT17 | BIT16 | BIT15) ///< L1 Exit Latency
#define N_PCIE_LCAP_EL1                           15
#define B_PCIE_LCAP_EL0                           (BIT14 | BIT13 | BIT12) ///< L0s Exit Latency
#define N_PCIE_LCAP_EL0                           12
#define B_PCIE_LCAP_APMS_L0S                      BIT10
#define B_PCIE_LCAP_APMS_L1                       BIT11
#define B_PCIE_LCAP_MLS                           (BIT3 | BIT2 | BIT1 | BIT0) ///< Max Link Speed
#define V_PCIE_LCAP_MLS_GEN3                      3
#define V_PCIE_LCAP_MLS_GEN4                      4

#define R_PCIE_LCTL_OFFSET                        0x10 ///< Link Control Register (Offset 10h)
#define B_PCIE_LCTL_ECPM                          BIT8 ///< Enable Clock Power Management
#define B_PCIE_LCTL_CCC                           BIT6 ///< Common Clock Configuration
#define B_PCIE_LCTL_RL                            BIT5 ///< Retrain Link
#define B_PCIE_LCTL_ASPM                          (BIT1 | BIT0) ///< Active State Power Management (ASPM) Control
#define V_PCIE_LCTL_ASPM_L0S                      1
#define V_PCIE_LCTL_ASPM_L1                       2
#define V_PCIE_LCTL_ASPM_L0S_L1                   3

#define R_PCIE_LSTS_OFFSET                        0x12 ///< Link Status Register (Offset 12h)
#define B_PCIE_LSTS_LA                            BIT13 ///< Data Link Layer Link Active
#define B_PCIE_LSTS_SCC                           BIT12 ///< Slot Clock Configuration
#define B_PCIE_LSTS_LT                            BIT11 ///< Link Training
#define B_PCIE_LSTS_NLW                           0x03F0 ///< Negotiated Link Width
#define N_PCIE_LSTS_NLW                           4
#define B_PCIE_LSTS_CLS                           0x000F ///< Current Link Speed

#define R_PCIE_SLCAP_OFFSET                       0x14 ///< Slot Capabilities Register (Offset 14h)
#define B_PCIE_SLCAP_HPC                          BIT6       ///< Hot-Plug Capable

#define R_PCIE_SLSTS_OFFSET                       0x1A ///< Slot Status Register (Offset 1Ah)
#define B_PCIE_SLSTS_PDS                          BIT6 ///< Presence Detect State

#define R_PCIE_DCAP2_OFFSET                       0x24 ///< Device Capabilities 2 Register (Offset 24h)
#define B_PCIE_DCAP2_LTRMS                        BIT11 ///< LTR Mechanism Supported

#define R_PCIE_DCTL2_OFFSET                       0x28 ///< Device Control 2 Register (Offset 28h)
#define B_PCIE_DCTL2_LTREN                        BIT10 ///< LTR Mechanism Enable

#define B_PCIE_LCTL2_TLS                          (BIT3 | BIT2 | BIT1 | BIT0) ///< Target Link Speed

//
// Latency Tolerance Reporting Extended Capability Registers (CAPID:0018h)
//
#define R_PCIE_LTRECH_CID                         0x0018

#define R_PCIE_LTRECH_MSLR_OFFSET                 0x04
#define N_PCIE_LTRECH_MSLR_VALUE                  0
#define N_PCIE_LTRECH_MSLR_SCALE                  10

#define R_PCIE_LTRECH_MNSLR_OFFSET                0x06
#define N_PCIE_LTRECH_MNSLR_VALUE                 0
#define N_PCIE_LTRECH_MNSLR_SCALE                 10

//
// Secondary PCI Express Extended Capability Header (CAPID:0019h)
//
#define R_PCIE_EX_LCTL3_OFFSET                    0x04 ///< Link Control 3 Register
#define B_PCIE_EX_LCTL3_PE                        BIT0 ///< Perform Equalization

//
// L1 Sub-States Extended Capability Register (CAPID:001Eh)
//
#define V_PCIE_EX_L1S_CID                        0x001E ///< Capability ID
#define R_PCIE_EX_L1SCAP_OFFSET                  0x04 ///< L1 Sub-States Capabilities
#define B_PCIE_EX_L1SCAP_PTV                     0x00F80000 //< Port Tpower_on value
#define N_PCIE_EX_L1SCAP_PTV                     19
#define B_PCIE_EX_L1SCAP_PTPOS                   0x00030000 //< Port Tpower_on scale
#define N_PCIE_EX_L1SCAP_PTPOS                   16
#define B_PCIE_EX_L1SCAP_CMRT                    0x0000FF00 //< Common Mode Restore time
#define N_PCIE_EX_L1SCAP_CMRT                    8
#define B_PCIE_EX_L1SCAP_L1PSS                   BIT4 ///< L1 PM substates supported
#define B_PCIE_EX_L1SCAP_AL1SS                   BIT3 ///< ASPM L1.1 supported
#define B_PCIE_EX_L1SCAP_AL12S                   BIT2 ///< ASPM L1.2 supported
#define B_PCIE_EX_L1SCAP_PPL11S                  BIT1 ///< PCI-PM L1.1 supported
#define B_PCIE_EX_L1SCAP_PPL12S                  BIT0 ///< PCI-PM L1.2 supported
#define R_PCIE_EX_L1SCTL1_OFFSET                 0x08 ///< L1 Sub-States Control 1
#define B_PCIE_EX_L1SCTL1_L1SSEIE                BIT4
#define N_PCIE_EX_L1SCTL1_L12LTRTLSV             29
#define N_PCIE_EX_L1SCTL1_L12LTRTLV              16
#define R_PCIE_EX_L1SCTL2_OFFSET                 0x0C ///< L1 Sub-States Control 2
#define N_PCIE_EX_L1SCTL2_POWT                   3

//
// PTM Extended Capability Register (CAPID:001Fh)
//
#define V_PCIE_EX_PTM_CID                         0x001F ///< Capability ID
#define R_PCIE_EX_PTMCAP_OFFSET                   0x04 ///< PTM Capabilities
#define R_PCIE_EX_PTMCTL_OFFSET                   0x08 ///< PTM Control Register

//
// Base Address Offset
//
#define B_PCI_BAR_MEMORY_TYPE_MASK                (BIT1 | BIT2)
#define B_PCI_BAR_MEMORY_TYPE_64                  BIT2

//
// PCI Express Extended Capability Header
//
#define R_PCIE_CFG_EXCAP_OFFSET                   0x100

#endif
