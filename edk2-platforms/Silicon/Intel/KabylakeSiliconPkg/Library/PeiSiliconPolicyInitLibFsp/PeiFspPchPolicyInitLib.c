/** @file
  Implementation of Fsp PCH Policy Initialization.

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PeiFspPolicyInitLib.h>

#include <Ppi/SiPolicy.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>

/**
  Performs FSP PCH PEI Policy pre mem initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  EFI_STATUS                       Status;
  UINTN                            Index;
  SI_PREMEM_POLICY_PPI             *SiPreMemPolicy;
  PCH_TRACE_HUB_PREMEM_CONFIG      *TraceHubPreMemConfig;
  PCH_SMBUS_PREMEM_CONFIG          *SmbusPreMemConfig;
  PCH_DCI_PREMEM_CONFIG            *DciPreMemConfig;
  PCH_HPET_PREMEM_CONFIG           *HpetPreMemConfig;
  PCH_HSIO_PCIE_PREMEM_CONFIG      *HsioPciePreMemConfig;
  PCH_HSIO_SATA_PREMEM_CONFIG      *HsioSataPreMemConfig;
  PCH_PCIE_RP_PREMEM_CONFIG        *PcieRpPreMemConfig;
  PCH_HSIO_PREMEM_CONFIG           *HsioPreMemConfig;
  PCH_LPC_PREMEM_CONFIG            *LpcPreMemConfig;
  PCH_GENERAL_PREMEM_CONFIG        *PchGeneralPreMemConfig;
  PCH_WDT_PREMEM_CONFIG            *WdtPreMemConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiPchPolicyPreMem\n"));
  //
  // Locate PchPreMemPolicyPpi
  //
  SiPreMemPolicy = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHpetPreMemConfigGuid, (VOID *) &HpetPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioPciePreMemConfigGuid, (VOID *) &HsioPciePreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioSataPreMemConfigGuid, (VOID *) &HsioSataPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioPreMemConfigGuid, (VOID *) &HsioPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gWatchDogPreMemConfigGuid, (VOID *) &WdtPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  //
  // Update PCIE RP policies
  //
  FspmUpd->FspmConfig.PcieRpEnableMask = PcieRpPreMemConfig->RpEnabledMask & ((1 << KBL_PCH_H_PCIE_MAX_ROOT_PORTS) - 1);

  //
  // Update TraceHub policies
  //
  FspmUpd->FspmConfig.EnableTraceHub = ((UINT8)TraceHubPreMemConfig->EnableMode == 0) ? 0 : 1;
  FspmUpd->FspmConfig.TraceHubMemReg0Size = (UINT32)TraceHubPreMemConfig->MemReg0Size;
  FspmUpd->FspmConfig.TraceHubMemReg1Size = (UINT32)TraceHubPreMemConfig->MemReg1Size;

  //
  // Update Smbus policies
  //
  FspmUpd->FspmConfig.SmbusEnable = (UINT8)SmbusPreMemConfig->Enable;
  FspmUpd->FspmConfig.SmbusArpEnable = (UINT8)SmbusPreMemConfig->ArpEnable;
  FspmUpd->FspmTestConfig.SmbusDynamicPowerGating = (UINT8)SmbusPreMemConfig->DynamicPowerGating;
  FspmUpd->FspmTestConfig.SmbusSpdWriteDisable = (UINT8)SmbusPreMemConfig->SpdWriteDisable;
  FspmUpd->FspmConfig.PchSmbusIoBase = (UINT16)SmbusPreMemConfig->SmbusIoBase;
  FspmUpd->FspmConfig.PchNumRsvdSmbusAddresses = (UINT8)SmbusPreMemConfig->NumRsvdSmbusAddresses;
  FspmUpd->FspmConfig.RsvdSmbusAddressTablePtr = (UINT32)SmbusPreMemConfig->RsvdSmbusAddressTable;

  //
  // Update Dci policies
  //
  FspmUpd->FspmTestConfig.PchDciEn = (UINT8)DciPreMemConfig->DciEn;

  //
  // Update HPET policies
  //
  FspmUpd->FspmConfig.PchHpetEnable         = (UINT8)HpetPreMemConfig->Enable;
  FspmUpd->FspmConfig.PchHpetBdfValid       = (UINT8)HpetPreMemConfig->BdfValid;
  FspmUpd->FspmConfig.PchHpetBusNumber      = (UINT8)HpetPreMemConfig->BusNumber;
  FspmUpd->FspmConfig.PchHpetDeviceNumber   = (UINT8)HpetPreMemConfig->DeviceNumber;
  FspmUpd->FspmConfig.PchHpetFunctionNumber = (UINT8)HpetPreMemConfig->FunctionNumber;
  FspmUpd->FspmConfig.PchHpetBase           = (UINT32)HpetPreMemConfig->Base;

  //
  // Update HSIO PCIE policies
  //
  FspmUpd->FspmConfig.PchPmPciePllSsc = (UINT8)HsioPciePreMemConfig->PciePllSsc;
  for (Index = 0; Index < KBL_PCH_H_PCIE_MAX_ROOT_PORTS; Index ++) {
    FspmUpd->FspmConfig.PchPcieHsioRxSetCtleEnable[Index]           = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioRxSetCtleEnable;
    FspmUpd->FspmConfig.PchPcieHsioRxSetCtle[Index]                 = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioRxSetCtle;
    FspmUpd->FspmConfig.PchPcieHsioTxGen1DownscaleAmpEnable[Index]  = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen1DownscaleAmp[Index]        = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmp;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DownscaleAmpEnable[Index]  = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DownscaleAmp[Index]        = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmp;
    FspmUpd->FspmConfig.PchPcieHsioTxGen3DownscaleAmpEnable[Index]  = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen3DownscaleAmp[Index]        = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmp;
    FspmUpd->FspmConfig.PchPcieHsioTxGen1DeEmphEnable[Index]        = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmphEnable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen1DeEmph[Index]              = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmph;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph3p5Enable[Index]     = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5Enable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph3p5[Index]           = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph6p0Enable[Index]     = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0Enable;
    FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph6p0[Index]           = (UINT8)HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0;
  }

  //
  // Update HSIO SATA policies
  //
  for (Index = 0; Index < PCH_MAX_SATA_PORTS; Index ++) {
    FspmUpd->FspmConfig.PchSataHsioRxGen1EqBoostMagEnable[Index]    = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen1EqBoostMagEnable;
    FspmUpd->FspmConfig.PchSataHsioRxGen1EqBoostMag[Index]          = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen1EqBoostMag;
    FspmUpd->FspmConfig.PchSataHsioRxGen2EqBoostMagEnable[Index]    = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen2EqBoostMagEnable;
    FspmUpd->FspmConfig.PchSataHsioRxGen2EqBoostMag[Index]          = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen2EqBoostMag;
    FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMagEnable[Index]    = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMagEnable;
    FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMag[Index]          = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMag;
    FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmpEnable[Index]  = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmp[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmp;
    FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmpEnable[Index]  = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmp[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmp;
    FspmUpd->FspmConfig.PchSataHsioTxGen3DownscaleAmpEnable[Index]  = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DownscaleAmpEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen3DownscaleAmp[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DownscaleAmp;
    FspmUpd->FspmConfig.PchSataHsioTxGen1DeEmphEnable[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DeEmphEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen1DeEmph[Index]              = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DeEmph;
    FspmUpd->FspmConfig.PchSataHsioTxGen2DeEmphEnable[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DeEmphEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen2DeEmph[Index]              = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DeEmph;
    FspmUpd->FspmConfig.PchSataHsioTxGen3DeEmphEnable[Index]        = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DeEmphEnable;
    FspmUpd->FspmConfig.PchSataHsioTxGen3DeEmph[Index]              = (UINT8)HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DeEmph;
  }

  //
  // Update HSIO policies
  //
  FspmUpd->FspmTestConfig.ChipsetInitMessage                      = (UINT8)HsioPreMemConfig->ChipsetInitMessage;
  FspmUpd->FspmTestConfig.BypassPhySyncReset                      = (UINT8)HsioPreMemConfig->BypassPhySyncReset;

  //
  // Update LPC policies
  //
  FspmUpd->FspmConfig.PchLpcEnhancePort8xhDecoding = (UINT8)LpcPreMemConfig->EnhancePort8xhDecoding;

  //
  // Update Pch General Premem policies
  //
  FspmUpd->FspmConfig.PchAcpiBase = (UINT16)PchGeneralPreMemConfig->AcpiBase;
  FspmUpd->FspmConfig.PchPort80Route = (UINT8)PchGeneralPreMemConfig->Port80Route;

  //
  // Update Wdt policies
  //
  FspmUpd->FspmTestConfig.WdtDisableAndLock = (UINT8)WdtPreMemConfig->DisableAndLock;


  return EFI_SUCCESS;
}

/**
  Performs FSP PCH PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
  EFI_STATUS                   Status;
  UINTN                        Index;
  SI_POLICY_PPI                *SiPolicy;
  PCH_LAN_CONFIG               *LanConfig;
  PCH_HDAUDIO_CONFIG           *HdAudioConfig;
  PCH_SCS_CONFIG               *ScsConfig;
  PCH_ISH_CONFIG               *IshConfig;
  PCH_SPI_CONFIG               *SpiConfig;
  PCH_SATA_CONFIG              *SataConfig;
  PCH_USB_CONFIG               *UsbConfig;
  PCH_SERIAL_IO_CONFIG         *SerialIoConfig;
  PCH_INTERRUPT_CONFIG         *InterruptConfig;
  PCH_LOCK_DOWN_CONFIG         *LockDownConfig;
  PCH_CIO2_CONFIG              *Cio2Config;
  PCH_PCIE_CONFIG              *PcieRpConfig;
  PCH_DMI_CONFIG               *DmiConfig;
  PCH_FLASH_PROTECTION_CONFIG  *FlashProtectionConfig;
  PCH_IOAPIC_CONFIG            *IoApicConfig;
  PCH_P2SB_CONFIG              *P2sbConfig;
  PCH_GENERAL_CONFIG           *PchGeneralConfig;
  PCH_PM_CONFIG                *PmConfig;
  PCH_PORT61H_SMM_CONFIG       *Port61hConfig;
  PCH_LPC_SIRQ_CONFIG          *PchSerialIrqConfig;
  PCH_THERMAL_CONFIG           *PchThermalConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiPchPolicy\n"));
  //
  // Locate SiPolicyPpi
  //
  SiPolicy = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLanConfigGuid, (VOID *) &LanConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gScsConfigGuid, (VOID *) &ScsConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIshConfigGuid, (VOID *) &IshConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSpiConfigGuid, (VOID *) &SpiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gCio2ConfigGuid, (VOID *) &Cio2Config);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPcieRpConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPort61ConfigGuid, (VOID *) &Port61hConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIrqConfigGuid, (VOID *) &PchSerialIrqConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gThermalConfigGuid, (VOID *) &PchThermalConfig);
  ASSERT_EFI_ERROR (Status);
  //
  // Update LAN policies
  //
  FspsUpd->FspsConfig.PchLanEnable = (UINT8)LanConfig->Enable;
  FspsUpd->FspsConfig.PchLanLtrEnable       = (UINT8)LanConfig->LtrEnable;
  FspsUpd->FspsConfig.PchLanK1OffEnable     = (UINT8)LanConfig->K1OffEnable;
  FspsUpd->FspsConfig.PchLanClkReqSupported = (UINT8)LanConfig->ClkReqSupported;
  FspsUpd->FspsConfig.PchLanClkReqNumber    = (UINT8)LanConfig->ClkReqNumber;

  //
  // Update HDA policies
  //
  FspsUpd->FspsConfig.PchHdaEnable            = (UINT8)HdAudioConfig->Enable;
  FspsUpd->FspsConfig.PchHdaDspEnable         = (UINT8)HdAudioConfig->DspEnable;
  FspsUpd->FspsConfig.PchHdaIoBufferOwnership = (UINT8)HdAudioConfig->IoBufferOwnership;

  FspsUpd->FspsConfig.PchHdaPme                  = (UINT8)HdAudioConfig->Pme;
  FspsUpd->FspsConfig.PchHdaIoBufferVoltage      = (UINT8)HdAudioConfig->IoBufferVoltage;
  FspsUpd->FspsConfig.PchHdaVcType               = (UINT8)HdAudioConfig->VcType;
  FspsUpd->FspsConfig.PchHdaLinkFrequency        = (UINT8)HdAudioConfig->HdAudioLinkFrequency;
  FspsUpd->FspsConfig.PchHdaIDispLinkFrequency   = (UINT8)HdAudioConfig->IDispLinkFrequency;
  FspsUpd->FspsConfig.PchHdaIDispLinkTmode       = (UINT8)HdAudioConfig->IDispLinkTmode;
  FspsUpd->FspsConfig.PchHdaDspUaaCompliance     = (UINT8)HdAudioConfig->DspUaaCompliance;
  FspsUpd->FspsConfig.PchHdaIDispCodecDisconnect = (UINT8)HdAudioConfig->IDispCodecDisconnect;
  FspsUpd->FspsConfig.PchHdaDspEndpointDmic      = (UINT8)HdAudioConfig->DspEndpointDmic;
  FspsUpd->FspsConfig.PchHdaDspEndpointBluetooth = (UINT8)HdAudioConfig->DspEndpointBluetooth;
  FspsUpd->FspsConfig.PchHdaDspEndpointI2s       = (UINT8)HdAudioConfig->DspEndpointI2s;
  FspsUpd->FspsConfig.PchHdaDspFeatureMask       = (UINT32)HdAudioConfig->DspFeatureMask;
  FspsUpd->FspsTestConfig.PchHdaResetWaitTimer   = (UINT16)HdAudioConfig->ResetWaitTimer;
  FspsUpd->FspsConfig.PchHdaVerbTableEntryNum    = HdAudioConfig->VerbTableEntryNum;
  FspsUpd->FspsConfig.PchHdaVerbTablePtr         = HdAudioConfig->VerbTablePtr;

  //
  // Update CIO2 policies
  //
  FspsUpd->FspsConfig.PchCio2Enable = (UINT8)Cio2Config->DeviceEnable;
  FspsUpd->FspsConfig.PchSkyCamPortATermOvrEnable = (UINT8)Cio2Config->SkyCamPortATermOvrEnable;
  FspsUpd->FspsConfig.PchSkyCamPortBTermOvrEnable = (UINT8)Cio2Config->SkyCamPortBTermOvrEnable;
  FspsUpd->FspsConfig.PchSkyCamPortCTermOvrEnable = (UINT8)Cio2Config->SkyCamPortCTermOvrEnable;
  FspsUpd->FspsConfig.PchSkyCamPortDTermOvrEnable = (UINT8)Cio2Config->SkyCamPortDTermOvrEnable;
  FspsUpd->FspsConfig.PchSkyCamPortATrimEnable = (UINT8)Cio2Config->PortATrimEnable;
  FspsUpd->FspsConfig.PchSkyCamPortBTrimEnable = (UINT8)Cio2Config->PortBTrimEnable;
  FspsUpd->FspsConfig.PchSkyCamPortCTrimEnable = (UINT8)Cio2Config->PortCTrimEnable;
  FspsUpd->FspsConfig.PchSkyCamPortDTrimEnable = (UINT8)Cio2Config->PortDTrimEnable;
  FspsUpd->FspsConfig.PchSkyCamPortACtleEnable  = (UINT8)Cio2Config->PortACtleEnable;
  FspsUpd->FspsConfig.PchSkyCamPortBCtleEnable  = (UINT8)Cio2Config->PortBCtleEnable;
  FspsUpd->FspsConfig.PchSkyCamPortCDCtleEnable = (UINT8)Cio2Config->PortCDCtleEnable;
  FspsUpd->FspsConfig.PchSkyCamPortACtleCapValue  = (UINT8)Cio2Config->PortACtleCapValue;
  FspsUpd->FspsConfig.PchSkyCamPortBCtleCapValue  = (UINT8)Cio2Config->PortBCtleCapValue;
  FspsUpd->FspsConfig.PchSkyCamPortCDCtleCapValue = (UINT8)Cio2Config->PortCDCtleCapValue;
  FspsUpd->FspsConfig.PchSkyCamPortACtleResValue  = (UINT8)Cio2Config->PortACtleResValue;
  FspsUpd->FspsConfig.PchSkyCamPortBCtleResValue  = (UINT8)Cio2Config->PortBCtleResValue;
  FspsUpd->FspsConfig.PchSkyCamPortCDCtleResValue = (UINT8)Cio2Config->PortCDCtleResValue;
  FspsUpd->FspsConfig.PchSkyCamPortAClkTrimValue = (UINT8)Cio2Config->PortAClkTrimValue;
  FspsUpd->FspsConfig.PchSkyCamPortBClkTrimValue = (UINT8)Cio2Config->PortBClkTrimValue;
  FspsUpd->FspsConfig.PchSkyCamPortCClkTrimValue = (UINT8)Cio2Config->PortCClkTrimValue;
  FspsUpd->FspsConfig.PchSkyCamPortDClkTrimValue = (UINT8)Cio2Config->PortDClkTrimValue;
  FspsUpd->FspsConfig.PchSkyCamPortADataTrimValue  = (UINT16)Cio2Config->PortADataTrimValue;
  FspsUpd->FspsConfig.PchSkyCamPortBDataTrimValue  = (UINT16)Cio2Config->PortBDataTrimValue;
  FspsUpd->FspsConfig.PchSkyCamPortCDDataTrimValue = (UINT16)Cio2Config->PortCDDataTrimValue;

  //
  // Update SCS policies
  //
  FspsUpd->FspsConfig.ScsEmmcEnabled = (UINT8)ScsConfig->ScsEmmcEnabled;
  FspsUpd->FspsConfig.ScsEmmcHs400Enabled = (UINT8)ScsConfig->ScsEmmcHs400Enabled;
  FspsUpd->FspsConfig.ScsSdCardEnabled = (UINT8)ScsConfig->ScsSdSwitch == 0 ? 0 : 1;
  FspsUpd->FspsConfig.PchScsEmmcHs400TuningRequired = (UINT8)ScsConfig->ScsEmmcHs400TuningRequired;
  FspsUpd->FspsConfig.PchScsEmmcHs400DllDataValid   = (UINT8)ScsConfig->ScsEmmcHs400DllDataValid;
  FspsUpd->FspsConfig.PchScsEmmcHs400RxStrobeDll1   = (UINT8)ScsConfig->ScsEmmcHs400RxStrobeDll1;
  FspsUpd->FspsConfig.PchScsEmmcHs400TxDataDll      = (UINT8)ScsConfig->ScsEmmcHs400TxDataDll;
  FspsUpd->FspsConfig.PchScsEmmcHs400DriverStrength = (UINT8)ScsConfig->ScsEmmcHs400DriverStrength;

  //
  // Update ISH policies
  //
  FspsUpd->FspsConfig.PchIshEnable = (UINT8)IshConfig->Enable;
  FspsUpd->FspsConfig.PchIshSpiGpioAssign   = (UINT8)IshConfig->SpiGpioAssign;
  FspsUpd->FspsConfig.PchIshUart0GpioAssign = (UINT8)IshConfig->Uart0GpioAssign;
  FspsUpd->FspsConfig.PchIshUart1GpioAssign = (UINT8)IshConfig->Uart1GpioAssign;
  FspsUpd->FspsConfig.PchIshI2c0GpioAssign  = (UINT8)IshConfig->I2c0GpioAssign;
  FspsUpd->FspsConfig.PchIshI2c1GpioAssign  = (UINT8)IshConfig->I2c1GpioAssign;
  FspsUpd->FspsConfig.PchIshI2c2GpioAssign  = (UINT8)IshConfig->I2c2GpioAssign;
  FspsUpd->FspsConfig.PchIshGp0GpioAssign   = (UINT8)IshConfig->Gp0GpioAssign;
  FspsUpd->FspsConfig.PchIshGp1GpioAssign   = (UINT8)IshConfig->Gp1GpioAssign;
  FspsUpd->FspsConfig.PchIshGp2GpioAssign   = (UINT8)IshConfig->Gp2GpioAssign;
  FspsUpd->FspsConfig.PchIshGp3GpioAssign   = (UINT8)IshConfig->Gp3GpioAssign;
  FspsUpd->FspsConfig.PchIshGp4GpioAssign   = (UINT8)IshConfig->Gp4GpioAssign;
  FspsUpd->FspsConfig.PchIshGp5GpioAssign   = (UINT8)IshConfig->Gp5GpioAssign;
  FspsUpd->FspsConfig.PchIshGp6GpioAssign   = (UINT8)IshConfig->Gp6GpioAssign;
  FspsUpd->FspsConfig.PchIshGp7GpioAssign   = (UINT8)IshConfig->Gp7GpioAssign;
  FspsUpd->FspsConfig.PchIshPdtUnlock       = (UINT8)IshConfig->PdtUnlock;

  //
  // Update SPI policies
  //
  FspsUpd->FspsConfig.ShowSpiController = (UINT8)SpiConfig->ShowSpiController;

  //
  // Update PCIE RP RootPort policies
  //
  for (Index = 0; Index < KBL_PCH_H_PCIE_MAX_ROOT_PORTS; Index ++) {
    FspsUpd->FspsConfig.PcieRpHotPlug[Index] = (UINT8)PcieRpConfig->RootPort[Index].HotPlug;
    FspsUpd->FspsConfig.PcieRpPmSci[Index] = (UINT8)PcieRpConfig->RootPort[Index].PmSci;
    FspsUpd->FspsConfig.PcieRpExtSync[Index] = (UINT8)PcieRpConfig->RootPort[Index].ExtSync;
    FspsUpd->FspsConfig.PcieRpTransmitterHalfSwing[Index] = (UINT8)PcieRpConfig->RootPort[Index].TransmitterHalfSwing;
    FspsUpd->FspsConfig.PcieRpClkReqSupport[Index] = (UINT8)PcieRpConfig->RootPort[Index].ClkReqSupported;
    FspsUpd->FspsConfig.PcieRpClkReqNumber[Index] = (UINT8)PcieRpConfig->RootPort[Index].ClkReqNumber;
    FspsUpd->FspsConfig.PcieRpClkReqDetect[Index] = (UINT8)PcieRpConfig->RootPort[Index].ClkReqDetect;
    FspsUpd->FspsConfig.PcieRpAdvancedErrorReporting[Index] = (UINT8)PcieRpConfig->RootPort[Index].AdvancedErrorReporting;
    FspsUpd->FspsConfig.PcieRpUnsupportedRequestReport[Index] = (UINT8)PcieRpConfig->RootPort[Index].UnsupportedRequestReport;
    FspsUpd->FspsConfig.PcieRpFatalErrorReport[Index] = (UINT8)PcieRpConfig->RootPort[Index].FatalErrorReport;
    FspsUpd->FspsConfig.PcieRpNoFatalErrorReport[Index] = (UINT8)PcieRpConfig->RootPort[Index].NoFatalErrorReport;
    FspsUpd->FspsConfig.PcieRpCorrectableErrorReport[Index] = (UINT8)PcieRpConfig->RootPort[Index].CorrectableErrorReport;
    FspsUpd->FspsConfig.PcieRpSystemErrorOnFatalError[Index] = (UINT8)PcieRpConfig->RootPort[Index].SystemErrorOnFatalError;
    FspsUpd->FspsConfig.PcieRpSystemErrorOnNonFatalError[Index] = (UINT8)PcieRpConfig->RootPort[Index].SystemErrorOnNonFatalError;
    FspsUpd->FspsConfig.PcieRpSystemErrorOnCorrectableError[Index] = (UINT8)PcieRpConfig->RootPort[Index].SystemErrorOnCorrectableError;
    FspsUpd->FspsConfig.PcieRpMaxPayload[Index] = (UINT8)PcieRpConfig->RootPort[Index].MaxPayload;
    FspsUpd->FspsConfig.PcieRpDeviceResetPadActiveHigh[Index] = (UINT8)PcieRpConfig->RootPort[Index].DeviceResetPadActiveHigh;
    FspsUpd->FspsConfig.PcieRpPcieSpeed[Index] = (UINT8)PcieRpConfig->RootPort[Index].PcieSpeed;
    FspsUpd->FspsConfig.PcieRpGen3EqPh3Method[Index] = (UINT8)PcieRpConfig->RootPort[Index].Gen3EqPh3Method;
    FspsUpd->FspsConfig.PcieRpPhysicalSlotNumber[Index] = (UINT8)PcieRpConfig->RootPort[Index].PhysicalSlotNumber;
    FspsUpd->FspsConfig.PcieRpCompletionTimeout[Index] = (UINT8)PcieRpConfig->RootPort[Index].CompletionTimeout;
    FspsUpd->FspsConfig.PcieRpDeviceResetPad[Index] = (UINT32)PcieRpConfig->RootPort[Index].DeviceResetPad;
    FspsUpd->FspsConfig.PcieRpAspm[Index] = (UINT8)PcieRpConfig->RootPort[Index].Aspm;
    FspsUpd->FspsConfig.PcieRpL1Substates[Index] = (UINT8)PcieRpConfig->RootPort[Index].L1Substates;
    FspsUpd->FspsConfig.PcieRpLtrEnable[Index] = (UINT8)PcieRpConfig->RootPort[Index].LtrEnable;
    FspsUpd->FspsConfig.PcieRpLtrConfigLock[Index] = (UINT8)PcieRpConfig->RootPort[Index].LtrConfigLock;
    FspsUpd->FspsTestConfig.PcieRpLtrMaxSnoopLatency[Index] = (UINT16)PcieRpConfig->RootPort[Index].LtrMaxSnoopLatency;
    FspsUpd->FspsTestConfig.PcieRpLtrMaxNoSnoopLatency[Index] = (UINT16)PcieRpConfig->RootPort[Index].LtrMaxNoSnoopLatency;

    FspsUpd->FspsTestConfig.PcieRpSnoopLatencyOverrideMode[Index] = (UINT8)PcieRpConfig->RootPort[Index].SnoopLatencyOverrideMode;
    FspsUpd->FspsTestConfig.PcieRpSnoopLatencyOverrideMultiplier[Index] = (UINT8)PcieRpConfig->RootPort[Index].SnoopLatencyOverrideMultiplier;
    FspsUpd->FspsTestConfig.PcieRpSnoopLatencyOverrideValue[Index] = (UINT16)PcieRpConfig->RootPort[Index].SnoopLatencyOverrideValue;

    FspsUpd->FspsTestConfig.PcieRpNonSnoopLatencyOverrideMode[Index] = (UINT8)PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideMode;
    FspsUpd->FspsTestConfig.PcieRpNonSnoopLatencyOverrideMultiplier[Index] = (UINT8)PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideMultiplier;
    FspsUpd->FspsTestConfig.PcieRpNonSnoopLatencyOverrideValue[Index] = (UINT16)PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideValue;

    FspsUpd->FspsTestConfig.PcieRpSlotPowerLimitScale[Index] = (UINT8)PcieRpConfig->RootPort[Index].SlotPowerLimitScale;
    FspsUpd->FspsTestConfig.PcieRpSlotPowerLimitValue[Index] = (UINT16)PcieRpConfig->RootPort[Index].SlotPowerLimitValue;
    FspsUpd->FspsTestConfig.PcieRpUptp[Index] = (UINT8)PcieRpConfig->RootPort[Index].Uptp;
    FspsUpd->FspsTestConfig.PcieRpDptp[Index] = (UINT8)PcieRpConfig->RootPort[Index].Dptp;
  }

  //
  // Update PCIE RP EqPh3LaneParam policies
  //
  for (Index = 0; Index < KBL_PCH_H_PCIE_MAX_ROOT_PORTS; Index ++) {
    FspsUpd->FspsConfig.PcieEqPh3LaneParamCm[Index] = (UINT8)PcieRpConfig->EqPh3LaneParam[Index].Cm;
    FspsUpd->FspsConfig.PcieEqPh3LaneParamCp[Index] = (UINT8)PcieRpConfig->EqPh3LaneParam[Index].Cp;
  }

  //
  // Update PCIE RP SwEqCoeffList policies
  //
  for (Index = 0; Index < PCH_PCIE_SWEQ_COEFFS_MAX; Index ++) {
    FspsUpd->FspsConfig.PcieSwEqCoeffListCm[Index] = (UINT8)PcieRpConfig->SwEqCoeffList[Index].Cm;
    FspsUpd->FspsConfig.PcieSwEqCoeffListCp[Index] = (UINT8)PcieRpConfig->SwEqCoeffList[Index].Cp;
  }

  //
  // Update PCIE RP policies
  //
  FspsUpd->FspsTestConfig.PcieEnablePort8xhDecode        = (UINT8)PcieRpConfig->EnablePort8xhDecode;
  FspsUpd->FspsTestConfig.PchPciePort8xhDecodePortIndex  = (UINT8)PcieRpConfig->PchPciePort8xhDecodePortIndex;
  FspsUpd->FspsConfig.PcieDisableRootPortClockGating = (UINT8)PcieRpConfig->DisableRootPortClockGating;
  FspsUpd->FspsConfig.PcieEnablePeerMemoryWrite      = (UINT8)PcieRpConfig->EnablePeerMemoryWrite;
  FspsUpd->FspsConfig.PcieAllowNoLtrIccPllShutdown   = (UINT8)PcieRpConfig->AllowNoLtrIccPllShutdown;
  FspsUpd->FspsConfig.PcieComplianceTestMode         = (UINT8)PcieRpConfig->ComplianceTestMode;
  FspsUpd->FspsConfig.PcieRpFunctionSwap             = (UINT8)PcieRpConfig->RpFunctionSwap;
  FspsUpd->FspsConfig.PcieDetectTimeoutMs            = (UINT16)PcieRpConfig->DetectTimeoutMs;
  FspsUpd->FspsConfig.PchPcieDeviceOverrideTablePtr  = PcieRpConfig->PcieDeviceOverrideTablePtr;

  //
  // Update Sata Policies
  //
  FspsUpd->FspsConfig.SataEnable                     = (UINT8)SataConfig->Enable;
  FspsUpd->FspsTestConfig.SataTestMode               = (UINT8)SataConfig->TestMode;
  FspsUpd->FspsConfig.SataSalpSupport                = (UINT8)SataConfig->SalpSupport;
  FspsUpd->FspsConfig.SataPwrOptEnable = (UINT8)SataConfig->PwrOptEnable;
  FspsUpd->FspsConfig.EsataSpeedLimit  = (UINT8)SataConfig->EsataSpeedLimit;
  FspsUpd->FspsConfig.SataMode         = (UINT8)SataConfig->SataMode;
  FspsUpd->FspsConfig.SataSpeedLimit   = (UINT8)SataConfig->SpeedLimit;

  for (Index = 0; Index < PCH_MAX_SATA_PORTS; Index++) {
    FspsUpd->FspsConfig.SataPortsEnable[Index] = (UINT8)SataConfig->PortSettings[Index].Enable;
    FspsUpd->FspsConfig.SataPortsHotPlug[Index]     = (UINT8)SataConfig->PortSettings[Index].HotPlug;
    FspsUpd->FspsConfig.SataPortsInterlockSw[Index] = (UINT8)SataConfig->PortSettings[Index].InterlockSw;
    FspsUpd->FspsConfig.SataPortsExternal[Index]    = (UINT8)SataConfig->PortSettings[Index].External;
    FspsUpd->FspsConfig.SataPortsSpinUp[Index]      = (UINT8)SataConfig->PortSettings[Index].SpinUp;
    FspsUpd->FspsConfig.SataPortsSolidStateDrive[Index]  = (UINT8)SataConfig->PortSettings[Index].SolidStateDrive;
    FspsUpd->FspsConfig.SataPortsDevSlp[Index] = (UINT8)SataConfig->PortSettings[Index].DevSlp;
    FspsUpd->FspsConfig.SataPortsEnableDitoConfig[Index] = (UINT8)SataConfig->PortSettings[Index].EnableDitoConfig;
    FspsUpd->FspsConfig.SataPortsDmVal[Index]       = (UINT8)SataConfig->PortSettings[Index].DmVal;
    FspsUpd->FspsConfig.SataPortsDitoVal[Index]     = (UINT16)SataConfig->PortSettings[Index].DitoVal;
    FspsUpd->FspsConfig.SataPortsZpOdd[Index]       = (UINT8)SataConfig->PortSettings[Index].ZpOdd;
  }

  FspsUpd->FspsConfig.SataRstRaidAlternateId = (UINT8)SataConfig->Rst.RaidDeviceId;
  FspsUpd->FspsConfig.SataRstRaid0           = (UINT8)SataConfig->Rst.Raid0;
  FspsUpd->FspsConfig.SataRstRaid1           = (UINT8)SataConfig->Rst.Raid1;
  FspsUpd->FspsConfig.SataRstRaid10          = (UINT8)SataConfig->Rst.Raid10;
  FspsUpd->FspsConfig.SataRstRaid5           = (UINT8)SataConfig->Rst.Raid5;
  FspsUpd->FspsConfig.SataRstIrrt            = (UINT8)SataConfig->Rst.Irrt;
  FspsUpd->FspsConfig.SataRstOromUiBanner    = (UINT8)SataConfig->Rst.OromUiBanner;
  FspsUpd->FspsConfig.SataRstOromUiDelay     = (UINT8)SataConfig->Rst.OromUiDelay;
  FspsUpd->FspsConfig.SataRstHddUnlock       = (UINT8)SataConfig->Rst.HddUnlock;
  FspsUpd->FspsConfig.SataRstLedLocate       = (UINT8)SataConfig->Rst.LedLocate;
  FspsUpd->FspsConfig.SataRstIrrtOnly        = (UINT8)SataConfig->Rst.IrrtOnly;
  FspsUpd->FspsConfig.SataRstSmartStorage    = (UINT8)SataConfig->Rst.SmartStorage;
  FspsUpd->FspsConfig.SataRstOptaneMemory    = (UINT8)SataConfig->Rst.OptaneMemory;

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    FspsUpd->FspsConfig.SataRstPcieEnable[Index]           = (UINT8)SataConfig->RstPcieStorageRemap[Index].Enable;
    FspsUpd->FspsConfig.SataRstPcieStoragePort[Index]      = (UINT8)SataConfig->RstPcieStorageRemap[Index].RstPcieStoragePort;
    FspsUpd->FspsConfig.SataRstPcieDeviceResetDelay[Index] = (UINT8)SataConfig->RstPcieStorageRemap[Index].DeviceResetDelay;
  }

  //
  // Update USB policies
  //
  FspsUpd->FspsConfig.PchDisableComplianceMode    = (UINT8)UsbConfig->DisableComplianceMode;
  for (Index = 0; Index < PCH_MAX_USB2_PORTS; Index++) {
    FspsUpd->FspsConfig.PortUsb20Enable[Index]  = (UINT8)UsbConfig->PortUsb20[Index].Enable;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[Index] = (UINT8)UsbConfig->PortUsb20[Index].OverCurrentPin;
    FspsUpd->FspsConfig.Usb2AfePetxiset[Index]  = (UINT8)UsbConfig->PortUsb20[Index].Afe.Petxiset;
    FspsUpd->FspsConfig.Usb2AfeTxiset[Index]    = (UINT8)UsbConfig->PortUsb20[Index].Afe.Txiset;
    FspsUpd->FspsConfig.Usb2AfePredeemp[Index]  = (UINT8)UsbConfig->PortUsb20[Index].Afe.Predeemp;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[Index] = (UINT8)UsbConfig->PortUsb20[Index].Afe.Pehalfbit;
  }
  for (Index = 0; Index < PCH_MAX_USB3_PORTS; Index++) {
    FspsUpd->FspsConfig.PortUsb30Enable[Index]              = (UINT8)UsbConfig->PortUsb30[Index].Enable;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[Index]           = (UINT8)UsbConfig->PortUsb30[Index].OverCurrentPin;
    FspsUpd->FspsConfig.Usb3HsioTxDeEmphEnable[Index]       = (UINT8)UsbConfig->PortUsb30[Index].HsioTxDeEmphEnable;
    FspsUpd->FspsConfig.Usb3HsioTxDeEmph[Index]             = (UINT8)UsbConfig->PortUsb30[Index].HsioTxDeEmph;
    FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmpEnable[Index] = (UINT8)UsbConfig->PortUsb30[Index].HsioTxDownscaleAmpEnable;
    FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmp[Index]       = (UINT8)UsbConfig->PortUsb30[Index].HsioTxDownscaleAmp;
  }
  FspsUpd->FspsConfig.SsicPortEnable = (UINT8)UsbConfig->SsicConfig.SsicPort[0].Enable;
  FspsUpd->FspsConfig.XdciEnable     = (UINT8)UsbConfig->XdciConfig.Enable;

  //
  // Update SerialIo policies
  //
  for (Index = 0; Index < PCH_SERIALIO_MAX_CONTROLLERS; Index++) {
    FspsUpd->FspsConfig.SerialIoDevMode[Index] = (UINT8)SerialIoConfig->DevMode[Index];
  }
  for (Index = 0; Index < PCH_SERIALIO_MAX_I2C_CONTROLLERS; Index++) {
    FspsUpd->FspsConfig.SerialIoI2cVoltage[Index] = (UINT8)SerialIoConfig->I2cVoltage[Index];
  }
  for (Index = 0; Index < PCH_SERIALIO_MAX_SPI_CONTROLLERS; Index++) {
    FspsUpd->FspsConfig.SerialIoSpiCsPolarity[Index] = (UINT8)SerialIoConfig->SpiCsPolarity[Index];
  }
  for (Index = 0; Index < PCH_SERIALIO_MAX_UART_CONTROLLERS; Index++) {
    FspsUpd->FspsConfig.SerialIoUartHwFlowCtrl[Index] = (UINT8)SerialIoConfig->UartHwFlowCtrl[Index];
  }
  FspsUpd->FspsConfig.SerialIoGpio                     = (UINT8)SerialIoConfig->Gpio;
  FspsUpd->FspsConfig.SerialIoDebugUartNumber          = (UINT8)SerialIoConfig->DebugUartNumber;
  FspsUpd->FspsConfig.SerialIoEnableDebugUartAfterPost = (UINT8)SerialIoConfig->EnableDebugUartAfterPost;

  //
  // Update Interrupt policies
  //
  FspsUpd->FspsConfig.DevIntConfigPtr = (UINT32)InterruptConfig->DevIntConfig;
  FspsUpd->FspsConfig.NumOfDevIntConfig = InterruptConfig->NumOfDevIntConfig;
  for (Index = 0; Index < PCH_MAX_PXRC_CONFIG; Index ++) {
    FspsUpd->FspsConfig.PxRcConfig[Index] = (UINT8)InterruptConfig->PxRcConfig[Index];
  }
  FspsUpd->FspsConfig.GpioIrqRoute = (UINT8)InterruptConfig->GpioIrqRoute;
  FspsUpd->FspsConfig.SciIrqSelect = (UINT8)InterruptConfig->SciIrqSelect;
  FspsUpd->FspsConfig.TcoIrqSelect = (UINT8)InterruptConfig->TcoIrqSelect;
  FspsUpd->FspsConfig.TcoIrqEnable = (UINT8)InterruptConfig->TcoIrqEnable;

  //
  // Update LockDown policies
  //
  FspsUpd->FspsTestConfig.PchLockDownRtcLock       = (UINT8)LockDownConfig->RtcLock;
  FspsUpd->FspsTestConfig.PchLockDownGlobalSmi     = (UINT8)LockDownConfig->GlobalSmi;
  FspsUpd->FspsTestConfig.PchLockDownBiosInterface = (UINT8)LockDownConfig->BiosInterface;
  FspsUpd->FspsConfig.PchLockDownBiosLock      = (UINT8)LockDownConfig->BiosLock;
  FspsUpd->FspsConfig.PchLockDownSpiEiss       = (UINT8)LockDownConfig->SpiEiss;

  //
  // Update Dmi policies
  //
  FspsUpd->FspsConfig.PchDmiAspm = (UINT8)DmiConfig->DmiAspm;
  FspsUpd->FspsConfig.PchPwrOptEnable = (UINT8)DmiConfig->PwrOptEnable;
  if (DmiConfig->LegacyIoLowLatency) {
    FspsUpd->FspsConfig.PchDmiAspm = 0;
    FspsUpd->FspsConfig.PchLegacyIoLowLatency = TRUE;
  }

  //
  // Update Flash Protection policies
  //
  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; Index ++) {
    FspsUpd->FspsConfig.PchWriteProtectionEnable[Index] = (UINT8)FlashProtectionConfig->ProtectRange[Index].WriteProtectionEnable;
    FspsUpd->FspsConfig.PchReadProtectionEnable[Index]  = (UINT8)FlashProtectionConfig->ProtectRange[Index].ReadProtectionEnable;
    FspsUpd->FspsConfig.PchProtectedRangeLimit[Index]  = (UINT16)FlashProtectionConfig->ProtectRange[Index].ProtectedRangeLimit;
    FspsUpd->FspsConfig.PchProtectedRangeBase[Index]   = (UINT16)FlashProtectionConfig->ProtectRange[Index].ProtectedRangeBase;
  }

  //
  // Update IO Apic policies
  //
  FspsUpd->FspsConfig.PchIoApicBdfValid          = (UINT8)IoApicConfig->BdfValid;
  FspsUpd->FspsConfig.PchIoApicBusNumber         = (UINT8)IoApicConfig->BusNumber;
  FspsUpd->FspsConfig.PchIoApicDeviceNumber      = (UINT8)IoApicConfig->DeviceNumber;
  FspsUpd->FspsConfig.PchIoApicFunctionNumber    = (UINT8)IoApicConfig->FunctionNumber;
  FspsUpd->FspsConfig.PchIoApicEntry24_119       = (UINT8)IoApicConfig->IoApicEntry24_119;
  FspsUpd->FspsConfig.Early8254ClockGatingEnable = (UINT8)IoApicConfig->Early8254ClockGatingEnable;
  FspsUpd->FspsConfig.PchIoApicId                = (UINT8)IoApicConfig->IoApicId;
  FspsUpd->FspsConfig.PchIoApicRangeSelect       = (UINT8)IoApicConfig->ApicRangeSelect;

  //
  // Update P2sb policies
  //
  FspsUpd->FspsTestConfig.PchSbiUnlock = (UINT8)P2sbConfig->SbiUnlock;
  FspsUpd->FspsTestConfig.PchSbAccessUnlock = (UINT8)P2sbConfig->SbAccessUnlock;

  //
  // Update Pch General policies
  //
  FspsUpd->FspsConfig.PchSubSystemVendorId = (UINT16)PchGeneralConfig->SubSystemVendorId;
  FspsUpd->FspsConfig.PchSubSystemId       = (UINT16)PchGeneralConfig->SubSystemId;
  FspsUpd->FspsConfig.PchCrid              = (UINT8)PchGeneralConfig->Crid;

  //
  // Update Pm policies
  //
  FspsUpd->FspsConfig.PchPmPmeB0S5Dis         = (UINT8)PmConfig->WakeConfig.PmeB0S5Dis;
  FspsUpd->FspsConfig.PchPmWolEnableOverride  = (UINT8)PmConfig->WakeConfig.WolEnableOverride;
  FspsUpd->FspsConfig.PchPmPcieWakeFromDeepSx = (UINT8)PmConfig->WakeConfig.PcieWakeFromDeepSx;
  FspsUpd->FspsConfig.PchPmWoWlanEnable       = (UINT8)PmConfig->WakeConfig.WoWlanEnable;
  FspsUpd->FspsConfig.PchPmWoWlanDeepSxEnable = (UINT8)PmConfig->WakeConfig.WoWlanDeepSxEnable;
  FspsUpd->FspsConfig.PchPmLanWakeFromDeepSx  = (UINT8)PmConfig->WakeConfig.LanWakeFromDeepSx;

  FspsUpd->FspsConfig.PchPmDeepSxPol          = (UINT8)PmConfig->PchDeepSxPol;
  FspsUpd->FspsConfig.PchPmSlpS3MinAssert     = (UINT8)PmConfig->PchSlpS3MinAssert;
  FspsUpd->FspsConfig.PchPmSlpS4MinAssert     = (UINT8)PmConfig->PchSlpS4MinAssert;
  FspsUpd->FspsConfig.PchPmSlpSusMinAssert    = (UINT8)PmConfig->PchSlpSusMinAssert;
  FspsUpd->FspsConfig.PchPmSlpAMinAssert      = (UINT8)PmConfig->PchSlpAMinAssert;

  FspsUpd->FspsConfig.PchPmLpcClockRun        = (UINT8)PmConfig->LpcClockRun;
  FspsUpd->FspsConfig.PchPmSlpStrchSusUp      = (UINT8)PmConfig->SlpStrchSusUp;
  FspsUpd->FspsConfig.PchPmSlpLanLowDc        = (UINT8)PmConfig->SlpLanLowDc;
  FspsUpd->FspsConfig.PchPmPwrBtnOverridePeriod = (UINT8)PmConfig->PwrBtnOverridePeriod;
  FspsUpd->FspsTestConfig.PchPmDisableEnergyReport  = (UINT8)PmConfig->DisableEnergyReport;
  FspsUpd->FspsConfig.PchPmDisableDsxAcPresentPulldown = (UINT8)PmConfig->DisableDsxAcPresentPulldown;
  FspsUpd->FspsTestConfig.PchPmPmcReadDisable     = (UINT8)PmConfig->PmcReadDisable;
  FspsUpd->FspsConfig.PchPmDisableNativePowerButton    = (UINT8)PmConfig->DisableNativePowerButton;
  FspsUpd->FspsConfig.PchPmSlpS0Enable        = (UINT8)PmConfig->SlpS0Enable;
  FspsUpd->FspsConfig.PchPmSlpS0VmEnable      = (UINT8)PmConfig->SlpS0VmEnable;
  FspsUpd->FspsConfig.PchPmMeWakeSts          = (UINT8)PmConfig->MeWakeSts;
  FspsUpd->FspsConfig.PchPmWolOvrWkSts        = (UINT8)PmConfig->WolOvrWkSts;
  FspsUpd->FspsConfig.EnableTcoTimer          = (UINT8)PmConfig->EnableTcoTimer;
  FspsUpd->FspsConfig.PchPmPwrCycDur          = (UINT8)PmConfig->PchPwrCycDur;

  //
  // Update Port61h policies
  //
  FspsUpd->FspsConfig.PchPort61hEnable = (UINT8)Port61hConfig->Enable;

  //
  // Update Pch Serial IRQ policies
  //
  FspsUpd->FspsConfig.PchSirqEnable       = (UINT8)PchSerialIrqConfig->SirqEnable;
  FspsUpd->FspsConfig.PchSirqMode         = (UINT8)PchSerialIrqConfig->SirqMode;
  FspsUpd->FspsConfig.PchStartFramePulse  = (UINT8)PchSerialIrqConfig->StartFramePulse;

  //
  // Update Pch Thermal policies
  //
  FspsUpd->FspsConfig.PchThermalDeviceEnable  = (UINT8)PchThermalConfig->ThermalDeviceEnable;
  FspsUpd->FspsConfig.PchTsmicLock            = (UINT8)PchThermalConfig->TsmicLock;

  FspsUpd->FspsConfig.PchT0Level          = (UINT16)PchThermalConfig->TTLevels.T0Level;
  FspsUpd->FspsConfig.PchT1Level          = (UINT16)PchThermalConfig->TTLevels.T1Level;
  FspsUpd->FspsConfig.PchT2Level          = (UINT16)PchThermalConfig->TTLevels.T2Level;
  FspsUpd->FspsConfig.PchTTEnable         = (UINT8)PchThermalConfig->TTLevels.TTEnable;
  FspsUpd->FspsConfig.PchTTState13Enable  = (UINT8)PchThermalConfig->TTLevels.TTState13Enable;
  FspsUpd->FspsConfig.PchTTLock           = (UINT8)PchThermalConfig->TTLevels.TTLock;
  FspsUpd->FspsConfig.TTSuggestedSetting  = (UINT8)PchThermalConfig->TTLevels.SuggestedSetting;
  FspsUpd->FspsConfig.TTCrossThrottling   = (UINT8)PchThermalConfig->TTLevels.PchCrossThrottling;

  FspsUpd->FspsConfig.PchDmiTsawEn        = (UINT8)PchThermalConfig->DmiHaAWC.DmiTsawEn;
  FspsUpd->FspsConfig.DmiSuggestedSetting = (UINT8)PchThermalConfig->DmiHaAWC.SuggestedSetting;
  FspsUpd->FspsConfig.DmiTS0TW            = (UINT8)PchThermalConfig->DmiHaAWC.TS0TW;
  FspsUpd->FspsConfig.DmiTS1TW            = (UINT8)PchThermalConfig->DmiHaAWC.TS1TW;
  FspsUpd->FspsConfig.DmiTS2TW            = (UINT8)PchThermalConfig->DmiHaAWC.TS2TW;
  FspsUpd->FspsConfig.DmiTS3TW            = (UINT8)PchThermalConfig->DmiHaAWC.TS3TW;

  FspsUpd->FspsConfig.SataP0T1M            = (UINT8)PchThermalConfig->SataTT.P0T1M;
  FspsUpd->FspsConfig.SataP0T2M            = (UINT8)PchThermalConfig->SataTT.P0T2M;
  FspsUpd->FspsConfig.SataP0T3M            = (UINT8)PchThermalConfig->SataTT.P0T3M;
  FspsUpd->FspsConfig.SataP0TDisp          = (UINT8)PchThermalConfig->SataTT.P0TDisp;
  FspsUpd->FspsConfig.SataP1T1M            = (UINT8)PchThermalConfig->SataTT.P1T1M;
  FspsUpd->FspsConfig.SataP1T2M            = (UINT8)PchThermalConfig->SataTT.P1T2M;
  FspsUpd->FspsConfig.SataP1T3M            = (UINT8)PchThermalConfig->SataTT.P1T3M;
  FspsUpd->FspsConfig.SataP1TDisp          = (UINT8)PchThermalConfig->SataTT.P1TDisp;
  FspsUpd->FspsConfig.SataP0Tinact         = (UINT8)PchThermalConfig->SataTT.P0Tinact;
  FspsUpd->FspsConfig.SataP0TDispFinit     = (UINT8)PchThermalConfig->SataTT.P0TDispFinit;
  FspsUpd->FspsConfig.SataP1Tinact         = (UINT8)PchThermalConfig->SataTT.P1Tinact;
  FspsUpd->FspsConfig.SataP1TDispFinit     = (UINT8)PchThermalConfig->SataTT.P1TDispFinit;
  FspsUpd->FspsConfig.SataThermalSuggestedSetting = (UINT8)PchThermalConfig->SataTT.SuggestedSetting;

  FspsUpd->FspsConfig.PchMemoryThrottlingEnable    = (UINT8)PchThermalConfig->MemoryThrottling.Enable;
  FspsUpd->FspsConfig.PchMemoryPmsyncEnable[0]     = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[0].PmsyncEnable;
  FspsUpd->FspsConfig.PchMemoryPmsyncEnable[1]     = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[1].PmsyncEnable;
  FspsUpd->FspsConfig.PchMemoryC0TransmitEnable[0] = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[0].C0TransmitEnable;
  FspsUpd->FspsConfig.PchMemoryC0TransmitEnable[1] = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[1].C0TransmitEnable;
  FspsUpd->FspsConfig.PchMemoryPinSelection[0]     = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[0].PinSelection;
  FspsUpd->FspsConfig.PchMemoryPinSelection[1]     = (UINT8)PchThermalConfig->MemoryThrottling.TsGpioPinSetting[1].PinSelection;

  FspsUpd->FspsConfig.PchTemperatureHotLevel = (UINT16)PchThermalConfig->PchHotLevel;


  return EFI_SUCCESS;
}
