#include "AcpiTables.h"

Device (UPLD)
{
    Name (_HID, EisaId ("PNP0A05") /* Generic Container Device */)  // _HID: Hardware ID
    Name (_UID, "USB _PLD Object Container")  // _UID: Unique ID
    Name (PLD0, Package (0x01)
    {
        ToPLD (
        PLD_Revision = 0x2,
        PLD_IgnoreColor = 0x1,
        PLD_Red = 0x37,
        PLD_Green = 0x44,
        PLD_Blue = 0xFF,
        PLD_Width = 0x4,
        PLD_Height = 0x19,
        PLD_UserVisible = 0x1,
        PLD_Dock = 0x0,
        PLD_Lid = 0x1,
        PLD_Panel = "TOP",
        PLD_VerticalPosition = "CENTER",
        PLD_HorizontalPosition = "RIGHT",
        PLD_Shape = "VERTICALRECTANGLE",
        PLD_GroupOrientation = 0x1,
        PLD_GroupToken = 0xA,
        PLD_GroupPosition = 0x21,
        PLD_Bay = 0x1,
        PLD_Ejectable = 0x0,
        PLD_EjectRequired = 0x1,
        PLD_CabinetNumber = 0x1E,
        PLD_CardCageNumber = 0x17,
        PLD_Reference = 0x0,
        PLD_Rotation = 0x7,
        PLD_Order = 0x3,
        PLD_VerticalOffset = 0x141,
        PLD_HorizontalOffset = 0x2C)
    })
    Name (PLD1, Package (0x01)
    {
        Buffer (0x14)
        {
            /* 0000 */  0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
            /* 0008 */  0x51, 0x04, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,  // Q.......
            /* 0010 */  0xFF, 0xFF, 0xFF, 0xFF                           // ....
        }
    })
    Name (PLD2, Package (0x01)
    {
        Buffer (0x14)
        {
            /* 0000 */  0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
            /* 0008 */  0x51, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,  // Q.......
            /* 0010 */  0xFF, 0xFF, 0xFF, 0xFF                           // ....
        }
    })
    Name (PLM0, Package (0x01)
    {
        Buffer (0x14)
        {
            /* 0000 */  0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
            /* 0008 */  0x51, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Q.......
            /* 0010 */  0xFF, 0xFF, 0xFF, 0xFF                           // ....
        }
    })
    Name (PLM1, Package (0x01)
    {
        Buffer (0x14)
        {
            /* 0000 */  0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
            /* 0008 */  0x51, 0x90, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,  // Q.......
            /* 0010 */  0xFF, 0xFF, 0xFF, 0xFF                           // ....
        }
    })
    Name (PLM2, Package (0x01)
    {
        Buffer (0x14)
        {
            /* 0000 */  0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
            /* 0008 */  0x51, 0x90, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,  // Q.......
            /* 0010 */  0xFF, 0xFF, 0xFF, 0xFF                           // ....
        }
    })
}

Device (UUPC)
{
    Name (_HID, EisaId ("PNP0A05") /* Generic Container Device */)  // _HID: Hardware ID
    Name (_UID, "USB _UPC Object Container")  // _UID: Unique ID
    Name (UPCC, Package (0x04)
    {
        One, // Connectable
        0x09, // Type, 
        0x3D, 
        Zero
    })
    Name (UPC2, Package (0x04)
    {
        One, 
        Zero, // Type ‘A’ connector
        Zero, 
        Zero
    })
    Name (UPC3, Package (0x04)
    {
        One, 
        0x03, // USB 3 Standard-A connector
        Zero, 
        Zero
    })
}

Device (UCSI)
{
    Name (_HID, "GEDU0500")  // _HID: Hardware ID
    Name (_CID, EisaId ("PNP0CA0"))  // _CID: Compatible ID
    Name (_UID, 0x05)  // _UID: Unique ID
    Name (_DDN, "USB Type-C")  // _DDN: DOS Device Name
    // Name (_DEP, Package (0x03)  // _DEP: Dependencies
    // {
        // \_SB.ABD, 
        // \_SB.PMGK, 
        // \_SB.UCS0
    // })
    Method (_STA, 0, NotSerialized)  // _STA: Status
    {
        // VERS = 0x0100
        // CCI = Zero
        // MSGI = Zero
        // If ((\_SB.PMGK.LKUP > Zero))
        // {
            Return (0x0F)
        // }
        // Else
        // {
        //     Return (Zero)
        // }
    }

    Device (UCN0)
    {
        Name (_ADR, Zero)  // _ADR: Address
        Method (_UPC, 0, NotSerialized)  // _UPC: USB Port Capabilities
        {
            Return (\_SB.UUPC.UPCC)
        }

        Method (_PLD, 0, NotSerialized)  // _PLD: Physical Location of Device
        {
            Return (\_SB.UPLD.PLD0)
        }
    }

    Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
    {
        Memory32Fixed (ReadWrite,
            0xff090070, //0x81F20000,         // Address Base
            0x00000030,         // Address Length
            )
    })
    OperationRegion (USBC, SystemMemory, 0xff090070, 0x30) //0x81F20000, 0x30)
    Field (USBC, ByteAcc, Lock, Preserve)
    {
        VERS,   16, 
        RESV,   16, 
        CCI,    32, 
        CTRL,   64, 
        MSGI,   128, 
        MSGO,   128
    }

    Name (BUFF, Buffer (0x32){})
    CreateField (BUFF, Zero, 0x08, BSTA)
    CreateField (BUFF, 0x08, 0x08, BSIZ)
    CreateField (BUFF, 0x10, 0x10, BVER)
    CreateField (BUFF, 0x30, 0x20, BCCI)
    CreateField (BUFF, 0x50, 0x40, BCTL)
    CreateField (BUFF, 0x90, 0x80, BMGI)
    CreateField (BUFF, 0x0110, 0x80, BMGO)
    Method (OPMW, 0, NotSerialized)
    {
        BCTL = CTRL /* \_SB_.UCSI.CTRL */
        BMGO = MSGO /* \_SB_.UCSI.MSGO */
        // \_SB.PMGK.UCSI = BUFF /* \_SB_.UCSI.BUFF */
        Return (Zero)
    }

    Method (OPMR, 0, NotSerialized)
    {
        // BUFF = \_SB.PMGK.UCSI
        VERS = BVER /* \_SB_.UCSI.BVER */
        CCI = BCCI /* \_SB_.UCSI.BCCI */
        MSGI = BMGI /* \_SB_.UCSI.BMGI */
        Return (Zero)
    }

    Method (_DSM, 4, Serialized)  // _DSM: Device-Specific Method
    {
        If ((Arg0 == ToUUID ("6f8398c2-7ca4-11e4-ad36-631042b5008f")))
        {
            While (One)
            {
                Name (_T_0, 0x00)  // _T_x: Emitted by ASL Compiler
                _T_0 = ToInteger (Arg2)
                If ((_T_0 == Zero))
                {
                    Return (Buffer (One)
                    {
                            0x0F                                             // .
                    })
                }
                ElseIf ((_T_0 == One))
                {
                    Return (OPMW ())
                }
                ElseIf ((_T_0 == 0x02))
                {
                    Return (OPMR ())
                }
                ElseIf ((_T_0 == 0x03))
                {
                    Return (Zero)
                }
                ElseIf ((_T_0 == 0x04))
                {
                    Return (One)
                }

                Break
            }
        }
    }
}
