/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _HW_MEM_INIT_LIB_H_
#define _HW_MEM_INIT_LIB_H_

#include <PlatformArch.h>

#define I2C_CHANNEL     2
#define MAX_I2C_DEV     6

#define SPD_MODULE_PART         18
#define SPD_MODULE_PART_DDR4    20

#define NVRAM_ADDR      0x00D00000

typedef enum {
    DDR_FREQ_AUTO = 0,
    DDR_FREQ_800,
    DDR_FREQ_1000,
    DDR_FREQ_1066,
    DDR_FREQ_1200,
    DDR_FREQ_1333,
    DDR_FREQ_1400,
    DDR_FREQ_1600,
    DDR_FREQ_1800,
    DDR_FREQ_1866,
    DDR_FREQ_2000,
    DDR_FREQ_2133,
    DDR_FREQ_2200,
    DDR_FREQ_2400,
    DDR_FREQ_2600,
    DDR_FREQ_2666,
    DDR_FREQ_2800,
    DDR_FREQ_2933,
    DDR_FREQ_3000,
    DDR_FREQ_3200,
    DDR_FREQ_MAX
} DDR_FREQUENCY_INDEX;

struct baseMargin {
  INT16 n;
  INT16 p;
};

struct rankMargin {
  struct baseMargin rank[MAX_CHANNEL][MAX_RANK_CH];
};

typedef struct _NVRAM_RANK_DATA {
    UINT16      MR0;
    UINT16      MR1;
    UINT16      MR2;
    UINT16      MR3;
    UINT16      MR4;
    UINT16      MR5;
    UINT16      MR6[9];
} NVRAM_RANK_DATA;

typedef struct _NVRAM_DIMM_DATA {
    NVRAM_RANK_DATA      Rank[MAX_RANK_DIMM];
} NVRAM_DIMM_DATA;


typedef struct _NVRAM_CHANNEL_DATA {
    NVRAM_DIMM_DATA      Dimm[MAX_DIMM];
    UINT32          DDRC_CFG_ECC;
    UINT32          DDRC_CFG_WORKMODE;
    UINT32          DDRC_CFG_WORKMODE1;
    UINT32          DDRC_CFG_WORKMODE2;
    UINT32          DDRC_CFG_DDRMODE;
    UINT32          DDRC_CFG_DIMM;
    UINT32          DDRC_CFG_RNKVOL_0;
    UINT32          DDRC_CFG_RNKVOL_1;
    UINT32          DDRC_CFG_RNKVOL_2;
    UINT32          DDRC_CFG_RNKVOL_3;
    UINT32          DDRC_CFG_RNKVOL_4;
    UINT32          DDRC_CFG_RNKVOL_5;
    UINT32          DDRC_CFG_RNKVOL_6;
    UINT32          DDRC_CFG_RNKVOL_7;
    UINT32          DDRC_CFG_RNKVOL_8;
    UINT32          DDRC_CFG_RNKVOL_9;
    UINT32          DDRC_CFG_RNKVOL_10;
    UINT32          DDRC_CFG_RNKVOL_11;
    UINT32          DDRC_CFG_ODT_0;
    UINT32          DDRC_CFG_ODT_1;
    UINT32          DDRC_CFG_ODT_2;
    UINT32          DDRC_CFG_ODT_3;
    UINT32          DDRC_CFG_ODT_4;
    UINT32          DDRC_CFG_ODT_5;
    UINT32          DDRC_CFG_ODT_6;
    UINT32          DDRC_CFG_ODT_7;
    UINT32          DDRC_CFG_ODT_8;
    UINT32          DDRC_CFG_ODT_9;
    UINT32          DDRC_CFG_ODT_10;
    UINT32          DDRC_CFG_ODT_11;
    UINT32          DDRC_CFG_TIMING0;
    UINT32          DDRC_CFG_TIMING1;
    UINT32          DDRC_CFG_TIMING2;
    UINT32          DDRC_CFG_TIMING3;
    UINT32          DDRC_CFG_TIMING4;
    UINT32          DDRC_CFG_TIMING5;
    UINT32          DDRC_CFG_TIMING6;
    UINT32          DDRC_CFG_TIMING7;
    UINT32          DDRC_CFG_DFI_LAT0;
    UINT32          DDRC_CFG_DFI_LAT1;
    UINT32          DDRC_CFG_DDRPHY;
    UINT32          Config[24];
    BOOLEAN         Status;
} NVRAM_CHANNEL_DATA;

typedef struct _NVRAM_DATA {
    UINT32              NvramCrc;
    NVRAM_CHANNEL_DATA  Channel[MAX_SOCKET][MAX_CHANNEL];
    UINT32              DdrFreqIdx;

} NVRAM_DATA;

struct DDR_RANK_DATA {
    BOOLEAN     Enabled;
};

