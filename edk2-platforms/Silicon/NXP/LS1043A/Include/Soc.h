/** @file

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef SOC_H__
#define SOC_H__

#include <Chassis.h>

/**
  Soc Memory Map
**/
#define LS1043A_DRAM0_PHYS_ADDRESS   0x80000000
#define LS1043A_DRAM0_SIZE           SIZE_2GB
#define LS1043A_DRAM1_PHYS_ADDRESS   0x880000000
#define LS1043A_DRAM1_SIZE           0x780000000 // 30 GB

#define LS1043A_CCSR_PHYS_ADDRESS    0x1000000
#define LS1043A_CCSR_SIZE            0xF000000

#define LS1043A_IFC0_PHYS_ADDRESS    0x60000000
#define LS1043A_IFC0_SIZE            SIZE_512MB
#define LS1043A_IFC1_PHYS_ADDRESS    0x620000000
#define LS1043A_IFC1_SIZE            0xE0000000 // 3.5 GB

#define LS1043A_QSPI_PHYS_ADDRESS    0x40000000
#define LS1043A_QSPI_SIZE            SIZE_512MB

#define LS1043A_QMAN_SW_PORTAL_PHYS_ADDRESS    0x500000000
#define LS1043A_QMAN_SW_PORTAL_SIZE            SIZE_128MB
#define LS1043A_BMAN_SW_PORTAL_PHYS_ADDRESS    0x508000000
#define LS1043A_BMAN_SW_PORTAL_SIZE            SIZE_128MB

#define LS1043A_PCI0_PHYS_ADDRESS    0x4000000000
#define LS1043A_PCI1_PHYS_ADDRESS    0x4800000000
#define LS1043A_PCI2_PHYS_ADDRESS    0x5000000000
#define LS1043A_PCI_SIZE             SIZE_32GB

#define LS1043A_I2C0_PHYS_ADDRESS    0x2180000
#define LS1043A_I2C_SIZE             0x10000
#define LS1043A_I2C_NUM_CONTROLLERS  4

#define LS1043A_DCFG_ADDRESS         NXP_LAYERSCAPE_CHASSIS2_DCFG_ADDRESS

//SVR
#define SVR_LS1043A                 0x879200

/**
  Reset Control Word (RCW) Bits

  RCWSR contains the Reset Configuration Word (RCW) information written with
  values read from flash memory by the device at power-on reset and read-only
  upon exiting reset.

  RCW bits in RCWSR registers are mirror of bit position in Little Endian (LE)

RCW Bits |
in RCWSR |
(MSBit 0)| 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
------------------------------------------------------------------------------------------------
LE       | 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
(LSBit 0)|

  Moreover the RCW bits are to be interpreted in below fasion

Bit(s) | Field Name  | Description                  | Notes/comments
----------------------------------------------------------------------
 2-6   | SYS_PLL_RAT | System PLL Multiplier/Ratio  | This field selects the platform
       |             |                              | clock:SYSCLK ratio.
       |             |                              | 0_0011 3:1
       |             |                              | 0_0100 4:1
       |             |                              | 0_1101 13:1
       |             |                              | 0_1111 15:1
       |             |                              | 1_0000 16:1

  which is why the RCW bits in RCWSR registers are parsed this way
**/
#define SYS_PLL_RAT(x)  (((x) >> 25) & 0x1f) // Bits 2-6

typedef NXP_LAYERSCAPE_CHASSIS2_DEVICE_CONFIG LS1043A_DEVICE_CONFIG;

#endif // SOC_H__
