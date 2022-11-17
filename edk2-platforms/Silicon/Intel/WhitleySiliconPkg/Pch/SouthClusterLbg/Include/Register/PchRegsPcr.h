/** @file
  Register names for PCH private chipset register
  Conventions:
  Prefixes:
  Definitions beginning with "R_" are registers
  Definitions beginning with "B_" are bits within registers
  Definitions beginning with "V_" are meaningful values within the bits
  Definitions beginning with "S_" are register sizes
  Definitions beginning with "N_" are the bit position
  In general, PCH registers are denoted by "_PCH_" in register names
  Registers / bits that are different between PCH generations are denoted by
  _PCH_[generation_name]_" in register/bit names.
  Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
  Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
  e.g., "_PCH_H_", "_PCH_LP_"
  Registers / bits names without _H_ or _LP_ apply for both H and LP.
  Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
  at the end of the register/bit names
  Registers / bits of new devices introduced in a PCH generation will be just named
  as "_PCH_" without [generation_name] inserted.

  @copyright
  Copyright 2013 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_PCR_H_
#define _PCH_REGS_PCR_H_

///
/// Definition for PCR base address (defined in PchReservedResources.h)
///
//#define PCH_PCR_BASE_ADDRESS            0xFD000000
//#define PCH_PCR_MMIO_SIZE               0x01000000
/**
  Definition for PCR address
  The PCR address is used to the PCR MMIO programming
**/
#define PCH_PCR_ADDRESS(Pid, Offset)                        (PCH_PCR_BASE_ADDRESS | ((UINT8) (Pid) << 16) | (UINT16) (Offset))
#define PCH_PCR_ADDRESS_BASE(PcrBaseAddress, Pid, Offset)   ((UINTN) (PcrBaseAddress) | ((UINT8) (Pid) << 16) | (UINT16) (Offset))

/**
  Definition for SBI PID
  The PCH_SBI_PID defines the PID for PCR MMIO programming and PCH SBI programming as well.
**/

