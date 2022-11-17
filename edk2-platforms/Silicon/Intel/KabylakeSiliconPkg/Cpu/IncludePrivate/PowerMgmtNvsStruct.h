/** @file
  This file contains CPU Gnvs Struct specific to processor

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _POWER_MGMT_NVS_STRUCT_H_
#define _POWER_MGMT_NVS_STRUCT_H_

//
// Processor Power Management GlobalNvs Revisions
//
#define CPU_GLOBAL_NVS_AREA_REVISION 2

//
// Structure Declarations
//
#pragma pack(1)
///
/// Config TDP level settings.
///
typedef struct {
  UINT16 CtdpPowerLimit1;      ///< CTDP Power Limit1
  UINT16 CtdpPowerLimit2;      ///< CTDP Power Limit2
  UINT8  CtdpPowerLimitWindow; ///< CTDP Power Limit Time Window
  UINT8  CtdpCtc;              ///< CTDP CTC
  UINT8  CtdpTar;              ///< CTDP TAR
  UINT8  CtdpPpc;              ///< CTDP PPC
} CTDP_LEVEL_SETTINGS;

///
/// CPU Global NVS Area definition
///
typedef struct {
  /**
  This member specifies the revision of the CPU_GLOBAL_NVS_AREA. This field is used to indicate backward
  compatible changes to the NVS AREA. Any such changes to this PPI will result in an update in the revision number.

  <b>Revision 1</b>:
   - Initial version.
  **/
  UINT8  Revision;                 ///< (0) CPU GlobalNvs Revision
  //
  // PPM Flag Values
  //
  UINT32 PpmFlags;                 ///< (1-4) PPM Flags
  UINT8  Reserved;                 ///< (5) Reserved
  //
  // Thermal Configuration Values
  //
  UINT8  AutoCriticalTripPoint;    ///< (6) Auto Critical Trip Point
  UINT8  AutoPassiveTripPoint;     ///< (7) Auto Passive Trip Point
  UINT8  AutoActiveTripPoint;      ///< (8) Auto Active Trip Point
  UINT32 Cpuid;                    ///< (9) CPUID
  //
  // ConfigTDP Values
  //
  UINT8 ConfigurablePpc;           ///< (13) Boot Mode vlues for _PPC
  //
  // ConfigTDP Level settngs
  //
  UINT8 CtdpLevelsSupported;       ///< (14) ConfigTdp Number Of Levels
  UINT8 ConfigTdpBootModeIndex;    ///< (15) CTDP Boot Mode Index
  ///
  /// (16) CTDP Level 0 Power Limit1
  /// (18) CTDP Level 0 Power Limit2
  /// (20) CTDP Level 0 Power Limit1 Time Window
  /// (21) CTDP Level 0 CTC
  /// (22) CTDP Level 0 TAR
  /// (23) CTDP Level 0 PPC
  /// (24) CTDP Level 1 Power Limit1
  /// (26) CTDP Level 1 Power Limit2
  /// (28) CTDP Level 1 Power Limit1 Time Window
  /// (29) CTDP Level 1 CTC
  /// (30) CTDP Level 1 TAR
  /// (31) CTDP Level 1 PPC
  /// (32) CTDP Level 2 Power Limit1
  /// (34) CTDP Level 2 Power Limit2
  /// (36) CTDP Level 2 Power Limit1 Time Window
  /// (37) CTDP Level 2 CTC
  /// (38) CTDP Level 2 TAR
  /// (39) CTDP Level 2 PPC
  ///
  CTDP_LEVEL_SETTINGS CtdpLevelSettings[3];
  //
  // Mwait Hints and Latency values for C3/C6/C7/C7S
  //
  UINT8  C3MwaitValue;             ///< (40) Mwait Hint value for C3
  UINT8  C6MwaitValue;             ///< (41) Mwait Hint value for C6
  UINT8  C7MwaitValue;             ///< (42) Mwait Hint value for C6
  UINT8  CDMwaitValue;             ///< (43) Mwait Hint value for C7/C8/C9/C10
  UINT16 C3Latency;                ///< (44-45) Latency value for C3
  UINT16 C6Latency;                ///< (46-47) Latency Value for C6
  UINT16 C7Latency;                ///< (48-49) Latency Value for C6
  UINT16 CDLatency;                ///< (50-51) Latency Value for C7/C8/C9/C10
  UINT16 CDIOLevel;                ///< (52-53) IO Level Value for C7/C8/C9/C10
  UINT16 CDPowerValue;             ///< (54-55) Power Value for C7/C8/C9/C10
  UINT8  MiscPowerManagementFlags; ///< (55) MiscPowerManagementFlags
  //
  // DTS
  //
  UINT8  EnableDigitalThermalSensor;           ///< (57) DTS Function enable
  UINT8  BspDigitalThermalSensorTemperature;   ///< (58) Temperature of BSP
  UINT8  ApDigitalThermalSensorTemperature;    ///< (59) Temperature of AP
  UINT8  DigitalThermalSensorSmiFunction;      ///< (60) SMI function call via DTS IO Trap
  UINT8  PackageDTSTemperature;                ///< (61) Package temperature
  UINT8  IsPackageTempMSRAvailable;            ///< (62) Package Temperature MSR available
  UINT8  Ap2DigitalThermalSensorTemperature;   ///< (63) Temperature of the second AP
  UINT8  Ap3DigitalThermalSensorTemperature;   ///< (64) Temperature of the third AP
  //
  // BIOS Guard
  //
  UINT64 BiosGuardMemAddress;                  ///< (65-72) BIOS Guard Memory Address for Tool Interface
  UINT8  BiosGuardMemSize;                     ///< (73) BIOS Guard Memory Size for Tool Interface
  UINT16 BiosGuardIoTrapAddress;               ///< (74-75) IoTrap Address for Tool Interface
  UINT16 BiosGuardIoTrapLength;                ///< (76-77) IoTrap Length for Tool Interface
  //
  // DTS I/O Trap
  //
  UINT16 DtsIoTrapAddress;                     ///< (78-79) DTS IO trap Address
  UINT8  DtsIoTrapLength;                      ///< (80)    DTS IO trap Length
  UINT8  DtsAcpiEnable;                        ///< (81) DTS is in ACPI Mode Enabled

  //
  // Software Guard Extension
  //
  UINT8  SgxStatus;                            ///< (82)    SE Status
  UINT64 EpcBaseAddress;                       ///< (83-90) EPC Base Address
  UINT64 EpcLength;                            ///< (91-98) EPC Length

  //
  //  HWP
  //
  UINT8  HwpVersion;                           ///< (99) HWP Status
  UINT16 HwpIoTrapAddress;                     ///< (100-101) IoTrap Address for HWP
  UINT16 HwpIoTrapLength;                      ///< (102-103) IoTrap Length for HWP

  UINT8  PowerState;                           ///< (104) Power State
  UINT8  EnableHdcPolicy;                      ///< (105) Hardware Duty Cycling Policy

  UINT8  HwpInterruptStatus;                   ///< (106) HWP Interrupt Status
  UINT8  DtsInterruptStatus;                   ///< (107) DTS Interrupt Status
} CPU_GLOBAL_NVS;
#pragma pack()

