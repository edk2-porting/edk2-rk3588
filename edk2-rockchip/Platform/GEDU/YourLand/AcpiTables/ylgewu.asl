/** @file
 *
 *  Copyright (c) 2023, GSL. All rights reserved.
 *
 **/
 #include "AcpiTables.h"

Device (GEWU)
{
    Name(_HID, "GEWU2023") // "TEST0003"
    Name(_UID, Zero)
    Name (_ADR, 0)

    Method (_CRS, 0x0, NotSerialized) {
      Name (RBUF, ResourceTemplate () {
        //
        // GPIO Interrupt Resources
        //
        // GpioInt(Edge, ActiveHigh, Shared, PullUp, 0, "\\_SB.GPI0", 0, ResourceConsumer,, RawDataBuffer() {1}) {1}
        //
        // GPIO IO Resources
        //
        GpioIo(Exclusive, PullUp, 0, 0,, "\\_SB.GPI4",0, ResourceConsumer, , RawDataBuffer() {1}) {13}
        GpioIo(Exclusive, PullUp, 0, 0,, "\\_SB.GPI0",0, ResourceConsumer, , RawDataBuffer() {1}) {24}
      })

      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized) {
        Return(0xf)
    }

}
