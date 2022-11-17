/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiPchPolicyLibrary.h"

/*
  Apply RVP3 PCH specific default settings

  @param[in] PchPolicyPpi      The pointer to PCH Policy PPI instance
*/
VOID
EFIAPI
PchRvp3DefaultPolicy (
  IN  PCH_POLICY_PPI           *PchPolicy
  )
{
  UINTN Index;

  //
  // PCIE RP
  //
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    PchPolicy->PcieConfig.RootPort[Index].ClkReqDetect                   = TRUE;
    PchPolicy->PcieConfig.RootPort[Index].AdvancedErrorReporting         = TRUE;
  }

  PchPolicy->PcieConfig.RootPort[0].ClkReqSupported = TRUE;
  PchPolicy->PcieConfig.RootPort[0].ClkReqNumber = 2;
  PchPolicy->HsioPcieConfig.Lane[0].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[0].HsioRxSetCtle = 6;

  PchPolicy->HsioPcieConfig.Lane[1].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[1].HsioRxSetCtle = 6;

  PchPolicy->HsioPcieConfig.Lane[2].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[2].HsioRxSetCtle = 6;

  PchPolicy->HsioPcieConfig.Lane[3].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[3].HsioRxSetCtle = 6;

  PchPolicy->PcieConfig.RootPort[4].ClkReqSupported = TRUE;
  PchPolicy->PcieConfig.RootPort[4].ClkReqNumber = 3;

  PchPolicy->PcieConfig.RootPort[5].ClkReqSupported = TRUE;
  PchPolicy->PcieConfig.RootPort[5].ClkReqNumber = 1;
  PchPolicy->HsioPcieConfig.Lane[5].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[5].HsioRxSetCtle = 8;

  PchPolicy->HsioPcieConfig.Lane[7].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[7].HsioRxSetCtle = 8;

  PchPolicy->PcieConfig.RootPort[8].ClkReqSupported = TRUE;
  PchPolicy->PcieConfig.RootPort[8].ClkReqNumber = 5;
  PchPolicy->HsioPcieConfig.Lane[8].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[8].HsioRxSetCtle = 8;

  PchPolicy->PcieConfig.RootPort[9].ClkReqSupported = TRUE;
  PchPolicy->PcieConfig.RootPort[9].ClkReqNumber = 4;
  PchPolicy->HsioPcieConfig.Lane[9].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[9].HsioRxSetCtle = 8;

  PchPolicy->HsioPcieConfig.Lane[10].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[10].HsioRxSetCtle = 8;

  PchPolicy->HsioPcieConfig.Lane[11].HsioRxSetCtleEnable = TRUE;
  PchPolicy->HsioPcieConfig.Lane[11].HsioRxSetCtle = 8;

  //
  // SATA
  //
  PchPolicy->HsioSataConfig.PortLane[0].HsioRxGen3EqBoostMagEnable = TRUE;
  PchPolicy->HsioSataConfig.PortLane[0].HsioRxGen3EqBoostMag = 4;
  PchPolicy->HsioSataConfig.PortLane[0].HsioTxGen1DownscaleAmpEnable = TRUE;
  PchPolicy->HsioSataConfig.PortLane[0].HsioTxGen1DownscaleAmp = 0x2C;
  PchPolicy->HsioSataConfig.PortLane[0].HsioTxGen2DownscaleAmpEnable = 0;
  PchPolicy->HsioSataConfig.PortLane[0].HsioTxGen2DownscaleAmp = 0;

  //
  // USB
  //
  PchPolicy->UsbConfig.PortUsb20[0].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[0].Afe.Txiset       = 0;
  PchPolicy->UsbConfig.PortUsb20[0].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[0].Afe.Pehalfbit    = 1;

  PchPolicy->UsbConfig.PortUsb20[1].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[1].Afe.Txiset       = 0;
  PchPolicy->UsbConfig.PortUsb20[1].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[1].Afe.Pehalfbit    = 1;

  PchPolicy->UsbConfig.PortUsb20[2].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[2].Afe.Txiset       = 0;
  PchPolicy->UsbConfig.PortUsb20[2].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[2].Afe.Pehalfbit    = 1;

  PchPolicy->UsbConfig.PortUsb20[3].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[3].Afe.Txiset       = 0;
  PchPolicy->UsbConfig.PortUsb20[3].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[3].Afe.Pehalfbit    = 1;

  PchPolicy->UsbConfig.PortUsb20[4].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[4].Afe.Txiset       = 0;
  PchPolicy->UsbConfig.PortUsb20[4].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[4].Afe.Pehalfbit    = 1;

  PchPolicy->UsbConfig.PortUsb20[5].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[5].Afe.Txiset       = 0;
  PchPolicy->UsbConfig.PortUsb20[5].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[5].Afe.Pehalfbit    = 1;

  PchPolicy->UsbConfig.PortUsb20[6].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[6].Afe.Txiset       = 0;
  PchPolicy->UsbConfig.PortUsb20[6].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[6].Afe.Pehalfbit    = 1;

  PchPolicy->UsbConfig.PortUsb20[7].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[7].Afe.Txiset       = 0;
  PchPolicy->UsbConfig.PortUsb20[7].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[7].Afe.Pehalfbit    = 1;

  PchPolicy->UsbConfig.PortUsb20[8].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[8].Afe.Txiset       = 5;
  PchPolicy->UsbConfig.PortUsb20[8].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[8].Afe.Pehalfbit    = 1;

  PchPolicy->UsbConfig.PortUsb20[9].Afe.Petxiset     = 7;
  PchPolicy->UsbConfig.PortUsb20[9].Afe.Txiset       = 0;
  PchPolicy->UsbConfig.PortUsb20[9].Afe.Predeemp     = 2;
  PchPolicy->UsbConfig.PortUsb20[9].Afe.Pehalfbit    = 1;

  // OC Map for USB2 Ports
  PchPolicy->UsbConfig.PortUsb20[ 0].OverCurrentPin = PchUsbOverCurrentPin0;
  PchPolicy->UsbConfig.PortUsb20[ 1].OverCurrentPin = PchUsbOverCurrentPin2;
  PchPolicy->UsbConfig.PortUsb20[ 2].OverCurrentPin = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb20[ 3].OverCurrentPin = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb20[ 4].OverCurrentPin = PchUsbOverCurrentPin2;
  PchPolicy->UsbConfig.PortUsb20[ 5].OverCurrentPin = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb20[ 6].OverCurrentPin = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb20[ 7].OverCurrentPin = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb20[ 8].OverCurrentPin = PchUsbOverCurrentPin1;
  PchPolicy->UsbConfig.PortUsb20[ 9].OverCurrentPin = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb20[10].OverCurrentPin = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb20[11].OverCurrentPin = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb20[12].OverCurrentPin = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb20[13].OverCurrentPin = PchUsbOverCurrentPinSkip;

  // OC Map for USB3 Ports
  PchPolicy->UsbConfig.PortUsb30[0].OverCurrentPin  = PchUsbOverCurrentPin0;
  PchPolicy->UsbConfig.PortUsb30[1].OverCurrentPin  = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb30[2].OverCurrentPin  = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb30[3].OverCurrentPin  = PchUsbOverCurrentPin1;
  PchPolicy->UsbConfig.PortUsb30[4].OverCurrentPin  = PchUsbOverCurrentPinSkip;
  PchPolicy->UsbConfig.PortUsb30[5].OverCurrentPin  = PchUsbOverCurrentPinSkip;

  PchPolicy->UsbConfig.SsicConfig.SsicPort[0].Enable = TRUE;
  PchPolicy->UsbConfig.SsicConfig.SsicPort[1].Enable = TRUE;

  //
  // IOAPIC
  //
  PchPolicy->IoApicConfig.BdfValid          = 1;
  PchPolicy->IoApicConfig.BusNumber         = 0xF0;
  PchPolicy->IoApicConfig.DeviceNumber      = 0x1F;
  PchPolicy->IoApicConfig.FunctionNumber    = 0;

  //
  // LAN
  //
  PchPolicy->LanConfig.K1OffEnable     = TRUE;
  PchPolicy->LanConfig.ClkReqSupported = TRUE;
  PchPolicy->LanConfig.ClkReqNumber    = 3;

  //
  // LOCK DOWN
  //
  PchPolicy->LockDownConfig.SpiEiss     = TRUE;
  PchPolicy->LockDownConfig.BiosLock    = TRUE;

  //
  // THERMAL
  //
  PchPolicy->ThermalConfig.ThermalThrottling.TTLevels.PchCrossThrottling = FALSE;

  //
  // PM CONFIG
  //
  PchPolicy->PmConfig.PciClockRun       = TRUE;

  //
  // DMI
  //
  PchPolicy->DmiConfig.PwrOptEnable     = TRUE;


  //
  // TRACEHUB
  //
  PchPolicy->PchTraceHubConfig.MemReg0Size = 0x100000;  // 1MB
  PchPolicy->PchTraceHubConfig.MemReg1Size = 0x100000;  // 1MB

}
