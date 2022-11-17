/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015-2016, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015-2016, Linaro Limited. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(_SB)
{
  Device(SAS0) {
    Name(_HID, "HISI0162")
    Name(_CCA, 1)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0xC3000000, 0x10000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, 0, "\\_SB.MBI2")
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

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI2")
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

    Method (_RST, 0x0, Serialized)
    {
      Store(0x7ffff, RST)
      Store(0x7ffff, CLKD)
      Sleep(1)
      Store(0x7ffff, DRST)
      Store(0x7ffff, CLK)
      Sleep(1)
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x00)
    }
	Method (_STA, 0, NotSerialized)
	{
	  Return (0x0)
	}
  }

  Device(SAS1) {
    Name(_HID, "HISI0162")
    Name(_CCA, 1)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0xA2000000, 0x10000)

      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, 0, "\\_SB.MBI3")
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

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI3")
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

    Method (_RST, 0x0, Serialized)
    {
      Store(0x7ffff, RST)
      Store(0x7ffff, CLKD)
      Sleep(1)
      Store(0x7ffff, DRST)
      Store(0x7ffff, CLK)
      Sleep(1)
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x00)
    }
    Method (_STA, 0, NotSerialized)
    {
      Return(0x0F)
    }
  }
  Device(SAS2) {
    Name(_HID, "HISI0162")
    Name(_CCA, 1)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0xA3000000, 0x10000)

      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, 0, "\\_SB.MBI4")
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

      Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, 0, "\\_SB.MBI4")
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
        Package (2) {"sas-addr", Package() {50, 01, 88, 20, 16, 00, 00, 00}},
        Package () {"queue-count", 16},
        Package () {"phy-count", 8},
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

    Method (_RST, 0x0, Serialized)
    {
      Store(0x7ffff, RST)
      Store(0x7ffff, CLKD)
      Sleep(1)
      Store(0x7ffff, DRST)
      Store(0x7ffff, CLK)
      Sleep(1)
    }
    Method (_PXM, 0, NotSerialized)
    {
      Return(0x00)
    }
	Method (_STA, 0, NotSerialized)
	{
	  Return (0x0)
	}
  }

}
