/** @file
  This file configures Aspire VN7-572G board-specific policies.

  Copyright (c) 2021, Baruch Binyamin Doron
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <CpuPolicyCommon.h>
#include <PchPolicyCommon.h>
#include <PchPreMemPolicyCommon.h>
#include <SaPolicyCommon.h>

/* FIXME/NB: Bring back in-line with API mode policies */

#define SA_VR           0
#define IA_VR           1
#define GT_UNSLICED_VR  2
#define GT_SLICED_VR    3

/**
  Performs the remainder of board-specific FSP Policy initialization.

  @param[in]       Policy              Policy PPI pointer.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspBoardPolicyUpdatePreMem (
  IN VOID *Policy
  )
{
  EFI_STATUS                 Status;
  SA_MISC_PEI_PREMEM_CONFIG  *MiscPeiPreMemConfig;
  MEMORY_CONFIGURATION       *MemConfig;
  PCH_HPET_PREMEM_CONFIG     *HpetPreMemConfig;

  // Retrieve the config blocks we depend on
  Status = GetConfigBlock (Policy, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (MiscPeiPreMemConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR (Status);
  if (MemConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gHpetPreMemConfigGuid, (VOID *) &HpetPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (HpetPreMemConfig == NULL) {
    return EFI_NOT_FOUND;
  }

  /* System Agent config */
  MiscPeiPreMemConfig->UserBd = PcdGet8(PcdSaMiscUserBd);
  MemConfig->DqPinsInterleaved = (UINT8)PcdGetBool(PcdMrcDqPinsInterleaved);
  MemConfig->CaVrefConfig = PcdGet8(PcdMrcCaVrefConfig);
  MemConfig->SaGv = 3;  // Enabled

  // TODO: Why should this be here?
  // FSP should program it's default BDF value (but where is bus 0xF0?)
  HpetPreMemConfig->BdfValid = 1;

  /* iGFX config */
//FIXME  FspmUpd->FspmConfig.PrimaryDisplay = 4;  // Switchable Graphics

  return EFI_SUCCESS;
}

