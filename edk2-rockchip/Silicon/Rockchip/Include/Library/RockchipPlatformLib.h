/** @file
*
*  Copyright (c) 2021, Rockchip Limited. All rights reserved.
*  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __PALTFORM_LIB_H__
#define __PALTFORM_LIB_H__

#include <Uefi.h>

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
EnableBacklight (
  IN BOOLEAN  en
  );

VOID
EFIAPI
EnablePWM (
  IN BOOLEAN  en
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

#endif
