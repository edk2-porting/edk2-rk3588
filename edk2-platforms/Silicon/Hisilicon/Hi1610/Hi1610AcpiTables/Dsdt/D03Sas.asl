/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(_SB)
{
  Device(SAS0) {
    Name(_HID, "HISI0162")
    Name(_CCA, 1)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0xC3000000, 0x10000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, 0, "\\_SB.MBI6")
      {
        64,65,66,67,68,
        69,70,71,72,73,
        74,75,76,77,78,
        79,80,81,82,83,
        84,85,86,87,88,
        89,90,91,92,93,
        94,95,96,97,98,
        99,100,101,102,103,
        104,105,106,107,108,
        109,110,111,112,113,
        114,115,116,117,118,
        119,120,121,122,123,
        124,125,126,127,128,
        129,130,131,132,133,
        134,135,136,137,138,
        139,140,141,142,143,
        144,145,146,147,148,
        149,150,151,152,153,
        154,155,156,157,158,
        159,
      }

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI6" )
      {
        601,602,603,604,
        605,606,607,608,609,
        610,611,612,613,614,
        615,616,617,618,619,
        620,621,622,623,624,
        625,626,627,628,629,
        630,631,632,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"interrupt-parent",Package() {\_SB.MBI6}},
        Package (2) {"sas-addr", Package() {50, 01, 88, 20, 16, 00, 00, 0x00}},
        Package () {"queue-count", 16},
        Package () {"phy-count", 8},
      }
    })

    OperationRegion (CTL, SystemMemory, 0xC0000000, 0x10000)
    Field (CTL, AnyAcc, NoLock, Preserve)
    {
      Offset (0x338),
      CLK, 32,
      CLKD, 32,
      Offset (0xa60),
      RST, 32,
      DRST, 32,
      Offset (0x5a30),
      STS, 32,
    }

    OperationRegion (PHYS, SystemMemory, 0xC3002000, 0x2000)
    Field (PHYS, DWordAcc, NoLock, Preserve) {
      Offset (0x0014),
      PHY0, 32,
      Offset (0x0414),
      PHY1, 32,
      Offset (0x0814),
      PHY2, 32,
      Offset (0x0c14),
      PHY3, 32,
      Offset (0x1014),
      PHY4, 32,
      Offset (0x1414),
      PHY5, 32,
      Offset (0x1814),
      PHY6, 32,
      Offset (0x1c14),
      PHY7, 32,
    }

    OperationRegion (SYSR, SystemMemory, 0xD0000000, 0x10000)
    Field (SYSR, DWordAcc, NoLock, Preserve) {
      Offset (0xe014),
      DIE4, 32,
    }

    Method (_RST, 0x0, Serialized)
    {
      Store(0x7ffff, RST)
      Store(0x7ffff, CLKD)
      Sleep(1)
      Store(0x7ffff, DRST)
      Store(0x7ffff, CLK)
      Sleep(1)
      Store(DIE4, local0)
      If (LEqual (local0, 0)) {
        /* 66MHZ */
        Store(0x0199B694, Local1)
        Store(Local1, PHY0)
        Store(Local1, PHY1)
        Store(Local1, PHY2)
        Store(Local1, PHY3)
        Store(Local1, PHY4)
        Store(Local1, PHY5)
        Store(Local1, PHY6)
        Store(Local1, PHY7)
      }
    }
  }

  Device(SAS1) {
    Name(_HID, "HISI0162")
    Name(_CCA, 1)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0xA2000000, 0x10000)

      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, 0, "\\_SB.MBI1")
      {
        64,65,66,67,68,
        69,70,71,72,73,
        74,75,76,77,78,
        79,80,81,82,83,
        84,85,86,87,88,
        89,90,91,92,93,
        94,95,96,97,98,
        99,100,101,102,103,
        104,105,106,107,108,
        109,110,111,112,113,
        114,115,116,117,118,
        119,120,121,122,123,
        124,125,126,127,128,
        129,130,131,132,133,
        134,135,136,137,138,
        139,140,141,142,143,
        144,145,146,147,148,
        149,150,151,152,153,
        154,155,156,157,158,
        159,
      }

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI1")
      {
        576,577,578,579,580,
        581,582,583,584,585,
        586,587,588,589,590,
        591,592,593,594,595,
        596,597,598,599,600,
        601,602,603,604,605,
        606,607,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"interrupt-parent",Package() {\_SB.MBI1}},
        Package (2) {"sas-addr", Package() {50, 01, 88, 20, 16, 00, 00, 00}},
        Package () {"queue-count", 16},
        Package () {"phy-count", 8},
        Package () {"hip06-sas-v2-quirk-amt", 1},
      }
    })

    OperationRegion (CTL, SystemMemory, 0xA0000000, 0x10000)
    Field (CTL, AnyAcc, NoLock, Preserve)
    {
      Offset (0x318),
      CLK, 32,
      CLKD, 32,
      Offset (0xa18),
      RST, 32,
      DRST, 32,
      Offset (0x5a0c),
      STS, 32,
    }

    OperationRegion (PHYS, SystemMemory, 0xA2002000, 0x2000)
    Field (PHYS, DWordAcc, NoLock, Preserve) {
      Offset (0x0014),
      PHY0, 32,
      Offset (0x0414),
      PHY1, 32,
      Offset (0x0814),
      PHY2, 32,
      Offset (0x0c14),
      PHY3, 32,
      Offset (0x1014),
      PHY4, 32,
      Offset (0x1414),
      PHY5, 32,
      Offset (0x1814),
      PHY6, 32,
      Offset (0x1c14),
      PHY7, 32,
    }

    OperationRegion (SYSR, SystemMemory, 0xD0000000, 0x10000)
    Field (SYSR, DWordAcc, NoLock, Preserve) {
      Offset (0xe014),
      DIE4, 32,
    }

    Method (_RST, 0x0, Serialized)
    {
      Store(0x7ffff, RST)
      Store(0x7ffff, CLKD)
      Sleep(1)
      Store(0x7ffff, DRST)
      Store(0x7ffff, CLK)
      Sleep(1)
      Store(DIE4, local0)
      If (LEqual (local0, 0)) {
        /* 66MHZ */
        Store(0x0199B694, Local1)
        Store(Local1, PHY0)
        Store(Local1, PHY1)
        Store(Local1, PHY2)
        Store(Local1, PHY3)
        Store(Local1, PHY4)
        Store(Local1, PHY5)
        Store(Local1, PHY6)
        Store(Local1, PHY7)
      }
    }
  }

  Device(SAS2) {
    Name(_HID, "HISI0162")
    Name(_CCA, 1)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0xA3000000, 0x10000)

      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, 0, "\\_SB.MBI2")
      {
        192,193,194,195,196,
        197,198,199,200,201,
        202,203,204,205,206,
        207,208,209,210,211,
        212,213,214,215,216,
        217,218,219,220,221,
        222,223,224,225,226,
        227,228,229,230,231,
        232,233,234,235,236,
        237,238,239,240,241,
        242,243,244,245,246,
        247,248,249,250,251,
        252,253,254,255,256,
        257,258,259,260,261,
        262,263,264,265,266,
        267,268,269,270,271,
        272,273,274,275,276,
        277,278,279,280,281,
        282,283,284,285,286,
        287,
      }

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI2")
      {
        608,609,610,611,
        612,613,614,615,616,
        617,618,619,620,621,
        622,623,624,625,626,
        627,628,629,630,631,
        632,633,634,635,636,
        637,638,639,
      }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"interrupt-parent",Package() {\_SB.MBI2}},
        Package (2) {"sas-addr", Package() {50, 01, 88, 20, 16, 00, 00, 00}},
        Package () {"queue-count", 16},
        Package () {"phy-count", 9},
      }
    })

    OperationRegion (CTL, SystemMemory, 0xA0000000, 0x10000)
    Field (CTL, AnyAcc, NoLock, Preserve)
    {
      Offset (0x3a8),
      CLK, 32,
      CLKD, 32,
      Offset (0xae0),
      RST, 32,
      DRST, 32,
      Offset (0x5a70),
      STS, 32,
    }

    OperationRegion (PHYS, SystemMemory, 0xA3002000, 0x2400)
    Field (PHYS, DWordAcc, NoLock, Preserve) {
      Offset (0x0014),
      PHY0, 32,
      Offset (0x0414),
      PHY1, 32,
      Offset (0x0814),
      PHY2, 32,
      Offset (0x0c14),
      PHY3, 32,
      Offset (0x1014),
      PHY4, 32,
      Offset (0x1414),
      PHY5, 32,
      Offset (0x1814),
      PHY6, 32,
      Offset (0x1c14),
      PHY7, 32,
      offset (0x2014),
      PHY8, 32,
    }

    OperationRegion (SYSR, SystemMemory, 0xD0000000, 0x10000)
    Field (SYSR, DWordAcc, NoLock, Preserve) {
      Offset (0xe014),
      DIE4, 32,
    }

    Method (_RST, 0x0, Serialized)
    {
      Store(0x7ffff, RST)
      Store(0x7ffff, CLKD)
      Sleep(1)
      Store(0x7ffff, DRST)
      Store(0x7ffff, CLK)
      Sleep(1)
      Store(DIE4, local0)
      If (LEqual (local0, 0)) {
        /* 66MHZ */
        Store(0x0199B694, Local1)
        Store(Local1, PHY0)
        Store(Local1, PHY1)
        Store(Local1, PHY2)
        Store(Local1, PHY3)
        Store(Local1, PHY4)
        Store(Local1, PHY5)
        Store(Local1, PHY6)
        Store(Local1, PHY7)
        Store(Local1, PHY8)
      }
    }
  }

}
