/** @file

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPUID_REGS_H_
#define _CPUID_REGS_H_

#define CPU_FAMILY_SKX     0x5065      // Skylake/CascadeLake CPU
#define CPU_FAMILY_ICX     0x606A      // IceLake CPU
#define EFI_CACHE_UNCACHEABLE                 0
#define EFI_CACHE_WRITEBACK                   6

#define APICID_MASK_BIT14_8 0x7F    //current Si support programmable APICID up to 15bits
#endif  //_CPUID_REGS_H_
