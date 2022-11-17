/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialzation.

  @copyright
  Copyright 2004 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>

#include "Guid/SetupVariable.h"
#include <PchSetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>

#include <PchAccess.h>
#include <Ppi/PchPolicy.h>

#include <Register/PchRegsSata.h>
#include <Library/HobLib.h>
#include <Platform.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Guid/GlobalVariable.h>
#include <Ppi/PchPcieDeviceTable.h>
#include <Guid/SocketVariable.h>
#include <Library/PcdLib.h>
#include <Library/PchInfoLib.h>
#include <Library/UbaUsbOcUpdateLib.h>
#include <Library/UbaHsioPtssTableConfigLib.h>
#include <Ppi/DynamicSiLibraryPpi.h>
#include <FspmUpd.h>

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

#define PCI_CLASS_NETWORK             0x02
#define PCI_CLASS_NETWORK_ETHERNET    0x00
#define PCI_CLASS_NETWORK_OTHER       0x80


GLOBAL_REMOVE_IF_UNREFERENCED PCH_PCIE_DEVICE_OVERRIDE mPcieDeviceTable[] = {
  //
  // Intel PRO/Wireless
  //
  { 0x8086, 0x422b, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x422c, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x4238, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x4239, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel WiMAX/WiFi Link
  //
  { 0x8086, 0x0082, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0085, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0083, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0084, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0086, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0087, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0088, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0089, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x008F, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0090, 0xff, 0xff, 0xff, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel Crane Peak WLAN NIC
  //
  { 0x8086, 0x08AE, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x08AF, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel Crane Peak w/BT WLAN NIC
  //
  { 0x8086, 0x0896, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0897, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel Kelsey Peak WiFi, WiMax
  //
  { 0x8086, 0x0885, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0886, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel Centrino Wireless-N 105
  //
  { 0x8086, 0x0894, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0895, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel Centrino Wireless-N 135
  //
  { 0x8086, 0x0892, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0893, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel Centrino Wireless-N 2200
  //
  { 0x8086, 0x0890, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0891, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel Centrino Wireless-N 2230
  //
  { 0x8086, 0x0887, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x0888, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel Centrino Wireless-N 6235
  //
  { 0x8086, 0x088E, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x088F, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel CampPeak 2 Wifi
  //
  { 0x8086, 0x08B5, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  { 0x8086, 0x08B6, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },
  //
  // Intel WilkinsPeak 1 Wifi
  //
  { 0x8086, 0x08B3, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2AndL1SubstatesOverride, 0x0154, 0x00000003 },
  { 0x8086, 0x08B3, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1SubstatesOverride,        0x0158, 0x00000003 },
  { 0x8086, 0x08B4, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2AndL1SubstatesOverride, 0x0154, 0x00000003 },
  { 0x8086, 0x08B4, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1SubstatesOverride,        0x0158, 0x00000003 },
  //
  // Intel Wilkins Peak 2 Wifi
  //
  { 0x8086, 0x08B1, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2AndL1SubstatesOverride, 0x0154, 0x00000003 },
  { 0x8086, 0x08B1, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1SubstatesOverride,        0x0158, 0x00000003 },
  { 0x8086, 0x08B2, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2AndL1SubstatesOverride, 0x0154, 0x00000003 },
  { 0x8086, 0x08B2, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1SubstatesOverride,        0x0158, 0x00000003 },
  //
  // Intel Wilkins Peak PF Wifi
  //
  { 0x8086, 0x08B0, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1, PchPcieL1L2Override, 0, 0 },

  //
  // End of Table
  //
  { 0 }
};

STATIC
EFI_STATUS
InstallPcieDeviceTable (
  IN    PCH_PCIE_DEVICE_OVERRIDE         *DeviceTable
  )
{
  EFI_PEI_PPI_DESCRIPTOR  *DeviceTablePpiDesc;
  EFI_STATUS               Status;

  DeviceTablePpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (DeviceTablePpiDesc != NULL);

  if (DeviceTablePpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  DeviceTablePpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  DeviceTablePpiDesc->Guid  = &gPchPcieDeviceTablePpiGuid;
  DeviceTablePpiDesc->Ppi   = DeviceTable;

  Status = PeiServicesInstallPpi (DeviceTablePpiDesc);
  ASSERT_EFI_ERROR (Status);

  return Status;
}


VOID
UpdatePchUsbConfig (
  IN PCH_USB_CONFIG            *PchUsbConfig,
  IN SYSTEM_CONFIGURATION      *SetupVariables,
  IN PCH_SETUP                 *PchRcVariables,
  IN VOID                      *Usb20OverCurrentMappings,
  IN VOID                      *Usb30OverCurrentMappings,
  IN VOID                      *Usb20AfeParams
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
  UINT8                           BmcRootPort;
  UINT8                           *SmBusReservedTable;
  UINT8                           SmBusReservedNum;
  USB_OVERCURRENT_PIN             *Usb20OverCurrentMappings=NULL;
  USB_OVERCURRENT_PIN             *Usb30OverCurrentMappings=NULL;
  USB2_PHY_PARAMETERS                  *Usb20AfeParams = NULL;
  UINT8                           VTdSupport;
  SYSTEM_CONFIGURATION            *SetupVariables;
  PCH_SETUP                       *PchRcVariables;
  FSPM_UPD                        *FspmUpd;
  EFI_STATUS                      Status = EFI_SUCCESS;
  DYNAMIC_SI_LIBARY_PPI           *DynamicSiLibraryPpi = NULL;

  DEBUG((DEBUG_INFO, "platform common UpdatePeiPchPolicy entry\n"));

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  FspmUpd = (FSPM_UPD *) PcdGet32 (PcdFspmUpdDataAddress);
  ASSERT (FspmUpd != NULL);
  SetupVariables = PcdGetPtr(PcdSetup);
  PchRcVariables = PcdGetPtr(PcdPchSetup);

  LpcBaseAddress = DynamicSiLibraryPpi->MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_PCH,
                    PCI_DEVICE_NUMBER_PCH_LPC,
                    PCI_FUNCTION_NUMBER_PCH_LPC
                    );

  PchPolicy->Port80Route          = PchRcVariables->IchPort80Route;

  //
  // DeviceEnables
  //
  if (DynamicSiLibraryPpi->PchIsGbeAvailable ()) {
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
  MaxSataPorts = DynamicSiLibraryPpi->GetPchMaxSataPortNum ();

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
    PchPolicy->SataConfig.Rst.EfiRaidDriverLoad = PchRcVariables->SataRaidLoadEfiDriver[0];
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
  MaxSataPorts = DynamicSiLibraryPpi->GetPchMaxsSataPortNum ();

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
    PchPolicy->sSataConfig.Rst.EfiRaidDriverLoad = PchRcVariables->SataRaidLoadEfiDriver[0];;
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
  PchPolicy->PcieConfig.DisableRootPortClockGating      = SetupVariables->PcieClockGatingDisabled;
  PchPolicy->PcieConfig.EnablePort8xhDecode           = PchRcVariables->PcieRootPort8xhDecode;
  PchPolicy->PcieConfig.PchPciePort8xhDecodePortIndex = PchRcVariables->Pcie8xhDecodePortIndex;
  PchPolicy->PcieConfig.EnablePeerMemoryWrite         = PchRcVariables->PcieRootPortPeerMemoryWriteEnable;
  PchPolicy->PcieConfig.ComplianceTestMode            = PchRcVariables->PcieComplianceTestMode;

  ///
  /// Temporary WA: Force Link speed on BMC board to GEN1
  /// TODO: remove this WA together with Purley platforms support
  ///
  BmcRootPort = PcdGet8(PcdOemSkuBmcPciePortNumber);
  if ((BmcRootPort != 0xFF) && (BmcRootPort < ARRAY_SIZE(PchRcVariables->PcieRootPortSpeed))) {
    DEBUG ((DEBUG_INFO, "WA Force Link Speed to GEN1: PciePort: %d", BmcRootPort));
    PchRcVariables->PcieRootPortSpeed[BmcRootPort] = 1;
  }
  for (Index = 0; Index < DynamicSiLibraryPpi->GetPchMaxPciePortNum (); Index++) {
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
    PchPolicy->PcieConfig.RootPort[Index].SlotImplemented                = TRUE;
  }
  PchPolicy->PcieConfig.RootPort[BmcRootPort].SlotImplemented            = FALSE;

  for (Index = 0; Index < DynamicSiLibraryPpi->GetPchMaxPciePortNum (); ++Index) {
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
  CopyMem (&VTdSupport, (UINT8 *)PcdGetPtr(PcdSocketIioConfig) + OFFSET_OF(SOCKET_IIO_CONFIGURATION, VTdSupport), sizeof(VTdSupport));
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

  PchPolicy->HdAudioConfig.VcType = PchRcVariables->DfxHdaVcType;
  //
  // LockDown
  //


    PchPolicy->LockDownConfig.RtcLock          = PchRcVariables->PchRtcLock;
    PchPolicy->LockDownConfig.SpiEiss          = TRUE;
    PchPolicy->LockDownConfig.GlobalSmi        = TRUE;
    PchPolicy->LockDownConfig.BiosInterface    = TRUE;
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

  PchPolicy->PmConfig.PmcReadDisable = TRUE;

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
  PchPolicy->IoApicConfig.BusNumber         = PCI_BUS_NUMBER_PCH_IOAPIC;
  PchPolicy->IoApicConfig.DeviceNumber      = PCI_DEVICE_NUMBER_PCH_IOAPIC;
  PchPolicy->IoApicConfig.FunctionNumber    = PCI_FUNCTION_NUMBER_PCH_IOAPIC;


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
  PchPolicy->PmConfig.Dwr_BmcRootPort                       = PchRcVariables->Dwr_BmcRootPort;

  PchPolicy->PmConfig.PchGbl2HostEn.Bits.PMCGBL             = PchRcVariables->DwrEn_PMCGBL;
  PchPolicy->PmConfig.PchGbl2HostEn.Bits.MEWDT              = PchRcVariables->DwrEn_MEWDT;
  PchPolicy->PmConfig.PchGbl2HostEn.Bits.IEWDT              = PchRcVariables->DwrEn_IEWDT;

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
    PchPolicy->ThermalConfig.ThermalDeviceEnable = PchThermalDeviceEnabledPci;
  } else {
    PchPolicy->ThermalConfig.ThermalDeviceEnable = PchRcVariables->ThermalDeviceEnable;
  }

  PchPolicy->ThermalConfig.ThermalThrottling.TTLevels.SuggestedSetting   = TRUE;
  PchPolicy->ThermalConfig.ThermalThrottling.SataTT.SuggestedSetting     = TRUE;
  PchPolicy->ThermalConfig.ThermalThrottling.sSataTT.SuggestedSetting     = TRUE;

  //
  // DCI (EXI)
  //
  PchPolicy->DciConfig.DciEn         = DCI_DISABLE;
  PchPolicy->DciConfig.DciAutoDetect = DCI_DISABLE;

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
  PlatformGetUsbOcMappings (
    (USB_OVERCURRENT_PIN **) &Usb20OverCurrentMappings,
    (USB_OVERCURRENT_PIN **) &Usb30OverCurrentMappings,
    (USB2_PHY_PARAMETERS **)      &Usb20AfeParams
    );
  UpdatePchUsbConfig (
    &PchPolicy->UsbConfig,
    SetupVariables,
    PchRcVariables,
    Usb20OverCurrentMappings,
    Usb30OverCurrentMappings,
    Usb20AfeParams
    );

  //
  // Install PCIe device override table
  //
  InstallPcieDeviceTable (mPcieDeviceTable);

  //
  // Initialize PTSS board specyfic HSIO settings
  //
  InstallPlatformHsioPtssTable (PchRcVariables, PchPolicy);


  PchPolicy->PchTraceHubConfig.PchTraceHubHide = TRUE;
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
