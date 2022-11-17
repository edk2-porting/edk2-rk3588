/** @file
  Register names for IPU block
  <b>Conventions</b>:
  - Prefixes:
    - Definitions beginning with "R_" are registers
    - Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    - Definitions beginning with "S_" are register sizes
    - Definitions beginning with "N_" are the bit position
  - IPU registers are denoted by "_IPU_" in register names
  - Registers / bits that are different between IPU generations are denoted by
    "_IPU_[generation_name]_" in register/bit names. e.g., "_IPU_TGL_"
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a IPU generation will be just named
    as "_IPU_" without [generation_name] inserted.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _IPU_REGS_H_
#define _IPU_REGS_H_

//
// Device 5 Equates
//
#define IPU_BUS_NUM    0x00
#define IPU_DEV_NUM    0x05
#define IPU_FUN_NUM    0x00

#endif  // _IPU_REGS_H_
