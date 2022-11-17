/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/


//;************************************************************************;
//;   IMPORTANT NOTE:
//;      Code in this file should be generic/common for any IIO PCIe root port.
//;      DO NOT use hard-coded Bus/Dev/Function # in this file.
//;
//;************************************************************************;



  Name(DBFL, 0)   //  Debug flag    0/1 = disable/enable debug checkpoints in this file



  //;************************************************************************;
  //; This DVPS() method detects if the root port is present and hot-plug capable.
  //;    Input :  None
  //;    Output:  Non-zero  -  Root port is present and hot-plug capable
  //;             Zero      -  otherwise
  //;************************************************************************;
  Method(DVPS,0) {
        // Check if VID/DID = 3C0x_8086 to see if the root port exists
        If (LNotEqual(VID, 0x8086)) { Return(Zero) }
        //If( LNotEqual(And(DID, 0xFFF0), 0x3C00)) { Return(Zero) }
        If( LNotEqual(And(DID, 0xFFF0), 0x2F00)) { Return(Zero) } //HSX
        // Check the root port to see if slot is implemented and Hot-Plug Capable
        If(LNot(And(SLIP, HPCP))) { Return(Zero) }
        Return (One)
  }


  //;************************************************************************;
  //; This HPEN() method programs "Enable ACPI mode for Hot-plug" bit based on input Arg0
  //;        See IIO PCIe rootport MISCCTRLSTS register 188h[3] definition
  //;    Input :  0/1   bit value to set "Enable ACPI mode for Hot-plug" (IIO PCIe rootport register 188h[3])  
  //;    Output:  None
  //;************************************************************************;
  Method (HPEN, 1, Serialized) {

      DB55(0x71, 0)   // debug point

      // get Bus/Dev/Func information of this root port
      Store(^^_BBN, Local0)                    // Local0 = Bus# of parent Host bus
      //Store(_BBN, Local0)                    //  implicit reference to PC00._BBN
      ShiftRight(_ADR, 16, Local1)             // Local1 = self Device #
      And(_ADR, 0x0000ffff, Local2)            // Local2 = self Function #
 
      // Calculate MMCFG config address of MISCCTRLSTS register at B:D:F:offset 188h
      //Name (MISR, 0xC0000188)
      Name (MISR, 0)                              // create a pointer to MMCFG region space
      Add(MMBS, 0x188, MISR)                      // MISR = MMCFG_BASE_ADDR + Offset of MISCCTRLSTS CSR
      Add(ShiftLeft(Local0, 20), MISR, MISR)      // Build bus/dev/func number fields of MISR
      Add(ShiftLeft(Local1, 15), MISR, MISR)
      Add(ShiftLeft(Local2, 12), MISR, MISR)

      DB55(0x77, MISR)   // debug point


      // Create OpRegion for MISCCTRLSTS register at B:D:F:offset 188h
      OperationRegion (OP37, SystemMemory, MISR, 0x04)
      Field (OP37, DWordAcc, NoLock, Preserve) {
             ,   3,
         HGPE,   1,                                  // "Enable ACPI mode for Hot-plug" (register 188h[3]) 
      }

      // Program "Enable ACPI mode for Hot-plug" bit to input Arg0
         Store(Arg0, HGPE)
  }

  //;************************************************************************;
  //; This DB55() method is a debug method
  //;    Input :  Arg0   Postcode to be sent to IO Port 80h
  //;             Arg1   DWord data to be copied to debug memory location
  //;                     in BIOS Data Area (DBA) 40:42
  //;    Output:  None
  //;************************************************************************;
  Method (DB55, 2, NotSerialized) {

      If (DBFL)   {   // if debug is enabled
        Store(Arg0, IO80)         // send postcode to port 80h
        Store(Arg1, MDWD)         // store Arg1 to debug memory location 
        Sleep(4000)              // stall for 4 seconds
      }
  }

  OperationRegion (OP38, SystemMemory, 0x442, 0x04)
  Field (OP38, AnyAcc, NoLock, Preserve) {
       MDWD,   32,                    // dword at BIOS Data Area (BDA)  40:42 (floppy task-file), used as debug memory location
  }


  Method (_INI, 0, NotSerialized) {

      If (LEqual(Zero,DVPS)) {
         Return                           // Do nothing if this root port is not "Present and Hot-plugable"
      }
      HPEN(1)                         // No. Enable ACPI Hot-plug events
  }


