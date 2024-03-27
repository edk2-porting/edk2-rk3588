#include "AcpiTables.h"

Device (YBAT)
{
    Name(_HID, "GEDU1829")
    Name(_UID, Zero)
    Method (_STA)
    {
      Return (0xF)
    }
    Method(_CRS, 0x0, NotSerialized)
    {
        Name (RBUF, ResourceTemplate ()
        {
            //
            // \_SB.I2C and \_SB.GPIO are paths to predefined I2C
            // and GPIO controller instances.
            //
            I2CSerialBus(0x63, ControllerInitiated, 198000000, AddressingMode7Bit, "\\_SB.I2C2")
            // GpioInt(Level, ActiveLow, Exclusive, PullDown, 0, "\\_SB.GPI0") {0}    // GPIO0-A0
            GpioIo(Exclusive, PullUp, 0, 0,, "\\_SB.GPI0",0, ResourceConsumer, , RawDataBuffer() {1}) {0x19} // DC_DETECT
        })
        Return(RBUF)
    }   
}
Device (AC)
{
    Name (_HID, "ACPI0003")
    Name (_UID, 0x00)
    Name (_PCL, Package (0x01)
    {
        \_SB
    })
    // 0 – Off-line (not on AC power)
    // 1 – On-line
    Method (_PSR, 0, NotSerialized)
    {
        Return (\_SB.GPI0.DCDT)
    }

    Method (_STA, 0, NotSerialized)
    {
        Return (0x0F)
    }
}        