typedef struct _DDR_DIMM_DATA {
    BOOLEAN         Enabled;
    UINT8           DramType;           //Byte 2
    UINT8           ModuleType;         //Byte 3
    UINT8           BankNum;            //Byte 4
    UINT8           RowBits;            //Byte 5
    UINT8           ColBits;            //Byte 5
    UINT8           SpdVdd;             //Byte 6
    UINT8           RankNum;            //Byte 7
    UINT8           PrimaryBusWidth;    //Byte 8
    UINT8           ExtensionBusWidth;  //Byte 8
    UINT8           SpdModPart[SPD_MODULE_PART];                // Module Part Number
    UINT8           SpdModPartDDR4[SPD_MODULE_PART_DDR4];       // Module Part Number DDR4
    UINT16          SpdMMfgId;              // Module Mfg Id from SPD
    UINT32          SpdSerialNum;
    UINT32          RankSize;
    UINT16          DimmSize;
    UINT16          DimmSpeed;
    UINT16          SpdMMDate;
    struct DDR_RANK_DATA    Rank[MAX_RANK_DIMM];
} DDR_DIMM_DATA;

typedef struct _DDR_CHANNEL_DATA {
    BOOLEAN         Enabled;
    DDR_DIMM_DATA   Dimm[MAX_DIMM];
    UINT8           CurrentDimmNum;
} DDR_CHANNEL_DATA;

typedef struct _MEMORY_DATA {
    UINT8           RascBypass;
} MEMORY_DATA;

typedef struct _NUMAINFO_DATA {
    UINT8           NodeId;
    UINT64          Base;
    UINT64          Length;
    UINT32          ScclInterleaveEn;
} NUMAINFO_DATA;


typedef struct _GBL_DATA_INTERFACE {
    DDR_CHANNEL_DATA        Channel[MAX_SOCKET][MAX_CHANNEL];
    UINT32                  DdrFreqIdx;
    UINT32                  Freq;
    UINT32                  EccEn;
    UINT32                  MemSize;
    BOOLEAN                 SetupExist;
    NVRAM_DATA              NvRamData;
    MEMORY_DATA             MemData;
    NUMAINFO_DATA           NumaInfo[MAX_SOCKET][MAX_NUM_PER_TYPE];
} GBL_INTERFACE;

typedef union {
    struct {
        UINT16  freqIndex:4;         //Frequency Index;
        UINT16  slot0:4;        //Channel slot0 for DIMM
        UINT16  slot1:4;        //Channel slot1 for DIMM
        UINT16  slot2:4;        //Channel slot2 for DIMM
    }Bits;
    UINT16      Data;
}ODT_VALUE_INDEX;

typedef union {
    struct {
        UINT8   RTTNom:3;
        UINT8   reserved_3:1;
        UINT8   RTTWr:2;
        UINT8   reserved_6:2;
    }Bits;
    UINT8      Data;
}ODT_RTT_VALUE_DDR3;

typedef union {
    struct {
        UINT8   RTTNom:3;
        UINT8   RTTPark:3;
        UINT8   RTTWr:2;
    }Bits;
    UINT8      Data;
}ODT_RTT_VALUE_DDR4;

typedef union {
    struct {
        UINT16  tarDimm:2;      // target DIMM
        UINT16  tarRank:2;      // target Rank
        UINT16  slot0:4;        // Channel slot0 for DIMM
        UINT16  slot1:4;        // Channel slot1 for DIMM
        UINT16  slot2:4;        // Channel slot2 for DIMM
    }Bits;
    UINT16      Data;
}ODT_ACTIVE_INDEX;

struct ODT_VALUE_STRUCT_DDR3 {
   UINT16       config;                                 // ODT_VALUE_INDEX
   UINT8        dramOdt[MAX_DIMM][MAX_RANK_DIMM];       // ODT_VALUE_RTT_DDR3
};

struct ODT_VALUE_STRUCT_DDR4 {
    UINT16      config;
    UINT8       dramOdt[MAX_DIMM][MAX_RANK_DIMM];
};

struct ODT_ACTIVE_STRUCT {
    UINT16      config;             // ODT config index
    UINT16      actBits[2];         // WR :Bits[3;0] = D0_R[3:0] Bits[7:4] = D1_R[3:0] Bits[11:8] = D2_R[3:0]
};

// JEDEC manufacturer IDs from JEP-106
#define MFGID_AENEON    0x5705
#define MFGID_QIMONDA   0x5105
#define MFGID_NEC       0x1000
#define MFGID_IDT       0xB300
#define MFGID_TI        0x9700
#define MFGID_HYNIX     0xAD00
#define MFGID_MICRON    0x2C00
#define MFGID_INFINEON  0xC100
#define MFGID_SAMSUNG   0xCE00
#define MFGID_TEK       0x3D00
#define MFGID_KINGSTON  0x9801
#define MFGID_ELPIDA    0xFE02
#define MFGID_SMART     0x9401
#define MFGID_AGILENT   0xC802
#define MFGID_NANYA     0x0B03
#define MFGID_INPHI     0xB304
#define MFGID_MONTAGE   0x3206
#define MFGID_RAMAXEL   0x4304

