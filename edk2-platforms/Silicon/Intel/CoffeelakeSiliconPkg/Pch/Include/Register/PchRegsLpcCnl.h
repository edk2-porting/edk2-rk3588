/** @file
  Register names for PCH LPC/eSPI device

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

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_LPC_CNL_H_
#define _PCH_REGS_LPC_CNL_H_

#define V_LPC_CFG_DID_CNL_H                       0xA300
#define V_LPC_CFG_DID_CNL_LP                      0x9D80
#define V_LPC_CFG_DID_CML_LP                      0x0280

//
// PCH-LP Device IDs
//
#define V_CNL_PCH_LP_LPC_CFG_DEVICE_ID_MB_SUPER_SKU   0x9D80          ///< PCH LP Mobile
#define V_CNL_PCH_LP_LPC_CFG_DEVICE_ID_MB_0           0x9D81          ///< PCH LP Mobile (U)
#define V_CNL_PCH_LP_LPC_CFG_DEVICE_ID_MB_1           0x9D82          ///< PCH LP Mobile Locked
#define V_CNL_PCH_LP_LPC_CFG_DEVICE_ID_MB_2           0x9D83          ///< PCH LP Mobile (Y)
#define V_CNL_PCH_LP_LPC_CFG_DEVICE_ID_MB_3           0x9D84          ///< PCH LP Mobile (U)
#define V_CNL_PCH_LP_LPC_CFG_DEVICE_ID_MB_4           0x9D85          ///< PCH LP Mobile (U)
#define V_CNL_PCH_LP_LPC_CFG_DEVICE_ID_MB_5           0x9D86          ///< PCH LP Mobile (Y)

//
// PCH-H Desktop LPC Device IDs
//
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A300_SKU        0xA300          ///< LPC/eSPI Controller
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A303_SKU        0xA303          ///< PCH H Mobile H310
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A304_SKU        0xA304          ///< PCH H Mobile H370
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A305_SKU        0xA305          ///< PCH H Mobile Z390
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A306_SKU        0xA306          ///< PCH H Mobile Q370
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A309_SKU        0xA309          ///< PCH H Mobile C246
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A30A_SKU        0xA30A          ///< PCH H Mobile C242
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A30B_SKU        0xA30B          ///< PCH H Mobile X399
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A30C_SKU        0xA30C          ///< PCH H Mobile QM370
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A30D_SKU        0xA30D          ///< PCH H Mobile HM370
#define V_CNL_PCH_H_LPC_CFG_DEVICE_ID_A30E_SKU        0xA30E          ///< PCH H Mobile CM246

#endif