/* Greencity code
  OperationRegion (MCTL, SystemMemory, 0xA0048188, 0x04)
  Field (MCTL, ByteAcc, NoLock, Preserve) {
        ,   3,
    HGPE,   1,
        ,   7,
        ,   8,
        ,   8
  }

  Method (_INI, 0, NotSerialized) {
    Store (0x01, HGPE)          //enable GPE message generation for ACPI hotplug support
  }
*/

//MCWU  Changed ^HP02 to HP02 to avoid re-definition when this file is included under multiple BRxx devices
  //Name(^HP02, Package(4){0x08, 0x40, 1, 0} )
  Name(HP02, Package(4){0x08, 0x40, 1, 0} )
  Method(_HPP, 0) { return(HP02) }

  //
  // begin hotplug code
  //
  Name(SHPC, 0x40)            // Slot Hot-plug Capable

  Name(SPDS, 0x040)           // Slot Presence Detect State

  Name(MRLS, 0x0)             // MRL Closed, Standby Power to slot is on
  Name(CCOM, 0x010)           // Command Complete
  Name(SPDC, 0x08)            // Slot Presence Detect Changes
  Name(MRLC, 0x04)            // Slot MRL Changed
  Name(SPFD, 0x02)            // Slot Power Fault Detected
  Name(SABP, 0x01)            // Slot Attention Button Pressed

  Name(SPOF, 0x10)            // Slot  Power Off
  Name(SPON, 0x0F)            // Slot  Power On Mask

  Name(ALMK, 0x1C)            // Slot Atten. LED Mask
  Name(ALON, 0x01)            // Slot Atten. LED On
  Name(ALBL, 0x02)            // Slot Atten LED Blink
  Name(ALOF, 0x03)            // Slot Atten LED Off

  Name(PLMK, 0x13)            // Slot Pwr. LED Mask
  Name(PLON, 0x04)            // Slot Pwr. LED On
  Name(PLBL, 0x08)            // Slot Pwr. LED Blink
  Name(PLOF, 0x0C)            // Slot Pwr. LED Off

  //;*************************************
  //;   Bit 3 = Presence Detect Event
  //;   Bit 2 = MRL Sensor Event
  //;   Bit 1 = PWR Fault Event
  //;   Bit 0 = Attention Button Event
  //;*************************************
  Name(HPEV, 0xF)             // Possible interrupt events (all)

  //;************************************************************************;
  //;
  //; PCIe Slot Capabilities Register A4-A7h
  //;    Bit - 31-7  -  Not used
  //;    Bit -   6   -  Hot-Plug Capable
  //;    Bit -   5   -  Hot-Plug Surprise
  //;    Bit -   4   -  Power Indicator Present.
  //;    Bit -   3   -  Attention Indicator Present.
  //;    Bit -   2   -  MRL Sensor Present.
  //;    Bit -   1   -  Power Controller Present.
  //;    Bit -   0   -  Attention Button Present.
  //;
  //; PCIe Slot control Register A8-A9h
  //;
  //;    Bit -  10   -  PWR Control Disable
  //;    Bit - 9:8   -  Attn Indicator
  //;    Bit - 7:6   -  PWR Indicator
  //;    Bit -   5   -  Hot-Plug Interrupt Event Enable
  //;    Bit -   4   -  Command Complete Interrupt enable
  //;    Bit -   3   -  Presence Detect Changed Interrupt enable
  //;    Bit -   2   -  MRL Sensor Changed Interrupt enable
  //;    Bit -   1   -  PwrFault Detect Interrupt enable
  //;    Bit -   0   -  Attention Button Pressed Interrupt Enable
  //;
  //; PCIe Slot Status Registers AA-ADh
  //;
  //;    Bit -   6   -  Presence Detect State.
  //;    Bit -   5   -  MRL Sensor State.
  //;    Bit -   4   -  Command Completed.
  //;
  //;    RWC Status Bits
  //;
  //;    Bit -   3   -  Presence Detect Changed.
  //;    Bit -   2   -  MRL Sensor Changed.
  //;    Bit -   1   -  Power Fault Detected.
  //;    Bit -   0   -  Attention Button Pressed.
  //;************************************************************************;

  OperationRegion(PPA4, PCI_Config, 0x00, 0x0ff)
  Field(PPA4,ByteAcc,NoLock,Preserve) {

    Offset(0x00),             //  VenderID/DeviceID register
    VID,  16,                 //  VID = 0x8086
    DID,  16,                 //  Device IDs for IIO PCI Express root ports are as follows:
                              //     0x3C00: DMI mode 0x3C01: the DMI port running in PCIe mode
                              //     0x3C02: Port 1a
                              //     0x3C03: Port 1b
                              //     0x3C04: Port 2a
                              //     0x3C05: Port 2b
                              //     0x3C06: Port 2c
                              //     0x3C07: Port 2d
                              //     0x3C08: Port 3a in PCIe mode
                              //     0x3C09: Port 3b
                              //     0x3C0A: Port 3c
                              //     0x3C0B: Port 3d
                              //    (0x3C0F: IIO NTB Secondary Endpoint)

    Offset(0x92),	      // PXPCAP - PCIe CAP Register
    ,     8,
    SLIP, 1,                  //    bit8   Slot Implemented

    offset(0xA4),             // SLTCAP - Slot Capabilities Register
    ATBP,1,                   //    bit0   Attention Button Present
    PWCP,1,                   //    bit1   Power Controller Present
    MRSP,1,                   //    bit2   MRL Sensor Present
    ATIP,1,                   //    bit3   Attention Indicator Present
    PWIP,1,                   //    bit4   Power Indicator Present
    HPSR,1,                   //    bit5   Hot-Plug Surprise
    HPCP,1,                   //    bit6   Hot-Plug Capable

    offset(0xA8),             // SLTCON - PCIE Slot Control Register
    ABIE,1,                   //    bit0   Attention Button Pressed Interrupt Enable
    PFIE,1,                   //    bit1   Power Fault Detected Interrupt Enable
    MSIE,1,                   //    bit2   MRL Sensor Changed Interrupt Enable
    PDIE,1,                   //    bit3   Presence Detect Changed Interrupt Enable.
    CCIE,1,                   //    bit4   Command Complete Interrupt Enable.
    HPIE,1,                   //    bit5   Hot-plug Interrupt Enable.
    SCTL,5,                   //    bit[10:6]  Attn/Power indicator and Power controller.

    offset(0xAA),             // SLTSTS - PCIE Slot Status Register
    SSTS,7,                   // The status bits in Slot Status Reg
   ,1,
}

  OperationRegion(PPA8, PCI_Config, 0x00, 0x0ff)
  Field(PPA8,ByteAcc,NoLock,Preserve) {
    Offset(0xA8),             // SLTCON - PCIE Slot Control Register
    ,6,
    ATID,2,                   //    bit[7:6]   Attention Indicator Control.
    PWID,2,                   //    bit[9:8]   Power Indicator Control.
    PWCC,1,                   //    bit[10]    Power Controller Control.
    ,5,
    Offset(0xAA),             // SLTSTS - PCIE Slot status Register  (WRC)
    ABPS,1,                   //     bit0  Attention Button Pressed Status (RWC)
    PFDS,1,                   //     bit1  Power Fault Detect Status (RWC)
    MSCS,1,                   //     bit2  MRL Sensor Changed Status
    PDCS,1,                   //     bit3  Presence Detect Changed Status
    CMCS,1,                   //     bit4  Command Complete Status
    MSSC,1,                   //     bit5  MRL Sensor State
    PRDS,1,                   //     bit6  Presence Detect State
    ,1,
  }

  //;************************************************************************;
  //; This OSHP (Operating System Hot Plug) method is provided for each HPC
  //; which is controlled by ACPI. This method disables ACPI access to the
  //; HPC and restores the normal System Interrupt and Wakeup Signal
  //; connection.
  //;************************************************************************;
  Method(OSHP) {              // OS call to unhook Legacy ASL PCI-Express HP code.
    Store(0, SSTS)            // Clear any status
//    Store(0x0, HGPE)          // Disable GPE generation
    HPEN(0)                 // Disable GPE generation
  }

  //;************************************************************************;
  //; Hot Plug Controller Command Method
  //;
  //; Input: Arg0 - Command to issue
  //;
  //;************************************************************************;
  Method(HPCC,1) {
    Store(SCTL, Local0)           // get current command state
    Store(0,    Local1)           // reset the timeout value
    If(LNotEqual(Arg0, Local0)) { // see if state is different
      Store(Arg0, SCTL)           // Update the Slot Control
      While(LAnd (LNot(CMCS), LNotEqual(100, Local1))) {    // spin while CMD complete bit is not set,
                                                            // check for timeout to avoid dead loop
        Store(0x2C, IO80)
        Sleep(2)                  // allow processor time slice
        Add(Local1, 2, Local1)
      }
      Store(0x1, CMCS)            // Clear the command complete status
    }
  }

  //;************************************************************************;
  //; Attention Indicator Command
  //;
  //; Input: Arg0 - Command to issue
  //;               1 = ON
  //;               2 = Blink
  //;               3 = OFF
  //;************************************************************************;
  Method(ATCM,1) {
    Store(SCTL, Local0)           // Get Slot Control
    And(Local0, ALMK, Local0)     // Mask the Attention Indicator Bits
    If(LEqual(Arg0, 0x1)){        // Attenion indicator "ON?"
      Or(Local0, ALON, Local0)    // Set the Attention Indicator to "ON"
    }
    If(LEqual(Arg0, 0x2)){        // Attenion indicator "BLINK?"
      Or(Local0, ALBL, Local0)    // Set the Attention Indicator to "BLINK"
    }
    If(LEqual(Arg0, 0x3)){        // Attenion indicator "OFF?"
      Or(Local0, ALOF, Local0)    // Set the Attention Indicator to "OFF"
    }
    HPCC(Local0)
  }

  //;************************************************************************;
  //; Power Indicator Command
  //;
  //; Input: Arg0 - Command to issue
  //;               1 = ON
  //;               2 = Blink
  //;               3 = OFF
  //;************************************************************************;
  Method(PWCM,1){
    Store(SCTL, Local0)           // Get Slot Control
    And(Local0, PLMK, Local0)     // Mask the Power Indicator Bits
    If(LEqual(Arg0, 0x1)){        // Power indicator "ON?"
      Or(Local0, PLON, Local0)    // Set the Power Indicator to "ON"
    }
    If(LEqual(Arg0, 0x2)){        // Power indicator "BLINK?"
      Or(Local0, PLBL, Local0)    // Set the Power Indicator to "BLINK"
    }
    If(LEqual(Arg0, 0x3)){        // Power indicator "OFF?"
      Or(Local0, PLOF, Local0)    // Set the Power Indicator to "OFF"
    }
    HPCC(Local0)
  }

  //;************************************************************************;
  //; Power Slot Command
  //;
  //; Input: Arg0 - Command to issue
  //;               1 = Slot Power ON
  //;               2 = Slot Power Off
  //;************************************************************************;
  Method(PWSL,1){
    Store(SCTL, Local0)           // Get Slot Control
    If(Arg0){                     // Power Slot "ON" Arg0 = 1
      And(Local0, SPON, Local0)   // Turns the Power "ON"
    } Else {                      // Power Slot "OFF"
      Or(Local0, SPOF, Local0)    // Turns the Power "OFF"
    }
    HPCC(Local0)
  }

  //;************************************************************************;
  //; _OST Methods to indicate that the device Eject/insert request is
  //; pending, OS could not complete it
  //;
  //; Input: Arg0 - Value used in Notify to OS
  //;               0x00 - card insert
  //;               0x03 - card eject  
  //;        Arg1 - status of Notify
  //;               0 - success
  //;               0x80 - Ejection not supported by OSPM
  //;               0x81 - Device in use
  //;               0x82 - Device Busy
  //;               0x84 - Ejection in progress-pending
  //;************************************************************************;
  Method(_OST,3,Serialized) {
    Switch(And(Arg0,0xFF)) {      // Mask to retain low byte
      Case(0x03) {                // Ejection Request
        Switch(ToInteger(Arg1)) {
          Case(Package() {0x80, 0x81, 0x82, 0x83}) {
            //
            // Ejection Failure for some reason
            //
            If (Lnot(PWCC)) {     // if slot is powered
              PWCM(0x1)           // Set PowerIndicator to ON
              Store(0x1,ABIE)     // Set AttnBtn Interrupt ON
            }
          }
        }
      }
    }
  } // End _OST

  //;************************************************************************;
  //; Eject Control Methods to indicate that the device is hot-ejectable and
  //; should "eject" the device.
  //;
  //; Input: Arg0 - Not use.
  //;
  //;************************************************************************;
  Method(EJ02, 1){
    Store(0xFF, IO80)
    Store(SCTL, Local0)             // Get IOH Port 9/SLot3 Control state
    if( LNot( LEqual( ATID, 1))) {  // Check if Attention LED is not solid "ON"
      And(Local0, ALMK, Local0)     //  Mask the Attention Indicator Bits
      Or(Local0, ALBL, Local0)      //  Set the Attention Indicator to blink
    }
    HPCC(Local0)                    // issue command

    Store(SCTL, Local0)             // Get IOH Port 9/SLot3 Control state
    Or(Local0, SPOF, Local0)        // Set the Power Controller Control to Power Off
    HPCC(Local0)

    Store(SCTL, Local0)             // Get PEXH Port 9/SLot3 Control state
    Or(Local0, PLOF, Local0)        // Set the Power Indicator to Off.
    HPCC(Local0)
  }  // End of EJ02

  //;************************************************************************;
  //; PM_PME Wake Handler for Slot 3 only
  //;
  //; Input: Arg0 - Slot Number
  //;
  //;************************************************************************;
  Method(PMEH,1){                   // Handler for PCI-E PM_PME Wake Event/Interrupt (GPI xxh)
    If(And(HPEV, SSTS)){            // Check for Hot-Plug Events
      If(ABPS) {
        Store (Arg0, IO80)          // Send slot number to Port 80
        Store(0x1, ABPS)            // Clear the interrupt status
        Sleep(200)                  // delay 200ms
      }
    }
    Return (0xff)                   // Indicate that this controller did not interrupt
  }  // End of Method PMEH

  //;************************************************************************;
  //; Hot-Plug Handler for an IIO PCIe root port slot
  //;
  //; Input: Arg0 - Slot Numnber (not used)
  //; Output: 
  //;    0xFF - No hotplug event detected
  //;    0x03 - Eject Request detected
  //;    0x00 - Device Presence Changed
  //;
  //;************************************************************************;
  Method(HPEH,1){                   // Handler for PCI-E Hot-Plug Event/Interupt Called from \_SB.GPE._L01()

    If (LEqual(Zero,DVPS)) {
       Return (0xff)                // Do nothing if root port is not "Present and Hot-plugable"
    }

    Store(0x22, IO80)
    Sleep(100)
    Store(0,CCIE)                   // Disable command interrupt
    If(And(HPEV, SSTS)){            // Check for Hot-Plug Events
      Store(0x3A, IO80)
      Sleep(10)
      Store(PP3H(0x0), Local0)      // Call the Slot 3 Hot plug Interrupt Handler
      Return(Local0)                // Return PP2H information
    }
    Else{
      Return (0xff)                 // Indicate that this controller did not interrupt
    }
    Store(0x2F, IO80)
    Sleep(10)
  }  // End of Method HPEH

  //;************************************************************************;
  //; Interrut Event Handler
  //;
  //; Input: Arg0 - Slot Numnber
  //;
  //;************************************************************************;
  Method(PP3H,1){                     // Slot 3 Hot plug Interrupt Handler
    //
    // Check for the Atention Button Press, Slot Empty/Presence, Power Controller Control.
    //
    Sleep(200)                        // HW Workaround for AttentionButton Status to stabilise
    If(ABPS) {                        // Check if Attention Button Pressed for Device 4
      If(LNot(PRDS)) {                // See if nothing installed (no card in slot)
        PWSL(0x0)                     // make sure Power is Off
        PWCM(0x3)                     // Set Power Indicator to "OFF"
        //
        // Check for MRL here and set attn indicator accordingly
        //
        If(LEqual(MSSC,MRLS)) {       // Standby power is on - MRL closed
          ATCM(0x2)                   // Set Attention Indicator to "BLINK"
        } else {                      // Standby power is off - MRL open
         ATCM(0x3)                    // set attention indicator "OFF"
        }
        Store(0x0, ABIE)              // set Attention Button Interrupt to disable
        Store(0x1, ABPS)              // Clear the interrupt status
        Sleep(200)                    // delay 200ms
        Return(0xff)                  // Attn Button pressed without card in slot. Do nothing
      }
      //
      // Card is present in slot so....
      //
      Store(0x0, ABIE)                // set Attention Button Interrupt to disable
                                      // Attn Btn Interrupt has to be enabled only after an insert oprn
      Store(0x1, ABPS)                // Clear the interrupt status
      Sleep(200)                      // delay 200ms
      //
      // Check for MRL here - only if SPWR is OFF blink AttnInd and retun 0xff
      //
      If(LNot(LEqual(MSSC,MRLS))) {   // Standby power is off
         PWSL(0x0)                    // make sure Power is Off
         PWCM(0x3)                    // Set Power Indicator to "OFF"
         ATCM(0x2)                    // Set Attention Indicator to "BLINK"
         Return(0xff)                 // Attn Button pressed with card in slot, but MRL open. Do nothing
      }
      //Card Present, if StandbyPwr is ON proceed as below with Eject Sequence
      If(PWCC) {                      // Slot not Powered
        PWCM(0x3)                     // Set Power Indicator to "OFF"
        ATCM(0x2)                     // Set Attention Indicator to "BLINK"
        Return(0xff)                  // Attn Button pressed with card in slot, MRL closed, Slot not powered. Do nothing
      } else {                        // See if Slot is already Powered
        PWCM(0x2)                     // Set power Indicator to BLINK
        Sleep(600)                    // Wait 100ms
        Store(600, Local0)            // set 5 second accumulator to 0
        Store(0x1, ABPS)              // Clear the interrupt status
        Sleep(200)                    // delay 200ms
        While(LNot(ABPS)) {           // check for someone pressing Attention
          Sleep(200)                  // Wait 200ms
          Add(Local0, 200, Local0)
          If(LEqual(5000, Local0)) {  // heck if 5sec has passed without pressing attnetion btn
            Store(0x1, ABPS)          // Clear the interrupt status
            Sleep(200)                // delay 200ms 
            Return (0x3)              // continue with Eject request
          }
        }
        PWCM(0x1)                     // Set power Indicator baCK "ON"
        Store(0x1, ABPS)              // Clear the Attention status
        Sleep(200)                    // delay 200ms
        Store(0x1, ABIE)              // set Attention Button Interrupt to enable
        Return (0xff)                 // do nothing and abort
      }
    }  // End if for the Attention Button Hot Plug Interrupt.

    If(PFDS) {                        // Check if Power Fault Detected
      Store(0x1, PFDS)                // Clear the Power Fault Status
      PWSL(0x0)                       // set Power Off
      PWCM(0x3)                       // set power indicator to OFF
      ATCM(0x1)                       // set attention indicator "ON"
      Return(0x03)            // Eject request.
    }  // End if for the Power Fault Interrupt.

    If(MSCS) {                        // Check interrupt caused by the MRL Sensor
      Store(0x1, MSCS)                // Clear the MRL Status
      If(LEqual(MSSC,MRLS)) {         // Standby power is on - MRL closed
        If(PRDS) {                    // Card is Present
                                      // Slot Power is Off, so power up the slot
        ATCM(0x3)                     // Set Attention Indicator to off
        PWCM(0x2)                     // Set Power Indicator to Blink
        Sleep(600)                    // Wait 100ms
        Store(600, Local0)            // set 5 second accumulator to 0
        Store(0x1, ABPS)              // Clear the interrupt status
        While(LNot(ABPS)) {           // check for someone pressing Attention
          Sleep(200)                  // Wait 200ms
          Add(Local0, 200, Local0)
          If(LEqual(5000, Local0)) {  // Check if 5 sec elapsed
            Store(0x1, ABIE)          // Enable Attention button interrupt
            ATCM(0x3)                 // set attention indicator "OFF"
            PWSL(0x1)                 // Power the Slot
            Sleep(500)                // Wait for .5 Sec for the Power to Stabilize.
            // Check for the Power Fault Detection
            If(LNot(PFDS)) {                    // No Power Fault
              PWCM(0x1)                         // Set Power Indicator to "ON"
              // Or(LVLS, 0x000010000, LVLS)    // Enable the Device 4 Slot Clock (GPIO16)
              // Notify the OS to load the Driver for the card
              Store(0x00, Local1)
              Store(0x1, ABIE)                  // Enable Attention button interrupt
            } Else {                            // Power Fault present
              PWSL(0x0)                         // set Slot Power Off
              PWCM(0x3)                         // set power indicator to OFF
              ATCM(0x1)                         // set attention indicator "ON"
              // And (LVLS, 0x0FFFEFFFF, LVLS)  // Disable the Device 4 Slot Clock (GPIO16)
              Store(0x03, Local1)               // Eject request.
            }                                   // End if for the Slot Power Fault
              Store(0x1, ABPS)                  // Clear the Attention status
              Sleep(200)                        // delay 200ms
              Return(Local1)
            }
          }
          //
          // someone pressed Attention Button
          //
          Store(0x1, ABPS)          // Clear the Attention status
          Sleep(200)                // delay 200ms
          PWSL(0x0)                 // Set Slot Power off
          PWCM(0x3)                 // Set Power Indicator back to "OFF"
          ATCM(02)                  // Set Attention Indicator to "BLINK"
          Return(0xff)              // leave it off
          // End of Insert sequence
        }
        //MRL is closed, Card is not present
        PWSL(0x0)                   // Set Slot Power off
        PWCM(0x3)                   // Set Power Indicator back to "OFF"
        ATCM(02)                    // Set Attention Indicator to "BLINK"
        Return(0xff)                // leave it off
      } else {                      // MRL is open i.e Stdby power is turned off
        If(PRDS) {
          //card present MRL switched off
          ATCM(0x2)                 // Set Attention Indicator to "BLINK"
          If(Lnot(PWCC)) {          // If slot is powered
            // This event is not supported and someone has opened the MRL and dumped the power
            //  on the slot with possible pending transactions. This could hose the OS.
            // Try to Notify the OS to unload the drivers.
            PWSL(0x0)               // Set Slot Power off
            PWCM(0x3)               // Set Power Indicator back to "OFF"
            Return(0x03)            // Eject request.
          } else {                  // Slot not powered, MRL is opened, card still in slot - Eject not fully complete
            Return(0xFF)
          }
        }
        //no card present and Stdby power switched off, turn AI off
        ATCM(0x3)                   // Set Attention Indicator to "OFF"
        Return(0xff)                // leave it off
      }                             // End of MRL switch open/close state
    }                               // End of MRL Sensor State Change

    If(PDCS) {                      // Check if Presence Detect Changed Status
      Store(0x1, PDCS)              // Clear the Presence Detect Changed Status
      If(LNot(PRDS)) {              // Slot is Empty
        PWSL(0x0)                   // Set Slot Power "OFF"
        PWCM(0x3)                   // set power indicator to "OFF"
        If(LEqual(MSSC,MRLS)) {     // If Standby power is on
          ATCM(0x2)                 // Set Attention Indicator to "Blink"
        } else {
          ATCM(0x3)                 // Set Attention Indicator to "OFF"
        }
        Return(0xFF)                // Do nothing
      } Else {                      // Slot Card is inserted
        // Irrespective of MRL state blink indicator
        PWSL(0x0)                   // Set Slot Power off
        PWCM(0x3)                   // Set Power Indicator back to "OFF"
        ATCM(0x2)                   // Set Attention Indicator to "Blink"
        Return(0xFF)                // Do nothing
      }
    }  // End if for the Presence Detect Changed Hot Plug Interrupt.
    Return(0xff)    // should not get here, but do device check if it does.
  }  // End of method PP5H
  //
  // End of hotplug code
  //

  Device(H000) {
    Name(_ADR, 0x00000000)
    Name(_SUN, 0x0002)              // Slot User Number
    Method(_EJ0, 1) { EJ02(Arg0) }  // Remove all power from the slot
  }
  Device(H001) {
    Name(_ADR, 0x00000001)
    Name(_SUN, 0x0002)              // Slot User Number
    Method(_EJ0, 1) { EJ02(Arg0) }  // Remove all power from the slot
  }
  Device(H002) {
    Name(_ADR, 0x00000002)
    Name(_SUN, 0x0002)              // Slot User Number
    Method(_EJ0, 1) { EJ02(Arg0) }  // Remove all power from the slot
  }
  Device(H003) {
    Name(_ADR, 0x00000003)
    Name(_SUN, 0x0002)              // Slot User Number
    Method(_EJ0, 1) { EJ02(Arg0) }  // Remove all power from the slot
  }
      Device(H004) {
    Name(_ADR, 0x00000004)
    Name(_SUN, 0x0002)              // Slot User Number
    Method(_EJ0, 1) { EJ02(Arg0) }  // Remove all power from the slot
  }
  Device(H005) {
    Name(_ADR, 0x00000005)
    Name(_SUN, 0x0002)              // Slot User Number
    Method(_EJ0, 1) { EJ02(Arg0) }  // Remove all power from the slot
  }
  Device(H006) {
    Name(_ADR, 0x00000006)
    Name(_SUN, 0x0002)              // Slot User Number
    Method(_EJ0, 1) { EJ02(Arg0) }  // Remove all power from the slot
  }
  Device(H007) {
    Name(_ADR, 0x00000007)
    Name(_SUN, 0x0002)              // Slot User Number
    Method(_EJ0, 1) { EJ02(Arg0) }  // Remove all power from the slot
  }
