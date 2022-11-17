/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock ("WFPPlatform.asl","DSDT",2,"INTEL","PLATWFP ",3)
{

  #include "CommonPlatform.asi"
  #include "PlatformPciTree_WFP.asi"
  #include "AMLUPD.asl"
  #include "DSDT.ASL"
  #include "Pch.asl"       //This is in another package (PchPkg)
  #include "Platform.asl"
  #include "PlatformGpe.asi"
  #include "PcieSeg.asi"
 
  Scope (\_SB.PC00.XHCI.RHUB) {



    //
    // Method for creating generic _PLD buffers
    // _PLD contains lots of data, but for purpose of internal validation we care only about
    // ports' visibility and pairing (this requires group position)
    // so these are the only 2 configurable parameters (User Visible, Group Position)
    //
    Method(GPLD, 2, Serialized) {
      Name(PCKG, Package() { Buffer(0x10) {} } )
      CreateField(DerefOf(Index(PCKG,0)), 0, 7, REV)
      Store(1,REV)
      CreateField(DerefOf(Index(PCKG,0)), 64, 1, VISI)
      Store(Arg0, VISI)
      CreateField(DerefOf(Index(PCKG,0)), 87, 8, GPOS)
      Store(Arg1, GPOS)


      return (PCKG)
    }

    //
    // Method for creating generic _UPC buffers
    // Similar to _PLD, for internal testing we only care about 1 parameter (port connectable)
    //
    Method(GUPC, 1, Serialized) {
      Name(PCKG, Package(4) { 0, 0xFF, 0, 0 } )
      Store(Arg0,Index(PCKG,0))


      return (PCKG)
    }



  } //end scope RHUB

  Scope (\_SB.PC00.XHCI.RHUB.HS01) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,1)) } //Rear Panel A [CONN27] - Upper - usb2 port 
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS02) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,2)) } //Rear Panel A [CONN27] - Center - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS03) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,3)) } //Rear Panel A [CONN27] - Bottom - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS04) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,4)) } //Internal A1 [CONN9] - Right - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS05) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,5)) } //Front Panel [CONN20] - Right - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS06) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,6)) } //Front Panel [CONN20] - Left - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS07) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,7)) } //Internal Type A3 [CONN4] - ? - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS08) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,8)) } //Internal Type A3 [CONN4] - ? - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS09) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,9)) } //Jacksonville [CONN22] - Bottom - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS10) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,10)) } //Usb daughter card [CONN14] - ? - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS11) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(0,11)) } //Jacksonville [CONN22] - Center - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS12) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(0,12)) } //Usb daughter card [CONN14] - ? - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS13) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,13)) } //Internal A1 [CONN4] - Left - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS14) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,14)) } //Usb daughter card [CONN14] - ? - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.USR1) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) }
  }

  Scope (\_SB.PC00.XHCI.RHUB.USR2) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } 
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS01) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,1)) } //Rear Panel A [CONN27] - Upper - usb3 port 
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS02) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,2)) } //Rear Panel A [CONN27] - Center - usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS03) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,3)) } //Rear Panel A [CONN27] - Bottom - usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS04) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,4)) } //Internal A1 [CONN9] - Right - usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS05) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,5)) } //Front Panel [CONN20] - Right - usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS06) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,6)) } //Front Panel [CONN20] - Left - usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS07) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS08) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS09) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS10) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

} // end of DSDT
