/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/


// Comment out includes as ifdefs don't work at trim stage

//
//

Scope(\_SB) {
  //
  //---------------------------------------------------------------------------
  // List of IRQ resource buffers compatible with _PRS return format.
  //---------------------------------------------------------------------------
  // Naming legend:
  // RSxy, PRSy - name of the IRQ resource buffer to be returned by _PRS, "xy" - last two characters of IRQ Link name.
  // Note. PRSy name is generated if IRQ Link name starts from "LNK".
  // HLxy , LLxy - reference names, can be used to access bit mask of available IRQs. HL and LL stand for active High(Low) Level triggered Irq model.
  //---------------------------------------------------------------------------
    Name(PRSA, ResourceTemplate(){  // Link name: LNKA
  	IRQ(Level, ActiveLow, Shared, LLKA) {3,4,5,6,10,11,12,14,15}
    })
    Alias(PRSA,PRSB)  // Link name: LNKB
    Alias(PRSA,PRSC)  // Link name: LNKC
    Alias(PRSA,PRSD)  // Link name: LNKD
    Alias(PRSA,PRSE)  // Link name: LNKE
    Alias(PRSA,PRSF)  // Link name: LNKF
    Alias(PRSA,PRSG)  // Link name: LNKG
    Alias(PRSA,PRSH)  // Link name: LNKH
}

//
//

  Scope(\_SB.PC00) {
    //
    // PCI-specific method's GUID
    //
    Name(PCIG, ToUUID("E5C937D0-3553-4d7a-9117-EA4D19C3434D"))
    //
    // PCI's _DSM - an attempt at modular _DSM implementation
    // When writing your own _DSM function that needs to include PCI-specific methods, do this:
    //
    // Method(_YOUR_DSM,4){ 
    //   if(Lequal(Arg0,PCIG)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }
    //   ...continue your _DSM by checking different GUIDs...
    //   else { return(0) }
    // } 
    //
    Method(PCID, 4, Serialized) {
      If(LEqual(Arg0, PCIG)) {         // PCIE capabilities UUID
        If(LGreaterEqual(Arg1,3)) {                                              // revision at least 3
          If(LEqual(Arg2,0)) { Return (Buffer(2){0x01,0x03}) }                   // function 0: list of supported functions
          If(LEqual(Arg2,8)) { Return (1) }                                      // function 8: Avoiding Power-On Reset Delay Duplication on Sx Resume
          If(LEqual(Arg2,9)) { Return (Package(5){50000,Ones,Ones,50000,Ones}) } // function 9: Specifying Device Readiness Durations
        }
      }
      return (Buffer(1){0})
    }
  }//scope
Scope(\_SB.PC00) {
  //PciCheck, Arg0=UUID, returns true if support for 'PCI delays optimization ECR' is enabled and the UUID is correct
  Method(PCIC,1,Serialized) {
    If(LEqual(ECR1,1)) {
      If(LEqual(Arg0, PCIG)) {
        return (1)
      }
    }
    return (0)
  }
}


