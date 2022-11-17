/** @file
  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

  //
  // Define CPU NVS Area operation region.
  //

#ifndef _CPU_NVS_AREA_DEF_H_
#define _CPU_NVS_AREA_DEF_H_

#pragma pack (push,1)
typedef struct {
  UINT8    Revision;                                ///< Offset 0       CPU GlobalNvs Revision
  UINT32   PpmFlags;                                ///< Offset 1       PPM Flags Values
  UINT8    Reserved0[1];                            ///< Offset 5:5
  UINT8    AutoCriticalTripPoint;                   ///< Offset 6       Auto Critical Trip Point
  UINT8    AutoPassiveTripPoint;                    ///< Offset 7       Auto Passive Trip Point
  UINT8    AutoActiveTripPoint;                     ///< Offset 8       Auto Active Trip Point
  UINT32   Cpuid;                                   ///< Offset 9       CPUID
  UINT8    ConfigurablePpc;                         ///< Offset 13      Boot Mode vlues for _PPC
  UINT8    CtdpLevelsSupported;                     ///< Offset 14      ConfigTdp Number Of Levels
  UINT8    ConfigTdpBootModeIndex;                  ///< Offset 15      CTDP Boot Mode Index
  UINT16   CtdpPowerLimit1[3];                      ///< Offset 16      CTDP Level 0 Power Limit1
                                                    ///< Offset 18      CTDP Level 1 Power Limit1
                                                    ///< Offset 20      CTDP Level 2 Power Limit1
  UINT16   CtdpPowerLimit2[3];                      ///< Offset 22      CTDP Level 0 Power Limit2
                                                    ///< Offset 24      CTDP Level 1 Power Limit2
                                                    ///< Offset 26      CTDP Level 2 Power Limit2
  UINT8    CtdpPowerLimitWindow[3];                 ///< Offset 28      CTDP Level 0 Power Limit1 Time Window
                                                    ///< Offset 29      CTDP Level 1 Power Limit1 Time Window
                                                    ///< Offset 30      CTDP Level 2 Power Limit1 Time Window
  UINT8    CtdpCtc[3];                              ///< Offset 31      CTDP Level 0 CTC
                                                    ///< Offset 32      CTDP Level 1 CTC
                                                    ///< Offset 33      CTDP Level 2 CTC
  UINT8    CtdpTar[3];                              ///< Offset 34      CTDP Level 0 TAR
                                                    ///< Offset 35      CTDP Level 1 TAR
                                                    ///< Offset 36      CTDP Level 2 TAR
  UINT8    CtdpPpc[3];                              ///< Offset 37      CTDP Level 0 PPC
                                                    ///< Offset 38      CTDP Level 1 PPC
                                                    ///< Offset 39      CTDP Level 2 PPC
  UINT8    Reserved1[1];                            ///< Offset 40:40
  UINT8    C6MwaitValue;                            ///< Offset 41      Mwait Hint value for C6
  UINT8    C7MwaitValue;                            ///< Offset 42      Mwait Hint value for C7/C7s
  UINT8    CDMwaitValue;                            ///< Offset 43      Mwait Hint value for C7/C8/C9/C10
  UINT8    Reserved2[2];                            ///< Offset 44:45
  UINT16   C6Latency;                               ///< Offset 46      Latency Value for C6
  UINT16   C7Latency;                               ///< Offset 48      Latency Value for C7/C7S
  UINT16   CDLatency;                               ///< Offset 50      Latency Value for C8/C9/C10
  UINT16   CDIOLevel;                               ///< Offset 52      IO LVL value for C8/C9/C10
  UINT16   CDPowerValue;                            ///< Offset 54      Power value for C8/C9/C10
  UINT8    MiscPowerManagementFlags;                ///< Offset 56      MiscPowerManagementFlags
  UINT8    EnableDigitalThermalSensor;              ///< Offset 57      Digital Thermal Sensor Enable
  UINT8    BspDigitalThermalSensorTemperature;      ///< Offset 58      Digital Thermal Sensor 1 Readingn for BSP
  UINT8    ApDigitalThermalSensorTemperature;       ///< Offset 59      Digital Thermal Sensor 2 Reading for AP1
  UINT8    DigitalThermalSensorSmiFunction;         ///< Offset 60      DTS SMI Function Call via DTS IO Trap
  UINT8    PackageDTSTemperature;                   ///< Offset 61      Package Temperature
  UINT8    IsPackageTempMSRAvailable;               ///< Offset 62      Package Temperature MSR available
  UINT8    Ap2DigitalThermalSensorTemperature;      ///< Offset 63      Digital Thermal Sensor 3 Reading for AP2
  UINT8    Ap3DigitalThermalSensorTemperature;      ///< Offset 64      Digital Thermal Sensor 4 Reading for AP3
  UINT64   BiosGuardMemAddress;                     ///< Offset 65      BIOS Guard Memory Address for Tool Interface
  UINT8    BiosGuardMemSize;                        ///< Offset 73      BIOS Guard Memory Size for Tool Interface
  UINT16   BiosGuardIoTrapAddress;                  ///< Offset 74      BIOS Guard IoTrap Address for Tool Interface
  UINT16   BiosGuardIoTrapLength;                   ///< Offset 76      BIOS Guard IoTrap Length for Tool Interface
  UINT16   DtsIoTrapAddress;                        ///< Offset 78      DTS IO trap Address
  UINT8    DtsIoTrapLength;                         ///< Offset 80      DTS IO trap Length
  UINT8    DtsAcpiEnable;                           ///< Offset 81      DTS is in ACPI Mode Enabled
  UINT8    SgxStatus;                               ///< Offset 82      SGX Status
  UINT64   EpcBaseAddress;                          ///< Offset 83      EPC Base Address
  UINT64   EpcLength;                               ///< Offset 91      EPC Length
  UINT8    HwpVersion;                              ///< Offset 99      HWP Version
  UINT8    HwpInterruptStatus;                      ///< Offset 100     HWP Interrupt Status
  UINT8    DtsInterruptStatus;                      ///< Offset 101     DTS Interrupt Status
  UINT8    HwpSmi;                                  ///< Offset 102     SMI to setup HWP LVT tables
  UINT8    LowestMaxPerf;                           ///< Offset 103     Max ratio of the slowest core.
  UINT8    EnableItbm;                              ///< Offset 104     Enable/Disable Intel Turbo Boost Max Technology 3.0.
  UINT8    EnableItbmDriver;                        ///< Offset 105     Enable/Disable Intel Turbo Boost Max Technology 3.0 Driver.
  UINT8    ItbmInterruptStatus;                     ///< Offset 106     Intel Turbo Boost Max Technology 3.0 interrupt status.
  UINT8    ItbmSmi;                                 ///< Offset 107     SMI to resume periodic SMM for Intel Turbo Boost Max Technology 3.0.
  UINT8    OcBins;                                  ///< Offset 108     Indicates bins of Oc support. MSR 194h FLEX_RATIO Bits (19:17)
  UINT8    C3MwaitValue;                            ///< Offset 109     Mwait Hint value for C3 (CFL only)
  UINT16   C3Latency;                               ///< Offset 110     Latency Value for C3 (CFL only)
} CPU_NVS_AREA;

#pragma pack(pop)
#endif
