/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>

#include <Library/PcdLib.h>
#include <PchLimits.h>
#include <PchPolicyCommon.h>

PCH_USB_OVERCURRENT_PIN Usb20OverCurrentMappings[PCH_MAX_USB2_PORTS] = {
                          PchUsbOverCurrentPinSkip,  //1  BMC,skip
                          PchUsbOverCurrentPinSkip,  //2  BMC,skip
                          PchUsbOverCurrentPin0,     //3  USB REAR PANEL, OC0
                          PchUsbOverCurrentPin1,     //4  USB REAR PANEL, OC1
                          PchUsbOverCurrentPin1,     //5  USB REAR PANEL, OC1
                          PchUsbOverCurrentPinSkip,  //6  Internal USB3.0, NC, skip(org OC2 in schematic)
                          PchUsbOverCurrentPinSkip,  //7  NC, skip
                          PchUsbOverCurrentPin4,     //8  Internal USB2.0, OC4
                          PchUsbOverCurrentPinSkip,  //9  NC, skip
                          PchUsbOverCurrentPinSkip,  //10 NC, skip
                          PchUsbOverCurrentPin6,     //11 USB FRONT PANEL, OC6
                          PchUsbOverCurrentPin5,     //12 USB STORAGE FRONT PANNEL, OC5
                          PchUsbOverCurrentPin6,     //13 USB FRONT PANEL, OC6
                          PchUsbOverCurrentPin5,     //14 USB STORAGE FRONT PANNEL, OC5
                          PchUsbOverCurrentPinSkip,
                          PchUsbOverCurrentPinSkip
                       };

PCH_USB_OVERCURRENT_PIN Usb30OverCurrentMappings[PCH_MAX_USB3_PORTS] = {
                          PchUsbOverCurrentPin6,    //1 USB FRONT PANEL, OC6
                          PchUsbOverCurrentPin6,    //2 USB FRONT PANEL, OC6
                          PchUsbOverCurrentPin0,    //3 USB REAR PANEL, OC0
                          PchUsbOverCurrentPin1,    //4 USB REAR PANEL, OC1
                          PchUsbOverCurrentPin1,    //5 USB REAR PANEL, OC1
                          PchUsbOverCurrentPinSkip, //6 Internal USB3.0, NC, skip(org OC2 in schematic)
                          PchUsbOverCurrentPinSkip,
                          PchUsbOverCurrentPinSkip,
                          PchUsbOverCurrentPinSkip,
                          PchUsbOverCurrentPinSkip
                       };

