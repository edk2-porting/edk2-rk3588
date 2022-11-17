/** @file
  This file contains the device definition of the System Agent
  ACPI reference code.
  Currently defines the device objects for the
  System Agent PCI Express* ports (PEG), iGfx and other devices.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

External(\_SB.PCI0, DeviceObj)
External(\_SB.PCI0.GFX0, DeviceObj)
External(\_SB.PCI0.PCIC, MethodObj)
External(\_SB.PCI0.PCID, MethodObj)


///
/// I.G.D
///
Scope (\_SB.PCI0.GFX0)
{
  include("Igfx.asl")
} // end I.G.D


