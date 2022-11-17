/*******************************************************************************
Copyright (C) 2018 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include "XenonSdMmcOverride.h"

STATIC EFI_HANDLE              mXenonSdMmcOverrideHandle;
STATIC EDKII_SD_MMC_OVERRIDE  *mSdMmcOverride;

STATIC
EFI_STATUS
EFIAPI
XenonGetSdMmcDesc (
  IN      EFI_HANDLE              ControllerHandle,
  IN OUT  MV_BOARD_SDMMC_DESC     *SdMmcDesc
  )
{
  EFI_STATUS                      Status;
  MV_BOARD_SDMMC_DESC             *SdMmcDescs;
  NON_DISCOVERABLE_DEVICE         *Device;
  MARVELL_BOARD_DESC_PROTOCOL     *BoardDescProtocol;
  UINTN                           Index;

  Device = NULL;
  Status = gBS->OpenProtocol (ControllerHandle,
                  &gEdkiiNonDiscoverableDeviceProtocolGuid,
                  (VOID **) &Device,
                  mXenonSdMmcOverrideHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  BoardDescProtocol = NULL;
  Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                  NULL,
                  (VOID **) &BoardDescProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = BoardDescProtocol->BoardDescSdMmcGet (BoardDescProtocol, &SdMmcDescs);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < SdMmcDescs->SdMmcDevCount; Index++) {
    if (SdMmcDescs[Index].SoC->SdMmcBaseAddress ==
        Device->Resources[0].AddrRangeMin) {
      *SdMmcDesc = SdMmcDescs[Index];
      break;
    }
  }

  if (Index == SdMmcDescs->SdMmcDevCount) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
XenonGetPciIo (
  IN      EFI_HANDLE              ControllerHandle,
  IN OUT  EFI_PCI_IO_PROTOCOL     **PciIo
  )
{
  EFI_STATUS Status;

  *PciIo  = NULL;
  Status = gBS->OpenProtocol (ControllerHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) PciIo,
                  mXenonSdMmcOverrideHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  return Status;
}

/**
  Set SD Host Controller control 2 registry according to selected speed.

  @param[in] ControllerHandle The EFI_HANDLE of the controller.
  @param[in] Slot             The slot number of the SD card to send the command to.
  @param[in] Timing           The timing to select.

  @retval EFI_SUCCESS         The override function completed successfully.
  @retval EFI_NOT_FOUND       The specified controller or slot does not exist.
**/
STATIC
EFI_STATUS
XenonSdMmcHcUhsSignaling (
  IN EFI_HANDLE             ControllerHandle,
  IN UINT8                  Slot,
  IN SD_MMC_BUS_MODE        Timing
  )
{
  EFI_PCI_IO_PROTOCOL      *PciIo;
  EFI_STATUS                Status;
  UINT8                     HostCtrl2;
  UINT8                     XenonUhsSelect;

  if (Slot != 0) {
    return EFI_NOT_FOUND;
  }

  //
  // Update Host Control Register 2 only for HS200/HS400.
  //
  switch (Timing) {
    case SdMmcMmcHs200:
      XenonUhsSelect = XENON_SD_MMC_HC_CTRL_HS200;
      break;
    case SdMmcMmcHs400:
      XenonUhsSelect = XENON_SD_MMC_HC_CTRL_HS400;
      break;
    default:
      return EFI_SUCCESS;
  }

  Status = XenonGetPciIo (ControllerHandle, &PciIo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HostCtrl2 = (UINT8)~UHS_MODE_SELECT_MASK;
  Status = XenonHcAndMmio (PciIo,
             Slot,
             SDHC_HOST_CTRL2,
             sizeof (HostCtrl2),
             &HostCtrl2);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = XenonHcOrMmio (PciIo,
             Slot,
             SDHC_HOST_CTRL2,
             sizeof (XenonUhsSelect),
             &XenonUhsSelect);

  return Status;
}

/**

  Additional operations specific for host controller

  @param[in]      ControllerHandle      The EFI_HANDLE of the controller.
  @param[in]      Slot                  The 0 based slot index.
  @param[in]      Timing                The timing which should be set by
                                        host controller.

  @retval EFI_SUCCESS           The override function completed successfully.
  @retval EFI_NOT_FOUND         The specified controller or slot does not exist.

**/
STATIC
EFI_STATUS
XenonSwitchClockFreqPost (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN      SD_MMC_BUS_MODE                 Timing
  )
{
  EFI_STATUS                      Status;
  MV_BOARD_SDMMC_DESC             SdMmcDesc;
  EFI_PCI_IO_PROTOCOL             *PciIo;

  if (Slot != 0) {
    return EFI_NOT_FOUND;
  }

  Status = XenonGetPciIo (ControllerHandle, &PciIo);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = XenonGetSdMmcDesc (ControllerHandle, &SdMmcDesc);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = XenonSetPhy (PciIo,
             SdMmcDesc.XenonSlowModeEnabled,
             SdMmcDesc.XenonTuningStepDivisor,
             Timing);

  return Status;
}

/**

  Override function for SDHCI controller operations

  @param[in]      ControllerHandle      The EFI_HANDLE of the controller.
  @param[in]      Slot                  The 0 based slot index.
  @param[in]      PhaseType             The type of operation and whether the
                                        hook is invoked right before (pre) or
                                        right after (post)
  @param[in]      PhaseData             The pointer to a phase-specific data.

  @retval EFI_SUCCESS           The override function completed successfully.
  @retval EFI_NOT_FOUND         The specified controller or slot does not exist.
  @retval EFI_UNSUPPORTED       Nothing has been done in connection of PhaseType
  @retval EFI_INVALID_PARAMETER PhaseType is invalid

**/
STATIC
EFI_STATUS
EFIAPI
XenonSdMmcNotifyPhase (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN      EDKII_SD_MMC_PHASE_TYPE         PhaseType,
  IN OUT  VOID                           *PhaseData
  )
{
  EFI_STATUS                      Status;
  MV_BOARD_SDMMC_DESC             SdMmcDesc;
  EFI_PCI_IO_PROTOCOL             *PciIo;
  SD_MMC_BUS_MODE                 *Timing;

  if (Slot != 0) {
    return EFI_NOT_FOUND;
  }

  switch (PhaseType) {
    case EdkiiSdMmcInitHostPre:
      Status = XenonGetPciIo (ControllerHandle, &PciIo);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      Status = XenonGetSdMmcDesc (ControllerHandle, &SdMmcDesc);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      Status = XenonInit (PciIo,
                 SdMmcDesc.Xenon1v8Enabled,
                 SdMmcDesc.XenonSlowModeEnabled,
                 SdMmcDesc.XenonTuningStepDivisor);
      return Status;
    case EdkiiSdMmcUhsSignaling:
      if (PhaseData == NULL) {
        return EFI_INVALID_PARAMETER;
      }

      Timing = (SD_MMC_BUS_MODE *)PhaseData;

      Status = XenonSdMmcHcUhsSignaling (ControllerHandle,
                 Slot,
                 *Timing);
      if (EFI_ERROR (Status)) {
        return Status;
      }
      break;
    case EdkiiSdMmcSwitchClockFreqPost:
      if (PhaseData == NULL) {
        return EFI_INVALID_PARAMETER;
      }

      Timing = (SD_MMC_BUS_MODE *)PhaseData;

      Status = XenonSwitchClockFreqPost (ControllerHandle,
                 Slot,
                 *Timing);
      if (EFI_ERROR (Status)) {
        return Status;
      }
      break;
    default:
      return EFI_SUCCESS;
  }

  return EFI_SUCCESS;
}

/**

  Override function for SDHCI capability bits

  @param[in]      ControllerHandle      The EFI_HANDLE of the controller.
  @param[in]      Slot                  The 0 based slot index.
  @param[in,out]  SdMmcHcSlotCapability The SDHCI capability structure.
  @param[in,out]  BaseClkFreq           The base clock frequency value that
                                        optionally can be updated.

  @retval EFI_SUCCESS           The override function completed successfully.
  @retval EFI_NOT_FOUND         The specified controller or slot does not exist.
  @retval EFI_INVALID_PARAMETER SdMmcHcSlotCapability is NULL

**/
STATIC
EFI_STATUS
EFIAPI
XenonSdMmcCapability (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN OUT  VOID                            *SdMmcHcSlotCapability,
  IN OUT  UINT32                          *BaseClkFreq
  )
{
  EFI_STATUS           Status;
  MV_BOARD_SDMMC_DESC  SdMmcDesc;
  UINT64               Capability;

  if (SdMmcHcSlotCapability == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (Slot != 0) {
    return EFI_NOT_FOUND;
  }
  Status = XenonGetSdMmcDesc (ControllerHandle, &SdMmcDesc);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Capability = ReadUnaligned64 (SdMmcHcSlotCapability);

  //
  // Override capabilities structure according to board configuration.
  //
  if (SdMmcDesc.Xenon1v8Enabled) {
    Capability &= ~(UINT64)(SDHC_CAP_VOLTAGE_33 | SDHC_CAP_VOLTAGE_30);
  } else {
    Capability &= ~(UINT64)(SDHC_CAP_SDR104 | SDHC_CAP_DDR50 |
                            SDHC_CAP_SDR50 | SDHC_CAP_HS400 |
                            SDHC_CAP_VOLTAGE_18);
  }

  if (!SdMmcDesc.Xenon8BitBusEnabled) {
    Capability &= ~(UINT64)(SDHC_CAP_BUS_WIDTH8);
  }

  if (SdMmcDesc.XenonSlowModeEnabled) {
    Capability &= ~(UINT64)(SDHC_CAP_SDR104 | SDHC_CAP_DDR50 | SDHC_CAP_HS400);
  }

  Capability &= ~(UINT64)(SDHC_CAP_SLOT_TYPE_MASK);
  Capability |= SdMmcDesc.SlotType << SDHC_CAP_SLOT_TYPE_OFFSET;

  WriteUnaligned64 (SdMmcHcSlotCapability, Capability);

  //
  // Override inappropriate base clock frequency from Capabilities Register 1.
  // Actual clock speed of Xenon controller is 400MHz.
  //
  *BaseClkFreq = XENON_MMC_MAX_CLK / 1000 / 1000;

  return EFI_SUCCESS;
}

/**
  The entry point for Xenon driver, used to install SdMMcOverrideProtocol
  on the ImageHandle.

  @param[in]  ImageHandle   The firmware allocated handle for this driver image.
  @param[in]  SystemTable   Pointer to the EFI system table.

  @retval EFI_SUCCESS   Driver loaded.
  @retval other         Driver not loaded.

**/
EFI_STATUS
EFIAPI
InitializeXenonDxe (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;

  mSdMmcOverride = AllocateZeroPool (sizeof (EDKII_SD_MMC_OVERRIDE));
  if (mSdMmcOverride == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  mSdMmcOverride->Version = EDKII_SD_MMC_OVERRIDE_PROTOCOL_VERSION;
  mSdMmcOverride->Capability = XenonSdMmcCapability;
  mSdMmcOverride->NotifyPhase = XenonSdMmcNotifyPhase;

  Status = gBS->InstallProtocolInterface (&ImageHandle,
                  &gEdkiiSdMmcOverrideProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  mSdMmcOverride);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Filed to install SdMmcOverride protocol\n",
      __FUNCTION__));
    return Status;
  }

  mXenonSdMmcOverrideHandle = ImageHandle;

  return Status;
}
