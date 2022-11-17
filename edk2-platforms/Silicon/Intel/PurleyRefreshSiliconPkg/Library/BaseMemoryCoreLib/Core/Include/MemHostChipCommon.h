/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _memhostchipcommon_h
#define _memhostchipcommon_h

#include "SysHostChipCommon.h"

#define NODE_TO_SKT(node)       (node / MAX_IMC)
#define SKT_TO_NODE(socket, mc) ((socket << 1) | (mc & BIT0))
#define NODE_TO_MC(node)        (node % MAX_IMC)
#define SKTMC_TO_NODE(socket, mc)  ((socket * MAX_IMC) | (mc % MAX_IMC))
#define NODECH_TO_SKTCH(node, ch)    (((node % MAX_IMC)*MAX_MC_CH) + ch)

//

//
//MAX_CHANNELS and DYNVAR_MAX were previously defined in MmrcProjectDefinitionsGenerated.h, but
// now are here and must be manually updated as needed depending on MMRC tool execution (they have been
// removed from automatic generation by the tool)
//
// Channels
//
#define MAX_CHANNELS                                      6

#define DYNVAR_MAX                                        51

#define MAX_IMC             2

#define MAX_MC_CH           3                               // Max channels per MC
#define MAX_CH              ((MAX_IMC)*(MAX_MC_CH))         // Max channels per socket
#define MC_MAX_NODE         (MAX_SOCKET * MAX_IMC)  // Max number of memory nodes
#define MAX_DIMM            2                       // Max DIMM per channel

#define MAX_DIMM3           3                   // Max DIMM per channel
#define MAX_TECH            19                  // Number of entries in DRAM technology table

#define MAX_RIR             4                   // Number of Rank Interleave Register rules for DDR
#define MAX_RIR_DDRT        4                   // Number of Rank Interleave Register rules for NVMDIMM
#define MAX_RIR_WAYS        8                   // Number of interleave ways for RIR for DDR
#define TAD_RULES           8                   // Number of TAD rule registers
#define MAX_TAD_WAYS        3                   // Number of interleave ways for TAD RULES
#define SAD_RULES          24                   // Number of SAD rule registers
#define MAX_SAD_RULES      24                   // Number of SAD rule registers
#define MAX_STROBE          18                  // Number of strobe groups
#define MAX_SEEDS           10                  // Maximum
#if   QR_DIMM_SUPPORT
#define MAX_RANK_DIMM       4                   // Max ranks per DIMM
#else
#define MAX_RANK_DIMM       2                   // Max ranks per DIMM
#endif
#define MAX_RANK_CH         8                   // Max ranks per channel
#define MAX_SPARE_RANK      2                   // Max number of spare ranks in a channel
#define SPD_MODULE_PART     18                  // Number of bytes of module part - DDR3
#define SPD_MODULE_PART_DDR4 20                 // Number of bytes of module part - DDR4
#define SAD_RULES_ADDR_RANGE  4                 // Max IOT rules = 4, Total address limits (lower(4) entries each)
// Ctl FUBs
#define NUM_CTL_PLATFORM_GROUPS 4

// SPD Defines
//-----------------------------------------------------------------------------

#pragma pack(1)

//
// Define the WDB line. The WDB line is like the cache line.
//
#define MRC_WDB_LINES                 32
#define MRC_WDB_LINE_SIZE             64
#define CADB_LINES                    16
// Define in the Critical Section function on what to wait.
//
typedef enum {
  DoneAndRefDrained,
  Done,
  Immediate
} EWaitOn;

typedef enum {
  ssOne             = 0,
  ssTwo,
  ssThree,
  ssFour,
} TSubSequencesNumber;

#define MAX_PHASE_IN_FINE_ADJUSTMENT    64
#pragma pack()

#define SKX_PCKG_TYPE 4 //CMD_CTL_DELAY_H

#define CHIP_IOGPDLY_PSECS SKX_PCKG_TYPE

//
// Chip specific section of the struct CPU_CSR_ACCESS_VAR
//
#define CPU_CSR_ACCESS_VAR_CHIP   \


typedef enum {
    TYPE_SCF_BAR = 0,
    TYPE_PCU_BAR,
    TYPE_MEM_BAR0,
    TYPE_MEM_BAR1,
    TYPE_MEM_BAR2,
    TYPE_MEM_BAR3,
    TYPE_MEM_BAR4,
    TYPE_MEM_BAR5,
    TYPE_MEM_BAR6,
    TYPE_MEM_BAR7,
    TYPE_SBREG_BAR,
    TYPE_MAX_MMIO_BAR
} MMIO_BARS;


// Output structures based on scope
#define  MAX_BITS    72

#endif // _memhostchipcommon_h
