/** @file
  Copies the memory related timing and configuration information into the
  Compatible BIOS data (BDAT) table.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _MrcRmtData_h_
#define _MrcRmtData_h_

#include "MrcTypes.h"

#define VDD_1_350             1350                      ///< VDD in millivolts
#define VDD_1_500             1500                      ///< VDD in millivolts
#define PI_STEP_BASE          2048                      ///< Magic number from spec
#define PI_STEP_INTERVAL      128                       ///< tCK is split into this amount of intervals
#define PI_STEP               ((PI_STEP_BASE) / (PI_STEP_INTERVAL))
#define VREF_STEP_BASE        100                       ///< Magic number from spec
#define TX_VREF_STEP          7800                      ///< TX Vref step in microvolts
#define TX_VREF(VDD)          (((TX_VREF_STEP) * (VREF_STEP_BASE)) / (VDD)) ///< VDD passed in is in millivolts
#define RX_VREF_STEP          8000                      ///< TX Vref step in microvolts
#define RX_VREF(VDD)          (((RX_VREF_STEP) * (VREF_STEP_BASE)) / (VDD)) ///< VDD passed in is in millivolts
#define CA_VREF_STEP          8000                      ///< TX Vref step in microvolts
#define CA_VREF(VDD)          (((CA_VREF_STEP) * (VREF_STEP_BASE)) / (VDD)) ///< VDD passed in is in millivolts

#define MAX_SPD_RMT           512                       ///< The maximum amount of data, in bytes, in an SPD structure.
#define RMT_PRIMARY_VERSION   4                         ///< The BDAT structure that is currently supported.
#define RMT_SECONDARY_VERSION 0                         ///< The BDAT structure that is currently supported.
#define MAX_MODE_REGISTER     7                         ///< Number of mode registers
#define MAX_DRAM_DEVICE       9                         ///< Maximum number of memory devices

//
// Warning: Bdat4.h has its own copy of this #define
// make sure to change it in both places
//
#define MAX_SCHEMA_LIST_LENGTH (10)

/*
 Memory Schema GUID
 This is private GUID used by MemoryInit internally.
 {CE3F6794-4883-492C-8DBA-2FC098447710}
*/
#ifdef BDAT_SUPPORT
extern EFI_GUID gEfiMemorySchemaGuid;
#endif
/*
 GUID for Schema List HOB
 This is private GUID used by MemoryInit internally.
 {3047C2AC-5E8E-4C55-A1CB-EAAD0A88861B}
*/
extern EFI_GUID gMrcSchemaListHobGuid;


#pragma pack(push, 1)

typedef struct {
  UINT8                       RxDqLeft;                 ///< Units = piStep
  UINT8                       RxDqRight;
  UINT8                       TxDqLeft;
  UINT8                       TxDqRight;
  UINT8                       RxVrefLow;                ///< Units = rxVrefStep
  UINT8                       RxVrefHigh;
  UINT8                       TxVrefLow;                ///< Units = txVrefStep
  UINT8                       TxVrefHigh;
} BDAT_DQ_MARGIN_STRUCTURE;

typedef struct {
  UINT8                       RxDqLeft;                 ///< Units = piStep
  UINT8                       RxDqRight;
  UINT8                       TxDqLeft;
  UINT8                       TxDqRight;
  UINT8                       CmdLeft;
  UINT8                       CmdRight;
  UINT8                       RecvenLeft;               ///< Units = recvenStep
  UINT8                       RecvenRight;
  UINT8                       WrLevelLeft;              ///< Units = wrLevelStep
  UINT8                       WrLevelRight;
  UINT8                       RxVrefLow;                ///< Units = rxVrefStep
  UINT8                       RxVrefHigh;
  UINT8                       TxVrefLow;                ///< Units = txVrefStep
  UINT8                       TxVrefHigh;
  UINT8                       CmdVrefLow;               ///< Units = caVrefStep
  UINT8                       CmdVrefHigh;
} BDAT_RANK_MARGIN_STRUCTURE;

