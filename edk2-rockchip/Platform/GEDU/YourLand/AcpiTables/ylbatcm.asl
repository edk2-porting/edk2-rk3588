#include "AcpiTables.h"

Device (BAT0)
{
    Name (_HID, EisaId ("PNP0C0A"))
    Name (_UID, 0x00)
    Name (_PCL, Package (0x01)
    {
        \_SB
    })

    Name (B0ST, 0x01)
    Name (BT0I, Package (0x0D)
    {
        0x00, // reported in mW
        0xB220, // design capacity 45.6
        0xB210, // last full capacity
        0x01,  // rechargeble
        0x1db0, // 7.6V design voltage
        0x00, // Design capacity of Warning
        0x00, // Design capacity of low
        0x01, // Battery Capacity Granularity 1
        0x01, // Battery Capacity Granularity 2
        "GSL-YL", 
        "2023-11-30", 
        "LIon", 
        "GEDU"
    })
    Name (BT0P, Package (0x04) {})
    /*
    Bit [0] – Set if the device is present.
    Bit [1] – Set if the device is enabled and decoding its resources.
    Bit [2] – Set if the device should be shown in the UI.
    Bit [3] – Set if the device is functioning properly (cleared if device failed its diagnostics).
    Bit [4] – Set if the battery is present.
    Bits [31:5] – Reserved (must be cleared).
    */
    Method (_STA, 0, NotSerialized)
    {
        /*
        Store (0x00, Local0)
        If (XOr (1, Local0))
        {
            //Store (0x00, BT0P)
            Store (0x00, Local1)
            If (LNot (LEqual (Local1, 0x1F)))
            {
                Store (0x00, Local1)
            }

            //\_SB.SSMI (0xEA3A, 0x00, Local1, 0x00, 0x00)
            //\_SB.WMID.WGWE (0x05, 0x00)
        }

        If (B0ST)
        {
            Return (0x1F)
        }
        Else
        {
            Return (0x0F)
        }
        */
        Return (0x1F)
        //Return (1) // (_UID)
    }
    
    Method (_BIF, 0, NotSerialized)
    {
        Return (BT0I)
    }
/*
Package {
Battery State // Integer (DWORD)
Battery Present Rate // Integer (DWORD)
Battery Remaining Capacity // Integer (DWORD)
Battery Present Voltage // Integer (DWORD)
*/
    Method (_BST, 0, NotSerialized)
    {
        Name (I2C0, ResourceTemplate ()
        {
            I2cSerialBusV2 (0x63, ControllerInitiated, 0x000186A0,
                AddressingMode7Bit, "\\_SB.I2C2",
                0x00, ResourceConsumer, , Exclusive,
                )
        })
        OperationRegion (CWRG, GenericSerialBus, Zero, 0xA2)
        Field (CWRG, BufferAcc, NoLock, Preserve)
        {
            Connection (I2C0), 
            AccessAs (BufferAcc, AttribByte), 
            ICVE, 8, // version
            UNKW, 8,
            VCLH, 8, // voltage cell high
            VCLL, 8, // voltage cell low
            SOCH, 8,
            SOCL, 8, 
            TEMP, 8,
        }        
        //XOr (DerefOf (Index (BT0I, 0x50)), 0x01, Local0)
        // Bit 0 discharge, Bit 1 charge, Bit 2 critical
        Store(0x2,Index(BT0P,0))
        Store(SOCH,Local0)
        If (LLess (Local0, 0x20)) {
            Store(0x50, Local0)
        }
        Store(Local0, Index(BT0P,1))
        Store(0xb010,Index(BT0P,2))
        Store(0x1bd0,Index(BT0P,3)) // in mW
        Return (BT0P)
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
    Method (_PSR, 0, NotSerialized)
    {
        Return (0x00)
    }

    Method (_STA, 0, NotSerialized)
    {
        Return (0x0F)
    }
}        