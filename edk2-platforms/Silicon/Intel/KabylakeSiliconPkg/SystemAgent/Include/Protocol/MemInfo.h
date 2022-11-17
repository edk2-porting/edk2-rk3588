/** @file
  This protocol provides the memory information data, such as
  total physical memory size, memory frequency, memory size
  of each dimm and rank.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _MEM_INFO_PROTOCOL_H_
#define _MEM_INFO_PROTOCOL_H_


//
// Extern the GUID for protocol users.
//
extern EFI_GUID gMemInfoProtocolGuid;

//
// Protocol definitions
//
#define NODE_NUM  1
#define CH_NUM    2
#define DIMM_NUM  2
#define RANK_NUM  2
#define SLOT_NUM  (CH_NUM * DIMM_NUM)
#define PROFILE_NUM 4 // number of memory profiles supported
#define XMP_PROFILE_NUM 2 // number of XMP profiles supported

//
// Matches MrcDdrType enum in MRC
//
#ifndef MRC_DDR_TYPE_DDR4
#define MRC_DDR_TYPE_DDR4     0
#endif
#ifndef MRC_DDR_TYPE_DDR3
#define MRC_DDR_TYPE_DDR3     1
#endif
#ifndef MRC_DDR_TYPE_LPDDR3
#define MRC_DDR_TYPE_LPDDR3   2
#endif
#ifndef MRC_DDR_TYPE_UNKNOWN
#define MRC_DDR_TYPE_UNKNOWN  3
#endif

//
// Matches MrcDimmSts enum in MRC
//
#ifndef DIMM_ENABLED
#define DIMM_ENABLED     0  // DIMM/rank Pair is enabled, presence will be detected.
#endif
#ifndef DIMM_DISABLED
#define DIMM_DISABLED    1  // DIMM/rank Pair is disabled, regardless of presence.
#endif
#ifndef DIMM_PRESENT
#define DIMM_PRESENT     2  // There is a DIMM present in the slot/rank pair and it will be used.
#endif
#ifndef DIMM_NOT_PRESENT
#define DIMM_NOT_PRESENT 3  // There is no DIMM present in the slot/rank pair.
#endif

#pragma pack(1)
///
/// Memory timing Structure
///
typedef struct {
  UINT32 tCK;     ///< Offset 0 Memory cycle time, in femtoseconds.
  UINT16 NMode;   ///< Offset 4 Number of tCK cycles for the channel DIMM's command rate mode.
  UINT16 tCL;     ///< Offset 6 Number of tCK cycles for the channel DIMM's CAS latency.
  UINT16 tCWL;    ///< Offset 8 Number of tCK cycles for the channel DIMM's minimum CAS write latency time.
  UINT16 tFAW;    ///< Offset 10 Number of tCK cycles for the channel DIMM's minimum four activate window delay time.
  UINT16 tRAS;    ///< Offset 12 Number of tCK cycles for the channel DIMM's minimum active to precharge delay time.
  UINT16 tRCDtRP; ///< Offset 14 Number of tCK cycles for the channel DIMM's minimum RAS# to CAS# delay time and Row Precharge delay time
  UINT16 tREFI;   ///< Offset 16 Number of tCK cycles for the channel DIMM's minimum Average Periodic Refresh Interval.
  UINT16 tRFC;    ///< Offset 18 Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRPab;   ///< Offset 20 Number of tCK cycles for the channel DIMM's minimum row precharge delay time for all banks.
  UINT16 tRRD;    ///< Offset 22 Number of tCK cycles for the channel DIMM's minimum row active to row active delay time.
  UINT16 tRTP;    ///< Offset 24 Number of tCK cycles for the channel DIMM's minimum internal read to precharge command delay time.
  UINT16 tWR;     ///< Offset 26 Number of tCK cycles for the channel DIMM's minimum write recovery time.
  UINT16 tWTR;    ///< Offset 28 Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time.
  UINT8  Rsvd[2]; ///< Offset 30
} MEMORY_TIMING;

// @todo use the MemInfoHob data instead of duplicate structure.
///
/// Memory information Data Structure
///
typedef struct {
  MEMORY_TIMING Timing[PROFILE_NUM];                   ///< Offset 0 Timming information for the DIMM
  UINT32  memSize;                                     ///< Offset 128 Total physical memory size
  UINT16  ddrFreq;                                     ///< Offset 132 DDR Current Frequency
  UINT16  ddrFreqMax;                                  ///< Offset 134 DDR Maximum Frequency
  UINT16  dimmSize[NODE_NUM * CH_NUM * DIMM_NUM];      ///< Offset 136 Size of each DIMM
  UINT16  VddVoltage[PROFILE_NUM];                     ///< Offset 144 The voltage setting for the DIMM
  UINT8   DimmStatus[NODE_NUM * CH_NUM * DIMM_NUM];    ///< Offset 152 The enumeration value from MrcDimmSts
  UINT8   RankInDimm[NODE_NUM * CH_NUM * DIMM_NUM];    ///< Offset 156 Number of ranks in a DIMM
  UINT8   *DimmsSpdData[NODE_NUM * CH_NUM * DIMM_NUM]; ///< Offset 160 SPD data of each DIMM
  UINT8   RefClk;                                      ///< Offset 192 Reference Clock
  UINT8   Ratio;                                       ///< Offset 193 Clock Multiplier
  BOOLEAN EccSupport;                                  ///< Offset 194 ECC supported or not
  UINT8   Profile;                                     ///< Offset 195 Currently running memory profile
  UINT8   XmpProfileEnable;                            ///< Offset 196 If XMP capable DIMMs are detected, this will indicate which XMP Profiles are common among all DIMMs.
  UINT8   DdrType;                                     ///< Offset 197 Current DDR type, see DDR_TYPE_xxx defines above
  UINT8   Reserved[2];                                 ///< Offset 198 Reserved bytes for future use
  UINT32  DefaultXmptCK[XMP_PROFILE_NUM];              ///< Offset 200 The Default XMP tCK values read from SPD.
} MEMORY_INFO_DATA;
#pragma pack()

///
/// Memory information Protocol definition
///
typedef struct {
  MEMORY_INFO_DATA  MemInfoData; ///< Memory Information Data Structure
} MEM_INFO_PROTOCOL;

#endif
