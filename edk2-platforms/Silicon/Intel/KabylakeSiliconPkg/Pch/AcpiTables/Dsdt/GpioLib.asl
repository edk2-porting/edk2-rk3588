/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
include("GpioLibObjects.asl")

//
// PADCFG register is split into multiple DW registers
// PAD_CFG_SIZE holds number of bytes used by all those registers for one pad
//
#define PAD_CFG_SIZE 0x08

//
// Get number of DoubleWord registers used by one group
//
#define TOTAL_NUMBER_OF_DW_REGS_FOR_GPIO_GROUP(GpioGroupIndex) \
  ( \
    Add( \
      ShiftRight( \
        GINF(GpioGroupIndex,GPIO_PAD_NUM_FIELD), \
        5), \
      1) \
  )

//
// Check if group belongs to 2-Tier GPE
//
#define IS_GPIO_GROUP_IN_2_TIER_GPE(GpioGroupIndex) \
  ( \
    LEqual( \
      And( \
        ShiftRight( \
          GPEM, \
          Multiply( \
            GpioGroupIndex, \
            2)), \
        0x3), \
      0x0) \
  )

//
// Get PADCFG_DW0 GpioPad register address
//
#define GET_GPIO_PAD_PADCFG_REG_ADDRESS(GpioGroupIndex,GpioPadNumber) \
  ( \
    Add( \
      GET_PADCFG_REG_ADDRESS(GpioGroupIndex), \
      Multiply( \
        GpioPadNumber, \
        PAD_CFG_SIZE) \
      ) \
  )

//
// Get GPIO register address
// If group has more DW registers for certain feature
// below macro will return address of the first one.
//
#define GET_PADCFG_REG_ADDRESS(GpioGroupIndex)      GADR(GpioGroupIndex,GPIO_PADCFG_REG_FIELD)
#define GET_HOSTSW_OWN_REG_ADDRESS(GpioGroupIndex)  GADR(GpioGroupIndex,GPIO_HOSTOWN_REG_FIELD)
#define GET_PAD_OWN_REG_ADDRESS(GpioGroupIndex)     GADR(GpioGroupIndex,GPIO_PADOWN_REG_FIELD)
#define GET_GPE_STS_REG_ADDRESS(GpioGroupIndex)     GADR(GpioGroupIndex,GPIO_GPE_STS_REG_FIELD)

