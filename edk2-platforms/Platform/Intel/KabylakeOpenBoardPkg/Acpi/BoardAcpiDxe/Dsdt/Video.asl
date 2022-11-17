/** @file
  ACPI DSDT table

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/



External(DIDX)

// Brightness Notification:
//    Generate a brightness related notification
//      to the LFP if its populated.
//
//  Arguments:
//    Arg0: Notification value.
//
//  Return Value:
//    None

Method(BRTN,1,Serialized)
{
    If(LEqual(And(DIDX,0x0F00),0x400))
    {
        Notify(\_SB.PCI0.GFX0.DD1F,Arg0)
    }
}
