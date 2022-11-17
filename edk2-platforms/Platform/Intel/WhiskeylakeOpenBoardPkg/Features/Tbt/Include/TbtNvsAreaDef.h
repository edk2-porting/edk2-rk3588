/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

  //
  // Define TBT NVS Area operation region.
  //

#ifndef _TBT_NVS_AREA_DEF_H_
#define _TBT_NVS_AREA_DEF_H_

#pragma pack (push,1)
typedef struct {
  UINT8    ThunderboltSmiFunction;                  ///< Offset 0       Thunderbolt(TM) SMI Function Number
  UINT8    ThunderboltHotSmi;                       ///< Offset 1       SMI on Hot Plug for TBT devices
  UINT8    TbtWin10Support;                         ///< Offset 2       TbtWin10Support
  UINT8    TbtGpioFilter;                           ///< Offset 3       Gpio filter to detect USB Hotplug event
  UINT8    ThunderboltHotNotify;                    ///< Offset 4       Notify on Hot Plug for TBT devices
  UINT8    TbtSelector;                             ///< Offset 5       Thunderbolt(TM) Root port selector
  UINT8    WAKFinished;                             ///< Offset 6       WAK Finished
  UINT8    DiscreteTbtSupport;                      ///< Offset 7       Thunderbolt(TM) support
  UINT8    TbtAcpiRemovalSupport;                   ///< Offset 8       TbtAcpiRemovalSupport
  UINT32   TbtFrcPwrEn;                             ///< Offset 9       TbtFrcPwrEn
  UINT32   TbtFrcPwrGpioNo0;                        ///< Offset 13      TbtFrcPwrGpioNo
  UINT8    TbtFrcPwrGpioLevel0;                     ///< Offset 17      TbtFrcPwrGpioLevel
  UINT32   TbtCioPlugEventGpioNo0;                  ///< Offset 18      TbtCioPlugEventGpioNo
  UINT32   TbtPcieRstGpioNo0;                       ///< Offset 22      TbtPcieRstGpioNo
  UINT8    TbtPcieRstGpioLevel0;                    ///< Offset 26      TbtPcieRstGpioLevel
  UINT8    CurrentDiscreteTbtRootPort;              ///< Offset 27      Current Port that has plug event
  UINT8    RootportSelected0;                       ///< Offset 28      Root port Selected by the User
  UINT8    RootportSelected0Type;                   ///< Offset 29      Root port Type
  UINT8    RootportSelected1;                       ///< Offset 30      Root port Selected by the User
  UINT8    RootportSelected1Type;                   ///< Offset 31      Root port Type
  UINT8    RootportEnabled0;                        ///< Offset 32      Root port Enabled by the User
  UINT8    RootportEnabled1;                        ///< Offset 33      Root port Enabled by the User
  UINT32   TbtFrcPwrGpioNo1;                        ///< Offset 34      TbtFrcPwrGpioNo
  UINT8    TbtFrcPwrGpioLevel1;                     ///< Offset 38      TbtFrcPwrGpioLevel
  UINT32   TbtCioPlugEventGpioNo1;                  ///< Offset 39      TbtCioPlugEventGpioNo
  UINT32   TbtPcieRstGpioNo1;                       ///< Offset 43      TbtPcieRstGpioNo
  UINT8    TbtPcieRstGpioLevel1;                    ///< Offset 47      TbtPcieRstGpioLevel
  UINT8    TBtCommonGpioSupport;                    ///< Offset 48      Set if Single GPIO is used for Multi/Different Controller Hot plug support
  UINT8    CurrentDiscreteTbtRootPortType;          ///< Offset 49      Root Port type for which SCI Triggered
  UINT8    TrOsup;                                  ///< Offset 50      Titan Ridge Osup command
  UINT8    TbtAcDcSwitch;                           ///< Offset 51      TBT Dynamic AcDc L1
  UINT8    DTbtControllerEn0;                       ///< Offset 52      DTbtController0 is enabled or not.  @deprecated since revision 2
  UINT8    DTbtControllerEn1;                       ///< Offset 53      DTbtController1 is enabled or not.  @deprecated since revision 2
  UINT8    TbtAspm;                                 ///< Offset 54      ASPM setting for all the PCIe device in TBT daisy chain.
  UINT8    TbtL1SubStates;                          ///< Offset 55      L1 SubState for for all the PCIe device in TBT daisy chain.
  UINT8    TbtSetClkReq;                            ///< Offset 56      CLK REQ for all the PCIe device in TBT daisy chain.
  UINT8    TbtLtr;                                  ///< Offset 57      LTR for for all the PCIe device in TBT daisy chain.
  UINT8    TbtPtm;                                  ///< Offset 58      PTM for for all the PCIe device in TBT daisy chain.
  UINT8    TbtWakeupSupport;                        ///< Offset 59      Send Go2SxNoWake or GoSxWake according to TbtWakeupSupport
  UINT16   Rtd3TbtOffDelay;                         ///< Offset 60      Rtd3TbtOffDelay TBT RTD3 Off Delay
  UINT8    TbtSxWakeSwitchLogicEnable;              ///< Offset 62      TbtSxWakeSwitchLogicEnable Set True if TBT_WAKE_N will be routed to PCH WakeB at Sx entry point. HW logic is required.
  UINT8    Rtd3TbtSupport;                          ///< Offset 63      Enable Rtd3 support for TBT. Corresponding to Rtd3Tbt in Setup.
  UINT8    Rtd3TbtClkReq;                           ///< Offset 64      Enable TBT RTD3 CLKREQ mask.
  UINT16   Rtd3TbtClkReqDelay;                      ///< Offset 65      TBT RTD3 CLKREQ mask delay.
  //
  // Revision Field:
  //
  UINT8    TbtRevision;                             ///< Offset 67      Revison of TbtNvsArea
} TBT_NVS_AREA;

#pragma pack(pop)
#endif

