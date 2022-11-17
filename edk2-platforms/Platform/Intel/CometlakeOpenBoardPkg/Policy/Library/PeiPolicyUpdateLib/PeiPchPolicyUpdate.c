/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPchPolicyUpdate.h"
#include <Library/BaseMemoryLib.h>
#include <Library/HdaVerbTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/PchGbeLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Ppi/Spi.h>
#include <GpioConfig.h>
#include <Library/DebugLib.h>
#include <Library/PchGbeLib.h>
#include <PlatformBoardConfig.h>
#include <Library/CnviLib.h>
#include <Register/PchRegsLpcCnl.h>
#include <PcieDeviceOverrideTable.h>
#include <Library/ConfigBlockLib.h>

VOID
UpdatePcieClockInfo (
  PCH_PCIE_CONFIG  *PcieRpConfig,
  UINTN            Index,
  UINT64           Data
  )
{
  PCD64_BLOB Pcd64;

  Pcd64.Blob = Data;
  DEBUG ((DEBUG_INFO, "UpdatePcieClockInfo ClkIndex %x ClkUsage %x, Supported %x\n", Index, Pcd64.PcieClock.ClockUsage, Pcd64.PcieClock.ClkReqSupported));

  PcieRpConfig->PcieClock[Index].Usage = (UINT8)Pcd64.PcieClock.ClockUsage;
  if (Pcd64.PcieClock.ClkReqSupported) {
    PcieRpConfig->PcieClock[Index].ClkReq = (UINT8)Index;
  } else {
    PcieRpConfig->PcieClock[Index].ClkReq = 0xFF;
  }
}

/**
  This is helper function for getting I2C Pads Internal Termination settings from Pcd

  @param[in]  Index            I2C Controller Index
**/
UINT8
GetSerialIoI2cPadsTerminationFromPcd (
  IN UINT8 Index
)
{
  switch (Index) {
    case 0:
      return PcdGet8 (PcdPchSerialIoI2c0PadInternalTerm);
    case 1:
      return PcdGet8 (PcdPchSerialIoI2c1PadInternalTerm);
    case 2:
      return PcdGet8 (PcdPchSerialIoI2c2PadInternalTerm);
    case 3:
      return PcdGet8 (PcdPchSerialIoI2c3PadInternalTerm);
    case 4:
      return PcdGet8 (PcdPchSerialIoI2c4PadInternalTerm);
    case 5:
      return PcdGet8 (PcdPchSerialIoI2c5PadInternalTerm);
    default:
      ASSERT (FALSE); // Invalid I2C Controller Index
  }
  return 0;
}
/**
  This is a helper function for updating USB Policy according to Blob data

  @param[in]  UsbConfig        Pointer to USB_CONFIG data buffer
  @param[in]  PortIndex        USB Port index
  @param[in]  Data32           Blob containing USB2 Afe (PCD32_BLOB) data
**/
VOID
UpdateUsb20AfePolicy (
  IN USB_CONFIG                 *UsbConfig,
  IN UINT8                      PortIndex,
  UINT32                        Data32
)
{
  PCD32_BLOB Pcd32;
  Pcd32.Blob = Data32;

  if (PortIndex < MAX_USB2_PORTS && Pcd32.Info.Petxiset != 0) {
    UsbConfig->PortUsb20[PortIndex].Afe.Petxiset     = Pcd32.Info.Petxiset;
    UsbConfig->PortUsb20[PortIndex].Afe.Txiset       = Pcd32.Info.Txiset;
    UsbConfig->PortUsb20[PortIndex].Afe.Predeemp     = Pcd32.Info.Predeemp;
    UsbConfig->PortUsb20[PortIndex].Afe.Pehalfbit    = Pcd32.Info.Pehalfbit;
  }
}