typedef struct _CPU_GLOBAL_NVS_AREA_CONFIG {
  CPU_GLOBAL_NVS *Area;
} CPU_GLOBAL_NVS_AREA_CONFIG;

typedef struct _FVID_HEADER {
  UINT32 Stepping;    ///< Matches value returned by CPUID function 1
  UINT16 MaxBusRatio; ///< Matches BUS_RATIO_MAX field in PERF_STS_MSR
  UINT16 EistStates;  ///< Number of states of FVID (N)
} FVID_HEADER;

typedef struct _FVID_STATE {
  UINT32 State;           ///< State Number (0 - N-1)
  UINT16 BusRatio;        ///< BUS_RATIO_SEL value to be written to PERF_CTL
  UINT32 Power;           ///< Typical power consumed by CPU in this state
  UINT32 Limit16State;    ///< State Number (0 - N-1) with limit 16 P-states
  UINT16 Limit16BusRatio; ///< BUS_RATIO_SEL value to be written to PERF_CTL with limit 16 P-states
  UINT32 Limit16Power;    ///< Typical power consumed by CPU in this state with limit 16 P-states
} FVID_STATE;

typedef union _FVID_TABLE {
  FVID_HEADER FvidHeader;
  FVID_STATE  FvidState;
  UINT64      FvidData;
} FVID_TABLE;

#endif