/**
  Performs the remainder of board-specific FSP Policy initialization.

  @param[in]       Policy              Policy PPI pointer.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspBoardPolicyUpdate (
  IN VOID *Policy
  )
{
  EFI_STATUS                   Status;
  PCH_LOCK_DOWN_CONFIG         *LockDownConfig;
  PCH_GENERAL_CONFIG           *PchGeneralConfig;
  CPU_POWER_MGMT_BASIC_CONFIG  *CpuPowerMgmtBasicConfig;
  CPU_POWER_MGMT_VR_CONFIG     *CpuPowerMgmtVrConfig;
  PCH_USB_CONFIG               *UsbConfig;
  PCH_SATA_CONFIG              *SataConfig;
  PCH_PCIE_CONFIG              *PchPcieConfig;
  PCH_PM_CONFIG                *PmConfig;
  PCH_LPC_SIRQ_CONFIG          *SerialIrqConfig;
  PCH_HDAUDIO_CONFIG           *HdAudioConfig;
  PCH_IOAPIC_CONFIG            *IoApicConfig;
  INTN                         Index;

  // Retrieve the config blocks we depend on (all are expected to be installed)
  Status = GetConfigBlock (Policy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  if (LockDownConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  if (PchGeneralConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);
  if (CpuPowerMgmtBasicConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gCpuPowerMgmtVrConfigGuid, (VOID *) &CpuPowerMgmtVrConfig);
  ASSERT_EFI_ERROR (Status);
  if (CpuPowerMgmtVrConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  if (UsbConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);
  if (SataConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gPcieRpConfigGuid, (VOID *) &PchPcieConfig);
  ASSERT_EFI_ERROR (Status);
  if (PchPcieConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  if (PmConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gSerialIrqConfigGuid, (VOID *) &SerialIrqConfig);
  ASSERT_EFI_ERROR (Status);
  if (SerialIrqConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  if (HdAudioConfig == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = GetConfigBlock (Policy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  if (IoApicConfig == NULL) {
    return EFI_NOT_FOUND;
  }

  // FIXME/NB: This is insecure and not production-ready!
  // TODO: Configure SPI lockdown by variable on FrontPage?
  LockDownConfig->BiosLock = 0;
  LockDownConfig->SpiEiss = 0;

  // TODO: Why should this be here?
  // FSP should program it's default BDF value (but where is bus 0xF0?)
  IoApicConfig->BdfValid = 1;

  // Note: SerialIoDevMode default is satisfactory, but not entirely accurate.
  //       Board has no GPIO expander on I2C4 (despite SetupUtility claim
  //       that it does - this appears to be static text?) and is UART0 merely supporting
  //       the UART2 devfn?

  // Acer IDs (TODO: "Newgate" IDs)
//FIXME  FspsUpd->FspsConfig.DefaultSvid = 0x1025;
//FIXME  FspsUpd->FspsConfig.DefaultSid = 0x1037;
  PchGeneralConfig->SubSystemVendorId = 0x1025;
  PchGeneralConfig->SubSystemId = 0x1037;

  /* System Agent config */
  // Set the Thermal Control Circuit (TCC) activation value to 97C
  // even though FSP integration guide says to set it to 100C for SKL-U
  // (offset at 0), because when the TCC activates at 100C, the CPU
  // will have already shut itself down from overheating protection.
  CpuPowerMgmtBasicConfig->TccActivationOffset = 3;

  // VR Slew rate setting for improving audible noise
  CpuPowerMgmtVrConfig->AcousticNoiseMitigation = 1;
  CpuPowerMgmtVrConfig->SlowSlewRateForIa = 3;  // Fast/16
  CpuPowerMgmtVrConfig->SlowSlewRateForGt = 3;  // Fast/16
  CpuPowerMgmtVrConfig->SlowSlewRateForSa = 0;  // Fast/2
  CpuPowerMgmtVrConfig->FastPkgCRampDisableIa = 0;
  CpuPowerMgmtVrConfig->FastPkgCRampDisableGt = 0;
  CpuPowerMgmtVrConfig->FastPkgCRampDisableSa = 0;

  // VR domain configuration (copied from board port, before VR config moved
  // to SoC. Should match SKL-U (GT2, 15W) in the SKL-U datasheet, vol. 1
  CpuPowerMgmtVrConfig->AcLoadline[SA_VR] = 1030;  // 10.3mOhm (in 1/100 increments)
  CpuPowerMgmtVrConfig->DcLoadline[SA_VR] = 1030;  // 10.3mOhm (in 1/100 increments)
  CpuPowerMgmtVrConfig->Psi1Threshold[SA_VR] = 80; // 20A (in 1/4 increments)
  CpuPowerMgmtVrConfig->Psi2Threshold[SA_VR] = 16; // 4A (in 1/4 increments)
  CpuPowerMgmtVrConfig->Psi3Threshold[SA_VR] = 4;  // 1A (in 1/4 increments)
  CpuPowerMgmtVrConfig->IccMax[SA_VR] = 18;        // 4.5A (in 1/4 increments)
  CpuPowerMgmtVrConfig->VrVoltageLimit[SA_VR] = 1520;  // 1520mV

  CpuPowerMgmtVrConfig->AcLoadline[IA_VR] = 240;  // 2.4mOhm (in 1/100 increments)
  CpuPowerMgmtVrConfig->DcLoadline[IA_VR] = 240;  // 2.4mOhm (in 1/100 increments)
  CpuPowerMgmtVrConfig->Psi1Threshold[IA_VR] = 80; // 20A (in 1/4 increments)
  CpuPowerMgmtVrConfig->Psi2Threshold[IA_VR] = 20; // 5A (in 1/4 increments)
  CpuPowerMgmtVrConfig->Psi3Threshold[IA_VR] = 4;  // 1A (in 1/4 increments)
  CpuPowerMgmtVrConfig->IccMax[IA_VR] = 116;       // 29A (in 1/4 increments)
  CpuPowerMgmtVrConfig->VrVoltageLimit[IA_VR] = 1520;  // 1520mV

  CpuPowerMgmtVrConfig->AcLoadline[GT_UNSLICED_VR] = 310;  // 3.1mOhm (in 1/100 increments)
  CpuPowerMgmtVrConfig->DcLoadline[GT_UNSLICED_VR] = 310;  // 3.1mOhm (in 1/100 increments)
  CpuPowerMgmtVrConfig->Psi1Threshold[GT_UNSLICED_VR] = 80; // 20A (in 1/4 increments)
  CpuPowerMgmtVrConfig->Psi2Threshold[GT_UNSLICED_VR] = 20; // 5A (in 1/4 increments)
  CpuPowerMgmtVrConfig->Psi3Threshold[GT_UNSLICED_VR] = 4;  // 1A (in 1/4 increments)
  CpuPowerMgmtVrConfig->IccMax[GT_UNSLICED_VR] = 124;       // 31A (in 1/4 increments)
  CpuPowerMgmtVrConfig->VrVoltageLimit[GT_UNSLICED_VR] = 1520;  // 1520mV

  CpuPowerMgmtVrConfig->AcLoadline[GT_SLICED_VR] = 310;  // 3.1mOhm (in 1/100 increments)
  CpuPowerMgmtVrConfig->DcLoadline[GT_SLICED_VR] = 310;  // 3.1mOhm (in 1/100 increments)
  CpuPowerMgmtVrConfig->Psi1Threshold[GT_SLICED_VR] = 80; // 20A (in 1/4 increments)
  CpuPowerMgmtVrConfig->Psi2Threshold[GT_SLICED_VR] = 20; // 5A (in 1/4 increments)
  CpuPowerMgmtVrConfig->Psi3Threshold[GT_SLICED_VR] = 4;  // 1A (in 1/4 increments)
  CpuPowerMgmtVrConfig->IccMax[GT_SLICED_VR] = 124;       // 31A (in 1/4 increments)
  CpuPowerMgmtVrConfig->VrVoltageLimit[GT_SLICED_VR] = 1520;  // 1520mV

  // PL1, PL2 override 35W, PL4 override 43W (in 125 mW increments)
  CpuPowerMgmtBasicConfig->PowerLimit1 = 280;
  CpuPowerMgmtBasicConfig->PowerLimit2Power = 280;
  CpuPowerMgmtBasicConfig->PowerLimit4 = 344;

  // ISL95857 VR
  // Send VR specific command for PS4 exit issue
  CpuPowerMgmtVrConfig->SendVrMbxCmd1 = 2;
  // Send VR mailbox command for IA/GT/SA rails
