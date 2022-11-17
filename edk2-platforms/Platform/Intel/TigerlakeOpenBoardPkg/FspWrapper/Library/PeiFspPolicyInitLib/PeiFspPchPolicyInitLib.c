/** @file
  Implementation of Fsp PCH Policy Initialization.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>

#include <Ppi/SiPolicy.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchInfoLib.h>
#include <Library/ConfigBlockLib.h>
#include <Pins/GpioPinsVer2Lp.h>
#include <TcssInfo.h>
#include <TcssPeiConfig.h>
#include <PolicyUpdateMacro.h>
#include <PlatformBoardConfig.h>

//
// USB limits
//
#define PCH_MAX_USB2_PORTS                  16
#define PCH_MAX_USB3_PORTS                  10

//
// TypeC port map GPIO pin
//
IOM_AUX_ORI_PAD_CONFIG mIomAuxNullTable[MAX_IOM_AUX_BIAS_COUNT] = {
  // Pull UP GPIO Pin, Pull Down GPIO pin
  {0,                  0}, // Port 0
  {0,                  0}, // Port 1
  {0,                  0}, // Port 2
  {0,                  0}, // Port 3
};


VOID
UpdatePcieClockInfo (
  PCH_PCIE_RP_PREMEM_CONFIG  *PcieRpPreMemConfig,
  IN FSPM_UPD                 *FspmUpd,
  UINTN                      Index,
  UINT64                     Data
  )
{
  PCD64_BLOB Pcd64;

  Pcd64.Blob = Data;
  DEBUG ((DEBUG_INFO, "UpdatePcieClockInfo ClkIndex %x ClkUsage %x, Supported %x\n", Index, Pcd64.PcieClock.ClockUsage, Pcd64.PcieClock.ClkReqSupported));

  UPDATE_POLICY (FspmUpd->FspmConfig.PcieClkSrcUsage[Index], PcieRpPreMemConfig->PcieClock[Index].Usage, (UINT8)Pcd64.PcieClock.ClockUsage);
  UPDATE_POLICY (FspmUpd->FspmConfig.PcieClkSrcClkReq[Index], PcieRpPreMemConfig->PcieClock[Index].ClkReq, Pcd64.PcieClock.ClkReqSupported ? (UINT8)Index : 0xFF);
}
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
  UINTN                        Index;
  PCH_PCIE_RP_PREMEM_CONFIG    *PcieRpPreMemConfig;
  HDAUDIO_PREMEM_CONFIG        *HdaPreMemConfig;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicy;
  EFI_STATUS                   Status;

  //
  // Locate PchPreMemPolicyPpi
  //
  SiPreMemPolicy = NULL;
  PcieRpPreMemConfig = NULL;
  HdaPreMemConfig = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
  ASSERT_EFI_ERROR (Status);
#else
  PcieRpPreMemConfig = NULL;
  HdaPreMemConfig = NULL;
#endif

  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 0, PcdGet64  (PcdPcieClock0));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 1, PcdGet64  (PcdPcieClock1));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 2, PcdGet64  (PcdPcieClock2));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 3, PcdGet64  (PcdPcieClock3));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 4, PcdGet64  (PcdPcieClock4));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 5, PcdGet64  (PcdPcieClock5));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 6, PcdGet64  (PcdPcieClock6));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 7, PcdGet64  (PcdPcieClock7));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 8, PcdGet64  (PcdPcieClock8));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 9, PcdGet64  (PcdPcieClock9));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 10, PcdGet64 (PcdPcieClock10));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 11, PcdGet64 (PcdPcieClock11));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 12, PcdGet64 (PcdPcieClock12));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 13, PcdGet64 (PcdPcieClock13));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 14, PcdGet64 (PcdPcieClock14));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 15, PcdGet64 (PcdPcieClock15));

  //
  // Update HDA policies
  //
  UPDATE_POLICY (FspmUpd->FspmConfig.PchHdaIDispLinkTmode, HdaPreMemConfig->IDispLinkTmode, 0);
  UPDATE_POLICY (FspmUpd->FspmConfig.PchHdaSdiEnable[0], HdaPreMemConfig->AudioLinkHda.SdiEnable[0], FALSE);

  for (Index = 0; Index < GetPchHdaMaxDmicLinkNum (); Index++) {
    UPDATE_POLICY (FspmUpd->FspmConfig.PchHdaAudioLinkDmicClockSelect[Index], HdaPreMemConfig->AudioLinkDmic[Index].DmicClockSelect, 0);
  }
  DEBUG((DEBUG_INFO | DEBUG_INIT, "UpdatePeiPchPolicyPreMem\n"));
  return EFI_SUCCESS;
}

/**
  This function updates USB Policy per port OC Pin number

  @param[in]  PchUsbConfig     Pointer to USB_CONFIG data buffer
  @param[in]  PortIndex        USB Port index
  @param[in]  Pin              OverCurrent pin number
**/
VOID
UpdateUsb20OverCurrentPolicy (
  IN OUT FSPS_UPD               *FspsUpd,
  IN USB_CONFIG                 *UsbConfig,
  IN UINT8                      PortIndex,
  UINT8                         Pin
)
{
  if (PortIndex < MAX_USB2_PORTS && ((Pin < USB_OC_MAX_PINS) || (Pin == USB_OC_SKIP))) {
      UPDATE_POLICY (
        FspsUpd->FspsConfig.Usb2OverCurrentPin[PortIndex],
        UsbConfig->PortUsb20[PortIndex].OverCurrentPin,
        Pin
        );
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
  IN OUT FSPS_UPD               *FspsUpd,
  IN USB_CONFIG                 *UsbConfig,
  IN UINT8                      PortIndex,
  UINT8                         Pin
)
{
  if (PortIndex < MAX_USB3_PORTS && ((Pin < USB_OC_MAX_PINS) || (Pin == USB_OC_SKIP))) {
    UPDATE_POLICY (
        FspsUpd->FspsConfig.Usb3OverCurrentPin[PortIndex],
        UsbConfig->PortUsb30[PortIndex].OverCurrentPin,
        Pin
        );
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
  IN OUT FSPS_UPD    *FspsUpd,
  IN OUT USB_CONFIG  *UsbConfig
  )
{
  UINTN              PortIndex;

  UPDATE_POLICY (FspsUpd->FspsConfig.PchUsbOverCurrentEnable, UsbConfig->OverCurrentEnable, TRUE);

  for (PortIndex = 0; PortIndex < GetPchUsb2MaxPhysicalPortNum (); PortIndex++) {
    UPDATE_POLICY (FspsUpd->FspsConfig.PortUsb20Enable[PortIndex], UsbConfig->PortUsb20[PortIndex].Enable, TRUE);
  }
  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
    UPDATE_POLICY (FspsUpd->FspsConfig.PortUsb30Enable[PortIndex], UsbConfig->PortUsb30[PortIndex].Enable, TRUE);
  }

  UPDATE_POLICY (FspsUpd->FspsConfig.XdciEnable, UsbConfig->XdciConfig.Enable, FALSE);

  //
  // Platform Board programming per the layout of each port.
  //
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 0, PcdGet8 (PcdUsb20OverCurrentPinPort0));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 1, PcdGet8 (PcdUsb20OverCurrentPinPort1));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 2, PcdGet8 (PcdUsb20OverCurrentPinPort2));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 3, PcdGet8 (PcdUsb20OverCurrentPinPort3));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 4, PcdGet8 (PcdUsb20OverCurrentPinPort4));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 5, PcdGet8 (PcdUsb20OverCurrentPinPort5));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 6, PcdGet8 (PcdUsb20OverCurrentPinPort6));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 7, PcdGet8 (PcdUsb20OverCurrentPinPort7));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 8, PcdGet8 (PcdUsb20OverCurrentPinPort8));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig, 9, PcdGet8 (PcdUsb20OverCurrentPinPort9));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig,10, PcdGet8 (PcdUsb20OverCurrentPinPort10));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig,11, PcdGet8 (PcdUsb20OverCurrentPinPort11));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig,12, PcdGet8 (PcdUsb20OverCurrentPinPort12));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig,13, PcdGet8 (PcdUsb20OverCurrentPinPort13));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig,14, PcdGet8 (PcdUsb20OverCurrentPinPort14));
  UpdateUsb20OverCurrentPolicy (FspsUpd, UsbConfig,15, PcdGet8 (PcdUsb20OverCurrentPinPort15));

  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 0, PcdGet8 (PcdUsb30OverCurrentPinPort0));
  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 1, PcdGet8 (PcdUsb30OverCurrentPinPort1));
  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 2, PcdGet8 (PcdUsb30OverCurrentPinPort2));
  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 3, PcdGet8 (PcdUsb30OverCurrentPinPort3));
  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 4, PcdGet8 (PcdUsb30OverCurrentPinPort4));
  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 5, PcdGet8 (PcdUsb30OverCurrentPinPort5));
  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 6, PcdGet8 (PcdUsb30OverCurrentPinPort6));
  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 7, PcdGet8 (PcdUsb30OverCurrentPinPort7));
  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 8, PcdGet8 (PcdUsb30OverCurrentPinPort8));
  UpdateUsb30OverCurrentPolicy (FspsUpd, UsbConfig, 9, PcdGet8 (PcdUsb30OverCurrentPinPort9));

}

