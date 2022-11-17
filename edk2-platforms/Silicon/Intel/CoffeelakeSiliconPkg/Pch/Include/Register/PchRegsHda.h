/** @file
  Register names for PCH High Definition Audio device.

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

#ifndef _PCH_REGS_HDA_H_
#define _PCH_REGS_HDA_H_

//
// HD-A Controller Registers (D31:F3)
//
// PCI Configuration Space Registers
//
#define PCI_DEVICE_NUMBER_PCH_HDA               31
#define PCI_FUNCTION_NUMBER_PCH_HDA             3

#define R_HDA_CFG_PI                            0x09
#define V_HDA_CFG_PI_ADSP_UAA                   0x80
#define R_HDA_CFG_SCC                           0x0A
#define V_HDA_CFG_SCC_ADSP                      0x01
#define R_HDA_CFG_HDALBA                        0x10
#define B_HDA_CFG_HDALBA_LBA                    0xFFFFC000
#define V_HDA_CFG_HDBAR_SIZE                    (1 << 14)
#define R_HDA_CFG_HDAUBA                        0x14
#define B_HDA_CFG_HDAUBA_UBA                    0xFFFFFFFF
#define R_HDA_CFG_CGCTL                         0x48
#define B_HDA_CFG_CGCTL_RSMTCGE                 BIT18
#define B_HDA_CFG_CGCTL_MISCBDCGE               BIT6
#define R_HDA_CFG_PC                            0x52
#define V_HDA_CFG_PC_PMES                       0x18
#define N_HDA_CFG_PC_PMES                       11
#define R_HDA_CFG_PCS                           0x54
#define B_HDA_CFG_PCS_PMEE                      BIT8
#define B_HDA_CFG_PCS_PS                        (BIT1 | BIT0)
#define R_HDA_CFG_MMC                           0x62
#define B_HDA_CFG_MMC_ME                        BIT0
#define R_HDA_CFG_DEVC                          0x78
#define B_HDA_CFG_DEVC_NSNPEN                   BIT11
#define R_HDA_CFG_SEM1                          0xC0
#define B_HDA_CFG_SEM1_LFLCS                    BIT24
#define B_HDA_CFG_SEM1_BLKC3DIS                 BIT17
#define B_HDA_CFG_SEM1_TMODE                    BIT12
#define B_HDA_CFG_SEM1_FIFORDYSEL               (BIT10 | BIT9)
#define R_HDA_CFG_SEM2                          0xC4
#define B_HDA_CFG_SEM2_BSMT                     (BIT27 | BIT26)
#define V_HDA_CFG_SEM2_BSMT                     0x1
#define N_HDA_CFG_SEM2_BSMT                     26
#define B_HDA_CFG_SEM2_VC0SNR                   BIT24
#define B_HDA_CFG_SEM2_DUM                      BIT23
#define R_HDA_CFG_SEM3L                         0xC8
#define B_HDA_CFG_SEM3L_ISL1EXT2                (BIT21 | BIT20)
#define V_HDA_CFG_SEM3L_ISL1EXT2                0x2
#define N_HDA_CFG_SEM3L_ISL1EXT2                20
#define R_HDA_CFG_SEM4L                         0xD0
#define B_HDA_CFG_SEM4L_OSL1EXT2                (BIT21 | BIT20)
#define V_HDA_CFG_SEM4L_OSL1EXT2                0x3
#define N_HDA_CFG_SEM4L_OSL1EXT2                20

//
// Memory Space Registers
//
//
// Resides in 'HD Audio Global Registers' (0000h)
//
#define R_HDA_MEM_GCAP                        0x00
#define R_HDA_MEM_GCTL                        0x08
#define B_HDA_MEM_GCTL_CRST                   BIT0

#define R_HDA_MEM_OUTPAY                      0x04
#define R_HDA_MEM_INPAY                       0x06
#define V_HDA_MEM_INPAY_DEFAULT               0x1C

#define R_HDA_MEM_WAKEEN                      0x0C
#define B_HDA_MEM_WAKEEN_SDI_3                BIT3
#define B_HDA_MEM_WAKEEN_SDI_2                BIT2
#define B_HDA_MEM_WAKEEN_SDI_1                BIT1
#define B_HDA_MEM_WAKEEN_SDI_0                BIT0

#define R_HDA_MEM_WAKESTS                     0x0E
#define B_HDA_MEM_WAKESTS_SDIN3               BIT3
#define B_HDA_MEM_WAKESTS_SDIN2               BIT2
#define B_HDA_MEM_WAKESTS_SDIN1               BIT1
#define B_HDA_MEM_WAKESTS_SDIN0               BIT0

//
// Resides in 'HD Audio Controller Registers' (0030h)
//
#define R_HDA_MEM_IC                          0x60
#define R_HDA_MEM_IR                          0x64
#define R_HDA_MEM_ICS                         0x68
#define B_HDA_MEM_ICS_IRV                     BIT1
#define B_HDA_MEM_ICS_ICB                     BIT0

//
// Resides in 'HD Audio Processing Pipe Capability Structure' (0800h)
//
#define R_HDA_MEM_PPC                         0x0800 // Processing Pipe Capability Structure (Memory Space, offset 0800h)
#define R_HDA_MEM_PPCTL                       (R_HDA_MEM_PPC + 0x04)
#define B_HDA_MEM_PPCTL_GPROCEN               BIT30

//
// Resides in 'HD Audio Multiple Links Capability Structure' (0C00h)
//
#define HDA_HDALINK_INDEX                     0
#define HDA_IDISPLINK_INDEX                   1

#define R_HDA_MEM_MLC                         0x0C00 // Multiple Links Capability Structure (Memory Space, offset 0C00h)
#define R_HDA_MEM_LCTLX(x)                    (R_HDA_MEM_MLC + (0x40 + (0x40 * (x)) + 0x04)) // x - Link index: 0 - HDA Link, 1 - iDisp Link
#define B_HDA_MEM_LCTLX_CPA                   BIT23
#define B_HDA_MEM_LCTLX_SPA                   BIT16
#define N_HDA_MEM_LCTLX_SCF                   0
#define V_HDA_MEM_LCTLX_SCF_6MHZ              0x0
#define V_HDA_MEM_LCTLX_SCF_12MHZ             0x1
#define V_HDA_MEM_LCTLX_SCF_24MHZ             0x2
#define V_HDA_MEM_LCTLX_SCF_48MHZ             0x3
#define V_HDA_MEM_LCTLX_SCF_96MHZ             0x4

//
// Resides in 'HD Audio Vendor Specific Registers' (1000h)
//
#define R_HDA_MEM_LTRC                        0x1048
#define V_HDA_MEM_LTRC_GB                     0x29
#define N_HDA_MEM_LTRC_GB                     0
#define R_HDA_MEM_PCE                         0x104B
#define B_HDA_MEM_PCE_D3HE                    BIT2

//
// Private Configuration Space Registers
//
//
// Resides in IOSF & Fabric Configuration Registers (000h)
//
#define R_HDA_PCR_TTCCFG                    0xE4
#define B_HDA_PCR_TTCCFG_HCDT               BIT1

//
// Resides in PCI & Codec Configuration Registers (500h)
//
#define R_HDA_PCR_PCICDCCFG                 0x500 // PCI & Codec Configuration Registers (PCR, offset 500h)
#define B_HDA_PCR_PCICDCCFG_ACPIIN          0x0000FF00
#define N_HDA_PCR_PCICDCCFG_ACPIIN          8
#define R_HDA_PCR_FNCFG                     (R_HDA_PCR_PCICDCCFG + 0x30)
#define B_HDA_PCR_FNCFG_PGD                 BIT5
#define B_HDA_PCR_FNCFG_BCLD                BIT4
#define B_HDA_PCR_FNCFG_CGD                 BIT3
#define B_HDA_PCR_FNCFG_ADSPD               BIT2
#define B_HDA_PCR_FNCFG_HDASD               BIT0
#define R_HDA_PCR_CDCCFG                    (R_HDA_PCR_PCICDCCFG + 0x34)
#define B_HDA_PCR_CDCCFG_DIS_SDIN2          BIT2

//
// Resides in Power Management & EBB Configuration Registers (600h)
//
#define R_HDA_PCR_PWRMANCFG                 0x600 // Power Management & EBB Configuration Registers (PCR, offset 600h)
#define R_HDA_PCR_APLLP0                    (R_HDA_PCR_PWRMANCFG + 0x10)
#define V_HDA_PCR_APLLP0                    0xFC1E0000
#define R_HDA_PCR_APLLP1                    (R_HDA_PCR_PWRMANCFG + 0x14)
#define V_HDA_PCR_APLLP1                    0x00003F00
#define R_HDA_PCR_APLLP2                    (R_HDA_PCR_PWRMANCFG + 0x18)
#define V_HDA_PCR_APLLP2                    0x0000011D
#define R_HDA_PCR_IOBCTL                    (R_HDA_PCR_PWRMANCFG + 0x1C)
#define B_HDA_PCR_IOBCTL_OSEL               (BIT9 | BIT8)
#define V_HDA_PCR_IOBCTL_OSEL_HDALINK       0
#define V_HDA_PCR_IOBCTL_OSEL_HDALINK_I2S   1
#define V_HDA_PCR_IOBCTL_OSEL_I2S           3
#define N_HDA_PCR_IOBCTL_OSEL               8
#define B_HDA_PCR_IOBCTL_VSEL               BIT1
#define R_HDA_PCR_PTDC                      (R_HDA_PCR_PWRMANCFG + 0x28)
#define B_HDA_PCR_PTDC_SRMIW                (BIT6 | BIT5 | BIT4)


#endif