//FIXME  FspsUpd->FspsConfig.IslVrCmd = 2;

  /* Skycam config */
//  FspsUpd->FspsConfig.SaImguEnable = 0;
//  FspsUpd->FspsConfig.PchCio2Enable = 0;

  /* Sensor hub config */
//  FspsUpd->FspsConfig.PchIshEnable = 0;

  /* xHCI config */
//  FspsUpd->FspsConfig.SsicPortEnable = 0;
  // Configure USB2 ports in two blocks
  for (Index = 0; Index < 3; Index++) {
    UsbConfig->PortUsb20[Index].Afe.Txiset = 0x2;  // 16.9mV
    UsbConfig->PortUsb20[Index].Afe.Predeemp = 1;  // De-emphasis on
    UsbConfig->PortUsb20[Index].Afe.Petxiset = 0x3;// 28.15mV
    UsbConfig->PortUsb20[Index].Afe.Pehalfbit = 1; // Half-bit
  }
  for (Index = 3; Index < 9; Index++) {
    UsbConfig->PortUsb20[Index].Afe.Txiset = 0;    // 0mV
    UsbConfig->PortUsb20[Index].Afe.Predeemp = 0x2;// Pre-emphasis and de-emphasis on
    UsbConfig->PortUsb20[Index].Afe.Petxiset = 0x7;// 56.3mV
    UsbConfig->PortUsb20[Index].Afe.Pehalfbit = 1; // Half-bit
  }
  // Configure all USB3 ports
  for (Index = 0; Index < 4; Index++) {
    UsbConfig->PortUsb30[Index].HsioTxDeEmphEnable = 1;
    UsbConfig->PortUsb30[Index].HsioTxDeEmph = 0x29;  // Default (approximately -3.5dB de-emphasis)
  }
  // Disable all OC pins
  for (Index = 0; Index < 9; Index++) {
    UsbConfig->PortUsb20[Index].OverCurrentPin = PchUsbOverCurrentPinSkip;
  }
  for (Index = 0; Index < 4; Index++) {
    UsbConfig->PortUsb30[Index].OverCurrentPin = PchUsbOverCurrentPinSkip;
  }

  /* xDCI config */
