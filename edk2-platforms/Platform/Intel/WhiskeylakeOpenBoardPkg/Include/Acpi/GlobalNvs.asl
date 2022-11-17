/** @file
  ACPI DSDT table


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

  // Define a Global region of ACPI NVS Region that may be used for any
  // type of implementation.  The starting offset and size will be fixed
  // up by the System BIOS during POST.  Note that the Size must be a word
  // in size to be fixed up correctly.

  OperationRegion(GNVS,SystemMemory,0xFFFF0000,0xAA55)
  Field(GNVS,AnyAcc,Lock,Preserve)
  {
  //
  // Miscellaneous Dynamic Registers:
  //
  Offset(0),      OSYS, 16, // Offset(0),     Operating System
  Offset(2),      SMIF, 8,  // Offset(2),     SMI Function Call (ASL to SMI via I/O Trap)
  Offset(3),      P80D, 32, // Offset(3),     Port 80 Debug Port Value
  Offset(7),      PWRS, 8,  // Offset(7),     Power State (AC Mode = 1)
  //
  // Thermal Policy Registers:
  //
  Offset(8),      DTSE, 8,  // Offset(8),    Digital Thermal Sensor Enable
  Offset(9),      DTSF, 8,  // Offset(9),    DTS SMI Function Call
  //
  // CPU Identification Registers:
  //
  Offset(10),     APIC, 8,  // Offset(10),    APIC Enabled by SBIOS (APIC Enabled = 1)
  Offset(11),     TCNT, 8,  // Offset(11),    Number of Enabled Threads
  //
  // PCIe Hot Plug
  //
  Offset(12),     OSCC, 8,  // Offset(12),    PCIE OSC Control
  Offset(13),     NEXP, 8,  // Offset(13),    Native PCIE Setup Value
  //
  // Global Variables
  //
  Offset(14),     DSEN, 8,  // Offset(14),    _DOS Display Support Flag.
  Offset(15),     GPIC, 8,  // Offset(15),    Global IOAPIC/8259 Interrupt Mode Flag.
  Offset(16),     L01C, 8,  // Offset(16),    Global L01 Counter.
  Offset(17),     LTR1, 8,  // Offset(17),    Latency Tolerance Reporting Enable
  Offset(18),     LTR2, 8,  // Offset(18),    Latency Tolerance Reporting Enable
  Offset(19),     LTR3, 8,  // Offset(19),    Latency Tolerance Reporting Enable
  Offset(20),     LTR4, 8,  // Offset(20),    Latency Tolerance Reporting Enable
  Offset(21),     LTR5, 8,  // Offset(21),    Latency Tolerance Reporting Enable
  Offset(22),     LTR6, 8,  // Offset(22),    Latency Tolerance Reporting Enable
  Offset(23),     LTR7, 8,  // Offset(23),    Latency Tolerance Reporting Enable
  Offset(24),     LTR8, 8,  // Offset(24),    Latency Tolerance Reporting Enable
  Offset(25),     LTR9, 8,  // Offset(25),    Latency Tolerance Reporting Enable
  Offset(26),     LTRA, 8,  // Offset(26),    Latency Tolerance Reporting Enable
  Offset(27),     LTRB, 8,  // Offset(27),    Latency Tolerance Reporting Enable
  Offset(28),     LTRC, 8,  // Offset(28),    Latency Tolerance Reporting Enable
  Offset(29),     LTRD, 8,  // Offset(29),    Latency Tolerance Reporting Enable
  Offset(30),     LTRE, 8,  // Offset(30),    Latency Tolerance Reporting Enable
  Offset(31),     LTRF, 8,  // Offset(31),    Latency Tolerance Reporting Enable
  Offset(32),     LTRG, 8,  // Offset(32),    Latency Tolerance Reporting Enable
  Offset(33),     LTRH, 8,  // Offset(33),    Latency Tolerance Reporting Enable
  Offset(34),     LTRI, 8,  // Offset(34),    Latency Tolerance Reporting Enable
  Offset(35),     LTRJ, 8,  // Offset(35),    Latency Tolerance Reporting Enable
  Offset(36),     LTRK, 8,  // Offset(36),    Latency Tolerance Reporting Enable
  Offset(37),     LTRL, 8,  // Offset(37),    Latency Tolerance Reporting Enable
  Offset(38),     LTRM, 8,  // Offset(38),    Latency Tolerance Reporting Enable
  Offset(39),     LTRN, 8,  // Offset(39),    Latency Tolerance Reporting Enable
  Offset(40),     LTRO, 8,  // Offset(40),    Latency Tolerance Reporting Enable
  Offset(41),     OBF1, 8,  // Offset(41),    Optimized Buffer Flush and Fill
  Offset(42),     OBF2, 8,  // Offset(42),    Optimized Buffer Flush and Fill
  Offset(43),     OBF3, 8,  // Offset(43),    Optimized Buffer Flush and Fill
  Offset(44),     OBF4, 8,  // Offset(44),    Optimized Buffer Flush and Fill
  Offset(45),     OBF5, 8,  // Offset(45),    Optimized Buffer Flush and Fill
  Offset(46),     OBF6, 8,  // Offset(46),    Optimized Buffer Flush and Fill
  Offset(47),     OBF7, 8,  // Offset(47),    Optimized Buffer Flush and Fill
  Offset(48),     OBF8, 8,  // Offset(48),    Optimized Buffer Flush and Fill
  Offset(49),     OBF9, 8,  // Offset(49),    Optimized Buffer Flush and Fill
  Offset(50),     OBFA, 8,  // Offset(50),    Optimized Buffer Flush and Fill
  Offset(51),     OBFB, 8,  // Offset(51),    Optimized Buffer Flush and Fill
  Offset(52),     OBFC, 8,  // Offset(52),    Optimized Buffer Flush and Fill
  Offset(53),     OBFD, 8,  // Offset(53),    Optimized Buffer Flush and Fill
  Offset(54),     OBFE, 8,  // Offset(54),    Optimized Buffer Flush and Fill
  Offset(55),     OBFF, 8,  // Offset(55),    Optimized Buffer Flush and Fill
  Offset(56),     OBFG, 8,  // Offset(56),    Optimized Buffer Flush and Fill
  Offset(57),     OBFH, 8,  // Offset(57),    Optimized Buffer Flush and Fill
  Offset(58),     OBFI, 8,  // Offset(58),    Optimized Buffer Flush and Fill
  Offset(59),     OBFJ, 8,  // Offset(59),    Optimized Buffer Flush and Fill
  Offset(60),     OBFK, 8,  // Offset(60),    Optimized Buffer Flush and Fill
  Offset(61),     OBFL, 8,  // Offset(61),    Optimized Buffer Flush and Fill
  Offset(62),     OBFM, 8,  // Offset(62),    Optimized Buffer Flush and Fill
  Offset(63),     OBFN, 8,  // Offset(63),    Optimized Buffer Flush and Fill
  Offset(64),     OBFO, 8,  // Offset(64),    Optimized Buffer Flush and Fill
  Offset(65),     RTD3, 8,  // Offset(65),    Runtime D3 support.
  Offset(66),     S0ID, 8,  // Offset(66),    Low Power S0 Idle Enable
  Offset(67),     GBSX, 8,  // Offset(67),    Virtual GPIO button Notify Sleep State Change
  Offset(68),     PSCP, 8,  // Offset(68),    P-state Capping
  Offset(69),     P2ME, 8,  // Offset(69),    Ps2 Mouse Enable
  Offset(70),     P2MK, 8,  // Offset(70),    Ps2 Keyboard and Mouse Enable
  //
  // Driver Mode
  //
  Offset(71),     GIRQ, 32, // Offset(71),    GPIO IRQ
  Offset(75),     PLCS, 8,  // Offset(75),    set PL1 limit when entering CS
  Offset(76),     PLVL, 16, // Offset(76),    PL1 limit value
  Offset(78),     PB1E, 8,  // Offset(78),    10sec Power button support
  Offset(79),     ECR1, 8,  // Offset(79),    Pci Delay Optimization Ecr
  Offset(80),     TBTS, 8,  // Offset(80),    Thunderbolt(TM) support
  Offset(81),     TNAT, 8,  // Offset(81),    TbtNativeOsHotPlug
  Offset(82),     TBSE, 8,  // Offset(82),    Thunderbolt(TM) Root port selector
  Offset(83),     TBS1, 8,  // Offset(83),    Thunderbolt(TM) Root port selector
  }

