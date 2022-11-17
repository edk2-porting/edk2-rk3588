/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#define SERIAL_IO_PMECTRLSTATUS 0x84

#define SERIAL_IO_DISABLED 0
#define SERIAL_IO_ACPI 1
#define SERIAL_IO_PCI 2
#define SERIAL_IO_ACPIHIDDEN 3
#define SERIAL_IO_LEGACY_UART 4

#define KBL_GENERATION 2

#define SERIAL_IO_SPT_LP_GPIO_HID "INT344B"
#define SERIAL_IO_SPT_H_GPIO_HID  "INT345D"
#define SERIAL_IO_KBL_H_GPIO_HID  "INT3451"

#define SERIAL_IO_I2C0_HID "INT3442"
#define SERIAL_IO_I2C1_HID "INT3443"
#define SERIAL_IO_I2C2_HID "INT3444"
#define SERIAL_IO_I2C3_HID "INT3445"
#define SERIAL_IO_I2C4_HID "INT3446"
#define SERIAL_IO_I2C5_HID "INT3447"
#define SERIAL_IO_SPI0_HID "INT3440"
#define SERIAL_IO_SPI1_HID "INT3441"
#define SERIAL_IO_UART0_HID "INT3448"
#define SERIAL_IO_UART1_HID "INT3449"
#define SERIAL_IO_UART2_HID "INT344A"

#define SERIAL_IO_I2C0_ADR 0x00150000
#define SERIAL_IO_I2C1_ADR 0x00150001
#define SERIAL_IO_I2C2_ADR 0x00150002
#define SERIAL_IO_I2C3_ADR 0x00150003
#define SERIAL_IO_I2C4_ADR 0x00190002
#define SERIAL_IO_I2C5_ADR 0x00190001
#define SERIAL_IO_SPI0_ADR 0x001E0002
#define SERIAL_IO_SPI1_ADR 0x001E0003
#define SERIAL_IO_UART0_ADR 0x001E0000
#define SERIAL_IO_UART1_ADR 0x001E0001
#define SERIAL_IO_UART2_ADR 0x00190000

Scope(\_SB.PCI0) {


  //
  //  Common code for D3 entry for all SerialIo devices including PCI mode
  //
  //  @param[in]  Arg0      PCI calculated address or ACPI BAR1 address of device to be put in D3
  //
  Method (LPD3, 1, Serialized) {
    OperationRegion(ICB1, SystemMemory, Add(Arg0,SERIAL_IO_PMECTRLSTATUS), 4)
    Field(ICB1, DWordAcc, NoLock, Preserve) { PMEC, 32 } //Note: SerialIO Does Not Support PME
    Store(0x3, PMEC) // Set BIT[1:0] = 11b - Power State D3
    Store(PMEC,TEMP) // perform a read to avoid ordering and noncoherency problems
  }

  //
  //  Common code for D0 entry for all SerialIo devices including PCI mode
  //
  //  @param[in]  Arg0      PCI calculated address or ACPI BAR1 address of device to be put in D0
  //
  Method (LPD0, 1, Serialized) {
    OperationRegion(ICB1, SystemMemory, Add(Arg0,SERIAL_IO_PMECTRLSTATUS), 4)
    Field(ICB1, DWordAcc, NoLock, Preserve) { PMEC, 32 } //Note: SerialIO Does Not Support PME
    And(PMEC, 0xFFFF7FFC, PMEC) // Set BIT[1:0] = 00b - Power State D0
    Store(PMEC,TEMP) // perform a read to avoid ordering and noncoherency problems
  }

  //
  //  Common code for retrieving hardware revision for all SerialIo devices
  //
  //  @param[in]  Arg0      BAR1 of device
  //
  //  @retval               Hardware Revison
  //
  Method (LHRV, 1, Serialized) {
    OperationRegion(ICB1, SystemMemory, Add(Arg0,0x8) , 4)
    Field(ICB1, DWordAcc, NoLock, Preserve) { HRV, 8 }
    Return(HRV)
  }

  //
  //  Get D state of SerialIo device - for use in _PSC method
  //
  //  @param[in]  Arg0      PCI calculated address or ACPI BAR1 of device
  //
  //  @retval               D State
  //
  Method (GETD, 1, Serialized) {
    OperationRegion(ICB1, SystemMemory, Add(Arg0,SERIAL_IO_PMECTRLSTATUS) , 4)
    Field(ICB1, DWordAcc, NoLock, Preserve) { PMEC, 32 }
    Return (And(PMEC, 0x3))
  }

  //
  //  Common code for reporting memory and interrupt resources for all SerialIo devices
  //
  //  @param[in]  Arg0      Device mode
  //  @param[in]  Arg1      Memory address
  //  @param[in]  Arg2      Interrupt number
  //
  //  @retval               Resource buffer
  //
  Method (LCRS, 3, Serialized) {
    Name (RBUF, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt(ResourceConsumer, Level, ActiveLow, Shared, , , SIRQ) {20}
    })
    CreateDWordField(RBUF,BAR0._BAS,BVAL)
    CreateDWordField(RBUF,BAR0._LEN,BLEN)
    CreateDWordField(RBUF,SIRQ._INT,IRQN)
    Store (Arg1, BVAL)
    Store (Arg2, IRQN)
    If(LEqual(Arg0,4)) {Store(8, BLEN)}
    Return (RBUF)
  }

  //
  //  Common code for reporting status of all SerialIo devices
  //
  //  @param[in]  Arg0      Device's mode
  //
  //  @retval               0 (do not show device) or 0xF (device is present)
  //
  Method (LSTA, 1, Serialized) {
    If(LOr(LEqual(Arg0,0),LEqual(Arg0,3)))  { Return(0) }
    If(LLess(OSYS,2012)) { Return(0) } // OS older than Win8 = not supported
    Return (0xF)
  }

  //
  //  Converts driver-visible GPIO number to interrupt number
  //
  //  @param[in]  Arg0      BAR1 of device
  //
  //  @retval               Interrupt number
  //
  Method (GIRQ,1,Serialized) {
    Return (Add(24,Mod(Arg0,96)))
  }

} // END Scope(\_SB.PCI0)