//
// DDR3 frequencies 800 - 2667
// DDR4 frequencies 1333 - 3200
//
#define DDR_800  0   // tCK(ns)=2.5
#define DDR_1000 1   // tCK(ns)=2.0
#define DDR_1066 2   // tCK(ns)=1.875
#define DDR_1200 3   // tCK(ns)=1.667
#define DDR_1333 4   // tCK(ns)=1.5
#define DDR_1400 5   // tCK(ns)=1.429
#define DDR_1600 6   // tCK(ns)=1.25
#define DDR_1800 7   // tCK(ns)=1.11
#define DDR_1866 8   // tCK(ns)=1.07
#define DDR_2000 9   // tCK(ns)=1.0
#define DDR_2133 10  // tCK(ns)=0.9375
#define DDR_2200 11  // tCK(ns)=0.909
#define DDR_2400 12  // tCK(ns)=0.833
#define DDR_2600 13  // tCK(ns)=0.769
#define DDR_2666 14  // tCK(ns)=0.750
#define DDR_2800 15  // tCK(ns)=0.714
#define DDR_2933 16  // tCK(ns)=0.682
#define DDR_3000 17  // tCK(ns)=0.667
#define DDR_3200 18  // tCK(ns)=0.625
#define DDR_MAX  (DDR_3200)

#define FREQUENCY_MTB_OFFSET            1000000
#define FREQUENCY_FTB_OFFSET               1000

//
#define DDR_800_TCK_MIN                 25000
#define DDR_1000_TCK_MIN                20000
#define DDR_1067_TCK_MIN                18750
#define DDR_1200_TCK_MIN                16670
#define DDR_1333_TCK_MIN                15000
#define DDR_1400_TCK_MIN                14290
#define DDR_1600_TCK_MIN                12500
#define DDR_1800_TCK_MIN                11100
#define DDR_1867_TCK_MIN                10710
#define DDR_2000_TCK_MIN                10000
#define DDR_2133_TCK_MIN                 9380
#define DDR_2200_TCK_MIN                 9090
#define DDR_2400_TCK_MIN                 8330
#define DDR_2600_TCK_MIN                 7690
#define DDR_2667_TCK_MIN                 7500
#define DDR_2800_TCK_MIN                 7140
#define DDR_2933_TCK_MIN                 6820
#define DDR_3000_TCK_MIN                 6670
#define DDR_3200_TCK_MIN                 6250


//
// Serial Presence Detect bytes (JEDEC revision 1.0)
//
#define SPD_SIZE              0     // Bytes used, Device size, CRC coverage
#define SPD_REVISION          1     // SPD Encoding Revision
#define SPD_KEY_BYTE          2     // DRAM Device Type
  #define SPD_TYPE_DDR3       0x0B  // DDR3 SDRAM
  #define SPD_TYPE_DDR4       0x0C  // DDR4 SDRAM
#define SPD_KEY_BYTE2         3     // Module Type and Thickness (RDIMM or UDIMM)
  #define SPD_RDIMM           1     // Module type is RDIMM
  #define SPD_UDIMM           2     // Module type is UDIMM
  #define SPD_SODIMM          3     // Module type is SODIMM
  #define SPD_MICRO_DIMM      4     // Module type is Micro-DIMM
  #define SPD_LRDIMM_DDR4     4     // Module type is LRDIMM (DDR4)
  #define SPD_MINI_RDIMM      5     // Module type is Mini-RDIMM
  #define SPD_MINI_UDIMM      6     // Module type is Mini-UDIMM
  #define SPD_MINI_CDIMM      7     // Module type is Mini-CDIMM
  #define SPD_ECC_SO_UDIMM    9     // Module type is 72b-SO-UDIMM
  #define SPD_ECC_SO_RDIMM    8     // Module type is 72b-SO-RDIMM
  #define SPD_ECC_SO_CDIMM    10    // Module type is 72b-SO-CDIMM
  #define SPD_LRDIMM          11    // Module type is LRDIMM
  #define SPD_UDIMM_ECC       18    // Module type is UDIMM-ECC
#define SPD_SDRAM_BANKS       4     // SDRAM Density and number of internal banks
  #define SPD_1Gb             2     // Total SDRAM Capacity 1 Gigabits
  #define SPD_2Gb             3     // Total SDRAM Capacity 2 Gigabits
  #define SPD_4Gb             4     // Total SDRAM Capacity 4 Gigabits
  #define SPD_8Gb             5     // Total SDRAM Capacity 8 Gigabits
  #define SPD_16Gb            6     // Total SDRAM Capacity 16 Gigabits
  #define SPD_32Gb            7     // Total SDRAM Capacity 32 Gigabits
#define SPD_SDRAM_ADDR        5     // Number of Row and Column address bits
  #define SPD_ROW_12          0     // 12 row bits
  #define SPD_ROW_13          1     // 13 row bits
  #define SPD_ROW_14          2     // 14 row bits
  #define SPD_ROW_15          3     // 15 row bits
  #define SPD_ROW_16          4     // 16 row bits
  #define SPD_ROW_17          5     // 17 row bits
  #define SPD_ROW_18          6     // 18 row bits
  #define SPD_COL_9           0     // 9 colum bits
  #define SPD_COL_10          1     // 10 colum bits
  #define SPD_COL_11          2     // 11 colum bits
  #define SPD_COL_12          3     // 12 colum bits
#define SPD_VDD_SUPPORT       6     // Vdd DIMM supports
  #define SPD_VDD_150         0     // Module Supports 1.50V
  #define SPD_VDD_135         BIT1  // Module Supports 1.35V
  #define SPD_VDD_125         BIT2  // Module Supports 1.25V