/**
  This function updates USB Policy per port OC Pin number

  @param[in]  PchUsbConfig     Pointer to USB_CONFIG data buffer
  @param[in]  PortIndex        USB Port index
  @param[in]  Pin              OverCurrent pin number
**/
VOID
UpdateUsb20OverCurrentPolicy (
  IN USB_CONFIG                 *UsbConfig,
  IN UINT8                      PortIndex,
  UINT8                         Pin
)
{
  if (PortIndex < MAX_USB2_PORTS && ((Pin < UsbOverCurrentPinMax) || (Pin == UsbOverCurrentPinSkip))) {
    UsbConfig->PortUsb20[PortIndex].OverCurrentPin = Pin;
  } else {
    if (PortIndex >= MAX_USB2_PORTS) {
      DEBUG ((DEBUG_ERROR, "UpdateUsb20OverCurrentPolicy: USB2 port number %d is not a valid USB2 port number\n", PortIndex));
    } else {
      DEBUG ((DEBUG_ERROR, "UpdateUsb20OverCurrentPolicy: Invalid OverCurrent pin specified USB2 port %d\n", PortIndex));
    }
  }
}

/**
  This function updates USB Policy per port OC Pin number

  @param[in]  PchUsbConfig     Pointer to USB_CONFIG data buffer
  @param[in]  PortIndex        USB Port index
  @param[in]  Pin              OverCurrent pin number
**/
VOID
UpdateUsb30OverCurrentPolicy (
  IN USB_CONFIG                 *UsbConfig,
  IN UINT8                      PortIndex,
  UINT8                         Pin
)
{
  if (PortIndex < MAX_USB3_PORTS && ((Pin < UsbOverCurrentPinMax) || (Pin == UsbOverCurrentPinSkip))) {
    UsbConfig->PortUsb30[PortIndex].OverCurrentPin = Pin;
  } else {
    if (PortIndex >= MAX_USB2_PORTS) {
      DEBUG ((DEBUG_ERROR, "UpdateUsb30OverCurrentPolicy: USB3 port number %d is not a valid USB3 port number\n", PortIndex));
    } else {
      DEBUG ((DEBUG_ERROR, "UpdateUsb30OverCurrentPolicy: Invalid OverCurrent pin specified USB3 port %d\n", PortIndex));
    }
  }
}

