/** @file
  This file is to load sample board policy.

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PeiPchPolicyLibrary.h"
#include <Library/ConfigBlockLib.h>

/*
  Apply sample board PCH specific default settings

  @param[in] SiPolicy      The pointer to SI Policy PPI instance
*/
VOID
EFIAPI
PchLoadSamplePolicy (
  IN  SI_POLICY_PPI           *SiPolicy
  )
{
  UINTN                           Index;
  EFI_STATUS                      Status;
  PCH_PCIE_CONFIG                 *PcieRpConfig;
  PCH_IOAPIC_CONFIG               *IoApicConfig;
  PCH_CIO2_CONFIG                 *Cio2Config;
  PCH_DMI_CONFIG                  *DmiConfig;
  PCH_LAN_CONFIG                  *LanConfig;
  PCH_LOCK_DOWN_CONFIG            *LockDownConfig;
  PCH_PM_CONFIG                   *PmConfig;
  PCH_SCS_CONFIG                  *ScsConfig;
  PCH_SERIAL_IO_CONFIG            *SerialIoConfig;
  PCH_THERMAL_CONFIG              *ThermalConfig;
  PCH_USB_CONFIG                  *UsbConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPcieRpConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gCio2ConfigGuid, (VOID *) &Cio2Config);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLanConfigGuid, (VOID *) &LanConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gScsConfigGuid, (VOID *) &ScsConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // PCIE RP
  //
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    PcieRpConfig->RootPort[Index].ClkReqDetect                   = TRUE;
    PcieRpConfig->RootPort[Index].AdvancedErrorReporting         = TRUE;
    PcieRpConfig->RootPort[Index].Gen3EqPh3Method                = PchPcieEqSoftware;
  }

  PcieRpConfig->RootPort[0].ClkReqSupported = TRUE;
  PcieRpConfig->RootPort[0].ClkReqNumber = 2;

  PcieRpConfig->RootPort[4].ClkReqSupported = TRUE;
  PcieRpConfig->RootPort[4].ClkReqNumber = 3;

  PcieRpConfig->RootPort[5].ClkReqSupported = TRUE;
  PcieRpConfig->RootPort[5].ClkReqNumber = 1;

  PcieRpConfig->RootPort[8].ClkReqSupported = TRUE;
  PcieRpConfig->RootPort[8].ClkReqNumber = 5;

  PcieRpConfig->RootPort[9].ClkReqSupported = TRUE;
  PcieRpConfig->RootPort[9].ClkReqNumber = 4;

  //
  // USB
  //
  UsbConfig->PortUsb20[0].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[0].Afe.Txiset       = 0;
  UsbConfig->PortUsb20[0].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[0].Afe.Pehalfbit    = 1;

  UsbConfig->PortUsb20[1].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[1].Afe.Txiset       = 0;
  UsbConfig->PortUsb20[1].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[1].Afe.Pehalfbit    = 1;

  UsbConfig->PortUsb20[2].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[2].Afe.Txiset       = 0;
  UsbConfig->PortUsb20[2].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[2].Afe.Pehalfbit    = 1;

  UsbConfig->PortUsb20[3].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[3].Afe.Txiset       = 0;
  UsbConfig->PortUsb20[3].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[3].Afe.Pehalfbit    = 1;

  UsbConfig->PortUsb20[4].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[4].Afe.Txiset       = 0;
  UsbConfig->PortUsb20[4].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[4].Afe.Pehalfbit    = 1;

  UsbConfig->PortUsb20[5].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[5].Afe.Txiset       = 0;
  UsbConfig->PortUsb20[5].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[5].Afe.Pehalfbit    = 1;

  UsbConfig->PortUsb20[6].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[6].Afe.Txiset       = 0;
  UsbConfig->PortUsb20[6].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[6].Afe.Pehalfbit    = 1;

  UsbConfig->PortUsb20[7].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[7].Afe.Txiset       = 0;
  UsbConfig->PortUsb20[7].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[7].Afe.Pehalfbit    = 1;

  UsbConfig->PortUsb20[8].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[8].Afe.Txiset       = 5;
  UsbConfig->PortUsb20[8].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[8].Afe.Pehalfbit    = 1;

  UsbConfig->PortUsb20[9].Afe.Petxiset     = 7;
  UsbConfig->PortUsb20[9].Afe.Txiset       = 0;
  UsbConfig->PortUsb20[9].Afe.Predeemp     = 2;
  UsbConfig->PortUsb20[9].Afe.Pehalfbit    = 1;

  // OC Map for USB2 Ports
  UsbConfig->PortUsb20[ 0].OverCurrentPin = PchUsbOverCurrentPin0;
  UsbConfig->PortUsb20[ 1].OverCurrentPin = PchUsbOverCurrentPin2;
  UsbConfig->PortUsb20[ 2].OverCurrentPin = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb20[ 3].OverCurrentPin = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb20[ 4].OverCurrentPin = PchUsbOverCurrentPin2;
  UsbConfig->PortUsb20[ 5].OverCurrentPin = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb20[ 6].OverCurrentPin = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb20[ 7].OverCurrentPin = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb20[ 8].OverCurrentPin = PchUsbOverCurrentPin1;
  UsbConfig->PortUsb20[ 9].OverCurrentPin = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb20[10].OverCurrentPin = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb20[11].OverCurrentPin = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb20[12].OverCurrentPin = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb20[13].OverCurrentPin = PchUsbOverCurrentPinSkip;

  // OC Map for USB3 Ports
  UsbConfig->PortUsb30[0].OverCurrentPin  = PchUsbOverCurrentPin0;
  UsbConfig->PortUsb30[1].OverCurrentPin  = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb30[2].OverCurrentPin  = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb30[3].OverCurrentPin  = PchUsbOverCurrentPin1;
  UsbConfig->PortUsb30[4].OverCurrentPin  = PchUsbOverCurrentPinSkip;
  UsbConfig->PortUsb30[5].OverCurrentPin  = PchUsbOverCurrentPinSkip;

  UsbConfig->SsicConfig.SsicPort[0].Enable = TRUE;
  UsbConfig->SsicConfig.SsicPort[1].Enable = TRUE;

  //
  // IOAPIC
  //
  IoApicConfig->BdfValid          = 1;
  IoApicConfig->BusNumber         = 0xF0;
  IoApicConfig->DeviceNumber      = 0x1F;
  IoApicConfig->FunctionNumber    = 0;

  //
  // LAN
  //
  LanConfig->K1OffEnable     = TRUE;
  LanConfig->ClkReqSupported = TRUE;
  LanConfig->ClkReqNumber    = 3;

  //
  // PM CONFIG
  //
  PmConfig->LpcClockRun       = TRUE;

  //
  // DMI
  //
  DmiConfig->PwrOptEnable     = TRUE;

  //
  // SERIALIO
  //
  SerialIoConfig->DevMode[PchSerialIoIndexI2C0]  = PchSerialIoPci;
  SerialIoConfig->DevMode[PchSerialIoIndexI2C1]  = PchSerialIoPci;
  SerialIoConfig->DevMode[PchSerialIoIndexI2C2]  = PchSerialIoDisabled;
  SerialIoConfig->DevMode[PchSerialIoIndexI2C3]  = PchSerialIoDisabled;
  SerialIoConfig->DevMode[PchSerialIoIndexI2C4]  = PchSerialIoAcpiHidden;
  SerialIoConfig->DevMode[PchSerialIoIndexI2C5]  = PchSerialIoDisabled;
  SerialIoConfig->DevMode[PchSerialIoIndexSpi0]  = PchSerialIoDisabled;
  SerialIoConfig->DevMode[PchSerialIoIndexSpi1]  = PchSerialIoDisabled;
  SerialIoConfig->DevMode[PchSerialIoIndexUart0] = PchSerialIoPci;
  SerialIoConfig->DevMode[PchSerialIoIndexUart1] = PchSerialIoDisabled;
  SerialIoConfig->DevMode[PchSerialIoIndexUart2] = PchSerialIoLegacyUart;

  SerialIoConfig->I2cVoltage[2] = 1;
  SerialIoConfig->I2cVoltage[3] = 1;

  SerialIoConfig->SpiCsPolarity[0] = 1;

  SerialIoConfig->UartHwFlowCtrl[0] = 1;
  SerialIoConfig->UartHwFlowCtrl[1] = 1;
  SerialIoConfig->UartHwFlowCtrl[2] = 1;


  //
  // SCS
  //
  ScsConfig->ScsEmmcHs400Enabled        = TRUE;
  ScsConfig->ScsEmmcHs400TuningRequired = TRUE;
}
