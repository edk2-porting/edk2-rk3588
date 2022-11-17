/** @file
  Register names for PCH LPC/eSPI device

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
#ifndef _PCH_REGS_LPC_H_
#define _PCH_REGS_LPC_H_

#define B_LPC_CFG_DID         0xFFE0

//
// PCI to LPC Bridge Registers
//

#define R_LPC_CFG_IOD                             0x80
#define V_LPC_CFG_IOD_COMB_2F8                    1
#define V_LPC_CFG_IOD_COMA_3F8                    0
#define V_LPC_CFG_IOD_COMA_2F8                    1
#define R_LPC_CFG_IOE                             0x82
#define B_LPC_CFG_IOE_SE                          BIT12           ///< Super I/O Enable, Enables decoding of I/O locations 2Eh and 2Fh to LPC.
#define B_LPC_CFG_IOE_KE                          BIT10           ///< Keyboard Enable, Enables decoding of the keyboard I/O locations 60h and 64h to LPC.
#define B_LPC_CFG_IOE_PPE                         BIT2            ///< Parallel Port Enable, Enables decoding of the LPT range to LPC. Range is selected by LIOD.LPT.
#define B_LPC_CFG_IOE_CBE                         BIT1            ///< Com Port B Enable, Enables decoding of the COMB range to LPC. Range is selected LIOD.CB.
#define B_LPC_CFG_IOE_CAE                         BIT0            ///< Com Port A Enable, Enables decoding of the COMA range to LPC. Range is selected LIOD.CA.
#define R_LPC_CFG_ULKMC                           0x94
#define B_LPC_CFG_ULKMC_A20PASSEN                 BIT5
#define B_LPC_CFG_ULKMC_64WEN                     BIT3
#define B_LPC_CFG_ULKMC_64REN                     BIT2
#define B_LPC_CFG_ULKMC_60WEN                     BIT1
#define B_LPC_CFG_ULKMC_60REN                     BIT0
#define R_LPC_CFG_LGMR                            0x98
#define B_LPC_CFG_LGMR_MA                         0xFFFF0000
#define B_LPC_CFG_LGMR_LMRD_EN                    BIT0
#define R_ESPI_CFG_CS1IORE                        0xA0
#define R_ESPI_CFG_CS1GMR1                        0xA8

#define R_LPC_CFG_BDE                             0xD8                          ///< BIOS decode enable

//
// APM Registers
//
#define R_PCH_IO_APM_CNT                             0xB2
#define R_PCH_IO_APM_STS                             0xB3

#define R_LPC_CFG_BC                              0xDC            ///< Bios Control
#define S_LPC_CFG_BC                              1
#define N_LPC_CFG_BC_LE                           1
#define B_LPC_CFG_BC_WPD                          BIT0            ///< Write Protect Disable

#define R_ESPI_CFG_PCBC                           0xDC            ///< Peripheral Channel BIOS Control
#define S_ESPI_CFG_PCBC                           4               ///< Peripheral Channel BIOS Control register size
#define B_ESPI_CFG_PCBC_BWRE                      BIT11           ///< BIOS Write Report Enable
#define B_ESPI_CFG_PCBC_BWRS                      BIT10           ///< BIOS Write Report Status
#define B_ESPI_CFG_PCBC_BWPDS                     BIT8            ///< BIOS Write Protect Disable Status
#define N_ESPI_CFG_PCBC_BWPDS                     8               ///< BIOS Write Protect Disable Status bit position
#define B_ESPI_CFG_PCBC_ESPI_EN                   BIT2            ///< eSPI Enable Pin Strap
#define B_ESPI_CFG_PCBC_LE                        BIT1            ///< Lock Enable
#define N_ESPI_CFG_PCBC_LE                        1

//
// eSPI slave registers
//
#define B_ESPI_SLAVE_BME                          BIT2            ///< Bus Master Enable

//
// Reset Generator I/O Port
//
#define R_PCH_IO_RST_CNT                             0xCF9
#define V_PCH_IO_RST_CNT_FULLRESET                   0x0E
#define V_PCH_IO_RST_CNT_HARDRESET                   0x06

//
// eSPI PCR Registers
//
#define R_ESPI_PCR_SLV_CFG_REG_CTL            0x4000                  ///< Slave Configuration Register and Link Control
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SCRE       BIT31                   ///< Slave Configuration Register Access Enable
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SCRS       (BIT30 | BIT29 | BIT28) ///< Slave Configuration Register Access Status
#define N_ESPI_PCR_SLV_CFG_REG_CTL_SCRS       28                      ///< Slave Configuration Register Access Status bit position
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SBLCL      BIT27                   ///< IOSF-SB eSPI Link Configuration Lock
#define V_ESPI_PCR_SLV_CFG_REG_CTL_SCRS_NOERR 7                       ///< No errors (transaction completed successfully)
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SID        (BIT20 | BIT19)         ///< Slave ID
#define N_ESPI_PCR_SLV_CFG_REG_CTL_SID        19                      ///< Slave ID bit position
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SCRT       (BIT17 | BIT16)         ///< Slave Configuration Register Access Type
#define N_ESPI_PCR_SLV_CFG_REG_CTL_SCRT       16                      ///< Slave Configuration Register Access Type bit position
#define B_ESPI_PCR_SLV_CFG_REG_CTL_SCRA       0x00000FFF              ///< Slave Configuration Register Address
#define R_ESPI_PCR_SLV_CFG_REG_DATA           0x4004                  ///< Slave Configuration Register Data

#define R_ESPI_PCR_PCERR_SLV0                 0x4020          ///< Peripheral Channel Error for Slave 0
#define B_ESPI_PCR_PCERR_PCURD                BIT24           ///< Peripheral Channel Unsupported Request Detected
#define R_ESPI_PCR_VWERR_SLV0                 0x4030          ///< Virtual Wire Channel Error for Slave 0
#define R_ESPI_PCR_FCERR_SLV0                 0x4040          ///< Flash Access Channel Error for Slave 0
#define B_ESPI_PCR_FCERR_SAFBLK               BIT17           ///< SAF Blocked (SAFBLK)
#define B_ESPI_PCR_XERR_XNFEE                 (BIT14 | BIT13) ///< Non-Fatal Error Reporting Enable bits
#define N_ESPI_PCR_XERR_XNFEE                 13              ///< Non-Fatal Error Reporting Enable bit position
#define V_ESPI_PCR_XERR_XNFEE_SMI             3               ///< Enable Non-Fatal Error Reporting as SMI
#define B_ESPI_PCR_XERR_XNFES                 BIT12           ///< Fatal Error Status
#define B_ESPI_PCR_XERR_XFEE                  (BIT6 | BIT5)   ///< Fatal Error Reporting Enable bits
#define N_ESPI_PCR_XERR_XFEE                  5               ///< Fatal Error Reporting Enable bit position
#define V_ESPI_PCR_XERR_XFEE_SMI              3               ///< Enable Fatal Error Reporting as SMI
#define B_ESPI_PCR_XERR_XFES                  BIT4            ///< Fatal Error Status
#define S_ESPI_PCR_XERR                       4               ///< Channel register sizes
#define B_ESPI_PCR_PCERR_SLV0_PCURD           BIT24           ///< Peripheral Channel Unsupported Request Detected
#define R_ESPI_PCR_LNKERR_SLV0                0x4050          ///< Link Error for Slave 0
#define S_ESPI_PCR_LNKERR_SLV0                4               ///< Link Error for Slave 0 register size
#define B_ESPI_PCR_LNKERR_SLV0_SLCRR          BIT31           ///< eSPI Link and Slave Channel Recovery Required
#define B_ESPI_PCR_LNKERR_SLV0_LFET1E         (BIT22 | BIT21) ///< Fatal Error Type 1 Reporting Enable
#define N_ESPI_PCR_LNKERR_SLV0_LFET1E         21              ///< Fatal Error Type 1 Reporting Enable bit position
#define V_ESPI_PCR_LNKERR_SLV0_LFET1E_SMI     3               ///< Enable Fatal Error Type 1 Reporting as SMI
#define B_ESPI_PCR_LNKERR_SLV0_LFET1S         BIT20           ///< Link Fatal Error Type 1 Status
#define R_ESPI_PCR_LNKERR_SLV1                0x4054          ///< Link Error for Slave 1
#define R_ESPI_PCR_CFG_VAL                    0xC00C          ///< ESPI Enabled Strap
#define B_ESPI_PCR_CFG_VAL_ESPI_EN            BIT0            ///< ESPI Enabled Strap bit position
#define R_ESPI_PCR_SOFTSTRAPS                 0xC210          ///< eSPI Sofstraps Register 0
#define B_ESPI_PCR_SOFTSTRAPS_CS1_EN          BIT12           ///< CS1# Enable

#endif