#define SPD_MODULE_ORG_DDR3   7     // Number of Ranks and SDRAM device width
#define SPD_MODULE_ORG_DDR4   12    // DDR4 Module Organization
  #define DEVICE_WIDTH_X4     0     // SDRAM device width = 4 bits
  #define DEVICE_WIDTH_X8     1     // SDRAM device width = 8 bits
  #define DEVICE_WIDTH_X16    2     // SDRAM device width = 16 bits
  #define SPD_NUM_RANKS_1   0
  #define SPD_NUM_RANKS_2   1
  #define SPD_NUM_RANKS_4   3
  #define SPD_NUM_RANKS_8   4
#define SPD_MEM_BUS_WID     8     // Width of SDRAM memory bus
#define SPD_FTB             9     // Timebase for fine grain timing calculations
#define SPD_MTB_DIVEND      10    // Medium Time Base Dividend
#define SPD_MTB_DIVISOR     11    // Medium Time Base Divisor
#define SPD_MIN_TCK         12    // Minimum cycle time (at max CL)
  #define SPD_TCKMIN_800      20    // tCK(MTB)=20, tCK(ns)=2.5
  #define SPD_TCKMIN_1067     15    // tCK(MTB)=15, tCK(ns)=1.875
  #define SPD_TCKMIN_1333     12    // tCK(MTB)=12, tCK(ns)=1.5
  #define SPD_TCKMIN_1600     10    // tCK(MTB)=10, tCK(ns)=1.25
  #define SPD_TCKMIN_1867     9     // tCK(MTB)=9, tCK(ns)=1.07
  #define SPD_TCKMIN_2133     8     // tCK(MTB)=8, tCK(ns)=0.9375
  #define SPD_TCKMIN_2400     7     // tCK(MTB)=7, tCK(ns)=.833
#define SPD_CAS_LT_SUP_LSB  14    // CAS Latencies Supported, Least Significant Byte
#define SPD_CAS_LT_SUP_MSB  15    // CAS Latencies Supported, Most Significant Byte
#define SPD_MIN_TAA         16    // Minimum CAS Latency Time (tAAmin)
#define SPD_MIN_TWR         17    // Minimum Write Recovery Time
#define SPD_MIN_TRCD        18    // Minimum RAS to CAS delay
#define SPD_MIN_TRRD        19    // Minimum Row active to row active delay
#define SPD_MIN_TRP         20    // Minimum Row Precharge time
#define SPD_EXT_TRC_TRAS    21    // Upper nibbles for min tRAS and tRC
#define SPD_MIN_TRAS        22    // Minimum Active to Precharge time
#define SPD_MIN_TRC         23    // Minimum Active to Active/Refresh time
#define SPD_MIN_TRFC_LSB    24    // Minimum Refresh Recovery time least-significant byte
#define SPD_MIN_TRFC_MSB    25    // Minimum Refresh Recovery time most-significant byte
#define SPD_MIN_TWTR        26    // Minimum Internal Write to Read command delay
#define SPD_MIN_TRTP        27    // Minimum Internal Read to Precharge command delay
#define SPD_UN_TFAW         28    // Upper Nibble for tFAW
#define SPD_MIN_TFAW        29    // Minimum Four Activate Window Delay Time (tFAWmin)
#define SPD_OD_SUP          30    // SDRAM Output Drivers Supported
#define SPD_RFSH_OPT        31    // SDRAM Refresh Options
  #define ETR                 BIT0  // Bit location for Extended Temp Range
  #define ETRR                BIT1  // Bit location for Extended Temp Refresh Rate
  #define ASR                 BIT2  // Bit location for Automatic Self Refresh
  #define ODTS                BIT3  // Bit location for On-die Thermal Sensor
#define SPD_DIMM_TS         32    // Module Temperature Sensor
#define SPD_SDRAM_TYPE      33    // SDRAM device type
#define SPD_FTB_TCK         34    // Fine Offset for SDRAM tCK
#define SPD_FTB_TAA         35    // Fine Offset for SDRAM tAA
#define SPD_FTB_TRCD        36    // Fine Offset for SDRAM tRCD
#define SPD_FTB_TRP         37    // Fine Offset for SDRAM tRP
#define SPD_FTB_TRC         38    // Fine Offset for SDRAM tRC
#define SPD_OPT_FEAT        41    // SDRAM Optional Features
  #define SPD_PTRR          BIT7  // Indicates if the DIMM is pTRR compliant

  // UDIMM specific bytes
  // Applicable when Module Type (key byte 3) = 2, 3, 4, 6, or 8

#define SPD_ADDR_MAP_FECTD  63    // Address Mapping from Edge Connector to DRAM

  // RDIMM specific bytes
  // Applicable when Module Type (key byte 3) = 1, 5, or 9

