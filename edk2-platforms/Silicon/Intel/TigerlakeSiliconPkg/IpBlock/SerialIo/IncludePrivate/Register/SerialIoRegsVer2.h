/** @file
  Device IDs for Serial IO Controllers for TGL PCH

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
#ifndef _SERIAL_IO_REGS_VER2_H_
#define _SERIAL_IO_REGS_VER2_H_
//
//  Serial IO I2C0 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_I2C0_DEVICE_ID       0xA0E8

//
//  Serial IO I2C1 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_I2C1_DEVICE_ID       0xA0E9

//
//  Serial IO I2C2 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_I2C2_DEVICE_ID       0xA0EA

//
//  Serial IO I2C3 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_I2C3_DEVICE_ID       0xA0EB

//
//  Serial IO I2C4 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_I2C4_DEVICE_ID       0xA0C5

//
//  Serial IO I2C5 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_I2C5_DEVICE_ID       0xA0C6

//
//  Serial IO SPI0 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_SPI0_DEVICE_ID       0xA0AA

//
//  Serial IO SPI1 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_SPI1_DEVICE_ID       0xA0AB

//
//  Serial IO SPI2 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_SPI2_DEVICE_ID       0xA0FB

//
//  Serial IO SPI3 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_SPI3_DEVICE_ID       0xA0FD

//
//  Serial IO UART0 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_UART0_DEVICE_ID      0xA0A8

//
//  Serial IO UART1 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_UART1_DEVICE_ID      0xA0A9

//
//  Serial IO UART2 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_UART2_DEVICE_ID      0xA0C7

//
//  Serial IO UART3 Controller Registers
//
#define V_VER2_PCH_LP_SERIAL_IO_CFG_UART3_DEVICE_ID      0xA0DA

#endif //_SERIAL_IO_REGS_VER2_H_
