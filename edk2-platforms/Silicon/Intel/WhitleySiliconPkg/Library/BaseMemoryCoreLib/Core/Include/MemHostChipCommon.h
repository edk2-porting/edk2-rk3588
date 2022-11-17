/** @file

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _memhostchipcommon_h
#define _memhostchipcommon_h


#include "SysHostChipCommon.h"
#include "NGNDimmPlatformCfgData.h"
#include <MemCommon.h>
#include <Library/MemTypeLib.h>

#define MAX_MEM_SS          8                   // Max Memory Subsystems per socket
#define MAX_CLUSTERS        4                   // Maximum number of clusters supported

#define MC_MAX_NODE                    (MAX_SOCKET * MAX_IMC)    // Max number of memory nodes
#define MAX_DIMM                       2                         // Max DIMM per channel
#define MAX_DDRT_DIMM_PER_CH           1                         // Max DDRT DIMM per channel
#define MAX_UNIQUE_NGN_DIMM_INTERLEAVE 2                         // Max number of unique interleaves for NGN DIMM

#define  MAX_BITS    72 // Max number of data bits per rank
#define MAX_TECH            19                  // Number of entries in DRAM technology table
#define MAX_TECH_DDRT        8

#define TAD_RULES           8                   // Number of near memory TAD rule registers
#define FM_TAD_RULES        12                  // Number of far memory TAD rule registers
#define FM_TAD_RULES_10NM   4                   // Number of far memory only TAD rule registers in 10nm
#define MAX_TAD_RULES       (TAD_RULES + FM_TAD_RULES)      // Number of combined near and far TAD rules
#define MAX_TAD_RULES_10NM  (TAD_RULES + FM_TAD_RULES_10NM) // Number of combined near and far TAD rules in 10nm
#define MAX_TAD_WAYS        3                   // Number of interleave ways for TAD RULES
#define MAX_RT_WAYS         8                   // Max. interleave ways for DDR/DDRT RTs in 256B McChan granularity
#define MAX_RT              2                   // Number of RTs per route table type
#define MAX_FPGA_REMOTE_SAD_RULES         2     // Maximum FPGA sockets exists on ICX platform

#define MAX_STROBE          18                  // Number of strobe groups

#define MAX_RANK_DIMM_3DS   2                   // Max physical CS ranks per 3DS DIMM
#define MAX_SUBRANK_3DS     4                   // Max logical C[2:0] subranks per CS in 3DS DIMM
#define MAX_SPARE_RANK      2                   // Max number of spare ranks in a channel
#define MAX_SUBRANK         2                   // Max subranks per logical rank
#define SPD_MODULE_PART_DDR4 20                 // Number of bytes of module part - DDR4
#define SPD_MODULE_SERIAL    4                  // Number of bytes of Module Serial Number
#define MAX_PARTIAL_MIRROR      4               //Maximum number of partial mirror regions that can be created

#define CONVERT_64MB_TO_4KB_GRAN    14
#define CONVERT_4KB_TO_64MB_GRAN    14
#define CONVERT_64MB_TO_GB_GRAN     4
#define CONVERT_GB_TO_64MB_GRAN     4
#define CONVERT_64MB_TO_MB_GRAN     6
#define CONVERT_MB_TO_64MB_GRAN     6
#define CONVERT_64MB_TO_4GB_GRAN    6
#define CONVERT_4GB_TO_64MB_GRAN    6
#define CONVERT_64MB_TO_32GB_GRAN   9
#define CONVERT_64B_TO_64MB         20
#define CONVERT_B_TO_MB             20
#define CONVERT_MB_TO_B             20
#define CONVERT_B_TO_64MB           26
#define CONVERT_64MB_TO_B           26
#define CONVERT_64MB_TO_128MB_GRAN  1
#define CONVERT_256MB_TO_64MB_GRAN  2
#define CONVERT_64MB_TO_256MB_GRAN  2
#define CONVERT_B_TO_256MB_GRAN     28

#define MEM_1GB_AT_64MB_GRAN        0x10
#define MEM_1GB_AT_4KB_GRAN         0x40000

#define GB_TO_MB_CONVERSION 1024

#define BITMAP_CH0_CH1_CH2  ( ( BIT0 ) | (BIT1 ) | (BIT2) )
#define BITMAP_CH0_CH1      ( ( BIT0 ) | (BIT1 ) )
#define BITMAP_CH1_CH2      ( ( BIT1 ) | (BIT2 ) )
#define BITMAP_CH0_CH2      ( ( BIT0 ) | (BIT2 ) )
#define BITMAP_CH0       BIT0
#define BITMAP_CH1       BIT1
#define BITMAP_CH2       BIT2

#define CONVERT_64MB_TO_BYTE              64 * 1024 * 1024

//
// Define the WDB line. The WDB line is like the cache line.
//
#define MRC_WDB_LINES                 32
#define MRC_WDB_LINE_SIZE             64

#define MAX_PHASE_IN_FINE_ADJUSTMENT         64
#define MAX_PHASE_IN_READ_ADJ_DQ_RX_DFE      152 // larger range for added DQ 1/16 PI adjustments

#pragma pack(1)
typedef struct TADTable {
  UINT8   Enable;             // Rule enable
  UINT8   SADId;              // SAD Index
  UINT8   socketWays;         // Socket Interleave ways for TAD
  UINT8   NmTadIndex;         // Index of near memory TAD
  UINT8   FmTadIndex;         // Index of far memory TAD
  UINT32  Limit;              // Limit of the current TAD entry
  UINT8   TargetGran;         // MC granularity of 1LM forward and 2LM forward/reverse address decoding.
  UINT8   ChGran;             // Channel granularity of 1LM forward and 2LM forward/reverse address decoding.
} TAD_TABLE;

typedef struct SADTable {
  UINT8    Enable;         // Rule enable
  MEM_TYPE type;           // Bit map of memory region types, See defines 'MEM_TYPE_???' above for bit definitions of the ranges.
  UINT8    granularity;    // Interleave granularities for current SAD entry.  Possible interleave granularity options depend on the SAD entry type.  Note that SAD entry type BLK Window and CSR/Mailbox/Ctrl region do not support any granularity options
  UINT32   Base;           // Base of the current SAD entry
  UINT32   Limit;          // Limit of the current SAD entry
  UINT8    ways;           // Interleave ways for SAD
  UINT8    channelInterBitmap[MAX_IMC];   //Bit map to denote which DDR4/NM channels are interleaved per IMC eg: 111b - Ch 2,1 & 0 are interleaved; 011b denotes Ch1 & 0 are interleaved
  UINT8    FMchannelInterBitmap[MAX_IMC];   //Bit map to denote which FM channels are interleaved per IMC eg: 111b - Ch 2,1 & 0 are interleaved; 011b denotes Ch1 & 0 are interleaved
  UINT8    NmChWays;         // Channel Interleave ways for SAD. Represents channelInterBitmap ways for DDR4/NM.
  UINT8    FmChWays;         // Channel Interleave ways for SAD. Represents FMchannelInterBitmap ways for DDRT.
  UINT8    imcInterBitmap;   // Bit map to denote which IMCs are interleaved from this socket.
  UINT8    NmImcInterBitmap; // Bit map to denote which IMCs are interleaved from this socket as NM (10nm usage only).
  BOOLEAN  local;          //0 - Remote 1- Local
  UINT8    IotEnabled;     // To indicate if IOT is enabled
  UINT8    mirrored;       //To Indicate the SAD is mirrored while enabling partial mirroring
  UINT8    Attr;
  UINT8    tgtGranularity; // Interleave mode for target list
  UINT8    Cluster;        // SNC cluster, hemisphere, or quadrant index.
} SAD_TABLE;

typedef struct IMC {
  UINT8     imcEnabled[MAX_IMC];
  UINT8     imcNum;                                   // imc Number
  UINT32    memSize;                                  // DDR4 memory size for this imc (64MB granularity)
  UINT32    NVmemSize;                                // NV Memory size of this ha
  UINT32    volSize;                                  // Volatile size of the NVM dimms for this imc (64MB granularity)
  UINT32    NonVolSize;                               // Non-Volatile size of the NVM DIMMs for this iMC (64MB granularity)
  UINT32    perSize;                                  // Persistent size of the NVM dimms for this imc (64MB granularity)
  UINT32    WbCachePerSize;                           // Persistent WB cache (AD-WB) size of the NVM dimms for this imc (64MB granularity)
  UINT8     TADintList[MAX_TAD_RULES][MAX_TAD_WAYS];  // TAD interleave list for this socket
  UINT8     TADChnIndex[MAX_TAD_RULES][MAX_TAD_WAYS]; // Corresponding TAD channel indexes (per channel)
  INT32     TADOffset[MAX_TAD_RULES][MAX_TAD_WAYS];   // Corresponding TAD offsets (per channel)
  TAD_TABLE TAD[MAX_TAD_RULES];                       // TAD table
  UINT8     imcChannelListStartIndex;                 // Index in channel list of first channel on this imc
} IMC_INFO_STRUCT;

typedef struct firmwareRev {
  UINT8  majorVersion;
  UINT8  minorVersion;
  UINT8  hotfixVersion;
  UINT16 buildVersion;
} FIRMWARE_REV;

typedef struct Reserved168 {
  UINT8                 Reserved79;
  UINT8                 Reserved80;
  UINT8                 Reserved83;
  UINT8                 Reserved86[MAX_SOCKET * MAX_IMC];
  UINT8                 Reserved89;
  UINT8                 Reserved87;
  UINT8                 Reserved148;
} MEM_RESERVED_1;

#pragma pack()

#define     MAX_SI_SOCKET        8   // Maximum silicon supported socket number

typedef struct {
  UINT32  BlockDecoderBase;   // 64MB unit
  UINT32  BlockDecoderLimit;
  UINT8   BlockSocketEnable;
  UINT8   BlockMcChEn[MAX_SI_SOCKET][MAX_IMC][MAX_MC_CH];
} MEMORY_MAP_BLOCK_DECODER_DATA;

//
// Chip specific section of struct Socket
//
#define SOCKET_CHIP     \
  struct SADTable   SAD[MAX_DRAM_CLUSTERS * MAX_SAD_RULES];                                                                                  \
  UINT8             DdrtChRouteTable[MAX_RT][MAX_RT_WAYS];  /* PMEM/BLK memory channel route table 2 for CR protocol */  \
  UINT8             DdrtTgtRouteTable[MAX_RT][MAX_RT_WAYS]; /* PMEM/BLK memory target route table 2 for CR protocol */   \
  struct IMC        imc[MAX_IMC];                                                                                        \
  UINT8             ddrClkData;                                                                                          \
  UINT8             ddrClkType;                                                                                          \
  UINT8             ddrFreqCheckFlag;                                                                                    \
  UINT8             SktSkuValid;                                                                                         \
  UINT32            SktSkuLimit;                                                                                         \
  UINT32            SktTotMemMapSPA;                                                                                     \
  UINT32            SktPmemMapSpa;                                                                                       \
  UINT32            SktMemSize2LM;                                                                                       \
  UINT8             maxFreq;                                                                                             \
  UINT8             clkSwapFixDis;                                                                                       \
  UINT8             ioInitdone;

#endif // _memhostchipcommon_h
