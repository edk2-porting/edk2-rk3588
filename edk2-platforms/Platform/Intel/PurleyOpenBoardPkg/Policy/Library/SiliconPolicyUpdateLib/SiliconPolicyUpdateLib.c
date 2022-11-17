/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>

#include "Guid/SetupVariable.h"
#include <Guid/PchRcVariable.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>

#include <PchAccess.h>
#include <Ppi/PchPolicy.h>
#include <Library/MmPciBaseLib.h>

#include <Register/PchRegsSata.h>
#include <Library/HobLib.h>
#include <Platform.h>
#include <Guid/PchRcVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/PchSerialIoLib.h>
#include <Guid/SocketVariable.h>
#include <Library/PcdLib.h>
#include <Library/PchGbeLib.h>
#include <Library/PchInfoLib.h>

//
// Haddock Creek
//
#define DIMM_SMB_SPD_P0C0D0_HC 0xA2
#define DIMM_SMB_SPD_P0C0D1_HC 0xA0
#define DIMM_SMB_SPD_P0C1D0_HC 0xA6
#define DIMM_SMB_SPD_P0C1D1_HC 0xA4
#define DIMM_SMB_SPD_P0C0D2_HC 0xAA
#define DIMM_SMB_SPD_P0C1D2_HC 0xA8

//
// Sawtooth Peak
// Single SPD EEPROM at 0xA2 serves both C0D0 and C1D0 (LPDDR is 1DPC only)
//
#define DIMM_SMB_SPD_P0C0D0_STP 0xA2
#define DIMM_SMB_SPD_P0C0D1_STP 0xA0
#define DIMM_SMB_SPD_P0C1D0_STP 0xA2
#define DIMM_SMB_SPD_P0C1D1_STP 0xA0

//
// Aden Hills
// DDR4 System (1DPC)
//
#define DIMM_SMB_SPD_P0C0D0_AH 0xA0
#define DIMM_SMB_SPD_P0C0D1_AH 0xA4
#define DIMM_SMB_SPD_P0C1D0_AH 0xA2
#define DIMM_SMB_SPD_P0C1D1_AH 0xA6


GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mSmbusHCRsvdAddresses[] = {
  DIMM_SMB_SPD_P0C0D0_HC,
  DIMM_SMB_SPD_P0C0D1_HC,
  DIMM_SMB_SPD_P0C1D0_HC,
  DIMM_SMB_SPD_P0C1D1_HC
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mSmbusSTPRsvdAddresses[] = {
  DIMM_SMB_SPD_P0C0D0_STP,
  DIMM_SMB_SPD_P0C0D1_STP,
  DIMM_SMB_SPD_P0C1D0_STP,
  DIMM_SMB_SPD_P0C1D1_STP
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mSmbusAHRsvdAddresses[] = {
  DIMM_SMB_SPD_P0C0D0_AH,
  DIMM_SMB_SPD_P0C0D1_AH,
  DIMM_SMB_SPD_P0C1D0_AH,
  DIMM_SMB_SPD_P0C1D1_AH
};

VOID
UpdatePchUsbConfig (
  IN PCH_USB_CONFIG            *PchUsbConfig,
  IN SYSTEM_CONFIGURATION      *SetupVariables,
  IN PCH_RC_CONFIGURATION      *PchRcVariables,
  IN VOID                      *Usb20OverCurrentMappings,
  IN VOID                      *Usb30OverCurrentMappings
  );

static
VOID
InstallPlatformVerbTables (
    IN          UINTN                       CodecType
  )
{

}

EFI_STATUS
EFIAPI
UpdatePeiPchPolicy (
  IN OUT PCH_POLICY_PPI        *PchPolicy
  )
/*++

Routine Description:

  This function performs PCH PEI Policy initialzation.

Arguments:

  PchPolicy               The PCH Policy PPI instance

Returns:

  EFI_SUCCESS             The PPI is installed and initialized.
  EFI ERRORS              The PPI is not successfully installed.
  EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver

--*/
{
  UINT8                           Index;
  UINTN                           LpcBaseAddress;
  UINT8                           MaxSataPorts;
  UINT8                           *SmBusReservedTable;
  UINT8                           SmBusReservedNum;
  PCH_USB_OVERCURRENT_PIN        *Usb20OverCurrentMappings=NULL;
  PCH_USB_OVERCURRENT_PIN        *Usb30OverCurrentMappings=NULL;
  UINT8                          VTdSupport;
  SYSTEM_CONFIGURATION           *SetupVariables;
  PCH_RC_CONFIGURATION           *PchRcVariables;

  DEBUG((EFI_D_ERROR, "platform common UpdatePeiPchPolicy entry\n"));

  SetupVariables = PcdGetPtr(PcdSetupData);
  PchRcVariables = PcdGetPtr(PcdPchRcConfigurationData);

  LpcBaseAddress = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_PCH,
                    PCI_DEVICE_NUMBER_PCH_LPC,
                    PCI_FUNCTION_NUMBER_PCH_LPC
                    );

  PchPolicy->Port80Route          = PchRcVariables->IchPort80Route;

  //
  // DeviceEnables
  //
  if (PchIsGbeAvailable ()) {
    PchPolicy->LanConfig.Enable         = TRUE;
    PchPolicy->LanConfig.K1OffEnable    = PchRcVariables->PchLanK1Off;
  } else {
    PchPolicy->LanConfig.Enable         = FALSE;
  }

  PchPolicy->SataConfig.Enable          = PchRcVariables->PchSata;

  PchPolicy->sSataConfig.Enable          = PchRcVariables->PchsSata;
  PchPolicy->SmbusConfig.Enable         = TRUE;
  //
  // CLOCKRUN in LPC has to be disabled:
  // - if a device is connected to LPC0
  // - for LBG A0 stepping
  //
  PchPolicy->PmConfig.PciClockRun       = FALSE;
  PchPolicy->PchConfig.Crid             = PchRcVariables->PchCrid;
  PchPolicy->PchConfig.Serm             = PchRcVariables->PchSerm;


  //
  // SMBUS reserved addresses
  //
  SmBusReservedTable = NULL;
  SmBusReservedNum   = 0;
  PchPolicy->SmbusConfig.SmbusIoBase = PCH_SMBUS_BASE_ADDRESS;
  SmBusReservedTable = mSmbusSTPRsvdAddresses;
  SmBusReservedNum   = sizeof (mSmbusSTPRsvdAddresses);

  if (SmBusReservedTable != NULL) {
    PchPolicy->SmbusConfig.NumRsvdSmbusAddresses = SmBusReservedNum;
    CopyMem (
      PchPolicy->SmbusConfig.RsvdSmbusAddressTable,
      SmBusReservedTable,
      SmBusReservedNum
      );
  }

  //
  // SATA Config
  //
  PchPolicy->SataConfig.SataMode  = PchRcVariables->SataInterfaceMode;
  MaxSataPorts = GetPchMaxSataPortNum ();

  for (Index = 0; Index < MaxSataPorts; Index++) {
    if (PchRcVariables->SataTestMode == TRUE)
    {
      PchPolicy->SataConfig.PortSettings[Index].Enable    = TRUE;
    } else {
      PchPolicy->SataConfig.PortSettings[Index].Enable = PchRcVariables->SataPort[Index];
    }
    PchPolicy->SataConfig.PortSettings[Index].HotPlug          = PchRcVariables->SataHotPlug[Index];
    PchPolicy->SataConfig.PortSettings[Index].SpinUp           = PchRcVariables->SataSpinUp[Index];
    PchPolicy->SataConfig.PortSettings[Index].External         = PchRcVariables->SataExternal[Index];
    PchPolicy->SataConfig.PortSettings[Index].DevSlp           = PchRcVariables->PxDevSlp[Index];
    PchPolicy->SataConfig.PortSettings[Index].EnableDitoConfig = PchRcVariables->EnableDitoConfig[Index];
    PchPolicy->SataConfig.PortSettings[Index].DmVal            = PchRcVariables->DmVal[Index];
    PchPolicy->SataConfig.PortSettings[Index].DitoVal          = PchRcVariables->DitoVal[Index];
    PchPolicy->SataConfig.PortSettings[Index].SolidStateDrive  = PchRcVariables->SataType[Index];
  }

  if (PchPolicy->SataConfig.SataMode == PchSataModeRaid) {
    PchPolicy->SataConfig.Rst.RaidAlternateId = PchRcVariables->SataAlternateId;
    PchPolicy->SataConfig.Rst.EfiRaidDriverLoad = PchRcVariables->SataRaidLoadEfiDriver;
  }
  PchPolicy->SataConfig.Rst.Raid0           = PchRcVariables->SataRaidR0;
  PchPolicy->SataConfig.Rst.Raid1           = PchRcVariables->SataRaidR1;
  PchPolicy->SataConfig.Rst.Raid10          = PchRcVariables->SataRaidR10;
  PchPolicy->SataConfig.Rst.Raid5           = PchRcVariables->SataRaidR5;
  PchPolicy->SataConfig.Rst.Irrt            = PchRcVariables->SataRaidIrrt;
  PchPolicy->SataConfig.Rst.OromUiBanner    = PchRcVariables->SataRaidOub;
  PchPolicy->SataConfig.Rst.HddUnlock       = PchRcVariables->SataHddlk;
  PchPolicy->SataConfig.Rst.LedLocate       = PchRcVariables->SataLedl;
  PchPolicy->SataConfig.Rst.IrrtOnly        = PchRcVariables->SataRaidIooe;
  PchPolicy->SataConfig.Rst.SmartStorage    = PchRcVariables->SataRaidSrt;
  PchPolicy->SataConfig.Rst.OromUiDelay     = PchRcVariables->SataRaidOromDelay;

  PchPolicy->SataConfig.EnclosureSupport    = TRUE;

  PchPolicy->SataConfig.SalpSupport     = PchRcVariables->SataSalp;
  PchPolicy->SataConfig.TestMode        = PchRcVariables->SataTestMode;

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    if ((PchRcVariables->PchSata == TRUE) && (PchRcVariables->SataInterfaceMode == PchSataModeRaid)) {
      PchPolicy->SataConfig.RstPcieStorageRemap[Index].Enable               = PchRcVariables->RstPcieStorageRemap[Index];
      PchPolicy->SataConfig.RstPcieStorageRemap[Index].RstPcieStoragePort   = PchRcVariables->RstPcieStorageRemapPort[Index];
    } else {
      PchPolicy->SataConfig.RstPcieStorageRemap[Index].Enable               = FALSE;
    }
  }

  //
  // sSATA Config
  //
  PchPolicy->sSataConfig.SataMode  = PchRcVariables->sSataInterfaceMode;
  MaxSataPorts = GetPchMaxsSataPortNum ();

  for (Index = 0; Index < MaxSataPorts; Index++) {
    if (PchRcVariables->sSataTestMode == TRUE)
    {
      PchPolicy->sSataConfig.PortSettings[Index].Enable    = TRUE;
    } else {
      PchPolicy->sSataConfig.PortSettings[Index].Enable = PchRcVariables->sSataPort[Index];
    }
    PchPolicy->sSataConfig.PortSettings[Index].HotPlug          = PchRcVariables->sSataHotPlug[Index];
    PchPolicy->sSataConfig.PortSettings[Index].SpinUp           = PchRcVariables->sSataSpinUp[Index];
    PchPolicy->sSataConfig.PortSettings[Index].External         = PchRcVariables->sSataExternal[Index];
    PchPolicy->sSataConfig.PortSettings[Index].DevSlp           = PchRcVariables->sPxDevSlp[Index];
    PchPolicy->sSataConfig.PortSettings[Index].EnableDitoConfig = PchRcVariables->sEnableDitoConfig[Index];
    PchPolicy->sSataConfig.PortSettings[Index].DmVal            = PchRcVariables->sDmVal[Index];
    PchPolicy->sSataConfig.PortSettings[Index].DitoVal          = PchRcVariables->sDitoVal[Index];
    PchPolicy->sSataConfig.PortSettings[Index].SolidStateDrive  = PchRcVariables->sSataType[Index];
  }

  if (PchPolicy->sSataConfig.SataMode == PchSataModeRaid) {
    PchPolicy->sSataConfig.Rst.RaidAlternateId = PchRcVariables->sSataAlternateId;
    PchPolicy->sSataConfig.Rst.EfiRaidDriverLoad = PchRcVariables->sSataRaidLoadEfiDriver;
  }
  PchPolicy->sSataConfig.Rst.Raid0           = PchRcVariables->sSataRaidR0;
  PchPolicy->sSataConfig.Rst.Raid1           = PchRcVariables->sSataRaidR1;
  PchPolicy->sSataConfig.Rst.Raid10          = PchRcVariables->sSataRaidR10;
  PchPolicy->sSataConfig.Rst.Raid5           = PchRcVariables->sSataRaidR5;
  PchPolicy->sSataConfig.Rst.Irrt            = PchRcVariables->sSataRaidIrrt;
  PchPolicy->sSataConfig.Rst.OromUiBanner    = PchRcVariables->sSataRaidOub;
  PchPolicy->sSataConfig.Rst.HddUnlock       = PchRcVariables->sSataHddlk;
  PchPolicy->sSataConfig.Rst.LedLocate       = PchRcVariables->sSataLedl;
  PchPolicy->sSataConfig.Rst.IrrtOnly        = PchRcVariables->sSataRaidIooe;
  PchPolicy->sSataConfig.Rst.SmartStorage    = PchRcVariables->sSataRaidSrt;
  PchPolicy->sSataConfig.Rst.OromUiDelay     = PchRcVariables->sSataRaidOromDelay;

  PchPolicy->sSataConfig.EnclosureSupport    = TRUE;

  PchPolicy->sSataConfig.SalpSupport     = PchRcVariables->sSataSalp;
  PchPolicy->sSataConfig.TestMode        = PchRcVariables->sSataTestMode;
  //
  // Initiate DMI Configuration
  //
  if (SetupVariables->PcieDmiAspm != PLATFORM_POR) {
    if (SetupVariables->PcieDmiAspm != 0xFF) {
      PchPolicy->DmiConfig.DmiAspm = TRUE;
    } else {
      PchPolicy->DmiConfig.DmiAspm = FALSE;
    }
  }
  DEBUG((DEBUG_ERROR, "PchPolicy->DmiConfig.DmiAspm =%x\n", PchPolicy->DmiConfig.DmiAspm));
  //
  // PCI express config
  //
  PchPolicy->PcieConfig.DisableRootPortClockGating      = SetupVariables->PcieClockGating;
  PchPolicy->PcieConfig.EnablePort8xhDecode           = PchRcVariables->PcieRootPort8xhDecode;
  PchPolicy->PcieConfig.PchPciePort8xhDecodePortIndex = PchRcVariables->Pcie8xhDecodePortIndex;
  PchPolicy->PcieConfig.EnablePeerMemoryWrite         = PchRcVariables->PcieRootPortPeerMemoryWriteEnable;
  PchPolicy->PcieConfig.ComplianceTestMode            = PchRcVariables->PcieComplianceTestMode;

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    PchPolicy->PcieConfig.RootPort[Index].Enable                         = PchRcVariables->PcieRootPortEn[Index];
    PchPolicy->PcieConfig.RootPort[Index].PhysicalSlotNumber             = (UINT8) Index;
    if (PchRcVariables->PchPcieGlobalAspm > PchPcieAspmDisabled) {
      // Disabled a.k.a. Per individual port
      PchPolicy->PcieConfig.RootPort[Index].Aspm                         = PchRcVariables->PchPcieGlobalAspm;
    } else {
      PchPolicy->PcieConfig.RootPort[Index].Aspm                         = PchRcVariables->PcieRootPortAspm[Index];
    }
    PchPolicy->PcieConfig.RootPort[Index].L1Substates                    = PchRcVariables->PcieRootPortL1SubStates[Index];
    PchPolicy->PcieConfig.RootPort[Index].AcsEnabled                     = PchRcVariables->PcieRootPortACS[Index];
    PchPolicy->PcieConfig.RootPort[Index].PmSci                          = PchRcVariables->PcieRootPortPMCE[Index];
    PchPolicy->PcieConfig.RootPort[Index].HotPlug                        = PchRcVariables->PcieRootPortHPE[Index];
    PchPolicy->PcieConfig.RootPort[Index].AdvancedErrorReporting         = PchRcVariables->PcieRootPortAER[Index];
    PchPolicy->PcieConfig.RootPort[Index].UnsupportedRequestReport       = PchRcVariables->PcieRootPortURE[Index];
    PchPolicy->PcieConfig.RootPort[Index].FatalErrorReport               = PchRcVariables->PcieRootPortFEE[Index];
    PchPolicy->PcieConfig.RootPort[Index].NoFatalErrorReport             = PchRcVariables->PcieRootPortNFE[Index];
    PchPolicy->PcieConfig.RootPort[Index].CorrectableErrorReport         = PchRcVariables->PcieRootPortCEE[Index];
    PchPolicy->PcieConfig.RootPort[Index].SystemErrorOnFatalError        = PchRcVariables->PcieRootPortSFE[Index];
    PchPolicy->PcieConfig.RootPort[Index].SystemErrorOnNonFatalError     = PchRcVariables->PcieRootPortSNE[Index];
    PchPolicy->PcieConfig.RootPort[Index].SystemErrorOnCorrectableError  = PchRcVariables->PcieRootPortSCE[Index];
    PchPolicy->PcieConfig.RootPort[Index].TransmitterHalfSwing           = PchRcVariables->PcieRootPortTHS[Index];
    PchPolicy->PcieConfig.RootPort[Index].CompletionTimeout              = PchRcVariables->PcieRootPortCompletionTimeout[Index];
    PchPolicy->PcieConfig.RootPort[Index].PcieSpeed                      = PchRcVariables->PcieRootPortSpeed[Index];

    PchPolicy->PcieConfig.RootPort[Index].MaxPayload                     = PchRcVariables->PcieRootPortMaxPayLoadSize[Index];
    PchPolicy->PcieConfig.RootPort[Index].Gen3EqPh3Method                = PchRcVariables->PcieRootPortEqPh3Method[Index];
  }

  for (Index = 0; Index < GetPchMaxPciePortNum (); ++Index) {
    PchPolicy->PcieConfig.EqPh3LaneParam[Index].Cm  = PchRcVariables->PcieLaneCm[Index];
    PchPolicy->PcieConfig.EqPh3LaneParam[Index].Cp  = PchRcVariables->PcieLaneCp[Index];
  }
  if (PchRcVariables->PcieSwEqOverride) {
    for (Index = 0; Index < PCH_PCIE_SWEQ_COEFFS_MAX; Index++) {
      PchPolicy->PcieConfig2.SwEqCoeffList[Index].Cm     = PchRcVariables->PcieSwEqCoeffCm[Index];
      PchPolicy->PcieConfig2.SwEqCoeffList[Index].Cp     = PchRcVariables->PcieSwEqCoeffCp[Index];
    }
  }

  PchPolicy->PcieConfig.MaxReadRequestSize                               = PchRcVariables->PcieRootPortMaxReadRequestSize;
  ///
  /// Update Competion Timeout settings for Upling ports for Server PCH
  ///
  PchPolicy->PcieConfig.PchPcieUX16CompletionTimeout                     = PchRcVariables->PchPcieUX16CompletionTimeout;
  PchPolicy->PcieConfig.PchPcieUX8CompletionTimeout                      = PchRcVariables->PchPcieUX8CompletionTimeout;
  ///
  /// Update Max Payload Size settings for Upling ports for Server PCH
  ///
  PchPolicy->PcieConfig.PchPcieUX16MaxPayload                            = PchRcVariables->PchPcieUX16MaxPayloadSize;
  PchPolicy->PcieConfig.PchPcieUX8MaxPayload                             = PchRcVariables->PchPcieUX8MaxPayloadSize;
  CopyMem (&VTdSupport, (UINT8 *)PcdGetPtr(PcdSocketIioConfigData) + OFFSET_OF(SOCKET_IIO_CONFIGURATION, VTdSupport), sizeof(VTdSupport));
  PchPolicy->PcieConfig.VTdSupport                                       = VTdSupport;

  ///
  /// Assign ClkReq signal to root port. (Base 0)
  /// For LP, Set 0 - 5
  /// For H,  Set 0 - 15
  /// Note that if GbE is enabled, ClkReq assigned to GbE will not be available for Root Port. (TODO for Purley)
  ///
  //
  // HdAudioConfig
  //
  PchPolicy->HdAudioConfig.Enable               = PchRcVariables->PchHdAudio;
  PchPolicy->HdAudioConfig.DspEnable            = FALSE;
  PchPolicy->HdAudioConfig.Pme                  = PchRcVariables->PchHdAudioPme;
  PchPolicy->HdAudioConfig.IoBufferOwnership    = PchRcVariables->PchHdAudioIoBufferOwnership;
  PchPolicy->HdAudioConfig.IoBufferVoltage      = PchRcVariables->PchHdAudioIoBufferVoltage;
  PchPolicy->HdAudioConfig.ResetWaitTimer       = 300;
  PchPolicy->HdAudioConfig.IDispCodecDisconnect = TRUE;  //iDisp is permanently disabled
  for(Index = 0; Index < HDAUDIO_FEATURES; Index++) {
    PchPolicy->HdAudioConfig.DspFeatureMask |= (UINT32)(PchRcVariables->PchHdAudioFeature[Index] ? (1 << Index) : 0);
  }

  for(Index = 0; Index < HDAUDIO_PP_MODULES; Index++) {
    PchPolicy->HdAudioConfig.DspPpModuleMask |= (UINT32)(PchRcVariables->PchHdAudioPostProcessingMod[Index] ? (1 << Index) : 0);
  }

  if (PchPolicy->HdAudioConfig.Enable) {
    InstallPlatformVerbTables (PchRcVariables->PchHdAudioCodecSelect);
  }

  PchPolicy->HdAudioConfig.VcType = PchRcVariables->RtoHdaVcType;
  //
  // LockDown
  //


    PchPolicy->LockDownConfig.RtcLock          = PchRcVariables->PchRtcLock;
    PchPolicy->LockDownConfig.BiosLock         = PchRcVariables->PchBiosLock;
    PchPolicy->LockDownConfig.SpiEiss          = TRUE;
    PchPolicy->LockDownConfig.GlobalSmi        = TRUE;
    PchPolicy->LockDownConfig.BiosInterface    = TRUE;
    PchPolicy->LockDownConfig.EvaLockDown      = PchRcVariables->PchEvaLockDown;
    PchPolicy->LockDownConfig.GpioLockDown     = PchRcVariables->PchGpioLockDown;
    PchPolicy->LockDownConfig.TcoLock          = TRUE;

  if(PchRcVariables->PchP2sbUnlock) {
    PchPolicy->P2sbConfig.SbiUnlock = TRUE;
    PchPolicy->P2sbConfig.PsfUnlock = TRUE;
  } else {
    PchPolicy->P2sbConfig.SbiUnlock = FALSE;
    PchPolicy->P2sbConfig.PsfUnlock = FALSE;
  }
  PchPolicy->P2sbConfig.P2SbReveal = PchRcVariables->PchP2sbDevReveal;

  //
  // Update SPI policies
  //
  PchPolicy->SpiConfig.ShowSpiController = TRUE;

  //
  // PMC Policy
  //
  PchPolicy->PmConfig.PmcReadDisable = PchRcVariables->PmcReadDisable;


  if (PchRcVariables->PchAdrEn != PLATFORM_POR) {
    PchPolicy->AdrConfig.PchAdrEn = PchRcVariables->PchAdrEn;
  }
  PchPolicy->AdrConfig.AdrGpioSel = PchRcVariables->AdrGpioSel;
  if (PchRcVariables->AdrHostPartitionReset != PLATFORM_POR) {
    PchPolicy->AdrConfig.AdrHostPartitionReset = PchRcVariables->AdrHostPartitionReset;
  }
  if (PchRcVariables->AdrTimerEn != PLATFORM_POR) {
    PchPolicy->AdrConfig.AdrTimerEn = PchRcVariables->AdrTimerEn;
  }
  if (PchRcVariables->AdrTimerVal != ADR_TMR_SETUP_DEFAULT_POR) {
    PchPolicy->AdrConfig.AdrTimerVal = PchRcVariables->AdrTimerVal;
  }
  if (PchRcVariables->AdrMultiplierVal != ADR_MULT_SETUP_DEFAULT_POR) {
    PchPolicy->AdrConfig.AdrMultiplierVal = PchRcVariables->AdrMultiplierVal;
  }

  //
  // Thermal Config
  //
  if ((PchRcVariables->MemoryThermalManagement != FALSE) &&
      ((PchRcVariables->ExttsViaTsOnBoard != FALSE) || (PchRcVariables->ExttsViaTsOnDimm != FALSE)))
  {
    PchPolicy->ThermalConfig.MemoryThrottling.Enable                                     = TRUE;
    PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioC].PmsyncEnable     = TRUE;
    PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioD].PmsyncEnable     = TRUE;
    PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioC].C0TransmitEnable = TRUE;
    PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioD].C0TransmitEnable = TRUE;
    PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioC].PinSelection     = 1;
    PchPolicy->ThermalConfig.MemoryThrottling.TsGpioPinSetting[TsGpioD].PinSelection     = 0;
  } else {
    PchPolicy->ThermalConfig.MemoryThrottling.Enable = FALSE;
  }

  //
  // IOAPIC Config
  //
  PchPolicy->IoApicConfig.IoApicEntry24_119 = PchRcVariables->PchIoApic24119Entries;
  PchPolicy->IoApicConfig.BdfValid          = 1;
  PchPolicy->IoApicConfig.BusNumber         = 0xF0;
  PchPolicy->IoApicConfig.DeviceNumber      = 0x1F;
  PchPolicy->IoApicConfig.FunctionNumber    = 0;


  //
  // Misc PM Config
  //
  PchPolicy->PmConfig.PchDeepSxPol                          = PchRcVariables->DeepSxMode;
  PchPolicy->PmConfig.WakeConfig.WolEnableOverride          = PchRcVariables->PchWakeOnLan;
  PchPolicy->PmConfig.WakeConfig.WoWlanEnable               = PchRcVariables->PchWakeOnWlan;
  PchPolicy->PmConfig.WakeConfig.WoWlanDeepSxEnable         = PchRcVariables->PchWakeOnWlanDeepSx;
  PchPolicy->PmConfig.WakeConfig.Gp27WakeFromDeepSx         = PchRcVariables->Gp27WakeFromDeepSx;
  PchPolicy->PmConfig.SlpLanLowDc                           = PchRcVariables->PchSlpLanLowDc;
  PchPolicy->PmConfig.PowerResetStatusClear.MeWakeSts       = TRUE;
  PchPolicy->PmConfig.PowerResetStatusClear.MeHrstColdSts   = TRUE;
  PchPolicy->PmConfig.PowerResetStatusClear.MeHrstWarmSts   = TRUE;
  PchPolicy->PmConfig.PciePllSsc                            = PchRcVariables->PciePllSsc;

  PchPolicy->PmConfig.DirtyWarmReset                        = PchRcVariables->Dwr_Enable;

  PchPolicy->PmConfig.StallDirtyWarmReset                   = PchRcVariables->Dwr_Stall;
  PchPolicy->PmConfig.Dwr_BmcRootPort                       = PchRcVariables->Dwr_BmcRootPort;

  PchPolicy->PmConfig.PchGbl2HostEn.Bits.PMCGBL             = PchRcVariables->DwrEn_PMCGBL;
  PchPolicy->PmConfig.PchGbl2HostEn.Bits.MEWDT              = PchRcVariables->DwrEn_MEWDT;
  PchPolicy->PmConfig.PchGbl2HostEn.Bits.IEWDT              = PchRcVariables->DwrEn_IEWDT;

  PchPolicy->PmConfig.Dwr_MeResetPrepDone                   = PchRcVariables->Dwr_MeResetPrepDone;
  PchPolicy->PmConfig.Dwr_IeResetPrepDone                   = PchRcVariables->Dwr_IeResetPrepDone;

  //
  // DefaultSvidSid Config
  //
  PchPolicy->PchConfig.SubSystemVendorId     = V_PCH_INTEL_VENDOR_ID;
  PchPolicy->PchConfig.SubSystemId           = V_PCH_DEFAULT_SID;
  PchPolicy->PchConfig.EnableClockSpreadSpec =  PchRcVariables->EnableClockSpreadSpec;
  //
  // Thermal Config
  //
  PchPolicy->ThermalConfig.ThermalThrottling.TTLevels.PchCrossThrottling = PchRcVariables->PchCrossThrottling;
  PchPolicy->ThermalConfig.ThermalThrottling.DmiHaAWC.SuggestedSetting   = TRUE;
  if (PchRcVariables->ThermalDeviceEnable == PchThermalDeviceAuto) {
    if (PchStepping () == LbgA0) {
      PchPolicy->ThermalConfig.ThermalDeviceEnable = PchThermalDeviceDisabled;
    } else {
      PchPolicy->ThermalConfig.ThermalDeviceEnable = PchThermalDeviceEnabledPci;
    }
  } else {
    PchPolicy->ThermalConfig.ThermalDeviceEnable = PchRcVariables->ThermalDeviceEnable;
  }

  PchPolicy->ThermalConfig.ThermalThrottling.TTLevels.SuggestedSetting   = TRUE;
  PchPolicy->ThermalConfig.ThermalThrottling.SataTT.SuggestedSetting     = TRUE;
  PchPolicy->ThermalConfig.ThermalThrottling.sSataTT.SuggestedSetting     = TRUE;

  //
  // DCI (EXI)
  //
  PchPolicy->DciConfig.DciEn         = PchRcVariables->PchDciEn;
  PchPolicy->DciConfig.DciAutoDetect = PchRcVariables->PchDciAutoDetect;


  //
  // Initialize Serial IRQ Config
  //
  PchPolicy->SerialIrqConfig.SirqEnable       = TRUE;
  PchPolicy->SerialIrqConfig.StartFramePulse  = PchSfpw4Clk;
  if (PchRcVariables->PchSirqMode == 0) {
    PchPolicy->SerialIrqConfig.SirqMode = PchQuietMode;
  } else {
    PchPolicy->SerialIrqConfig.SirqMode = PchContinuousMode;
  }

  //
  // Port 61h emulation
  //
  PchPolicy->Port61hSmmConfig.Enable = TRUE;

  //
  // DMI configuration
  //
  PchPolicy->DmiConfig.DmiLinkDownHangBypass = PchRcVariables->DmiLinkDownHangBypass;
  PchPolicy->DmiConfig.DmiStopAndScreamEnable = PchRcVariables->PcieDmiStopAndScreamEnable;

  //
  // Update Pch Usb Config
  //
  Usb20OverCurrentMappings = (PCH_USB_OVERCURRENT_PIN *)(UINTN)PcdGet64 (PcdUsb20OverCurrentMappings);
  Usb30OverCurrentMappings = (PCH_USB_OVERCURRENT_PIN *)(UINTN)PcdGet64 (PcdUsb30OverCurrentMappings);
  UpdatePchUsbConfig (
    &PchPolicy->UsbConfig,
    SetupVariables,
    PchRcVariables,
    Usb20OverCurrentMappings,
    Usb30OverCurrentMappings
    );
                  
  //
  // Update TraceHub config based on setup options
  //
  PchPolicy->PchTraceHubConfig.EnableMode        = PchRcVariables->TraceHubEnableMode;

  switch (PchRcVariables->MemRegion0BufferSize) {
    case 0:
       PchPolicy->PchTraceHubConfig.MemReg0Size = 0; // No memory
       break;
    case 1:
       PchPolicy->PchTraceHubConfig.MemReg0Size = 0x100000;  // 1MB
       break;
    case 2:
       PchPolicy->PchTraceHubConfig.MemReg0Size = 0x800000;  // 8MB
       break;
    case 3:
       PchPolicy->PchTraceHubConfig.MemReg0Size = 0x4000000; // 64MB
       break;
  }

  switch (PchRcVariables->MemRegion1BufferSize) {
    case 0:
       PchPolicy->PchTraceHubConfig.MemReg1Size = 0; // No memory
       break;
    case 1:
       PchPolicy->PchTraceHubConfig.MemReg1Size = 0x100000;  // 1MB
       break;
    case 2:
       PchPolicy->PchTraceHubConfig.MemReg1Size = 0x800000;  // 8MB
       break;
    case 3:
       PchPolicy->PchTraceHubConfig.MemReg1Size = 0x4000000; // 64MB
       break;
  }

  PchPolicy->PchTraceHubConfig.PchTraceHubHide  =  PchRcVariables->PchTraceHubHide;
  return EFI_SUCCESS;
}


