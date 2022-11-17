/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_REGS_USB_H_
#define _PCH_REGS_USB_H_

//
// USB3 (XHCI) related definitions
//
#define PCI_BUS_NUMBER_PCH_XHCI             0
#define PCI_DEVICE_NUMBER_PCH_XHCI          20
#define PCI_FUNCTION_NUMBER_PCH_XHCI        0

//
// XHCI PCI Config Space registers
//
#define V_PCH_USB_VENDOR_ID                 V_PCH_INTEL_VENDOR_ID
#define V_PCH_H_USB_DEVICE_ID_XHCI_1        0x8C31  ///< SKL PCH H XHCI#1
//
// LBG Production (PRQ) XHCI Controller Device ID
//
#define V_PCH_LBG_PROD_USB_DEVICE_ID_XHCI_1 0xA1AF  ///< LBG Production DID XHCI#1
//
// LBG Super SKU (SSX) XHCI Controller Device ID
//
#define V_PCH_LBG_USB_DEVICE_ID_XHCI_1      0xA22F  ///< LBG Super SKU DID XHCI#1
#define V_PCH_LP_USB_DEVICE_ID_XHCI_1       0x9C31  ///< SKL PCH LP XHCI#1

#define R_PCH_XHCI_MEM_BASE                 0x10
#define V_PCH_XHCI_MEM_LENGTH               0x10000
#define N_PCH_XHCI_MEM_ALIGN                16
#define B_PCH_XHCI_MEM_ALIGN_MASK           0xFFFF

#define R_PCH_XHCI_XHCC1                    0x40
#define B_PCH_XHCI_XHCC1_ACCTRL             BIT31
#define B_PCH_XHCI_XHCC1_RMTASERR           BIT24
#define B_PCH_XHCI_XHCC1_URD                BIT23
#define B_PCH_XHCI_XHCC1_URRE               BIT22
#define B_PCH_XHCI_XHCC1_IIL1E              (BIT21 | BIT20 | BIT19)
#define V_PCH_XHCI_XHCC1_IIL1E_DIS          0
#define V_PCH_XHCI_XHCC1_IIL1E_32           (BIT19)
#define V_PCH_XHCI_XHCC1_IIL1E_64           (BIT20)
#define V_PCH_XHCI_XHCC1_IIL1E_128          (BIT20 | BIT19)
#define V_PCH_XHCI_XHCC1_IIL1E_256          (BIT21)
#define V_PCH_XHCI_XHCC1_IIL1E_512          (BIT21 | BIT19)
#define V_PCH_XHCI_XHCC1_IIL1E_1024         (BIT21 | BIT20)
#define V_PCH_XHCI_XHCC1_IIL1E_131072       (BIT21 | BIT20 | BIT19)
#define B_PCH_XHCI_XHCC1_XHCIL1E            BIT18
#define B_PCH_XHCI_XHCC1_D3IL1E             BIT17
#define B_PCH_XHCI_XHCC1_UNPPA              (BIT16 | BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_XHCI_XHCC1_SWAXHCI            BIT11
#define B_PCH_XHCI_XHCC1_L23HRAWC           (BIT10 | BIT9 | BIT8)
#define B_PCH_XHCI_XHCC1_UTAGCP             (BIT7 | BIT6)
#define B_PCH_XHCI_XHCC1_UDAGCNP            (BIT5 | BIT4)
#define B_PCH_XHCI_XHCC1_UDAGCCP            (BIT3 | BIT2)
#define B_PCH_XHCI_XHCC1_UDAGC              (BIT1 | BIT0)

#define R_PCH_XHCI_XHCC2                    0x44
#define B_PCH_XHCI_XHCC2_OCCFDONE           BIT31
#define B_PCH_XHCI_XHCC2_XHCUPRDROE         BIT11
#define B_PCH_XHCI_XHCC2_IOSFSRAD           BIT10
#define B_PCH_XHCI_XHCC2_SWACXIHB           (BIT9 | BIT8)
#define B_PCH_XHCI_XHCC2_SWADMIL1IHB        (BIT7 | BIT6)
#define B_PCH_XHCI_XHCC2_L1FP2CGWC          (BIT5 | BIT4 | BIT3)
#define B_PCH_XHCI_XHCC2_RDREQSZCTRL        (BIT2 | BIT1 | BIT0)

