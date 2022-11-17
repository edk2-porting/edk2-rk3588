/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
// GPIO Access Library
//
Scope(\_SB)
{

#define PCH_LP_PKG_INDEX 0
#define PCH_H_PKG_INDEX  1

  //
  // GPIO Community address for each group
  //
  Name(GCOM, Package(){
    Package(){ //SPT-LP
      PCH_GPIO_COM0, //SPT-LP GPP_A
      PCH_GPIO_COM0, //SPT-LP GPP_B
      PCH_GPIO_COM1, //SPT-LP GPP_C
      PCH_GPIO_COM1, //SPT-LP GPP_D
      PCH_GPIO_COM1, //SPT-LP GPP_E
      PCH_GPIO_COM3, //SPT-LP GPP_F
      PCH_GPIO_COM3, //SPT-LP GPP_G
      PCH_GPIO_COM2, //SPT-LP GPD
    },
    Package(){ //SPT-H
      PCH_GPIO_COM0, //SPT-H GPP_A
      PCH_GPIO_COM0, //SPT-H GPP_B
      PCH_GPIO_COM1, //SPT-H GPP_C
      PCH_GPIO_COM1, //SPT-H GPP_D
      PCH_GPIO_COM1, //SPT-H GPP_E
      PCH_GPIO_COM1, //SPT-H GPP_F
      PCH_GPIO_COM1, //SPT-H GPP_G
      PCH_GPIO_COM1, //SPT-H GPP_H
      PCH_GPIO_COM3, //SPT-H GPP_I
      PCH_GPIO_COM2  //SPT-H GPD
    }
  })

  //
  // Number of GPIO pads per group
  //
  Name(GPPG, Package(){
    Package(){ //SPT-LP
      V_PCH_GPIO_GPP_A_PAD_MAX,    // SPT-LP GPP_A
      V_PCH_GPIO_GPP_B_PAD_MAX,    // SPT-LP GPP_B
      V_PCH_GPIO_GPP_C_PAD_MAX,    // SPT-LP GPP_C
      V_PCH_GPIO_GPP_D_PAD_MAX,    // SPT-LP GPP_D
      V_PCH_LP_GPIO_GPP_E_PAD_MAX, // SPT-LP GPP_E
      V_PCH_GPIO_GPP_F_PAD_MAX,    // SPT-LP GPP_F
      V_PCH_LP_GPIO_GPP_G_PAD_MAX, // SPT-LP GPP_G
      V_PCH_GPIO_GPD_PAD_MAX       // SPT-LP GPD
    },
    Package(){ //SPT-H
      V_PCH_GPIO_GPP_A_PAD_MAX,   // SPT-H GPP_A
      V_PCH_GPIO_GPP_B_PAD_MAX,   // SPT-H GPP_B
      V_PCH_GPIO_GPP_C_PAD_MAX,   // SPT-H GPP_C
      V_PCH_GPIO_GPP_D_PAD_MAX,   // SPT-H GPP_D
      V_PCH_H_GPIO_GPP_E_PAD_MAX, // SPT-H GPP_E
      V_PCH_GPIO_GPP_F_PAD_MAX,   // SPT-H GPP_F
      V_PCH_H_GPIO_GPP_G_PAD_MAX, // SPT-H GPP_G
      V_PCH_H_GPIO_GPP_H_PAD_MAX, // SPT_H GPP_H
      V_PCH_H_GPIO_GPP_I_PAD_MAX, // SPT_H GPP_I
      V_PCH_GPIO_GPD_PAD_MAX      // SPT-H GPD
    }
  })

  //
  // GPIO Pad Configuration offset
  //
  Name(PCFG, Package(){
    Package(){ //SPT-LP
      R_PCH_PCR_GPIO_GPP_A_PADCFG_OFFSET,    // SPT-LP GPP_A
      R_PCH_PCR_GPIO_GPP_B_PADCFG_OFFSET,    // SPT-LP GPP_B
      R_PCH_PCR_GPIO_GPP_C_PADCFG_OFFSET,    // SPT-LP GPP_C
      R_PCH_PCR_GPIO_GPP_D_PADCFG_OFFSET,    // SPT-LP GPP_D
      R_PCH_PCR_GPIO_GPP_E_PADCFG_OFFSET,    // SPT-LP GPP_E
      R_PCH_LP_PCR_GPIO_GPP_F_PADCFG_OFFSET, // SPT-LP GPP_F
      R_PCH_LP_PCR_GPIO_GPP_G_PADCFG_OFFSET, // SPT-LP GPP_G
      R_PCH_PCR_GPIO_GPD_PADCFG_OFFSET       // SPT-LP GPD
    },
    Package(){ //SPT-H
      R_PCH_PCR_GPIO_GPP_A_PADCFG_OFFSET,    // SPT-H GPP_A
      R_PCH_PCR_GPIO_GPP_B_PADCFG_OFFSET,    // SPT-H GPP_B
      R_PCH_PCR_GPIO_GPP_C_PADCFG_OFFSET,    // SPT-H GPP_C
      R_PCH_PCR_GPIO_GPP_D_PADCFG_OFFSET,    // SPT-H GPP_D
      R_PCH_PCR_GPIO_GPP_E_PADCFG_OFFSET,    // SPT-H GPP_E
      R_PCH_H_PCR_GPIO_GPP_F_PADCFG_OFFSET,  // SPT-H GPP_F
      R_PCH_H_PCR_GPIO_GPP_G_PADCFG_OFFSET,  // SPT-H GPP_G
      R_PCH_H_PCR_GPIO_GPP_H_PADCFG_OFFSET,  // SPT_H GPP_H
      R_PCH_H_PCR_GPIO_GPP_I_PADCFG_OFFSET,  // SPT_H GPP_I
      R_PCH_PCR_GPIO_GPD_PADCFG_OFFSET       // SPT-H GPD
    }
  })

  //
  // GPIO Host Software Pad Ownership offset
  //
  Name(HOWN, Package(){
    Package(){ //SPT-LP
      R_PCH_PCR_GPIO_GPP_A_HOSTSW_OWN,    // SPT-LP GPP_A
      R_PCH_PCR_GPIO_GPP_B_HOSTSW_OWN,    // SPT-LP GPP_B
      R_PCH_PCR_GPIO_GPP_C_HOSTSW_OWN,    // SPT-LP GPP_C
      R_PCH_PCR_GPIO_GPP_D_HOSTSW_OWN,    // SPT-LP GPP_D
      R_PCH_PCR_GPIO_GPP_E_HOSTSW_OWN,    // SPT-LP GPP_E
      R_PCH_LP_PCR_GPIO_GPP_F_HOSTSW_OWN, // SPT-LP GPP_F
      R_PCH_LP_PCR_GPIO_GPP_G_HOSTSW_OWN, // SPT-LP GPP_G
      R_PCH_PCR_GPIO_GPD_HOSTSW_OWN       // SPT-LP GPD
    },
    Package(){ //SPT-H
      R_PCH_PCR_GPIO_GPP_A_HOSTSW_OWN,   // SPT-H GPP_A
      R_PCH_PCR_GPIO_GPP_B_HOSTSW_OWN,   // SPT-H GPP_B
      R_PCH_PCR_GPIO_GPP_C_HOSTSW_OWN,   // SPT-H GPP_C
      R_PCH_PCR_GPIO_GPP_D_HOSTSW_OWN,   // SPT-H GPP_D
      R_PCH_PCR_GPIO_GPP_E_HOSTSW_OWN,   // SPT-H GPP_E
      R_PCH_H_PCR_GPIO_GPP_F_HOSTSW_OWN, // SPT-H GPP_F
      R_PCH_H_PCR_GPIO_GPP_G_HOSTSW_OWN, // SPT-H GPP_G
      R_PCH_H_PCR_GPIO_GPP_H_HOSTSW_OWN, // SPT-H GPP_H
      R_PCH_H_PCR_GPIO_GPP_I_HOSTSW_OWN, // SPT-H GPP_I
      R_PCH_PCR_GPIO_GPD_HOSTSW_OWN      // SPT-H GPD
    }
  })

  //
  // GPIO Pad Ownership offset
  //
  Name(POWN, Package(){
    Package(){ //SPT-LP
      R_PCH_LP_PCR_GPIO_GPP_A_PAD_OWN, // SPT-LP GPP_A
      R_PCH_LP_PCR_GPIO_GPP_B_PAD_OWN, // SPT-LP GPP_B
      R_PCH_LP_PCR_GPIO_GPP_C_PAD_OWN, // SPT-LP GPP_C
      R_PCH_LP_PCR_GPIO_GPP_D_PAD_OWN, // SPT-LP GPP_D
      R_PCH_LP_PCR_GPIO_GPP_E_PAD_OWN, // SPT-LP GPP_E
      R_PCH_LP_PCR_GPIO_GPP_F_PAD_OWN, // SPT-LP GPP_F
      R_PCH_LP_PCR_GPIO_GPP_G_PAD_OWN, // SPT-LP GPP_G
      R_PCH_LP_PCR_GPIO_GPD_PAD_OWN    // SPT-LP GPD
    },
    Package(){ //SPT-H
      R_PCH_H_PCR_GPIO_GPP_A_PAD_OWN,  // SPT-H GPP_A
      R_PCH_H_PCR_GPIO_GPP_B_PAD_OWN,  // SPT-H GPP_B
      R_PCH_H_PCR_GPIO_GPP_C_PAD_OWN,  // SPT-H GPP_C
      R_PCH_H_PCR_GPIO_GPP_D_PAD_OWN,  // SPT-H GPP_D
      R_PCH_H_PCR_GPIO_GPP_E_PAD_OWN,  // SPT-H GPP_E
      R_PCH_H_PCR_GPIO_GPP_F_PAD_OWN,  // SPT-H GPP_F
      R_PCH_H_PCR_GPIO_GPP_G_PAD_OWN,  // SPT-H GPP_G
      R_PCH_H_PCR_GPIO_GPP_H_PAD_OWN,  // SPT-H GPP_H
      R_PCH_H_PCR_GPIO_GPP_I_PAD_OWN,  // SPT-H GPP_I
      R_PCH_H_PCR_GPIO_GPD_PAD_OWN     // SPT-H GPD
    }
  })

  //
  // GPIO GPI_GPE_STS Offset
  //
  Name(GPEO, Package(){
    Package(){ //SPT-LP
      R_PCH_PCR_GPIO_GPP_A_GPI_GPE_STS,    // SPT-LP GPP_A
      R_PCH_PCR_GPIO_GPP_B_GPI_GPE_STS,    // SPT-LP GPP_B
      R_PCH_PCR_GPIO_GPP_C_GPI_GPE_STS,    // SPT-LP GPP_C
      R_PCH_PCR_GPIO_GPP_D_GPI_GPE_STS,    // SPT-LP GPP_D
      R_PCH_PCR_GPIO_GPP_E_GPI_GPE_STS,    // SPT-LP GPP_E
      R_PCH_LP_PCR_GPIO_GPP_F_GPI_GPE_STS, // SPT-LP GPP_F
      R_PCH_LP_PCR_GPIO_GPP_G_GPI_GPE_STS, // SPT-LP GPP_G
      R_PCH_PCR_GPIO_GPD_GPI_GPE_STS       // SPT-LP GPD
    },
    Package(){ //SPT-H
      R_PCH_PCR_GPIO_GPP_A_GPI_GPE_STS,    // SPT-H GPP_A
      R_PCH_PCR_GPIO_GPP_B_GPI_GPE_STS,    // SPT-H GPP_B
      R_PCH_PCR_GPIO_GPP_C_GPI_GPE_STS,    // SPT-H GPP_C
      R_PCH_PCR_GPIO_GPP_D_GPI_GPE_STS,    // SPT-H GPP_D
      R_PCH_PCR_GPIO_GPP_E_GPI_GPE_STS,    // SPT-H GPP_E
      R_PCH_H_PCR_GPIO_GPP_F_GPI_GPE_STS,  // SPT-H GPP_F
      R_PCH_H_PCR_GPIO_GPP_G_GPI_GPE_STS,  // SPT-H GPP_G
      R_PCH_H_PCR_GPIO_GPP_H_GPI_GPE_STS,  // SPT-H GPP_H
      R_PCH_H_PCR_GPIO_GPP_I_GPI_GPE_STS,  // SPT-H GPP_I
      R_PCH_PCR_GPIO_GPD_GPI_GPE_STS       // SPT-H GPD
    }
  })

  //
  // GPE Enable and Status object. Each bit within one value
  // equals to 0 (1-tier) or And(GPE_EN,GPE_STS) (2-tier) and represents
  // one pad in selected group.
  //
  Name(GPES, Package(){
    0x00000000,  // GPP_A
    0x00000000,  // GPP_B
    0x00000000,  // GPP_C
    0x00000000,  // GPP_D
    0x00000000,  // GPP_E
    0x00000000,  // GPP_F
    0x00000000,  // GPP_G
    0x00000000,  // GPP_H
    0x00000000,  // GPP_I
    0x00000000   // GPD
  })

  //
  // Object for storing RX Level/Edge Configuration for all pads.
  // Each pad needs 2-bits.
  // 00b = Level
  // 01b = Edge (RxInv=0 for rising edge; 1 for falling edge)
  // 10b = Disabled
  // 11b = Either rising edge or falling edge
  //
  Name(RXEV, Package(){
    0xFFFFFFFFFFFFFFFF,  // GPP_A
    0xFFFFFFFFFFFFFFFF,  // GPP_B
    0xFFFFFFFFFFFFFFFF,  // GPP_C
    0xFFFFFFFFFFFFFFFF,  // GPP_D
    0xFFFFFFFFFFFFFFFF,  // GPP_E
    0xFFFFFFFFFFFFFFFF,  // GPP_F
    0xFFFFFFFFFFFFFFFF,  // GPP_G
    0xFFFFFFFFFFFFFFFF,  // GPP_H
    0xFFFFFFFFFFFFFFFF,  // GPP_I
    0xFFFFFFFFFFFFFFFF   // GPD
  })

  //
  // Get GPIO absolute number for selected GpioPad
  //
  Method(GNUM, 0x1, NotSerialized)
  {
    //
    // Arg0 - GpioPad
    //

    // Local1 - Gpio pad number
    Store (GNMB(Arg0), Local1)
    // Local2 - Gpio group index for GpioPad
    Store (GGRP(Arg0), Local2)

    Return (Add(Local1,Multiply(Local2, 24)))
  }

  //
  // Get interrupt number for for selected GpioPad
  //
  Method(INUM, 0x1, NotSerialized)
  {
    //
    // Arg0 - GpioPad
    //

    If(LEqual(PCHS, 0x2)) { // SPT-LP
      Store(PCH_LP_PKG_INDEX, Local0)
    } Else { //SPT-H
      Store(PCH_H_PKG_INDEX,Local0)
    }
    // Local1 - Gpio pad number
    Store (GNMB(Arg0), Local1)
    // Local2 - Gpio group index for GpioPad
    Store (GGRP(Arg0), Local2)
    // Local3 - Group index used in a loop
    Store (0 , Local3)

    While(LLess  (Local3, Local2)) {
      Add( DeRefOf( Index (DeRefOf(Index (GPPG, Local0)),Local3)),Local1,Local1)
      Increment(Local3)
    }

    return(Add(24,Mod(Local1,96)))
  }

  //
  // Get GPIO group index for GpioPad
  //
  Method(GGRP,1,serialized) {
    //
    // Arg0 - GpioPad
    //
    ShiftRight( And(Arg0,0x00FF0000), 16, Local0)
    return (Local0)
  }

  //
  // Get GPIO pin number for GpioPad
  //
  Method(GNMB,1,serialized) {
    //
    // Arg0 - GpioPad
    //
    return (And(Arg0,0x0000FFFF))
  }

  //
  // GPEM (part of PCH NVS) is an object for informing how GPIO groups are mapped to GPE.
  // Mapping for GPP_x is evaluated from (GPEM >> (GroupNumber*2)) & 0x3
  // Possible values for each group:
  // 00b - 2-tier
  // 01b - 1-tier, GPE_DW0
  // 10b - 1-tier, GPE_DW1
  // 11b - 1-tier, GPE_DW2
  //

  //
  // Get GPE number for selected GpioPad
  //
  Method(GGPE, 0x1, NotSerialized)
  {
    //
    // Arg0 - GPIO pad
    //

    //Local0 - GPIO group index (GPP_A - 0, GPP_B - 1 ... )
    Store (GGRP(Arg0), Local0)
    //Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    //
    // Get mapping for certain group
    // Local2 = (GPEM >> (Local0*2)) & 0x3
    //
    Store(And(ShiftRight(GPEM,Multiply(Local0,2)),0x3),Local2)

    If (LEqual(Local2,0x0)) {
      //
      // Pads mapped to 2-tier GPE will all generate GPE_111
      //
      Return (0x6F)
    } Else {
      //
      // For 1-tier GPE calculate GPE number
      // GPE number = (Local2 - 1)*32 + Local1
      //
      Return (Add(Multiply(Subtract(Local2,1),32),Local1))
    }
  }

  //
  // Get GPIO register address
  //
  Method(GADR, 0x2, NotSerialized)
  {
    //
    // Arg0 - GPIO Group index
    // Arg1 - Package with registers offsets for GPIO groups
    //

    If(LEqual(PCHS, 0x2)) { // SPT-LP
      Store(PCH_LP_PKG_INDEX, Local0)
    } Else { //SPT-H
      Store(PCH_H_PKG_INDEX, Local0)
    }
    //Local1 = GpioCommunityAddress
    Store( Add( DeRefOf(Index (DeRefOf( Index(GCOM,Local0)),Arg0)),SBRG),Local1)

    //Local2 = Register Offset
    Store( DeRefOf(Index (DeRefOf( Index(Arg1,Local0)),Arg0)),Local2)

    Return( Add (Local1, Local2))
  }

  //
  // Get Pad Configuration DW0 register value
  //
  Method(GPC0, 0x1, Serialized)
  {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG), Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,32
    }
    Return(TEMP)
  }

  //
  // Set Pad Configuration DW0 register value
  //
  Method(SPC0, 0x2, Serialized)
  {
    //
    // Arg0 - GPIO pad
    // Arg1 - Value for DW0 register
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG), Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,32
    }
    Store(Arg1,TEMP)
  }

  //
  // Get Pad Configuration DW1 register value
  //
  Method(GPC1, 0x1, Serialized)
  {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08) + 0x4
    Store( Add( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),0x4),Local2)
    OperationRegion(PDW1, SystemMemory, Local2, 4)
    Field(PDW1, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,32
    }
    Return(TEMP)
  }

  //
  // Set Pad Configuration DW1 register value
  //
  Method(SPC1, 0x2, Serialized)
  {
    //
    // Arg0 - GPIO pad
    // Arg1 - Value for DW1 register
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local0 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08) + 0x4
    Store( Add( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),0x4),Local2)
    OperationRegion(PDW1, SystemMemory, Local2, 4)
    Field(PDW1, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,32
    }
    Store(Arg1,TEMP)
  }

  //
  // Set RX Override
  //
  Method(SRXO, 0x2, Serialized)
  {
    //
    // Arg0 - GPIO pad
    // Arg1 - 0=no override, 1=drive RX to 1 internally
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
            ,28,
        TEMP,1,
            ,3
    }
    Store(Arg1,TEMP)
  }

  //
  // Get GPI Input Value
  //
  Method(GGIV, 0x1, Serialized)
  {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        ,    1,
        TEMP,1,
        ,    30
    }
    Return(TEMP)
  }

  //
  // Get GPO Output Value
  //
  Method(GGOV, 0x1, Serialized)
  {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,1,
        ,    31
    }
    Return(TEMP)
  }

  //
  // Set GPO Output Value
  //
  Method(SGOV, 0x2, Serialized)
  {
    //
    // Arg0 - GPIO pad
    // Arg1 - Value of GPIO Tx State
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,1,
        ,    31
    }
    Store(Arg1,TEMP)
  }

  //
  // Get GPI Input Invert Bit
  //
  Method(GGII, 0x1, Serialized)
  {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        ,    23,
        TEMP,1,
        ,    8
    }
    Return(TEMP)
  }

  //
  // Set GPI Input Invert Bit
  //
  Method(SGII, 0x2, Serialized)
  {
    //
    // Arg0 - GPIO pad
    // Arg1 - Value of RXINV bit for selected pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        ,    23,
        TEMP,1,
        ,    8
    }
    Store(Arg1,TEMP)
  }

  //
  // Get GPIO Pad Mode
  //
  Method(GPMV, 0x1, Serialized)
  {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        ,    10,
        TEMP,3,
        ,    19
    }
    Return(TEMP)
  }

  //
  // Set GPIO Pad Mode
  //
  Method(SPMV, 0x2, Serialized)
  {
    //
    // Arg0 - GPIO pad
    // Arg1 - Value for Pad Mode for selected pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        ,    10,
        TEMP,3,
        ,    19
    }
    Store(Arg1,TEMP)
  }

  //
  // Get GPIO Host Software Pad Ownership
  //
  Method(GHPO, 0x1, Serialized)
  {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    OperationRegion(PREG, SystemMemory, GADR(Local0,HOWN), 4)
    Field(PREG, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,32
    }
    // HostSwOwnValue = (TEMP >> Local1) & 0x1
    Return( And( ShiftRight(TEMP,Local1),0x1))
  }

  //
  // Set GPIO Host Software Pad Ownership
  //
  Method(SHPO, 0x2, Serialized)
  {
    //
    // Arg0 - GPIO pad
    // Arg1 - Value for GPIO Host Software Pad Ownership
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    OperationRegion(PREG, SystemMemory, GADR(Local0,HOWN), 4)
    Field(PREG, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,32
    }
    // TEMP = (TEMP & (~(1 << PadNumber))) | (HostSwOwnValue << PadNumber)
    Or(And(TEMP, Not(ShiftLeft(1,Local1))),ShiftLeft(Arg1,Local1),TEMP)
  }

  //
  // Get GPIO Pad Ownership
  //
  Method(GGPO, 0x1, Serialized)
  {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = GpioCommunityAddress + PadOwnOffset + (PadNumber >> 3) * 0x4
    Store( Add( GADR(Local0,POWN) , Multiply( ShiftRight(Local1,3),0x4)),Local2)
    OperationRegion(PREG, SystemMemory, Local2, 4)
    Field(PREG, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,32
    }
    // PadOwnValue = (TEMP >> ((Local1 & 0x7) * 4)) & 0x3
    Return( And( ShiftRight(TEMP,Multiply(And(Local1,0x7),0x4)),0x3))
  }

  //
  // Set GPIO GPIRoutIOxAPIC value
  //
  Method(SGRA, 0x2, Serialized)
  {
    //
    // Arg0 - GPIO pad
    // Arg1 - Value for GPIRoutIOxAPIC
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        ,    20,
        TEMP,1,
        ,    11
    }
    Store(Arg1,TEMP)
  }

  //
  // Set GPIO weak pull-up/down value
  //
  Method(SGWP, 0x2, Serialized)
  {
    //
    // Arg0 - GPIO pad
    // Arg1 - Value for weak pull-up/down
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + 0x4 + (GPIn * 0x08)
    Store( Add( Add( GADR(Local0,PCFG), Multiply(Local1,0x08)),0x4),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        ,    10,
        TEMP,4,
        ,    18
    }
    Store(Arg1,TEMP)
  }

  //
  // UGPS, ISME and CGPS methods are used to properly handle 2-tier GPE
  // Example:
  /*
    Method(_L6F, 0) // Method which is called for all 2-tier GPE, must be within _GPE scope
    {
      \_SB.UGPS() // Update information on GPIO mapping to 1 and 2 tier GPE
      If (\_SB.ISME(GpioPad))
        {
          \_SB.DeviceXYZ.GPEH() // Custom function to handle GPE for certain GPIO pad
        }
      \_SB.CGPS() //Clear STS here
    }
  */

  //
  // Update one group entry in GPES object
  // This method is used by UGPS()
  //
  Method(UGP1, 0x1, Serialized)
  {
    //
    // Arg0 - GPIO group index
    //
    OperationRegion(GPPX, SystemMemory, GADR(Arg0,GPEO), 36)
    Field(GPPX, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        STSX,32,
      Offset(0x20),
        GENX,32
    }
    //Check if 2-tier
    If(LEqual(And(ShiftRight(GPEM,Multiply(Arg0,2)),0x3),0x0)) {
      //Store result of GPI_GPE_EN&GPI_GPE_STS in GPES object
      Store ( And(STSX,GENX), Index (GPES, Arg0))
    } Else {
      // If 1-tier store 0 in GPES object
      Store ( 0x0, Index (GPES, Arg0))
    }
  }

  //
  // Update GPES object
  //
  Method(UGPS, 0x0, Serialized)
  {
    //Local0 - GPIO group index
    If(LEqual(PCHS, 0x1)) { // SPT-H
      Store(V_PCH_H_GPIO_GROUP_MAX,Local0)
    } Else { //SPT-LP
      Store(V_PCH_LP_GPIO_GROUP_MAX,Local0)
    }

    While(Local0) {
      Decrement(Local0)
      UGP1(Local0)
    }
  }

  //
  // Clear GPE status for one group from 2-tier
  // This method is used by CGPS()
  //
  Method(CGP1, 0x2, Serialized) {
    //
    // Arg0 - GPIO group index
    // Arg1 - Mask of bits (GpioPads) for which status should be cleared
    //
    //Check if 2-tier
    If(LEqual(And(ShiftRight(GPEM,Multiply(Arg0,2)),0x3),0x0)) {
      //Get GPI_GPE_STS for GPP_x
      OperationRegion(GPPX, SystemMemory, GADR(Arg0,GPEO), 4)
      Field(GPPX, AnyAcc, NoLock, Preserve) {
        Offset(0x0),
          STSX,32,
      }
      //Clear status
      Store (Arg1, STSX)
    }
  }

  //
  // Clear all GPE status for 2-tier
  //
  Method(CGPS, 0x0, Serialized) {
    //Local0 - GPIO group index
    If(LEqual(PCHS, 0x1)) { // SPT-H
      Store(V_PCH_H_GPIO_GROUP_MAX,Local0)
    } Else { //SPT-LP
      Store(V_PCH_LP_GPIO_GROUP_MAX,Local0)
    }

    While(Local0) {
      Decrement(Local0)
      CGP1(Local0, 0xFFFFFFFF)
    }
  }

  //
  // Clear all GPE status for 2-tier which are level sensitive
  //
  Method(CGLS, 0x0, Serialized) {
    //Local0 - GPIO group index
    If(LEqual(PCHS, 0x1)) { // SPT-H
      Store(V_PCH_H_GPIO_GROUP_MAX,Local0)
    } Else { //SPT-LP
      Store(V_PCH_LP_GPIO_GROUP_MAX,Local0)
    }

    While(Local0) {
      Decrement(Local0)

      If(LEqual(Local0,9)){Store(G2L9,Local1);}
      ElseIf(LEqual(Local0,8)){Store(G2L8,Local1);}
      ElseIf(LEqual(Local0,7)){Store(G2L7,Local1);}
      ElseIf(LEqual(Local0,6)){Store(G2L6,Local1);}
      ElseIf(LEqual(Local0,5)){Store(G2L5,Local1);}
      ElseIf(LEqual(Local0,4)){Store(G2L4,Local1);}
      ElseIf(LEqual(Local0,3)){Store(G2L3,Local1);}
      ElseIf(LEqual(Local0,2)){Store(G2L2,Local1);}
      ElseIf(LEqual(Local0,1)){Store(G2L1,Local1);}
      ElseIf(LEqual(Local0,0)){Store(G2L0,Local1);}
      Else {continue}

      CGP1(Local0,Local1)
    }
  }

  //
  // Clear a particular GPE status for 2-tier
  //
  Method(CAGS, 0x1, Serialized) {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    //Check if 2-tier
    If(LEqual(And(ShiftRight(GPEM,Multiply(Local0,2)),0x3),0x0)) {
      //Get GPI_GPE_STS for GPP_x
      OperationRegion(GPPX, SystemMemory, GADR(Local0,GPEO), 4)
      Field(GPPX, AnyAcc, NoLock, Preserve) {
        Offset(0x0),
          STSX,32,
      }
      //Clear status
      Store (STSX, Local3)
      ShiftLeft(1, Local1, Local2)
      Or(STSX, Local2, STSX) // Clear GPIO status
    }
  }

  //
  // Check GPES buffer
  //
  Method(ISME, 0x1, NotSerialized) {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // return (GPES[Group]>>PadNumber) & 0x1
    Return( And( ShiftRight(DeRefOf( Index(GPES,Local0)),Local1),0x1))
  }

  //
  // Do Interrupt Pin Isolation
  // This method should be called before power gating external device
  //
  Method(DIPI, 0x1, Serialized) {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG) , Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        ,    9,
        RDIS,1,
        ,   15,
        RCFG,2,
        ,    5
    }
    If(LNotEqual(RCFG,2)) {
      // Save RxEvCfg state in RXEV object:
      // Local3 = RXEV[Group]
      // Local3 &= ~(0x3 << (PadNr*2))
      // RXEV[Group] = Local3
      Store(DeRefOf(Index (RXEV, Local0)),Local3)
      And(Local3,Not(ShiftLeft(0x3,Multiply(Local1,2))),Local3)
      Or(Local3,ShiftLeft(RCFG,Multiply(Local1,2)),Index(RXEV,Local0))
      // Set RxEvCfg to 2
      Store(2,RCFG)
      // Set GPIORxDis to 1
      Store(1,RDIS)
    }
  }

  //
  // Undo Interrupt Pin Isolation
  // This method should be called after un-power gating external device
  //
  Method(UIPI, 0x1, Serialized) {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * 0x08)
    Store( Add( GADR(Local0,PCFG), Multiply(Local1,0x08)),Local2)
    OperationRegion(PDW0, SystemMemory, Local2, 4)
    Field(PDW0, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        ,    9,
        RDIS,1,
        ,   15,
        RCFG,2,
        ,    5
    }
    // Get RxEvCfg original value from RXEV object
    // Local3 = (RXEV[Group] >> (PadNr*2)) & 0x3
    Store(And(ShiftRight(DeRefOf(Index (RXEV, Local0)),Multiply(Local1,2)),0x3),Local3)

    If(LNotEqual(Local3,2)) {
      // Set GPIORxDis to 0
      Store(0,RDIS)
      // Set RxEvCfg to original value
      Store(Local3,RCFG)
    }
  }

} // \_SB Scope


