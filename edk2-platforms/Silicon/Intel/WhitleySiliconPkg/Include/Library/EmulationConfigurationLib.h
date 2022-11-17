/** @file

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _EMULATION_CONFIGURATION_LIB_H_
#define _EMULATION_CONFIGURATION_LIB_H_

#include <Uefi.h>

#pragma pack(1)

typedef struct {
  UINT8   UbiosGenerationSetting;                        // indicate if uBIOS generation is enable or not in setup menu
  UINT8   HybridSystemLevelEmulationSetting;             // indicate if HSLE is enable or not in setup menu
  UINT8   UbiosOutputMode;                               // indicate if we should output register writes to the serial port
  UINT32  LoopBackLabelNumber;                           // used to create ASM reads for emulation
  UINT8   FnvAccessValue;                                // indicate if FNV access enable or not
  UINT8   MsrTraceEnable;                                // indicate if Mrs Trace is enable or not in setup menu
  UINT8   MsrTraceOutputMode;                            // indicate if Mrs Trace Asm Output Mode
} EMULATION_SETTING;

#pragma pack()

#define ASM_OUTPUT_ENABLE           BIT0                 // flag to indicate ASM output is enabled

#define FNV_ACCESS_DISABLE          0                    // flag to indicate FNV access is disabled

#define MSR_OUTPUT_DISABLE          0                    // flag to indicate Mrs Trace Asm Output is disabled

#endif // _EMULATION_CONFIGURATION_LIB_H_
