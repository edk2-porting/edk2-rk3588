/** @file
 *
 *  RK3588 CPU devices for YourLand.
 *
 *  Copyright (c) 2023, GEDU Tech. All rights reserved.
 *
 *
 **/

 #include "AcpiTables.h"

 Device (PKG0)
 {
   Name (_HID, "ACPI0010")
   Name (_UID, 8)
   Method (_STA)
   {
     Return (0xF)
   }
 
   Device (CLU0)
   {
     Name (_HID, "ACPI0010")
     Name (_UID, 9)
     Method (_STA)
     {
       Return (0xF)
     }
 
     Device (CPU0)
     {
       Name (_HID, "ACPI0007")
       Name (_UID, 0)
       Method (_STA)
       {
         Return (0xF)
       }
     }
 
     Device (CPU1)
     {
       Name (_HID, "ACPI0007")
       Name (_UID, 1)
       Method (_STA)
       {
         Return (0xF)
       }
     }
 
     Device (CPU2)
     {
       Name (_HID, "ACPI0007")
       Name (_UID, 2)
       Method (_STA)
       {
         Return (0xF)
       }
     }
 
     Device (CPU3)
     {
       Name (_HID, "ACPI0007")
       Name (_UID, 3)
       Method (_STA)
       {
         Return (0xF)
       }
     }
   }
 
   Device (CLU1)
   {
     Name (_HID, "ACPI0010")
     Name (_UID, 10)
     Method (_STA)
     {
       Return (0xF)
     }
 
     Device (CPU4)
     {
       Name (_HID, "ACPI0007")
       Name (_UID, 4)
       Method (_STA)
       {
         Return (0xF)
       }
     }
 
     Device (CPU5)
     {
       Name (_HID, "ACPI0007")
       Name (_UID, 5)
       Method (_STA)
       {
         Return (0xF)
       }
     }
   }
 
   Device (CLU2)
   {
     Name (_HID, "ACPI0010")
     Name (_UID, 11)
     Method (_STA)
     {
       Return (0xF)
     }
 
     Device (CPU6)
     {
       Name (_HID, "ACPI0007")
       Name (_UID, 6)
       Method (_STA)
       {
         Return (0xF)
       }
     }
 
     Device (CPU7)
     {
       Name (_HID, "ACPI0007")
       Name (_UID, 7)
       Method (_STA)
       {
         Return (0xF)
       }
     }
   }
 }
 