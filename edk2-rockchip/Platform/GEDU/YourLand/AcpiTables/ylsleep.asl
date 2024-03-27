#include "AcpiTables.h"
 
  // Sleep states supported by Chipset/Board.
  // SSx - BIOS setup controlled enabled _Sx Sleep state status
  // Values to be written to SLP_TYPE register are provided by SBACPI.SDL (South Bridge ACPI ModulePart)

 Name(\_S0, Package(4) {0x0,0x0,0,0}) // mandatory System state
//  Name(\_S1, Package(4) {0x1,0x0,0,0})
 // Name(\_S3, Package(4) {0x5,0x0,0,0})
//  Name(\_S4, Package(4) {0x6,0x0,0,0})
 Name(\_S5, Package(4) {0x7,0x0,0,0}) // mandatory System state

  Method(PTS, 1)          // METHOD CALLED FROM _PTS PRIOR TO ENTER ANY SLEEP STATE
  {
    If(Arg0)            // entering any sleep state
    {
        Store(0, \_SB.GPI4.BLEN)
    }
  }
  Method(WAK, 1)          // METHOD CALLED FROM _WAK RIGHT AFTER WAKE UP
  {
    Store(1, \_SB.GPI4.BLEN)
  }