#define R_PCH_XHCI_XHCLKGTEN                0x50
#define B_PCH_XHCI_XHCLKGTEN_SSLSE          BIT26
#define B_PCH_XHCI_XHCLKGTEN_USB2PLLSE      BIT25
#define B_PCH_XHCI_XHCLKGTEN_IOSFSTCGE      BIT24
#define B_PCH_XHCI_XHCLKGTEN_HSTCGE         (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_XHCI_XHCLKGTEN_SSTCGE         (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_XHCI_XHCLKGTEN_XHCIGEU3S      BIT15
#define B_PCH_XHCI_XHCLKGTEN_XHCFTCLKSE     BIT14
#define B_PCH_XHCI_XHCLKGTEN_XHCBBTCGIPISO  BIT13
#define B_PCH_XHCI_XHCLKGTEN_XHCHSTCGU2NRWE BIT12
#define B_PCH_XHCI_XHCLKGTEN_XHCUSB2PLLSDLE (BIT11 | BIT10)
#define B_PCH_XHCI_XHCLKGTEN_HSPLLSUE       (BIT9 | BIT8)
#define B_PCH_XHCI_XHCLKGTEN_SSPLLSUE       (BIT7 | BIT6 | BIT5)
#define B_PCH_XHCI_XHCLKGTEN_XHCBLCGE       BIT4
#define B_PCH_XHCI_XHCLKGTEN_HSLTCGE        BIT3
#define B_PCH_XHCI_XHCLKGTEN_SSLTCGE        BIT2
#define B_PCH_XHCI_XHCLKGTEN_IOSFBTCGE      BIT1
#define B_PCH_XHCI_XHCLKGTEN_IOSFGBLCGE     BIT0

#define R_PCH_XHCI_USB_RELNUM               0x60
#define B_PCH_XHCI_USB_RELNUM               0xFF
#define R_PCH_XHCI_FL_ADJ                   0x61
#define B_PCH_XHCI_FL_ADJ                   0x3F
#define R_PCH_XHCI_PWR_CAPID                0x70
#define B_PCH_XHCI_PWR_CAPID                0xFF
#define R_PCH_XHCI_NXT_PTR1                 0x71
#define B_PCH_XHCI_NXT_PTR1                 0xFF
#define R_PCH_XHCI_PWR_CAP                  0x72
#define B_PCH_XHCI_PWR_CAP_PME_SUP          0xF800
#define B_PCH_XHCI_PWR_CAP_D2_SUP           BIT10
#define B_PCH_XHCI_PWR_CAP_D1_SUP           BIT9
#define B_PCH_XHCI_PWR_CAP_AUX_CUR          (BIT8 | BIT7 | BIT6)
#define B_PCH_XHCI_PWR_CAP_DSI              BIT5
#define B_PCH_XHCI_PWR_CAP_PME_CLK          BIT3
#define B_PCH_XHCI_PWR_CAP_VER              (BIT2 | BIT1 | BIT0)
#define R_PCH_XHCI_PWR_CNTL_STS             0x74
#define B_PCH_XHCI_PWR_CNTL_STS_PME_STS     BIT15
#define B_PCH_XHCI_PWR_CNTL_STS_DATASCL     (BIT14 | BIT13)
#define B_PCH_XHCI_PWR_CNTL_STS_DATASEL     (BIT12 | BIT11 | BIT10 | BIT9)
#define B_PCH_XHCI_PWR_CNTL_STS_PME_EN      BIT8
#define B_PCH_XHCI_PWR_CNTL_STS_PWR_STS     (BIT1 | BIT0)
#define V_PCH_XHCI_PWR_CNTL_STS_PWR_STS_D3  (BIT1 | BIT0)
#define R_PCH_XHCI_MSI_MCTL                 0x82
#define R_PCH_XHCI_U2OCM                    0xB0
#define R_PCH_XHCI_U3OCM                    0xD0
#define V_PCH_XHCI_NUMBER_OF_OC_PINS        8

#define R_PCH_XHCI_FUS                      0xE0
#define B_PCH_XHCI_FUS_USBR                 (BIT5)
#define V_PCH_XHCI_FUS_USBR_EN              0
#define V_PCH_XHCI_FUS_USBR_DIS             (BIT5)

#define R_PCH_XHCI_FC                       0xFC

#define B_PCH_XHCI_FUS_SSPRTCNT             (BIT4 | BIT3)
#define V_PCH_XHCI_FUS_SSPRTCNT_00B         0
#define V_PCH_XHCI_FUS_SSPRTCNT_01B         (BIT3)
#define V_PCH_XHCI_FUS_SSPRTCNT_10B         (BIT4)
#define V_PCH_XHCI_FUS_SSPRTCNT_11B         (BIT4 | BIT3)

#define B_PCH_XHCI_FUS_HSPRTCNT             (BIT2 | BIT1)
#define V_PCH_XHCI_FUS_HSPRTCNT_00B         0
#define V_PCH_XHCI_FUS_HSPRTCNT_01B         (BIT1)
#define V_PCH_XHCI_FUS_HSPRTCNT_10B         (BIT2)
#define V_PCH_XHCI_FUS_HSPRTCNT_11B         (BIT2 | BIT1)

