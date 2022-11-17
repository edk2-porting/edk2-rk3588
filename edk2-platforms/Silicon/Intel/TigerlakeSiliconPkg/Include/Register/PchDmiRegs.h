/** @file
  Register names for PCH DMI and OP-DMI

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_DMI_REGS_H_
#define _PCH_DMI_REGS_H_

//
// PCH DMI Chipset Configuration Registers (PID:DMI)
//

//
// PCH DMI Source Decode PCRs (Common)
//
#define R_PCH_DMI_PCR_LPCLGIR1                   0x2730                       ///< LPC Generic I/O Range 1
#define R_PCH_DMI_PCR_LPCGMR                     0x2740                       ///< LPC Generic Memory Range
#define R_PCH_DMI_PCR_SEGIR                      0x27BC                       ///< Second ESPI Generic I/O Range
#define R_PCH_DMI_PCR_SEGMR                      0x27C0                       ///< Second ESPI Generic Memory Range
#define R_PCH_DMI_PCR_LPCBDE                     0x2744                       ///< LPC BIOS Decode Enable
#define R_PCH_DMI_PCR_UCPR                       0x2748                       ///< uCode Patch Region
#define B_PCH_DMI_PCR_UCPR_UPRE                  BIT0                         ///< uCode Patch Region Enable
#define R_PCH_DMI_PCR_GCS                        0x274C                       ///< Generic Control and Status
#define B_PCH_DMI_PCR_BBS                        BIT10                        ///< Boot BIOS Strap
#define B_PCH_DMI_PCR_RPR                        BIT11                        ///< Reserved Page Route
#define B_PCH_DMI_PCR_BILD                       BIT0                         ///< BIOS Interface Lock-Down
#define R_PCH_DMI_PCR_IOT1                       0x2750                       ///< I/O Trap Register 1
#define R_PCH_DMI_PCR_LPCIOD                     0x2770                       ///< LPC I/O Decode Ranges
#define R_PCH_DMI_PCR_LPCIOE                     0x2774                       ///< LPC I/O Enables
#define R_PCH_DMI_PCR_TCOBASE                    0x2778                       ///< TCO Base Address
#define B_PCH_DMI_PCR_TCOBASE_TCOBA              0xFFE0                       ///< TCO Base Address Mask
#define R_PCH_DMI_PCR_GPMR1                      0x277C                       ///< General Purpose Memory Range 1
#define R_PCH_DMI_PCR_GPMR1DID                   0x2780                       ///< General Purpose Memory Range 1 Destination ID

#endif
