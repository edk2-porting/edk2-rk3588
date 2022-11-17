/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCIE_REGS_H_
#define _PCIE_REGS_H_

#include <IndustryStandard/Pci30.h>

//
// PCI type 0 Header
//
#define R_PCI_PI_OFFSET                           0x09
#define R_PCI_SCC_OFFSET                          0x0A
#define R_PCI_BCC_OFFSET                          0x0B

//
// PCI type 1 Header
//
#define R_PCI_BRIDGE_BNUM                         0x18 ///< Bus Number Register
#define B_PCI_BRIDGE_BNUM_SBBN                    0x00FF0000 ///< Subordinate Bus Number
#define B_PCI_BRIDGE_BNUM_SCBN                    0x0000FF00 ///< Secondary Bus Number
#define B_PCI_BRIDGE_BNUM_PBN                     0x000000FF ///< Primary Bus Number
#define B_PCI_BRIDGE_BNUM_SBBN_SCBN               (B_PCI_BRIDGE_BNUM_SBBN | B_PCI_BRIDGE_BNUM_SCBN)

#define R_PCI_BRIDGE_IOBL                         0x1C ///< I/O Base and Limit Register

#define R_PCI_BRIDGE_MBL                          0x20 ///< Memory Base and Limit Register
#define B_PCI_BRIDGE_MBL_ML                       0xFFF00000 ///< Memory Limit
#define B_PCI_BRIDGE_MBL_MB                       0x0000FFF0 ///< Memory Base

#define R_PCI_BRIDGE_PMBL                         0x24 ///< Prefetchable Memory Base and Limit Register
#define B_PCI_BRIDGE_PMBL_PML                     0xFFF00000 ///< Prefetchable Memory Limit
#define B_PCI_BRIDGE_PMBL_I64L                    0x000F0000 ///< 64-bit Indicator
#define B_PCI_BRIDGE_PMBL_PMB                     0x0000FFF0 ///< Prefetchable Memory Base
#define B_PCI_BRIDGE_PMBL_I64B                    0x0000000F ///< 64-bit Indicator

#define R_PCI_BRIDGE_PMBU32                       0x28 ///< Prefetchable Memory Base Upper 32-Bit Register
#define B_PCI_BRIDGE_PMBU32                       0xFFFFFFFF

#define R_PCI_BRIDGE_PMLU32                       0x2C ///< Prefetchable Memory Limit Upper 32-Bit Register
#define B_PCI_BRIDGE_PMLU32                       0xFFFFFFFF

//
// PCIE capabilities register
//
#define R_PCIE_CAP_ID_OFFSET                      0x00 ///< Capability ID
#define R_PCIE_CAP_NEXT_PRT_OFFSET                0x01 ///< Next Capability Capability ID Pointer

//
// PCI Express Capability List Register (CAPID:10h)
//
#define R_PCIE_XCAP_OFFSET                        0x02 ///< PCI Express Capabilities Register (Offset 02h)
#define S_PCIE_XCAP                               2
#define B_PCIE_XCAP_SI                            BIT8 ///< Slot Implemented
#define B_PCIE_XCAP_DT                            (BIT7 | BIT6 | BIT5 | BIT4) ///< Device/Port Type
#define N_PCIE_XCAP_DT                            4

#define R_PCIE_DCAP_OFFSET                        0x04 ///< Device Capabilities Register (Offset 04h)
#define S_PCIE_DCAP                               4
#define B_PCIE_DCAP_E1AL                          (BIT11 | BIT10 | BIT9) ///< Endpoint L1 Acceptable Latency
#define N_PCIE_DCAP_E1AL                          9
#define B_PCIE_DCAP_E0AL                          (BIT8 | BIT7 | BIT6) ///< Endpoint L0s Acceptable Latency
#define N_PCIE_DCAP_E0AL                          6
#define B_PCIE_DCAP_MPS                           (BIT2 | BIT1 | BIT0) ///< Max_Payload_Size Supported

#define R_PCIE_DCTL_OFFSET                        0x08 ///< Device Control Register (Offset 08h)
#define B_PCIE_DCTL_MPS                           (BIT7 | BIT6 | BIT5) ///< Max_Payload_Size
#define N_PCIE_DCTL_MPS                           5
#define B_PCIE_DCTL_URE                           BIT3 ///< Unsupported Request Reporting Enable
#define B_PCIE_DCTL_FEE                           BIT2 ///< Fatal Error Reporting Enable
#define B_PCIE_DCTL_NFE                           BIT1 ///< Non-Fatal Error Reporting Enable
#define B_PCIE_DCTL_CEE                           BIT0 ///< Correctable Error Reporting Enable

