/** @file
  ACPI DSDT table

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  // Define a Global region of ACPI NVS Region that may be used for any
  // type of implementation.  The starting offset and size will be fixed
  // up by the System BIOS during POST.  Note that the Size must be a word
  // in size to be fixed up correctly.

  
#ifndef _GLOBAL_NVS_AREA_DEF_H_
#define _GLOBAL_NVS_AREA_DEF_H_

#pragma pack (push,1)
typedef struct {
  //
  // Miscellaneous Dynamic Registers:
  //
  UINT16   OperatingSystem;                         ///< Offset 0       Operating System
  UINT8    SmiFunction;                             ///< Offset 2       SMI Function Call (ASL to SMI via I/O Trap)
  UINT32   Port80DebugValue;                        ///< Offset 3       Port 80 Debug Port Value
  UINT8    PowerState;                              ///< Offset 7       Power State (AC Mode = 1)
  //
  // Thermal Policy Registers:
  //
  UINT8    EnableDigitalThermalSensor;              ///< Offset 8       Digital Thermal Sensor Enable
  UINT8    DigitalThermalSensorSmiFunction;         ///< Offset 9       DTS SMI Function Call
  //                 
  // CPU Identification Registers:
  //
  UINT8    ApicEnable;                              ///< Offset 10      APIC Enabled by SBIOS (APIC Enabled = 1)
  UINT8    ThreadCount;                             ///< Offset 11      Number of Enabled Threads
  //
  // PCIe Hot Plug
  //
  UINT8    PcieOSCControl;                          ///< Offset 12      PCIE OSC Control
  UINT8    NativePCIESupport;                       ///< Offset 13      Native PCIE Setup Value
  //
  // Global Variables
  //
  UINT8    DisplaySupportFlag;                      ///< Offset 14      _DOS Display Support Flag.
  UINT8    InterruptModeFlag;                       ///< Offset 15      Global IOAPIC/8259 Interrupt Mode Flag.
  UINT8    L01Counter;                              ///< Offset 16      Global L01 Counter.
  UINT8    LtrEnable[24];                           ///< Offset 17      Latency Tolerance Reporting Enable
                                                    ///< Offset 18      Latency Tolerance Reporting Enable
                                                    ///< Offset 19      Latency Tolerance Reporting Enable
                                                    ///< Offset 20      Latency Tolerance Reporting Enable
                                                    ///< Offset 21      Latency Tolerance Reporting Enable
                                                    ///< Offset 22      Latency Tolerance Reporting Enable
                                                    ///< Offset 23      Latency Tolerance Reporting Enable
                                                    ///< Offset 24      Latency Tolerance Reporting Enable
                                                    ///< Offset 25      Latency Tolerance Reporting Enable
                                                    ///< Offset 26      Latency Tolerance Reporting Enable
                                                    ///< Offset 27      Latency Tolerance Reporting Enable
                                                    ///< Offset 28      Latency Tolerance Reporting Enable
                                                    ///< Offset 29      Latency Tolerance Reporting Enable
                                                    ///< Offset 30      Latency Tolerance Reporting Enable
                                                    ///< Offset 31      Latency Tolerance Reporting Enable
                                                    ///< Offset 32      Latency Tolerance Reporting Enable
                                                    ///< Offset 33      Latency Tolerance Reporting Enable
                                                    ///< Offset 34      Latency Tolerance Reporting Enable
                                                    ///< Offset 35      Latency Tolerance Reporting Enable
                                                    ///< Offset 36      Latency Tolerance Reporting Enable
                                                    ///< Offset 37      Latency Tolerance Reporting Enable
                                                    ///< Offset 38      Latency Tolerance Reporting Enable
                                                    ///< Offset 39      Latency Tolerance Reporting Enable
                                                    ///< Offset 40      Latency Tolerance Reporting Enable
  UINT8    ObffEnable[24];                          ///< Offset 41      Optimized Buffer Flush and Fill
                                                    ///< Offset 42      Optimized Buffer Flush and Fill
                                                    ///< Offset 43      Optimized Buffer Flush and Fill
                                                    ///< Offset 44      Optimized Buffer Flush and Fill
                                                    ///< Offset 45      Optimized Buffer Flush and Fill
                                                    ///< Offset 46      Optimized Buffer Flush and Fill
                                                    ///< Offset 47      Optimized Buffer Flush and Fill
                                                    ///< Offset 48      Optimized Buffer Flush and Fill
                                                    ///< Offset 49      Optimized Buffer Flush and Fill
                                                    ///< Offset 50      Optimized Buffer Flush and Fill
                                                    ///< Offset 51      Optimized Buffer Flush and Fill
                                                    ///< Offset 52      Optimized Buffer Flush and Fill
                                                    ///< Offset 53      Optimized Buffer Flush and Fill
                                                    ///< Offset 54      Optimized Buffer Flush and Fill
                                                    ///< Offset 55      Optimized Buffer Flush and Fill
                                                    ///< Offset 56      Optimized Buffer Flush and Fill
                                                    ///< Offset 57      Optimized Buffer Flush and Fill
                                                    ///< Offset 58      Optimized Buffer Flush and Fill
                                                    ///< Offset 59      Optimized Buffer Flush and Fill
                                                    ///< Offset 60      Optimized Buffer Flush and Fill
                                                    ///< Offset 61      Optimized Buffer Flush and Fill
                                                    ///< Offset 62      Optimized Buffer Flush and Fill
                                                    ///< Offset 63      Optimized Buffer Flush and Fill
                                                    ///< Offset 64      Optimized Buffer Flush and Fill
  UINT8    Rtd3Support;                             ///< Offset 65      Runtime D3 support.
  UINT8    LowPowerS0Idle;                          ///< Offset 66      Low Power S0 Idle Enable
  UINT8    VirtualGpioButtonSxBitmask;              ///< Offset 67      Virtual GPIO button Notify Sleep State Change
  UINT8    PstateCapping;                           ///< Offset 68      P-state Capping
  UINT8    Ps2MouseEnable;                          ///< Offset 69      Ps2 Mouse Enable
  UINT8    Ps2KbMsEnable;                           ///< Offset 70      Ps2 Keyboard and Mouse Enable
  //
  // Driver Mode
  //
  UINT32   GpioIrqRoute;                            ///< Offset 71      GPIO IRQ
  UINT8    PL1LimitCS;                              ///< Offset 75      set PL1 limit when entering CS
  UINT16   PL1LimitCSValue;                         ///< Offset 76      PL1 limit value
  UINT8    TenSecondPowerButtonEnable;              ///< Offset 78      10sec Power button support
  UINT8    PciDelayOptimizationEcr;                 ///< Offset 79      Pci Delay Optimization Ecr
  UINT8    TbtSupport;                              ///< Offset 80      Thunderbolt(TM) support
  UINT8    TbtNativeOsHotPlug;                      ///< Offset 81      TbtNativeOsHotPlug
  UINT8    TbtSelector;                             ///< Offset 82      Thunderbolt(TM) Root port selector
  UINT8    TbtSelector1;                            ///< Offset 83      Thunderbolt(TM) Root port selector
  UINT8    BoardId;                                 ///< Offset 84      Board ID
} EFI_GLOBAL_NVS_AREA;

#pragma pack(pop)
#endif
