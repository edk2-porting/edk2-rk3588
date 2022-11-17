/** @file
  Register definition for PSF component

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

#ifndef _PCH_REGS_PSF_CNL_H_
#define _PCH_REGS_PSF_CNL_H_

//PSF 1 Multicast Message Configuration
#define R_CNL_PCH_LP_PSF1_PCR_PSF_MC_CONTROL_MCAST0_EOI                 0x404C      ///< Multicast Control Register
#define R_CNL_PCH_LP_PSF1_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI              0x4064      ///< Destination ID
#define R_CNL_PCH_H_PSF1_PCR_PSF_MC_CONTROL_MCAST0_EOI                  0x403C      ///< Multicast Control Register
#define R_CNL_PCH_H_PSF1_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI               0x4054      ///< Destination ID

//
// PSF3 PCRs (PID:PSF3)
//
// PSF3 PCH-LP Specific Base Address
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_SPI2_REG_BASE         0x0100                  ///< D18F6 PSF base address (SerialIo: SPI2)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_ISH_REG_BASE          0x0200                  ///< D19F0 PSF base address (ISH)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_CNVI_REG_BASE         0x0400                  ///< D20F3 PSF base address (CNVi)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_SDCARD_REG_BASE       0x0500                  ///< D20F5 PSF base address (SCC: SDCard)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C0_REG_BASE         0x0600                  ///< D21F0 PSF base address (SerialIo: I2C0)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C1_REG_BASE         0x0700                  ///< D21F1 PSF base address (SerialIo: I2C1)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C2_REG_BASE         0x0800                  ///< D21F2 PSF base address (SerialIo: I2C2)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C3_REG_BASE         0x0900                  ///< D21F3 PSF base address (SerialIo: I2C3)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C4_REG_BASE         0x0A00                  ///< D25F0 PSF base address (SerialIo: I2C4)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_I2C5_REG_BASE         0x0B00                  ///< D25F1 PSF base address (SerialIo: I2C5)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_UART2_REG_BASE        0x0C00                  ///< D25F2 PSF base address (SerialIo: UART2)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_UART0_REG_BASE        0x0D00                  ///< D30F0 PSF base address (SerialIo: UART0)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_UART1_REG_BASE        0x0E00                  ///< D30F1 PSF base address (SerialIo: UART1)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_SPI0_REG_BASE         0x0F00                  ///< D30F2 PSF base address (SerialIo: SPI0)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_SPI1_REG_BASE         0x1000                  ///< D30F3 PSF base address (SerialIo: SPI1)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_LPC_REG_BASE          0x1100                  ///< D31F0 PSF base address (LPC)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_P2SB_REG_BASE         0x1300                  ///< D31F1 PSF base address (P2SB)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_PMC_REG_BASE          0x1400                  ///< D31F2 PSF base address (PMC)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_AUD_REG_BASE          0x1500                  ///< D31F3 PSF base address (HDA, ADSP)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_SMBUS_REG_BASE        0x1600                  ///< D31F4 PSF base address (SMBUS)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_SPI_SPI_REG_BASE      0x1700                  ///< D31F5 PSF base address (SPI SPI)
#define R_CNL_PCH_LP_PSF3_PCR_T0_SHDW_GBE_REG_BASE          0x1800                  ///< D31F6 PSF base address (GBE)
// PSF3 PCH-H Specific Base Address
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_SPI2_REG_BASE          0x0100                  ///< D18F6 PSF base address (SerialIo: SPI2)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_ISH_REG_BASE           0x0180                  ///< D19F0 PSF base address (ISH)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_CNVI_REG_BASE          0x0280                  ///< D20F3 PSF base address (CNVi)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_SDCARD_REG_BASE        0x0300                  ///< D20F5 PSF base address (SCC: SDCard)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_I2C0_REG_BASE          0x0380                  ///< D21F0 PSF base address (SerialIo: I2C0)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_I2C1_REG_BASE          0x0400                  ///< D21F1 PSF base address (SerialIo: I2C1)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_I2C2_REG_BASE          0x0480                  ///< D21F2 PSF base address (SerialIo: I2C2)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_I2C3_REG_BASE          0x0500                  ///< D21F3 PSF base address (SerialIo: I2C3)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_UART2_REG_BASE         0x0580                  ///< D25F2 PSF base address (SerialIo: UART2)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_UART0_REG_BASE         0x0600                  ///< D30F0 PSF base address (SerialIo: UART0)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_UART1_REG_BASE         0x0680                  ///< D30F1 PSF base address (SerialIo: UART1)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_SPI0_REG_BASE          0x0700                  ///< D30F2 PSF base address (SerialIo: SPI0)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_SPI1_REG_BASE          0x0780                  ///< D30F3 PSF base address (SerialIo: SPI1)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_LPC_REG_BASE           0x0800                  ///< D31F0 PSF base address (LPC)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_P2SB_REG_BASE          0x0900                  ///< D31F1 PSF base address (P2SB)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_PMC_REG_BASE           0x0980                  ///< D31F2 PSF base address (PMC)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_AUD_REG_BASE           0x0A00                  ///< D31F3 PSF base address (HDA, ADSP)H
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_SMBUS_REG_BASE         0x0A80                  ///< D31F4 PSF base address (SMBUS)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_SPI_SPI_REG_BASE       0x0B00                  ///< D31F5 PSF base address (SPI SPI)
#define R_CNL_PCH_H_PSF3_PCR_T0_SHDW_GBE_REG_BASE           0x0B80                  ///< D31F6 PSF base address (GBE)

// Other PSF3 PCRs definition
#define R_CNL_PCH_PSF3_PCR_PSF_MC_CONTROL_MCAST0_EOI        0x4058                  ///< Multicast Control Register  // LP&H
#define R_CNL_PCH_PSF3_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI     0x4064                  ///< Destination ID  // LP&H

#define R_CNL_PCH_H_PSF6_PCR_PSF_MC_CONTROL_MCAST0_EOI              0x4030          ///< Multicast Control Register
#define R_CNL_PCH_H_PSF6_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI           0x4048          ///< Destination ID
#define R_CNL_PCH_H_PSF6_PCR_PSF_MC_CONTROL_MCAST1_RS0_MCTP1        0x403C          ///< Multicast Control Register
#define R_CNL_PCH_H_PSF6_PCR_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1     0x4070          ///< Destination ID

#define R_CNL_PCH_H_PSF7_PCR_PSF_MC_CONTROL_MCAST0_EOI              0x4030          ///< Multicast Control Register
#define R_CNL_PCH_H_PSF7_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI           0x4048          ///< Destination ID
#define R_CNL_PCH_H_PSF7_PCR_PSF_MC_CONTROL_MCAST1_RS0_MCTP1        0x403C          ///< Multicast Control Register
#define R_CNL_PCH_H_PSF7_PCR_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1     0x4070          ///< Destination ID

#define R_CNL_PCH_H_PSF8_PCR_PSF_MC_CONTROL_MCAST0_EOI              0x4030          ///< Multicast Control Register
#define R_CNL_PCH_H_PSF8_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI           0x4048          ///< Destination ID
#define R_CNL_PCH_H_PSF8_PCR_PSF_MC_CONTROL_MCAST1_RS0_MCTP1        0x403C          ///< Multicast Control Register
#define R_CNL_PCH_H_PSF8_PCR_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1     0x4070          ///< Destination ID
#endif