Scope(\_SB.PCI0) {
  //
  //  An invisible device that claims memory resources reserved for SerialIo devices
  //  SerialIo devices in ACPI mode can't claim their BAR1 resources because these are aliases for PciCfg space
  //
  Device(SIRC) {
    Name(_HID,EISAID("PNP0C02"))
    Name(_STA,0x3) // device present and decodes its resources, but not to be displayed in OSPM
    Name(_UID,5)

    //
    //  Returns resource buffer with memory ranges used but not explicitely claimed by the device:
    //  in ACPI mode (SMDn=1) returns BAR1. In HIDDEN mode (SMDn=3) returns both BARs.
    //  In LegacyUART mode(SMDn=4) returns BAR1+ BAR0 except for first 8 bytes
    //
    //  @param[in]  Arg0      SMDn
    //  @param[in]  Arg1      SB0n
    //  @param[in]  Arg2      SB1n
    //
    //  @retval               Resource buffer with memory ranges
    //                        if SMDn=0 (SERIAL_IO_DISABLED) or SMDn=2 (SERIAL_IO_PCI) returns an empty buffer
    //
    Method (ADDB, 3, Serialized) {

      Name(BUFF,ResourceTemplate() { Memory32Fixed (ReadWrite, 0, 0x1000, BUF) })
      CreateDWordField(BUFF, BUF._BAS, ADDR)
      CreateDWordField(BUFF, BUF._LEN, LENG)

      Store(ResourceTemplate() { }, Local0)  // an empty buffer

      If(LOr(LOr(LEqual(Arg0, SERIAL_IO_ACPI), LEqual(Arg0, SERIAL_IO_ACPIHIDDEN)), LEqual(Arg0, SERIAL_IO_LEGACY_UART))) {
        Store(Arg2, ADDR)
        ConcatenateResTemplate(Local0, BUFF, Local1) // add BAR1
        Store(Local1, Local0)
      }

      If(LEqual(Arg0,SERIAL_IO_ACPIHIDDEN)) {
        Store(Arg1,ADDR)
        ConcatenateResTemplate(Local0, BUFF, Local1) // add BAR0
        Store(Local1, Local0)
      }

      If(LEqual(Arg0, SERIAL_IO_LEGACY_UART)) {
        Store(Add(8, Arg1), ADDR)
        Store(0xFF8, LENG)
        ConcatenateResTemplate(Local0, BUFF, Local1) // add BAR0 without 8 bytes
        Store(Local1, Local0)
      }
      Return (Local0)
    }

    Method (_CRS, 0x0, Serialized) {
      Store(ResourceTemplate() { }, Local0)  // placeholder for concatenated buffers
      //
      // SB1[X] if device is configured as ACPI represents BAR1
      //        in case of PCI it stands for memory mapped BDF
      //
      ConcatenateResTemplate(Local0, ADDB(SMD0,SB00,SB10), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMD1,SB01,SB11), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMD2,SB02,SB12), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMD3,SB03,SB13), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMD4,SB04,SB14), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMD5,SB05,SB15), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMD6,SB06,SB16), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMD7,SB07,SB17), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMD8,SB08,SB18), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMD9,SB09,SB19), Local1)
      Store(Local1, Local0)
      ConcatenateResTemplate(Local0, ADDB(SMDA,SB0A,SB1A), Local1)
      Store(Local1, Local0)
      //
      //  GPIO device always exists in chipset. If it's not visible in ACPI
      //  and doesn't claim its resources, declare it here to prevent OS from reusing its mem addresses
      //
      If(LEqual(\_SB.PCI0.GPI0._STA(),0)) {
        ConcatenateResTemplate(Local0, \_SB.PCI0.GPI0._CRS(), Local1)
        Store(Local1, Local0)
      }
      Return (Local0)
    } // END _CRS
  } // END Device(SIRC)

  //----------------------------
  //  Serial IO GPIO Controller
  //----------------------------
  Device (GPI0)
  {
    Method (_HID) {
      // Return HID based on PCH Product
      If (LEqual(PCHV, SPTH)) {
        If (LEqual(PCHG, KBL_GENERATION)) { Return(SERIAL_IO_KBL_H_GPIO_HID) }
        Return(SERIAL_IO_SPT_H_GPIO_HID)
      }
      Return(SERIAL_IO_SPT_LP_GPIO_HID)
    }

    Name (LINK,"\\_SB.PCI0.GPI0")

    Method (_CRS, 0x0, NotSerialized) {
      Name (RBUF, ResourceTemplate () {
        Memory32Fixed (ReadWrite, 0x00000000, 0x00010000, BAR0)
        Memory32Fixed (ReadWrite, 0x00000000, 0x00010000, BAR1)
        Memory32Fixed (ReadWrite, 0x00000000, 0x00010000, BAR3)
        Interrupt (ResourceConsumer, Level, ActiveLow, Shared, , , IRQ) { 14 } //Interrupt IRQ_EN
      })
      CreateDWordField(RBUF,BAR0._BAS,COM0)
      CreateDWordField(RBUF,BAR1._BAS,COM1)
      CreateDWordField(RBUF,IRQ._INT,IRQN)
      Store( Add(SBRG,PCH_GPIO_COM0), COM0)
      Store( Add(SBRG,PCH_GPIO_COM1), COM1)
      CreateDWordField(RBUF,BAR3._BAS,COM3)
      Store( Add(SBRG,PCH_GPIO_COM3), COM3)
      Store(SGIR,IRQN)
      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized) {
      If(LEqual(SBRG, 0))   { Return(0x0) } // has BAR for this device been programmed by the OS?
      If(LEqual(GPEN, 0))   { Return(0x0) } // GPIO disabled in platform policy
      Return(0xF)
    }
  }

} // END Scope(\_SB.PCI0)