#define R_PCIE_DSTS_OFFSET                        0x0A ///< Device Status Register (Offset 0Ah)
#define B_PCIE_DSTS_TDP                           BIT5 ///< Transactions Pending
#define B_PCIE_DSTS_APD                           BIT4 ///< AUX Power Detected
#define B_PCIE_DSTS_URD                           BIT3 ///< Unsupported Request Detected
#define B_PCIE_DSTS_FED                           BIT2 ///< Fatal Error Detected
#define B_PCIE_DSTS_NFED                          BIT1 ///< Non-Fatal Error Detected
#define B_PCIE_DSTS_CED                           BIT0 ///< Correctable Error Detected

#define R_PCIE_LCAP_OFFSET                        0x0C ///< Link Capabilities Register (Offset 0Ch)
#define B_PCIE_LCAP_ASPMOC                        BIT22 ///< ASPM Optionality Compliance
#define B_PCIE_LCAP_CPM                           BIT18 ///< Clock Power Management
#define B_PCIE_LCAP_EL1                           (BIT17 | BIT16 | BIT15) ///< L1 Exit Latency
#define N_PCIE_LCAP_EL1                           15
#define B_PCIE_LCAP_EL0                           (BIT14 | BIT13 | BIT12) ///< L0s Exit Latency
#define N_PCIE_LCAP_EL0                           12
#define B_PCIE_LCAP_APMS                          (BIT11 | BIT10) ///< Active State Power Management (ASPM) Support
#define B_PCIE_LCAP_APMS_L0S                      BIT10
#define B_PCIE_LCAP_APMS_L1                       BIT11
#define N_PCIE_LCAP_APMS                          10
#define B_PCIE_LCAP_MLW                           0x000003F0 ///< Maximum Link Width
#define N_PCIE_LCAP_MLW                           4
#define B_PCIE_LCAP_MLS                           (BIT3 | BIT2 | BIT1 | BIT0) ///< Max Link Speed
#define V_PCIE_LCAP_MLS_GEN3                      3

#define R_PCIE_LCTL_OFFSET                        0x10 ///< Link Control Register (Offset 10h)
#define B_PCIE_LCTL_ECPM                          BIT8 ///< Enable Clock Power Management
#define B_PCIE_LCTL_ES                            BIT7 ///< Extended Synch
#define B_PCIE_LCTL_CCC                           BIT6 ///< Common Clock Configuration
#define B_PCIE_LCTL_RL                            BIT5 ///< Retrain Link
#define B_PCIE_LCTL_LD                            BIT4 ///< Link Disable
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
#define V_PCIE_LSTS_NLW_1                         0x0010
#define V_PCIE_LSTS_NLW_2                         0x0020
#define V_PCIE_LSTS_NLW_4                         0x0040
#define B_PCIE_LSTS_CLS                           0x000F ///< Current Link Speed
#define V_PCIE_LSTS_CLS_GEN1                      1
#define V_PCIE_LSTS_CLS_GEN2                      2
#define V_PCIE_LSTS_CLS_GEN3                      3

#define R_PCIE_SLCAP_OFFSET                       0x14 ///< Slot Capabilities Register (Offset 14h)
#define S_PCIE_SLCAP                              4
#define B_PCIE_SLCAP_PSN                          0xFFF80000 ///< Physical Slot Number
#define B_PCIE_SLCAP_SLS                          0x00018000 ///< Slot Power Limit Scale
#define B_PCIE_SLCAP_SLV                          0x00007F80 ///< Slot Power Limit Value
#define B_PCIE_SLCAP_HPC                          BIT6 ///< Hot-Plug Capable
#define B_PCIE_SLCAP_HPS                          BIT5 ///< Hot-Plug Surprise

#define R_PCIE_SLCTL_OFFSET                       0x18 ///< Slot Control Register (Offset 18h)
#define S_PCIE_SLCTL                              2
#define B_PCIE_SLCTL_HPE                          BIT5 ///< Hot Plug Interrupt Enable
#define B_PCIE_SLCTL_PDE                          BIT3 ///< Presence Detect Changed Enable

#define R_PCIE_SLSTS_OFFSET                       0x1A ///< Slot Status Register (Offset 1Ah)
#define S_PCIE_SLSTS                              2
#define B_PCIE_SLSTS_PDS                          BIT6 ///< Presence Detect State
#define B_PCIE_SLSTS_PDC                          BIT3 ///< Presence Detect Changed

#define R_PCIE_RCTL_OFFSET                        0x1C ///< Root Control Register (Offset 1Ch)
#define S_PCIE_RCTL                               2
#define B_PCIE_RCTL_PIE                           BIT3 ///< PME Interrupt Enable
#define B_PCIE_RCTL_SFE                           BIT2 ///< System Error on Fatal Error Enable
#define B_PCIE_RCTL_SNE                           BIT1 ///< System Error on Non-Fatal Error Enable
#define B_PCIE_RCTL_SCE                           BIT0 ///< System Error on Correctable Error Enable