#define V_PCH_H_XHCI_FUS_SSPRTCNT_00B_CNT   6
#define V_PCH_H_XHCI_FUS_SSPRTCNT_01B_CNT   4
#define V_PCH_H_XHCI_FUS_SSPRTCNT_10B_CNT   2
#define V_PCH_H_XHCI_FUS_SSPRTCNT_11B_CNT   0
#define V_PCH_H_XHCI_FUS_SSPRTCNT_00B_MASK  0x3F
#define V_PCH_H_XHCI_FUS_SSPRTCNT_01B_MASK  0x0F
#define V_PCH_H_XHCI_FUS_SSPRTCNT_10B_MASK  0x03
#define V_PCH_H_XHCI_FUS_SSPRTCNT_11B_MASK  0x00

#define V_PCH_H_XHCI_FUS_HSPRTCNT_00B_CNT   14
#define V_PCH_H_XHCI_FUS_HSPRTCNT_01B_CNT   12
#define V_PCH_H_XHCI_FUS_HSPRTCNT_10B_CNT   10
#define V_PCH_H_XHCI_FUS_HSPRTCNT_11B_CNT   8
#define V_PCH_H_XHCI_FUS_HSPRTCNT_00B_MASK  0x3FFF
#define V_PCH_H_XHCI_FUS_HSPRTCNT_01B_MASK  0x3F3F
#define V_PCH_H_XHCI_FUS_HSPRTCNT_10B_MASK  0x03FF
#define V_PCH_H_XHCI_FUS_HSPRTCNT_11B_MASK  0x00FF

#define V_PCH_LP_XHCI_FIXED_SSPRTCNT        4
#define V_PCH_LP_XHCI_FIXED_SSPRTCNT_MASK   0x0F

#define V_PCH_LP_XHCI_FIXED_HSPRTCNT        8
#define V_PCH_LP_XHCI_FIXED_HSPRTCNT_MASK   0x00FF

//
// xHCI MMIO registers
//

//
// 0x00 - 0x1F - Capability Registers
//
#define R_PCH_XHCI_CAPLENGTH                0x00
#define R_PCH_XHCI_HCIVERSION               0x02
#define R_PCH_XHCI_HCSPARAMS1               0x04
#define R_PCH_XHCI_HCSPARAMS2               0x08
#define R_PCH_XHCI_HCSPARAMS3               0x0C
#define B_PCH_XHCI_HCSPARAMS3_U2DEL         0xFFFF0000
#define B_PCH_XHCI_HCSPARAMS3_U1DEL         0x0000FFFF
#define R_PCH_XHCI_HCCPARAMS                0x10
#define B_PCH_XHCI_HCCPARAMS_LHRC           BIT5
#define B_PCH_XHCI_HCCPARAMS_MAXPSASIZE     0xF000
#define R_PCH_XHCI_DBOFF                    0x14
#define R_PCH_XHCI_RTSOFF                   0x18

//
// 0x80 - 0xBF - Operational Registers
//
#define R_PCH_XHCI_USBCMD                   0x80
#define B_PCH_XHCI_USBCMD_RS                BIT0   ///< Run/Stop
#define B_PCH_XHCI_USBCMD_RST               BIT1   ///< HCRST
#define R_PCH_XHCI_USBSTS                   0x84
#define B_PCH_XHCI_USBSTS_HCH               BIT0
#define B_PCH_XHCI_USBSTS_CNR               BIT11

//
// 0x480 - 0x5CF - Port Status and Control Registers
//
#define R_PCH_LP_XHCI_PORTSC01USB2          0x480
#define R_PCH_LP_XHCI_PORTSC02USB2          0x490
#define R_PCH_LP_XHCI_PORTSC03USB2          0x4A0
#define R_PCH_LP_XHCI_PORTSC04USB2          0x4B0
#define R_PCH_LP_XHCI_PORTSC05USB2          0x4C0
#define R_PCH_LP_XHCI_PORTSC06USB2          0x4D0
#define R_PCH_LP_XHCI_PORTSC07USB2          0x4E0
#define R_PCH_LP_XHCI_PORTSC08USB2          0x4F0
#define R_PCH_LP_XHCI_PORTSC09USB2          0x500
#define R_PCH_LP_XHCI_PORTSC10USB2          0x510