//
// GPIO Access Library
//
Scope(\_SB)
{
  //
  // Arg1 possible values for internal GINF function
  //
  #define GPIO_COM_FIELD          0
  #define GPIO_PAD_NUM_FIELD      1
  #define GPIO_PADCFG_REG_FIELD   2
  #define GPIO_HOSTOWN_REG_FIELD  3
  #define GPIO_PADOWN_REG_FIELD   4
  #define GPIO_GPE_STS_REG_FIELD  5

  //
  // Get GPIO group information
  // This is internal library function
  //
  Method(GINF, 0x2, NotSerialized)
  {
    //
    // Arg0 - GPIO Group index
    // Arg1 - GPIO field type
    //
    If(LEqual(PCHS, SPTL)) { // PCH-LP
      Return (PCH_LP_GPIO_GET_FIELD(Arg0,Arg1))
    } Else { //PCH-H
      Return (PCH_H_GPIO_GET_FIELD(Arg0,Arg1))
    }
  }

  //
  // Get GPIO maximal group
  // This is internal library function
  //
  Method(GMXG, 0x0, NotSerialized)
  {
    If(LEqual(PCHS, SPTL)) { // PCH-LP
      Return (V_PCH_LP_GPIO_GROUP_MAX)
    } Else { //PCH-H
      Return (V_PCH_H_GPIO_GROUP_MAX)
    }
  }

  //
  // Get GPIO register address
  // This is internal library function
  //
  Method(GADR, 0x2, NotSerialized)
  {
    //
    // Arg0 - GPIO Group index
    // Arg1 - GPIO register type, must match what is used by GINF method
    //
    //Local0 = GpioCommunityAddress
    Store( Add(GINF(Arg0,GPIO_COM_FIELD), SBRG), Local0)

    //Local1 = Register Offset
    Store(GINF(Arg0,Arg1), Local1)

    Return( Add (Local0, Local1))
  }

  //
  // Get GPIO absolute number for selected GpioPad
  //
  Method(GNUM, 0x1, NotSerialized)
  {
    //
    // Arg0 - GpioPad
    //

    // Local0 - Gpio pad number
    Store (GNMB(Arg0), Local0)
    // Local1 - Gpio group index for GpioPad
    Store (GGRP(Arg0), Local1)

    Return (Add(Local0,Multiply(Local1, 24)))
  }

  //
  // Get interrupt number for selected GpioPad
  //
  Method(INUM, 0x1, NotSerialized)
  {
    //
    // Arg0 - GpioPad
    //
    // Local1 - Gpio pad number
    Store (GNMB(Arg0), Local1)
    // Local2 - Gpio group index for GpioPad
    Store (GGRP(Arg0), Local2)
    // Local3 - Group index used in a loop
    Store (0 , Local3)

    While(LLess  (Local3, Local2)) {
      // Get Number of GpioPads for certain Group (GroupIndex - Local3) and add it to Local1
      Add( GINF(Local3,GPIO_PAD_NUM_FIELD) ,Local1,Local1)

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

    If (IS_GPIO_GROUP_IN_2_TIER_GPE(Local0)) {
      //
      // Pads mapped to 2-tier GPE will all generate GPE_111
      //
      Return (0x6F)
    } Else {
      //
      // Get mapping for certain group
      // Local2 = (GPEM >> (Local0*2)) & 0x3
      //
      Store(And(ShiftRight(GPEM,Multiply(Local0,2)),0x3),Local2)
      //
      // For 1-tier GPE calculate GPE number
      // GPE number = (Local2 - 1)*32 + Local1
      //
      Return (Add(Multiply(Subtract(Local2,1),32),Local1))
    }
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store(GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE) + 0x4
    Store( Add( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),0x4),Local2)
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

    // Local0 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE) + 0x4
    Store( Add( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),0x4),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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
    // Local3 - Register offset = HostSwOwnRegOffset + (GpioPadNumber >> 5)*0x4
    Add (GET_HOSTSW_OWN_REG_ADDRESS(Local0), Multiply (ShiftRight(Local1,5), 0x4), Local3)
    // Local4 - Pad bit position within register, Local4 = GpioPad & 0x1F = GpioPad % 32
    And (Local1, 0x1F, Local4)

    OperationRegion(PREG, SystemMemory, Local3, 4)
    Field(PREG, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,32
    }
    // HostSwOwnValue = (TEMP >> Local4) & 0x1
    Return( And( ShiftRight(TEMP,Local4),0x1))
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
    // Local3 - Register offset = HostSwOwnRegOffset + (GpioPadNumber >> 5)*0x4
    Add (GET_HOSTSW_OWN_REG_ADDRESS(Local0), Multiply (ShiftRight(Local1,5), 0x4), Local3)
    // Local4 - Pad bit position within register, Local4 = GpioPad & 0x1F = GpioPad % 32
    And (Local1, 0x1F, Local4)

    OperationRegion(PREG, SystemMemory, Local3, 4)
    Field(PREG, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        TEMP,32
    }

    If (Arg1) {
      //TEMP = TEMP | (1 << PadBitPosition)
      Or(TEMP, ShiftLeft(1,Local4), TEMP)
    } Else {
      //TEMP = TEMP & ~(1 << PadBitPosition)
      And(TEMP, Not(ShiftLeft(1,Local4)),TEMP)
    }
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

    // Local2 = PadOwnAddress + (PadNumber >> 3) * 0x4
    Store( Add( GET_PAD_OWN_REG_ADDRESS(Local0) , Multiply( ShiftRight(Local1,3),0x4)),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + 0x4 + (GPIn * PAD_CFG_SIZE)
    Store( Add( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),0x4),Local2)
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
  // ISME and CAGS methods are used to properly handle 2-tier GPE
  // Example:
  /*
    Method(_L6F, 0) // Method which is called for all 2-tier GPE, must be within _GPE scope
    {
      If (\_SB.ISME(GpioPad))
        {
          \_SB.DeviceXYZ.GPEH() // Custom function to handle GPE for certain GPIO pad
          \_SB.CAGS(GpioPad)    // Clear 2-Tier GPE status
        }
    }
  */

  //
  // @deprecated method, maintained for compatibility
  // Will be removed in future generations
  //
  Method(UGPS, 0x0, Serialized) {

  }

  //
  // Clear all GPE status for 2-tier
  // @deprecated method, maintained for compatibility
  // Will be removed in future generations
  //
  Method(CGPS, 0x0, Serialized) {

  }

  //
  // Clear all GPE status for 2-tier which are level sensitive
  // @deprecated method, maintained for compatibility
  // Will be removed in future generations
  //
  Method(CGLS, 0x0, Serialized) {

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
    // Local4 - Additional offset to handle cases when GPIO group has more than 32 pads
    Multiply(ShiftRight (Local1,5),4,Local4)

    //Check if 2-tier
    If (IS_GPIO_GROUP_IN_2_TIER_GPE(Local0)) {
      //Get GPI_GPE_STS for GPP_x
      OperationRegion(GPPX, SystemMemory, Add(GET_GPE_STS_REG_ADDRESS(Local0),Local4), 4)
      Field(GPPX, AnyAcc, NoLock, Preserve) {
        Offset(0x0),
          STSX,32,
      }
      // Local2 - GpioPad bit mask within register
      ShiftLeft(1, Mod(Local1,32), Local2)
      // Clear GPIO status by writing 1b for a pad
      Store(Local2, STSX)
    }
  }

  //
  // Check if GPIO pad was source of 2-Tier GPE event
  //
  Method(ISME, 0x1, NotSerialized) {
    //
    // Arg0 - GPIO pad
    //
    If (LNot(IS_GPIO_GROUP_IN_2_TIER_GPE(Arg0))) {
      Return(0)
    }

    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)
    // Local2 - Register offset = GpeStsRegOffset + (GpioPadNumber >> 5)*0x4
    Add (GET_GPE_STS_REG_ADDRESS(Local0), Multiply (ShiftRight(Local1,5), 0x4), Local2)
    // Local3 - Pad bit position within register, Local3 = GpioPad & 0x1F = GpioPad % 32
    And (Local1, 0x1F, Local3)

    OperationRegion(GPPX, SystemMemory, Local2, 0x24)
    Field(GPPX, AnyAcc, NoLock, Preserve) {
      Offset(0x0),
        STSX,32,
      Offset(0x20),
        GENX,32
    }

    //Return result of GPI_GPE_EN&GPI_GPE_STS for this pad
    Return(And(ShiftRight(And(STSX,GENX),Local3),0x1))
  }

  //
  // Do Interrupt Pin Isolation
  // This method should be called before power gating external device
  // which uses Gpio pad as an interrupt
  //
  Method(DIPI, 0x1, Serialized) {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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
      // RXEV[GroupIndex][PadNumber] =  RCFG
      Store(RCFG, Index( DeRefOf(Index(RXEV,Local0)), Local1))
      // Set RxEvCfg to 2
      Store(2,RCFG)
      // Set GPIORxDis to 1
      Store(1,RDIS)
    }
  }

  //
  // Undo Interrupt Pin Isolation
  // This method should be called after un-power gating external device
  // which uses Gpio pad as an interrupt
  //
  Method(UIPI, 0x1, Serialized) {
    //
    // Arg0 - GPIO pad
    //
    // Local0 - GPIO group index
    Store (GGRP(Arg0), Local0)
    // Local1 - GPIO pad number
    Store (GNMB(Arg0), Local1)

    // Local2 = (GpioCommunityAddress + PadCfgOffset) + (GPIn * PAD_CFG_SIZE)
    Store( GET_GPIO_PAD_PADCFG_REG_ADDRESS(Local0,Local1),Local2)
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
    // Local3 = RXEV[GroupIndex][PadNumber]
    Store( DeRefOf(Index( DeRefOf(Index (RXEV, Local0)), Local1)), Local3)

    If(LNotEqual(Local3,2)) {
      // Set GPIORxDis to 0
      Store(0,RDIS)
      // Set RxEvCfg to original value
      Store(Local3,RCFG)
    }
  }

} // \_SB Scope


