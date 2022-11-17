/** @file
  Device IDs for PCH Serial IO Controllers for PCH

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

#ifndef _PCH_REGS_SERIAL_IO_CNL_
#define _PCH_REGS_SERIAL_IO_CNL_

//
//  Serial IO I2C0 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C0            21
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C0          0

#define V_CNL_PCH_LP_SERIAL_IO_CFG_I2C0_DEVICE_ID       0x9DE8
#define V_CNL_PCH_H_SERIAL_IO_CFG_I2C0_DEVICE_ID        0xA368

//
//  Serial IO I2C1 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C1            21
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C1          1

#define V_CNL_PCH_LP_SERIAL_IO_CFG_I2C1_DEVICE_ID       0x9DE9
#define V_CNL_PCH_H_SERIAL_IO_CFG_I2C1_DEVICE_ID        0xA369

//
//  Serial IO I2C2 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C2            21
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C2          2

#define V_CNL_PCH_LP_SERIAL_IO_CFG_I2C2_DEVICE_ID       0x9DEA
#define V_CNL_PCH_H_SERIAL_IO_CFG_I2C2_DEVICE_ID        0xA36A

//
//  Serial IO I2C3 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C3            21
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C3          3

#define V_CNL_PCH_LP_SERIAL_IO_CFG_I2C3_DEVICE_ID       0x9DEB
#define V_CNL_PCH_H_SERIAL_IO_CFG_I2C3_DEVICE_ID        0xA36B

//
//  Serial IO I2C4 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C4            25
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C4          0

#define V_CNL_PCH_LP_SERIAL_IO_CFG_I2C4_DEVICE_ID       0x9DC5

//
//  Serial IO I2C5 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C5            25
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C5          1

#define V_CNL_PCH_LP_SERIAL_IO_CFG_I2C5_DEVICE_ID       0x9DC6

//
//  Serial IO SPI0 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI0            30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI0          2

#define V_CNL_PCH_LP_SERIAL_IO_CFG_SPI0_DEVICE_ID       0x9DAA
#define V_CNL_PCH_H_SERIAL_IO_CFG_SPI0_DEVICE_ID        0xA32A

//
//  Serial IO SPI1 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI1            30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI1          3

#define V_CNL_PCH_LP_SERIAL_IO_CFG_SPI1_DEVICE_ID       0x9DAB
#define V_CNL_PCH_H_SERIAL_IO_CFG_SPI1_DEVICE_ID        0xA32B

//
//  Serial IO UART0 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART0           30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART0         0

#define V_CNL_PCH_LP_SERIAL_IO_CFG_UART0_DEVICE_ID      0x9DA8
#define V_CNL_PCH_H_SERIAL_IO_CFG_UART0_DEVICE_ID       0xA328

//
//  Serial IO UART1 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART1           30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART1         1

#define V_CNL_PCH_LP_SERIAL_IO_CFG_UART1_DEVICE_ID      0x9DA9
#define V_CNL_PCH_H_SERIAL_IO_CFG_UART1_DEVICE_ID       0xA329

//
//  Serial IO UART2 Controller Registers
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2           25
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2         2

#define V_CNL_PCH_LP_SERIAL_IO_CFG_UART2_DEVICE_ID      0x9DC7
#define V_CNL_PCH_H_SERIAL_IO_CFG_UART2_DEVICE_ID       0xA347

#endif