#define R_PCH_LP_XHCI_PORTSC01USB3          0x540
#define R_PCH_LP_XHCI_PORTSC02USB3          0x550
#define R_PCH_LP_XHCI_PORTSC03USB3          0x560
#define R_PCH_LP_XHCI_PORTSC04USB3          0x570
#define R_PCH_LP_XHCI_PORTSC05USB3          0x580
#define R_PCH_LP_XHCI_PORTSC06USB3          0x590

//
// 0x480 - 0x5CF - Port Status and Control Registers
//
#define R_PCH_H_XHCI_PORTSC01USB2           0x480
#define R_PCH_H_XHCI_PORTSC02USB2           0x490
#define R_PCH_H_XHCI_PORTSC03USB2           0x4A0
#define R_PCH_H_XHCI_PORTSC04USB2           0x4B0
#define R_PCH_H_XHCI_PORTSC05USB2           0x4C0
#define R_PCH_H_XHCI_PORTSC06USB2           0x4D0
#define R_PCH_H_XHCI_PORTSC07USB2           0x4E0
#define R_PCH_H_XHCI_PORTSC08USB2           0x4F0
#define R_PCH_H_XHCI_PORTSC09USB2           0x500
#define R_PCH_H_XHCI_PORTSC10USB2           0x510
#define R_PCH_H_XHCI_PORTSC11USB2           0x520
#define R_PCH_H_XHCI_PORTSC12USB2           0x530
#define R_PCH_H_XHCI_PORTSC13USB2           0x540
#define R_PCH_H_XHCI_PORTSC14USB2           0x550

#define R_PCH_H_XHCI_PORTSC15USBR           0x560
#define R_PCH_H_XHCI_PORTSC16USBR           0x570

#define R_PCH_H_XHCI_PORTSC01USB3           0x580
#define R_PCH_H_XHCI_PORTSC02USB3           0x590
#define R_PCH_H_XHCI_PORTSC03USB3           0x5A0
#define R_PCH_H_XHCI_PORTSC04USB3           0x5B0
#define R_PCH_H_XHCI_PORTSC05USB3           0x5C0
#define R_PCH_H_XHCI_PORTSC06USB3           0x5D0
#define R_PCH_H_XHCI_PORTSC07USB3           0x5E0
#define R_PCH_H_XHCI_PORTSC08USB3           0x5F0
#define R_PCH_H_XHCI_PORTSC09USB3           0x600
#define R_PCH_H_XHCI_PORTSC10USB3           0x610

#define B_PCH_XHCI_PORTSCXUSB2_WPR          BIT31  ///< Warm Port Reset
#define B_PCH_XHCI_PORTSCXUSB2_CEC          BIT23  ///< Port Config Error Change
#define B_PCH_XHCI_PORTSCXUSB2_PLC          BIT22  ///< Port Link State Change
#define B_PCH_XHCI_PORTSCXUSB2_PRC          BIT21  ///< Port Reset Change
#define B_PCH_XHCI_PORTSCXUSB2_OCC          BIT20  ///< Over-current Change
#define B_PCH_XHCI_PORTSCXUSB2_WRC          BIT19  ///< Warm Port Reset Change
#define B_PCH_XHCI_PORTSCXUSB2_PEC          BIT18  ///< Port Enabled Disabled Change
#define B_PCH_XHCI_PORTSCXUSB2_CSC          BIT17  ///< Connect Status Change
#define B_PCH_XHCI_PORTSCXUSB2_LWS          BIT16  ///< Port Link State Write Strobe
#define B_PCH_XHCI_USB2_U3_EXIT             (BIT5 | BIT6 | BIT7 | BIT8)
#define B_PCH_XHCI_USB2_U0_MASK             (BIT5 | BIT6 | BIT7 | BIT8)
#define B_PCH_XHCI_PORTSCXUSB2_PP           BIT9
#define B_PCH_XHCI_PORTSCXUSB2_PLS          (BIT5 | BIT6 | BIT7 | BIT8)  ///< Port Link State
#define B_PCH_XHCI_PORTSCXUSB2_PR           BIT4   ///< Port Reset
#define B_PCH_XHCI_PORTSCXUSB2_PED          BIT1   ///< Port Enable/Disabled
#define B_PCH_XHCI_PORTSCXUSB2_CCS          BIT0   ///< Current Connect Status
#define B_PCH_XHCI_PORT_CHANGE_ENABLE_MASK  (B_PCH_XHCI_PORTSCXUSB2_CEC | B_PCH_XHCI_PORTSCXUSB2_PLC | B_PCH_XHCI_PORTSCXUSB2_PRC | B_PCH_XHCI_PORTSCXUSB2_OCC | B_PCH_XHCI_PORTSCXUSB2_WRC | B_PCH_XHCI_PORTSCXUSB2_PEC | B_PCH_XHCI_PORTSCXUSB2_CSC | B_PCH_XHCI_PORTSCXUSB2_PED)
#define B_PCH_XHCI_PORTPMSCXUSB2_PTC        (BIT28 | BIT29 | BIT30 | BIT31)  ///< Port Test Control

