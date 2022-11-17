/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  //
  // Define PCH NVS Area operatino region.
  //
  


  OperationRegion(PNVA,SystemMemory,PNVB,PNVL)
  Field(PNVA,AnyAcc,Lock,Preserve)
  {
  Offset(0),      PCHS, 16, // Offset(0),     PCH Series
  Offset(2),      PCHG, 16, // Offset(2),     PCH Generation
  Offset(4),      RPA1, 32, // Offset(4),     Root Port address 1
  Offset(8),      RPA2, 32, // Offset(8),     Root Port address 2
  Offset(12),     RPA3, 32, // Offset(12),    Root Port address 3
  Offset(16),     RPA4, 32, // Offset(16),    Root Port address 4
  Offset(20),     RPA5, 32, // Offset(20),    Root Port address 5
  Offset(24),     RPA6, 32, // Offset(24),    Root Port address 6
  Offset(28),     RPA7, 32, // Offset(28),    Root Port address 7
  Offset(32),     RPA8, 32, // Offset(32),    Root Port address 8
  Offset(36),     RPA9, 32, // Offset(36),    Root Port address 9
  Offset(40),     RPAA, 32, // Offset(40),    Root Port address 10
  Offset(44),     RPAB, 32, // Offset(44),    Root Port address 11
  Offset(48),     RPAC, 32, // Offset(48),    Root Port address 12
  Offset(52),     RPAD, 32, // Offset(52),    Root Port address 13
  Offset(56),     RPAE, 32, // Offset(56),    Root Port address 14
  Offset(60),     RPAF, 32, // Offset(60),    Root Port address 15
  Offset(64),     RPAG, 32, // Offset(64),    Root Port address 16
  Offset(68),     RPAH, 32, // Offset(68),    Root Port address 17
  Offset(72),     RPAI, 32, // Offset(72),    Root Port address 18
  Offset(76),     RPAJ, 32, // Offset(76),    Root Port address 19
  Offset(80),     RPAK, 32, // Offset(80),    Root Port address 20
  Offset(84),     RPAL, 32, // Offset(84),    Root Port address 21
  Offset(88),     RPAM, 32, // Offset(88),    Root Port address 22
  Offset(92),     RPAN, 32, // Offset(92),    Root Port address 23
  Offset(96),     RPAO, 32, // Offset(96),    Root Port address 24
  Offset(100),    NHLA, 64, // Offset(100),   HD-Audio NHLT ACPI address
  Offset(108),    NHLL, 32, // Offset(108),   HD-Audio NHLT ACPI length
  Offset(112),    ADFM, 32, // Offset(112),   HD-Audio DSP Feature Mask
  Offset(116),    SBRG, 32, // Offset(116),   SBREG_BAR
  Offset(120),    GPEM, 32, // Offset(120),   GPP_X to GPE_DWX mapping
  Offset(124),    G2L0, 32, // Offset(124),   GPE 2-tier level edged enabled Gpio pads (Group Index 0)
  Offset(128),    G2L1, 32, // Offset(128),   GPE 2-tier level edged enabled Gpio pads (Group Index 1)
  Offset(132),    G2L2, 32, // Offset(132),   GPE 2-tier level edged enabled Gpio pads (Group Index 2)
  Offset(136),    G2L3, 32, // Offset(136),   GPE 2-tier level edged enabled Gpio pads (Group Index 3)
  Offset(140),    G2L4, 32, // Offset(140),   GPE 2-tier level edged enabled Gpio pads (Group Index 4)
  Offset(144),    G2L5, 32, // Offset(144),   GPE 2-tier level edged enabled Gpio pads (Group Index 5)
  Offset(148),    G2L6, 32, // Offset(148),   GPE 2-tier level edged enabled Gpio pads (Group Index 6)
  Offset(152),    G2L7, 32, // Offset(152),   GPE 2-tier level edged enabled Gpio pads (Group Index 7)
  Offset(156),    G2L8, 32, // Offset(156),   GPE 2-tier level edged enabled Gpio pads (Group Index 8)
  Offset(160),    G2L9, 32, // Offset(160),   GPE 2-tier level edged enabled Gpio pads (Group Index 9)
  Offset(164),    PML1, 16, // Offset(164),   PCIE LTR max snoop Latency 1
  Offset(166),    PML2, 16, // Offset(166),   PCIE LTR max snoop Latency 2
  Offset(168),    PML3, 16, // Offset(168),   PCIE LTR max snoop Latency 3
  Offset(170),    PML4, 16, // Offset(170),   PCIE LTR max snoop Latency 4
  Offset(172),    PML5, 16, // Offset(172),   PCIE LTR max snoop Latency 5
  Offset(174),    PML6, 16, // Offset(174),   PCIE LTR max snoop Latency 6
  Offset(176),    PML7, 16, // Offset(176),   PCIE LTR max snoop Latency 7
  Offset(178),    PML8, 16, // Offset(178),   PCIE LTR max snoop Latency 8
  Offset(180),    PML9, 16, // Offset(180),   PCIE LTR max snoop Latency 9
  Offset(182),    PMLA, 16, // Offset(182),   PCIE LTR max snoop Latency 10
  Offset(184),    PMLB, 16, // Offset(184),   PCIE LTR max snoop Latency 11
  Offset(186),    PMLC, 16, // Offset(186),   PCIE LTR max snoop Latency 12
  Offset(188),    PMLD, 16, // Offset(188),   PCIE LTR max snoop Latency 13
  Offset(190),    PMLE, 16, // Offset(190),   PCIE LTR max snoop Latency 14
  Offset(192),    PMLF, 16, // Offset(192),   PCIE LTR max snoop Latency 15
  Offset(194),    PMLG, 16, // Offset(194),   PCIE LTR max snoop Latency 16
  Offset(196),    PMLH, 16, // Offset(196),   PCIE LTR max snoop Latency 17
  Offset(198),    PMLI, 16, // Offset(198),   PCIE LTR max snoop Latency 18
  Offset(200),    PMLJ, 16, // Offset(200),   PCIE LTR max snoop Latency 19
  Offset(202),    PMLK, 16, // Offset(202),   PCIE LTR max snoop Latency 20
  Offset(204),    PMLL, 16, // Offset(204),   PCIE LTR max snoop Latency 21
  Offset(206),    PMLM, 16, // Offset(206),   PCIE LTR max snoop Latency 22
  Offset(208),    PMLN, 16, // Offset(208),   PCIE LTR max snoop Latency 23
  Offset(210),    PMLO, 16, // Offset(210),   PCIE LTR max snoop Latency 24
  Offset(212),    PNL1, 16, // Offset(212),   PCIE LTR max no snoop Latency 1
  Offset(214),    PNL2, 16, // Offset(214),   PCIE LTR max no snoop Latency 2
  Offset(216),    PNL3, 16, // Offset(216),   PCIE LTR max no snoop Latency 3
  Offset(218),    PNL4, 16, // Offset(218),   PCIE LTR max no snoop Latency 4
  Offset(220),    PNL5, 16, // Offset(220),   PCIE LTR max no snoop Latency 5
  Offset(222),    PNL6, 16, // Offset(222),   PCIE LTR max no snoop Latency 6
  Offset(224),    PNL7, 16, // Offset(224),   PCIE LTR max no snoop Latency 7
  Offset(226),    PNL8, 16, // Offset(226),   PCIE LTR max no snoop Latency 8
  Offset(228),    PNL9, 16, // Offset(228),   PCIE LTR max no snoop Latency 9
  Offset(230),    PNLA, 16, // Offset(230),   PCIE LTR max no snoop Latency 10
  Offset(232),    PNLB, 16, // Offset(232),   PCIE LTR max no snoop Latency 11
  Offset(234),    PNLC, 16, // Offset(234),   PCIE LTR max no snoop Latency 12
  Offset(236),    PNLD, 16, // Offset(236),   PCIE LTR max no snoop Latency 13
  Offset(238),    PNLE, 16, // Offset(238),   PCIE LTR max no snoop Latency 14
  Offset(240),    PNLF, 16, // Offset(240),   PCIE LTR max no snoop Latency 15
  Offset(242),    PNLG, 16, // Offset(242),   PCIE LTR max no snoop Latency 16
  Offset(244),    PNLH, 16, // Offset(244),   PCIE LTR max no snoop Latency 17
  Offset(246),    PNLI, 16, // Offset(246),   PCIE LTR max no snoop Latency 18
  Offset(248),    PNLJ, 16, // Offset(248),   PCIE LTR max no snoop Latency 19
  Offset(250),    PNLK, 16, // Offset(250),   PCIE LTR max no snoop Latency 20
  Offset(252),    PNLL, 16, // Offset(252),   PCIE LTR max no snoop Latency 21
  Offset(254),    PNLM, 16, // Offset(254),   PCIE LTR max no snoop Latency 22
  Offset(256),    PNLN, 16, // Offset(256),   PCIE LTR max no snoop Latency 23
  Offset(258),    PNLO, 16, // Offset(258),   PCIE LTR max no snoop Latency 24
  Offset(260),    U0C0, 32, // Offset(260),   SerialIo Hidden UART0 BAR 0
  Offset(264),    U1C0, 32, // Offset(264),   SerialIo Hidden UART1 BAR 0
  Offset(268),    XHPC, 8,  // Offset(268),   Number of HighSpeed ports implemented in XHCI controller
  Offset(269),    XRPC, 8,  // Offset(269),   Number of USBR ports implemented in XHCI controller
  Offset(270),    XSPC, 8,  // Offset(270),   Number of SuperSpeed ports implemented in XHCI controller
  Offset(271),    XSPA, 8,  // Offset(271),   Address of 1st SuperSpeed port
  Offset(272),    HPTB, 32, // Offset(272),   HPET base address
  Offset(276),    HPTE, 8,  // Offset(276),   HPET enable
  //SerialIo block
  Offset(277),    SMD0, 8,  // Offset(277),   SerialIo controller 0 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(278),    SMD1, 8,  // Offset(278),   SerialIo controller 1 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(279),    SMD2, 8,  // Offset(279),   SerialIo controller 2 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(280),    SMD3, 8,  // Offset(280),   SerialIo controller 3 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(281),    SMD4, 8,  // Offset(281),   SerialIo controller 4 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(282),    SMD5, 8,  // Offset(282),   SerialIo controller 5 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(283),    SMD6, 8,  // Offset(283),   SerialIo controller 6 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(284),    SMD7, 8,  // Offset(284),   SerialIo controller 7 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(285),    SMD8, 8,  // Offset(285),   SerialIo controller 8 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(286),    SMD9, 8,  // Offset(286),   SerialIo controller 9 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(287),    SMDA, 8,  // Offset(287),   SerialIo controller A mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(288),    SIR0, 8,  // Offset(288),   SerialIo controller 0 irq number
  Offset(289),    SIR1, 8,  // Offset(289),   SerialIo controller 1 irq number
  Offset(290),    SIR2, 8,  // Offset(290),   SerialIo controller 2 irq number
  Offset(291),    SIR3, 8,  // Offset(291),   SerialIo controller 3 irq number
  Offset(292),    SIR4, 8,  // Offset(292),   SerialIo controller 4 irq number
  Offset(293),    SIR5, 8,  // Offset(293),   SerialIo controller 5 irq number
  Offset(294),    SIR6, 8,  // Offset(294),   SerialIo controller 6 irq number
  Offset(295),    SIR7, 8,  // Offset(295),   SerialIo controller 7 irq number
  Offset(296),    SIR8, 8,  // Offset(296),   SerialIo controller 8 irq number
  Offset(297),    SIR9, 8,  // Offset(297),   SerialIo controller 9 irq number
  Offset(298),    SIRA, 8,  // Offset(298),   SerialIo controller A irq number
  Offset(299),    SB00, 64, // Offset(299),   SerialIo controller 0 BAR0
  Offset(307),    SB01, 64, // Offset(307),   SerialIo controller 1 BAR0
  Offset(315),    SB02, 64, // Offset(315),   SerialIo controller 2 BAR0
  Offset(323),    SB03, 64, // Offset(323),   SerialIo controller 3 BAR0
  Offset(331),    SB04, 64, // Offset(331),   SerialIo controller 4 BAR0
  Offset(339),    SB05, 64, // Offset(339),   SerialIo controller 5 BAR0
  Offset(347),    SB06, 64, // Offset(347),   SerialIo controller 6 BAR0
  Offset(355),    SB07, 64, // Offset(355),   SerialIo controller 7 BAR0
  Offset(363),    SB08, 64, // Offset(363),   SerialIo controller 8 BAR0
  Offset(371),    SB09, 64, // Offset(371),   SerialIo controller 9 BAR0
  Offset(379),    SB0A, 64, // Offset(379),   SerialIo controller A BAR0
  Offset(387),    SB10, 64, // Offset(387),   SerialIo controller 0 BAR1
  Offset(395),    SB11, 64, // Offset(395),   SerialIo controller 1 BAR1
  Offset(403),    SB12, 64, // Offset(403),   SerialIo controller 2 BAR1
  Offset(411),    SB13, 64, // Offset(411),   SerialIo controller 3 BAR1
  Offset(419),    SB14, 64, // Offset(419),   SerialIo controller 4 BAR1
  Offset(427),    SB15, 64, // Offset(427),   SerialIo controller 5 BAR1
  Offset(435),    SB16, 64, // Offset(435),   SerialIo controller 6 BAR1
  Offset(443),    SB17, 64, // Offset(443),   SerialIo controller 7 BAR1
  Offset(451),    SB18, 64, // Offset(451),   SerialIo controller 8 BAR1
  Offset(459),    SB19, 64, // Offset(459),   SerialIo controller 9 BAR1
  Offset(467),    SB1A, 64, // Offset(467),   SerialIo controller A BAR1
  //end of SerialIo block
  Offset(475),    GPEN, 8,  // Offset(475),   GPIO enabled
  Offset(476),    SGIR, 8,  // Offset(476),   GPIO IRQ
  Offset(477),    NIT1, 8,  // Offset(477),   RST PCIe Storage Cycle Router#1 Interface Type
  Offset(478),    NIT2, 8,  // Offset(478),   RST PCIe Storage Cycle Router#2 Interface Type
  Offset(479),    NIT3, 8,  // Offset(479),   RST PCIe Storage Cycle Router#3 Interface Type
  Offset(480),    NPM1, 8,  // Offset(480),   RST PCIe Storage Cycle Router#1 Power Management Capability Pointer
  Offset(481),    NPM2, 8,  // Offset(481),   RST PCIe Storage Cycle Router#2 Power Management Capability Pointer
  Offset(482),    NPM3, 8,  // Offset(482),   RST PCIe Storage Cycle Router#3 Power Management Capability Pointer
  Offset(483),    NPC1, 8,  // Offset(483),   RST PCIe Storage Cycle Router#1 PCIe Capabilities Pointer
  Offset(484),    NPC2, 8,  // Offset(484),   RST PCIe Storage Cycle Router#2 PCIe Capabilities Pointer
  Offset(485),    NPC3, 8,  // Offset(485),   RST PCIe Storage Cycle Router#3 PCIe Capabilities Pointer
  Offset(486),    NL11, 16, // Offset(486),   RST PCIe Storage Cycle Router#1 L1SS Capability Pointer
  Offset(488),    NL12, 16, // Offset(488),   RST PCIe Storage Cycle Router#2 L1SS Capability Pointer
  Offset(490),    NL13, 16, // Offset(490),   RST PCIe Storage Cycle Router#3 L1SS Capability Pointer
  Offset(492),    ND21, 8,  // Offset(492),   RST PCIe Storage Cycle Router#1 Endpoint L1SS Control Data2
  Offset(493),    ND22, 8,  // Offset(493),   RST PCIe Storage Cycle Router#2 Endpoint L1SS Control Data2
  Offset(494),    ND23, 8,  // Offset(494),   RST PCIe Storage Cycle Router#3 Endpoint L1SS Control Data2
  Offset(495),    ND11, 32, // Offset(495),   RST PCIe Storage Cycle Router#1 Endpoint L1SS Control Data1
  Offset(499),    ND12, 32, // Offset(499),   RST PCIe Storage Cycle Router#2 Endpoint L1SS Control Data1
  Offset(503),    ND13, 32, // Offset(503),   RST PCIe Storage Cycle Router#3 Endpoint L1SS Control Data1
  Offset(507),    NLR1, 16, // Offset(507),   RST PCIe Storage Cycle Router#1 LTR Capability Pointer
  Offset(509),    NLR2, 16, // Offset(509),   RST PCIe Storage Cycle Router#2 LTR Capability Pointer
  Offset(511),    NLR3, 16, // Offset(511),   RST PCIe Storage Cycle Router#3 LTR Capability Pointer
  Offset(513),    NLD1, 32, // Offset(513),   RST PCIe Storage Cycle Router#1 Endpoint LTR Data
  Offset(517),    NLD2, 32, // Offset(517),   RST PCIe Storage Cycle Router#2 Endpoint LTR Data
  Offset(521),    NLD3, 32, // Offset(521),   RST PCIe Storage Cycle Router#3 Endpoint LTR Data
  Offset(525),    NEA1, 16, // Offset(525),   RST PCIe Storage Cycle Router#1 Endpoint LCTL Data
  Offset(527),    NEA2, 16, // Offset(527),   RST PCIe Storage Cycle Router#2 Endpoint LCTL Data
  Offset(529),    NEA3, 16, // Offset(529),   RST PCIe Storage Cycle Router#3 Endpoint LCTL Data
  Offset(531),    NEB1, 16, // Offset(531),   RST PCIe Storage Cycle Router#1 Endpoint DCTL Data
  Offset(533),    NEB2, 16, // Offset(533),   RST PCIe Storage Cycle Router#2 Endpoint DCTL Data
  Offset(535),    NEB3, 16, // Offset(535),   RST PCIe Storage Cycle Router#3 Endpoint DCTL Data
  Offset(537),    NEC1, 16, // Offset(537),   RST PCIe Storage Cycle Router#1 Endpoint DCTL2 Data
  Offset(539),    NEC2, 16, // Offset(539),   RST PCIe Storage Cycle Router#2 Endpoint DCTL2 Data
  Offset(541),    NEC3, 16, // Offset(541),   RST PCIe Storage Cycle Router#3 Endpoint DCTL2 Data
  Offset(543),    NRA1, 16, // Offset(543),   RST PCIe Storage Cycle Router#1 RootPort DCTL2 Data
  Offset(545),    NRA2, 16, // Offset(545),   RST PCIe Storage Cycle Router#2 RootPort DCTL2 Data
  Offset(547),    NRA3, 16, // Offset(547),   RST PCIe Storage Cycle Router#3 RootPort DCTL2 Data
  Offset(549),    NMB1, 32, // Offset(549),   RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X Table BAR
  Offset(553),    NMB2, 32, // Offset(553),   RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X Table BAR
  Offset(557),    NMB3, 32, // Offset(557),   RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X Table BAR
  Offset(561),    NMV1, 32, // Offset(561),   RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X Table BAR value
  Offset(565),    NMV2, 32, // Offset(565),   RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X Table BAR value
  Offset(569),    NMV3, 32, // Offset(569),   RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X Table BAR value
  Offset(573),    NPB1, 32, // Offset(573),   RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X PBA BAR
  Offset(577),    NPB2, 32, // Offset(577),   RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X PBA BAR
  Offset(581),    NPB3, 32, // Offset(581),   RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X PBA BAR
  Offset(585),    NPV1, 32, // Offset(585),   RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X PBA BAR value
  Offset(589),    NPV2, 32, // Offset(589),   RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X PBA BAR value
  Offset(593),    NPV3, 32, // Offset(593),   RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X PBA BAR value
  Offset(597),    NRP1, 32, // Offset(597),   RST PCIe Storage Cycle Router#1 Root Port number
  Offset(601),    NRP2, 32, // Offset(601),   RST PCIe Storage Cycle Router#2 Root Port number
  Offset(605),    NRP3, 32, // Offset(605),   RST PCIe Storage Cycle Router#3 Root Port number
  Offset(609),        , 8,  // Offset(609),   Flag indicating Exit Boot Service, to inform SMM
  Offset(610),    SXRB, 32, // Offset(610),   Sx handler reserved MMIO base
  Offset(614),    SXRS, 32, // Offset(614),   Sx handler reserved MMIO size
  Offset(618),    CIOE, 8,  // Offset(618),   Cio2 Device Enabled as ACPI device
  Offset(619),    CIOI, 8,  // Offset(619),   Cio2 Interrupt Number
  Offset(620),    TAEN, 8,  // Offset(620),   Thermal Device Acpi mode enabled
  Offset(621),    TIRQ, 8,  // Offset(621),   Thermal Device IRQ number
  Offset(622),    XWMB, 32, // Offset(622),   XHCI memory base address
  Offset(626),    EMH4, 8,  // Offset(626),   eMMC HS400 mode enabled
  Offset(627),    EMDS, 8,  // Offset(627),   eMMC Driver Strength
  Offset(628),    CSKU, 8,  // Offset(628),   CPU SKU
  Offset(629),    ITA0, 16, // Offset(629),
  Offset(631),    ITA1, 16, // Offset(631),
  Offset(633),    ITA2, 16, // Offset(633),
  Offset(635),    ITA3, 16, // Offset(635),
  Offset(637),    ITS0, 8,  // Offset(637),
  Offset(638),    ITS1, 8,  // Offset(638),
  Offset(639),    ITS2, 8,  // Offset(639),
  Offset(640),    ITS3, 8,  // Offset(640),
  Offset(641),    PMBS, 16, // Offset(641),   ACPI IO BASE address
  Offset(643),    PWRM, 32, // Offset(643),   PWRM MEM BASE address
  }
