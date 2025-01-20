/** @file
 *
 *  Synopsys DesignWare Cores SDHCI eMMC driver
 *
 *  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>
 *  Copyright (c) 2022, Patrick Wildt <patrick@blueri.se>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DwcSdhciPlatformLib.h>

#include <Protocol/NonDiscoverableDevice.h>
#include <Protocol/SdMmcOverride.h>

#include "DwcSdhciDxe.h"

#define EMMC_FORCE_HIGH_SPEED  FixedPcdGetBool(PcdDwcSdhciForceHighSpeed)
#define EMMC_DISABLE_HS400     FixedPcdGetBool(PcdDwcSdhciDisableHs400)

STATIC EFI_HANDLE  mSdMmcControllerHandle;

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
EmmcSdMmcCapability (
  IN      EFI_HANDLE  ControllerHandle,
  IN      UINT8       Slot,
  IN OUT  VOID        *SdMmcHcSlotCapability,
  IN OUT  UINT32      *BaseClkFreq
  )
{
  SD_MMC_HC_SLOT_CAP  *Capability = SdMmcHcSlotCapability;

  if (SdMmcHcSlotCapability == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (ControllerHandle != mSdMmcControllerHandle) {
    return EFI_NOT_FOUND;
  }

  //
  // Disable ADMA2 to avoid data corruption.
  // This controller has the limitation that a single descriptor
  // cannot cross 128 MB boundaries and must be split.
  // This would require a patch in SdMmcPciHcDxe, but SDMA works
  // fine for the time being.
  //
  Capability->Adma2 = 0;

  Capability->Hs400 = !EMMC_DISABLE_HS400;

  if (EMMC_FORCE_HIGH_SPEED) {
    Capability->BaseClkFreq = 52;
    Capability->Sdr50       = 0;
    Capability->Ddr50       = 0;
    Capability->Sdr104      = 0;
    Capability->Hs400       = 0;
  }

  return EFI_SUCCESS;
}

/**

  Override function for SDHCI controller operations

  @param[in]      ControllerHandle      The EFI_HANDLE of the controller.
  @param[in]      Slot                  The 0 based slot index.
  @param[in]      PhaseType             The type of operation and whether the
                                        hook is invoked right before (pre) or
                                        right after (post)
  @param[in,out]  PhaseData             The pointer to a phase-specific data.

  @retval EFI_SUCCESS           The override function completed successfully.
  @retval EFI_NOT_FOUND         The specified controller or slot does not exist.
  @retval EFI_INVALID_PARAMETER PhaseType is invalid

**/
STATIC
EFI_STATUS
EFIAPI
EmmcSdMmcNotifyPhase (
  IN      EFI_HANDLE               ControllerHandle,
  IN      UINT8                    Slot,
  IN      EDKII_SD_MMC_PHASE_TYPE  PhaseType,
  IN OUT  VOID                     *PhaseData
  )
{
  SD_MMC_BUS_MODE  *Timing;
  UINTN            MaxClockFreq;
  UINT32           Value, i;
  UINT32           TxClkTapNum;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  if (ControllerHandle != mSdMmcControllerHandle) {
    return EFI_SUCCESS;
  }

  ASSERT (Slot == 0);

  switch (PhaseType) {
    case EdkiiSdMmcInitHostPost:
      /*
       * Just before this Notification POWER_CTRL is toggled to power off
       * and on the card.  On this controller implementation, toggling
       * power off also removes SDCLK_ENABLE (BIT2) from from CLOCK_CTRL.
       * Since the clock has already been set up prior to the power toggle,
       * re-add the SDCLK_ENABLE bit to start the clock.
       */
      MmioOr16 ((UINT32)SD_MMC_HC_CLOCK_CTRL, CLOCK_CTRL_SDCLK_ENABLE);
      break;

    case EdkiiSdMmcUhsSignaling:
      if (PhaseData == NULL) {
        return EFI_INVALID_PARAMETER;
      }

      Timing = (SD_MMC_BUS_MODE *)PhaseData;
      if (*Timing == SdMmcMmcHs400) {
        /* HS400 uses a non-standard setting */
        MmioOr16 ((UINT32)SD_MMC_HC_HOST_CTRL2, HOST_CTRL2_HS400);
      }

      break;

    case EdkiiSdMmcSwitchClockFreqPost:
      if (PhaseData == NULL) {
        return EFI_INVALID_PARAMETER;
      }

      Timing = (SD_MMC_BUS_MODE *)PhaseData;
      switch (*Timing) {
        case SdMmcMmcHs400:
        case SdMmcMmcHs200:
          MaxClockFreq = 200000000UL;
          break;
        case SdMmcMmcHsSdr:
        case SdMmcMmcHsDdr:
          MaxClockFreq = 52000000UL;
          break;
        default:
          MaxClockFreq = 26000000UL;
          break;
      }

      DwcSdhciSetClockRate (MaxClockFreq);

      if (MaxClockFreq <= 52000000UL) {
        MmioWrite32 (EMMC_DLL_CTRL, 0);
        MmioWrite32 (EMMC_DLL_RXCLK, 0);
        MmioWrite32 (EMMC_DLL_TXCLK, 0);
        MmioWrite32 (EMMC_DLL_CMDOUT, 0);
        MmioWrite32 (
          EMMC_DLL_STRBIN,
          EMMC_DLL_DLYENA |
          EMMC_DLL_STRBIN_DELAY_NUM_SEL |
          EMMC_DLL_STRBIN_DELAY_NUM_DEFAULT << EMMC_DLL_STRBIN_DELAY_NUM_OFFSET
          );
        break;
      }

      /* Switch to eMMC mode */
      MmioOr32 (EMMC_EMMC_CTRL, EMMC_CTRL_CARD_IS_EMMC);

      MmioWrite32 (EMMC_DLL_CTRL, EMMC_DLL_CTRL_SRST);
      gBS->Stall (1);
      MmioWrite32 (EMMC_DLL_CTRL, 0);

      MmioWrite32 (
        EMMC_DLL_CTRL,
        EMMC_DLL_CTRL_START_POINT_DEFAULT |
        EMMC_DLL_CTRL_INCREMENT_DEFAULT | EMMC_DLL_CTRL_START
        );

      for (i = 0; i < 500; i++) {
        Value = MmioRead32 (EMMC_DLL_STATUS0);
        if (Value & EMMC_DLL_STATUS0_DLL_LOCK &&
            !(Value & EMMC_DLL_STATUS0_DLL_TIMEOUT))
        {
          break;
        }

        gBS->Stall (1);
      }

      TxClkTapNum = EMMC_DLL_TXCLK_TAPNUM_DEFAULT;

      if (*Timing == SdMmcMmcHs400) {
        TxClkTapNum = EMMC_DLL_TXCLK_TAPNUM_90_DEGREES;

        MmioWrite32 (
          EMMC_DLL_CMDOUT,
          EMMC_DLL_CMDOUT_SRC_CLK_NEG |
          EMMC_DLL_CMDOUT_EN_SRC_CLK_NEG |
          EMMC_DLL_DLYENA |
          EMMC_DLL_CMDOUT_TAPNUM_90_DEGREES |
          EMMC_DLL_TAPNUM_FROM_SW
          );
      }

      MmioWrite32 (EMMC_DLL_RXCLK, EMMC_DLL_DLYENA);

      MmioWrite32 (
        EMMC_DLL_TXCLK,
        EMMC_DLL_DLYENA |
        TxClkTapNum | EMMC_DLL_TAPNUM_FROM_SW |
        EMMC_DLL_NO_INVERTER
        );

      MmioWrite32 (
        EMMC_DLL_STRBIN,
        EMMC_DLL_DLYENA |
        EMMC_DLL_STRBIN_TAPNUM_DEFAULT | EMMC_DLL_TAPNUM_FROM_SW
        );
      break;

    default:
      break;
  }

  return EFI_SUCCESS;
}