#define B_PCH_XHCI_PORTSCXUSB3_WPR          BIT31  ///< Warm Port Reset
#define B_PCH_XHCI_PORTSCXUSB3_CEC          BIT23  ///< Port Config Error Change
#define B_PCH_XHCI_PORTSCXUSB3_PLC          BIT22  ///< Port Link State Change
#define B_PCH_XHCI_PORTSCXUSB3_PRC          BIT21  ///< Port Reset Change
#define B_PCH_XHCI_PORTSCXUSB3_OCC          BIT20  ///< Over-current Change
#define B_PCH_XHCI_PORTSCXUSB3_WRC          BIT19  ///< Warm Port Reset Change
#define B_PCH_XHCI_PORTSCXUSB3_PEC          BIT18  ///< Port Enabled Disabled Change
#define B_PCH_XHCI_PORTSCXUSB3_CSC          BIT17  ///< Connect Status Change
#define B_PCH_XHCI_PORTSCXUSB3_PP           BIT9   ///< Port Power
#define B_PCH_XHCI_PORTSCXUSB3_PLS          (BIT8 | BIT7 | BIT6 | BIT5)    ///< Port Link State
#define V_PCH_XHCI_PORTSCXUSB3_PLS_POLLING  0x000000E0    ///< Link is in the Polling State
#define V_PCH_XHCI_PORTSCXUSB3_PLS_RXDETECT 0x000000A0    ///< Link is in the RxDetect State
#define B_PCH_XHCI_PORTSCXUSB3_PR           BIT4   ///< Port Reset
#define B_PCH_XHCI_PORTSCXUSB3_PED          BIT1   ///< Port Enable/Disabled
#define B_PCH_XHCI_PORTSCXUSB3_CHANGE_ENABLE_MASK  (B_PCH_XHCI_PORTSCXUSB3_CEC | B_PCH_XHCI_PORTSCXUSB3_PLC | B_PCH_XHCI_PORTSCXUSB3_PRC | B_PCH_XHCI_PORTSCXUSB3_OCC | B_PCH_XHCI_PORTSCXUSB3_WRC | B_PCH_XHCI_PORTSCXUSB3_PEC | B_PCH_XHCI_PORTSCXUSB3_CSC | B_PCH_XHCI_PORTSCXUSB3_PED)
//
// 0x2000 - 0x21FF - Runtime Registers
// 0x3000 - 0x307F - Doorbell Registers
//
#define R_PCH_XHCI_XECP_SUPP_USB2_2             0x8008
#define R_PCH_XHCI_XECP_SUPP_USB3_2             0x8028
#define R_PCH_XHCI_HOST_CTRL_SCH_REG            0x8094
#define R_PCH_XHCI_HOST_CTRL_IDMA_REG           0x809C
#define R_PCH_XHCI_PMCTRL                       0x80A4
#define R_PCH_XHCI_PGCBCTRL                     0x80A8  ///< PGCB Control
#define R_PCH_XHCI_HOST_CTRL_MISC_REG           0x80B0  ///< Host Controller Misc Reg
#define R_PCH_XHCI_HOST_CTRL_MISC_REG_2         0x80B4  ///< Host Controller Misc Reg 2
#define R_PCH_XHCI_SSPE                         0x80B8  ///< Super Speed Port Enables
#define B_PCH_XHCI_LP_SSPE_MASK                 0x3F    ///< LP: Mask for 6 USB3 ports
#define B_PCH_XHCI_H_SSPE_MASK                  0x3FF   ///< H: Mask for 10 USB3 ports
#define R_PCH_XHCI_DUAL_ROLE_CFG0               0x80D8
#define R_PCH_XHCI_DUAL_ROLE_CFG1               0x80DC
#define R_PCH_XHCI_AUX_CTRL_REG1                0x80E0
#define R_PCH_XHCI_HOST_CTRL_PORT_LINK_REG      0x80EC  ///< SuperSpeed Port Link Control
#define R_PCH_XHCI_XECP_CMDM_CTRL_REG1          0x818C  ///< Command Manager Control 1
#define R_PCH_XHCI_XECP_CMDM_CTRL_REG2          0x8190  ///< Command Manager Control 2
#define R_PCH_XHCI_XECP_CMDM_CTRL_REG3          0x8194  ///< Command Manager Control 3
#define R_PCH_XHCI_USB2_LINK_MGR_CTRL_REG1_DW1  0x80F0  ///< USB2_LINK_MGR_CTRL_REG1 - USB2 Port Link Control 1, 2, 3, 4
#define R_PCH_XHCI_USB2_LINK_MGR_CTRL_REG1_DW2  0x80F4  ///< USB2_LINK_MGR_CTRL_REG1 - USB2 Port Link Control 1, 2, 3, 4
#define R_PCH_XHCI_USB2_LINK_MGR_CTRL_REG1_DW3  0x80F8  ///< USB2_LINK_MGR_CTRL_REG1 - USB2 Port Link Control 1, 2, 3, 4
#define R_PCH_XHCI_USB2_LINK_MGR_CTRL_REG1_DW4  0x80FC  ///< USB2_LINK_MGR_CTRL_REG1 - USB2 Port Link Control 1, 2, 3, 4
#define R_PCH_XHCI_HOST_CTRL_TRM_REG2           0x8110  ///< HOST_CTRL_TRM_REG2 - Host Controller Transfer Manager Control 2
#define R_PCH_XHCI_AUX_CTRL_REG2                0x8154  ///< AUX_CTRL_REG2 - Aux PM Control Register 2
#define R_PCH_XHCI_AUXCLKCTL                    0x816C  ///< xHCI Aux Clock Control Register
#define R_PCH_XHCI_HOST_IF_PWR_CTRL_REG0        0x8140  ///< HOST_IF_PWR_CTRL_REG0 - Power Scheduler Control 0
#define R_PCH_XHCI_HOST_IF_PWR_CTRL_REG1        0x8144  ///< HOST_IF_PWR_CTRL_REG1 - Power Scheduler Control 1
#define R_PCH_XHCI_XHCLTVCTL2                   0x8174  ///< xHC Latency Tolerance Parameters - LTV Control
#define R_PCH_XHCI_LTVHIT                       0x817C  ///< xHC Latency Tolerance Parameters - High Idle Time Control
#define R_PCH_XHCI_LTVMIT                       0x8180  ///< xHC Latency Tolerance Parameters - Medium Idle Time Control
#define R_PCH_XHCI_LTVLIT                       0x8184  ///< xHC Latency Tolerance Parameters - Low Idle Time Control
#define R_PCH_XHCI_USB2PHYPM                    0x8164  ///< USB2 PHY Power Management Control
#define R_PCH_XHCI_PDDIS                        0x8198  ///< xHC Pulldown Disable Control
#define R_PCH_XHCI_THROTT                       0x819C  ///< XHCI Throttle Control
#define R_PCH_XHCI_LFPSPM                       0x81A0  ///< LFPS PM Control
#define R_PCH_XHCI_THROTT2                      0x81B4  ///< XHCI Throttle
#define R_PCH_XHCI_LFPSONCOUNT                  0x81B8  ///< LFPS On Count
#define R_PCH_XHCI_D0I2CTRL                     0x81BC  ///< D0I2 Control Register
#define R_PCH_XHCI_USB2PMCTRL                   0x81C4  ///< USB2 Power Management Control

