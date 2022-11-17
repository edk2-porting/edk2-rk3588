/** @file
  This file containa all erratas need to be applied on different SoCs.

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/ArmLib.h>
#include <Library/BaseLib.h>
#include <Library/ChassisLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#include "Erratum.h"

/*
* A-008997   : USB3 LFPS peak-to-peak differential output
*              voltage adjustment settings
* Affects    : USB
* Description: Low Frequency Periodic Signaling (LFPS)
*              peak-to-peak differential output voltage test
*              compliance fails using default transmitter settings.
*              Software is required to change the transmitter
*              signal swings to pass compliance tests.
* Impact     : LFPS peak-to-peak differential output voltage
*              compliance test fails.
*/
VOID
ErratumA008997 (
  VOID
  )
{
  NXP_LAYERSCAPE_CHASSIS2_SUPPLEMENTAL_CONFIG *Scfg;
  UINT32 Value;

  Scfg = (NXP_LAYERSCAPE_CHASSIS2_SUPPLEMENTAL_CONFIG *)NXP_LAYERSCAPE_CHASSIS2_SCFG_ADDRESS;

  Value = ScfgRead32 ((UINTN)&Scfg->Usb1Prm2Cr);
  Value &= ~(0x7F << 9);
  ScfgWrite32 ((UINTN)&Scfg->Usb1Prm2Cr, Value | (USB_PCSTXSWINGFULL << 9));
  Value = ScfgRead32 ((UINTN)&Scfg->Usb2Prm2Cr);
  Value &= ~(0x7F << 9);
  ScfgWrite32 ((UINTN)&Scfg->Usb2Prm2Cr, Value | (USB_PCSTXSWINGFULL << 9));
  Value = ScfgRead32 ((UINTN)&Scfg->Usb3Prm2Cr);
  Value &= ~(0x7F << 9);
  ScfgWrite32 ((UINTN)&Scfg->Usb3Prm2Cr, Value | (USB_PCSTXSWINGFULL << 9));

  return;
}

/*
* A-009007   : USB3PHY observing intermittent failure in
*              receive compliance tests at higher jitter frequency
*              using default register values
* Affects    : USB
* Description: Receive compliance tests may fail intermittently at
*              high jitter frequencies using default register values.
* Impact     : Receive compliance test fails at default register setting.
*/

VOID
ConfigUsbLane0 (
  IN  UINTN UsbPhy
  )
{
  UINTN     RegAddress;

  RegAddress = UsbPhy + USB_PHY_RX_OVRD_IN_HI;

  ArmDataMemoryBarrier ();
  MmioWrite16 (RegAddress, USB_PHY_RX_EQ_VAL_1);
  ArmDataMemoryBarrier ();
  MmioWrite16 (RegAddress, USB_PHY_RX_EQ_VAL_2);
  ArmDataMemoryBarrier ();
  MmioWrite16 (RegAddress, USB_PHY_RX_EQ_VAL_3);
  ArmDataMemoryBarrier ();
  MmioWrite16 (RegAddress, USB_PHY_RX_EQ_VAL_4);

  return;
}

VOID
ErratumA009007 (
  VOID
  )
{
  UINTN     UsbPhy;

  UsbPhy = USB_PHY1_BASE_ADDRESS;
  ConfigUsbLane0 (UsbPhy);

  UsbPhy = USB_PHY2_BASE_ADDRESS;
  ConfigUsbLane0 (UsbPhy);

  UsbPhy = USB_PHY3_BASE_ADDRESS;
  ConfigUsbLane0 (UsbPhy);

  return;
}

/*
* A-009008   : USB High Speed (HS) eye height adjustment
* Affects    : USB
* Description: USB HS eye diagram fails with the default
*              value at many corners, particularly at a
*              high temperature (105°C).
* Impact     : USB HS eye diagram may fail using the default value.
*/
VOID
ErratumA009008 (
  VOID
  )
{
  NXP_LAYERSCAPE_CHASSIS2_SUPPLEMENTAL_CONFIG *Scfg;
  UINT32 Value;

  Scfg = (NXP_LAYERSCAPE_CHASSIS2_SUPPLEMENTAL_CONFIG *)NXP_LAYERSCAPE_CHASSIS2_SCFG_ADDRESS;

  Value = ScfgRead32 ((UINTN)&Scfg->Usb1Prm1Cr);
  Value &= ~(0xF << 6);
  ScfgWrite32 ((UINTN)&Scfg->Usb1Prm1Cr, Value|(USB_TXVREFTUNE << 6));
  Value = ScfgRead32 ((UINTN)&Scfg->Usb2Prm1Cr);
  Value &= ~(0xF << 6);
  ScfgWrite32 ((UINTN)&Scfg->Usb2Prm1Cr, Value|(USB_TXVREFTUNE << 6));
  Value = ScfgRead32 ((UINTN)&Scfg->Usb3Prm1Cr);
  Value &= ~(0xF << 6);
  ScfgWrite32 ((UINTN)&Scfg->Usb3Prm1Cr, Value|(USB_TXVREFTUNE << 6));

  return;
}

/*
* A-009798   : USB high speed squelch threshold adjustment
* Affects    : USB
* Description: The default setting for USB high speed
*              squelch threshold results in a threshold close
*              to or lower than 100mV. This leads to a receiver
*              compliance test failure for a 100mV threshold.
* Impact     : If the errata is not applied, only the USB high
*              speed receiver sensitivity compliance test fails,
*              however USB data continues to transfer.
*/
VOID
ErratumA009798 (
  VOID
  )
{
  NXP_LAYERSCAPE_CHASSIS2_SUPPLEMENTAL_CONFIG *Scfg;
  UINT32 Value;

  Scfg = (NXP_LAYERSCAPE_CHASSIS2_SUPPLEMENTAL_CONFIG *)NXP_LAYERSCAPE_CHASSIS2_SCFG_ADDRESS;

  Value = ScfgRead32 ((UINTN)&Scfg->Usb1Prm1Cr);
  ScfgWrite32 ((UINTN)&Scfg->Usb1Prm1Cr, Value & USB_SQRXTUNE);
  Value = ScfgRead32 ((UINTN)&Scfg->Usb2Prm1Cr);
  ScfgWrite32 ((UINTN)&Scfg->Usb2Prm1Cr, Value & USB_SQRXTUNE);
  Value = ScfgRead32 ((UINTN)&Scfg->Usb3Prm1Cr);
  ScfgWrite32 ((UINTN)&Scfg->Usb3Prm1Cr, Value & USB_SQRXTUNE);

  return;
}
