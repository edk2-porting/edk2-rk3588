/*
 * Intel ACPI Component Architecture
 * iASL Compiler/Disassembler version 20151124-64
 * Copyright (c) 2015 - 2016 Intel Corporation
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 * Template for [IORT] ACPI Table (static data table)
 * Format: [ByteLength]  FieldName : HexFieldValue
 */
[0004]                          Signature : "IORT"    [IO Remapping Table]
[0004]                       Table Length : 000002e4
[0001]                           Revision : 00
[0001]                           Checksum : BC
[0006]                             Oem ID : "HISI  "
[0008]                       Oem Table ID : "HIP07   "
[0004]                       Oem Revision : 00000000
[0004]                    Asl Compiler ID : "INTL"
[0004]              Asl Compiler Revision : 20151124

[0004]                         Node Count : 00000008
[0004]                        Node Offset : 00000034
[0004]                           Reserved : 00000000
[0004]                   Optional Padding : 00 00 00 00

/* ITS 0, for peri a */
[0001]                               Type : 00
[0002]                             Length : 0018
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000000
[0004]                     Mapping Offset : 00000000

[0004]                           ItsCount : 00000001
[0004]                        Identifiers : 00000000
//4c
/* ITS 1, for peri b */
[0001]                               Type : 00
[0002]                             Length : 0018
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000000
[0004]                     Mapping Offset : 00000000

[0004]                           ItsCount : 00000001
[0004]                        Identifiers : 00000001
//64
/* ITS 2, for dsa a */
[0001]                               Type : 00
[0002]                             Length : 0018
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000000
[0004]                     Mapping Offset : 00000000

[0004]                           ItsCount : 00000001
[0004]                        Identifiers : 00000002
//7c
/* ITS 3, for dsa b */
[0001]                               Type : 00
[0002]                             Length : 0018
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000000
[0004]                     Mapping Offset : 00000000

[0004]                           ItsCount : 00000001
[0004]                        Identifiers : 00000003
//94
/*Sec CPU ITS 0, for peri a */
[0001]                               Type : 00
[0002]                             Length : 0018
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000000
[0004]                     Mapping Offset : 00000000

[0004]                           ItsCount : 00000001
[0004]                        Identifiers : 00000004
//ac
/* SEC CPU ITS 1, for peri b */
[0001]                               Type : 00
[0002]                             Length : 0018
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000000
[0004]                     Mapping Offset : 00000000

[0004]                           ItsCount : 00000001
[0004]                        Identifiers : 00000005
//c4
/* SEC CPU ITS 2, for dsa a */
[0001]                               Type : 00
[0002]                             Length : 0018
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000000
[0004]                     Mapping Offset : 00000000

[0004]                           ItsCount : 00000001
[0004]                        Identifiers : 00000006
//dc
/* SEC CPU ITS 3, for dsa b */
[0001]                               Type : 00
[0002]                             Length : 0018
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000000
[0004]                     Mapping Offset : 00000000

[0004]                           ItsCount : 00000001
[0004]                        Identifiers : 00000007



/* mbi-gen peri b, named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                                Coherency : 0
                         Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI0"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 000120c7 //device id
[0004]                   Output Reference : 0000004C
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen1 dsa a, named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                               Transient : 0
                          Write Allocate : 0
                           Read Allocate : 0
                                Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                               Coherency : 0
                        Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI1"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00040800    //device id
[0004]                   Output Reference : 00000064
[0004]              Flags (decoded below) : 00000001
                          Single Mapping : 1

/* mbi-gen mbi7 - RoCE named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                                Coherency : 0
                         Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI9"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00040b1e
[0004]                   Output Reference : 00000064
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen dsa a - usb named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                               Transient : 0
                          Write Allocate : 0
                           Read Allocate : 0
                                Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                               Coherency : 0
                        Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI5"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00040080    //device id
[0004]                   Output Reference : 00000064
[0004]              Flags (decoded below) : 00000001
                          Single Mapping : 1

/* mbi-gen1 dsa a, named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                               Transient : 0
                          Write Allocate : 0
                           Read Allocate : 0
                                Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                               Coherency : 0
                        Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI2"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00040900    //device id
[0004]                   Output Reference : 00000064
[0004]              Flags (decoded below) : 00000001
                          Single Mapping : 1

/* mbi-gen1 pcie, named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                               Transient : 0
                          Write Allocate : 0
                           Read Allocate : 0
                                Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                               Coherency : 0
                        Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI3"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00040000    //device id
[0004]                   Output Reference : 00000064
[0004]              Flags (decoded below) : 00000001
                          Single Mapping : 1

/* mbi-gen1 pcie, named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                               Transient : 0
                          Write Allocate : 0
                           Read Allocate : 0
                                Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                               Coherency : 0
                        Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI4"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00040040    //device id
[0004]                   Output Reference : 00000064
[0004]              Flags (decoded below) : 00000001
                          Single Mapping : 1

/* mbi-gen1 alg a, i2c 0 named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                               Transient : 0
                          Write Allocate : 0
                           Read Allocate : 0
                                Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                               Coherency : 0
                        Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI6"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00040B0E    //device id
[0004]                   Output Reference : 00000064
[0004]              Flags (decoded below) : 00000001
                          Single Mapping : 1

/* mbi-gen1 alg a, i2c 2 named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                               Transient : 0
                          Write Allocate : 0
                           Read Allocate : 0
                                Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                               Coherency : 0
                        Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI7"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00040B10    //device id
[0004]                   Output Reference : 00000064
[0004]              Flags (decoded below) : 00000001
                          Single Mapping : 1

/*1P NA PCIe2 */
[0001]                               Type : 02
[0002]                             Length : 0038
[0001]                           Revision : 01
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000024