//
// SKL PCH LP FUSE
//
#define R_PCH_XHCI_LP_FUSE1                     0x8410
#define B_PCH_XHCI_LP_FUS_HSPRTCNT              (BIT1)
#define B_PCH_XHCI_LP_FUS_USBR                  (BIT5)
#define R_PCH_XHCI_STRAP2                       0x8420  ///< USB3 Mode Strap
#define R_PCH_XHCI_USBLEGCTLSTS                 0x8470  ///< USB Legacy Support Control Status
#define B_PCH_XHCI_USBLEGCTLSTS_SMIBAR          BIT31   ///< SMI on BAR Status
#define B_PCH_XHCI_USBLEGCTLSTS_SMIPCIC         BIT30   ///< SMI on PCI Command Status
#define B_PCH_XHCI_USBLEGCTLSTS_SMIOSC          BIT29   ///< SMI on OS Ownership Change Status
#define B_PCH_XHCI_USBLEGCTLSTS_SMIBARE         BIT15   ///< SMI on BAR Enable
#define B_PCH_XHCI_USBLEGCTLSTS_SMIPCICE        BIT14   ///< SMI on PCI Command Enable
#define B_PCH_XHCI_USBLEGCTLSTS_SMIOSOE         BIT13   ///< SMI on OS Ownership Enable
#define B_PCH_XHCI_USBLEGCTLSTS_SMIHSEE         BIT4    ///< SMI on Host System Error Enable
#define B_PCH_XHCI_USBLEGCTLSTS_USBSMIE         BIT0    ///< USB SMI Enable