//  FspsUpd->FspsConfig.XdciEnable = 0;

  /* SATA config */
  // This is a hard silicon requirement, discovered several times by coreboot boards
  SataConfig->PwrOptEnable = 1;

  /* PCIe config */
  // Port 1 (dGPU; x4)
  PchPcieConfig->RootPort[0].AdvancedErrorReporting = 1;
  PchPcieConfig->RootPort[0].LtrEnable = 1;
  PchPcieConfig->RootPort[0].ClkReqSupported = 1;
  PchPcieConfig->RootPort[0].ClkReqNumber = 0x0;
  PchPcieConfig->RootPort[0].MaxPayload = PchPcieMaxPayload256;
  // Port 7 (NGFF; x2)
  PchPcieConfig->RootPort[6].AdvancedErrorReporting = 1;
  PchPcieConfig->RootPort[6].LtrEnable = 1;
  PchPcieConfig->RootPort[6].ClkReqSupported = 1;
  PchPcieConfig->RootPort[6].ClkReqNumber = 0x3;
  PchPcieConfig->RootPort[6].MaxPayload = PchPcieMaxPayload256;
  // Port 9 (LAN)
  PchPcieConfig->RootPort[8].AdvancedErrorReporting = 1;
  PchPcieConfig->RootPort[8].LtrEnable = 1;
  PchPcieConfig->RootPort[8].ClkReqSupported = 1;
  PchPcieConfig->RootPort[8].ClkReqNumber = 0x1;
  PchPcieConfig->RootPort[8].MaxPayload = PchPcieMaxPayload256;
  // Port 10 (WLAN)
  PchPcieConfig->RootPort[9].AdvancedErrorReporting = 1;
  PchPcieConfig->RootPort[9].LtrEnable = 1;
  PchPcieConfig->RootPort[9].ClkReqSupported = 1;
  PchPcieConfig->RootPort[9].ClkReqNumber = 0x2;
  PchPcieConfig->RootPort[9].MaxPayload = PchPcieMaxPayload256;
  // ASPM L0s is broken/unsupported on Qualcomm Atheros QCA6174 (AER: corrected errors)
  PchPcieConfig->RootPort[9].Aspm = PchPcieAspmL1;

  /* LPC config */
  // EC/KBC requires continuous mode
  PmConfig->LpcClockRun = 1;
  SerialIrqConfig->SirqMode = PchContinuousMode;

  /* HDA config */
  HdAudioConfig->DspEndpointDmic = PchHdaDmic1chArray;

  /* SCS config */
  // Although platform NVS area shows this enabled, the SD card reader is connected over USB, not SCS
//  FspsUpd->FspsConfig.ScsEmmcEnabled = 0;
//  FspsUpd->FspsConfig.ScsSdCardEnabled = 0;

  return EFI_SUCCESS;
}
