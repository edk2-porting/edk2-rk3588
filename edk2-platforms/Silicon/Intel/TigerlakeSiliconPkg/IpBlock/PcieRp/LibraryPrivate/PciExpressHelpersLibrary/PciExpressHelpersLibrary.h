/** @file
  Header file for Pci Express helps library implementation.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCI_EXPRESS_HELPERS_LIBRARY_H_
#define _PCI_EXPRESS_HELPERS_LIBRARY_H_

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Pci.h>
#include <PchPolicyCommon.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/TimerLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/PcieRpLib.h>
#include <PcieRegs.h>
#include <Register/CpuPcieRegs.h>
#include <Register/PchPcieRpRegs.h>

#define LTR_VALUE_MASK (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7 + BIT8 + BIT9)
#define LTR_SCALE_MASK (BIT10 + BIT11 + BIT12)

  #define CONFIG_WRITE_LOOP_COUNT   100000

//
// LTR related macros
//
#define LTR_LATENCY_VALUE(x)           ((x) & LTR_VALUE_MASK)
#define LTR_SCALE_VALUE(x)             (((x) & LTR_SCALE_MASK) >> 10)
#define LTR_LATENCY_NS(x)              (LTR_LATENCY_VALUE(x) * (1 << (5 * LTR_SCALE_VALUE(x))))

#endif