/**
  Update CNVi config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] FspsUpd              Pointer to FspsUpd structure
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateCnviConfig (
  IN OUT FSPS_UPD              *FspsUpd,
  IN OUT CNVI_CONFIG           *CnviConfig
  )
{

  UPDATE_POLICY (FspsUpd->FspsConfig.CnviMode,           CnviConfig->Mode,           CnviModeDisabled);
  UPDATE_POLICY (FspsUpd->FspsConfig.CnviBtCore,         CnviConfig->BtCore,         FALSE);
  UPDATE_POLICY (FspsUpd->FspsConfig.CnviBtAudioOffload, CnviConfig->BtAudioOffload, 0);
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
  UINTN                        Index;
  SATA_CONFIG                  *SataConfig;
  USB_CONFIG                   *UsbConfig;
  TCSS_PEI_CONFIG              *TcssConfig;
  SERIAL_IO_CONFIG             *SerialIoConfig;
  CNVI_CONFIG                  *CnviConfig;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  SI_POLICY_PPI                *SiPolicy;
  EFI_STATUS                   Status;
#endif
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiPchPolicy\n"));

  SataConfig = NULL;
  UsbConfig = NULL;
  TcssConfig = NULL;
  SerialIoConfig = NULL;
  CnviConfig = NULL;
#if FixedPcdGet8(PcdFspModeSelection) == 0
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

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gTcssPeiConfigGuid, (VOID *) &TcssConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gCnviConfigGuid, (VOID *) &CnviConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
#endif

  //
  // Update Sata Policies
  //
  UPDATE_POLICY (FspsUpd->FspsConfig.SataEnable, SataConfig->Enable, TRUE);
  UPDATE_POLICY (FspsUpd->FspsConfig.SataMode, SataConfig->SataMode, SataModeAhci);

  for (Index = 0; Index < PCH_MAX_SATA_PORTS; Index++) {
    UPDATE_POLICY (FspsUpd->FspsConfig.SataPortsEnable[Index], SataConfig->PortSettings[Index].Enable, TRUE);
  }

  //
  // Update Pch Usb Config
  //
  UpdatePchUsbConfig (FspsUpd, UsbConfig);

  //
  // I2C
  //
  for (Index = 0; Index < 8; Index++) {
    UPDATE_POLICY (FspsUpd->FspsConfig.SerialIoI2cMode[Index], SerialIoConfig->I2cDeviceConfig[Index].Mode, 0);
    UPDATE_POLICY (FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[Index], SerialIoConfig->I2cDeviceConfig[Index].PadTermination, 0);
  }

  UPDATE_POLICY (FspsUpd->FspsConfig.PchSerialIoI2cSdaPinMux[4],  SerialIoConfig->I2cDeviceConfig[4].PinMux.Sda,  GPIO_VER2_LP_MUXING_SERIALIO_I2C4_SDA_GPP_H8);
  UPDATE_POLICY (FspsUpd->FspsConfig.PchSerialIoI2cSclPinMux[4],  SerialIoConfig->I2cDeviceConfig[4].PinMux.Scl,  GPIO_VER2_LP_MUXING_SERIALIO_I2C4_SCL_GPP_H9);

  //
  // Type C
  //
  for (Index = 0; Index < MAX_IOM_AUX_BIAS_COUNT; Index++) {
    UPDATE_POLICY (FspsUpd->FspsConfig.IomTypeCPortPadCfg[(Index * 2)],     TcssConfig->IomConfig.IomAuxPortPad[Index].GpioPullN, mIomAuxNullTable[Index].GpioPullN);
    UPDATE_POLICY (FspsUpd->FspsConfig.IomTypeCPortPadCfg[(Index * 2) + 1], TcssConfig->IomConfig.IomAuxPortPad[Index].GpioPullP, mIomAuxNullTable[Index].GpioPullP);
  }

  //
  // Cnvi
  //
  UpdateCnviConfig (FspsUpd, CnviConfig);

  return EFI_SUCCESS;
}
