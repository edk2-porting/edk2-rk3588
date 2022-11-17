/** @file
  SPD data format header file.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MrcSpdData_h_
#define _MrcSpdData_h_
#pragma pack (push, 1)

#include "MrcTypes.h"

#define MAX_XMP_PROFILES  (2)
#define SPD3_MANUF_SIZE   (SPD3_MANUF_END - SPD3_MANUF_START + 1)   ///< The size of the SPD manufacturing data.
#define SPD4_MANUF_SIZE   (SPD4_MANUF_END - SPD4_MANUF_START + 1)   ///< The size of the SPD manufacturing data.
#define SPDLP_MANUF_SIZE  (SPDLP_MANUF_END - SPDLP_MANUF_START + 1) ///< The size of the SPD manufacturing data

typedef union {
  struct {
    UINT8  BytesUsed                           :  4; ///< Bits 3:0
    UINT8  BytesTotal                          :  3; ///< Bits 6:4
    UINT8  CrcCoverage                         :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_DEVICE_DESCRIPTION_STRUCT;

typedef union {
  struct {
    UINT8  Minor                               :  4; ///< Bits 3:0
    UINT8  Major                               :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_REVISION_STRUCT;

typedef union {
  struct {
    UINT8  Type                                :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_DRAM_DEVICE_TYPE_STRUCT;

typedef union {
  struct {
    UINT8  ModuleType                          :  4; ///< Bits 3:0
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_MODULE_TYPE_STRUCT;

typedef union {
  struct {
    UINT8  Density                             :  4; ///< Bits 3:0
    UINT8  BankAddress                         :  3; ///< Bits 6:4
    UINT8                                      :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_SDRAM_DENSITY_BANKS_STRUCT;

typedef union {
  struct {
    UINT8  ColumnAddress                       :  3; ///< Bits 2:0
    UINT8  RowAddress                          :  3; ///< Bits 5:3
    UINT8                                      :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_SDRAM_ADDRESSING_STRUCT;

typedef union {
  struct {
    UINT8  OperationAt1_50                     :  1; ///< Bits 0:0
    UINT8  OperationAt1_35                     :  1; ///< Bits 1:1
    UINT8  OperationAt1_25                     :  1; ///< Bits 2:2
    UINT8                                      :  5; ///< Bits 7:3
  } Bits;
  UINT8  Data;
} SPD_MODULE_NOMINAL_VOLTAGE_STRUCT;

typedef union {
  struct {
    UINT8  SdramDeviceWidth                    :  3; ///< Bits 2:0
    UINT8  RankCount                           :  3; ///< Bits 5:3
    UINT8                                      :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_MODULE_ORGANIZATION_STRUCT;

typedef union {
  struct {
    UINT8  PrimaryBusWidth                     :  3; ///< Bits 2:0
    UINT8  BusWidthExtension                   :  2; ///< Bits 4:3
    UINT8                                      :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD_MODULE_MEMORY_BUS_WIDTH_STRUCT;

typedef union {
  struct {
    UINT8  Divisor                             :  4; ///< Bits 3:0
    UINT8  Dividend                            :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_FINE_TIMEBASE_STRUCT;

typedef union {
  struct {
    UINT8  Dividend                            :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_MEDIUM_TIMEBASE_DIVIDEND_STRUCT;

typedef union {
  struct {
    UINT8  Divisor                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_MEDIUM_TIMEBASE_DIVISOR_STRUCT;

typedef struct {
  SPD_MEDIUM_TIMEBASE_DIVIDEND_STRUCT Dividend; ///< Medium Timebase (MTB) Dividend
  SPD_MEDIUM_TIMEBASE_DIVISOR_STRUCT  Divisor;  ///< Medium Timebase (MTB) Divisor
} SPD_MEDIUM_TIMEBASE;

typedef union {
  struct {
    UINT8  tCKmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TCK_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT16 CL4                                 :  1; ///< Bits 0:0
    UINT16 CL5                                 :  1; ///< Bits 1:1
    UINT16 CL6                                 :  1; ///< Bits 2:2
    UINT16 CL7                                 :  1; ///< Bits 3:3
    UINT16 CL8                                 :  1; ///< Bits 4:4
    UINT16 CL9                                 :  1; ///< Bits 5:5
    UINT16 CL10                                :  1; ///< Bits 6:6
    UINT16 CL11                                :  1; ///< Bits 7:7
    UINT16 CL12                                :  1; ///< Bits 8:8
    UINT16 CL13                                :  1; ///< Bits 9:9
    UINT16 CL14                                :  1; ///< Bits 10:10
    UINT16 CL15                                :  1; ///< Bits 11:11
    UINT16 CL16                                :  1; ///< Bits 12:12
    UINT16 CL17                                :  1; ///< Bits 13:13
    UINT16 CL18                                :  1; ///< Bits 14:14
    UINT16                                     :  1; ///< Bits 15:15
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_CAS_LATENCIES_SUPPORTED_STRUCT;

typedef union {
  struct {
    UINT8  tAAmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TAA_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tWRmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TWR_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRCDmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRCD_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRRDmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRRD_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRPmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRP_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRPab                               :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRP_AB_MTB_STRUCT;

typedef union {
  struct {
    INT8  tRPabFine                            :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRP_AB_FTB_STRUCT;

typedef union {
  struct {
    UINT8  tRPpb                               :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRP_PB_MTB_STRUCT;

typedef union {
  struct {
    INT8  tRPpbFine                            :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRP_PB_FTB_STRUCT;

typedef union {
  struct {
    UINT16  tRFCab                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_TRFC_AB_MTB_STRUCT;

typedef union {
struct {
    UINT16  tRFCpb                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_TRFC_PB_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRASminUpper                        :  4; ///< Bits 3:0
    UINT8  tRCminUpper                         :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_TRAS_TRC_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRASmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRAS_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRCmin                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRC_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT16 tRFCmin                             :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_TRFC_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tWTRmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TWTR_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tRTPmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TRTP_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tFAWminUpper                        :  4; ///< Bits 3:0
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_TFAW_MIN_MTB_UPPER_STRUCT;

typedef union {
  struct {
    UINT8  tFAWmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TFAW_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  tCWLmin                             :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_TCWL_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  NMode                               :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_SYSTEM_COMMAND_RATE_STRUCT;

typedef union {
  struct {
    UINT16 tREFImin                            :  16; ///< Bits 15:0
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_TREFI_MIN_MTB_STRUCT;

typedef union {
  struct {
    UINT8  RZQ6                                :  1; ///< Bits 0:0
    UINT8  RZQ7                                :  1; ///< Bits 1:1
    UINT8                                      :  5; ///< Bits 6:2
    UINT8  DllOff                              :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_SDRAM_OPTIONAL_FEATURES_STRUCT;

typedef union {
  struct {
    UINT8  ExtendedTemperatureRange            :  1; ///< Bits 0:0
    UINT8  ExtendedTemperatureRefreshRate      :  1; ///< Bits 1:1
    UINT8  AutoSelfRefresh                     :  1; ///< Bits 2:2
    UINT8  OnDieThermalSensor                  :  1; ///< Bits 3:3
    UINT8                                      :  3; ///< Bits 6:4
    UINT8  PartialArraySelfRefresh             :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_SDRAM_THERMAL_REFRESH_STRUCT;

typedef union {
  struct {
    UINT8  ThermalSensorAccuracy               :  7; ///< Bits 6:0
    UINT8  ThermalSensorPresence               :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_MODULE_THERMAL_SENSOR_STRUCT;

typedef union {
  struct {
    UINT8  NonStandardDeviceDescription        :  7; ///< Bits 6:0
    UINT8  SdramDeviceType                     :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_SDRAM_DEVICE_TYPE_STRUCT;

typedef union {
  struct {
    UINT8                                      :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_AUTO_SELF_REFRESH_PERF_STRUCT;

typedef union {
  struct {
    INT8  tCKminFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TCK_MIN_FTB_STRUCT;

typedef union {
  struct {
    INT8  tAAminFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TAA_MIN_FTB_STRUCT;

typedef union {
  struct {
    INT8  tRCDminFine                          :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRCD_MIN_FTB_STRUCT;

typedef union {
  struct {
    INT8  tRPminFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRP_MIN_FTB_STRUCT;

typedef union {
  struct {
    INT8  tRCminFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRC_MIN_FTB_STRUCT;

typedef union {
  struct {
    UINT8  tMACencoding                        :  4; ///< Bits 3:0
    UINT8  tMAWencoding                        :  2; ///< Bits 5:4
    UINT8  Reserved                            :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_PTRR_SUPPORT_STRUCT;

typedef union {
  struct {
    INT8  tRRDminFine                          :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_TRRD_MIN_FTB_STRUCT;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8  RawCardExtension                    :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD_UNBUF_MODULE_NOMINAL_HEIGHT;

typedef union {
  struct {
    UINT8  FrontThickness                      :  4; ///< Bits 3:0
    UINT8  BackThickness                       :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_UNBUF_MODULE_NOMINAL_THICKNESS;

typedef union {
  struct {
    UINT8  Card                                :  5; ///< Bits 4:0
    UINT8  Revision                            :  2; ///< Bits 6:5
    UINT8  Extension                           :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_UNBUF_REFERENCE_RAW_CARD;

typedef union {
  struct {
    UINT8  MappingRank1                        :  1; ///< Bits 0:0
    UINT8                                      :  7; ///< Bits 7:1
  } Bits;
  UINT8  Data;
} SPD_UNBUF_ADDRESS_MAPPING;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8                                      :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD_RDIMM_MODULE_NOMINAL_HEIGHT;

typedef union {
  struct {
    UINT8  FrontThickness                      :  4; ///< Bits 3:0
    UINT8  BackThickness                       :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_RDIMM_MODULE_NOMINAL_THICKNESS;

typedef union {
  struct {
    UINT8  Card                                :  5; ///< Bits 4:0
    UINT8  Revision                            :  2; ///< Bits 6:5
    UINT8  Extension                           :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_RDIMM_REFERENCE_RAW_CARD;

typedef union {
  struct {
    UINT8  RegisterCount                       :  2; ///< Bits 1:0
    UINT8  DramRowCount                        :  2; ///< Bits 3:2
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_RDIMM_MODULE_ATTRIBUTES;

typedef union {
  struct {
    UINT16 ContinuationCount                   :  7; ///< Bits 6:0
    UINT16 ContinuationParity                  :  1; ///< Bits 7:7
    UINT16 LastNonZeroByte                     :  8; ///< Bits 15:8
  } Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SPD_MANUFACTURER_ID_CODE;

typedef struct {
  UINT8  Year;                                 ///< Year represented in BCD (00h = 2000)
  UINT8  Week;                                 ///< Year represented in BCD (47h = week 47)
} SPD_MANUFACTURING_DATE;

typedef union {
  UINT32 Data;
  UINT16 SerialNumber16[2];
  UINT8  SerialNumber8[4];
} SPD_MANUFACTURER_SERIAL_NUMBER;

typedef struct {
  UINT8 Location;                              ///< Module Manufacturing Location
} SPD_MANUFACTURING_LOCATION;

typedef struct {
  SPD_MANUFACTURER_ID_CODE            IdCode;                   ///< Module Manufacturer ID Code
  SPD_MANUFACTURING_LOCATION          Location;                 ///< Module Manufacturing Location
  SPD_MANUFACTURING_DATE              Date;                     ///< Module Manufacturing Year, in BCD (range: 2000-2255)
  SPD_MANUFACTURER_SERIAL_NUMBER      SerialNumber;             ///< Module Serial Number
} SPD_UNIQUE_MODULE_ID;

typedef union {
  UINT16 Crc[1];
  UINT8  Data8[2];
} SPD_CYCLIC_REDUNDANCY_CODE;

typedef union {
  struct {
    UINT8  ProfileEnable1                :  1;                     ///< Bits 0:0
    UINT8  ProfileEnable2                :  1;                     ///< Bits 1:1
    UINT8  ProfileConfig1                :  2;                     ///< Bits 3:2
    UINT8  ProfileConfig2                :  2;                     ///< Bits 5:4
    UINT8                                :  2;                     ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_XMP_ORG_CONFIG;

typedef struct {
  UINT16                              XmpId;                    ///< 176-177 XMP Identification String
  SPD_XMP_ORG_CONFIG                  XmpOrgConf;               ///< 178 XMP Organization & Configuration
  SPD_REVISION_STRUCT                 XmpRevision;              ///< 179 XMP Revision
  SPD_MEDIUM_TIMEBASE                 MediumTimeBase[MAX_XMP_PROFILES]; ///< 180-183 Medium Timebase (MTB)
  SPD_FINE_TIMEBASE_STRUCT            FineTimeBase;             ///< 184 Fine Timebase (FTB) Dividend / Divisor
} SPD_EXTREME_MEMORY_PROFILE_HEADER;

typedef union {
  struct {
    UINT8  Decimal : 5;
    UINT8  Integer : 2;
    UINT8          : 1;
  } Bits;
  UINT8 Data;
} SPD_VDD_VOLTAGE_LEVEL_STRUCT;

typedef union {
  struct {
    UINT8  Decimal : 7;
    UINT8  Integer : 1;
  } Bits;
  UINT8 Data;
} SPD_VDD_VOLTAGE_LEVEL_STRUCT_2_0;

typedef union {
  struct {
    UINT8  Fine                                :  2; ///< Bits 1:0
    UINT8  Medium                              :  2; ///< Bits 3:2
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD4_TIMEBASE_STRUCT;

typedef union {
  struct {
    UINT32 CL7                                 :  1; ///< Bits 0:0
    UINT32 CL8                                 :  1; ///< Bits 1:1
    UINT32 CL9                                 :  1; ///< Bits 2:2
    UINT32 CL10                                :  1; ///< Bits 3:3
    UINT32 CL11                                :  1; ///< Bits 4:4
    UINT32 CL12                                :  1; ///< Bits 5:5
    UINT32 CL13                                :  1; ///< Bits 6:6
    UINT32 CL14                                :  1; ///< Bits 7:7
    UINT32 CL15                                :  1; ///< Bits 8:8
    UINT32 CL16                                :  1; ///< Bits 9:9
    UINT32 CL17                                :  1; ///< Bits 10:10
    UINT32 CL18                                :  1; ///< Bits 11:11
    UINT32 CL19                                :  1; ///< Bits 12:12
    UINT32 CL20                                :  1; ///< Bits 13:13
    UINT32 CL21                                :  1; ///< Bits 14:14
    UINT32 CL22                                :  1; ///< Bits 15:15
    UINT32 CL23                                :  1; ///< Bits 16:16
    UINT32 CL24                                :  1; ///< Bits 17:17
    UINT32                                     :  14; ///< Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SPD4_CAS_LATENCIES_SUPPORTED_STRUCT;

typedef struct {
  SPD_VDD_VOLTAGE_LEVEL_STRUCT        Vdd;                      ///< 185, 220 XMP Module VDD Voltage Level
  SPD_TCK_MIN_MTB_STRUCT              tCKmin;                   ///< 186, 221 XMP SDRAM Minimum Cycle Time (tCKmin)
  SPD_TAA_MIN_MTB_STRUCT              tAAmin;                   ///< 187, 222 XMP Minimum CAS Latency Time (tAAmin)
  SPD_CAS_LATENCIES_SUPPORTED_STRUCT  CasLatencies;             ///< 188-189, 223-224 XMP CAS Latencies Supported, Least Significant Byte
  SPD_TCWL_MIN_MTB_STRUCT             tCWLmin;                  ///< 190, 225 XMP Minimum CAS Write Latency Time (tCWLmin)
  SPD_TRP_MIN_MTB_STRUCT              tRPmin;                   ///< 191, 226 XMP Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRCD_MIN_MTB_STRUCT             tRCDmin;                  ///< 192, 227 XMP Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TWR_MIN_MTB_STRUCT              tWRmin;                   ///< 193, 228 XMP Minimum Write Recovery Time (tWRmin)
  SPD_TRAS_TRC_MIN_MTB_STRUCT         tRASMintRCMinUpper;       ///< 194, 229 XMP Upper Nibbles for tRAS and tRC
  SPD_TRAS_MIN_MTB_STRUCT             tRASmin;                  ///< 195, 230 XMP Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte
  SPD_TRC_MIN_MTB_STRUCT              tRCmin;                   ///< 196, 231 XMP Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte
  SPD_TREFI_MIN_MTB_STRUCT            tREFImin;                 ///< 197-198, 232-233 XMP Maximum tREFI Time (Average Periodic Refresh Interval), Least Significant Byte
  SPD_TRFC_MIN_MTB_STRUCT             tRFCmin;                  ///< 199-200, 234-235 XMP Minimum Refresh Recovery Delay Time (tRFCmin), Least Significant Byte
  SPD_TRTP_MIN_MTB_STRUCT             tRTPmin;                  ///< 201, 236 XMP Minimum Internal Read to Precharge Command Delay Time (tRTPmin)
  SPD_TRRD_MIN_MTB_STRUCT             tRRDmin;                  ///< 202, 237 XMP Minimum Row Active to Row Active Delay Time (tRRDmin)
  SPD_TFAW_MIN_MTB_UPPER_STRUCT       tFAWMinUpper;             ///< 203, 238 XMP Upper Nibble for tFAW
  SPD_TFAW_MIN_MTB_STRUCT             tFAWmin;                  ///< 204, 239 XMP Minimum Four Activate Window Delay Time (tFAWmin)
  SPD_TWTR_MIN_MTB_STRUCT             tWTRmin;                  ///< 205, 240 XMP Minimum Internal Write to Read Command Delay Time (tWTRmin)
  UINT8                               Reserved1[207 - 206 + 1]; ///< 206-207, 241-242 XMP Reserved
  SPD_SYSTEM_COMMAND_RATE_STRUCT      SystemCmdRate;            ///< 208, 243 XMP System ADD/CMD Rate (1N or 2N mode)
  SPD_AUTO_SELF_REFRESH_PERF_STRUCT   AsrPerf;                  ///< 209, 244 XMP SDRAM Auto Self Refresh Performance (Sub 1x Refresh and IDD6 impact)
  UINT8                               VoltageLevel;             ///< 210, 245 XMP Memory Controller Voltage Level
  SPD_TCK_MIN_FTB_STRUCT              tCKminFine;               ///< 211, 246 XMP Fine Offset for SDRAM Minimum Cycle Time (tCKmin)
  SPD_TAA_MIN_FTB_STRUCT              tAAminFine;               ///< 212, 247 XMP Fine Offset for Minimum CAS Latency Time (tAAmin)
  SPD_TRP_MIN_FTB_STRUCT              tRPminFine;               ///< 213, 248 XMP Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRCD_MIN_FTB_STRUCT             tRCDminFine;              ///< 214, 249 XMP Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRC_MIN_FTB_STRUCT              tRCminFine;               ///< 215, 250 XMP Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)
  UINT8                               Reserved2[218 - 216 + 1]; ///< 216-218, 251-253 XMP Reserved
  UINT8                               VendorPersonality;        ///< 219, 254 XMP Vendor Personality
} SPD_EXTREME_MEMORY_PROFILE_DATA;

typedef struct {
  SPD_EXTREME_MEMORY_PROFILE_HEADER   Header;                   ///< 176-184 XMP header
  SPD_EXTREME_MEMORY_PROFILE_DATA     Data[MAX_XMP_PROFILES];   ///< 185-254 XMP profiles
} SPD_EXTREME_MEMORY_PROFILE;

typedef struct {
  UINT16                              XmpId;                        ///< 384-385 XMP Identification String
  SPD_XMP_ORG_CONFIG                  XmpOrgConf;                   ///< 386 XMP Organization & Configuration
  SPD_REVISION_STRUCT                 XmpRevision;                  ///< 387 XMP Revision
  SPD4_TIMEBASE_STRUCT                TimeBase[MAX_XMP_PROFILES];   ///< 388-389 Medium and Fine Timebase
  UINT8                               Reserved[392 - 390 + 1];     ///< 390-392 Reserved
} SPD_EXTREME_MEMORY_PROFILE_HEADER_2_0;

typedef struct {
  SPD_VDD_VOLTAGE_LEVEL_STRUCT_2_0    Vdd;                      ///< 393, 440 XMP Module VDD Voltage Level
  UINT8                               Reserved1[395 - 394 + 1]; ///< 394-395, 441-442 XMP Reserved
  SPD_TCK_MIN_MTB_STRUCT              tCKAVGmin;                ///< 396, 443 XMP SDRAM Minimum Cycle Time (tCKAVGmin)
  SPD4_CAS_LATENCIES_SUPPORTED_STRUCT CasLatencies;             ///< 397-400, 444-447 XMP CAS Latencies Supported
  SPD_TAA_MIN_MTB_STRUCT              tAAmin;                   ///< 401, 448 XMP Minimum CAS Latency Time (tAAmin)
  SPD_TRCD_MIN_MTB_STRUCT             tRCDmin;                  ///< 402, 449 XMP Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRP_MIN_MTB_STRUCT              tRPmin;                   ///< 403, 450 XMP Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRAS_TRC_MIN_MTB_STRUCT         tRASMintRCMinUpper;       ///< 404, 451 XMP Upper Nibbles for tRAS and tRC
  SPD_TRAS_MIN_MTB_STRUCT             tRASmin;                  ///< 405, 452 XMP Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte
  SPD_TRC_MIN_MTB_STRUCT              tRCmin;                   ///< 406, 453 XMP Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte
  SPD_TRFC_MIN_MTB_STRUCT             tRFC1min;                 ///< 407-408, 454-455 XMP Minimum Refresh Recovery Delay Time (tRFC1min)
  SPD_TRFC_MIN_MTB_STRUCT             tRFC2min;                 ///< 409-410, 456-457 XMP Minimum Refresh Recovery Delay Time (tRFC2min)
  SPD_TRFC_MIN_MTB_STRUCT             tRFC4min;                 ///< 411-412, 458-459 XMP Minimum Refresh Recovery Delay Time (tRFC4min)
  SPD_TFAW_MIN_MTB_UPPER_STRUCT       tFAWMinUpper;             ///< 413, 460 Upper Nibble for tFAW
  SPD_TFAW_MIN_MTB_STRUCT             tFAWmin;                  ///< 414, 461 Minimum Four Activate Window Delay Time (tFAWmin)
  SPD_TRRD_MIN_MTB_STRUCT             tRRD_Smin;                ///< 415, 462 Minimum Activate to Activate Delay Time (tRRD_Smin), different bank group
  SPD_TRRD_MIN_MTB_STRUCT             tRRD_Lmin;                ///< 416, 463 Minimum Activate to Activate Delay Time (tRRD_Lmin), same bank group
  UINT8                               Reserved2[424 - 417 + 1]; ///< 417-424, 464-471 XMP Reserved
  SPD_TRRD_MIN_FTB_STRUCT             tRRD_LminFine;            ///< 425, 472 Fine Offset for Minimum Activate to Activate Delay Time (tRRD_Lmin), different bank group
  SPD_TRRD_MIN_FTB_STRUCT             tRRD_SminFine;            ///< 426, 473 Fine Offset for Minimum Activate to Activate Delay Time (tRRD_Smin), same bank group
  SPD_TRC_MIN_FTB_STRUCT              tRCminFine;               ///< 427, 474 Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)
  SPD_TRP_MIN_FTB_STRUCT              tRPminFine;               ///< 428, 475 Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRCD_MIN_FTB_STRUCT             tRCDminFine;              ///< 429, 476 Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TAA_MIN_FTB_STRUCT              tAAminFine;               ///< 430, 477 Fine Offset for Minimum CAS Latency Time (tAAmin)
  SPD_TCK_MIN_FTB_STRUCT              tCKAVGminFine;            ///< 431, 478 Fine Offset for SDRAM Maximum Cycle Time (tCKAVGmin)
  UINT8                               Reserved3[439 - 432 + 1]; ///< 432-439, 479-486 XMP Reserved
} SPD_EXTREME_MEMORY_PROFILE_DATA_2_0;

typedef struct {
  SPD_EXTREME_MEMORY_PROFILE_HEADER_2_0      Header;                   ///< 384-392 XMP header
  SPD_EXTREME_MEMORY_PROFILE_DATA_2_0        Data[MAX_XMP_PROFILES];   ///< 393-486 XMP profiles
} SPD_EXTREME_MEMORY_PROFILE_2_0;

typedef struct {
  SPD_DEVICE_DESCRIPTION_STRUCT       Description;              ///< 0   Number of Serial PD Bytes Written / SPD Device Size / CRC Coverage 1, 2
  SPD_REVISION_STRUCT                 Revision;                 ///< 1   SPD Revision
  SPD_DRAM_DEVICE_TYPE_STRUCT         DramDeviceType;           ///< 2   DRAM Device Type
  SPD_MODULE_TYPE_STRUCT              ModuleType;               ///< 3   Module Type
  SPD_SDRAM_DENSITY_BANKS_STRUCT      SdramDensityAndBanks;     ///< 4   SDRAM Density and Banks
  SPD_SDRAM_ADDRESSING_STRUCT         SdramAddressing;          ///< 5   SDRAM Addressing
  SPD_MODULE_NOMINAL_VOLTAGE_STRUCT   ModuleNominalVoltage;     ///< 6   Module Nominal Voltage, VDD
  SPD_MODULE_ORGANIZATION_STRUCT      ModuleOrganization;       ///< 7   Module Organization
  SPD_MODULE_MEMORY_BUS_WIDTH_STRUCT  ModuleMemoryBusWidth;     ///< 8   Module Memory Bus Width
  SPD_FINE_TIMEBASE_STRUCT            FineTimebase;             ///< 9   Fine Timebase (FTB) Dividend / Divisor
  SPD_MEDIUM_TIMEBASE                 MediumTimebase;           ///< 10-11 Medium Timebase (MTB) Dividend
  SPD_TCK_MIN_MTB_STRUCT              tCKmin;                   ///< 12  SDRAM Minimum Cycle Time (tCKmin)
  UINT8                               Reserved1;                ///< 13  Reserved
  SPD_CAS_LATENCIES_SUPPORTED_STRUCT  CasLatencies;             ///< 14-15 CAS Latencies Supported
  SPD_TAA_MIN_MTB_STRUCT              tAAmin;                   ///< 16  Minimum CAS Latency Time (tAAmin)
  SPD_TWR_MIN_MTB_STRUCT              tWRmin;                   ///< 17  Minimum Write Recovery Time (tWRmin)
  SPD_TRCD_MIN_MTB_STRUCT             tRCDmin;                  ///< 18  Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRRD_MIN_MTB_STRUCT             tRRDmin;                  ///< 19  Minimum Row Active to Row Active Delay Time (tRRDmin)
  SPD_TRP_MIN_MTB_STRUCT              tRPmin;                   ///< 20  Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRAS_TRC_MIN_MTB_STRUCT         tRASMintRCMinUpper;       ///< 21  Upper Nibbles for tRAS and tRC
  SPD_TRAS_MIN_MTB_STRUCT             tRASmin;                  ///< 22  Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte
  SPD_TRC_MIN_MTB_STRUCT              tRCmin;                   ///< 23  Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte
  SPD_TRFC_MIN_MTB_STRUCT             tRFCmin;                  ///< 24-25  Minimum Refresh Recovery Delay Time (tRFCmin)
  SPD_TWTR_MIN_MTB_STRUCT             tWTRmin;                  ///< 26  Minimum Internal Write to Read Command Delay Time (tWTRmin)
  SPD_TRTP_MIN_MTB_STRUCT             tRTPmin;                  ///< 27  Minimum Internal Read to Precharge Command Delay Time (tRTPmin)
  SPD_TFAW_MIN_MTB_UPPER_STRUCT       tFAWMinUpper;             ///< 28  Upper Nibble for tFAW
  SPD_TFAW_MIN_MTB_STRUCT             tFAWmin;                  ///< 29  Minimum Four Activate Window Delay Time (tFAWmin)
  SPD_SDRAM_OPTIONAL_FEATURES_STRUCT  SdramOptionalFeatures;    ///< 30  SDRAM Optional Features
  SPD_SDRAM_THERMAL_REFRESH_STRUCT    ThermalAndRefreshOptions; ///< 31  SDRAMThermalAndRefreshOptions
  SPD_MODULE_THERMAL_SENSOR_STRUCT    ModuleThermalSensor;      ///< 32  Module Thermal Sensor
  SPD_SDRAM_DEVICE_TYPE_STRUCT        SdramDeviceType;          ///< 33  SDRAM Device Type
  SPD_TCK_MIN_FTB_STRUCT              tCKminFine;               ///< 34  Fine Offset for SDRAM Minimum Cycle Time (tCKmin)
  SPD_TAA_MIN_FTB_STRUCT              tAAminFine;               ///< 35  Fine Offset for Minimum CAS Latency Time (tAAmin)
  SPD_TRCD_MIN_FTB_STRUCT             tRCDminFine;              ///< 36  Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRP_MIN_FTB_STRUCT              tRPminFine;               ///< 37  Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRC_MIN_FTB_STRUCT              tRCminFine;               ///< 38  Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)
  SPD_TRP_AB_MTB_STRUCT               tRPab;                    ///< 39  Minimum Row Precharge Delay Time for all banks (tRPab)
  SPD_TRP_AB_FTB_STRUCT               tRPabFine;                ///< 40  Fine Offset for Minimum Row Precharge Delay Time for all banks (tRPab)
  SPD_PTRR_SUPPORT_STRUCT             pTRRsupport;              ///< 41 - pTRR support with TMAC value
  UINT8                               Reserved3[59 - 42 + 1];   ///< 42 - 59 Reserved
} SPD_GENERAL_SECTION;

typedef struct {
  SPD_UNBUF_MODULE_NOMINAL_HEIGHT     ModuleNominalHeight;      ///< 60 Module Nominal Height
  SPD_UNBUF_MODULE_NOMINAL_THICKNESS  ModuleMaximumThickness;   ///< 61 Module Maximum Thickness
  SPD_UNBUF_REFERENCE_RAW_CARD        ReferenceRawCardUsed;     ///< 62 Reference Raw Card Used
  SPD_UNBUF_ADDRESS_MAPPING           AddressMappingEdgeConn;   ///< 63 Address Mapping from Edge Connector to DRAM
  UINT8                               Reserved[116 - 64 + 1];   ///< 64-116 Reserved
} SPD_MODULE_UNBUFFERED;

typedef struct {
  SPD_RDIMM_MODULE_NOMINAL_HEIGHT     ModuleNominalHeight;      ///< 60 Module Nominal Height
  SPD_RDIMM_MODULE_NOMINAL_THICKNESS  ModuleMaximumThickness;   ///< 61 Module Maximum Thickness
  SPD_RDIMM_REFERENCE_RAW_CARD        ReferenceRawCardUsed;     ///< 62 Reference Raw Card Used
  SPD_RDIMM_MODULE_ATTRIBUTES         DimmModuleAttributes;     ///< 63 DIMM Module Attributes
  UINT8                               Reserved[116 - 64 + 1];   ///< 64-116 Reserved
} SPD_MODULE_REGISTERED;

typedef union {
  SPD_MODULE_UNBUFFERED               Unbuffered;
  SPD_MODULE_REGISTERED               Registered;
} SPD_MODULE_SPECIFIC;

typedef struct {
  UINT8                          ModulePartNumber[145 - 128 + 1];        ///< 128-145 Module Part Number
} SPD_MODULE_PART_NUMBER;

typedef struct {
  UINT8                          ModuleRevisionCode[147 - 146 + 1];      ///< 146-147 Module Revision Code
} SPD_MODULE_REVISION_CODE;

typedef struct {
  UINT8                          ManufactureSpecificData[175 - 150 + 1]; ///< 150-175 Manufacturer's Specific Data
} SPD_MANUFACTURE_SPECIFIC;

///
/// DDR3 Serial Presence Detect structure
///
typedef struct {
  SPD_GENERAL_SECTION         General;                                ///< 0-59 General Section
  SPD_MODULE_SPECIFIC         Module;                                 ///< 60-116 Module-Specific Section
  SPD_UNIQUE_MODULE_ID        ModuleId;                               ///< 117-125 Unique Module ID
  SPD_CYCLIC_REDUNDANCY_CODE  Crc;                                    ///< 126-127 Cyclical Redundancy Code (CRC)
  SPD_MODULE_PART_NUMBER      ModulePartNumber;                       ///< 128-145 Module Part Number
  SPD_MODULE_REVISION_CODE    ModuleRevisionCode;                     ///< 146-147 Module Revision Code
  SPD_MANUFACTURER_ID_CODE    DramIdCode;                             ///< 148-149 Dram Manufacturer ID Code
  SPD_MANUFACTURE_SPECIFIC    ManufactureSpecificData;                ///< 150-175 Manufacturer's Specific Data
  SPD_EXTREME_MEMORY_PROFILE  Xmp;                                    ///< 176-254 Intel(r) Extreme Memory Profile support
  UINT8                       Reserved;                               ///< 255 Reserved
} MrcSpdDdr3;

typedef union {
  struct {
    UINT8  Density                             :  4; ///< Bits 3:0
    UINT8  BankAddress                         :  2; ///< Bits 5:4
    UINT8  BankGroup                           :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD4_SDRAM_DENSITY_BANKS_STRUCT;

typedef union {
  struct {
    UINT8  SignalLoading                       :  2; ///< Bits 1:0
    UINT8                                      :  2; ///< Bits 3:2
    UINT8  DieCount                            :  3; ///< Bits 6:4
    UINT8  SdramDeviceType                     :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD4_SDRAM_DEVICE_TYPE_STRUCT;

typedef union {
  struct {
    UINT8  OperationAt1_20                     :  1; ///< Bits 0:0
    UINT8  EndurantAt1_20                      :  1; ///< Bits 1:1
    UINT8                                      :  6; ///< Bits 7:2
  } Bits;
  UINT8  Data;
} SPD4_MODULE_NOMINAL_VOLTAGE_STRUCT;

typedef union {
  struct {
    UINT8  tCKmax                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD4_TCK_MAX_MTB_STRUCT;

typedef union {
  struct {
    INT8  tCKmaxFine                          :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD4_TCK_MAX_FTB_STRUCT;

typedef union {
  struct {
    UINT8                                      :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD4_SDRAM_THERMAL_REFRESH_STRUCT;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8  RawCardExtension                    :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD4_UNBUF_MODULE_NOMINAL_HEIGHT;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8  RawCardExtension                    :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD4_RDIMM_MODULE_NOMINAL_HEIGHT;

typedef struct {
  SPD_DEVICE_DESCRIPTION_STRUCT       Description;              ///< 0       Number of Serial PD Bytes Written / SPD Device Size / CRC Coverage 1, 2
  SPD_REVISION_STRUCT                 Revision;                 ///< 1       SPD Revision
  SPD_DRAM_DEVICE_TYPE_STRUCT         DramDeviceType;           ///< 2       DRAM Device Type
  SPD_MODULE_TYPE_STRUCT              ModuleType;               ///< 3       Module Type
  SPD4_SDRAM_DENSITY_BANKS_STRUCT     SdramDensityAndBanks;     ///< 4       SDRAM Density and Banks
  SPD_SDRAM_ADDRESSING_STRUCT         SdramAddressing;          ///< 5       SDRAM Addressing
  SPD4_SDRAM_DEVICE_TYPE_STRUCT       SdramDeviceType;          ///< 6       SDRAM Device Type
  SPD_PTRR_SUPPORT_STRUCT             pTRRsupport;              ///< 7       pTRR support with TMAC value
  SPD4_SDRAM_THERMAL_REFRESH_STRUCT   ThermalAndRefreshOptions; ///< 8       SDRAM Thermal and Refresh Options
  UINT8                               Reserved0[10 - 9 + 1];    ///< 9-10    Reserved
  SPD4_MODULE_NOMINAL_VOLTAGE_STRUCT  ModuleNominalVoltage;     ///< 11      Module Nominal Voltage, VDD
  SPD_MODULE_ORGANIZATION_STRUCT      ModuleOrganization;       ///< 12      Module Organization
  SPD_MODULE_MEMORY_BUS_WIDTH_STRUCT  ModuleMemoryBusWidth;     ///< 13      Module Memory Bus Width
  SPD_MODULE_THERMAL_SENSOR_STRUCT    ModuleThermalSensor;      ///< 14      Module Thermal Sensor
  UINT8                               Reserved1[16 - 15 + 1];   ///< 15-16   Reserved
  SPD4_TIMEBASE_STRUCT                Timebase;                 ///< 17      Timebases
  SPD_TCK_MIN_MTB_STRUCT              tCKmin;                   ///< 18      SDRAM Minimum Cycle Time (tCKmin)
  SPD4_TCK_MAX_MTB_STRUCT             tCKmax;                   ///< 19      SDRAM Maximum Cycle Time (tCKmax)
  SPD4_CAS_LATENCIES_SUPPORTED_STRUCT CasLatencies;             ///< 20-23   CAS Latencies Supported
  SPD_TAA_MIN_MTB_STRUCT              tAAmin;                   ///< 24      Minimum CAS Latency Time (tAAmin)
  SPD_TRCD_MIN_MTB_STRUCT             tRCDmin;                  ///< 25      Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRP_MIN_MTB_STRUCT              tRPmin;                   ///< 26      Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRAS_TRC_MIN_MTB_STRUCT         tRASMintRCMinUpper;       ///< 27      Upper Nibbles for tRAS and tRC
  SPD_TRAS_MIN_MTB_STRUCT             tRASmin;                  ///< 28      Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte
  SPD_TRC_MIN_MTB_STRUCT              tRCmin;                   ///< 29      Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte
  SPD_TRFC_MIN_MTB_STRUCT             tRFC1min;                 ///< 30-31   Minimum Refresh Recovery Delay Time (tRFC1min)
  SPD_TRFC_MIN_MTB_STRUCT             tRFC2min;                 ///< 32-33   Minimum Refresh Recovery Delay Time (tRFC2min)
  SPD_TRFC_MIN_MTB_STRUCT             tRFC4min;                 ///< 34-35   Minimum Refresh Recovery Delay Time (tRFC4min)
  SPD_TFAW_MIN_MTB_UPPER_STRUCT       tFAWMinUpper;             ///< 36      Upper Nibble for tFAW
  SPD_TFAW_MIN_MTB_STRUCT             tFAWmin;                  ///< 37      Minimum Four Activate Window Delay Time (tFAWmin)
  SPD_TRRD_MIN_MTB_STRUCT             tRRD_Smin;                ///< 38      Minimum Activate to Activate Delay Time (tRRD_Smin), different bank group
  SPD_TRRD_MIN_MTB_STRUCT             tRRD_Lmin;                ///< 39      Minimum Activate to Activate Delay Time (tRRD_Lmin), same bank group
  UINT8                               Reserved2[117 - 40 + 1];  ///< 40-117  Reserved
  SPD_TRRD_MIN_FTB_STRUCT             tRRD_LminFine;            ///< 118     Fine Offset for Minimum Activate to Activate Delay Time (tRRD_Lmin), different bank group
  SPD_TRRD_MIN_FTB_STRUCT             tRRD_SminFine;            ///< 119     Fine Offset for Minimum Activate to Activate Delay Time (tRRD_Smin), same bank group
  SPD_TRC_MIN_FTB_STRUCT              tRCminFine;               ///< 120     Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)
  SPD_TRP_MIN_FTB_STRUCT              tRPminFine;               ///< 121     Minimum Row Precharge Delay Time (tRPmin)
  SPD_TRCD_MIN_FTB_STRUCT             tRCDminFine;              ///< 122     Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TAA_MIN_FTB_STRUCT              tAAminFine;               ///< 123     Fine Offset for Minimum CAS Latency Time (tAAmin)
  SPD4_TCK_MAX_FTB_STRUCT             tCKmaxFine;               ///< 124     Fine Offset for SDRAM Minimum Cycle Time (tCKmax)
  SPD_TCK_MIN_FTB_STRUCT              tCKminFine;               ///< 125     Fine Offset for SDRAM Maximum Cycle Time (tCKmin)
  SPD_CYCLIC_REDUNDANCY_CODE          Crc;                      ///< 126-127 Cyclical Redundancy Code (CRC)
} SPD4_BASE_SECTION;

typedef struct {
  SPD4_UNBUF_MODULE_NOMINAL_HEIGHT    ModuleNominalHeight;      ///< 128     Module Nominal Height
  SPD_UNBUF_MODULE_NOMINAL_THICKNESS  ModuleMaximumThickness;   ///< 129     Module Maximum Thickness
  SPD_UNBUF_REFERENCE_RAW_CARD        ReferenceRawCardUsed;     ///< 130     Reference Raw Card Used
  SPD_UNBUF_ADDRESS_MAPPING           AddressMappingEdgeConn;   ///< 131     Address Mapping from Edge Connector to DRAM
  UINT8                               Reserved[253 - 132 + 1];  ///< 132-253 Reserved
  SPD_CYCLIC_REDUNDANCY_CODE          Crc;                      ///< 254-255 Cyclical Redundancy Code (CRC)
} SPD4_MODULE_UNBUFFERED;

typedef struct {
  SPD4_RDIMM_MODULE_NOMINAL_HEIGHT    ModuleNominalHeight;      ///< 128     Module Nominal Height
  SPD_RDIMM_MODULE_NOMINAL_THICKNESS  ModuleMaximumThickness;   ///< 129     Module Maximum Thickness
  SPD_RDIMM_REFERENCE_RAW_CARD        ReferenceRawCardUsed;     ///< 130     Reference Raw Card Used
  SPD_RDIMM_MODULE_ATTRIBUTES         DimmModuleAttributes;     ///< 131     DIMM Module Attributes
  UINT8                               Reserved[253 - 132 + 1];  ///< 253-132 Reserved
  SPD_CYCLIC_REDUNDANCY_CODE          Crc;                      ///< 254-255 Cyclical Redundancy Code (CRC)
} SPD4_MODULE_REGISTERED;

typedef union {
  SPD4_MODULE_UNBUFFERED              Unbuffered;               ///< 128-255 Unbuffered Memory Module Types
  SPD4_MODULE_REGISTERED              Registered;               ///< 128-255 Registered Memory Module Types
} SPD4_MODULE_SPECIFIC;

typedef struct {
  UINT8                               ModulePartNumber[348 - 329 + 1]; ///< 329-348 Module Part Number
} SPD4_MODULE_PART_NUMBER;

typedef struct {
  UINT8                               ManufactureSpecificData[381 - 353 + 1]; ///< 353-381 Manufacturer's Specific Data
} SPD4_MANUFACTURE_SPECIFIC;

typedef UINT8                         SPD4_MODULE_REVISION_CODE;///< 349     Module Revision Code
typedef UINT8                         SPD4_DRAM_STEPPING;       ///< 352     Dram Stepping

typedef struct {
  SPD_UNIQUE_MODULE_ID                ModuleId;                 ///< 320-328 Unique Module ID
  SPD4_MODULE_PART_NUMBER             ModulePartNumber;         ///< 329-348 Module Part Number
  SPD4_MODULE_REVISION_CODE           ModuleRevisionCode;       ///< 349     Module Revision Code
  SPD_MANUFACTURER_ID_CODE            DramIdCode;               ///< 350-351 Dram Manufacturer ID Code
  SPD4_DRAM_STEPPING                  DramStepping;             ///< 352     Dram Stepping
  SPD4_MANUFACTURE_SPECIFIC           ManufactureSpecificData;  ///< 353-381 Manufacturer's Specific Data
  SPD_CYCLIC_REDUNDANCY_CODE          Crc;                      ///< 382-383 Cyclical Redundancy Code (CRC)
} SPD4_MANUFACTURING_DATA;

typedef union {
  SPD_EXTREME_MEMORY_PROFILE_2_0      Xmp;                      ///< 384-463 Intel(r) Extreme Memory Profile support
  UINT8                               Reserved0[511 - 384 + 1]; ///< 384-511 Unbuffered Memory Module Types
} SPD4_END_USER_SECTION;

///
/// DDR4 Serial Presence Detect structure
///
typedef struct {
  SPD4_BASE_SECTION                   Base;                     ///< 0-127   Base Configuration and DRAM Parameters
  SPD4_MODULE_SPECIFIC                Module;                   ///< 128-255 Module-Specific Section
  UINT8                               Reserved0[319 - 256 + 1]; ///< 256-319 Reserved
  SPD4_MANUFACTURING_DATA             ManufactureInfo;          ///< 320-383 Manufacturing Information
  SPD4_END_USER_SECTION               EndUser;                  ///< 384-511 End User Programmable
} MrcSpdDdr4;

typedef union {
  struct {
    UINT8  Fine                                :  2; ///< Bits 1:0
    UINT8  Medium                              :  2; ///< Bits 3:2
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_LPDDR_TIMEBASE_STRUCT;

typedef union {
  struct {
    UINT32 CL3                                 :  1;  ///< Bits 0:0
    UINT32 CL6                                 :  1;  ///< Bits 1:1
    UINT32 CL8                                 :  1;  ///< Bits 2:2
    UINT32 CL9                                 :  1;  ///< Bits 3:3
    UINT32 CL10                                :  1;  ///< Bits 4:4
    UINT32 CL11                                :  1;  ///< Bits 5:5
    UINT32 CL12                                :  1;  ///< Bits 6:6
    UINT32 CL14                                :  1;  ///< Bits 7:7
    UINT32 CL16                                :  1;  ///< Bits 8:8
    UINT32                                     :  1;  ///< Bits 9:9
    UINT32 CL20                                :  1;  ///< Bits 10:10
    UINT32 CL22                                :  1;  ///< Bits 11:11
    UINT32 CL24                                :  1;  ///< Bits 12:12
    UINT32                                     :  1;  ///< Bits 13:13
    UINT32 CL28                                :  1;  ///< Bits 14:14
    UINT32                                     :  1;  ///< Bits 15:15
    UINT32 CL32                                :  1;  ///< Bits 16:16
    UINT32                                     :  1;  ///< Bits 17:17
    UINT32 CL36                                :  1;  ///< Bits 18:18
    UINT32                                     :  1;  ///< Bits 19:19
    UINT32 CL40                                :  1;  ///< Bits 20:20
    UINT32                                     :  11; ///< Bits 31:21
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SPD_LPDDR_CAS_LATENCIES_SUPPORTED_STRUCT;

typedef union {
  struct {
    UINT8  Density                             :  4; ///< Bits 3:0
    UINT8  BankAddress                         :  2; ///< Bits 5:4
    UINT8  BankGroup                           :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_LPDDR_SDRAM_DENSITY_BANKS_STRUCT;

typedef union {
  struct {
    UINT8  SignalLoading                       :  2; ///< Bits 1:0
    UINT8  ChannelsPerDie                      :  2; ///< Bits 3:2
    UINT8  DieCount                            :  3; ///< Bits 6:4
    UINT8  SdramPackageType                    :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_LPDDR_SDRAM_PACKAGE_TYPE_STRUCT;

typedef union {
  struct {
    UINT8  OperationAt1_20                     :  1; ///< Bits 0:0
    UINT8  EndurantAt1_20                      :  1; ///< Bits 1:1
    UINT8  OperationAt1_10                     :  1; ///< Bits 2:2
    UINT8  EndurantAt1_10                      :  1; ///< Bits 3:3
    UINT8  OperationAtTBD2V                    :  1; ///< Bits 4:4
    UINT8  EndurantAtTBD2V                     :  1; ///< Bits 5:5
    UINT8                                      :  2; ///< Bits 7:6
  } Bits;
  UINT8  Data;
} SPD_LPDDR_MODULE_NOMINAL_VOLTAGE_STRUCT;

typedef union {
  struct {
    UINT8  tCKmax                              :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_LPDDR_TCK_MAX_MTB_STRUCT;

typedef union {
  struct {
    UINT8  ReadLatencyMode                     :  2; ///< Bits 1:0
    UINT8  WriteLatencySet                     :  2; ///< Bits 3:2
    UINT8                                      :  4; ///< Bits 7:4
  } Bits;
  UINT8  Data;
} SPD_LPDDR_RW_LATENCY_OPTION_STRUCT;

typedef union {
  struct {
    INT8  tCKmaxFine                           :  8; ///< Bits 7:0
  } Bits;
  INT8  Data;
} SPD_LPDDR_TCK_MAX_FTB_STRUCT;

typedef union {
  struct {
    UINT8                                      :  8; ///< Bits 7:0
  } Bits;
  UINT8  Data;
} SPD_LPDDR_SDRAM_THERMAL_REFRESH_STRUCT;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8  RawCardExtension                    :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD_LPDDR_UNBUF_MODULE_NOMINAL_HEIGHT;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8  RawCardExtension                    :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD_LPDDR_RDIMM_MODULE_NOMINAL_HEIGHT;

typedef struct {
  SPD_DEVICE_DESCRIPTION_STRUCT             Description;              ///< 0       Number of Serial PD Bytes Written / SPD Device Size / CRC Coverage 1, 2
  SPD_REVISION_STRUCT                       Revision;                 ///< 1       SPD Revision
  SPD_DRAM_DEVICE_TYPE_STRUCT               DramDeviceType;           ///< 2       DRAM Device Type
  SPD_MODULE_TYPE_STRUCT                    ModuleType;               ///< 3       Module Type
  SPD_LPDDR_SDRAM_DENSITY_BANKS_STRUCT      SdramDensityAndBanks;     ///< 4       SDRAM Density and Banks
  SPD_SDRAM_ADDRESSING_STRUCT               SdramAddressing;          ///< 5       SDRAM Addressing
  SPD_LPDDR_SDRAM_PACKAGE_TYPE_STRUCT       SdramPackageType;         ///< 6       SDRAM Package Type
  SPD_PTRR_SUPPORT_STRUCT                   pTRRsupport;              ///< 7       pTRR support with TMAC value - SDRAM Optional Features
  SPD_LPDDR_SDRAM_THERMAL_REFRESH_STRUCT    ThermalAndRefreshOptions; ///< 8       SDRAM Thermal and Refresh Options
  UINT8                                     Reserved0[10 - 9 + 1];    ///< 9-10    Reserved
  SPD_LPDDR_MODULE_NOMINAL_VOLTAGE_STRUCT   ModuleNominalVoltage;     ///< 11      Module Nominal Voltage, VDD
  SPD_MODULE_ORGANIZATION_STRUCT            ModuleOrganization;       ///< 12      Module Organization
  SPD_MODULE_MEMORY_BUS_WIDTH_STRUCT        ModuleMemoryBusWidth;     ///< 13      Module Memory Bus Width
  SPD_MODULE_THERMAL_SENSOR_STRUCT          ModuleThermalSensor;      ///< 14      Module Thermal Sensor
  UINT8                                     Reserved1[16 - 15 + 1];   ///< 15-16   Reserved
  SPD_LPDDR_TIMEBASE_STRUCT                 Timebase;                 ///< 17      Timebases
  SPD_TCK_MIN_MTB_STRUCT                    tCKmin;                   ///< 18      SDRAM Minimum Cycle Time (tCKmin)
  SPD_LPDDR_TCK_MAX_MTB_STRUCT              tCKmax;                   ///< 19      SDRAM Maximum Cycle Time (tCKmax)
  SPD_LPDDR_CAS_LATENCIES_SUPPORTED_STRUCT  CasLatencies;             ///< 20-23   CAS Latencies Supported
  SPD_TAA_MIN_MTB_STRUCT                    tAAmin;                   ///< 24      Minimum CAS Latency Time (tAAmin)
  SPD_LPDDR_RW_LATENCY_OPTION_STRUCT        LatencySetOptions;        ///< 25      Read and Write Latency Set Options
  SPD_TRCD_MIN_MTB_STRUCT                   tRCDmin;                  ///< 26      Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TRP_AB_MTB_STRUCT                     tRPab;                    ///< 27      Minimum Row Precharge Delay Time (tRPab), all banks
  SPD_TRP_PB_MTB_STRUCT                     tRPpb;                    ///< 28      Minimum Row Precharge Delay Time (tRPpb), per bank
  SPD_TRFC_AB_MTB_STRUCT                    tRFCab;                   ///< 29-30   Minimum Refresh Recovery Delay Time (tRFCab), all banks
  SPD_TRFC_PB_MTB_STRUCT                    tRFCpb;                   ///< 31-32   Minimum Refresh Recovery Delay Time (tRFCpb), per bank
  UINT8                                     Reserved2[119 - 33 + 1];  ///< 33-119  Reserved
  SPD_TRP_PB_FTB_STRUCT                     tRPpbFine;                ///< 120     Fine Offset for Minimum Row Precharge Delay Time (tRPpbFine), per bank
  SPD_TRP_AB_FTB_STRUCT                     tRPabFine;                ///< 121     Fine Offset for Minimum Row Precharge Delay Time (tRPabFine), all ranks
  SPD_TRCD_MIN_FTB_STRUCT                   tRCDminFine;              ///< 122     Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
  SPD_TAA_MIN_FTB_STRUCT                    tAAminFine;               ///< 123     Fine Offset for Minimum CAS Latency Time (tAAmin)
  SPD_LPDDR_TCK_MAX_FTB_STRUCT              tCKmaxFine;               ///< 124     Fine Offset for SDRAM Maximum Cycle Time (tCKmax)
  SPD_TCK_MIN_FTB_STRUCT                    tCKminFine;               ///< 125     Fine Offset for SDRAM Minimum Cycle Time (tCKmin)
  SPD_CYCLIC_REDUNDANCY_CODE                Crc;                      ///< 126-127 Cyclical Redundancy Code (CRC)
} SPD_LPDDR_BASE_SECTION;

typedef union {
  struct {
    UINT8  FrontThickness                      :  4; ///< Bits 3:0
    UINT8  BackThickness                       :  4; ///< Bits 7:4
  } Bits;
  UINT8 Data;
} SPD_LPDDR_MODULE_MAXIMUM_THICKNESS;

typedef union {
  struct {
    UINT8  Height                              :  5; ///< Bits 4:0
    UINT8  RawCardExtension                    :  3; ///< Bits 7:5
  } Bits;
  UINT8  Data;
} SPD_LPDDR_MODULE_NOMINAL_HEIGHT;

typedef union {
  struct {
    UINT8  Card                                :  5; ///< Bits 4:0
    UINT8  Revision                            :  2; ///< Bits 6:5
    UINT8  Extension                           :  1; ///< Bits 7:7
  } Bits;
  UINT8  Data;
} SPD_LPDDR_REFERENCE_RAW_CARD;

typedef struct {
  SPD_LPDDR_MODULE_NOMINAL_HEIGHT         ModuleNominalHeight;      ///< 128     Module Nominal Height
  SPD_LPDDR_MODULE_MAXIMUM_THICKNESS      ModuleMaximumThickness;   ///< 129     Module Maximum Thickness
  SPD_LPDDR_REFERENCE_RAW_CARD            ReferenceRawCardUsed;     ///< 130     Reference Raw Card Used
  UINT8                                   Reserved[253 - 131 + 1];  ///< 131-253 Reserved
  SPD_CYCLIC_REDUNDANCY_CODE              Crc;                      ///< 254-255 Cyclical Redundancy Code (CRC)
} SPD_LPDDR_MODULE_LPDIMM;

typedef union {
  SPD_LPDDR_MODULE_LPDIMM                 LpDimm;                   ///< 128-255 Unbuffered Memory Module Types
} SPD_LPDDR_MODULE_SPECIFIC;

typedef struct {
  UINT8                                   ModulePartNumber[348 - 329 + 1]; ///< 329-348 Module Part Number
} SPD_LPDDR_MODULE_PART_NUMBER;

typedef struct {
  UINT8                                   ManufactureSpecificData[381 - 353 + 1]; ///< 353-381 Manufacturer's Specific Data
} SPD_LPDDR_MANUFACTURE_SPECIFIC;

typedef UINT8                             SPD_LPDDR_MODULE_REVISION_CODE;///< 349     Module Revision Code
typedef UINT8                             SPD_LPDDR_DRAM_STEPPING;       ///< 352     Dram Stepping

typedef struct {
  SPD_UNIQUE_MODULE_ID                    ModuleId;                 ///< 320-328 Unique Module ID
  SPD_LPDDR_MODULE_PART_NUMBER            ModulePartNumber;         ///< 329-348 Module Part Number
  SPD_LPDDR_MODULE_REVISION_CODE          ModuleRevisionCode;       ///< 349     Module Revision Code
  SPD_MANUFACTURER_ID_CODE                DramIdCode;               ///< 350-351 Dram Manufacturer ID Code
  SPD_LPDDR_DRAM_STEPPING                 DramStepping;             ///< 352     Dram Stepping
  SPD_LPDDR_MANUFACTURE_SPECIFIC          ManufactureSpecificData;  ///< 353-381 Manufacturer's Specific Data
  UINT8                                   Reserved[383 - 382 + 1];  ///< 382-383 Reserved
} SPD_LPDDR_MANUFACTURING_DATA;

typedef union {
  UINT8                                   Reserved0[511 - 384 + 1]; ///< 384-511 End User Programmable
} SPD_LPDDR_END_USER_SECTION;

typedef struct {
  SPD_LPDDR_BASE_SECTION                  Base;                     ///< 0-127   Base Configuration and DRAM Parameters
  SPD_LPDDR_MODULE_SPECIFIC               Module;                   ///< 128-255 Module-Specific Section
  UINT8                                   Reserved0[319 - 256 + 1]; ///< 256-319 Reserved
  SPD_LPDDR_MANUFACTURING_DATA            ManufactureInfo;          ///< 320-383 Manufacturing Information
  SPD_LPDDR_END_USER_SECTION              EndUser;                  ///< 384-511 End User Programmable
} MrcSpdLpDdr;

typedef union {
  MrcSpdDdr3  Ddr3;
  MrcSpdDdr4  Ddr4;
  MrcSpdLpDdr Lpddr;
} MrcSpd;

#ifndef MAX_SPD_SAVE
#define MAX_SPD_SAVE (sizeof (SPD_MANUFACTURER_ID_CODE) + \
                      sizeof (SPD_MANUFACTURING_LOCATION) + \
                      sizeof (SPD_MANUFACTURING_DATE) + \
                      sizeof (SPD_MANUFACTURER_SERIAL_NUMBER) + \
                      sizeof (SPD4_MODULE_PART_NUMBER))
#endif

#pragma pack (pop)
#endif // _MrcSpdData_h_
