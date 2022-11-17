 /** @file
  SynQuacer DXE platform driver - eMMC support

  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PlatformDxe.h"

// F_SDH30 extended Controller registers
#define F_SDH30_AHB_CONFIG        0x100
#define  F_SDH30_AHB_BIGED        BIT6
#define  F_SDH30_BUSLOCK_DMA      BIT5
#define  F_SDH30_BUSLOCK_EN       BIT4
#define  F_SDH30_SIN              BIT3
#define  F_SDH30_AHB_INCR_16      BIT2
#define  F_SDH30_AHB_INCR_8       BIT1
#define  F_SDH30_AHB_INCR_4       BIT0

#define F_SDH30_TUNING_SETTING    0x108
#define  F_SDH30_CMD_CHK_DIS      BIT16

#define F_SDH30_IO_CONTROL2       0x114
#define  F_SDH30_MSEL_O_1_8       BIT18
#define  F_SDH30_CRES_O_DN        BIT19

#define F_SDH30_ESD_CONTROL       0x124
#define  F_SDH30_EMMC_RST         BIT1
#define  F_SDH30_EMMC_HS200       BIT24
#define  F_SDH30_CMD_DAT_DELAY    BIT9

#define F_SDH30_TUNING_SETTING    0x108
#define  F_SDH30_CMD_CHK_DIS      BIT16

#define F_SDH30_IO_CONTROL2       0x114
#define  F_SDH30_MSEL_O_1_8       BIT18
#define  F_SDH30_CRES_O_DN        BIT19

#define F_SDH30_ESD_CONTROL       0x124
#define  F_SDH30_EMMC_RST         BIT1
#define  F_SDH30_EMMC_HS200       BIT24
#define  F_SDH30_CMD_DAT_DELAY    BIT9

#define SD_HC_CLOCK_CTRL          0x2C
#define SYNQUACER_CLOCK_CTRL_VAL  0xBC01

#define SD_HC_CAP_SDR104          BIT33
#define SD_HC_CAP_DDR50           BIT34

#define ESD_CONTROL_RESET_DELAY   (20 * 1000)
#define IO_CONTROL2_SETTLE_US     3000

STATIC EFI_HANDLE mSdMmcControllerHandle;

/**

  Override function for SDHCI capability bits

  @param[in]      PassThru              A pointer to the
                                        EFI_SD_MMC_PASS_THRU_PROTOCOL instance.
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
SynQuacerSdMmcCapability (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN OUT  VOID                            *SdMmcHcSlotCapability,
  IN OUT  UINT32                          *BaseClkFreq
  )
{
  UINT64 Capability;

  if (ControllerHandle != mSdMmcControllerHandle) {
    return EFI_SUCCESS;
  }

  ASSERT (Slot == 0);

  //
  // Clear the SDR104 capability bit. This avoids the need for a HS200 tuning
  // quirk that is difficult to support using the generic driver.
  // Clear the DDR50 bit as well to work around an issue with the Kingston
  // EMMC08G-M325-A52 part that was fitted on 96board DeveloperBox samples.
  //
  Capability = ReadUnaligned64 (SdMmcHcSlotCapability);
  Capability &= ~(UINT64)(SD_HC_CAP_SDR104 | SD_HC_CAP_DDR50);
  WriteUnaligned64 (SdMmcHcSlotCapability, Capability);

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
SynQuacerSdMmcNotifyPhase (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN      EDKII_SD_MMC_PHASE_TYPE         PhaseType,
  IN OUT  VOID                           *PhaseData
  )
{
  if (ControllerHandle != mSdMmcControllerHandle) {
    return EFI_SUCCESS;
  }

  ASSERT (Slot == 0);

  switch (PhaseType) {
  case EdkiiSdMmcResetPre:
    // Soft reset does not complete unless the clock is already enabled.
    MmioWrite16 (SYNQUACER_EMMC_BASE + SD_HC_CLOCK_CTRL,
      SYNQUACER_CLOCK_CTRL_VAL);
    break;

  case EdkiiSdMmcInitHostPre:
    // init vendor specific regs
    MmioAnd16 (SYNQUACER_EMMC_BASE + F_SDH30_AHB_CONFIG,
      ~(F_SDH30_AHB_BIGED | F_SDH30_BUSLOCK_EN));

    MmioOr16 (SYNQUACER_EMMC_BASE + F_SDH30_AHB_CONFIG,
      F_SDH30_SIN | F_SDH30_AHB_INCR_16 | F_SDH30_AHB_INCR_8 |
      F_SDH30_AHB_INCR_4);

    MmioAnd32 (SYNQUACER_EMMC_BASE + F_SDH30_ESD_CONTROL, ~F_SDH30_EMMC_RST);
    MemoryFence ();
    gBS->Stall (ESD_CONTROL_RESET_DELAY);

    MmioOr32 (SYNQUACER_EMMC_BASE + F_SDH30_ESD_CONTROL,
      F_SDH30_EMMC_RST | F_SDH30_CMD_DAT_DELAY | F_SDH30_EMMC_HS200);

    gBS->Stall (IO_CONTROL2_SETTLE_US);
    MmioOr32 (SYNQUACER_EMMC_BASE + F_SDH30_IO_CONTROL2, F_SDH30_CRES_O_DN);
    MemoryFence ();
    MmioOr32 (SYNQUACER_EMMC_BASE + F_SDH30_IO_CONTROL2, F_SDH30_MSEL_O_1_8);
    MemoryFence ();
    MmioAnd32 (SYNQUACER_EMMC_BASE + F_SDH30_IO_CONTROL2, ~F_SDH30_CRES_O_DN);
    MemoryFence ();
    gBS->Stall (IO_CONTROL2_SETTLE_US);

    MmioOr32 (SYNQUACER_EMMC_BASE + F_SDH30_TUNING_SETTING,
      F_SDH30_CMD_CHK_DIS);
    break;

  default:
    break;
  }
  return EFI_SUCCESS;
}

STATIC EDKII_SD_MMC_OVERRIDE mSdMmcOverride = {
  EDKII_SD_MMC_OVERRIDE_PROTOCOL_VERSION,
  SynQuacerSdMmcCapability,
  SynQuacerSdMmcNotifyPhase,
};

EFI_STATUS
EFIAPI
RegisterEmmc (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_HANDLE                      Handle;

  Status = RegisterNonDiscoverableMmioDevice (
             NonDiscoverableDeviceTypeSdhci,
             NonDiscoverableDeviceDmaTypeCoherent,
             NULL,
             &mSdMmcControllerHandle,
             1,
             SYNQUACER_EMMC_BASE, SYNQUACER_EMMC_BASE_SZ);
  ASSERT_EFI_ERROR (Status);

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (&Handle,
                  &gEdkiiSdMmcOverrideProtocolGuid,
                  EFI_NATIVE_INTERFACE, (VOID **)&mSdMmcOverride);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