#define SPD_RDIMM_ATTR      63    // RDIMM module attributes
#define SPD_DIMM_HS         64    // Module Heat Spreader Solution
#define SPD_REG_VEN_LSB     65    // Register Vendor ID LSB
#define SPD_REG_VEN_MSB     66    // Register Vendor ID MSB
#define SPD_REG_REV         67    // Register Revision
#define SPD_CNTL_0          69    // Register Control Word 0 & 1
#define SPD_CNTL_1          70    // Register Control Word 2 & 3
#define SPD_CNTL_2          71    // Register Control Word 4 & 5
#define SPD_CNTL_3          72    // Register Control Word 6 & 7   (reserved)
#define SPD_CNTL_4          73    // Register Control Word 8 & 9   (reserved)
#define SPD_CNTL_5          74    // Register Control Word 10 & 11 (reserved)
#define SPD_CNTL_6          75    // Register Control Word 12 & 13 (reserved)
#define SPD_CNTL_7          76    // Register Control Word 14 & 15 (reserved)

  // LRDIMM specific bytes
  // Applicable when Module Type (key byte 3) = 0xB
  // Based on DDR3 SPD 1.0 Document Release 2.1 draft, dated May 27, 2011

#define SPD_LRDIMM_ATTR       63    // LRDIMM module attributes
#define SPD_LRBUF_REV         64    // LR Buffer Revision
#define SPD_LRBUF_VEN_LSB     65    // LR Buffer Vendor ID LSB
#define SPD_LRBUF_VEN_MSB     66    // LR Buffer Vendor ID MSB
#define SPD_LR_F0_RC2_3       67    // LR Buffer Function 0, Control Word 2 & 3
#define SPD_LR_F0_RC4_5       68    // LR Buffer Function 0, Control Word 4 & 5
#define SPD_LR_F1_RC8_11      69    // LR Buffer Function 1, Control Word 8 & 11
#define SPD_LR_F1_RC12_13     70    // LR Buffer Function 1, Control Word 12 & 13
#define SPD_LR_F1_RC14_15     71    // LR Buffer Function 1, Control Word 14 & 15

  // Speed bin 0 = 800 & 1066
#define SPD_LR_SB0_MDQ_DS_ODT       72    // LR Buffer Function 3, Control Word 8 & 9
#define SPD_LR_SB0_DR01_QODT_ACT    73    // LR Buffer Function 3 & 4, Control Word 10 & 11
#define SPD_LR_SB0_DR23_QODT_ACT    74    // LR Buffer Function 5 & 6, Control Word 10 & 11
#define SPD_LR_SB0_DR45_QODT_ACT    75    // LR Buffer Function 7 & 8, Control Word 10 & 11
#define SPD_LR_SB0_DR67_QODT_ACT    76    // LR Buffer Function 9 & 10, Control Word 10 & 11
#define SPD_LR_SB0_MR1_2_RTT        77    // LR Buffer SMBus offsets 0xC0 - 0xC7

  // Speed bin 1 = 1333 & 1600
#define SPD_LR_SB1_MDQ_DS_ODT       78    // LR Buffer Function 3, Control Word 8 & 9
#define SPD_LR_SB1_DR01_QODT_ACT    79    // LR Buffer Function 3 & 4, Control Word 10 & 11
#define SPD_LR_SB1_DR23_QODT_ACT    80    // LR Buffer Function 5 & 6, Control Word 10 & 11
#define SPD_LR_SB1_DR45_QODT_ACT    81    // LR Buffer Function 7 & 8, Control Word 10 & 11
#define SPD_LR_SB1_DR67_QODT_ACT    82    // LR Buffer Function 9 & 10, Control Word 10 & 11
#define SPD_LR_SB1_MR1_2_RTT        83    // LR Buffer SMBus offsets 0xC0 - 0xC7

  // Speed bin 2 = 1866 & 2133
#define SPD_LR_SB2_MDQ_DS_ODT       84    // LR Buffer Function 3, Control Word 8 & 9
#define SPD_LR_SB2_DR01_QODT_ACT    85    // LR Buffer Function 3 & 4, Control Word 10 & 11
#define SPD_LR_SB2_DR23_QODT_ACT    86    // LR Buffer Function 5 & 6, Control Word 10 & 11
#define SPD_LR_SB2_DR45_QODT_ACT    87    // LR Buffer Function 7 & 8, Control Word 10 & 11
#define SPD_LR_SB2_DR67_QODT_ACT    88    // LR Buffer Function 9 & 10, Control Word 10 & 11
#define SPD_LR_SB2_MR1_2_RTT        89    // LR Buffer SMBus offsets 0xC0 - 0xC7

#define SPD_LR_150_MIN_MOD_DELAY    90    // LR DIMM minimum DQ Read propagation delay at 1.5V
#define SPD_LR_150_MAX_MOD_DELAY    91    // LR DIMM maximum DQ Read propagation delay at 1.5V
#define SPD_LR_135_MIN_MOD_DELAY    92    // LR DIMM minimum DQ Read propagation delay at 1.35V
#define SPD_LR_135_MAX_MOD_DELAY    93    // LR DIMM maximum DQ Read propagation delay at 1.35V
#define SPD_LR_12x_MIN_MOD_DELAY    94    // LR DIMM minimum DQ Read propagation delay at 1.2xV
#define SPD_LR_12x_MAX_MOD_DELAY    95    // LR DIMM maximum DQ Read propagation delay at 1.2xV

