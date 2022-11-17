/** @file
  Register names for PCH private chipset register

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

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_PCR_H_
#define _PCH_REGS_PCR_H_

/**
  Definition for SBI PID
  The PCH_SBI_PID defines the PID for PCR MMIO programming and PCH SBI programming as well.
**/
#define PID_OPIPHY     0xAC
#define PID_MODPHY0    0xAB
#define PID_MODPHY1    0xAA
#define PID_USB2       0xCA
#define PID_DMI        0x88
#define PID_PSTH       0x89
#define PID_DSP        0xD7
#define PID_ESPISPI    0x72
#define PID_FIA        0xCF
#define PID_SPF        0x85
#define PID_SPE        0x84
#define PID_SPD        0x83
#define PID_SPC        0x82
#define PID_SPB        0x81
#define PID_SPA        0x80
#define PID_SERIALIO   0xCB
#define PID_LPC        0xC7
#define PID_SMB        0xC6
#define PID_ITSS       0xC4
#define PID_RTC_HOST   0xC3
#define PID_PSF6       0x7F
#define PID_PSF7       0x7E
#define PID_PSF8       0x7D
#define PID_PSF4       0xBD
#define PID_PSF3       0xBC
#define PID_PSF2       0xBB
#define PID_PSF1       0xBA
#define PID_GPIOCOM0   0x6E
#define PID_GPIOCOM1   0x6D
#define PID_GPIOCOM2   0x6C
#define PID_GPIOCOM3   0x6B
#define PID_GPIOCOM4   0x6A
#define PID_CSME12     0x9C

#define PID_CSME0      0x90
#define PID_CSME_PSF   0x8F

#endif