#define  PID_BROADCAST1                         0xFF
#define  PID_BROADCAST2                         0xFE
//Rsv                                = 0xFD-0xF0,
#define  PID_DMI                                0xEF
#define  PID_ESPISPI                            0xEE
#define  PID_ICLK                               0xED
#define  PID_MODPHY4                            0xEB
#define  PID_MODPHY5                            0x10
#define  PID_MODPHY1                            0xE9
#define  PID_PMC                                0xE8
//Rsv                                = 0xE7,
#define  PID_XHCI                               0xE6
#define  PID_OTG                                0xE5
#define  PID_SPE                                0xE4       // Reserved in SKL PCH LP
#define  PID_SPD                                0xE3       // Reserved in SKL PCH LP
#define  PID_SPC                                0xE2
#define  PID_SPB                                0xE1
#define  PID_SPA                                0xE0
#define  PID_UPSX8                              0x06
#define  PID_UPSX16                             0x07
#define  PID_TAP2IOSFSB1                        0xDF
#define  PID_TRSB                               0xDD
#define  PID_ICC                                0xDC
#define  PID_GBE                                0xDB
//Rsv                                = 0xDA,
#define  PID_SATA                               0xD9
#define  PID_SSATA                              0x0F
#define  PID_LDO                                0x14
//Rsv                                = 0xD8,
#define  PID_DSP                                0xD7
//Rsv                                = 0xD6,
#define  PID_FUSE                               0xD5
#define  PID_FSPROX0                            0xD4
#define  PID_DRNG                               0xD2
//Rsv                                = 0xD1,
#define  PID_FIA                                0xCF
#define  PID_FIAWM26                            0x13
//Rsv                                = 0xCE-0xCC,
#define  PID_USB2                               0xCA
//Rsv                                = 0xC8
#define  PID_LPC                                0xC7
#define  PID_SMB                                0xC6
#define  PID_P2S                                0xC5
#define  PID_ITSS                               0xC4
#define  PID_RTC_HOST                           0xC3
//Rsv                                 = 0xC2-0xC1,
#define  PID_PSF5                               0x8F
#define  PID_PSF6                               0x70
#define  PID_PSF7                               0x01
#define  PID_PSF8                               0x29
#define  PID_PSF9                               0x21
#define  PID_PSF10                              0x36
#define  PID_PSF4                               0xBD
#define  PID_PSF3                               0xBC
#define  PID_PSF2                               0xBB
#define  PID_PSF1                               0xBA
#define  PID_HOTHARM                            0xB9
#define  PID_DCI                                0xB8
#define  PID_DFXAGG                             0xB7
#define  PID_NPK                                0xB6
//Rsv                                = 0xB5-0xB1,
#define  PID_MMP0                               0xB0
#define  PID_GPIOCOM0                           0xAF
#define  PID_GPIOCOM1                           0xAE
#define  PID_GPIOCOM2                           0xAD
#define  PID_GPIOCOM3                           0xAC
#define  PID_GPIOCOM4                           0xAB
#define  PID_GPIOCOM5                           0x11
#define  PID_MODPHY2                            0xA9
#define  PID_MODPHY3                            0xA8
//Rsv                                = 0xA7-0xA6,
#define  PID_PNCRC                              0xA5
#define  PID_PNCRB                              0xA4
#define  PID_PNCRA                              0xA3
#define  PID_PNCR0                              0xA2
#define  PID_CSME15                             0x9F // SMS2
#define  PID_CSME14                             0x9E // SMS1
#define  PID_CSME13                             0x9D // PMT
#define  PID_CSME12                             0x9C // PTIO
#define  PID_CSME11                             0x9B // PECI
#define  PID_CSME9                              0x99 // SMT6
#define  PID_CSME8                              0x98 // SMT5
#define  PID_CSME7                              0x97 // SMT4
#define  PID_CSME6                              0x96 // SMT3
#define  PID_CSME5                              0x95 // SMT2
#define  PID_CSME4                              0x94 // SMT1 (SMBus Message Transport 1)
#define  PID_CSME3                              0x93 // FSC
#define  PID_CSME2                              0x92 // USB-R SAI
#define  PID_CSME0                              0x90 // CSE
#define  PID_CSME_PSF                           0x8F // ME PSF
//Rsv                                 = 0x88-0x30,
//#define PID_EVA                              0x2F-0x00
#define  PID_CSMERTC                           0x8E
#define  PID_IEUART                            0x80
#define  PID_IEHOTHAM                          0x7F
#define  PID_IEPMT                             0x7E
#define  PID_IESSTPECI                         0x7D
#define  PID_IEFSC                             0x7C
#define  PID_IESMT5                            0x7B
#define  PID_IESMT4                            0x7A
#define  PID_IESMT3                            0x79
#define  PID_IESMT2                            0x78
#define  PID_IESMT1                            0x77
#define  PID_IESMT0                            0x76
#define  PID_IEUSBR                            0x74
#define  PID_IEPTIO                            0x73
#define  PID_IEIOSFGASKET                      0x72
#define  PID_IEPSF                             0x70
#define  PID_FPK                               0x0A
#define  PID_MP0KR                             0x3C
#define  PID_MP1KR                             0x3E
#define  PID_RUAUX                             0x0B
#define  PID_RUMAIN                            0x3B
#define  PID_EC                                0x20
#define  PID_CPM2                              0x38
#define  PID_CPM1                              0x37
#define  PID_CPM0                              0x0C
#define  PID_VSPTHERM                          0x25
#define  PID_VSPP2SB                           0x24
#define  PID_VSPFPK                            0x22
#define  PID_VSPCPM2                           0x35
#define  PID_VSPCPM1                           0x34
#define  PID_VSPCPM0                           0x33
#define  PID_MSMROM                            0x08
#define  PID_PSTH                              0x89

typedef  UINT8                            PCH_SBI_PID;


#endif
