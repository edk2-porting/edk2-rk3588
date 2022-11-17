/** @file
  Register names for PCH Integrated Sensor Hub (ISH3.0)

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

#ifndef _PCH_REGS_ISH_H_
#define _PCH_REGS_ISH_H_

//
// ISH Controller Registers
//
// D19:F0
#define PCI_DEVICE_NUMBER_PCH_ISH             19
#define PCI_FUNCTION_NUMBER_PCH_ISH           0

// PCI Configuration Space Registers
#define R_ISH_CFG_BAR0_LOW                    0x10
#define R_ISH_CFG_BAR0_HIGH                   0x14
#define V_ISH_CFG_BAR0_SIZE                   0x100000
#define N_ISH_CFG_BAR0_ALIGNMENT              20
#define R_ISH_CFG_BAR1_LOW                    0x18
#define R_ISH_CFG_BAR1_HIGH                   0x1C
#define V_ISH_CFG_BAR1_SIZE                   0x1000
#define N_ISH_CFG_BAR1_ALIGNMENT              12

//
// ISH Private Configuration Space Registers (IOSF2OCP)
// (PID:ISH)
//
#define R_ISH_PCR_PMCTL                   0x1D0                         ///< Power Management
#define R_ISH_PCR_PCICFGCTRL              0x200                         ///< PCI Configuration Control
#define B_ISH_PCR_PCICFGCTR_PCI_IRQ       0x0FF00000                    ///< PCI IRQ number
#define N_ISH_PCR_PCICFGCTR_PCI_IRQ       20
#define B_ISH_PCR_PCICFGCTR_ACPI_IRQ      0x000FF000                    ///< ACPI IRQ number
#define N_ISH_PCR_PCICFGCTR_ACPI_IRQ      12
#define B_ISH_PCR_PCICFGCTR_IPIN1         (BIT11 | BIT10 | BIT9 | BIT8) ///< Interrupt Pin
#define N_ISH_PCR_PCICFGCTR_IPIN1         8
#define B_ISH_PCR_PCICFGCTRL_BAR1DIS      BIT7                          ///< BAR1 Disable

//
// Number of pins used by ISH controllers
//
#define PCH_ISH_PINS_PER_I2C_CONTROLLER               2
#define PCH_ISH_PINS_PER_UART_CONTROLLER              4
#define PCH_ISH_PINS_PER_SPI_CONTROLLER               4

#endif
