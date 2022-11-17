/** @file
  USB3 debug Port Parameter library instance based on PCD.

  Copyright (c) 2013 - 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/Usb3DebugPortParamLib.h>

/**
  Returns the USB3 debug port controller.
  bit:  0~ 7: Function
  bit:  8~15: Device
  bit: 16~24: Bus

  @return  Controller information of USB debug port.

**/
UINT32
EFIAPI
GetUsb3DebugPortController (
  VOID
  )
{
  USB3_DEBUG_PORT_CONTROLLER       UsbDebugPort;

  UsbDebugPort.Controller = 0;
  UsbDebugPort.PciAddress.Bus = PcdGet8(PcdUsbSerialXhciBus);
  UsbDebugPort.PciAddress.Device = PcdGet8(PcdUsbSerialXhciDev);
  UsbDebugPort.PciAddress.Function = PcdGet8(PcdUsbSerialXhciFunc);

  return UsbDebugPort.Controller;
}

/**
  Sets the USB debug port controller.
  bit:  0~ 7: Function
  bit:  8~15: Device
  bit: 16~24: Bus

  @param[in]    Controller information of USB debug port value to be set.

  @retval  TRUE           The controller of USB debug port was sucessfully set.
  @retval  FALSE          The controller of USB debug port could not be set.

**/
BOOLEAN
EFIAPI
SetUsb3DebugPortController (
  UINT32        Controller
  )
{
  return FALSE;
}