/**
  This function performs PCH USB Platform Policy initialization

  @param[in] PchUsbConfig         Pointer to USB_CONFIG data buffer
  @param[in] PchSetup             Pointer to PCH_SETUP data buffer
**/
VOID
UpdatePchUsbConfig (
  IN USB_CONFIG                *UsbConfig
  )
{
  UINTN              PortIndex;

  UsbConfig->OverCurrentEnable = TRUE;

  for (PortIndex = 0; PortIndex < GetPchUsb2MaxPhysicalPortNum (); PortIndex++) {
      UsbConfig->PortUsb20[PortIndex].Enable = TRUE;
  }
  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
      UsbConfig->PortUsb30[PortIndex].Enable = TRUE;
  }

  UsbConfig->XdciConfig.Enable = FALSE;


  //
  // USB2 AFE settings.
  //
  UpdateUsb20AfePolicy (UsbConfig, 0, PcdGet32 (PcdUsb20Port0Afe));
  UpdateUsb20AfePolicy (UsbConfig, 1, PcdGet32 (PcdUsb20Port1Afe));
  UpdateUsb20AfePolicy (UsbConfig, 2, PcdGet32 (PcdUsb20Port2Afe));
  UpdateUsb20AfePolicy (UsbConfig, 3, PcdGet32 (PcdUsb20Port3Afe));
  UpdateUsb20AfePolicy (UsbConfig, 4, PcdGet32 (PcdUsb20Port4Afe));
  UpdateUsb20AfePolicy (UsbConfig, 5, PcdGet32 (PcdUsb20Port5Afe));
  UpdateUsb20AfePolicy (UsbConfig, 6, PcdGet32 (PcdUsb20Port6Afe));
  UpdateUsb20AfePolicy (UsbConfig, 7, PcdGet32 (PcdUsb20Port7Afe));
  UpdateUsb20AfePolicy (UsbConfig, 8, PcdGet32 (PcdUsb20Port8Afe));
  UpdateUsb20AfePolicy (UsbConfig, 9, PcdGet32 (PcdUsb20Port9Afe));
  UpdateUsb20AfePolicy (UsbConfig,10, PcdGet32 (PcdUsb20Port10Afe));
  UpdateUsb20AfePolicy (UsbConfig,11, PcdGet32 (PcdUsb20Port11Afe));
  UpdateUsb20AfePolicy (UsbConfig,12, PcdGet32 (PcdUsb20Port12Afe));
  UpdateUsb20AfePolicy (UsbConfig,13, PcdGet32 (PcdUsb20Port13Afe));
  UpdateUsb20AfePolicy (UsbConfig,14, PcdGet32 (PcdUsb20Port14Afe));
  UpdateUsb20AfePolicy (UsbConfig,15, PcdGet32 (PcdUsb20Port15Afe));

  //
  // Platform Board programming per the layout of each port.
  //
  UpdateUsb20OverCurrentPolicy (UsbConfig, 0, PcdGet8 (PcdUsb20OverCurrentPinPort0));
  UpdateUsb20OverCurrentPolicy (UsbConfig, 1, PcdGet8 (PcdUsb20OverCurrentPinPort1));
  UpdateUsb20OverCurrentPolicy (UsbConfig, 2, PcdGet8 (PcdUsb20OverCurrentPinPort2));
  UpdateUsb20OverCurrentPolicy (UsbConfig, 3, PcdGet8 (PcdUsb20OverCurrentPinPort3));
  UpdateUsb20OverCurrentPolicy (UsbConfig, 4, PcdGet8 (PcdUsb20OverCurrentPinPort4));
  UpdateUsb20OverCurrentPolicy (UsbConfig, 5, PcdGet8 (PcdUsb20OverCurrentPinPort5));
  UpdateUsb20OverCurrentPolicy (UsbConfig, 6, PcdGet8 (PcdUsb20OverCurrentPinPort6));
  UpdateUsb20OverCurrentPolicy (UsbConfig, 7, PcdGet8 (PcdUsb20OverCurrentPinPort7));
  UpdateUsb20OverCurrentPolicy (UsbConfig, 8, PcdGet8 (PcdUsb20OverCurrentPinPort8));
  UpdateUsb20OverCurrentPolicy (UsbConfig, 9, PcdGet8 (PcdUsb20OverCurrentPinPort9));
  UpdateUsb20OverCurrentPolicy (UsbConfig,10, PcdGet8 (PcdUsb20OverCurrentPinPort10));
  UpdateUsb20OverCurrentPolicy (UsbConfig,11, PcdGet8 (PcdUsb20OverCurrentPinPort11));
  UpdateUsb20OverCurrentPolicy (UsbConfig,12, PcdGet8 (PcdUsb20OverCurrentPinPort12));
  UpdateUsb20OverCurrentPolicy (UsbConfig,13, PcdGet8 (PcdUsb20OverCurrentPinPort13));
  UpdateUsb20OverCurrentPolicy (UsbConfig,14, PcdGet8 (PcdUsb20OverCurrentPinPort14));
  UpdateUsb20OverCurrentPolicy (UsbConfig,15, PcdGet8 (PcdUsb20OverCurrentPinPort15));

  UpdateUsb30OverCurrentPolicy (UsbConfig, 0, PcdGet8 (PcdUsb30OverCurrentPinPort0));
  UpdateUsb30OverCurrentPolicy (UsbConfig, 1, PcdGet8 (PcdUsb30OverCurrentPinPort1));
  UpdateUsb30OverCurrentPolicy (UsbConfig, 2, PcdGet8 (PcdUsb30OverCurrentPinPort2));
  UpdateUsb30OverCurrentPolicy (UsbConfig, 3, PcdGet8 (PcdUsb30OverCurrentPinPort3));
  UpdateUsb30OverCurrentPolicy (UsbConfig, 4, PcdGet8 (PcdUsb30OverCurrentPinPort4));
  UpdateUsb30OverCurrentPolicy (UsbConfig, 5, PcdGet8 (PcdUsb30OverCurrentPinPort5));
  UpdateUsb30OverCurrentPolicy (UsbConfig, 6, PcdGet8 (PcdUsb30OverCurrentPinPort6));
  UpdateUsb30OverCurrentPolicy (UsbConfig, 7, PcdGet8 (PcdUsb30OverCurrentPinPort7));
  UpdateUsb30OverCurrentPolicy (UsbConfig, 8, PcdGet8 (PcdUsb30OverCurrentPinPort8));
  UpdateUsb30OverCurrentPolicy (UsbConfig, 9, PcdGet8 (PcdUsb30OverCurrentPinPort9));

}

