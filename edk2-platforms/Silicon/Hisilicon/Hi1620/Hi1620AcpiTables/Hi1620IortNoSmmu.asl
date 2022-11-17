/*
 * Intel ACPI Component Architecture
 * iASL Compiler/Disassembler version 20151124-64
 * Copyright (c) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 * Template for [IORT] ACPI Table (static data table)
 * Format: [ByteLength]  FieldName : HexFieldValue
 */
[0004]                          Signature : "IORT"    [IO Remapping Table]
[0004]                       Table Length : 01c8
[0001]                           Revision : 00
[0001]                           Checksum : BC
[0006]                             Oem ID : "HISI  "            // ?
[0008]                       Oem Table ID : "HIP08   "          // ?
[0004]                       Oem Revision : 00000000            // ?
[0004]                    Asl Compiler ID : "INTL"
[0004]              Asl Compiler Revision : 20150410

[0004]                         Node Count : 00000005           // ITS, SMMU and RC
[0004]                        Node Offset : 00000034           // ?
[0004]                           Reserved : 00000000
[0004]                   Optional Padding : 00 00 00 00

/* 0x34 ITS, for PCIe */
/* Here we use the P680/Hi1620 ACPI table which includes MADT table to help to debuge */
[0001]                               Type : 00
[0002]                             Length : 0018
[0001]                           Revision : 00
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 00000000           // ?
[0004]                     Mapping Offset : 00000000           // ?

[0004]                           ItsCount : 00000001           // ?
[0004]                        Identifiers : 00000000           // how to refer to MADT ?

/*0x4c RC 0 */
[0001]                               Type : 02
[0002]                             Length : 00A0
[0001]                           Revision : 01
[0004]                           Reserved : 00000000
[0004]                      Mapping Count : 0000000C
[0004]                     Mapping Offset : 00000024

[0008]                  Memory Properties : [IORT Memory Access Properties]
[0004]                    Cache Coherency : 00000001
[0001]              Hints (decoded below) : 00
                                Transient : 0
                           Write Allocate : 0
                            Read Allocate : 0
                                 Override : 0
[0002]                           Reserved : 0000
[0001]       Memory Flags (decoded below) : 00
                                Coherency : 0
                         Device Attribute : 0
[0004]                      ATS Attribute : 00000000
[0004]                 PCI Segment Number : 00000000           // should match with above MCFG
                        Memory Size Limit : 00
                                 Reserved : 00000000

/* BDF of pcie host 0 -> stream ID of pcie 0/1 SMMU */
[0004]                         Input base : 00000000
[0004]                           ID Count : 00004000          // the number of IDs in range
[0004]                        Output Base : 00000000
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 1 -> stream ID of pcie 0/1 SMMU */
[0004]                         Input base : 00007b00
[0004]                           ID Count : 00000100          // the number of IDs in range
[0004]                        Output Base : 00007b00
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 2 -> stream ID of pcie 0/1 ITS */
[0004]                         Input base : 00007a00
[0004]                           ID Count : 00000100          // the number of IDs in range
[0004]                        Output Base : 00007a00
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 3 -> stream ID of pcie 0/1 ITS */
[0004]                         Input base : 00007800
[0004]                           ID Count : 00000200          // the number of IDs in range
[0004]                        Output Base : 00007800
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 4 -> stream ID of pcie 4 SMMU */
[0004]                         Input base : 00007c00
[0004]                           ID Count : 00000200          // the number of IDs in range
[0004]                        Output Base : 00007c00
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 5 -> stream ID of pcie 5 SMMU */
[0004]                         Input base : 00007400
[0004]                           ID Count : 00000300          // the number of IDs in range
[0004]                        Output Base : 00007400
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 6 -> stream ID of pcie 6/7 SMMU */
[0004]                         Input base : 00008000
[0004]                           ID Count : 00002000          // the number of IDs in range
[0004]                        Output Base : 00008000
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 7 -> stream ID of pcie 6/7 SMMU */
[0004]                         Input base : 0000BB00
[0004]                           ID Count : 00000100          // the number of IDs in range
[0004]                        Output Base : 0000BB00
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 8 -> stream ID of pcie ITS */
[0004]                         Input base : 0000BA00
[0004]                           ID Count : 00000100          // the number of IDs in range
[0004]                        Output Base : 0000BA00
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 9 -> stream ID of pcie 0/1 ITS */
[0004]                         Input base : 0000B800
[0004]                           ID Count : 00000200          // the number of IDs in range
[0004]                        Output Base : 0000B800
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 10 -> stream ID of pcie 10 SMMU */
[0004]                         Input base : 0000BC00
[0004]                           ID Count : 00000200          // the number of IDs in range
[0004]                        Output Base : 0000BC00
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* BDF of pcie host 11 -> stream ID of pcie 11 SMMU */
[0004]                         Input base : 0000B400
[0004]                           ID Count : 00000300          // the number of IDs in range
[0004]                        Output Base : 0000B400
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000000
                           Single Mapping : 0