//------------------------------------------
//  Serial IO I2C Controller 0 Configuration
//------------------------------------------
Scope(\_SB.PCI0) {
  Device (I2C0) {
    Name (LINK,"\\_SB.PCI0.I2C0") // Support for Windows 7
    //
    // I2C0 PCI/ACPI PS0 PS3 and PSC
    //
    Method (_PSC) {
      Return (GETD(SB10))
    }
    Method (_PS0) {
      LPD0(SB10)
    }
    Method (_PS3) {
      LPD3(SB10)
    }

    //
    // Apllies for I2C0 all modes except PCI
    //
    If(LNotEqual(SMD0,SERIAL_IO_PCI)) {
      Name (_HID, SERIAL_IO_I2C0_HID)
      Method (_HRV) { Return (LHRV(SB10)) }
      Method (_CRS) { Return (LCRS(SMD0,SB00,SIR0)) }
      Method (_STA) { Return (LSTA(SMD0)) }
    }

    //
    // Apllies for I2C0 PCI mode only
    //
    If(LEqual(SMD0,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_I2C0_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
  }

  } // Device (I2C0)
} // Scope(\_SB.PCI0)

//------------------------------------------
//  Serial IO I2C Controller 1 Configuration
//------------------------------------------
Scope(\_SB.PCI0) {
  Device (I2C1) {
    Name (LINK,"\\_SB.PCI0.I2C1") // Support for Windows 7
    //
    // I2C1 PCI/ACPI PS0 PS3 and PSC
    //
    Method (_PSC) {
      Return (GETD(SB11))
    }
    Method (_PS0) {
      LPD0(SB11)
    }
    Method (_PS3) {
      LPD3(SB11)
    }

    //
    // Apllies for I2C1 all modes except PCI
    //
    If(LNotEqual(SMD1,SERIAL_IO_PCI)) {
      Name (_HID, SERIAL_IO_I2C1_HID)
      Method (_HRV) { Return (LHRV(SB11)) }
      Method (_CRS) { Return (LCRS(SMD1,SB01,SIR1)) }
      Method (_STA) { Return (LSTA(SMD1)) }
    }

    //
    // Apllies for I2C1 PCI mode only
    //
    If(LEqual(SMD1,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_I2C1_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

  } // Device (I2C1)
} // Scope(\_SB.PCI0)

//------------------------------------------
//  Serial IO I2C Controller 2 Configuration
//------------------------------------------
Scope(\_SB.PCI0) {
  Device (I2C2) {
    Name (LINK,"\\_SB.PCI0.I2C2") // Support for Windows 7
    //
    // I2C2 PCI/ACPI PS0 PS3 and PSC
    //
    Method (_PSC) {
      Return (GETD(SB12))
    }
    Method (_PS0) {
      LPD0(SB12)
    }
    Method (_PS3) {
      LPD3(SB12)
    }

    //
    // Apllies for I2C2 all modes except PCI
    //
    If(LNotEqual(SMD2,SERIAL_IO_PCI)) {
      Name (_HID, SERIAL_IO_I2C2_HID)
      Method (_HRV) { Return (LHRV(SB12)) }
      Method (_CRS) { Return (LCRS(SMD2,SB02,SIR2)) }
      Method (_STA) { Return (LSTA(SMD2)) }
    }

    //
    // Apllies for I2C2 PCI mode only
    //
    If(LEqual(SMD2,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_I2C2_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

  } // Device (I2C2)
} // Scope(\_SB.PCI0)

//------------------------------------------
//  Serial IO I2C Controller 3 Configuration
//------------------------------------------
Scope(\_SB.PCI0) {
  Device (I2C3) {
    Name (LINK,"\\_SB.PCI0.I2C3") // Windows 7 W/A
    //
    // I2C3 PCI/ACPI PS0 PS3 and PSC
    //
    Method (_PSC) {
      Return (GETD(SB13))
    }
    Method (_PS0) {
      LPD0(SB13)
    }
    Method (_PS3) {
      LPD3(SB13)
    }

    //
    // Apllies for I2C3 all modes except PCI
    //
    If(LNotEqual(SMD3,SERIAL_IO_PCI)) {
      Name (_HID, SERIAL_IO_I2C3_HID)
      Method (_HRV) { Return (LHRV(SB13)) }
      Method (_CRS) { Return (LCRS(SMD3,SB03,SIR3)) }
      Method (_STA) { Return (LSTA(SMD3)) }
    }

    //
    // Apllies for I2C3 PCI mode only
    //
    If(LEqual(SMD3,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_I2C3_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

  } // Device (I2C3)
} // Scope(\_SB.PCI0)

//------------------------------------------
//  Serial IO I2C Controller 4 Configuration
//------------------------------------------
Scope(\_SB.PCI0) {
  Device (I2C4) {
    Name (LINK,"\\_SB.PCI0.I2C4") // Support for Windows 7
    //
    // I2C4 PCI/ACPI PS0 PS3 and PSC
    //
    Method (_PSC) {
      Return (GETD(SB14))
    }
    Method (_PS0) {
      LPD0(SB14)
    }
    Method (_PS3) {
      LPD3(SB14)
    }

    //
    // Apllies for I2C4 all modes except PCI
    //
    If(LNotEqual(SMD4,SERIAL_IO_PCI)) {
      Name (_HID, SERIAL_IO_I2C4_HID)
      Method (_HRV) { Return (LHRV(SB14)) }
      Method (_CRS) { Return (LCRS(SMD4,SB04,SIR4)) }
      Method (_STA) { Return (LSTA(SMD4)) }
    }

    //
    // Apllies for I2C4 PCI mode only
    //
    If(LEqual(SMD4,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_I2C4_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

  } // Device (I2C4)
} // Scope(\_SB.PCI0)

//------------------------------------------
//  Serial IO I2C Controller 5 Configuration
//------------------------------------------
Scope(\_SB.PCI0) {
  Device (I2C5) {
    Name (LINK,"\\_SB.PCI0.I2C5") // Support for Windows 7
    //
    // I2C5 PCI/ACPI PS0 PS3 and PSC
    //
    Method (_PSC) {
      Return (GETD(SB15))
    }
    Method (_PS0) {
      LPD0(SB15)
    }
    Method (_PS3) {
      LPD3(SB15)
    }

    //
    // Apllies for I2C5 all modes except PCI
    //
    If(LNotEqual(SMD5,SERIAL_IO_PCI)) {
      Name (_HID, SERIAL_IO_I2C5_HID)
      Method (_HRV) { Return (LHRV(SB15)) }
      Method (_CRS) { Return (LCRS(SMD5,SB05,SIR5)) }
      Method (_STA) { Return (LSTA(SMD5)) }
    }

    //
    // Apllies for I2C5 PCI mode only
    //
    If(LEqual(SMD5,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_I2C5_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

  } // Device (I2C5)
} // Scope(\_SB.PCI0)

//-------------------------------------------
//  Serial IO SPI Controller 0 Configuration
//-------------------------------------------
Scope(\_SB.PCI0) {
  Device (SPI0) {
    //
    // SPI0 PCI/ACPI PS0 PS3 and PSC
    //
    Method (_PSC) {
      Return (GETD(SB16))
    }
    Method (_PS0) {
      LPD0(SB16)
    }
    Method (_PS3) {
      LPD3(SB16)
    }

    //
    // Apllies for SPI0 all modes except PCI
    //
    If(LNotEqual(SMD6,SERIAL_IO_PCI)) {
      Name (_HID, SERIAL_IO_SPI0_HID)
      Method (_HRV) { Return (LHRV(SB16)) }
      Method (_CRS) { Return (LCRS(SMD6,SB06,SIR6)) }
      Method (_STA) { Return (LSTA(SMD6)) }
    }

    //
    // Apllies for SPI0 PCI mode only
    //
    If(LEqual(SMD6,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_SPI0_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

  } // Device (SPI0)
} // Scope(\_SB.PCI0)

//------------------------------------------
//  Serial IO SPI Controller 1 Configuration
//------------------------------------------
Scope(\_SB.PCI0) {
  Device (SPI1) {
    //
    // SPI1 PCI/ACPI PS0 PS3 and PSC
    //
    Method (_PSC) {
      Return (GETD (SB17))
    }
    Method (_PS0) {
      LPD0(SB17)
    }
    Method (_PS3) {
      LPD3(SB17)
    }

    //
    // Apllies for SPI0 all modes except PCI
    //
    If(LNotEqual(SMD7,SERIAL_IO_PCI)) {
      Name (_HID, SERIAL_IO_SPI1_HID)
      Method (_HRV) { Return (LHRV(SB17)) }
      Method (_CRS) { Return (LCRS(SMD7,SB07,SIR7)) }
      Method (_STA) { Return (LSTA(SMD7)) }
    }

    //
    // Apllies for SPI1 PCI mode only
    //
    If(LEqual(SMD7,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_SPI1_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

  } // Device (SPI1)
} // Scope(\_SB.PCI0)

//-------------------------------------------
//  Serial IO UART Controller 0 Configuration
//-------------------------------------------
Scope(\_SB.PCI0) {
  Device (UA00) {

    //
    // Apllies for UART0 all modes except PCI
    //
    If(LNotEqual(SMD8,SERIAL_IO_PCI)) {
      Method(_HID) {
        If(LEqual(SMD8,SERIAL_IO_LEGACY_UART)) { Return (EISAID("PNP0C02")) }
        Else { Return (SERIAL_IO_UART0_HID) }
      }
      Name (_UID, "SerialIoUart0")
      Name (_DDN, "SerialIoUart0")
      Method (_HRV) { Return (LHRV(SB18)) }
      Method (_CRS) { Return (LCRS(SMD8,SB08,SIR8)) }
      Method (_STA) { Return (LSTA(SMD8)) }
    }

    //
    // Apllies for UART0 PCI mode only
    //
    If(LEqual(SMD8,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_UART0_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

    //
    // UART0 PCI/ACPI PS0 PS3 and PSC
    //
    If(LNotEqual(SMD8,SERIAL_IO_LEGACY_UART)) {
      Method (_PSC) {
        Return (GETD (SB18))
      }
      Method (_PS0) {
        LPD0(SB18)
      }
      Method (_PS3) {
        LPD3(SB18)
      }
    }

  } // Device (UA00)
} // Scope(\_SB.PCI0)

//-------------------------------------------
//  Serial IO UART Controller 1 Configuration
//-------------------------------------------
Scope(\_SB.PCI0) {
  Device (UA01) {

    //
    // Apllies for UART1 all modes except PCI
    //
    If(LNotEqual(SMD9,SERIAL_IO_PCI)) {
      Method(_HID) {
        If(LEqual(SMD9,SERIAL_IO_LEGACY_UART)) { Return (EISAID("PNP0C02")) }
        Else { Return (SERIAL_IO_UART1_HID) }
      }
      Name (_UID, "SerialIoUart1")
      Name (_DDN, "SerialIoUart1")
      Method (_HRV) { Return (LHRV(SB19)) }
      Method (_CRS) { Return (LCRS(SMD9,SB09,SIR9)) }
      Method (_STA) { Return (LSTA(SMD9)) }
    }

    //
    // Apllies for UART1 PCI mode only
    //
    If(LEqual(SMD9,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_UART1_ADR)
      Method (_DSM,4,Serialized){If(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

    //
    // UART1 PCI/ACPI PS0 PS3 and PSC
    //
    If(LNotEqual(SMD9,SERIAL_IO_LEGACY_UART)) {
      Method (_PSC) {
        Return (GETD (SB19))
      }
      Method (_PS0) {
        LPD0(SB19)
      }
      Method (_PS3) {
        LPD3(SB19)
      }
    }

  } // Device (UA01)
} // Scope(\_SB.PCI0)

//-------------------------------------------
//  Serial IO UART Controller 2 Configuration
//-------------------------------------------
Scope(\_SB.PCI0) {
  Device (UA02) {

    //
    // Apllies for UART2 all modes except PCI
    //
    If(LNotEqual(SMDA,SERIAL_IO_PCI)) {
      Method (_HID) {
        If(LEqual(SMDA,SERIAL_IO_LEGACY_UART)) { Return (EISAID("PNP0C02")) }
        Else { Return (SERIAL_IO_UART2_HID) }
      }
      Name (_UID, "SerialIoUart2")
      Name (_DDN, "SerialIoUart2")
      Method (_HRV) { Return (LHRV(SB1A)) }
      Method (_CRS) { Return (LCRS(SMDA,SB0A,SIRA)) }
      Method (_STA) { Return (LSTA(SMDA)) }
    }

    //
    // Apllies for UART2 PCI mode only
    //
    If(LEqual(SMDA,SERIAL_IO_PCI)) {
      Name (_ADR, SERIAL_IO_UART2_ADR)
      Method (_DSM,4,Serialized){if(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
    }

    //
    // UART2 PCI/ACPI PS0 PS3 and PSC excluding Legacy Uart
    //
    If(LNotEqual(SMDA,SERIAL_IO_LEGACY_UART)) {
      Method (_PSC) {
        Return (GETD(SB1A))
      }
      Method (_PS0) {
        LPD0(SB1A)
      }
      Method (_PS3) {
        LPD3(SB1A)
      }
    }

  } // Device (UA02)
} // Scope(\_SB.PCI0)

