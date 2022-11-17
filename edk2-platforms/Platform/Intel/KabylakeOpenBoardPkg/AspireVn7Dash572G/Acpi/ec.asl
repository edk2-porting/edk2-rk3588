/** @file

  Copyright (c) 2021, Baruch Binyamin Doron
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

/* Global TODO: (externally: Optimus GC6 and GPS)
 * - TRPS: This is SMI 0xDD, likely in SmmOemDriver. This SW SMI adds to and executes
 *         a table of function pointers produced throughout the OEM 'value-add' stack.
 * - WMI: This is likely SMI 0xD0 in A01WMISmmCallback. This SW SMI likely uses the WMI
 *        object and consumes the OEM 'value-add' stack for EC and presumably the A01*
 *        OEM/ODM 'value-add' stack.
 *
 * Generally, more reversing is needed.
 */
/* TODO: Implement more features around reference code (except, check BoardAcpiDxe first) */

// TODO: Enable and test
#undef LGMR_ENABLED

// "DIDX" - "DeviceIdX" is uninitialised, cannot use "BRTN" method yet
External (\_SB.PCI0.GFX0.DD1F, DeviceObj)
// TODO: Might need fixed VBT - didn't port display toggle tables previously
External (\_SB.PCI0.GFX0.GHDS, MethodObj)
External (\_SB.PCI0.LPCB, DeviceObj)

Device (\_SB.PCI0.LPCB.EC0)
{
  Name (_HID, EisaId ("PNP0C09") /* Embedded Controller Device */)  // _HID: Hardware ID
  Name (_GPE, 0x50)  // _GPE: General Purpose Events
  Name (\ECOK, 0)
#ifdef LGMR_ENABLED
  Name (LGMR, 0xFE800000)  // Static, may depend on EC configuration. Unsure which register.
#endif

  Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
  {
    IO (Decode16, 0x62, 0x62, 0, 1)
    IO (Decode16, 0x66, 0x66, 0, 1)
  })

  OperationRegion (ECO1, SystemIO, 0x62, 1)
  Field (ECO1, ByteAcc, Lock, Preserve)
  {
    PX62, 8
  }

  OperationRegion (ECO2, SystemIO, 0x66, 1)
  Field (ECO2, ByteAcc, Lock, Preserve)
  {
    PX66, 8
  }

#ifdef LGMR_ENABLED
  OperationRegion (ECMB, SystemMemory, LGMR, 0x200)
#endif
  OperationRegion (RAM, EmbeddedControl, 0, 0xFF)
  Field (RAM, ByteAcc, Lock, Preserve)
  {
    CMDB, 8,        /* EC commands */
    ETID, 8,        /* Thermal page selector */
    EBID, 8,        /* Battery page selector */
    Offset (0x06),
    CMD2, 8,        /* param 2: UNUSED */
    CMD1, 8,        /* param 1: UNUSED */
    CMD0, 8,        /* param 0 to EC command */
    Offset (0x0A),
        , 1,
        , 1,
    Offset (0x10),
    EQEN, 1,        /* EQ enable */
    ETEE, 1,        /* TODO */
    Offset (0x4E),
    ISEN, 1,        /* TODO */
    Offset (0x4F),
    ECTP, 8,        /* Touchpad ID */
    Offset (0x51),
        , 3,
    TPEN, 1,        /* Touchpad enable */
    Offset (0x52),
    WLEX, 1,        /* WLAN present */
    BTEX, 1,        /* Bluetooth present */
    EX3G, 1,        /* 3G */
        , 3,
    RFEX, 1,        /* RF present */
#if 0  // Merely a guess
    Offset (0x55),
    BTH0, 8,        /* Battery threshold? TODO: Actually diff in modified vendor FW */
#endif
    Offset (0x57),
        , 7,
    AHKB, 1,        /* Hotkey triggered */
    AHKE, 8,        /* Hotkey data */
    Offset (0x5C),
    Offset (0x5D),
    Offset (0x6C),
    PWLT, 1,        /* NVIDIA GPS: Panel? */
        , 3,
    GCON, 1,        /* Enter Optimus GC6 */
    Offset (0x70),
        , 1,
    ELID, 1,        /* Lid state */
        , 3,
    EACS, 1,        /* AC state */
    Offset (0x71),
    WLEN, 1,        /* WLAN enable */
    BTEN, 1,        /* Bluetooth enable */
        , 3,
    ISS3, 1,
    ISS4, 1,
    ISS5, 1,
        , 4,
    EIDW, 1,        /* Device wake */
    Offset (0x74),
        , 2,
        , 1,
    TPEX, 1,        /* Touchpad present */
    Offset (0x75),
    BLST, 1,        /* Bluetooth state */
    LMIB, 1,        /* TODO */
    Offset (0x76),
    ECSS, 4,        /* EC Notify of power state */
    EOSS, 4,        /* EC Notify of power state */
    Offset (0x88),  /* TODO: Aliased to "EB0S" */
    EB0A, 1,
        , 2,
    EB0R, 1,
    EB0L, 1,
    EB0F, 1,
    EB0N, 1,
    Offset (0x90),
    SCPM, 1,        /* Set cooling policy */
    Offset (0x92),  /* TODO: Aliased to "ETAF" */
    ESSF, 1,
    ECTT, 1,
    EDTT, 1,
    EOSD, 1,        /* Trip */
    EVTP, 1,
    ECP1, 1,
        , 1,
    ECP2, 1,
    Offset (0xA8),
    ES0T, 8,        /* Temperature */
    ES1T, 8,        /* Temperature */
    Offset (0xD0),
    ESP0, 8,        /* Passive temp */
    ESC0, 8,        /* Critical temp */
    ESP1, 8,        /* Passive temp */
    ESC1, 8,        /* Critical temp */
  }
  /* Aliases several battery registers */
  Field (RAM, ByteAcc, Lock, Preserve)
  {
    Offset (0x88),
    EB0S, 8,        /* Battery 0 state */
  }
  /* Aliases several thermal registers */
  Field (RAM, ByteAcc, Lock, Preserve)
  {
    Offset (0x92),
    ETAF, 8,
  }