/**
  Return if input ImageGuid belongs to system FMP GUID list.

  @param[in] ImageGuid A pointer to GUID

  @retval TRUE  ImageGuid is in the list of PcdSystemFmpCapsuleImageTypeIdGuid
  @retval FALSE ImageGuid is not in the list of PcdSystemFmpCapsuleImageTypeIdGuid
**/
BOOLEAN
IsSystemFmpGuid (
  IN GUID   *ImageGuid
  )
{
  GUID      *Guid;
  UINTN     Count;
  UINTN     Index;

  Guid = PcdGetPtr (PcdSystemFmpCapsuleImageTypeIdGuid);
  Count = PcdGetSize (PcdSystemFmpCapsuleImageTypeIdGuid) / sizeof (GUID);

  for (Index = 0; Index < Count; Index++, Guid++) {
    if (CompareGuid (ImageGuid, Guid)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  This function performs PCH PEI Policy initialization.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicy (
  IN OUT      SI_POLICY_PPI     *SiPolicy
  )
{
  EFI_STATUS                      Status;
  UINT8                           Index;
  DMI_HW_WIDTH_CONTROL            *DmiHaAWC;
  PCH_GENERAL_CONFIG              *PchGeneralConfig;
  PCH_PCIE_CONFIG                 *PcieRpConfig;
  PCH_SATA_CONFIG                 *SataConfig;
  PCH_IOAPIC_CONFIG               *IoApicConfig;
  PCH_DMI_CONFIG                  *DmiConfig;
  PCH_FLASH_PROTECTION_CONFIG     *FlashProtectionConfig;
  PCH_HDAUDIO_CONFIG              *HdAudioConfig;
  PCH_INTERRUPT_CONFIG            *InterruptConfig;
  PCH_ISH_CONFIG                  *IshConfig;
  PCH_LAN_CONFIG                  *LanConfig;
  PCH_LOCK_DOWN_CONFIG            *LockDownConfig;
  PCH_PM_CONFIG                   *PmConfig;
  PCH_SCS_CONFIG                  *ScsConfig;
  PCH_SERIAL_IO_CONFIG            *SerialIoConfig;
  PCH_LPC_SIRQ_CONFIG             *SerialIrqConfig;
  PCH_THERMAL_CONFIG              *ThermalConfig;
  USB_CONFIG                      *UsbConfig;
  PCH_ESPI_CONFIG                 *EspiConfig;
  PCH_CNVI_CONFIG                 *CnviConfig;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPcieRpConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIshConfigGuid, (VOID *) &IshConfig);
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
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIrqConfigGuid, (VOID *) &SerialIrqConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gCnviConfigGuid, (VOID *) &CnviConfig);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesLocatePpi (
                &gSiPreMemPolicyPpiGuid,
                0,
                NULL,
                (VOID **) &SiPreMemPolicyPpi
                );
  ASSERT_EFI_ERROR (Status);

  DmiConfig->PwrOptEnable = TRUE;
  PmConfig->PchSlpS3MinAssert = 0;
  PmConfig->PchSlpS4MinAssert = 0;
  PmConfig->PchSlpSusMinAssert = 0;
  PmConfig->PchSlpAMinAssert = 0;

  SataConfig->ThermalThrottling.P1T3M = 3;
  SataConfig->ThermalThrottling.P1T2M = 2;
  SataConfig->ThermalThrottling.P1T1M = 1;
  SataConfig->ThermalThrottling.P0T3M = 3;
  SataConfig->ThermalThrottling.P0T2M = 2;
  SataConfig->ThermalThrottling.P0T1M = 1;

  UpdatePcieClockInfo (PcieRpConfig, 0, PcdGet64  (PcdPcieClock0));
  UpdatePcieClockInfo (PcieRpConfig, 1, PcdGet64  (PcdPcieClock1));
  UpdatePcieClockInfo (PcieRpConfig, 2, PcdGet64  (PcdPcieClock2));
  UpdatePcieClockInfo (PcieRpConfig, 3, PcdGet64  (PcdPcieClock3));
  UpdatePcieClockInfo (PcieRpConfig, 4, PcdGet64  (PcdPcieClock4));
  UpdatePcieClockInfo (PcieRpConfig, 5, PcdGet64  (PcdPcieClock5));
  UpdatePcieClockInfo (PcieRpConfig, 6, PcdGet64  (PcdPcieClock6));
  UpdatePcieClockInfo (PcieRpConfig, 7, PcdGet64  (PcdPcieClock7));
  UpdatePcieClockInfo (PcieRpConfig, 8, PcdGet64  (PcdPcieClock8));
  UpdatePcieClockInfo (PcieRpConfig, 9, PcdGet64  (PcdPcieClock9));
  UpdatePcieClockInfo (PcieRpConfig, 10, PcdGet64 (PcdPcieClock10));
  UpdatePcieClockInfo (PcieRpConfig, 11, PcdGet64 (PcdPcieClock11));
  UpdatePcieClockInfo (PcieRpConfig, 12, PcdGet64 (PcdPcieClock12));
  UpdatePcieClockInfo (PcieRpConfig, 13, PcdGet64 (PcdPcieClock13));
  UpdatePcieClockInfo (PcieRpConfig, 14, PcdGet64 (PcdPcieClock14));
  UpdatePcieClockInfo (PcieRpConfig, 15, PcdGet64 (PcdPcieClock15));

  PcieRpConfig->PcieDeviceOverrideTablePtr = (UINT32) mPcieDeviceTable;
  PcieRpConfig->RootPort[0].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[1].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[2].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[3].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[4].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[5].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[6].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[7].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[8].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[9].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[10].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[11].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[12].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[13].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[14].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[15].ClkReqDetect = TRUE;
  PcieRpConfig->RootPort[0].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[1].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[2].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[3].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[4].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[5].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[6].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[7].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[8].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[9].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[10].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[11].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[12].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[13].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[14].AdvancedErrorReporting = TRUE;
  PcieRpConfig->RootPort[15].AdvancedErrorReporting = TRUE;

  //
  // Install HDA Link/iDisplay Codec Verb Table
  //
  AddPlatformVerbTables (
    PchHdaCodecPlatformOnboard,
    &(HdAudioConfig->VerbTableEntryNum),
    &(HdAudioConfig->VerbTablePtr)
    );

  LockDownConfig->BiosLock = FALSE;
  LockDownConfig->BiosInterface = FALSE;

  //
  // IOAPIC Config
  //
//  IoApicConfig->IoApicEntry24_119     = PchSetup.PchIoApic24119Entries;
  //
  // To support SLP_S0, it's required to disable 8254 timer.
  // Note that CSM may require this option to be disabled for correct operation.
  // Once 8254 timer disabled, some legacy OPROM and legacy OS will fail while using 8254 timer.
  // For some OS environment that it needs to set 8254CGE in late state it should
  // set this policy to FALSE and use PmcSet8254ClockGateState (TRUE) in SMM later.
  // This is also required during S3 resume.
  //
  // The Enable8254ClockGatingOnS3 is only applicable when Enable8254ClockGating is disabled.
  // If Enable8254ClockGating is enabled, RC will do 8254 CGE programming on S3 as well.
  // else, RC will do the programming on S3 when Enable8254ClockGatingOnS3 is enabled.
  // This avoids the SMI requirement for the programming.
  //
  // If S0ix is not enabled, then disable 8254CGE for leagcy boot case.
  //
  IoApicConfig->Enable8254ClockGating     = FALSE;
  IoApicConfig->Enable8254ClockGatingOnS3 = FALSE;

  //
  // SerialIo Config
  //
  SerialIoConfig->DevMode[0] = 1;
  SerialIoConfig->DevMode[1] = 1;
  SerialIoConfig->DevMode[2] = 0;
  SerialIoConfig->DevMode[3] = 0;
  SerialIoConfig->DevMode[4] = 1;
  SerialIoConfig->DevMode[5] = 0;
  SerialIoConfig->DevMode[6] = 0;
  SerialIoConfig->DevMode[7] = 0;
  SerialIoConfig->DevMode[8] = 0;
  SerialIoConfig->DevMode[9] = 0;
  SerialIoConfig->DevMode[10] = 0;
  SerialIoConfig->DevMode[11] = 3;

  SerialIoConfig->Uart0PinMuxing = 1;
  SerialIoConfig->SpiCsPolarity[0] = 1;
  SerialIoConfig->SpiCsPolarity[1] = 0;
  SerialIoConfig->SpiCsPolarity[2] = 0;

  SerialIoConfig->UartHwFlowCtrl[0] = 1;
  SerialIoConfig->UartHwFlowCtrl[1] = 1;
  SerialIoConfig->UartHwFlowCtrl[2] = 1;
  //
  // I2C4 and I2C5 don't exist in SPT-H chipset
  //
  if (IsPchH ()) {
    SerialIoConfig->DevMode[PchSerialIoIndexI2C4] = PchSerialIoDisabled;
    SerialIoConfig->DevMode[PchSerialIoIndexI2C5] = PchSerialIoDisabled;
  }

  for (Index = 0; Index < GetPchMaxSerialIoI2cControllersNum (); Index++) {
    SerialIoConfig->I2cPadsTermination[Index] = GetSerialIoI2cPadsTerminationFromPcd (Index);
  }

  PmConfig->SlpS0Override                         = 2; //PchSetup.SlpS0Override;
  PmConfig->SlpS0DisQForDebug                     = 3;  //PchSetup.SlpS0DisQForDebug;
  PmConfig->SlpS0Vm075VSupport                    = 1; // PcdGetBool(PcdSlpS0Vm075VSupport);
  PmConfig->CpuC10GatePinEnable                   = 1;

  //
  // Thermal Config
  //
  ThermalConfig->TsmicLock           = TRUE;
  ThermalConfig->PchHotEnable        = PcdGetBool (PcdPchThermalHotEnable);

  DmiHaAWC = &ThermalConfig->DmiHaAWC;
  DmiHaAWC->TS3TW = 0;
  DmiHaAWC->TS2TW = 1;
  DmiHaAWC->TS1TW = 2;
  DmiHaAWC->TS0TW = 3;
  //
  // Update Pch Usb Config
  //
  UpdatePchUsbConfig (
    UsbConfig
    );

  ScsConfig->ScsUfsEnabled = 0;
  ScsConfig->ScsEmmcHs400Enabled = 1;
  ScsConfig->ScsEmmcHs400TuningRequired = TRUE;

  IshConfig->I2c0GpioAssign = 1;
  IshConfig->I2c1GpioAssign = 1;
  IshConfig->Gp0GpioAssign = 1;
  IshConfig->Gp1GpioAssign = 1;
  IshConfig->Gp2GpioAssign = 1;
  IshConfig->Gp3GpioAssign = 1;
  IshConfig->Gp4GpioAssign = 1;
  IshConfig->Gp5GpioAssign = 1;
  IshConfig->Gp6GpioAssign = 1;

  return Status;
}
