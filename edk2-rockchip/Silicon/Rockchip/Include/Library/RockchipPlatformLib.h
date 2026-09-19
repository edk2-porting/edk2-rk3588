/** @file
*
*  Copyright (c) 2021, Rockchip Limited. All rights reserved.
*  Copyright (c) 2023-2025, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __PALTFORM_LIB_H__
#define __PALTFORM_LIB_H__

#include <Uefi.h>
#include <Protocol/Fusb302PlatformDevice.h>

VOID
EFIAPI
SdmmcIoMux (
  VOID
  );

VOID
EFIAPI
SdhciEmmcIoMux (
  VOID
  );

VOID
EFIAPI
EdpEnableBacklight (
  IN UINT32   Id,
  IN BOOLEAN  Enable
  );

VOID
EFIAPI
GmacIomux (
  IN UINT32  Id
  );

VOID
EFIAPI
GmacIoPhyReset (
  IN UINT32   Id,
  IN BOOLEAN  Enable
  );

VOID
EFIAPI
Rk806SpiIomux (
  VOID
  );

VOID
EFIAPI
Rk806Configure (
  VOID
  );

VOID
EFIAPI
SetCPULittleVoltage (
  IN UINT32  Microvolts
  );

VOID
EFIAPI
NorFspiIomux (
  VOID
  );

VOID
EFIAPI
NorFspiEnableClock (
  UINT32  *CruBase
  );

VOID
EFIAPI
I2cIomux (
  UINT32  id
  );

VOID
EFIAPI
UsbPortPowerEnable (
  VOID
  );

VOID
EFIAPI
Usb2PhyResume (
  VOID
  );

VOID
EFIAPI
PcieIoInit (
  UINT32  Segment
  );

VOID
EFIAPI
PciePowerEn (
  UINT32   Segment,
  BOOLEAN  Enable
  );

VOID
EFIAPI
PciePeReset (
  UINT32   Segment,
  BOOLEAN  Enable
  );

VOID
EFIAPI
HdmiTxIomux (
  IN UINT32  Id
  );

VOID
EFIAPI
PwmFanIoSetup (
  VOID
  );

VOID
EFIAPI
PwmFanSetSpeed (
  IN UINT32  Percentage
  );

VOID
EFIAPI
PlatformInitLeds (
  VOID
  );

VOID
EFIAPI
PlatformSetStatusLed (
  IN BOOLEAN  Enable
  );

CONST EFI_GUID *
EFIAPI
PlatformGetDtbFileGuid (
  IN UINT32  CompatMode
  );

VOID
EFIAPI
PlatformEarlyInit (
  VOID
  );

/**
  Describe one of this board's Type-C ports: what it may offer an attached
  sink, what it will accept from a source, and how to switch its supply.

  Called once per entry of PcdFusb302Addresses. Leaving SetVbus NULL is how
  a port says it cannot energise its rail, whatever it may otherwise offer.
  The caller fills in which I2C device the controller answers on and which
  combo PHY it feeds, so a board need not repeat either.

  @param[in]  PortIndex   Which port, indexed like PcdFusb302Addresses.
  @param[out] Port        Receives the description.

  @retval EFI_SUCCESS       The port was described.
  @retval EFI_UNSUPPORTED   This board has no such Type-C port.

**/
EFI_STATUS
EFIAPI
PlatformGetTypeCPort (
  IN  UINTN                             PortIndex,
  OUT FUSB302_PLATFORM_DEVICE_PROTOCOL  *Port
  );

#endif