#ifdef LGMR_ENABLED
  Field (ECMB, ByteAcc, Lock, Preserve)
  {
    Offset (0x02),
        , 1,
    MLID, 1,
        , 3,
    MACS, 1,
    Offset (0x06),
    MBTP, 8,
    Offset (0x08),
    MB0S, 8,
    Offset (0x20),
    MS0T, 8,
    MS1T, 8,
    MS2T, 8,
    MS3T, 8,
    MS4T, 8,
    MS5T, 8,
    Offset (0x53),
    MCSS, 1,
    MCTT, 1,
    MDTT, 1,
    MOSD, 1,
    MVTP, 1,
    Offset (0x54),
    MSP0, 8,
    MSC0, 8,
    MCC0, 8,
    MSC1, 8,
  }
#endif

  Method (_REG, 2, NotSerialized)  // _REG: Region Availability
  {
    If (Arg0 == 3)
    {
      ECOK = Arg1  // OS can clear region availability
      If (Arg1 == 1)  // On initialise
      {
        TINI ()
        EOSS = 0x05
//      OSIN ()

        /* Other pages return valid data too, but this seems to be the page
         * we are expecting - persistently in ectool dump with vendor firmware
         * FIXME: Contents of other pages? */
        ETID = 0x20
      }
    }
  }

  Method (TINI, 0, NotSerialized)
  {
    If (ECOK)
    {
      ETAF = 0
      ETEE = 1
    }
    Else
    {
      /* WBEC: Called SMI function 0x11 */
//    EC_WRITE (0x92, 0)  // ETAF = 0
      /* MBEC: Called SMI function 0x12 */
//    MBEC (0x10, 0xFD, 0x02)  // ETEE = 1
    }
  }

  Name (RFST, 0)  /* RF state */
  Method (ECPS, 1, NotSerialized)  // _PTS: Prepare To Sleep
  {
    ECSS = Arg0
//  COSI = OSYS
//  SPR1 = Arg0
    /* TRPS: Generic SMI trap handler */
//  TRPS (0x82, 0x02)
    If ((Arg0 == 3) || (Arg0 == 4))
    {
      RFST = RFEX
    }
  }

  Method (ECWK, 1, NotSerialized)  // _WAK: Wake
  {
    EQEN = 1
    EOSS = Arg0
    TINI ()
    Notify (BAT0, 0x81) // Information Change
//  COSI = OSYS
//  SPR1 = Arg0
    /* TRPS: Generic SMI trap handler */
//  TRPS (0x82, 0x03)
    If ((Arg0 == 3) || (Arg0 == 4))
    {
      RFEX = RFST
      Notify (SLPB, 0x02) // Device Wake
    }
  }

#if 0  // TODO: Figure out what this is for
  Method (OSIN, 0, NotSerialized)
  {
    COSI = OSYS
    /* TRPS: Generic SMI trap handler */
    TRPS (0x82, 1)
  }
#endif

