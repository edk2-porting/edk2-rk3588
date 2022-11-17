/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
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
#define PCH_PCR_ADDRESS(Pid, Offset)    (PCH_PCR_BASE_ADDRESS | ((UINT8)(Pid) << 16) | (UINT16)(Offset))

/**
  Definition for SBI PID
  The PCH_SBI_PID defines the PID for PCR MMIO programming and PCH SBI programming as well.
**/
typedef enum {
  PID_DMI                               = 0xEF,
  PID_ESPISPI                           = 0xEE,
  PID_MODPHY1                             = 0xE9,
  PID_OTG                               = 0xE5,
  PID_SPE                               = 0xE4,        // Reserved in SKL PCH LP
  PID_SPD                               = 0xE3,        // Reserved in SKL PCH LP
  PID_SPC                               = 0xE2,
  PID_SPB                               = 0xE1,
  PID_SPA                               = 0xE0,
  PID_ICC                               = 0xDC,
  PID_DSP                               = 0xD7,
  PID_FIA                               = 0xCF,
  PID_FIAWM26                           = 0x13,
  PID_USB2                              = 0xCA,
  PID_LPC                               = 0xC7,
  PID_SMB                               = 0xC6,
  PID_ITSS                              = 0xC4,
  PID_RTC                               = 0xC3,
  PID_PSF4                              = 0xBD,
  PID_PSF3                              = 0xBC,
  PID_PSF2                              = 0xBB,
  PID_PSF1                              = 0xBA,
  PID_DCI                               = 0xB8,
  PID_MMP0                              = 0xB0,
  PID_GPIOCOM0                          = 0xAF,
  PID_GPIOCOM1                          = 0xAE,
  PID_GPIOCOM2                          = 0xAD,
  PID_GPIOCOM3                          = 0xAC,
  PID_GPIOCOM4                          = 0xAB,
  PID_GPIOCOM5                          = 0x11,
  PID_MODPHY2                             = 0xA9,
  PID_MODPHY3                             = 0xA8,
  PID_CSME0                             = 0x90, // CSE
  PID_CSME_PSF                          = 0x8F, // ME PSF
  PID_PSTH                              = 0x89
} PCH_SBI_PID;

#endif
