/** @file
  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

  //
  // Define SA NVS Area operatino region.
  //



  OperationRegion(SANV,SystemMemory, 0xFFFF0000,0xAA55)
  Field(SANV,AnyAcc,Lock,Preserve)
  {  Offset(0),      ASLB, 32, // Offset(0),     IGD OpRegion base address
  Offset(4),      IMON, 8,  // Offset(4),     IMON Current Value
  Offset(5),      IGDS, 8,  // Offset(5),     IGD State (Primary Display = 1)
  Offset(6),      IBTT, 8,  // Offset(6),     IGD Boot Display Device
  Offset(7),      IPAT, 8,  // Offset(7),     IGD Panel Type CMOS option
  Offset(8),      IPSC, 8,  // Offset(8),     IGD Panel Scaling
  Offset(9),      IBIA, 8,  // Offset(9),     IGD BIA Configuration
  Offset(10),     ISSC, 8,  // Offset(10),    IGD SSC Configuration
  Offset(11),     IDMS, 8,  // Offset(11),    IGD DVMT Memory Size
  Offset(12),     IF1E, 8,  // Offset(12),    IGD Function 1 Enable
  Offset(13),     HVCO, 8,  // Offset(13),    HPLL VCO
  Offset(14),     GSMI, 8,  // Offset(14),    GMCH SMI/SCI mode (0=SCI)
  Offset(15),     PAVP, 8,  // Offset(15),    IGD PAVP data
  Offset(16),     CADL, 8,  // Offset(16),    Current Attached Device List
  Offset(17),     CSTE, 16, // Offset(17),    Current Display State
  Offset(19),     NSTE, 16, // Offset(19),    Next Display State
  Offset(21),     NDID, 8,  // Offset(21),    Number of Valid Device IDs
  Offset(22),     DID1, 32, // Offset(22),    Device ID 1
  Offset(26),     DID2, 32, // Offset(26),    Device ID 2
  Offset(30),     DID3, 32, // Offset(30),    Device ID 3
  Offset(34),     DID4, 32, // Offset(34),    Device ID 4
  Offset(38),     DID5, 32, // Offset(38),    Device ID 5
  Offset(42),     DID6, 32, // Offset(42),    Device ID 6
  Offset(46),     DID7, 32, // Offset(46),    Device ID 7
  Offset(50),     DID8, 32, // Offset(50),    Device ID 8
  Offset(54),     DID9, 32, // Offset(54),    Device ID 9
  Offset(58),     DIDA, 32, // Offset(58),    Device ID 10
  Offset(62),     DIDB, 32, // Offset(62),    Device ID 11
  Offset(66),     DIDC, 32, // Offset(66),    Device ID 12
  Offset(70),     DIDD, 32, // Offset(70),    Device ID 13
  Offset(74),     DIDE, 32, // Offset(74),    Device ID 14
  Offset(78),     DIDF, 32, // Offset(78),    Device ID 15
  Offset(82),     DIDX, 32, // Offset(82),    Device ID for eDP device
  Offset(86),     NXD1, 32, // Offset(86),    Next state DID1 for _DGS
  Offset(90),     NXD2, 32, // Offset(90),    Next state DID2 for _DGS
  Offset(94),     NXD3, 32, // Offset(94),    Next state DID3 for _DGS
  Offset(98),     NXD4, 32, // Offset(98),    Next state DID4 for _DGS
  Offset(102),    NXD5, 32, // Offset(102),   Next state DID5 for _DGS
  Offset(106),    NXD6, 32, // Offset(106),   Next state DID6 for _DGS
  Offset(110),    NXD7, 32, // Offset(110),   Next state DID7 for _DGS
  Offset(114),    NXD8, 32, // Offset(114),   Next state DID8 for _DGS
  Offset(118),    NXDX, 32, // Offset(118),   Next state DID for eDP
  Offset(122),    LIDS, 8,  // Offset(122),   Lid State (Lid Open = 1)
  Offset(123),    KSV0, 32, // Offset(123),   First four bytes of AKSV (manufacturing mode)
  Offset(127),    KSV1, 8,  // Offset(127),   Fifth byte of AKSV (manufacturing mode)
  Offset(128),    BRTL, 8,  // Offset(128),   Brightness Level Percentage
  Offset(129),    ALSE, 8,  // Offset(129),   Ambient Light Sensor Enable
  Offset(130),    ALAF, 8,  // Offset(130),   Ambient Light Adjusment Factor
  Offset(131),    LLOW, 8,  // Offset(131),   LUX Low Value
  Offset(132),    LHIH, 8,  // Offset(132),   LUX High Value
  Offset(133),    ALFP, 8,  // Offset(133),   Active LFP
  Offset(134),    IPTP, 8,  // Offset(134),   IPU ACPI device type (0=Disabled, 1=AVStream virtual device as child of GFX)
  Offset(135),    EDPV, 8,  // Offset(135),   Check for eDP display device
  Offset(136),    SGMD, 8,  // Offset(136),   SG Mode (0=Disabled, 1=SG Muxed, 2=SG Muxless, 3=DGPU Only)
  Offset(137),    SGFL, 8,  // Offset(137),   SG Feature List
  Offset(138),    SGGP, 8,  // Offset(138),   PCIe0 GPIO Support (0=Disabled, 1=PCH Based, 2=I2C Based)
  Offset(139),    HRE0, 8,  // Offset(139),   PCIe0 HLD RST IO Expander Number
  Offset(140),    HRG0, 32, // Offset(140),   PCIe0 HLD RST GPIO Number
  Offset(144),    HRA0, 8,  // Offset(144),   PCIe0 HLD RST GPIO Active Information
  Offset(145),    PWE0, 8,  // Offset(145),   PCIe0 PWR Enable IO Expander Number
  Offset(146),    PWG0, 32, // Offset(146),   PCIe0 PWR Enable GPIO Number
  Offset(150),    PWA0, 8,  // Offset(150),   PCIe0 PWR Enable GPIO Active Information
  Offset(151),    P1GP, 8,  // Offset(151),   PCIe1 GPIO Support (0=Disabled, 1=PCH Based, 2=I2C Based)
  Offset(152),    HRE1, 8,  // Offset(152),   PCIe1 HLD RST IO Expander Number
  Offset(153),    HRG1, 32, // Offset(153),   PCIe1 HLD RST GPIO Number
  Offset(157),    HRA1, 8,  // Offset(157),   PCIe1 HLD RST GPIO Active Information
  Offset(158),    PWE1, 8,  // Offset(158),   PCIe1 PWR Enable IO Expander Number
  Offset(159),    PWG1, 32, // Offset(159),   PCIe1 PWR Enable GPIO Number
  Offset(163),    PWA1, 8,  // Offset(163),   PCIe1 PWR Enable GPIO Active Information
  Offset(164),    P2GP, 8,  // Offset(164),   PCIe2 GPIO Support (0=Disabled, 1=PCH Based, 2=I2C Based)
  Offset(165),    HRE2, 8,  // Offset(165),   PCIe2 HLD RST IO Expander Number
  Offset(166),    HRG2, 32, // Offset(166),   PCIe2 HLD RST GPIO Number
  Offset(170),    HRA2, 8,  // Offset(170),   PCIe2 HLD RST GPIO Active Information
  Offset(171),    PWE2, 8,  // Offset(171),   PCIe2 PWR Enable IO Expander Number
  Offset(172),    PWG2, 32, // Offset(172),   PCIe2 PWR Enable GPIO Number
  Offset(176),    PWA2, 8,  // Offset(176),   PCIe2 PWR Enable GPIO Active Information
  Offset(177),    DLPW, 16, // Offset(177),   Delay after power enable for PCIe
  Offset(179),    DLHR, 16, // Offset(179),   Delay after Hold Reset for PCIe
  Offset(181),    EECP, 8,  // Offset(181),   PCIe0 Endpoint Capability Structure Offset
  Offset(182),    XBAS, 32, // Offset(182),   Any Device's PCIe Config Space Base Address
  Offset(186),    GBAS, 16, // Offset(186),   GPIO Base Address
  Offset(188),    NVGA, 32, // Offset(188),   NVIG opregion address
  Offset(192),    NVHA, 32, // Offset(192),   NVHM opregion address
  Offset(196),    AMDA, 32, // Offset(196),   AMDA opregion address
  Offset(200),    LTRX, 8,  // Offset(200),   Latency Tolerance Reporting Enable
  Offset(201),    OBFX, 8,  // Offset(201),   Optimized Buffer Flush and Fill
  Offset(202),    LTRY, 8,  // Offset(202),   Latency Tolerance Reporting Enable
  Offset(203),    OBFY, 8,  // Offset(203),   Optimized Buffer Flush and Fill
  Offset(204),    LTRZ, 8,  // Offset(204),   Latency Tolerance Reporting Enable
  Offset(205),    OBFZ, 8,  // Offset(205),   Optimized Buffer Flush and Fill
  Offset(206),    LTRW, 8,  // Offset(206),   Latency Tolerance Reporting Enable
  Offset(207),    OBFA, 8,  // Offset(207),   Optimized Buffer Flush and Fill
  Offset(208),    SMSL, 16, // Offset(208),   SA Peg Latency Tolerance Reporting Max Snoop Latency
  Offset(210),    SNSL, 16, // Offset(210),   SA Peg Latency Tolerance Reporting Max No Snoop Latency
  Offset(212),    P0UB, 8,  // Offset(212),   Peg0 Unused Bundle Control
  Offset(213),    P1UB, 8,  // Offset(213),   Peg1 Unused Bundle Control
  Offset(214),    P2UB, 8,  // Offset(214),   Peg2 Unused Bundle Control
  Offset(215),    P3UB, 8,  // Offset(215),   Peg3 Unused Bundle Control
  Offset(216),    PCSL, 8,  // Offset(216),   The lowest C-state for the package
  Offset(217),    PBGE, 8,  // Offset(217),   Pegx Unused Bundle Control Global Enable (0=Disabled, 1=Enabled)
  Offset(218),    M64B, 64, // Offset(218),   Base of above 4GB MMIO resource
  Offset(226),    M64L, 64, // Offset(226),   Length of above 4GB MMIO resource
  Offset(234),    CPEX, 32, // Offset(234),   CPU ID info to get Family Id or Stepping
  Offset(238),    EEC1, 8,  // Offset(238),   PCIe1 Endpoint Capability Structure Offset
  Offset(239),    EEC2, 8,  // Offset(239),   PCIe2 Endpoint Capability Structure Offset
  Offset(240),    SBN0, 8,  // Offset(240),   PCIe0 Secondary Bus Number (PCIe0 Endpoint Bus Number)
  Offset(241),    SBN1, 8,  // Offset(241),   PCIe1 Secondary Bus Number (PCIe0 Endpoint Bus Number)
  Offset(242),    SBN2, 8,  // Offset(242),   PCIe2 Secondary Bus Number (PCIe0 Endpoint Bus Number)
  Offset(243),    M32B, 32, // Offset(243),   Base of below 4GB MMIO resource
  Offset(247),    M32L, 32, // Offset(247),   Length of below 4GB MMIO resource
  Offset(251),    P0WK, 32, // Offset(251),   PCIe0 RTD3 Device Wake GPIO Number
  Offset(255),    P1WK, 32, // Offset(255),   PCIe1 RTD3 Device Wake GPIO Number
  Offset(259),    P2WK, 32, // Offset(259),   PCIe2 RTD3 Device Wake GPIO Number
  Offset(263),    VTDS, 8,  // Offset(263),   VT-d Enable/Disable
  Offset(264),    VTB1, 32, // Offset(264),   VT-d Base Address 1
  Offset(268),    VTB2, 32, // Offset(268),   VT-d Base Address 2
  Offset(272),    VTB3, 32, // Offset(272),   VT-d Base Address 3
  Offset(276),    VE1V, 16, // Offset(276),   VT-d Engine#1 Vendor ID
  Offset(278),    VE2V, 16, // Offset(278),   VT-d Engine#2 Vendor ID
  Offset(280),    SBN3, 8,  // Offset(280),   PCIe3 Secondary Bus Number (PCIe3 Endpoint Bus Number)
  Offset(281),    P3GP, 8,  // Offset(281),   PCIe3 GPIO Support (0=Disabled, 1=PCH Based, 2=I2C Based)
  Offset(282),    HRE3, 8,  // Offset(282),   PCIe3 HLD RST IO Expander Number
  Offset(283),    HRG3, 32, // Offset(283),   PCIe3 HLD RST GPIO Number
  Offset(287),    HRA3, 8,  // Offset(287),   PCIe3 HLD RST GPIO Active Information
  Offset(288),    PWE3, 8,  // Offset(288),   PCIe3 PWR Enable IO Expander Number
  Offset(289),    PWG3, 32, // Offset(289),   PCIe3 PWR Enable GPIO Number
  Offset(293),    PWA3, 8,  // Offset(293),   PCIe3 PWR Enable GPIO Active Information
  Offset(294),    P3WK, 32, // Offset(294),   PCIe3 RTD3 Device Wake GPIO Number
  Offset(298),    EEC3, 8,  // Offset(298),   PCIe3 Endpoint Capability Structure Offset
  Offset(299),    RPIN, 8,  // Offset(299),   RootPort Number
  Offset(300),    RPBA, 32, // Offset(300),   RootPortAddress
  Offset (500),             // Offset(304) : Offset(499), Reserved bytes
  }
