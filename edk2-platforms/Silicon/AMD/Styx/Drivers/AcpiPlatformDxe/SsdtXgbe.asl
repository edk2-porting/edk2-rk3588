/** @file

  SSDT for 10GbE network controllers

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>
  Copyright (c) 2018, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock ("SsdtXgbe.aml", "SSDT", 2, "AMDINC", "StyxXgbe", 3)
{
    Scope (_SB)
    {
        Device (ETH0)
        {
            Name (_HID, "AMDI8001")  // _HID: Hardware ID
            Name (_UID, 0x00)  // _UID: Unique ID
            Name (_CCA, 0x01)  // _CCA: Cache Coherency Attribute
            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings

            {
                Memory32Fixed (ReadWrite,
                    0xE0700000,         // Address Base (XGMAC)
                    0x00010000,         // Address Length
                    )
                Memory32Fixed (ReadWrite,
                    0xE0780000,         // Address Base (XPCS)
                    0x00080000,         // Address Length
                    )
                Memory32Fixed (ReadWrite,
                    0xE1240800,         // Address Base (SERDES_RxTx)
                    0x00000400,         // Address Length
                    )
                Memory32Fixed (ReadWrite,
                    0xE1250000,         // Address Base (SERDES_IR_1)
                    0x00000060,         // Address Length
                    )
                Memory32Fixed (ReadWrite,
                    0xE12500F8,         // Address Base (SERDES_IR_2)
                    0x00000004,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) {0x00000165, } // XGMAC
                Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, ,, )  {0x0000017A, } // DMA0
                Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, ,, )  {0x0000017B, } // DMA1
                Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, ,, )  {0x0000017C, } // DMA2
                Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, ,, )  {0x0000017D, } // DMA3
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) {0x00000163, } // XPCS
            })
            Name (_DSD, Package (0x02)  // _DSD: Device-Specific Data
            {
                ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                Package ()
                {
                    Package (0x02) {"mac-address", Package (0x06) {0x02, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5}},
                    Package (0x02) {"phy-mode", "xgmii"},
                    Package (0x02) {"amd,speed-set", 0x00},
                    Package (0x02) {"amd,dma-freq", 0x0EE6B280},
                    Package (0x02) {"amd,ptp-freq",  0x0EE6B280},
                    Package (0x02) {"amd,serdes-blwc", Package (0x03) {1, 1, 0}},
                    Package (0x02) {"amd,serdes-cdr-rate", Package (0x03) {2, 2, 7}},
                    Package (0x02) {"amd,serdes-pq-skew", Package (0x03) {10, 10, 18}},
                    Package (0x02) {"amd,serdes-tx-amp", Package (0x03) {15, 15, 10}},
                    Package (0x02) {"amd,serdes-dfe-tap-config", Package (0x03) {3, 3, 1}},
                    Package (0x02) {"amd,serdes-dfe-tap-enable", Package (0x03) {0, 0, 127}},
                    Package (0x02) {"amd,per-channel-interrupt", 0x01}
                }
            })
        }

        Device (ETH1)
        {
            Name (_HID, "AMDI8001")  // _HID: Hardware ID
            Name (_UID, 0x01)  // _UID: Unique ID
            Name (_CCA, 0x01)  // _CCA: Cache Coherency Attribute
            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadWrite,
                    0xE0900000,         // Address Base (XGMAC)
                    0x00010000,         // Address Length
                    )
                Memory32Fixed (ReadWrite,
                    0xE0980000,         // Address Base (XPCS)
                    0x00080000,         // Address Length
                    )
                Memory32Fixed (ReadWrite,
                    0xE1240C00,         // Address Base (SERDES_RxTx)
                    0x00000400,         // Address Length
                    )
                Memory32Fixed (ReadWrite,
                    0xE1250080,         // Address Base (SERDES_IR_1)
                    0x00000060,         // Address Length
                    )
                Memory32Fixed (ReadWrite,
                    0xE12500FC,         // Address Base (SERDES_IR_2)
                    0x00000004,         // Address Length
                    )
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) {0x00000164, } // XGMAC
                Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, ,, )  {0x00000175, } // DMA0
                Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, ,, )  {0x00000176, } // DMA1
                Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, ,, )  {0x00000177, } // DMA2
                Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, ,, )  {0x00000178, } // DMA3
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) {0x00000162, } // XPCS
            })
            Name (_DSD, Package (0x02)  // _DSD: Device-Specific Data
            {
                ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                Package ()
                {
                    Package (0x02) {"mac-address", Package (0x06) {0x02, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5}},
                    Package (0x02) {"phy-mode", "xgmii"},
                    Package (0x02) {"amd,speed-set", 0x00},
                    Package (0x02) {"amd,dma-freq", 0x0EE6B280},
                    Package (0x02) {"amd,ptp-freq",  0x0EE6B280},
                    Package (0x02) {"amd,serdes-blwc", Package (0x03) {1, 1, 0}},
                    Package (0x02) {"amd,serdes-cdr-rate", Package (0x03) {2, 2, 7}},
                    Package (0x02) {"amd,serdes-pq-skew", Package (0x03) {10, 10, 18}},
                    Package (0x02) {"amd,serdes-tx-amp", Package (0x03) {15, 15, 10}},
                    Package (0x02) {"amd,serdes-dfe-tap-config", Package (0x03) {3, 3, 1}},
                    Package (0x02) {"amd,serdes-dfe-tap-enable", Package (0x03) {0, 0, 127}},
                    Package (0x02) {"amd,per-channel-interrupt", 0x01}
                }
            })
        }
    }
}

