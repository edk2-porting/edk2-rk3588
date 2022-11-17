/** @file

  Copyright 2018-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef SOC_H__
#define SOC_H__

#include <Chassis.h>

/**
  Soc Memory Map
**/
#define LX2160A_DRAM0_PHYS_ADDRESS   (BASE_2GB)
#define LX2160A_DRAM0_SIZE           (SIZE_2GB)
#define LX2160A_DRAM1_PHYS_ADDRESS   (BASE_128GB + BASE_2GB)
#define LX2160A_DRAM1_SIZE           (SIZE_128GB - SIZE_2GB) // 126 GB
#define LX2160A_DRAM2_PHYS_ADDRESS   (BASE_256GB + BASE_128GB)
#define LX2160A_DRAM2_SIZE           (SIZE_128GB)

#define LX2160A_CCSR_PHYS_ADDRESS    (BASE_16MB)
#define LX2160A_CCSR_SIZE            (SIZE_256MB - SIZE_16MB) // 240MB

#define LX2160A_FSPI0_PHYS_ADDRESS   (BASE_512MB)
#define LX2160A_FSPI0_SIZE           (SIZE_256MB)

#define LX2160A_PCI1_PHYS_ADDRESS    FixedPcdGet64 (PcdPciExp1BaseAddr)
#define LX2160A_PCI_SIZE             SIZE_32GB

#define LX2160A_DCFG_ADDRESS         NXP_LAYERSCAPE_CHASSIS3V2_DCFG_ADDRESS

// SVR
#define SVR_LX2160A                  0x873600

/**
  Reset Control Word (RCW) Bits

  RCWSR contains the Reset Configuration Word (RCW) information written with
  values read from flash memory by the device at power-on reset and read-only
  upon exiting reset.

  RCW bits in RCWSR registers are same as bit position in Little Endian (LE)

RCW Bits |
in RCWSR |
(LSBit 0)| 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
------------------------------------------------------------------------------------------------
LE       | 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
(LSBit 0)|

  Moreover the RCW bits are to be interpreted in below fasion

Bit(s) | Field Name  | Description                  | Notes/comments
----------------------------------------------------------------------
 6-2   | SYS_PLL_RAT | System PLL Multiplier/Ratio  | This field selects the platform
       |             |                              | clock:SYSCLK ratio.
       |             |                              | 0_0100 4:1
       |             |                              | 0_0110 6:1
       |             |                              | 0_1000 8:1
       |             |                              | 0_1101 13:1
       |             |                              | 0_1111 15:1

**/
#define SYS_PLL_RAT(x)  (((x) & 0x7c) >> 2) // Bits 6-2

typedef NXP_LAYERSCAPE_CHASSIS3V2_DEVICE_CONFIG LX2160A_DEVICE_CONFIG;

#endif // SOC_H__
