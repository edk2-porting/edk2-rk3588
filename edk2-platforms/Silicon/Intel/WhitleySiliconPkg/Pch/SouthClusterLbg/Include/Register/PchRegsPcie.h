/** @file
  Register names for PCH PCI-E root port devices
  Conventions:
  Prefixes:
  Definitions beginning with "R_" are registers
  Definitions beginning with "B_" are bits within registers
  Definitions beginning with "V_" are meaningful values within the bits
  Definitions beginning with "S_" are register sizes
  Definitions beginning with "N_" are the bit position
  In general, PCH registers are denoted by "_PCH_" in register names
  Registers / bits that are different between PCH generations are denoted by
  _PCH_[generation_name]_" in register/bit names.
  Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
  Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
  e.g., "_PCH_H_", "_PCH_LP_"
  Registers / bits names without _H_ or _LP_ apply for both H and LP.
  Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
  at the end of the register/bit names
  Registers / bits of new devices introduced in a PCH generation will be just named
  as "_PCH_" without [generation_name] inserted.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_PCIE_H_
#define _PCH_REGS_PCIE_H_

#define PCI_INVALID_VALUE_16BIT                   0xFFFF
#define PCI_INVALID_VALUE_32BIT                   0xFFFFFFFF

//
// PCH PCI Express Root Ports (D28:F0~7 & D29:F0~3)
//
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1       28
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2       29
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3       27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS     28
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1  0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2  1
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3  2
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4  3
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5  4
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6  5
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_7  6
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8  7
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_9  0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_10 1
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_11 2
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_12 3
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_13 4
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_14 5
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_15 6
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_16 7
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_17 0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_18 1
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_19 2
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_20 3

#define V_PCH_PCIE_VENDOR_ID                      V_PCH_INTEL_VENDOR_ID

#define V_PCH_H_PCIE_DEVICE_ID_PORT1              0xA110  ///< PCI Express Root Port #1, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT2              0xA111  ///< PCI Express Root Port #2, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT3              0xA112  ///< PCI Express Root Port #3, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT4              0xA113  ///< PCI Express Root Port #4, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT5              0xA114  ///< PCI Express Root Port #5, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT6              0xA115  ///< PCI Express Root Port #6, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT7              0xA116  ///< PCI Express Root Port #7, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT8              0xA117  ///< PCI Express Root Port #8, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT9              0xA118  ///< PCI Express Root Port #9, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT10             0xA119  ///< PCI Express Root Port #10, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT11             0xA11A  ///< PCI Express Root Port #11, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT12             0xA11B  ///< PCI Express Root Port #12, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT13             0xA11C  ///< PCI Express Root Port #13, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT14             0xA11D  ///< PCI Express Root Port #14, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT15             0xA11E  ///< PCI Express Root Port #15, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT16             0xA11F  ///< PCI Express Root Port #16, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT17             0xA167  ///< PCI Express Root Port #17, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT18             0xA168  ///< PCI Express Root Port #18, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT19             0xA169  ///< PCI Express Root Port #19, SKL PCH H
#define V_PCH_H_PCIE_DEVICE_ID_PORT20             0xA16A  ///< PCI Express Root Port #20, SKL PCH H

#define V_PCH_LP_PCIE_DEVICE_ID_PORT1             0x9D10  ///< PCI Express Root Port #1, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT2             0x9D11  ///< PCI Express Root Port #2, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT3             0x9D12  ///< PCI Express Root Port #3, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT4             0x9D13  ///< PCI Express Root Port #4, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT5             0x9D14  ///< PCI Express Root Port #5, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT6             0x9D15  ///< PCI Express Root Port #6, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT7             0x9D16  ///< PCI Express Root Port #7, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT8             0x9D17  ///< PCI Express Root Port #8, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT9             0x9D18  ///< PCI Express Root Port #9, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT10            0x9D19  ///< PCI Express Root Port #10, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT11            0x9D1A  ///< PCI Express Root Port #11, SKL PCH LP PCIe Device ID
#define V_PCH_LP_PCIE_DEVICE_ID_PORT12            0x9D1B  ///< PCI Express Root Port #12, SKL PCH LP PCIe Device ID

#define V_PCH_QAT_DEVICE_ID                       0x37c8  ///< PCH QAT Device ID

//
// LBG Production (PRQ) PCI Express Root Ports Device ID's
//
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT1            0xA190  ///< PCI Express Root Port #1,  LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT2            0xA191  ///< PCI Express Root Port #2,  LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT3            0xA192  ///< PCI Express Root Port #3,  LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT4            0xA193  ///< PCI Express Root Port #4,  LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT5            0xA194  ///< PCI Express Root Port #5,  LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT6            0xA195  ///< PCI Express Root Port #6,  LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT7            0xA196  ///< PCI Express Root Port #7,  LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT8            0xA197  ///< PCI Express Root Port #8,  LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT9            0xA198  ///< PCI Express Root Port #9,  LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT10           0xA199  ///< PCI Express Root Port #10, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT11           0xA19A  ///< PCI Express Root Port #11, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT12           0xA19B  ///< PCI Express Root Port #12, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT13           0xA19C  ///< PCI Express Root Port #13, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT14           0xA19D  ///< PCI Express Root Port #14, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT15           0xA19E  ///< PCI Express Root Port #15, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT16           0xA19F  ///< PCI Express Root Port #16, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT17           0xA1E7  ///< PCI Express Root Port #17, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT18           0xA1E8  ///< PCI Express Root Port #18, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT19           0xA1E9  ///< PCI Express Root Port #19, LBG PRQ
#define V_PCH_LBG_PROD_PCIE_DEVICE_ID_PORT20           0xA1EA  ///< PCI Express Root Port #20, LBG PRQ
//
// LBG Super SKU (SSX) PCI Express Root Ports Device ID's
//
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT1         0xA210  ///< PCI Express Root Port #1,  LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT2         0xA211  ///< PCI Express Root Port #2,  LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT3         0xA212  ///< PCI Express Root Port #3,  LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT4         0xA213  ///< PCI Express Root Port #4,  LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT5         0xA214  ///< PCI Express Root Port #5,  LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT6         0xA215  ///< PCI Express Root Port #6,  LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT7         0xA216  ///< PCI Express Root Port #7,  LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT8         0xA217  ///< PCI Express Root Port #8,  LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT9         0xA218  ///< PCI Express Root Port #9,  LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT10        0xA219  ///< PCI Express Root Port #10, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT11        0xA21A  ///< PCI Express Root Port #11, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT12        0xA21B  ///< PCI Express Root Port #12, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT13        0xA21C  ///< PCI Express Root Port #13, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT14        0xA21D  ///< PCI Express Root Port #14, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT15        0xA21E  ///< PCI Express Root Port #15, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT16        0xA21F  ///< PCI Express Root Port #16, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT17        0xA267  ///< PCI Express Root Port #17, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT18        0xA268  ///< PCI Express Root Port #18, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT19        0xA269  ///< PCI Express Root Port #19, LBG SSKU
#define V_PCH_LBG_PCIE_DEVICE_ID_PORT20        0xA26A  ///< PCI Express Root Port #20, LBG SSKU
typedef enum {
  PCIE_SPA,
  PCIE_SPB,
  PCIE_SPC,
  PCIE_SPD,
  PCIE_SPE
} PCIE_CONTROLLER_INDEX;

#define R_PCH_PCIE_PCISTS                         0x06
#define B_PCH_PCIE_PCISTS_CAP_LST                 BIT4
#define B_PCH_PCIE_XCAP_CV                        0x000F
#define B_PCH_PCIE_XCAP_DT                        (BIT7 | BIT6 | BIT5 | BIT4)

#define R_PCH_PCIE_CLIST                          0x40
#define R_PCH_PCIE_XCAP                           (R_PCH_PCIE_CLIST + R_PCIE_XCAP_OFFSET)
#define R_PCH_PCIE_CFG_DEVCAP                     (R_PCH_PCIE_CLIST + R_PCIE_DCAP_OFFSET)
#define R_PCH_PCIE_DCTL                           (R_PCH_PCIE_CLIST + R_PCIE_DCTL_OFFSET)
#define R_PCH_PCIE_DSTS                           (R_PCH_PCIE_CLIST + R_PCIE_DSTS_OFFSET)
#define R_PCH_PCIE_LCAP                           (R_PCH_PCIE_CLIST + R_PCIE_LCAP_OFFSET)
#define B_PCH_PCIE_LCAP_PN                        0xFF000000
#define N_PCH_PCIE_LCAP_PN                        24
#define R_PCH_PCIE_LCTL                           (R_PCH_PCIE_CLIST + R_PCIE_LCTL_OFFSET)
#define R_PCH_PCIE_LSTS                           (R_PCH_PCIE_CLIST + R_PCIE_LSTS_OFFSET)
#define R_PCH_PCIE_CFG_LINKSTS                    (R_PCH_PCIE_CLIST + R_PCIE_LSTS_OFFSET)
#define R_PCH_PCIE_SLCAP                          (R_PCH_PCIE_CLIST + R_PCIE_SLCAP_OFFSET)
#define R_PCH_PCIE_SLCTL                          (R_PCH_PCIE_CLIST + R_PCIE_SLCTL_OFFSET)
#define R_PCH_PCIE_SLSTS                          (R_PCH_PCIE_CLIST + R_PCIE_SLSTS_OFFSET)
#define R_PCH_PCIE_CFG_ROOTCTL                    (R_PCH_PCIE_CLIST + R_PCIE_RCTL_OFFSET)
#define R_PCH_PCIE_RSTS                           (R_PCH_PCIE_CLIST + R_PCIE_RSTS_OFFSET)
#define R_PCH_PCIE_CFG_DCAP2                      (R_PCH_PCIE_CLIST + R_PCIE_DCAP2_OFFSET)
#define R_PCH_PCIE_DCTL2                          (R_PCH_PCIE_CLIST + R_PCIE_DCTL2_OFFSET)
#define R_PCH_PCIE_LCTL2                          (R_PCH_PCIE_CLIST + R_PCIE_LCTL2_OFFSET)
#define R_PCH_PCIE_LSTS2                          (R_PCH_PCIE_CLIST + R_PCIE_LSTS2_OFFSET)


#define R_PCH_PCIE_MID                            0x80
#define S_PCH_PCIE_MID                            2
#define R_PCH_PCIE_MC                             0x82
#define S_PCH_PCIE_MC                             2
#define R_PCH_PCIE_MA                             0x84
#define S_PCH_PCIE_MA                             4
#define R_PCH_PCIE_MD                             0x88
#define S_PCH_PCIE_MD                             2

#define R_PCH_PCIE_SVCAP                          0x90
#define S_PCH_PCIE_SVCAP                          2
#define R_PCH_PCIE_SSVID                           0x94
#define S_PCH_PCIE_SSVID                           4

#define R_PCH_PCIE_PMCAP                          0xA0
#define R_PCH_PCIE_PMCS                           (R_PCH_PCIE_PMCAP + R_PCIE_PMCS_OFFST)

#define R_PCH_PCIE_CCFG                           0xD0
#define B_PCH_PCIE_CCFG_UPMWPD                    BIT25
#define B_PCH_PCIE_CCFG_UPSD                      BIT24
#define B_PCH_PCIE_CCFG_UNSD                      BIT23
#define B_PCH_PCIE_CCFG_NPAP                      BIT16
#define B_PCH_PCIE_CCFG_DCGEISMA                  BIT15
#define B_PCH_PCIE_CCFG_UNRD                      (BIT13 | BIT12)
#define N_PCH_PCIE_CCFG_UNRD                      12
#define B_PCH_PCIE_CCFG_UNRS                      (BIT6 | BIT5 | BIT4)
#define N_PCH_PCIE_CCFG_UNRS                      4
#define V_PCH_PCIE_CCFG_UNRS_128B                 0
#define V_PCH_PCIE_CCFG_UNRS_256B                 1
#define B_PCH_PCIE_CCFG_UPRS                      (BIT2 | BIT1 | BIT0)
#define N_PCH_PCIE_CCFG_UPRS                      0
#define V_PCH_PCIE_CCFG_UPRS_128B                 0
#define V_PCH_PCIE_CCFG_UPRS_256B                 1

#define R_PCH_PCIE_MPC2                           0xD4
#define S_PCH_PCIE_MPC2                           4
#define B_PCH_PCIE_MPC2_PTNFAE                    BIT12
#define B_PCH_PCIE_MPC2_LSTP                      BIT6
#define B_PCH_PCIE_MPC2_IEIME                     BIT5
#define B_PCH_PCIE_MPC2_ASPMCOEN                  BIT4
#define B_PCH_PCIE_MPC2_ASPMCO                    (BIT3 | BIT2)
#define V_PCH_PCIE_MPC2_ASPMCO_DISABLED           0
#define V_PCH_PCIE_MPC2_ASPMCO_L0S                (1 << 2)
#define V_PCH_PCIE_MPC2_ASPMCO_L1                 (2 << 2)
#define V_PCH_PCIE_MPC2_ASPMCO_L0S_L1             (3 << 2)
#define B_PCH_PCIE_MPC2_EOIFD                     BIT1

#define R_PCH_PCIE_MPC                            0xD8
#define S_PCH_PCIE_MPC                            4
#define B_PCH_PCIE_MPC_PMCE                       BIT31
#define B_PCH_PCIE_MPC_HPCE                       BIT30
#define B_PCH_PCIE_MPC_MMBNCE                     BIT27
#define B_PCH_PCIE_MPC_P8XDE                      BIT26
#define B_PCH_PCIE_MPC_IRRCE                      BIT25
#define B_PCH_PCIE_MPC_SRL                        BIT23
#define B_PCH_PCIE_MPC_UCEL                       (BIT20 | BIT19 | BIT18)
#define N_PCH_PCIE_MPC_UCEL                       18
#define B_PCH_PCIE_MPC_CCEL                       (BIT17 | BIT16 | BIT15)
#define N_PCH_PCIE_MPC_CCEL                       15
#define B_PCH_PCIE_MPC_PCIESD                     (BIT14 | BIT13)
#define N_PCH_PCIE_MPC_PCIESD                     13
#define V_PCH_PCIE_MPC_PCIESD_GEN1                1
#define V_PCH_PCIE_MPC_PCIESD_GEN2                2
#define B_PCH_PCIE_MPC_MCTPSE                     BIT3
#define B_PCH_PCIE_MPC_HPME                       BIT1
#define N_PCH_PCIE_MPC_HPME                       1
#define B_PCH_PCIE_MPC_PMME                       BIT0

#define R_PCH_PCIE_SMSCS                          0xDC
#define S_PCH_PCIE_SMSCS                          4
#define N_PCH_PCIE_SMSCS_LERSMIS                  5
#define N_PCH_PCIE_SMSCS_HPLAS                    4
#define N_PCH_PCIE_SMSCS_HPPDM                    1

#define R_PCH_PCIE_RPDCGEN                        0xE1
#define S_PCH_PCIE_RPDCGEN                        1
#define B_PCH_PCIE_RPDCGEN_RPSCGEN                BIT7
#define B_PCH_PCIE_RPDCGEN_PTOCGE                 BIT6
#define B_PCH_PCIE_RPDCGEN_LCLKREQEN              BIT5
#define B_PCH_PCIE_RPDCGEN_BBCLKREQEN             BIT4
#define B_PCH_PCIE_RPDCGEN_SRDBCGEN               BIT2
#define B_PCH_PCIE_RPDCGEN_RPDLCGEN               BIT1
#define B_PCH_PCIE_RPDCGEN_RPDBCGEN               BIT0

#define R_PCH_PCIE_RPPGEN                         0xE2
#define B_PCH_PCIE_RPPGEN_PTOTOP                  BIT6
#define B_PCH_PCIE_RPPGEN_SEOSCGE                 BIT4

#define R_PCH_PCIE_PWRCTL                         0xE8
#define B_PCH_PCIE_PWRCTL_LTSSMRTC                BIT20
#define B_PCH_PCIE_PWRCTL_WPDMPGEP                BIT17
#define B_PCH_PCIE_PWRCTL_DBUPI                   BIT15
#define B_PCH_PCIE_PWRCTL_TXSWING                 BIT13
#define B_PCH_PCIE_PWRCTL_RPL1SQPOL               BIT1
#define B_PCH_PCIE_PWRCTL_RPDTSQPOL               BIT0

#define R_PCH_PCIE_DC                             0xEC
#define B_PCH_PCIE_DC_PCIBEM                      BIT2

#define R_PCH_PCIE_PHYCTL2                        0xF5
#define B_PCH_PCIE_PHYCTL2_TDFT                   (BIT7 | BIT6)
#define B_PCH_PCIE_PHYCTL2_TXCFGCHGWAIT           (BIT5 | BIT4)
#define N_PCH_PCIE_PHYCTL2_TXCFGCHGWAIT           4
#define B_PCH_PCIE_PHYCTL2_PXPG3PLLOFFEN          BIT1
#define B_PCH_PCIE_PHYCTL2_PXPG2PLLOFFEN          BIT0

#define R_PCH_PCIE_IOSFSBCS                       0xF7
#define B_PCH_PCIE_IOSFSBCS_SCPTCGE               BIT6
#define B_PCH_PCIE_IOSFSBCS_SIID                  (BIT3 | BIT2)

#define R_PCH_PCIE_STRPFUSECFG                    0xFC
#define B_PCH_PCIE_STRPFUSECFG_SERM               BIT29
#define B_PCH_PCIE_STRPFUSECFG_PXIP               (BIT27 | BIT26 | BIT25 | BIT24)
#define N_PCH_PCIE_STRPFUSECFG_PXIP               24
#define B_PCH_PCIE_STRPFUSECFG_RPC                (BIT15 | BIT14)
#define V_PCH_PCIE_STRPFUSECFG_RPC_1_1_1_1        0
#define V_PCH_PCIE_STRPFUSECFG_RPC_2_1_1          1
#define V_PCH_PCIE_STRPFUSECFG_RPC_2_2            2
#define V_PCH_PCIE_STRPFUSECFG_RPC_4              3
#define N_PCH_PCIE_STRPFUSECFG_RPC                14
#define B_PCH_PCIE_STRPFUSECFG_MODPHYIOPMDIS      BIT9
#define B_PCH_PCIE_STRPFUSECFG_PLLSHTDWNDIS       BIT8
#define B_PCH_PCIE_STRPFUSECFG_STPGATEDIS         BIT7
#define B_PCH_PCIE_STRPFUSECFG_ASPMDIS            BIT6
#define B_PCH_PCIE_STRPFUSECFG_LDCGDIS            BIT5
#define B_PCH_PCIE_STRPFUSECFG_LTCGDIS            BIT4
#define B_PCH_PCIE_STRPFUSECFG_CDCGDIS            BIT3
#define B_PCH_PCIE_STRPFUSECFG_DESKTOPMOB         BIT2

//
//PCI Express Extended Capability Registers
//

#define R_PCH_PCIE_EXCAP_OFFSET                   0x100

#define R_PCH_PCIE_CFG_ROOTERRSTS                 0x130
#define S_PCH_PCIE_CFG_ROOTERRSTS                 4
#define B_PCH_PCIE_CFG_ROOTERRSTS_AEMN            0xF8000000
#define B_PCH_PCIE_CFG_ROOTERRSTS_FEMR            BIT6
#define B_PCH_PCIE_CFG_ROOTERRSTS_NFEMR           BIT5
#define B_PCH_PCIE_CFG_ROOTERRSTS_FUF             BIT4
#define B_PCH_PCIE_CFG_ROOTERRSTS_MEFR            BIT3
#define B_PCH_PCIE_CFG_ROOTERRSTS_EFR             BIT2
#define B_PCH_PCIE_CFG_ROOTERRSTS_MCER            BIT1
#define B_PCH_PCIE_CFG_ROOTERRSTS_CER             BIT0

#define R_PCH_PCIE_EX_AECH                        0x100 ///< Advanced Error Reporting Capability Header
#define V_PCH_PCIE_EX_AEC_CV                      0x1
#define R_PCH_PCIE_EX_UEM                         (R_PCH_PCIE_EX_AECH + R_PCIE_EX_UEM_OFFSET)
#define R_PCH_PCIE_EX_REC                         0x12C ///< Root Error Command
#define B_PCH_PCIE_EX_REC_FERE                    BIT2  ///< Root Error Command Fatal Error Reporting Enable
#define B_PCH_PCIE_EX_REC_NERE                    BIT1  ///< Root Error Command Non-fatal Error Reporting Enable
#define B_PCH_PCIE_EX_REC_CERE                    BIT0  ///< Root Error Command Correctable Error Reporting Enable

#define R_PCH_PCIE_EX_CES                         0x110 ///< Correctable Error Status
#define B_PCH_PCIE_EX_CES_BD                      BIT7  ///< Bad DLLP Status
#define B_PCH_PCIE_EX_CES_BT                      BIT6  ///< Bad TLP Status
#define B_PCH_PCIE_EX_CES_RE                      BIT0  ///< Receiver Error Status


//CES.RE, CES.BT, CES.BD

#define R_PCH_PCIE_EX_ACSECH                      0x140 ///< ACS Extended Capability Header
#define V_PCH_PCIE_EX_ACS_CV                      0x1
#define R_PCH_PCIE_EX_ACSCAPR                     (R_PCH_PCIE_EX_ACSECH + R_PCIE_EX_ACSCAPR_OFFSET)

#define R_PCH_PCIE_EX_L1SECH                      0x200 ///< L1 Sub-States Extended Capability Header
#define V_PCH_PCIE_EX_L1S_CV                      0x1
#define R_PCH_PCIE_EX_L1SCAP                      (R_PCH_PCIE_EX_L1SECH + R_PCIE_EX_L1SCAP_OFFSET)
#define R_PCH_PCIE_EX_L1SCTL1                     (R_PCH_PCIE_EX_L1SECH + R_PCIE_EX_L1SCTL1_OFFSET)
#define R_PCH_PCIE_EX_L1SCTL2                     (R_PCH_PCIE_EX_L1SECH + R_PCIE_EX_L1SCTL2_OFFSET)

#define R_PCH_PCIE_EX_SPEECH                      0x220 ///< Secondary PCI Express Extended Capability Header
#define V_PCH_PCIE_EX_SPEECH_CV                   0x1

#define R_PCH_PCIE_EX_LCTL3                       (R_PCH_PCIE_EX_SPEECH + R_PCIE_EX_LCTL3_OFFSET)
#define R_PCH_PCIE_EX_LES                         (R_PCH_PCIE_EX_SPEECH + R_PCIE_EX_LES_OFFSET)
#define R_PCH_PCIE_EX_LECTL                       (R_PCH_PCIE_EX_SPEECH + R_PCIE_EX_L01EC_OFFSET)
#define B_PCH_PCIE_EX_LECTL_UPTPH                 (BIT14 | BIT13 | BIT12)
#define N_PCH_PCIE_EX_LECTL_UPTPH                 12
#define B_PCH_PCIE_EX_LECTL_UPTP                  0x0F00
#define N_PCH_PCIE_EX_LECTL_UPTP                  8
#define B_PCH_PCIE_EX_LECTL_DPTPH                 (BIT6 | BIT5 | BIT4)
#define N_PCH_PCIE_EX_LECTL_DPTPH                 4
#define B_PCH_PCIE_EX_LECTL_DPTP                  0x000F
#define N_PCH_PCIE_EX_LECTL_DPTP                  0

#define R_PCH_PCIE_EX_L01EC                       (R_PCH_PCIE_EX_SPEECH + R_PCIE_EX_L01EC_OFFSET)
#define R_PCH_PCIE_EX_L23EC                       (R_PCH_PCIE_EX_SPEECH + R_PCIE_EX_L23EC_OFFSET)

#define R_PCH_PCIE_PCIERTP1                       0x300
#define R_PCH_PCIE_PCIERTP2                       0x304
#define R_PCH_PCIE_PCIENFTS                       0x314
#define R_PCH_PCIE_PCIEL0SC                       0x318

#define R_PCH_PCIE_PCIECFG2                       0x320
#define B_PCH_PCIE_PCIECFG2_LBWSSTE               BIT30
#define B_PCH_PCIE_PCIECFG2_RLLG3R                BIT27
#define B_PCH_PCIE_PCIECFG2_CROAOV                BIT24
#define B_PCH_PCIE_PCIECFG2_CROAOE                BIT23
#define B_PCH_PCIE_PCIECFG2_CRSREN                BIT22
#define B_PCH_PCIE_PCIECFG2_PMET                  (BIT21 | BIT20)
#define V_PCH_PCIE_PCIECFG2_PMET                  1
#define N_PCH_PCIE_PCIECFG2_PMET                  20

#define R_PCH_PCIE_PCIEDBG                        0x324
#define B_PCH_PCIE_PCIEDBG_USSP                   (BIT27 | BIT26)
#define B_PCH_PCIE_PCIEDBG_LGCLKSQEXITDBTIMERS    (BIT25 | BIT24)
#define B_PCH_PCIE_PCIEDBG_CTONFAE                BIT14
#define B_PCH_PCIE_PCIEDBG_SQOL0                  BIT7
#define B_PCH_PCIE_PCIEDBG_SPCE                   BIT5
#define B_PCH_PCIE_PCIEDBG_LR                     BIT4
#define B_PCH_PCIE_DMIL1EDM                       BIT3

#define R_PCH_PCIE_PCIESTS1                              0x328
#define B_PCH_PCIE_PCIESTS1_LTSMSTATE                    0xFF000000
#define N_PCH_PCIE_PCIESTS1_LTSMSTATE                    24
#define V_PCH_PCIE_PCIESTS1_LTSMSTATE_DETRDY             0x01
#define V_PCH_PCIE_PCIESTS1_LTSMSTATE_DETRDYECINP1CG     0x0E
#define V_PCH_PCIE_PCIESTS1_LTSMSTATE_L0                 0x33
#define V_PCH_PCIE_PCIESTS1_LTSMSTATE_DISWAIT            0x5E
#define V_PCH_PCIE_PCIESTS1_LTSMSTATE_DISWAITPG          0x60
#define V_PCH_PCIE_PCIESTS1_LTSMSTATE_RECOVERYSPEEDREADY 0x6C
#define V_PCH_PCIE_PCIESTS1_LTSMSTATE_RECOVERYLNK2DETECT 0x6F


#define B_PCH_PCIE_PCIESTS1_LNKSTAT               (BIT22 | BIT21 | BIT20 | BIT19)
#define N_PCH_PCIE_PCIESTS1_LNKSTAT               19
#define V_PCH_PCIE_PCIESTS1_LNKSTAT_L0            0x7

#define R_PCH_PCIE_PCIESTS2                       0x32C
#define B_PCH_PCIE_PCIESTS2_P4PNCCWSSCMES         BIT31
#define B_PCH_PCIE_PCIESTS2_P3PNCCWSSCMES         BIT30
#define B_PCH_PCIE_PCIESTS2_P2PNCCWSSCMES         BIT29
#define B_PCH_PCIE_PCIESTS2_P1PNCCWSSCMES         BIT28
#define B_PCH_PCIE_PCIESTS2_CLRE                  0x0000F000
#define N_PCH_PCIE_PCIESTS2_CLRE                  12

#define R_PCH_PCIE_PCIEALC                        0x338
#define B_PCH_PCIE_PCIEALC_ITLRCLD                BIT29
#define B_PCH_PCIE_PCIEALC_ILLRCLD                BIT28
#define B_PCH_PCIE_PCIEALC_BLKDQDA                BIT26

#define R_PCH_PCIE_LTROVR                         0x400

#define R_PCH_PCIE_LTROVR2                        0x404

#define R_PCH_PCIE_PHYCTL4                        0x408
#define B_PCH_PCIE_PHYCTL4_SQDIS                  BIT27


#define R_PCH_PCIE_PCIEPMECTL                     0x420
#define B_PCH_PCIE_PCIEPMECTL_FDPPGE              BIT31
#define B_PCH_PCIE_PCIEPMECTL_DLSULPPGE           BIT30
#define B_PCH_PCIE_PCIEPMECTL_DLSULDLSD           BIT29
#define B_PCH_PCIE_PCIEPMECTL_L1LE                BIT17
#define B_PCH_PCIE_PCIEPMECTL_L1LTRTLV            (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8 | BIT7 | BIT6 | BIT5 | BIT4)
#define N_PCH_PCIE_PCIEPMECTL_L1LTRTLV            4
#define V_PCH_PCIE_PCIEPMECTL_L1LTRTLV            0x32
#define B_PCH_PCIE_PCIEPMECTL_L1FSOE              BIT0

#define R_PCH_PCIE_PCIEPMECTL2                    0x424
#define B_PCH_PCIE_PCIEPMECTL2_PHYCLPGE           BIT11
#define B_PCH_PCIE_PCIEPMECTL2_FDCPGE             BIT8
#define B_PCH_PCIE_PCIEPMECTL2_DETSCPGE           BIT7
#define B_PCH_PCIE_PCIEPMECTL2_L23RDYSCPGE        BIT6
#define B_PCH_PCIE_PCIEPMECTL2_DISSCPGE           BIT5
#define B_PCH_PCIE_PCIEPMECTL2_L1SCPGE            BIT4

#define R_PCH_PCIE_PCE                            0x428
#define B_PCH_PCIE_PCE_HAE                        BIT5
#define B_PCH_PCIE_PCE_PMCRE                      BIT0

#define R_PCH_PCIE_EQCFG1                         0x450
#define S_PCH_PCIE_EQCFG1                         4
#define B_PCH_PCIE_EQCFG1_REC                     0xFF000000
#define N_PCH_PCIE_EQCFG1_REC                     24
#define B_PCH_PCIE_EQCFG1_REIFECE                 BIT23
#define N_PCH_PCIE_EQCFG1_LERSMIE                 21
#define B_PCH_PCIE_EQCFG1_LEP23B                  BIT18
#define B_PCH_PCIE_EQCFG1_LEP3B                   BIT17
#define B_PCH_PCIE_EQCFG1_RTLEPCEB                BIT16
#define B_PCH_PCIE_EQCFG1_RTPCOE                  BIT15
#define B_PCH_PCIE_EQCFG1_HPCMQE                  BIT13
#define B_PCH_PCIE_EQCFG1_HAED                    BIT12
#define B_PCH_PCIE_EQCFG1_EQTS2IRRC               BIT7
#define B_PCH_PCIE_EQCFG1_TUPP                    BIT1

#define R_PCH_PCIE_RTPCL1                         0x454
#define B_PCH_PCIE_RTPCL1_PCM                     BIT31
#define B_PCH_PCIE_RTPCL1_RTPRECL2PL4             0x3F000000
#define B_PCH_PCIE_RTPCL1_RTPOSTCL1PL3            0xFC0000
#define B_PCH_PCIE_RTPCL1_RTPRECL1PL2             0x3F000
#define B_PCH_PCIE_RTPCL1_RTPOSTCL0PL1            0xFC0
#define B_PCH_PCIE_RTPCL1_RTPRECL0PL0             0x3F

#define R_PCH_PCIE_RTPCL2                         0x458
#define B_PCH_PCIE_RTPCL2_RTPOSTCL3PL             0x3F000
#define B_PCH_PCIE_RTPCL2_RTPRECL3PL6             0xFC0
#define B_PCH_PCIE_RTPCL2_RTPOSTCL2PL5            0x3F

#define R_PCH_PCIE_RTPCL3                         0x45C
#define B_PCH_PCIE_RTPCL3_RTPRECL7                0x3F000000
#define B_PCH_PCIE_RTPCL3_RTPOSTCL6               0xFC0000
#define B_PCH_PCIE_RTPCL3_RTPRECL6                0x3F000
#define B_PCH_PCIE_RTPCL3_RTPOSTCL5               0xFC0
#define B_PCH_PCIE_RTPCL3_RTPRECL5PL10            0x3F

#define R_PCH_PCIE_RTPCL4                         0x460
#define B_PCH_PCIE_RTPCL4_RTPOSTCL9               0x3F000000
#define B_PCH_PCIE_RTPCL4_RTPRECL9                0xFC0000
#define B_PCH_PCIE_RTPCL4_RTPOSTCL8               0x3F000
#define B_PCH_PCIE_RTPCL4_RTPRECL8                0xFC0
#define B_PCH_PCIE_RTPCL4_RTPOSTCL7               0x3F

#define R_PCH_PCIE_FOMS                           0x464
#define B_PCH_PCIE_FOMS_I                         (BIT30 | BIT29)
#define N_PCH_PCIE_FOMS_I                         29
#define B_PCH_PCIE_FOMS_LN                        0x1F000000
#define N_PCH_PCIE_FOMS_LN                        24
#define B_PCH_PCIE_FOMS_FOMSV                     0x00FFFFFF
#define B_PCH_PCIE_FOMS_FOMSV0                    0x000000FF
#define N_PCH_PCIE_FOMS_FOMSV0                    0
#define B_PCH_PCIE_FOMS_FOMSV1                    0x0000FF00
#define N_PCH_PCIE_FOMS_FOMSV1                    8
#define B_PCH_PCIE_FOMS_FOMSV2                    0x00FF0000
#define N_PCH_PCIE_FOMS_FOMSV2                    16

#define R_PCH_PCIE_HAEQ                           0x468
#define B_PCH_PCIE_HAEQ_HAPCCPI                   (BIT31 | BIT30 | BIT29 | BIT28)
#define N_PCH_PCIE_HAEQ_HAPCCPI                   28
#define B_PCH_PCIE_HAEQ_MACFOMC                   BIT19

#define R_PCH_PCIE_LTCO1                          0x470
#define B_PCH_PCIE_LTCO1_L1TCOE                   BIT25
#define B_PCH_PCIE_LTCO1_L0TCOE                   BIT24
#define B_PCH_PCIE_LTCO1_L1TPOSTCO                0xFC0000
#define N_PCH_PCIE_LTCO1_L1TPOSTCO                18
#define B_PCH_PCIE_LTCO1_L1TPRECO                 0x3F000
#define N_PCH_PCIE_LTCO1_L1TPRECO                 12
#define B_PCH_PCIE_LTCO1_L0TPOSTCO                0xFC0
#define N_PCH_PCIE_LTCO1_L0TPOSTCO                6
#define B_PCH_PCIE_LTCO1_L0TPRECO                 0x3F
#define N_PCH_PCIE_LTCO1_L0TPRECO                 0

#define R_PCH_PCIE_LTCO2                          0x474
#define B_PCH_PCIE_LTCO2_L3TCOE                   BIT25
#define B_PCH_PCIE_LTCO2_L2TCOE                   BIT24
#define B_PCH_PCIE_LTCO2_L3TPOSTCO                0xFC0000
#define B_PCH_PCIE_LTCO2_L3TPRECO                 0x3F000
#define B_PCH_PCIE_LTCO2_L2TPOSTCO                0xFC0
#define B_PCH_PCIE_LTCO2_L2TPRECO                 0x3F

#define R_PCH_PCIE_G3L0SCTL                       0x478
#define B_PCH_PCIE_G3L0SCTL_G3UCNFTS              0x0000FF00
#define B_PCH_PCIE_G3L0SCTL_G3CCNFTS              0x000000FF

#define R_PCH_PCIE_EQCFG2                         0x47C
#define B_PCH_PCIE_EQCFG2_NTIC                    0xFF000000
#define B_PCH_PCIE_EQCFG2_EMD                     BIT23
#define B_PCH_PCIE_EQCFG2_NTSS                    (BIT22 | BIT21 | BIT20)
#define B_PCH_PCIE_EQCFG2_PCET                    (BIT19 | BIT18 | BIT17 | BIT16)
#define N_PCH_PCIE_EQCFG2_PCET                    16
#define B_PCH_PCIE_EQCFG2_HAPCSB                  (BIT15 | BIT14 | BIT13 | BIT12)
#define N_PCH_PCIE_EQCFG2_HAPCSB                  12
#define B_PCH_PCIE_EQCFG2_NTEME                   BIT11
#define B_PCH_PCIE_EQCFG2_MPEME                   BIT10
#define B_PCH_PCIE_EQCFG2_REWMETM                 (BIT9 | BIT8)
#define B_PCH_PCIE_EQCFG2_REWMET                  0xFF

#define R_PCH_PCIE_MM                             0x480
#define B_PCH_PCIE_MM_MSST                        0xFFFFFF00
#define N_PCH_PCIE_MM_MSST                        8
#define B_PCH_PCIE_MM_MSS                         0xFF

//
//PCI Express Extended End Point Capability Registers
//

#define R_PCH_PCIE_LTRECH_OFFSET                  0
#define R_PCH_PCIE_LTRECH_CID                     0x0018
#define R_PCH_PCIE_LTRECH_MSLR_OFFSET             0x04
#define R_PCH_PCIE_LTRECH_MNSLR_OFFSET            0x06

//
// Pcie Uplink ports related registers and defines
//
#define PCI_DEVICE_NUMBER_PCH_PCIE_UX16           0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_UX16         0
#define PCI_DEVICE_NUMBER_PCH_PCIE_UX8            0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_UX8          0
#define V_PCH_LBG_PCIE_DEVICE_ID_UX16             0x37C0  ///< PCI Express Uplink x16 Device ID
#define V_PCH_LBG_NS_PCIE_DEVICE_ID_UX8           0x37C1  ///< PCI Express Uplink x8 Device ID, LBG-NS



//
// PCIE PCRs (PID:SPA SPB SPC SPD SPE)
//
#define R_PCH_PCR_SPX_PCD                         0                       ///< Port configuration and disable
#define B_PCH_PCR_SPX_PCD_RP1FN                   (BIT2 | BIT1 | BIT0)    ///< Port 1 Function Number
#define B_PCH_PCR_SPX_PCD_RP1CH                   BIT3                    ///< Port 1 config hide
#define B_PCH_PCR_SPX_PCD_RP2FN                   (BIT6 | BIT5 | BIT4)    ///< Port 2 Function Number
#define B_PCH_PCR_SPX_PCD_RP2CH                   BIT7                    ///< Port 2 config hide
#define B_PCH_PCR_SPX_PCD_RP3FN                   (BIT10 | BIT9 | BIT8)   ///< Port 3 Function Number
#define B_PCH_PCR_SPX_PCD_RP3CH                   BIT11                   ///< Port 3 config hide
#define B_PCH_PCR_SPX_PCD_RP4FN                   (BIT14 | BIT13 | BIT12) ///< Port 4 Function Number
#define B_PCH_PCR_SPX_PCD_RP4CH                   BIT15                   ///< Port 4 config hide
#define S_PCH_PCR_SPX_PCD_RP_FIELD                4                       ///< 4 bits for each RP FN
#define B_PCH_PCR_SPX_PCD_P1D                     BIT16                   ///< Port 1 disable
#define B_PCH_PCR_SPX_PCD_P2D                     BIT17                   ///< Port 2 disable
#define B_PCH_PCR_SPX_PCD_P3D                     BIT18                   ///< Port 3 disable
#define B_PCH_PCR_SPX_PCD_P4D                     BIT19                   ///< Port 4 disable
#define B_PCH_PCR_SPX_PCD_SRL                     BIT31                   ///< Secured Register Lock

#define R_PCH_PCR_SPX_PCIEHBP                     0x0004                  ///< PCI Express high-speed bypass
#define B_PCH_PCR_SPX_PCIEHBP_PCIEHBPME           BIT0                    ///< PCIe HBP mode enable
#define B_PCH_PCR_SPX_PCIEHBP_PCIEGMO             (BIT2 | BIT1)           ///< PCIe gen mode override
#define B_PCH_PCR_SPX_PCIEHBP_PCIETIL0O           BIT3                    ///< PCIe transmitter-in-L0 override
#define B_PCH_PCR_SPX_PCIEHBP_PCIERIL0O           BIT4                    ///< PCIe receiver-in-L0 override
#define B_PCH_PCR_SPX_PCIEHBP_PCIELRO             BIT5                    ///< PCIe link recovery override
#define B_PCH_PCR_SPX_PCIEHBP_PCIELDO             BIT6                    ///< PCIe link down override
#define B_PCH_PCR_SPX_PCIEHBP_PCIESSM             BIT7                    ///< PCIe SKP suppression mode
#define B_PCH_PCR_SPX_PCIEHBP_PCIESST             BIT8                    ///< PCIe suppress SKP transmission
#define B_PCH_PCR_SPX_PCIEHBP_PCIEHBPPS           (BIT13 | BIT12)         ///< PCIe HBP port select
#define B_PCH_PCR_SPX_PCIEHBP_CRCSEL              (BIT15 | BIT14)         ///< CRC select
#define B_PCH_PCR_SPX_PCIEHBP_PCIEHBPCRC          0xFFFF0000              ///< PCIe HBP CRC


//
// ICC PCR (PID: ICC)
//
#define R_PCH_PCR_ICC_TMCSRCCLK                   0x1000                  ///< Timing Control SRC Clock Register
#define R_PCH_PCR_ICC_TMCSRCCLK2                  0x1004                  ///< Timing Control SRC Clock Register 2


#define R_PCH_VSPX_ERRCORSTS                      0x0110                  //< Correctable Error Status Register
#define B_PCH_VSPX_ERRCORSTS_ANFE                 BIT13

#endif
