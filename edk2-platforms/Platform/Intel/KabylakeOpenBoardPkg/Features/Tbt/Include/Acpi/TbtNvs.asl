/**@file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  //
  // Define TBT NVS Area operation region.
  //
  


  OperationRegion(BNVS,SystemMemory,TNVB,TNVL)
  Field(BNVS,AnyAcc,Lock,Preserve)
  {  Offset(0),      TBSF, 8,  // Offset(0),     Thunderbolt(TM) SMI Function Number
  Offset(1),      SOHP, 8,  // Offset(1),     SMI on Hot Plug for TBT devices
  Offset(2),      TWIN, 8,  // Offset(2),     TbtWin10Support
  Offset(3),      GP5F, 8,  // Offset(3),     Gpio filter to detect USB Hotplug event
  Offset(4),      NOHP, 8,  // Offset(4),     Notify on Hot Plug for TBT devices
  Offset(5),      TBSE, 8,  // Offset(5),     Thunderbolt(TM) Root port selector
  Offset(6),      WKFN, 8,  // Offset(6),     WAK Finished
  Offset(7),      TBTS, 8,  // Offset(7),     Thunderbolt(TM) support
  Offset(8),      TARS, 8,  // Offset(8),     TbtAcpiRemovalSupport
  Offset(9),      FPEN, 32, // Offset(9),     TbtFrcPwrEn
  Offset(13),     FPG0, 32, // Offset(13),    TbtFrcPwrGpioNo
  Offset(17),     FP0L, 8,  // Offset(17),    TbtFrcPwrGpioLevel
  Offset(18),     CPG0, 32, // Offset(18),    TbtCioPlugEventGpioNo
  Offset(22),     RSG0, 32, // Offset(22),    TbtPcieRstGpioNo
  Offset(26),     RS0L, 8,  // Offset(26),    TbtPcieRstGpioLevel
  Offset(27),     DTCP, 8,  // Offset(27),    Current Port that has plug event
  Offset(28),     RPS0, 8,  // Offset(28),    Root port Selected by the User
  Offset(29),     RPT0, 8,  // Offset(29),    Root port Type
  Offset(30),     RPS1, 8,  // Offset(30),    Root port Selected by the User
  Offset(31),     RPT1, 8,  // Offset(31),    Root port Type
  Offset(32),     RPN0, 8,  // Offset(32),    Root port Enabled by the User
  Offset(33),     RPN1, 8,  // Offset(33),    Root port Enabled by the User
  Offset(34),     FPG1, 32, // Offset(34),    TbtFrcPwrGpioNo
  Offset(38),     FP1L, 8,  // Offset(38),    TbtFrcPwrGpioLevel
  Offset(39),     CPG1, 32, // Offset(39),    TbtCioPlugEventGpioNo
  Offset(43),     RSG1, 32, // Offset(43),    TbtPcieRstGpioNo
  Offset(47),     RS1L, 8,  // Offset(47),    TbtPcieRstGpioLevel
  Offset(48),     CGST, 8,  // Offset(48),    Set if Single GPIO is used for Multi/Different Controller Hot plug support
  Offset(49),     DTPT, 8,  // Offset(49),    Root Port type for which SCI Triggered
  Offset(50),     TRWA, 8,  // Offset(50),    Titan Ridge Osup command
  Offset(51),     ACDC, 8,  // Offset(51),    TBT Dynamic AcDc L1
  Offset(52),     DT0E, 8,  // Offset(52),    DTbtController0 is enabled or not.
  Offset(53),     DT1E, 8,  // Offset(53),    DTbtController1 is enabled or not.
  Offset(54),     TASP, 8,  // Offset(54),    ASPM setting for all the PCIe device in TBT daisy chain.
  Offset(55),     TL1S, 8,  // Offset(55),    L1 SubState for for all the PCIe device in TBT daisy chain.
  Offset(56),     TCLK, 8,  // Offset(56),    CLK REQ for all the PCIe device in TBT daisy chain.
  Offset(57),     TLTR, 8,  // Offset(57),    LTR for for all the PCIe device in TBT daisy chain.
  Offset(58),     TPTM, 8,  // Offset(58),    PTM for for all the PCIe device in TBT daisy chain.
  Offset(59),     TWAK, 8,  // Offset(59),    Send Go2SxNoWake or GoSxWake according to TbtWakeupSupport
  Offset(60),     TBOD, 16, // Offset(60),    Rtd3TbtOffDelay TBT RTD3 Off Delay
  Offset(62),     TSXW, 8,  // Offset(62),    TbtSxWakeSwitchLogicEnable Set True if TBT_WAKE_N will be routed to PCH WakeB at Sx entry point. HW logic is required.
  Offset(63),     RTBT, 8,  // Offset(63),    Enable Rtd3 support for TBT. Corresponding to Rtd3Tbt in Setup.
  Offset(64),     RTBC, 8,  // Offset(64),    Enable TBT RTD3 CLKREQ mask.
  Offset(65),     TBCD, 16, // Offset(65),    TBT RTD3 CLKREQ mask delay.
  }