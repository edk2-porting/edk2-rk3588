/** @file
  Register names for Camera block

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
#ifndef _PCH_REGS_CAM_H_
#define _PCH_REGS_CAM_H_

//
//  CIO2 Registers (D20:F3)
//
#define PCI_DEVICE_NUMBER_PCH_CIO2          20
#define PCI_FUNCTION_NUMBER_PCH_CIO2        3
#define V_PCH_CIO2_VENDOR_ID                V_PCH_INTEL_VENDOR_ID
#define V_PCH_LP_CIO2_DEVICE_ID             0x9D32

//
// CIO2 PCI Configuration space definitions
//
#define R_PCH_CIO2_CIOLBA                   0x10 // Camera IO Controller Lower Base Address
#define R_PCH_CIO2_CIOUBA                   0x14 // Camera IO Controller Upper Base Address
#define B_PCH_CIO2_CFG_PMCSR_NSR            BIT3 // No Software Reset
#define R_PCH_CIO2_CFG_MID_MMC              0x90 // MSI Capability ID
#define R_PCH_CIO2_CFG_MMLA                 0x94 // MSI Message Lower Address
#define R_PCH_CIO2_CFG_MMUA                 0x98 // MSI Message Lower Address
#define R_PCH_CIO2_CFG_MMD                  0x9C // MSI Message Data
#define R_PCH_CIO2_AFID                     0xA0 // Advanced Features Capability Identifiers
#define R_PCH_CIO2_AF_CMD_STS               0xA4 // Adavanced Features Command and Staus Register
#define R_PCH_CIO2_CFG_PID_PC               0xD0 // Power Management Capability Identifiers
#define R_PCH_CIO2_CFG_PMCSR                0xD4 // Power Management Control & Status
#define V_PCH_CIO2_CFG_PMCSR_PS_D3HOT       (BIT0 | BIT1)

//
// CAM_MMIO_CSI2
//

//
// CAM_MMIO_PRI
// Camera Pipe Host Controller's MMIO registers in Primary clock domain
//
#define R_PCH_CAM_MMIO_PRI_CIO2_CGC                    0x1400 ///< CIO2 Clock Gating Control
#define V_PCH_CAM_MMIO_PRI_CIO2_CGC_CLK_GATING_EN  (0x00003D7E)
#define V_PCH_CAM_MMIO_PRI_CIO2_CGC_CLK_GATING_DIS (0x05300000)
#define R_PCH_CAM_MMIO_PRI_CIO2_D0I3C                  0x1408 ///< CIO2 D0i3 Control Register
#define B_PCH_CAM_MMIO_PRI_CIO2_D0I3C_I3               BIT2   ///< I3 (D0i3). SW sets this bit to 1 to move the IP into the D0i3 state. Writing this bit to 0 will return the IP to the fully active D0 state (D0i0)
#define R_PCH_CAM_MMIO_PRI_PCE                         0x1430 ///< PCE Power Control Enable Register
#define B_PCH_CAM_MMIO_PRI_PCE_D3HE                    BIT2   ///< D3HE: D3-Hot Enable
#define B_PCH_CAM_MMIO_PRI_PCE_I3E                     BIT1   ///< I3E: I3 Enable
#define R_PCH_CAM_MMIO_PRI_CIO2_GPR0                   0x1434 ///< CIO2 General Purpose register 0
#define R_PCH_CAM_MMIO_PRI_CIO2_GPR1                   0x1438 ///< CIO2 General Purpose register 1

//
// CAM_PVT CHC space defininitions
// Private registers description for Camera Pipe Host Controller IP
// MSG IOSF-SB Port 0xA1 (PID_CAM_CHC)
//
#define R_PCH_PCR_CAM_CHC_PVT_FUSVAL                  0x00   ///< Fuse Value
#define R_PCH_PCR_CAM_CHC_PVT_ECCLOG                  0x04   ///< SRAM Error Count Log
#define R_PCH_PCR_CAM_CHC_PVT_DBGCTL                  0x08   ///< Debug Control
#define R_PCH_PCR_CAM_CHC_PVT_FNCFG                   0x0C   ///< Lock bits
#define B_PCH_PCR_CAM_CHC_PVT_FNCFG_MEM_LOCK          BIT8   ///< lock all lockable field in MEM space
#define B_PCH_PCR_CAM_CHC_PVT_FNCFG_BCLD              BIT0   ///< lock all lockable fields in CFG space
#define R_PCH_PCR_CAM_CHC_HDEVC                       0x10   ///< Hidden Device register
#define R_PCH_PCR_CAM_CHC_PVT_FUSE_DBG                0x14   ///< Hidden Device register

//
// fls space definitions
// CSI2 host controller's FLIS registers
// MSG IOSF-SB Port 0xAA (PID_CAM_FLS)
//
#define R_PCH_PCR_CAM_FLIS_CSI0_RXCNTRL               0x00
#define R_PCH_PCR_CAM_FLIS_CSI0_RCCRCOMP              0x01
#define R_PCH_PCR_CAM_FLIS_CSI0_BSCOMPARE             0x02
#define R_PCH_PCR_CAM_FLIS_CSI1_RXCNTRL               0x03
#define R_PCH_PCR_CAM_FLIS_CSI1_RCCRCOMP              0x04
#define R_PCH_PCR_CAM_FLIS_CSI0_INTLPBK_CFG1          0x05
#define R_PCH_PCR_CAM_FLIS_CSI1_BSCOMPARE             0x06
#define R_PCH_PCR_CAM_FLIS_CSI0_INTLPBK_CFG2          0x07
#define R_PCH_PCR_CAM_FLIS_CSI0_INTLPBK_CFG3          0x08
#define R_PCH_PCR_CAM_FLIS_CSI2_INTLPBK_CFG2          0x09
#define R_PCH_PCR_CAM_FLIS_CSI2_RXCNTRL               0x0A
#define R_PCH_PCR_CAM_FLIS_CSI2_RCCRCOMP              0x0B
#define R_PCH_PCR_CAM_FLIS_CSI2_BSCOMPARE             0x0C
#define R_PCH_PCR_CAM_FLIS_CSI0_INTLPBK_CMP_STAT      0x0D
#define R_PCH_PCR_CAM_FLIS_CSI0_INTLPBK_ERR_REG       0x0E
#define R_PCH_PCR_CAM_FLIS_CSI_CLKTRIM                0x0F
#define R_PCH_PCR_CAM_FLIS_CSI3_RXCNTRL               0x10
#define R_PCH_PCR_CAM_FLIS_CSI3_RCCRCOMP              0x11
#define R_PCH_PCR_CAM_FLIS_CSI3_BSCOMPARE             0x12
#define R_PCH_PCR_CAM_FLIS_CSI_CFG                    0x13
#define B_PCH_PCR_CAM_FLIS_CSI_CFG_ACIO_LB_EN         BIT26
#define R_PCH_PCR_CAM_FLIS_CSI1_INTLPBK_CFG1          0x14
#define R_PCH_PCR_CAM_FLIS_CSI1_INTLPBK_CFG2          0x15
#define R_PCH_PCR_CAM_FLIS_CSI1_INTLPBK_CFG3          0x16
#define R_PCH_PCR_CAM_FLIS_CSI2_INTLPBK_CFG1          0x17
#define R_PCH_PCR_CAM_FLIS_CSI2_INTLPBK_CFG3          0x18
#define R_PCH_PCR_CAM_FLIS_CSI3_INTLPBK_CFG1          0x19
#define R_PCH_PCR_CAM_FLIS_CSI3_INTLPBK_CFG3          0x1A
#define R_PCH_PCR_CAM_FLIS_CSI1_INTLPBK_ERR_REG       0x1B
#define R_PCH_PCR_CAM_FLIS_CSI1_INTLPBK_CMP_STAT      0x1C
#define R_PCH_PCR_CAM_FLIS_CSI2_INTLPBK_ERR_REG       0x1D
#define R_PCH_PCR_CAM_FLIS_CSI2_INTLPBK_CMP_STAT      0x1E
#define R_PCH_PCR_CAM_FLIS_CSI3_INTLPBK_ERR_REG       0x1F
#define R_PCH_PCR_CAM_FLIS_CSI3_INTLPBK_CMP_STAT      0x20
#define R_PCH_PCR_CAM_FLIS_CSI_RCOMPSTAT_REG          0x21
#define R_PCH_PCR_CAM_FLIS_CSI_DLLCTL_REG             0x22
#define R_PCH_PCR_CAM_FLIS_CSI_DATAEYE_REG            0x23
#define R_PCH_PCR_CAM_FLIS_CSI_DATATRIM               0x24
#define R_PCH_PCR_CAM_FLIS_CSI_CTLE                   0x25
#define R_PCH_PCR_CAM_FLIS_CSI0_DFT_CFG               0x26
#define R_PCH_PCR_CAM_FLIS_CSI1_DFT_CFG               0x27
#define R_PCH_PCR_CAM_FLIS_CSI2_DFT_CFG               0x28
#define R_PCH_PCR_CAM_FLIS_CSI3_DFT_CFG               0x29
#define R_PCH_PCR_CAM_FLIS_CSI_AFE_HS_CONTROL         0x2A
#define R_PCH_PCR_CAM_FLIS_CSI_RCOMP_STATUS           0x2B
#define R_PCH_PCR_CAM_FLIS_CSI_RCOMP_CONTROL          0x2C
#define R_PCH_PCR_CAM_FLIS_CSI_DATAEYE1_REG           0x2D
#define R_PCH_PCR_CAM_FLIS_CSI_ALL01                  0x2E
#define R_PCH_PCR_CAM_FLIS_CSI_DLLCTL1_REG            0x2F
#define R_PCH_PCR_CAM_FLIS_CSI_DATATRIM1              0x30

#endif

