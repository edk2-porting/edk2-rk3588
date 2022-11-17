/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  //
  // Define PCH NVS Area operatino region.
  //



  OperationRegion(PNVA,SystemMemory,PNVB,PNVL)
  Field(PNVA,AnyAcc,Lock,Preserve)
  {
  Offset(0),      RCRV, 32, // Offset(0),     RC Revision
  Offset(4),      PCHS, 16, // Offset(4),     PCH Series
  Offset(6),      PCHG, 16, // Offset(6),     PCH Generation
  Offset(8),      RPA1, 32, // Offset(8),     Root Port address 1
  Offset(12),     RPA2, 32, // Offset(12),    Root Port address 2
  Offset(16),     RPA3, 32, // Offset(16),    Root Port address 3
  Offset(20),     RPA4, 32, // Offset(20),    Root Port address 4
  Offset(24),     RPA5, 32, // Offset(24),    Root Port address 5
  Offset(28),     RPA6, 32, // Offset(28),    Root Port address 6
  Offset(32),     RPA7, 32, // Offset(32),    Root Port address 7
  Offset(36),     RPA8, 32, // Offset(36),    Root Port address 8
  Offset(40),     RPA9, 32, // Offset(40),    Root Port address 9
  Offset(44),     RPAA, 32, // Offset(44),    Root Port address 10
  Offset(48),     RPAB, 32, // Offset(48),    Root Port address 11
  Offset(52),     RPAC, 32, // Offset(52),    Root Port address 12
  Offset(56),     RPAD, 32, // Offset(56),    Root Port address 13
  Offset(60),     RPAE, 32, // Offset(60),    Root Port address 14
  Offset(64),     RPAF, 32, // Offset(64),    Root Port address 15
  Offset(68),     RPAG, 32, // Offset(68),    Root Port address 16
  Offset(72),     RPAH, 32, // Offset(72),    Root Port address 17
  Offset(76),     RPAI, 32, // Offset(76),    Root Port address 18
  Offset(80),     RPAJ, 32, // Offset(80),    Root Port address 19
  Offset(84),     RPAK, 32, // Offset(84),    Root Port address 20
  Offset(88),     NHLA, 64, // Offset(88),    HD-Audio NHLT ACPI address
  Offset(96),     NHLL, 32, // Offset(96),    HD-Audio NHLT ACPI length
  Offset(100),    ADFM, 32, // Offset(100),   HD-Audio DSP Feature Mask
  Offset(104),    SBRG, 32, // Offset(104),   SBREG_BAR
  Offset(108),    GPEM, 32, // Offset(108),   GPP_X to GPE_DWX mapping
  Offset(112),    G2L0, 32, // Offset(112),   GPE 2-tier level edged enabled Gpio pads (Group Index 0)
  Offset(116),    G2L1, 32, // Offset(116),   GPE 2-tier level edged enabled Gpio pads (Group Index 1)
  Offset(120),    G2L2, 32, // Offset(120),   GPE 2-tier level edged enabled Gpio pads (Group Index 2)
  Offset(124),    G2L3, 32, // Offset(124),   GPE 2-tier level edged enabled Gpio pads (Group Index 3)
  Offset(128),    G2L4, 32, // Offset(128),   GPE 2-tier level edged enabled Gpio pads (Group Index 4)
  Offset(132),    G2L5, 32, // Offset(132),   GPE 2-tier level edged enabled Gpio pads (Group Index 5)
  Offset(136),    G2L6, 32, // Offset(136),   GPE 2-tier level edged enabled Gpio pads (Group Index 6)
  Offset(140),    G2L7, 32, // Offset(140),   GPE 2-tier level edged enabled Gpio pads (Group Index 7)
  Offset(144),    G2L8, 32, // Offset(144),   GPE 2-tier level edged enabled Gpio pads (Group Index 8)
  Offset(148),    G2L9, 32, // Offset(148),   GPE 2-tier level edged enabled Gpio pads (Group Index 9)
  Offset(152),    G2LA, 32, // Offset(152),   GPE 2-tier level edged enabled Gpio pads (Group Index 10)
  Offset(156),    G2LB, 32, // Offset(156),   GPE 2-tier level edged enabled Gpio pads (Group Index 11)
  Offset(160),    G2LC, 32, // Offset(160),   GPE 2-tier level edged enabled Gpio pads (Groip Index 12)

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
  Offset(204),    PNL1, 16, // Offset(204),   PCIE LTR max no snoop Latency 1
  Offset(206),    PNL2, 16, // Offset(206),   PCIE LTR max no snoop Latency 2
  Offset(208),    PNL3, 16, // Offset(208),   PCIE LTR max no snoop Latency 3
  Offset(210),    PNL4, 16, // Offset(210),   PCIE LTR max no snoop Latency 4
  Offset(212),    PNL5, 16, // Offset(212),   PCIE LTR max no snoop Latency 5
  Offset(214),    PNL6, 16, // Offset(214),   PCIE LTR max no snoop Latency 6
  Offset(216),    PNL7, 16, // Offset(216),   PCIE LTR max no snoop Latency 7
  Offset(218),    PNL8, 16, // Offset(218),   PCIE LTR max no snoop Latency 8
  Offset(220),    PNL9, 16, // Offset(220),   PCIE LTR max no snoop Latency 9
  Offset(222),    PNLA, 16, // Offset(222),   PCIE LTR max no snoop Latency 10
  Offset(224),    PNLB, 16, // Offset(224),   PCIE LTR max no snoop Latency 11
  Offset(226),    PNLC, 16, // Offset(226),   PCIE LTR max no snoop Latency 12
  Offset(228),    PNLD, 16, // Offset(228),   PCIE LTR max no snoop Latency 13
  Offset(230),    PNLE, 16, // Offset(230),   PCIE LTR max no snoop Latency 14
  Offset(232),    PNLF, 16, // Offset(232),   PCIE LTR max no snoop Latency 15
  Offset(234),    PNLG, 16, // Offset(234),   PCIE LTR max no snoop Latency 16
  Offset(236),    PNLH, 16, // Offset(236),   PCIE LTR max no snoop Latency 17
  Offset(238),    PNLI, 16, // Offset(238),   PCIE LTR max no snoop Latency 18
  Offset(240),    PNLJ, 16, // Offset(240),   PCIE LTR max no snoop Latency 19
  Offset(242),    PNLK, 16, // Offset(242),   PCIE LTR max no snoop Latency 20
  Offset(244),    U0C0, 32, // Offset(244),   SerialIo Hidden UART0 BAR 0
  Offset(248),    U1C0, 32, // Offset(248),   SerialIo Hidden UART1 BAR 0
  Offset(252),    ADPM, 32, // Offset(252),   HD-Audio DSP Post-Processing Module Mask
  Offset(256),    XHPC, 8,  // Offset(256),   Number of HighSpeed ports implemented in XHCI controller
  Offset(257),    XRPC, 8,  // Offset(257),   Number of USBR ports implemented in XHCI controller
  Offset(258),    XSPC, 8,  // Offset(258),   Number of SuperSpeed ports implemented in XHCI controller
  Offset(259),    XSPA, 8,  // Offset(259),   Address of 1st SuperSpeed port
  Offset(260),    HPTB, 32, // Offset(260),   HPET base address
  Offset(264),    HPTE, 8,  // Offset(264),   HPET enable
  //110-bytes large SerialIo block
  Offset(265),    SMD0, 8,  // Offset(265),   SerialIo controller 0 (sdma) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(266),    SMD1, 8,  // Offset(266),   SerialIo controller 1 (i2c0) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(267),    SMD2, 8,  // Offset(267),   SerialIo controller 2 (i2c1) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(268),    SMD3, 8,  // Offset(268),   SerialIo controller 3 (spi0) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(269),    SMD4, 8,  // Offset(269),   SerialIo controller 4 (spi1) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(270),    SMD5, 8,  // Offset(270),   SerialIo controller 5 (ua00) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(271),    SMD6, 8,  // Offset(271),   SerialIo controller 6 (ua01) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(272),    SMD7, 8,  // Offset(272),   SerialIo controller 7 (shdc) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(273),    SMD8, 8,  // Offset(273),   SerialIo controller 8 (shdc) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(274),    SMD9, 8,  // Offset(274),   SerialIo controller 9 (shdc) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(275),    SMDA, 8,  // Offset(275),   SerialIo controller A (shdc) mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  Offset(276),    SIR0, 8,  // Offset(276),   SerialIo controller 0 (sdma) irq number
  Offset(277),    SIR1, 8,  // Offset(277),   SerialIo controller 1 (i2c0) irq number
  Offset(278),    SIR2, 8,  // Offset(278),   SerialIo controller 2 (i2c1) irq number
  Offset(279),    SIR3, 8,  // Offset(279),   SerialIo controller 3 (spi0) irq number
  Offset(280),    SIR4, 8,  // Offset(280),   SerialIo controller 4 (spi1) irq number
  Offset(281),    SIR5, 8,  // Offset(281),   SerialIo controller 5 (ua00) irq number
  Offset(282),    SIR6, 8,  // Offset(282),   SerialIo controller 6 (ua01) irq number
  Offset(283),    SIR7, 8,  // Offset(283),   SerialIo controller 7 (shdc) irq number
  Offset(284),    SIR8, 8,  // Offset(284),   SerialIo controller 8 (shdc) irq number
  Offset(285),    SIR9, 8,  // Offset(285),   SerialIo controller 9 (shdc) irq number
  Offset(286),    SIRA, 8,  // Offset(286),   SerialIo controller A (shdc) irq number
  Offset(287),    SB00, 32, // Offset(287),   SerialIo controller 0 (sdma) BAR0
  Offset(291),    SB01, 32, // Offset(291),   SerialIo controller 1 (i2c0) BAR0
  Offset(295),    SB02, 32, // Offset(295),   SerialIo controller 2 (i2c1) BAR0
  Offset(299),    SB03, 32, // Offset(299),   SerialIo controller 3 (spi0) BAR0
  Offset(303),    SB04, 32, // Offset(303),   SerialIo controller 4 (spi1) BAR0
  Offset(307),    SB05, 32, // Offset(307),   SerialIo controller 5 (ua00) BAR0
  Offset(311),    SB06, 32, // Offset(311),   SerialIo controller 6 (ua01) BAR0
  Offset(315),    SB07, 32, // Offset(315),   SerialIo controller 7 (shdc) BAR0
  Offset(319),    SB08, 32, // Offset(319),   SerialIo controller 8 (shdc) BAR0
  Offset(323),    SB09, 32, // Offset(323),   SerialIo controller 9 (shdc) BAR0
  Offset(327),    SB0A, 32, // Offset(327),   SerialIo controller A (shdc) BAR0
  Offset(331),    SB10, 32, // Offset(331),   SerialIo controller 0 (sdma) BAR1
  Offset(335),    SB11, 32, // Offset(335),   SerialIo controller 1 (i2c0) BAR1
  Offset(339),    SB12, 32, // Offset(339),   SerialIo controller 2 (i2c1) BAR1
  Offset(343),    SB13, 32, // Offset(343),   SerialIo controller 3 (spi0) BAR1
  Offset(347),    SB14, 32, // Offset(347),   SerialIo controller 4 (spi1) BAR1
  Offset(351),    SB15, 32, // Offset(351),   SerialIo controller 5 (ua00) BAR1
  Offset(355),    SB16, 32, // Offset(355),   SerialIo controller 6 (ua01) BAR1
  Offset(359),    SB17, 32, // Offset(359),   SerialIo controller 7 (shdc) BAR1
  Offset(363),    SB18, 32, // Offset(363),   SerialIo controller 8 (shdc) BAR1
  Offset(367),    SB19, 32, // Offset(367),   SerialIo controller 9 (shdc) BAR1
  Offset(371),    SB1A, 32, // Offset(371),   SerialIo controller A (shdc) BAR1
  //end of SerialIo block
  Offset(375),    GPEN, 8,  // Offset(375),   GPIO enabled
  Offset(376),    SGIR, 8,  // Offset(376),   GPIO IRQ
  Offset(377),    NIT1, 8,  // Offset(377),   RST PCIe Storage Cycle Router#1 Interface Type
  Offset(378),    NIT2, 8,  // Offset(378),   RST PCIe Storage Cycle Router#2 Interface Type
  Offset(379),    NIT3, 8,  // Offset(379),   RST PCIe Storage Cycle Router#3 Interface Type
  Offset(380),    NPM1, 8,  // Offset(380),   RST PCIe Storage Cycle Router#1 Power Management Capability Pointer
  Offset(381),    NPM2, 8,  // Offset(381),   RST PCIe Storage Cycle Router#2 Power Management Capability Pointer
  Offset(382),    NPM3, 8,  // Offset(382),   RST PCIe Storage Cycle Router#3 Power Management Capability Pointer
  Offset(383),    NPC1, 8,  // Offset(383),   RST PCIe Storage Cycle Router#1 PCIe Capabilities Pointer
  Offset(384),    NPC2, 8,  // Offset(384),   RST PCIe Storage Cycle Router#2 PCIe Capabilities Pointer
  Offset(385),    NPC3, 8,  // Offset(385),   RST PCIe Storage Cycle Router#3 PCIe Capabilities Pointer
  Offset(386),    NL11, 16, // Offset(386),   RST PCIe Storage Cycle Router#1 L1SS Capability Pointer
  Offset(388),    NL12, 16, // Offset(388),   RST PCIe Storage Cycle Router#2 L1SS Capability Pointer
  Offset(390),    NL13, 16, // Offset(390),   RST PCIe Storage Cycle Router#3 L1SS Capability Pointer
  Offset(392),    ND21, 8,  // Offset(392),   RST PCIe Storage Cycle Router#1 Endpoint L1SS Control Data2
  Offset(393),    ND22, 8,  // Offset(393),   RST PCIe Storage Cycle Router#2 Endpoint L1SS Control Data2
  Offset(394),    ND23, 8,  // Offset(394),   RST PCIe Storage Cycle Router#3 Endpoint L1SS Control Data2
  Offset(395),    ND11, 32, // Offset(395),   RST PCIe Storage Cycle Router#1 Endpoint L1SS Control Data1
  Offset(399),    ND12, 32, // Offset(399),   RST PCIe Storage Cycle Router#2 Endpoint L1SS Control Data1
  Offset(403),    ND13, 32, // Offset(403),   RST PCIe Storage Cycle Router#3 Endpoint L1SS Control Data1
  Offset(407),    NLR1, 16, // Offset(407),   RST PCIe Storage Cycle Router#1 LTR Capability Pointer
  Offset(409),    NLR2, 16, // Offset(409),   RST PCIe Storage Cycle Router#2 LTR Capability Pointer
  Offset(411),    NLR3, 16, // Offset(411),   RST PCIe Storage Cycle Router#3 LTR Capability Pointer
  Offset(413),    NLD1, 32, // Offset(413),   RST PCIe Storage Cycle Router#1 Endpoint LTR Data
  Offset(417),    NLD2, 32, // Offset(417),   RST PCIe Storage Cycle Router#2 Endpoint LTR Data
  Offset(421),    NLD3, 32, // Offset(421),   RST PCIe Storage Cycle Router#3 Endpoint LTR Data
  Offset(425),    NEA1, 16, // Offset(425),   RST PCIe Storage Cycle Router#1 Endpoint LCTL Data
  Offset(427),    NEA2, 16, // Offset(427),   RST PCIe Storage Cycle Router#2 Endpoint LCTL Data
  Offset(429),    NEA3, 16, // Offset(429),   RST PCIe Storage Cycle Router#3 Endpoint LCTL Data
  Offset(431),    NEB1, 16, // Offset(431),   RST PCIe Storage Cycle Router#1 Endpoint DCTL Data
  Offset(433),    NEB2, 16, // Offset(433),   RST PCIe Storage Cycle Router#2 Endpoint DCTL Data
  Offset(435),    NEB3, 16, // Offset(435),   RST PCIe Storage Cycle Router#3 Endpoint DCTL Data
  Offset(437),    NEC1, 16, // Offset(437),   RST PCIe Storage Cycle Router#1 Endpoint DCTL2 Data
  Offset(439),    NEC2, 16, // Offset(439),   RST PCIe Storage Cycle Router#2 Endpoint DCTL2 Data
  Offset(441),    NEC3, 16, // Offset(441),   RST PCIe Storage Cycle Router#3 Endpoint DCTL2 Data
  Offset(443),    NRA1, 16, // Offset(443),   RST PCIe Storage Cycle Router#1 RootPort DCTL2 Data
  Offset(445),    NRA2, 16, // Offset(445),   RST PCIe Storage Cycle Router#2 RootPort DCTL2 Data
  Offset(447),    NRA3, 16, // Offset(447),   RST PCIe Storage Cycle Router#3 RootPort DCTL2 Data
  Offset(449),    NMB1, 32, // Offset(449),   RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X Table BAR
  Offset(453),    NMB2, 32, // Offset(453),   RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X Table BAR
  Offset(457),    NMB3, 32, // Offset(457),   RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X Table BAR
  Offset(461),    NMV1, 32, // Offset(461),   RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X Table BAR value
  Offset(465),    NMV2, 32, // Offset(465),   RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X Table BAR value
  Offset(469),    NMV3, 32, // Offset(469),   RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X Table BAR value
  Offset(473),    NPB1, 32, // Offset(473),   RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X PBA BAR
  Offset(477),    NPB2, 32, // Offset(477),   RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X PBA BAR
  Offset(481),    NPB3, 32, // Offset(481),   RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X PBA BAR
  Offset(485),    NPV1, 32, // Offset(485),   RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X PBA BAR value
  Offset(489),    NPV2, 32, // Offset(489),   RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X PBA BAR value
  Offset(493),    NPV3, 32, // Offset(493),   RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X PBA BAR value
  Offset(497),        , 8,  // Offset(497),   Flag indicating Exit Boot Service, to inform SMM
  Offset(498),    SXRB, 32, // Offset(498),   Sx handler reserved MMIO base
  Offset(502),    SXRS, 32, // Offset(502),   Sx handler reserved MMIO size
  Offset(506),    CIOE, 8,  // Offset(506),   Cio2 Device Enabled as ACPI device
  Offset(507),    CIOI, 8,  // Offset(507),   Cio2 Interrupt Number
  Offset(508),    TAEN, 8,  // Offset(508),   Thermal Device Acpi mode enabled
  Offset(509),    TIRQ, 8,  // Offset(509),   Thermal Device IRQ number
  Offset(510),    XWMB, 32, // Offset(510),   XHCI memory base address
  Offset(514),    EMH4, 8,  // Offset(514),   eMMC HS400 mode enabled
  Offset(515),    CSKU, 8,  // Offset(515),   CPU SKU
  Offset(516),    ITA0, 16, // Offset(516),
  Offset(518),    ITA1, 16, // Offset(518),
  Offset(520),    ITA2, 16, // Offset(520),
  Offset(522),    ITA3, 16, // Offset(522),
  Offset(524),    ITS0, 8,  // Offset(524),
  Offset(525),    ITS1, 8,  // Offset(525),
  Offset(526),    ITS2, 8,  // Offset(526),
  Offset(527),    ITS3, 8,  // Offset(527),
  Offset(528),    LTR1, 8,  // Offset(528),   Latency Tolerance Reporting Enable
  Offset(529),    LTR2, 8,  // Offset(529),   Latency Tolerance Reporting Enable
  Offset(530),    LTR3, 8,  // Offset(530),   Latency Tolerance Reporting Enable
  Offset(531),    LTR4, 8,  // Offset(531),   Latency Tolerance Reporting Enable
  Offset(532),    LTR5, 8,  // Offset(532),   Latency Tolerance Reporting Enable
  Offset(533),    LTR6, 8,  // Offset(533),   Latency Tolerance Reporting Enable
  Offset(534),    LTR7, 8,  // Offset(534),   Latency Tolerance Reporting Enable
  Offset(535),    LTR8, 8,  // Offset(535),   Latency Tolerance Reporting Enable
  Offset(536),    LTR9, 8,  // Offset(536),   Latency Tolerance Reporting Enable
  Offset(537),    LTRA, 8,  // Offset(537),   Latency Tolerance Reporting Enable
  Offset(538),    LTRB, 8,  // Offset(538),   Latency Tolerance Reporting Enable
  Offset(539),    LTRC, 8,  // Offset(539),   Latency Tolerance Reporting Enable
  Offset(540),    LTRD, 8,  // Offset(540),   Latency Tolerance Reporting Enable
  Offset(541),    LTRE, 8,  // Offset(541),   Latency Tolerance Reporting Enable
  Offset(542),    LTRF, 8,  // Offset(542),   Latency Tolerance Reporting Enable
  Offset(543),    LTRG, 8,  // Offset(543),   Latency Tolerance Reporting Enable
  Offset(544),    LTRH, 8,  // Offset(544),   Latency Tolerance Reporting Enable
  Offset(545),    LTRI, 8,  // Offset(545),   Latency Tolerance Reporting Enable
  Offset(546),    LTRJ, 8,  // Offset(546),   Latency Tolerance Reporting Enable
  Offset(547),    LTRK, 8,  // Offset(547),   Latency Tolerance Reporting Enable
  Offset(548),    OBF1, 8,  // Offset(548),   Optimized Buffer Flush and Fill
  Offset(549),    OBF2, 8,  // Offset(549),   Optimized Buffer Flush and Fill
  Offset(550),    OBF3, 8,  // Offset(550),   Optimized Buffer Flush and Fill
  Offset(551),    OBF4, 8,  // Offset(551),   Optimized Buffer Flush and Fill
  Offset(552),    OBF5, 8,  // Offset(552),   Optimized Buffer Flush and Fill
  Offset(553),    OBF6, 8,  // Offset(553),   Optimized Buffer Flush and Fill
  Offset(554),    OBF7, 8,  // Offset(554),   Optimized Buffer Flush and Fill
  Offset(555),    OBF8, 8,  // Offset(555),   Optimized Buffer Flush and Fill
  Offset(556),    OBF9, 8,  // Offset(556),   Optimized Buffer Flush and Fill
  Offset(557),    OBFA, 8,  // Offset(557),   Optimized Buffer Flush and Fill
  Offset(558),    OBFB, 8,  // Offset(558),   Optimized Buffer Flush and Fill
  Offset(559),    OBFC, 8,  // Offset(559),   Optimized Buffer Flush and Fill
  Offset(560),    OBFD, 8,  // Offset(560),   Optimized Buffer Flush and Fill
  Offset(561),    OBFE, 8,  // Offset(561),   Optimized Buffer Flush and Fill
  Offset(562),    OBFF, 8,  // Offset(562),   Optimized Buffer Flush and Fill
  Offset(563),    OBFG, 8,  // Offset(563),   Optimized Buffer Flush and Fill
  Offset(564),    OBFH, 8,  // Offset(564),   Optimized Buffer Flush and Fill
  Offset(565),    OBFI, 8,  // Offset(565),   Optimized Buffer Flush and Fill
  Offset(566),    OBFJ, 8,  // Offset(566),   Optimized Buffer Flush and Fill
  Offset(567),    OBFK, 8,  // Offset(567),   Optimized Buffer Flush and Fill
  Offset(568),    ECR1, 8,  // Offset(568),   External Change Request
  Offset(569),    AG1L, 64, // Offset(569),   HDA PP module custom GUID 1 - first 64bit  [0-63]
  Offset(577),    AG1H, 64, // Offset(577),   HDA PP module custom GUID 1 - second 64bit [64-127]
  Offset(585),    AG2L, 64, // Offset(585),   HDA PP module custom GUID 2 - first 64bit  [0-63]
  Offset(593),    AG2H, 64, // Offset(593),   HDA PP module custom GUID 2 - second 64bit [64-127]
  Offset(601),    AG3L, 64, // Offset(601),   HDA PP module custom GUID 3 - first 64bit  [0-63]
  Offset(609),    AG3H, 64, // Offset(609),   HDA PP module custom GUID 3 - second 64bit [64-127]
  Offset(617),    MCFG, 32  // Offset(617),   PcieMmCfgBaseAddress
  }
