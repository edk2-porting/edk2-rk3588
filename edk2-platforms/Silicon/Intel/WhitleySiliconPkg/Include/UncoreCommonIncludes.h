/** @file
  This file defines common equates.

  @copyright
  Copyright 2011 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UNCORE_COMMON_INCLUDES_H_
#define _UNCORE_COMMON_INCLUDES_H_

#include <MaxSocket.h>
#include <MaxCore.h>
#include <MaxThread.h>

#ifndef V_INTEL_VID
#define V_INTEL_VID               0x8086
#endif

#define MAX_PROCESSOR_TSEG                     5

// CPX CPU steppings/revisions
#ifndef CPX_A0_CPU_STEP
#define CPX_A0_CPU_STEP  0x0A
#endif //#ifdef CPX_A0_CPU_STEP

#ifndef CPX_A1_CPU_STEP
#define CPX_A1_CPU_STEP  0x0B
#endif //#ifdef CPX_A1_CPU_STEP

//
// ICX SP CPU steppings
//
#ifndef ICXSP_R0_CPU_STEP
#define ICXSP_R0_CPU_STEP  0x00
#endif
#ifndef ICXSP_L0_CPU_STEP
#define ICXSP_L0_CPU_STEP  0x04
#endif
#ifndef ICXSP_C0_CPU_STEP
#define ICXSP_C0_CPU_STEP  0x05
#endif
#ifndef ICXSP_D0_CPU_STEP
#define ICXSP_D0_CPU_STEP  0x06
#endif

#define MAX_DIE                         1
#define MAX_CPU_NUM                     (MAX_THREAD * MAX_CORE * MAX_DIE * MAX_SOCKET)

#ifndef MAX_HA
#define MAX_HA                          2
#endif

// If you change this, please also update MAX_IMC in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in Library\ProcMemInit\Platform\Include\MemDefaults.h
#ifndef MAX_IMC
#define MAX_IMC                       2                     // Maximum memory controllers per socket
#endif

// If you change this, please also update MAX_MC_CH in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in Library\ProcMemInit\Platform\Include\MemDefaults.h
#ifndef MAX_MC_CH
#define MAX_MC_CH                     3                     // Max number of channels per MC (3 for EP)
#endif

// If you change this, please also update MAX_CH in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in Library\ProcMemInit\Platform\Include\MemDefaults.h
#ifndef MAX_CH
#define MAX_CH                        ((MAX_IMC)*(MAX_MC_CH))     // Max channels per socket (worst case EP * EX combination = 16)
#endif

// If you change this, please also update MAX_DIMM in Library\ProcMemInit\Include\MemHostChip.h
#ifndef MAX_DIMM
#define MAX_DIMM                      2                       // Max DIMM per channel
#endif

// If you change this, please also update MC_MAX_NODE in Library\ProcMemInit\Include\MemHostChip.h
#ifndef MC_MAX_NODE
#define MC_MAX_NODE                   (MAX_SOCKET * MAX_IMC)  // Max number of memory nodes
#endif

#ifndef TOTAL_CB3_DEVICES
#define TOTAL_CB3_DEVICES             64   // IOAT_TOTAL_FUNCS * MAX_SOCKET. Note: this covers up to 8S.
#endif

#ifndef MaxIIO
#define MaxIIO                        MAX_SOCKET
#endif

#ifndef TOTAL_IIO_STACKS
#define TOTAL_IIO_STACKS              48   // MAX_SOCKET * MAX_IIO_STACK. Not reflect architecture but only sysHost structure!
#endif

#ifndef NUMBER_NTB_PORTS_PER_SOCKET
#define NUMBER_NTB_PORTS_PER_SOCKET     5
#endif  // #ifndef NUMBER_NTB_PORTS_PER_SOCKET

#ifndef MAX_DEVHIDE_REGS_PER_SYSTEM
  #if MaxIIO > 4
    #define MAX_DEVHIDE_REGS_PER_SYSTEM                  512 // MAX_DEVHIDE_REGS_PER_SOCKET * MaxIIO
  #else
    #define MAX_DEVHIDE_REGS_PER_SYSTEM                  256 // MAX_DEVHIDE_REGS_PER_SOCKET * MaxIIO
  #endif
#endif

#ifndef MAX_B2P_MAILBOX_GROUPS
#define MAX_B2P_MAILBOX_GROUPS    32
#endif // !MAX_B2P_MAILBOX_GROUPS

#endif