/* mbi-gen for S0-TB-L3T0, named component */
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
[0016]                        Device Name : "\_SB_.MB30"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FD1 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-L3T1, named component */
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
[0016]                        Device Name : "\_SB_.MB31"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FD2 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-L3T2, named component */
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
[0016]                        Device Name : "\_SB_.MB32"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FD3 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-L3T3, named component */
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
[0016]                        Device Name : "\_SB_.MB33"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FD4 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-L3T4, named component */
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
[0016]                        Device Name : "\_SB_.MB34"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FD5 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-L3T5, named component */
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
[0016]                        Device Name : "\_SB_.MB35"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FD6 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-DDRC0, named component */
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
[0016]                        Device Name : "\_SB_.MB38"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FDD // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-DDRC1, named component */
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
[0016]                        Device Name : "\_SB_.MB39"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FDE // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-DDRC2, named component */
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
[0016]                        Device Name : "\_SB_.MB3A"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FDF // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-DDRC3, named component */
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
[0016]                        Device Name : "\_SB_.MB3B"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FC7 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-HHA0, named component */
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
[0016]                        Device Name : "\_SB_.MB3C"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FC8 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TB-HHA1, named component */
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
[0016]                        Device Name : "\_SB_.MB3D"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007FC9 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-L3T0, named component */
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
[0016]                        Device Name : "\_SB_.MB10"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F51 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-L3T1, named component */
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
[0016]                        Device Name : "\_SB_.MB11"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F52 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-L3T2, named component */
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
[0016]                        Device Name : "\_SB_.MB12"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F53 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-L3T3, named component */
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
[0016]                        Device Name : "\_SB_.MB13"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F54 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-L3T4, named component */
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
[0016]                        Device Name : "\_SB_.MB14"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F55 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-L3T5, named component */
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
[0016]                        Device Name : "\_SB_.MB15"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F56 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-DDRC0, named component */
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
[0016]                        Device Name : "\_SB_.MB18"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F5D // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-DDRC1, named component */
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
[0016]                        Device Name : "\_SB_.MB19"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F5E // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-DDRC2, named component */
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
[0016]                        Device Name : "\_SB_.MB1A"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F5F // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-DDRC3, named component */
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
[0016]                        Device Name : "\_SB_.MB1B"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F47 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-HHA0, named component */
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
[0016]                        Device Name : "\_SB_.MB1C"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F48 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S0-TA-HHA1, named component */
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
[0016]                        Device Name : "\_SB_.MB1D"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 00007F49 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-L3T0, named component */
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
[0016]                        Device Name : "\_SB_.MB70"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFD1 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-L3T1, named component */
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
[0016]                        Device Name : "\_SB_.MB71"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFD2 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-L3T2, named component */
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
[0016]                        Device Name : "\_SB_.MB72"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFD3 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-L3T3, named component */
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
[0016]                        Device Name : "\_SB_.MB73"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFD4 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-L3T4, named component */
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
[0016]                        Device Name : "\_SB_.MB74"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFD5 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-L3T5, named component */
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
[0016]                        Device Name : "\_SB_.MB75"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFD6 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-DDRC0, named component */
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
[0016]                        Device Name : "\_SB_.MB78"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFDD // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-DDRC1, named component */
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
[0016]                        Device Name : "\_SB_.MB79"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFDE // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-DDRC2, named component */
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
[0016]                        Device Name : "\_SB_.MB7A"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFDF // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-DDRC3, named component */
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
[0016]                        Device Name : "\_SB_.MB7B"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFC7 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-HHA0, named component */
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
[0016]                        Device Name : "\_SB_.MB7C"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFC8 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TB-HHA1, named component */
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
[0016]                        Device Name : "\_SB_.MB7D"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BFC9 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-L3T0, named component */
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
[0016]                        Device Name : "\_SB_.MB50"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF51 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-L3T1, named component */
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
[0016]                        Device Name : "\_SB_.MB51"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF52 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-L3T2, named component */
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
[0016]                        Device Name : "\_SB_.MB52"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF53 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-L3T3, named component */
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
[0016]                        Device Name : "\_SB_.MB53"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF54 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-L3T4, named component */
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
[0016]                        Device Name : "\_SB_.MB54"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF55 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-L3T5, named component */
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
[0016]                        Device Name : "\_SB_.MB55"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF56 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-DDRC0, named component */
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
[0016]                        Device Name : "\_SB_.MB58"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF5D // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-DDRC1, named component */
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
[0016]                        Device Name : "\_SB_.MB59"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF5E // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-DDRC2, named component */
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
[0016]                        Device Name : "\_SB_.MB5A"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF5F // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-DDRC3, named component */
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
[0016]                        Device Name : "\_SB_.MB5B"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF47 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-HHA0, named component */
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
[0016]                        Device Name : "\_SB_.MB5C"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF48 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1