#define SPD_LR_PERS_BYTE_0          102   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_1          103   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_2          104   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_3          105   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_4          106   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_5          107   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_6          108   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_7          109   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_8          110   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_9          111   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_10         112   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_11         113   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_12         114   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_13         115   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTE_14         116   // LR DIMM Personality Byte
#define SPD_LR_PERS_BYTES_TOTAL     15    // LR DIMM Total number of Personality Bytes

  // End module specific section

#define SPD_MMID_LSB        117   // Module Manufacturer ID Code, Least Significant Byte
#define SPD_MMID_MSB        118   // Module Manufacturer ID Code, Mostst Significant Byte
#define SPD_MM_LOC          119   // Module Manufacturing Location
#define SPD_MM_DATE         120   // Module Manufacturing Date 120-121
#define SPD_MODULE_SN       122   // Module Serial Number 122-125
#define SPD_CRC_LSB         126   // LSB of 16-bit CRC
#define SPD_CRC_MSB         127   // MSB of 16-bit CRC

#define SPD_MODULE_PN       128   // Module Part Number 128-145
#define SPD_MODULE_RC       146   // Module Revision Code 146-147
#define SPD_DRAM_MIDC_LSB   148   // DRAM Manufacturer ID Code, Least Significant Byte
#define SPD_DRAM_MIDC_MSB   149   // DRAM Manufacturer ID Code, Most Significant Byte
#ifdef  MEM_NVDIMM_EN
#define SPD_NVDIMM_ID_N     174   // If NVDIMM value will be 'N'
#define SPD_NVDIMM_ID_V     175   // If NVDIMM value will be 'V'
#endif  //MEM_NVDIMM_EN
#define SPD_BYTE_200        200   // Fixed value 0xBE

  //
  // DDR4 Specific Bytes
  //
#define SPD_SDRAM_TYPE_DDR4   6     // SDRAM Device Type (DDR4)
#define SPD_OPT_FEAT_DDR4     7     // SDRAM Optional Features (DDR4)
  #define SPD_MAC_MASK        BIT0 | BIT1 | BIT2  // Mask for Maximum Active Count field
  #define SPD_TRR_IMMUNE      BIT3  // Indicates this DIMM does not require DRAM Maintenance
#define SPD_RFSH_OPT_DDR4     8     // SDRAM Refresh Options (DDR4)
#define SPD_VDD_DDR4          11    // Vdd DIMM supports (DDR4)
  #define SPD_VDD_120         3     // Module operable and endurant 1.20V
#define SPD_MODULE_ORG_DDR4   12    // Number of Ranks and SDRAM device width (DDR4)
#define SPD_MEM_BUS_WID_DDR4  13    // Width of SDRAM memory bus
#define SPD_DIMM_TS_DDR4      14    // Module Thermal Sensor
#define SPD_TB_DDR4           17    // Timebase [3:2] MTB, [1:0] FTB
#define SPD_MIN_TCK_DDR4      18    // Minimum cycle time
  #define SPD_TCKMIN_DDR4_1600      10  // tCK(MTB)=10, tCK(ns)=1.25
  #define SPD_TCKMIN_DDR4_1866      9   // tCK(MTB)=9, tCK(ns)=1.071
  #define SPD_TCKMIN_DDR4_2133      8   // tCK(MTB)=8, tCK(ns)=.938
  #define SPD_TCKMIN_DDR4_2400      7   // tCK(MTB)=7, tCK(ns)=.833
#define SPD_MAX_TCK_DDR4      19    // Maximum cycle time
#define SPD_CAS_LT_SUP_1_DDR4 20    // CAS Latencies Supported, first byte
#define SPD_CAS_LT_SUP_2_DDR4 21    // CAS Latencies Supported, second byte
#define SPD_CAS_LT_SUP_3_DDR4 22    // CAS Latencies Supported, third byte
#define SPD_CAS_LT_SUP_4_DDR4 23    // CAS Latencies Supported, fourth byte
#define SPD_MIN_TAA_DDR4      24    // Minimum CAS Latency Time (tAAmin)
#define SPD_MIN_TRCD_DDR4     25    // Minimum RAS to CAS delay
#define SPD_MIN_TRP_DDR4      26    // Minimum Row Precharge time
#define SPD_EXT_TRC_TRAS_DDR4 27    // Upper nibbles for min tRAS and tRC
#define SPD_MIN_TRAS_DDR4     28    // Minimum Active to Precharge time
#define SPD_MIN_TRC_DDR4      29    // Minimum Active to Active/Refresh time
#define SPD_MIN_TRFC1_LSB_DDR4 30    // Minimum Refresh Recovery time least-significant byte
#define SPD_MIN_TRFC1_MSB_DDR4 31    // Minimum Refresh Recovery time most-significant byte
#define SPD_MIN_TRFC2_LSB_DDR4 32    // Minimum Refresh Recovery time least-significant byte
#define SPD_MIN_TRFC2_MSB_DDR4 33    // Minimum Refresh Recovery time most-significant byte
#define SPD_MIN_TRFC3_LSB_DDR4 34    // Minimum Refresh Recovery time least-significant byte
#define SPD_MIN_TRFC3_MSB_DDR4 35    // Minimum Refresh Recovery time most-significant byte
#define SPD_TFAW_UPPER_DDR4   36    // Upper nibble for tFAW
#define SPD_MIN_TFAW_DDR4     37    // Minimum For Active Window Delay Time (tFAW)
#define SPD_MIN_TRRDS_DDR4    38    // Minimum Active to Active Delay Time tRRD_S Different Bank Group
#define SPD_MIN_TRRDL_DDR4    39    // Minimum Active to Active Delay Time tRRD_L Same Bank Group
#define SPD_MIN_TCCDL_DDR4    40    // Minimum CAS to CAS Delay Time (tCCD_Lmin), same bank group
#define SPD_FTB_TCCDL_DDR4    117   // Fine offset for tCCD_L
#define SPD_FTB_TRRDL_DDR4    118   // Fine offset for tRRD_L
#define SPD_FTB_TRRDS_DDR4    119   // Fine offset for tRRD_S
#define SPD_FTB_TRC_DDR4      120   // Fine offset for TRC
#define SPD_FTB_TRP_DDR4      121   // Fine offset for TRP
#define SPD_FTB_TRCD_DDR4     122   // Fine offset for TRCD
#define SPD_FTB_TAA_DDR4      123   // Fine offset for TAA
#define SPD_FTB_MAX_TCK_DDR4  124   // Fine offset for max TCK
#define SPD_FTB_MIN_TCK_DDR4  125   // Fine offset for min TCK
#define SPD_MIRROR_UNBUFFERED 131   // Unbuffered:Address Mapping from Edge Connector to DRAM
#define SPD_MIRROR_REGISTERED 136   // Registered:Address Address Mapping from Register to DRAM