typedef struct {
  UINT16                      RecEnDelay[MAX_STROBE];
  UINT16                      WlDelay[MAX_STROBE];
  UINT8                       RxDqDelay[MAX_STROBE];
  UINT8                       TxDqDelay[MAX_STROBE];
  UINT8                       ClkDelay;
  UINT8                       CtlDelay;
  UINT8                       CmdDelay[3];
  UINT8                       IoLatency;
  UINT8                       Roundtrip;
} BDAT_RANK_TRAINING_STRUCTURE;

typedef struct {
  UINT16 ModeRegister[MAX_MODE_REGISTER];                  ///< Mode register settings
} BDAT_DRAM_MRS_STRUCTURE;

typedef struct {
  UINT8                       RankEnabled;              ///< 0 = Rank disabled
  UINT8                       RankMarginEnabled;        ///< 0 = Rank margin disabled
  UINT8                       DqMarginEnabled;          ///< 0 = Dq margin disabled
  BDAT_RANK_MARGIN_STRUCTURE  RankMargin;               ///< Rank margin data
  BDAT_DQ_MARGIN_STRUCTURE    DqMargin[MAX_DQ];         ///< Array of Dq margin data per rank
  BDAT_RANK_TRAINING_STRUCTURE RankTraining;            ///< Rank training settings
  BDAT_DRAM_MRS_STRUCTURE     RankMRS[MAX_DRAM_DEVICE]; ///< Rank MRS settings
} BDAT_RANK_STRUCTURE;

typedef struct {
  UINT8                       SpdValid[MAX_SPD_RMT / (CHAR_BITS * sizeof (UINT8))]; ///< Each valid bit maps to SPD byte
  UINT8                       SpdData[MAX_SPD_RMT];     ///< Array of raw SPD data bytes
} BDAT_SPD_STRUCTURE;

typedef struct {
  UINT8                       DimmEnabled;              ///< 0 = DIMM disabled
  BDAT_RANK_STRUCTURE         RankList[MAX_RANK_IN_DIMM]; ///< Array of ranks per DIMM
  BDAT_SPD_STRUCTURE          SpdBytes;                 ///< SPD data per DIMM
} BDAT_DIMM_STRUCTURE;

typedef struct {
  UINT8                       ChannelEnabled;           ///< 0 = Channel disabled
  UINT8                       NumDimmSlot;              ///< Number of slots per channel on the board
  BDAT_DIMM_STRUCTURE         DimmList[MAX_DIMMS_IN_CHANNEL]; ///< Array of DIMMs per channel
} BDAT_CHANNEL_STRUCTURE;

typedef struct {
  UINT8                       ControllerEnabled;        ///< 0 = MC disabled
  UINT16                      ControllerDeviceId;       ///< MC device Id
  UINT8                       ControllerRevisionId;     ///< MC revision Id
  UINT16                      MemoryFrequency;          ///< Memory frequency in units of MHz / 10
                                                        ///< e.g. ddrFreq = 13333 for tCK = 1.5 ns
  UINT16                      MemoryVoltage;            ///< Memory Vdd in units of mV
                                                        ///< e.g. ddrVoltage = 1350 for Vdd = 1.35 V
  UINT8                       PiStep;                   ///< Step unit = piStep * tCK / 2048
                                                        ///< e.g. piStep = 16 for step = 11.7 ps (1/128 tCK)
  UINT16                      RxVrefStep;               ///< Step unit = rxVrefStep * Vdd / 100
                                                        ///< e.g. rxVrefStep = 520 for step = 7.02 mV
  UINT16                      TxVrefStep;               ///< Step unit = txVrefStep * Vdd / 100
  UINT16                      CaVrefStep;               ///< Step unit = caVrefStep * Vdd / 100
  UINT8                       RecvenStep;               ///< Step unit = recvenStep * tCK / 2048
  UINT8                       WrLevelStep;              ///< Step unit = wrLevelStep * tCK / 2048
  BDAT_CHANNEL_STRUCTURE      ChannelList[MAX_CHANNEL]; ///< Array of channels per memory controller
} BDAT_SOCKET_STRUCTURE;