STATIC EDKII_SD_MMC_OVERRIDE  mSdMmcOverride = {
  EDKII_SD_MMC_OVERRIDE_PROTOCOL_VERSION,
  EmmcSdMmcCapability,
  EmmcSdMmcNotifyPhase,
};

EFI_STATUS
EFIAPI
DwcSdhciDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  DEBUG ((DEBUG_BLKIO, "%a\n", __FUNCTION__));

  /* Start card on 375 kHz */
  DwcSdhciSetClockRate (375000UL);

  /* Configure pins */
  DwcSdhciSetIoMux ();

  /* Disable Command Conflict Check */
  MmioWrite32 (EMMC_HOST_CTRL3, 0);

  /* Disable DLL for identification */
  MmioWrite32 (EMMC_DLL_CTRL, 0);
  MmioWrite32 (EMMC_DLL_RXCLK, 0);
  MmioWrite32 (EMMC_DLL_TXCLK, 0);
  MmioWrite32 (EMMC_DLL_STRBIN, 0);

  Status = RegisterNonDiscoverableMmioDevice (
             NonDiscoverableDeviceTypeSdhci,
             NonDiscoverableDeviceDmaTypeNonCoherent,
             NULL,
             &mSdMmcControllerHandle,
             1,
             DWC_SDHCI_BASE,
             0x10000
             );
  ASSERT_EFI_ERROR (Status);

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEdkiiSdMmcOverrideProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID **)&mSdMmcOverride
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