//
// Extended Capability Registers
//
#define R_PCH_XHCI_USB2PDO                      0x84F8
#define B_PCH_XHCI_LP_USB2PDO_MASK              0x3FF   ///< LP: Mask for 10 USB2 ports
#define B_PCH_XHCI_H_USB2PDO_MASK               0x7FFF  ///< H: Mask for 14 USB2 ports
#define B_PCH_XHCI_USB2PDO_DIS_PORT0            BIT0

#define R_PCH_XHCI_USB3PDO                      0x84FC
#define B_PCH_XHCI_LP_USB3PDO_MASK              0x3F    ///< LP: Mask for 6 USB3 ports
#define B_PCH_XHCI_H_USB3PDO_MASK               0x3FF   ///< H: Mask for 10 USB3 ports
#define B_PCH_XHCI_USB3PDO_DIS_PORT0            BIT0

//
// Debug Capability Descriptor Parameters
//
#define R_PCH_XHCI_DBC_DBCCTL                   0x8760    ///< DBCCTL - DbC Control

//
// xDCI (OTG) USB Device Controller
//
#define PCI_DEVICE_NUMBER_PCH_XDCI              20
#define PCI_FUNCTION_NUMBER_PCH_XDCI            1

//
// xDCI (OTG) PCI Config Space Registers
//
#define R_PCH_XDCI_MEM_BASE                   0x10
#define V_PCH_XDCI_MEM_LENGTH                 0x200000
#define R_PCH_XDCI_PMCSR                      0x84  ///< Power Management Control and Status Register
#define R_PCH_XDCI_GENERAL_PURPOSER_REG1      0xA0  ///< General Purpose PCI RW Register1
#define R_PCH_XDCI_CPGE                       0xA2  ///< Chassis Power Gate Enable
#define R_PCH_XDCI_GENERAL_PURPOSER_REG4      0xAC  ///< General Purpose PCI RW Register4
#define R_PCH_OTG_GENERAL_INPUT_REG           0xC0  ///< General Input Register

//
// xDCI (OTG) MMIO registers
//
#define R_PCH_XDCI_GCTL                       0xC110  ///< Xdci Global Ctrl
#define B_PCH_XDCI_GCTL_GHIBEREN              BIT1    ///< Hibernation enable
#define R_PCH_XDCI_GUSB2PHYCFG                0xC200  ///< Global USB2 PHY Configuration Register
#define B_PCH_XDCI_GUSB2PHYCFG_SUSPHY         BIT6    ///< Suspend USB2.0 HS/FS/LS PHY
#define R_PCH_XDCI_GUSB3PIPECTL0              0xC2C0  ///< Global USB3 PIPE Control Register 0
#define B_PCH_XDCI_GUSB3PIPECTL0_UX_IN_PX     BIT27   ///< Ux Exit in Px
#define R_PCH_XDCI_APBFC_U3PMU_CFG2           0x10F810
#define R_PCH_XDCI_APBFC_U3PMU_CFG4           0x10F818

//
// xDCI (OTG) Private Configuration Registers
// (PID:OTG)
//
#define R_PCH_PCR_OTG_IOSF_A2                 0xA2
#define R_PCH_PCR_OTG_IOSF_PMCTL              0x1D0
#define R_PCH_PCR_OTG_PCICFGCTRL1             0x200
#define B_PCH_PCR_OTG_PCICFGCTRL_PCI_IRQ      0x0FF00000
#define N_PCH_PCR_OTG_PCICFGCTRL_PCI_IRQ      20
#define B_PCH_PCR_OTG_PCICFGCTRL_ACPI_IRQ     0x000FF000
#define N_PCH_PCR_OTG_PCICFGCTRL_ACPI_IRQ     12
#define B_PCH_PCR_OTG_PCICFGCTRL_INT_PIN      0x00000F00
#define N_PCH_PCR_OTG_PCICFGCTRL_INT_PIN      8
#define B_PCH_PCR_OTG_PCICFGCTRL_BAR1_DIS     0x00000080
#define B_PCH_PCR_OTG_PCICFGCTRL_PME_SUP      0x0000007C
#define B_PCH_PCR_OTG_PCICFGCTRL_ACPI_INT_EN  0x00000002
#define B_PCH_PCR_OTG_PCICFGCTRL_PCI_CFG_DIS  0x00000001