#define R_PCIE_RSTS_OFFSET                        0x20 ///< Root Status Register (Offset 20h)
#define S_PCIE_RSTS                               4

#define R_PCIE_DCAP2_OFFSET                       0x24 ///< Device Capabilities 2 Register (Offset 24h)
#define B_PCIE_DCAP2_OBFFS                        (BIT19 | BIT18) ///< OBFF Supported
#define B_PCIE_DCAP2_LTRMS                        BIT11 ///< LTR Mechanism Supported

#define R_PCIE_DCTL2_OFFSET                       0x28 ///< Device Control 2 Register (Offset 28h)
#define B_PCIE_DCTL2_OBFFEN                       (BIT14 | BIT13) ///< OBFF Enable
#define N_PCIE_DCTL2_OBFFEN                       13
#define V_PCIE_DCTL2_OBFFEN_DIS                   0 ///< Disabled
#define V_PCIE_DCTL2_OBFFEN_WAKE                  3 ///< Enabled using WAKE# signaling
#define B_PCIE_DCTL2_LTREN                        BIT10 ///< LTR Mechanism Enable
#define B_PCIE_DCTL2_CTD                          BIT4 ///< Completion Timeout Disable
#define B_PCIE_DCTL2_CTV                          (BIT3 | BIT2 | BIT1 | BIT0) ///< Completion Timeout Value
#define V_PCIE_DCTL2_CTV_DEFAULT                  0x0
#define V_PCIE_DCTL2_CTV_40MS_50MS                0x5
#define V_PCIE_DCTL2_CTV_160MS_170MS              0x6
#define V_PCIE_DCTL2_CTV_400MS_500MS              0x9
#define V_PCIE_DCTL2_CTV_1P6S_1P7S                0xA

#define R_PCIE_LCTL2_OFFSET                       0x30 ///< Link Control 2 Register (Offset 30h)
#define B_PCIE_LCTL2_SD                           BIT6 ///< Selectable de-emphasis (0 = -6dB, 1 = -3.5dB)
#define B_PCIE_LCTL2_TLS                          (BIT3 | BIT2 | BIT1 | BIT0) ///< Target Link Speed
#define V_PCIE_LCTL2_TLS_GEN1                     1
#define V_PCIE_LCTL2_TLS_GEN2                     2
#define V_PCIE_LCTL2_TLS_GEN3                     3

#define R_PCIE_LSTS2_OFFSET                       0x32 ///< Link Status 2 Register (Offset 32h)
#define B_PCIE_LSTS2_LER                          BIT5 ///< Link Equalization Request
#define B_PCIE_LSTS2_EQP3S                        BIT4 ///< Equalization Phase 3 Successful
#define B_PCIE_LSTS2_EQP2S                        BIT3 ///< Equalization Phase 2 Successful
#define B_PCIE_LSTS2_EQP1S                        BIT2 ///< Equalization Phase 1 Successful
#define B_PCIE_LSTS2_EC                           BIT1 ///< Equalization Complete
#define B_PCIE_LSTS2_CDL                          BIT0 ///< Current De-emphasis Level

//
// PCI Power Management Capability (CAPID:01h)
//
#define R_PCIE_PMC_OFFSET                         0x02 ///< Power Management Capabilities Register
#define S_PCIE_PMC                                2
#define B_PCIE_PMC_PMES                           (BIT15 | BIT14 | BIT13 | BIT12 | BIT11) ///< PME Support
#define B_PCIE_PMC_PMEC                           BIT3 ///< PME Clock

#define R_PCIE_PMCS_OFFST                         0x04 ///< Power Management Status/Control Register
#define S_PCIE_PMCS                               4
#define B_PCIE_PMCS_BPCE                          BIT23 ///< Bus Power/Clock Control Enable
#define B_PCIE_PMCS_B23S                          BIT22 ///< B2/B3 Support
#define B_PCIE_PMCS_PMES                          BIT15 ///< PME_Status
#define B_PCIE_PMCS_PMEE                          BIT8 ///< PME Enable
#define B_PCIE_PMCS_NSR                           BIT3 ///< No Soft Reset
#define B_PCIE_PMCS_PS                            (BIT1 | BIT0) ///< Power State
#define V_PCIE_PMCS_PS_D0                         0
#define V_PCIE_PMCS_PS_D3H                        3

//
// PCIE Extension Capability Register
//
#define B_PCIE_EXCAP_NCO                          0xFFF00000 ///< Next Capability Offset
#define N_PCIE_EXCAP_NCO                          20
#define V_PCIE_EXCAP_NCO_LISTEND                  0
#define B_PCIE_EXCAP_CV                           0x000F0000 ///< Capability Version
#define N_PCIE_EXCAP_CV                           16
#define B_PCIE_EXCAP_CID                          0x0000FFFF ///< Capability ID