#if 0  // TODO: Implement
  Method (MBEC, 3, Serialized)  // Read-Modify-Write
  {
    /* Based on similar methods/tables at
     * https://github.com/linuxhw/ACPI/blob/master/Notebook/Sony/SVE1713/SVE1713S1RW/506CDC50E671#L9359
     * which use ASL instead of SMM calls */
    Local0 = EC_READ (Arg0)
    Local0 &= Arg1
    Local0 |= Arg2
    EC_WRITE (Arg0, Local0)
  }
#endif

  /* Graphical hotkey */
  Method (_Q19, 0, NotSerialized)
  {
    ^^^GFX0.GHDS (0x03)
  }

  /* Increase brightness */
  Method (_Q1C, 0, NotSerialized)
  {
    Notify (^^^GFX0.DD1F, 0x86)
  }

  /* Decrease brightness */
  Method (_Q1D, 0, NotSerialized)
  {
    Notify (^^^GFX0.DD1F, 0x87)
  }

  /* Hotkeys */
  Method (_Q2C, 0, NotSerialized)
  {
    If (LMIB)
    {
      If (!AHKB)  /* Else, WMI clears its buffer? */
      {
        Local1 = AHKE
        If ((Local1 > 0) && (Local1 < 0x80))
        {
          \DBGH ("Hotkeys - TODO: Airplane mode?")
          /* WMI -> "GCMS" method */
        }
        ElseIf ((Local1 > 0x80) && (Local1 < 0xA0))
        {
          TPEN ^= 1  /* TODO: Not working. What else does WMI do here? */
        }
      }
    }
  }

  Method (_Q36, 0, NotSerialized)
  {
    If (ECOK)
    {
      EOSD = 1  // Thermal trip
    }
    Else
    {
      /* MBEC: Called SMI function 0x12 */
//    MBEC (0x92, 0xF7, 0x08)  // EOSD = 1
    }

    Sleep (500)
    Notify (\_TZ.TZ01, 0x80) // Thermal Status Change
    Notify (\_TZ.TZ00, 0x80) // Thermal Status Change
  }

  Method (_Q3F, 0, NotSerialized)
  {
    \DBGH ("EC Query: 0x3F - TRPS")
    /* TRPS: Generic SMI trap handler */
//  TRPS (0x80, 0)
  }

  Method (_Q40, 0, NotSerialized)
  {
    Notify (BAT0, 0x81) // Information Change
  }

  Method (_Q41, 0, NotSerialized)
  {
    Notify (BAT0, 0x81) // Information Change
  }

  /* Battery status change */
  Method (_Q48, 0, NotSerialized)
  {
    Notify (BAT0, 0x80)
  }

  /* Battery critical? */
  Method (_Q4C, 0, NotSerialized)
  {
    If (B0ST)
    {
      Notify (BAT0, 0x80) // Status Change
    }
  }

  /* AC status change: present */
  Method (_Q50, 0, NotSerialized)
  {
    Notify (ADP1, 0x80)
  }

  /* AC status change: not present */
  Method (_Q51, 0, NotSerialized)
  {
    Notify (ADP1, 0x80)
  }

  /* Lid status change: open */
  Method (_Q52, 0, NotSerialized)
  {
    Notify (LID0, 0x80)
  }

  /* Lid status change: close */
  Method (_Q53, 0, NotSerialized)
  {
    Notify (LID0, 0x80)
  }

  Method (_Q60, 0, NotSerialized)
  {
    \DBGH ("EC Query: 0x60 -> WMI")
  }

  Method (_Q61, 0, NotSerialized)
  {
    \DBGH ("EC Query: 0x61 -> WMI")
  }

  Method (_Q62, 0, NotSerialized)
  {
    \DBGH ("EC Query: 0x62 -> Optimus GC6")
  }

  Method (_Q63, 0, NotSerialized)
  {
    \DBGH ("EC Query: 0x63 -> Optimus GC6")
  }

  Method (_Q67, 0, NotSerialized)
  {
    \DBGH ("EC Query: 0x67 -> NVIDIA GPS")
  }

  Method (_Q68, 0, NotSerialized)
  {
    \DBGH ("EC Query: 0x68 -> NVIDIA GPS")
  }

  Method (_Q6C, 0, NotSerialized)
  {
    \DBGH ("EC Query: 0x6C - TRPS")
    /* TRPS: Generic SMI trap handler */
//  TRPS (0x81, 0)
  }

  Method (_Q6D, 0, NotSerialized)
  {
    \DBGH ("EC Query: 0x6D - TRPS")
    /* TRPS: Generic SMI trap handler */
//  TRPS (0x81, 1)
  }

  #include "ac.asl"
  #include "battery.asl"
}