//
// USB2 Private Configuration Registers
// USB2 HIP design featured
// (PID:USB2)
//
#define R_PCH_PCR_USB2_GLOBAL_PORT            0x4001    ///< USB2 GLOBAL PORT
#define R_PCH_PCR_USB2_400C                   0x400C
#define R_PCH_PCR_USB2_PP_LANE_BASE_ADDR      0x4000    ///< PP LANE base address
#define V_PCH_PCR_USB2_PER_PORT               0x00      ///< USB2 PER PORT          Addr[7:2] = 0x00
#define V_PCH_PCR_USB2_UTMI_MISC_PER_PORT     0x08      ///< UTMI MISC REG PER PORT Addr[7:2] = 0x08
#define V_PCH_PCR_USB2_PER_PORT_2             0x26      ///< USB2 PER PORT 2        Addr[7:2] = 0x26
#define R_PCH_PCR_USB2_402A                   0x402A
#define R_PCH_PCR_USB2_GLB_ADP_VBUS_REG       0x402B    ///< GLB ADP VBUS REG
#define R_PCH_PCR_USB2_GLOBAL_PORT_2          0x402C    ///< USB2 GLOBAL PORT 2
#define R_PCH_PCR_USB2_7034                   0x7034
#define R_PCH_PCR_USB2_7038                   0x7038
#define R_PCH_PCR_USB2_703C                   0x703C
#define R_PCH_PCR_USB2_7040                   0x7040
#define R_PCH_PCR_USB2_7044                   0x7044
#define R_PCH_PCR_USB2_7048                   0x7048
#define R_PCH_PCR_USB2_704C                   0x704C
#define R_PCH_PCR_USB2_CFG_COMPBG             0x7F04    ///< USB2 COMPBG

//
// xHCI SSIC registers
//
#define R_PCH_XHCI_SSIC_GLOBAL_CONF_CTRL      0x8804    ///< SSIC Global Configuration Control
#define R_PCH_XHCI_SSIC_CONF_REG1_PORT_1      0x8808    ///< SSIC Configuration Register 1 Port 1
#define R_PCH_XHCI_SSIC_CONF_REG2_PORT_1      0x880C    ///< SSIC Configuration Register 2 Port 1
#define R_PCH_XHCI_SSIC_CONF_REG3_PORT_1      0x8810    ///< SSIC Configuration Register 3 Port 1
#define R_PCH_XHCI_SSIC_CONF_REG1_PORT_2      0x8838    ///< SSIC Configuration Register 1 Port 2
#define R_PCH_XHCI_SSIC_CONF_REG2_PORT_2      0x883C    ///< SSIC Configuration Register 2 Port 2
#define R_PCH_XHCI_SSIC_CONF_REG3_PORT_2      0x8840    ///< SSIC Configuration Register 3 Port 2
#define B_PCH_XHCI_SSIC_CONF_REG2_PORT_UNUSED BIT31
#define B_PCH_XHCI_SSIC_CONF_REG2_PROG_DONE   BIT30

#define R_PCH_XHCI_SSIC_PROF_ATTR_PORT_1      0x8900    ///< Profile Attributes: Port 1 ... N
#define R_PCH_XHCI_SSIC_ACCESS_CONT_PORT_1    0x8904    ///< SSIC Port N Register Access Control: Port 1
#define R_PCH_XHCI_SSIC_PROF_ATTR_P1_0C       0x890C
#define R_PCH_XHCI_SSIC_PROF_ATTR_P1_10       0x8910
#define R_PCH_XHCI_SSIC_PROF_ATTR_P1_14       0x8914
#define R_PCH_XHCI_SSIC_PROF_ATTR_P1_18       0x8918
#define R_PCH_XHCI_SSIC_PROF_ATTR_P1_1C       0x891C
#define R_PCH_XHCI_SSIC_PROF_ATTR_P1_20       0x8920
#define R_PCH_XHCI_SSIC_PROF_ATTR_P1_24       0x8924
#define R_PCH_XHCI_SSIC_PROF_ATTR_P1_28       0x8928

#define R_PCH_XHCI_SSIC_PROF_ATTR_PORT_2      0x8A10    ///< Profile Attributes: Port 2 ... N
#define R_PCH_XHCI_SSIC_ACCESS_CONT_PORT_2    0x8A14    ///< SSIC Port N Register Access Control: Port 2
#define R_PCH_XHCI_SSIC_PROF_ATTR_P2_0C       0x8A1C
#define R_PCH_XHCI_SSIC_PROF_ATTR_P2_10       0x8A20
#define R_PCH_XHCI_SSIC_PROF_ATTR_P2_14       0x8A24
#define R_PCH_XHCI_SSIC_PROF_ATTR_P2_18       0x8A28
#define R_PCH_XHCI_SSIC_PROF_ATTR_P2_1C       0x8A2C
#define R_PCH_XHCI_SSIC_PROF_ATTR_P2_20       0x8A30
#define R_PCH_XHCI_SSIC_PROF_ATTR_P2_24       0x8A34
#define R_PCH_XHCI_SSIC_PROF_ATTR_P2_28       0x8A38

#endif