#define SPD_MMID_LSB_DDR4     320   // Module Manufacturer ID Code, Least Significant Byte
#define SPD_MMID_MSB_DDR4     321   // Module Manufacturer ID Code, Most Significant Byte
#define SPD_MM_LOC_DDR4       322   // Module Manufacturing Location
#define SPD_MM_DATE_DDR4      323   // Module Manufacturing Date 323-324
#define SPD_MODULE_SN_DDR4    325   // Module Serial Number 325-328
#define SPD_MODULE_PN_DDR4    329   // Module Part Number 329-348
#define SPD_MODULE_RC_DDR4    349   // Module Revision Code
#define SPD_DRAM_MIDC_LSB_DDR4 350  // DRAM Manufacturer ID Code, Least Significant Byte
#define SPD_DRAM_MIDC_MSB_DDR4 351  // DRAM Manufacturer ID Code, Most Significant Byte
#define SPD_DRAM_REV_DDR4     352   // DRAM Revision ID
#define SPD_CRC_LSB_DDR4      382   // LSB of 16-bit CRC
#define SPD_CRC_MSB_DDR4      383   // MSB of 16-bit CRC

  // Begin DDR4 module specific section
#define SPD_MODULE_NH_DDR4    128    // Module Nominal Height
#define SPD_MODULE_MT_DDR4    129    // Module Maximum Thickness
#define SPD_REF_RAW_CARD_DDR4 130    // Reference Raw Card Used

  // UDIMM specific bytes
  // Applicable when Module Type (key byte 3) = 2
#define SPD_ADDR_MAP_FECTD_DDR4 131    // Address Mapping from Edge Connector to DRAM

  // RDIMM specific bytes
  // Applicable when Module Type (key byte 3) = 1
#define SPD_RDIMM_ATTR_DDR4    131   // RDIMM module attributes
#define SPD_DIMM_HS_DDR4       132   // Module Heat Spreader Solution
#define SPD_REG_VEN_LSB_DDR4   133   // Register Vendor ID LSB
#define SPD_REG_VEN_MSB_DDR4   134   // Register Vendor ID MSB
#define SPD_REG_REV_DDR4       135   // Register Revision
#define SPD_ADD_MAPPING_DDR4   136   // Address mapping from Reg to DRAM
#define SPD_REG_OD_CTL_DDR4    137   // Register Output Drive Strength for Control
#define SPD_REG_OD_CK_DDR4     138   // Register Output Drive Strength for Clock

  // LRDIMM specific bytes
  // Applicable when Module Type (key byte 3) = 0x4
