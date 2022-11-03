/** @file
*
*  Copyright (c) 2021, Rockchip Inc. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef __PALTFORM_LIB_H__
#define __PALTFORM_LIB_H__

#include <Uefi.h>

/**
  Check to see if any data is available to be read from the debug device.

  @retval TRUE       At least one byte of data is available to be read
  @retval FALSE      No data is available to be read

**/
VOID
EFIAPI
DwEmmcDxeIoMux ();

void
EFIAPI
SdhciEmmcDxeIoMux();

VOID
EFIAPI
DebugPrintHex(
VOID *buf,
UINT32 width,
UINT32 len);

EFI_STATUS
EFIAPI
SdhciGetClockRate (
  IN UINTN TargetFreq,
  OUT UINTN *BaseFreq
  );

void
EFIAPI
GmacIomux(
  UINT32 id
  );

void
EFIAPI
Rk806SpiIomux(void);

void
EFIAPI
NorFspiIomux(void);

void
EFIAPI
NorFspiEnableClock (
  UINT32 *CruBase
);

UINT32
EFIAPI
I2cGetBase (
   UINT32 id
);

void
EFIAPI
UdPhyU3PortDisable (void);

void
EFIAPI
UsbPortPowerEnable (void);

void
EFIAPI
Usb2PhySuspend (void);

void
EFIAPI
Usb2PhyResume (void);

void
EFIAPI
Dwc3Force20ClkFor30Clk (UINT32 Address, BOOLEAN enable);

void
EFIAPI
Pcie30IoInit(void);

void
EFIAPI
Pcie30PowerEn(void);

void
EFIAPI
Pcie30PeReset(BOOLEAN enable);

#endif
