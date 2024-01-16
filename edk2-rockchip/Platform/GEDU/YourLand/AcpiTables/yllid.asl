#include "AcpiTables.h"

Device (LID0)
{
    Name(_HID, EISAID("PNP0C0D"))
    Name(_UID, Zero)

    // Define a Lid switch change to GPIO
    // OperationRegion(\PHO, SystemIO, 0x201, 0x1)
    //    Field(\PHO, ByteAcc, NoLock, Preserve) {
    //    LPOL, 1 // Lid polarity control bit
    //}
    Alias (\_SB.PSUB, _SUB)
    Name (_DEP, Package (0x02)  // _DEP: Dependencies
    {
        \_SB.GPI0, 
        \_SB.SCM0
    })
    Name (LIDB, One)
    Method (_LID, 0, NotSerialized)  // _LID: Lid Status
    {
        Return (LIDB) /* \_SB_.LID0.LIDB */
    }

    Method (_PS0, 0, NotSerialized)  // _PS0: Power State 0
    {
        If (\_SB.GPI0.GABL)
        {
            \_SB.LID0.LIDB = \_SB.GPI0.LIDR
            Notify (\_SB.LID0, 0x80) // Status Change
        }
    }

    Method (_PS3, 0, NotSerialized)  // _PS3: Power State 3
    {
        If (\_SB.GPI0.GABL)
        {
            \_SB.LID0.LIDB = \_SB.GPI0.LIDR
            Notify (\_SB.LID0, 0x80) // Status Change
        }
    }


    // Name(_PRW, Package(2){
    // 1, // bit 1 of GPE to enable Lid wakeup
    // 0x04} // can wakeup from S4 state
    // )
    // }
    
    // Scope(\_GPE){ // Root level event handlers
    //     Method(_L01){ // uses bit 1 of GP0_STS register
    //     Not(LPOL, LPOL) // Flip the lid polarity bit
    //     Notify(LID, 0x80) // Notify OS of event
    // }
}