[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000001
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 01
                                Coherency : 1
                         Device Attribute : 0
[0004]                      ATS Attribute : 00000000
[0004]                 PCI Segment Number : 00000002
                        Memory Size Limit : 00
                                 Reserved : 00000000

[0004]                         Input base : 0000f800
[0004]                           ID Count : 00000800
[0004]                        Output Base : 0000f800
[0004]                   Output Reference : 00000064
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0
/* 1P NB PCIe0 */
[0001]                               Type : 02
[0002]                             Length : 0038
[0001]                           Revision : 01
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000024

[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000001
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 01
                                Coherency : 1
                         Device Attribute : 0
[0004]                      ATS Attribute : 00000000
[0004]                 PCI Segment Number : 00000004
                        Memory Size Limit : 00
                                 Reserved : 00000000

[0004]                         Input base : 00008800
[0004]                           ID Count : 00000800
[0004]                        Output Base : 00008800
[0004]                   Output Reference : 0000007c
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* 1P NB PCIe1 */
[0001]                               Type : 02
[0002]                             Length : 0038
[0001]                           Revision : 01
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000024

[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000001
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 01
                                Coherency : 1
                         Device Attribute : 0
[0004]                      ATS Attribute : 00000000
[0004]                 PCI Segment Number : 00000005
                        Memory Size Limit : 00
                                 Reserved : 00000000

[0004]                         Input base : 00007800
[0004]                           ID Count : 00000800
[0004]                        Output Base : 00007800
[0004]                   Output Reference : 0000007c
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* 1P NB PCIe2 */
[0001]                               Type : 02
[0002]                             Length : 0038
[0001]                           Revision : 01
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000024

[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000001
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 01
                                Coherency : 1
                         Device Attribute : 0
[0004]                      ATS Attribute : 00000000
[0004]                 PCI Segment Number : 00000006
                        Memory Size Limit : 00
                                 Reserved : 00000000

[0004]                         Input base : 0000c000
[0004]                           ID Count : 00000800
[0004]                        Output Base : 0000c000
[0004]                   Output Reference : 0000007c
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0
/* 1P NB PCIe3 */
[0001]                               Type : 02
[0002]                             Length : 0038
[0001]                           Revision : 01
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000024

[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000001
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 01
                                Coherency : 1
                         Device Attribute : 0
[0004]                      ATS Attribute : 00000000
[0004]                 PCI Segment Number : 00000007
                        Memory Size Limit : 00
                                 Reserved : 00000000

[0004]                         Input base : 00009000
[0004]                           ID Count : 00000800
[0004]                        Output Base : 00009000
[0004]                   Output Reference : 0000007c
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0
/* 2P NA PCIe2*/
[0001]                               Type : 02
[0002]                             Length : 0038
[0001]                           Revision : 01
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000024

[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000001
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 01
                                Coherency : 1
                         Device Attribute : 0
[0004]                      ATS Attribute : 00000000
[0004]                 PCI Segment Number : 0000000a
                        Memory Size Limit : 00
                                 Reserved : 00000000

[0004]                         Input base : 00001000
[0004]                           ID Count : 00001000
[0004]                        Output Base : 00001000
[0004]                   Output Reference : 000000c4
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* 2P NB PCIe0*/
[0001]                               Type : 02
[0002]                             Length : 0038
[0001]                           Revision : 01
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000024

[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000001
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 01
                                Coherency : 1
                         Device Attribute : 0
[0004]                      ATS Attribute : 00000000
[0004]                 PCI Segment Number : 0000000c
                        Memory Size Limit : 00
                                 Reserved : 00000000

[0004]                         Input base : 00002000
[0004]                           ID Count : 00001000
[0004]                        Output Base : 00002000
[0004]                   Output Reference : 000000dc
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

 /* 2P NB PCIe1*/
[0001]                               Type : 02
[0002]                             Length : 0038
[0001]                           Revision : 01
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000024

[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000001
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 01
                                Coherency : 1
                         Device Attribute : 0
[0004]                      ATS Attribute : 00000000
[0004]                 PCI Segment Number : 0000000d
                        Memory Size Limit : 00
                                 Reserved : 00000000

[0004]                         Input base : 00003000
[0004]                           ID Count : 00001000
[0004]                        Output Base : 00003000
[0004]                   Output Reference : 000000dc
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* mbi-gen1 P1 dsa a, named component */
[0001]                               Type : 01
[0002]                             Length : 0046
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000001
[0004]                     Mapping Offset : 00000032

[0004]                         Node Flags : 00000000
[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000000
[0001]              Hints (decoded below) : 00
                               Transient : 0
                          Write Allocate : 0
                           Read Allocate : 0
                                Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                               Coherency : 0
                        Device Attribute : 0
[0001]                  Memory Size Limit : 00
[0016]                        Device Name : "\_SB_.MBI8"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00044800  //device id
[0004]                   Output Reference : 000000c4
[0004]              Flags (decoded below) : 00000001
                          Single Mapping : 1
