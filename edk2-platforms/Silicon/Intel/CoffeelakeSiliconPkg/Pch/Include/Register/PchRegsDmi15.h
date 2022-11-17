/** @file
  Register names for DMI and OP-DMI

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

#ifndef _PCH_REGS_DMI15_H_
#define _PCH_REGS_DMI15_H_

#define R_PCH_DMI15_PCR_MPC                 0x20D8                       ///< Miscellaneous Port Configuration
#define B_PCH_DMI15_PCR_MPC_SRL             BIT23                        ///< Secured register lock
#define R_PCH_DMI15_PCR_V0CTL               0x2284                       ///< Virtual channel 0 resource control
#define R_PCH_DMI15_PCR_V0STS               0x228A                       ///< Virtual channel 0 status

#define R_PCH_DMI15_PCR_V1CTL               0x2290                       ///< Virtual channel 1 resource control
#define R_PCH_DMI15_PCR_V1STS               0x2296                       ///< Virtual channel 1 status

#define R_PCH_DMI15_PCR_VMCTL               0x22B0                       ///< ME Virtual Channel (VCm) resource control

#define R_PCH_DMI15_PCR_UPHWAWC             0x249C                       ///< Upstream Port HW Autonomous Width Control
#define B_PCH_DMI15_PCR_UPHWAWC_TS3TW       (BIT15 | BIT14 | BIT13)      ///< Thermal Sensor 3 Target Width
#define N_PCH_DMI15_PCR_UPHWAWC_TS3TW       13                           ///< Thermal Sensor 3 Target Width
#define B_PCH_DMI15_PCR_UPHWAWC_TS2TW       (BIT12 | BIT11 | BIT10)      ///< Thermal Sensor 2 Target Width
#define N_PCH_DMI15_PCR_UPHWAWC_TS2TW       10                           ///< Thermal Sensor 2 Target Width
#define B_PCH_DMI15_PCR_UPHWAWC_TS1TW       (BIT9 | BIT8 | BIT7)         ///< Thermal Sensor 1 Target Width
#define N_PCH_DMI15_PCR_UPHWAWC_TS1TW       7                            ///< Thermal Sensor 1 Target Width
#define B_PCH_DMI15_PCR_UPHWAWC_TS0TW       (BIT6 | BIT5 | BIT4)         ///< Thermal Sensor 0 Target Width
#define N_PCH_DMI15_PCR_UPHWAWC_TS0TW       4                            ///< Thermal Sensor 0 Target Width
#define B_PCH_DMI15_PCR_UPHWAWC_TSAWEN      BIT0                         ///< Thermal Sensor Autonomous Width Enable

#endif