typedef struct {
  union {
    UINT32                    Data32;                   ///< MRC version: Major.Minor.Revision.Build
    struct {
      UINT8                   Build;                    ///< MRC version: Build
      UINT8                   Revision;                 ///< MRC version: Revision
      UINT8                   Minor;                    ///< MRC version: Minor
      UINT8                   Major;                    ///< MRC version: Major
    } Version;
  }                           RefCodeRevision;          ///< Major.Minor.Revision.Build
  UINT8                       MaxController;            ///< Max controllers per system, e.g. 1
  UINT8                       MaxChannel;               ///< Max channels per memory controller, e.g. 2
  UINT8                       MaxDimm;                  ///< Max DIMM per channel, e.g. 2
  UINT8                       MaxRankDimm;              ///< Max ranks per DIMM, e.g. 2
  UINT8                       MaxStrobe;                ///< Number of Dqs used by the rank, e.g. 18
  UINT8                       MaxDq;                    ///< Number of Dq bits used by the rank, e.g. 72
  UINT32                      MarginLoopCount;          ///< Units of cache line
  BDAT_SOCKET_STRUCTURE       ControllerList[MAX_CONTROLLERS]; ///< Array of memory controllers per system
} BDAT_SYSTEM_STRUCTURE;

typedef struct {
  UINT32                      Data1;
  UINT16                      Data2;
  UINT16                      Data3;
  UINT8                       Data4[8];
} BDAT_EFI_GUID;

typedef struct {
  UINT16  HobType;
  UINT16  HobLength;
  UINT32  Reserved;
} BDAT_HOB_GENERIC_HEADER;

typedef struct {
  BDAT_HOB_GENERIC_HEADER  Header;
  BDAT_EFI_GUID            Name;
  ///
  /// Guid specific data goes here
  ///
} BDAT_HOB_GUID_TYPE;

typedef struct {
  BDAT_EFI_GUID               SchemaId;                         ///< The GUID uniquely identifies the format of the data contained within the structure.
  UINT32                      DataSize;                         ///< The total size of the memory block, including both the header as well as the schema specific data.
  UINT16                      Crc16;                            ///< Crc16 is computed in the same manner as the field in the BDAT_HEADER_STRUCTURE.
} MRC_BDAT_SCHEMA_HEADER_STRUCTURE;

typedef struct {
  MRC_BDAT_SCHEMA_HEADER_STRUCTURE SchemaHeader;                ///< The schema header.
  union {
    UINT32                    Data;                             ///< MRC version: Major.Minor.Revision.Build
    struct {
      UINT8                   Build;                            ///< MRC version: Build
      UINT8                   Revision;                         ///< MRC version: Revision
      UINT8                   Minor;                            ///< MRC version: Minor
      UINT8                   Major;                            ///< MRC version: Major
    } Version;
  }                           RefCodeRevision;                  ///< Major.Minor.Revision.Build
  UINT8                       MaxController;                    ///< Max controllers per system, e.g. 1
  UINT8                       MaxChannel;                       ///< Max channels per memory controller, e.g. 2
  UINT8                       MaxDimm;                          ///< Max DIMM per channel, e.g. 2
  UINT8                       MaxRankDimm;                      ///< Max ranks per DIMM, e.g. 2
  UINT8                       MaxStrobe;                        ///< Number of Dqs used by the rank, e.g. 18
  UINT8                       MaxDq;                            ///< Number of Dq bits used by the rank, e.g. 72
  UINT32                      MarginLoopCount;                  ///< Units of cache line
  BDAT_SOCKET_STRUCTURE       ControllerList[MAX_CONTROLLERS];  ///< Array of memory controllers per system
} BDAT_MEMORY_DATA_STRUCTURE;

typedef struct {
  BDAT_HOB_GUID_TYPE          HobGuidType;
  BDAT_MEMORY_DATA_STRUCTURE  MemorySchema;
} BDAT_MEMORY_DATA_HOB;

#pragma pack (pop)

typedef struct {
  BDAT_HOB_GUID_TYPE          HobGuidType;
  UINT16                      SchemaHobCount;
  UINT16                      Reserved;
  BDAT_EFI_GUID               SchemaHobGuids[MAX_SCHEMA_LIST_LENGTH];
} MRC_BDAT_SCHEMA_LIST_HOB;

#endif //_MrcRmtData_h_