/* mbi-gen for S1-TA-HHA1, named component */
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
[0016]                        Device Name : "\_SB_.MB5D"
[0004]                            Padding : 00 00 00 00

[0004]                         Input base : 00000000
[0004]                           ID Count : 00000001
[0004]                        Output Base : 0000BF49 // PMU device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1


[320h 0800   1]                         Type : 01
[321h 0801   2]                       Length : 0054
[323h 0803   1]                     Revision : 00
[324h 0804   4]                     Reserved : 00000000
[328h 0808   4]                Mapping Count : 00000001
[32Ch 0812   4]               Mapping Offset : 00000040

[330h 0816   4]                   Node Flags : 00000000
[334h 0820   8]            Memory Properties : [IORT Memory Access Properties]
[334h 0820   4]              Cache Coherency : 00000000
[338h 0824   1]        Hints (decoded below) : 00
                                   Transient : 0
                              Write Allocate : 0
                               Read Allocate : 0
                                    Override : 0
[339h 0825   2]                     Reserved : 0000
[33Bh 0827   1] Memory Flags (decoded below) : 00
                                   Coherency : 0
                            Device Attribute : 0
[33Ch 0828   1]            Memory Size Limit : 00
[33Dh 0829  11]                  Device Name : "\_SB_.SEC0"
[348h 0840  24]                      Padding : \
    00 00 00 00 00 00 00 00 01 00 00 00 80 00 04 00 \
    4C 00 00 00 01 00 00 00

[34Ch 0844   4]                   Input base : 00000000
[350h 0848   4]                     ID Count : 00000001
[354h 0852   4]                  Output Base : 00000100
[358h 0856   4]             Output Reference : 00000034
[35Ch 0860   4]        Flags (decoded below) : 00000001
                              Single Mapping : 1
/* RDE device report++.*/
[320h 0800   1]                         Type : 01
[321h 0801   2]                       Length : 0054
[323h 0803   1]                     Revision : 00
[324h 0804   4]                     Reserved : 00000000
[328h 0808   4]                Mapping Count : 00000001
[32Ch 0812   4]               Mapping Offset : 00000040

[330h 0816   4]                   Node Flags : 00000000
[334h 0820   8]            Memory Properties : [IORT Memory Access Properties]
[334h 0820   4]              Cache Coherency : 00000000
[338h 0824   1]        Hints (decoded below) : 00
                                   Transient : 0
                              Write Allocate : 0
                               Read Allocate : 0
                                    Override : 0
[339h 0825   2]                     Reserved : 0000
[33Bh 0827   1] Memory Flags (decoded below) : 00
                                   Coherency : 0
                            Device Attribute : 0
[33Ch 0828   1]            Memory Size Limit : 00
[33Dh 0829  11]                  Device Name : "\_SB_.RDE0"
[348h 0840  24]                      Padding : \
    00 00 00 00 00 00 00 00 01 00 00 00 80 00 04 00 \
    4C 00 00 00 01 00 00 00

[34Ch 0844   4]                   Input base : 00000000
[350h 0848   4]                     ID Count : 00000001
[354h 0852   4]                  Output Base : 00007f13
[358h 0856   4]             Output Reference : 00000034
[35Ch 0860   4]        Flags (decoded below) : 00000001
                              Single Mapping : 1

/* mbi-gen for MCTP, named component */
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
[0004]                        Output Base : 00007F18 // MCTP device id
[0004]                   Output Reference : 00000034
[0004]              Flags (decoded below) : 00000001
                           Single Mapping : 1