/**
  Performs silicon pre-mem policy update.

  The meaning of Policy is defined by silicon code.
  It could be the raw data, a handle, a PPI, etc.
  
  The input Policy must be returned by SiliconPolicyDonePreMem().
  
  1) In FSP path, the input Policy should be FspmUpd.
  A platform may use this API to update the FSPM UPD policy initialized
  by the silicon module or the default UPD data.
  The output of FSPM UPD data from this API is the final UPD data.

  2) In non-FSP path, the board may use additional way to get
  the silicon policy data field based upon the input Policy.

  @param[in, out] Policy       Pointer to policy.

  @return the updated policy.
**/
VOID *
EFIAPI
SiliconPolicyUpdatePreMem (
  IN OUT VOID *Policy
  )
{
  UpdatePeiPchPolicy (Policy);
  return Policy;
}

/**
  Performs silicon post-mem policy update.

  The meaning of Policy is defined by silicon code.
  It could be the raw data, a handle, a PPI, etc.
  
  The input Policy must be returned by SiliconPolicyDonePostMem().
  
  1) In FSP path, the input Policy should be FspsUpd.
  A platform may use this API to update the FSPS UPD policy initialized
  by the silicon module or the default UPD data.
  The output of FSPS UPD data from this API is the final UPD data.

  2) In non-FSP path, the board may use additional way to get
  the silicon policy data field based upon the input Policy.

  @param[in, out] Policy       Pointer to policy.

  @return the updated policy.
**/
VOID *
EFIAPI
SiliconPolicyUpdatePostMem (
  IN OUT VOID *Policy
  )
{
  return Policy;
}