//
// Advanced Error Reporting Capability (CAPID:0001h)
//
#define V_PCIE_EX_AEC_CID                         0x0001 ///< Capability ID
#define R_PCIE_EX_UEM_OFFSET                      0x08 ///< Uncorrectable Error Mask Register
#define B_PCIE_EX_UEM_CT                          BIT14 ///< Completion Timeout Mask
#define B_PCIE_EX_UEM_UC                          BIT16 ///< Unexpected Completion

//
// ACS Extended Capability (CAPID:000Dh)
//
#define V_PCIE_EX_ACS_CID                         0x000D ///< Capability ID
#define R_PCIE_EX_ACSCAPR_OFFSET                  0x04 ///< ACS Capability Register
//#define R_PCIE_EX_ACSCTLR_OFFSET                  0x08 ///< ACS Control Register (NOTE: register size in PCIE spce is not match the PCH register size)

//
// Secondary PCI Express Extended Capability Header (CAPID:0019h)
//
#define V_PCIE_EX_SPE_CID                         0x0019 ///< Capability ID
#define R_PCIE_EX_LCTL3_OFFSET                    0x04 ///< Link Control 3 Register
#define B_PCIE_EX_LCTL3_PE                        BIT0 ///< Perform Equalization
#define R_PCIE_EX_LES_OFFSET                      0x08 ///< Lane Error Status
#define R_PCIE_EX_L01EC_OFFSET                    0x0C ///< Lane 0 and Lan 1 Equalization Control Register (Offset 0Ch)
#define B_PCIE_EX_L01EC_UPL1TP                    0x0F000000 ///< Upstream Port Lane 1 Transmitter Preset
#define N_PCIE_EX_L01EC_UPL1TP                    24
#define B_PCIE_EX_L01EC_DPL1TP                    0x000F0000 ///< Downstream Port Lane 1 Transmitter Preset
#define N_PCIE_EX_L01EC_DPL1TP                    16
#define B_PCIE_EX_L01EC_UPL0TP                    0x00000F00 ///< Upstream Port Transmitter Preset
#define N_PCIE_EX_L01EC_UPL0TP                    8
#define B_PCIE_EX_L01EC_DPL0TP                    0x0000000F ///< Downstream Port Transmitter Preset
#define N_PCIE_EX_L01EC_DPL0TP                    0

#define R_PCIE_EX_L23EC_OFFSET                    0x10 ///< Lane 2 and Lane 3 Equalization Control Register (Offset 10h)
#define B_PCIE_EX_L23EC_UPL3TP                    0x0F000000 ///< Upstream Port Lane 3 Transmitter Preset
#define N_PCIE_EX_L23EC_UPL3TP                    24
#define B_PCIE_EX_L23EC_DPL3TP                    0x000F0000 ///< Downstream Port Lane 3 Transmitter Preset
#define N_PCIE_EX_L23EC_DPL3TP                    16
#define B_PCIE_EX_L23EC_UPL2TP                    0x00000F00 ///< Upstream Port Lane 2 Transmitter Preset
#define N_PCIE_EX_L23EC_UPL2TP                    8
#define B_PCIE_EX_L23EC_DPL2TP                    0x0000000F ///< Downstream Port Lane 2 Transmitter Preset
#define N_PCIE_EX_L23EC_DPL2TP                    0


//
// L1 Sub-States Extended Capability Register (CAPID:001Eh)
//
#define V_PCIE_EX_L1S_CID                         0x001E ///< Capability ID
#define R_PCIE_EX_L1SCAP_OFFSET                   0x04 ///< L1 Sub-States Capabilities
#define R_PCIE_EX_L1SCTL1_OFFSET                  0x08 ///< L1 Sub-States Control 1
#define R_PCIE_EX_L1SCTL2_OFFSET                  0x0C ///< L1 Sub-States Control 2
#define N_PCIE_EX_L1SCTL2_POWT                    3

//
// Base Address Offset
//
#define R_BASE_ADDRESS_OFFSET_0                   0x0010 ///< Base Address Register 0
#define R_BASE_ADDRESS_OFFSET_1                   0x0014 ///< Base Address Register 1
#define R_BASE_ADDRESS_OFFSET_2                   0x0018 ///< Base Address Register 2
#define R_BASE_ADDRESS_OFFSET_3                   0x001C ///< Base Address Register 3
#define R_BASE_ADDRESS_OFFSET_4                   0x0020 ///< Base Address Register 4
#define R_BASE_ADDRESS_OFFSET_5                   0x0024 ///< Base Address Register 5

#endif