#define SPD_LRDIMM_ATTR_DDR4   131   // LRDIMM module attributes
#define SPD_LRBUF_HS_DDR4      132   // LR Buffer Heat Spreader Solution
#define SPD_LRBUF_VEN_LSB_DDR4 133   // LR Buffer Vendor ID LSB
#define SPD_LRBUF_VEN_MSB_DDR4 134   // LR Buffer Vendor ID MSB
#define SPD_LRBUF_REV_DDR4     135   // LR Buffer Register Revision
#define SPD_LRBUF_DB_REV_DDR4  139   // LR Buffer Data Buffer Revision
#define SPD_LRBUF_DRAM_VREFDQ_R0_DDR4 140 // LR Buffer DRAM VrefDQ for Package Rank 0
#define SPD_LRBUF_DRAM_VREFDQ_R1_DDR4 141 // LR Buffer DRAM VrefDQ for Package Rank 1
#define SPD_LRBUF_DRAM_VREFDQ_R2_DDR4 142 // LR Buffer DRAM VrefDQ for Package Rank 2
#define SPD_LRBUF_DRAM_VREFDQ_R3_DDR4 143 // LR Buffer DRAM VrefDQ for Package Rank 3
#define SPD_LRBUF_DB_VREFDQ_DDR4 144 // LR Data Buffer VrefDQ for DRAM Interface
#define SPD_LRBUF_DB_DS_RTT_LE1866_DDR4 145 // LR Data Buffer MDQ Drive Strength and RTT for data rate <= 1866
#define SPD_LRBUF_DB_DS_RTT_GT1866_LE2400_DDR4 146 // LR Data Buffer MDQ Drive Strength and RTT for data rate > 1866 and <= 2400
#define SPD_LRBUF_DB_DS_RTT_GT2400_LE3200_DDR4 147 // LR Data Buffer MDQ Drive Strength and RTT for data rate > 2400 and <= 3200
#define SPD_LRBUF_DRAM_DS_DDR4 148   // LR Buffer DRAM Drive Strength (for data rates <= 1866, 1866 < data rate <= 2400, and 2400 < data rate <= 3200)
#define SPD_LRBUF_DRAM_ODT_WR_NOM_LE1866_DDR4 149 // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate <= 1866
#define SPD_LRBUF_DRAM_ODT_WR_NOM_GT1866_LE2400_DDR4 150 // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 1866 and <= 2400
#define SPD_LRBUF_DRAM_ODT_WR_NOM_GT2400_LE3200_DDR4 151 // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 2400 and <= 3200
#define SPD_LRBUF_DRAM_ODT_PARK_LE1866_DDR4 152 // LR Buffer DRAM ODT (RTT_PARK) for data rate <= 1866
#define SPD_LRBUF_DRAM_ODT_PARK_GT1866_LE2400_DDR4 153 // LR Buffer DRAM ODT (RTT_PARK) for data rate > 1866 and <= 2400
#define SPD_LRBUF_DRAM_ODT_PARK_GT2400_LE3200_DDR4 154 // LR Buffer DRAM ODT (RTT_PARK) for data rate > 2400 and <= 3200

  //
  // End DDR4 Specific Bytes
  //
#define BANK0   0
#define BANK1   BIT0
#define BANK2   BIT1
#define BANK3   BIT0 + BIT1
#define BANK4   BIT2
#define BANK5   BIT2 + BIT0
#define BANK6   BIT2 + BIT1
#define BANK7   BIT2 + BIT1 + BIT0

#define RDIMM_RC00     0x00
#define RDIMM_RC01     0x01
#define RDIMM_RC02     0x02
#define RDIMM_RC03     0x03
#define RDIMM_RC04     0x04
#define RDIMM_RC05     0x05
#define RDIMM_RC08     0x08
#define RDIMM_RC09     0x09
#define RDIMM_RC0A     0x0A
#define RDIMM_RC0B     0x0B
#define RDIMM_RC0C     0x0C
#define RDIMM_RC0D     0x0D
#define RDIMM_RC0E     0x0E
#define RDIMM_RC0F     0x0F
#define RDIMM_RC1x     0x10
#define RDIMM_RC2x     0x20
#define RDIMM_RC3x     0x30
#define RDIMM_RC4x     0x40
#define RDIMM_RC5x     0x50
#define RDIMM_RC6x     0x60
#define RDIMM_RC7x     0x70
#define RDIMM_RC8x     0x80
#define RDIMM_RC9x     0x90
#define RDIMM_RCAx     0xA0

#define LRDIMM_BC00    0x00
#define LRDIMM_BC01    0x01
#define LRDIMM_BC02    0x02
#define LRDIMM_BC03    0x03
#define LRDIMM_BC04    0x04
#define LRDIMM_BC05    0x05
#define LRDIMM_BC06    0x06
#define LRDIMM_BC07    0x07
#define LRDIMM_BC08    0x08
#define LRDIMM_BC09    0x09
#define LRDIMM_BC0A    0x0A
#define LRDIMM_BC0B    0x0B
#define LRDIMM_BC0C    0x0C
#define LRDIMM_BC0E    0x0E

#define LRDIMM_BC0x    0x00
#define LRDIMM_BC1x    0x10
#define LRDIMM_BC2x    0x20
#define LRDIMM_BC3x    0x30
#define LRDIMM_BC4x    0x40
#define LRDIMM_BC5x    0x50
#define LRDIMM_BC6x    0x60
#define LRDIMM_BC7x    0x70
#define LRDIMM_BC8x    0x80
#define LRDIMM_BC9x    0x90
#define LRDIMM_BCAx    0xA0
#define LRDIMM_BCBx    0xB0
#define LRDIMM_BCCx    0xC0
#define LRDIMM_BCDx    0xD0
#define LRDIMM_BCEx    0xE0
#define LRDIMM_BCFx    0xF0
#define LRDIMM_F0      0x0
#define LRDIMM_F1      0x1
#define LRDIMM_F5      0x5
#define LRDIMM_F6      0x6
#define LRDIMM_F7      0x7
#define LRDIMM_F8      0x8
#define LRDIMM_F9      0x9

#endif /* _HW_MEM_INIT_LIB_H_ */
