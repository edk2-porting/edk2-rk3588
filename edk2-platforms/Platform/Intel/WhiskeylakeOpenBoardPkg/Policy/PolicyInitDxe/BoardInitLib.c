/** @file
  Source code for the board configuration init function in DXE init phase.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "BoardInitLib.h"
#include <Library/HobLib.h>
#include <MemInfoHob.h>
#include <Library/PchSerialIoLib.h>
#include <PlatformBoardConfig.h>
#include <GpioPinsCnlLp.h>
#include <GpioPinsCnlH.h>
#include <Library/PchInfoLib.h>
#include <Library/PchEspiLib.h>
#include <Library/CpuPlatformLib.h>
#include <TbtBoardInfo.h>
#include <Library/CpuPlatformLib.h>
#include <GopConfigLib.h>
//
// Null function for nothing GOP VBT update.
//
VOID
GopVbtSpecificUpdateNull(
  IN CHILD_STRUCT **ChildStructPtr
);

//
// for CFL U DDR4
//
VOID
CflUDdr4GopVbtSpecificUpdate(
  IN CHILD_STRUCT **ChildStructPtr
);

/**
  Updates DIMM slots status for Desktop,server and workstation boards

**/
VOID
UpdateDimmPopulationConfig(
  VOID
  )
{
  MEMORY_INFO_DATA_HOB    *MemInfo;
  UINT8                   Slot0;
  UINT8                   Slot1;
  UINT8                   Slot2;
  UINT8                   Slot3;
  CONTROLLER_INFO         *ControllerInfo;
  EFI_HOB_GUID_TYPE       *GuidHob;

  GuidHob = NULL;
  MemInfo = NULL;

  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob != NULL) {
    MemInfo = (MEMORY_INFO_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
  }
  if (MemInfo != NULL) {
    if (PcdGet8 (PcdPlatformFlavor) == FlavorDesktop ||
        PcdGet8 (PcdPlatformFlavor) == FlavorUpServer ||
        PcdGet8 (PcdPlatformFlavor) == FlavorWorkstation) {
      ControllerInfo = &MemInfo->Controller[0];
      Slot0 = ControllerInfo->ChannelInfo[0].DimmInfo[0].Status;
      Slot1 = ControllerInfo->ChannelInfo[0].DimmInfo[1].Status;
      Slot2 = ControllerInfo->ChannelInfo[1].DimmInfo[0].Status;
      Slot3 = ControllerInfo->ChannelInfo[1].DimmInfo[1].Status;

      //
      // Channel 0          Channel 1
      // Slot0   Slot1      Slot0   Slot1      - Population            AIO board
      // 0          0          0          0          - Invalid        - Invalid
      // 0          0          0          1          - Valid          - Invalid
      // 0          0          1          0          - Invalid        - Valid
      // 0          0          1          1          - Valid          - Valid
      // 0          1          0          0          - Valid          - Invalid
      // 0          1          0          1          - Valid          - Invalid
      // 0          1          1          0          - Invalid        - Invalid
      // 0          1          1          1          - Valid          - Invalid
      // 1          0          0          0          - Invalid        - Valid
      // 1          0          0          1          - Invalid        - Invalid
      // 1          0          1          0          - Invalid        - Valid
      // 1          0          1          1          - Invalid        - Valid
      // 1          1          0          0          - Valid          - Valid
      // 1          1          0          1          - Valid          - Invalid
      // 1          1          1          0          - Invalid        - Valid
      // 1          1          1          1          - Valid          - Valid
      //

      if ((Slot0 && (Slot1 == 0)) || (Slot2 && (Slot3 == 0))) {
        PcdSetBoolS (PcdDimmPopulationError, TRUE);
      }
    }
  }
}

/**
  Init Misc Platform Board Config Block.

  @param[in]  BoardId           An unsigned integer represent the board id.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
BoardMiscInit (
  IN UINT16 BoardId
  )
{
//  PcdSet64S (PcdFuncBoardHookPlatformSetupOverride, (UINT64) (UINTN) BoardHookPlatformSetup);

  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSetBoolS (PcdPssReadSN, TRUE);
      PcdSet8S (PcdPssI2cSlaveAddress, 0x6E);
      PcdSet8S (PcdPssI2cBusNumber, 0x04);
      break;
    default:
      PcdSetBoolS (PcdPssReadSN, FALSE);
      PcdSet8S (PcdPssI2cSlaveAddress, 0x6E);
      PcdSet8S (PcdPssI2cBusNumber, 0x04);
      break;
  }


  return EFI_SUCCESS;
}

/**
  Init Platform Board Config Block for ACPI platform.

  @param[in]  BoardId           An unsigned integer represent the board id.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
InitAcpiPlatformPcd (
  IN UINT16 BoardId
  )
{
  TBT_INFO_HOB  *TbtInfoHob = NULL;

  TbtInfoHob = (TBT_INFO_HOB *) GetFirstGuidHob (&gTbtInfoHobGuid);

  //
  // Update OEM table ID
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      if ((TbtInfoHob != NULL) && (TbtInfoHob->DTbtControllerConfig[0].DTbtControllerEn == 1)) {
        PcdSet64S (PcdXhciAcpiTableSignature, SIGNATURE_64 ('x', 'h', '_', 'w', 'h', 'l', 't', '4'));
      } else {
        PcdSet64S (PcdXhciAcpiTableSignature, SIGNATURE_64 ('x', 'h', '_', 'w', 'h', 'l', 'd', '4'));
      }
      break;
    default:
      PcdSet64S (PcdXhciAcpiTableSignature, 0);
      break;
  }

  //
  // Modify Preferred_PM_Profile field based on Board SKU's. Default is set to Mobile
  //
  PcdSet8S (PcdPreferredPmProfile, EFI_ACPI_2_0_PM_PROFILE_MOBILE);

  //
  // Assign FingerPrint, Gnss, TouchPanel, Audio related GPIO.
  //
  switch(BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdFingerPrintSleepGpio, GPIO_CNL_LP_GPP_B17);
      PcdSet32S (PcdFingerPrintIrqGpio,   GPIO_CNL_LP_GPP_B16);
      //
      // Configure WWAN Reset pin
      //
      PcdSet32S (PcdGnssResetGpio,      GPIO_CNL_LP_GPP_F1);
      PcdSet32S (PcdTouchpanelIrqGpio,  GPIO_CNL_LP_GPP_D10);
      PcdSet32S (PcdTouchpadIrqGpio,    GPIO_CNL_LP_GPP_B3);
      PcdSet32S (PcdHdaI2sCodecIrqGpio, GPIO_CNL_LP_GPP_C8);
      break;
    default:
      break;
  }

  //
  // Configure GPIOs for discrete USB BT module
  //
  switch(BoardId) {

    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdBtIrqGpio,    GPIO_CNL_LP_GPP_B2);
      PcdSet32S (PcdBtRfKillGpio, GPIO_CNL_LP_GPP_B4);
      break;
    default:
      break;
  }

  //
  // Board Specific Init
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSetBoolS(PcdWhlErbRtd3TableEnable, TRUE);
      PcdSet8S (PcdHdaI2sCodecI2cBusNumber, 0); // I2S Audio Codec conntected to I2C0
      PcdSet8S (PcdBleUsbPortNumber, 9);
      break;
    default:
      break;
  }

  return EFI_SUCCESS;
}

/**
  Init Common Platform Board Config Block.

  @param[in]  BoardId           An unsigned integer represent the board id.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
InitCommonPlatformPcd (
  IN UINT16 BoardId
  )
{
  PCD64_BLOB Data64;

  //
  // Enable EC SMI# for SMI
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdEcSmiGpio, GPIO_CNL_LP_GPP_E3);
      PcdSet32S (PcdEcLowPowerExitGpio, GPIO_CNL_LP_GPP_B23);
      break;
  };

  //
  // HID I2C Interrupt GPIO.
  //
  switch (BoardId) {
    default:
      // on all supported boards interrupt input is on same GPIO pad. How convenient.
      PcdSet32S (PcdHidI2cIntPad, GPIO_CNL_LP_GPP_D10);
      break;
  }

  //
  // PS2 KB Specific Init for Sds Serial platform.
  //
  if (BoardId == BoardIdWhiskeyLakeRvp) {
    PcdSetBoolS (PcdDetectPs2KbOnCmdAck, TRUE);
  } else {
    PcdSetBoolS (PcdDetectPs2KbOnCmdAck,  FALSE);
  }

  switch (BoardId) {
    default:
      PcdSetBoolS (PcdSpdAddressOverride, FALSE);
      break;
  }

  //
  // DDISelection
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSet8S (PcdDDISelection, 1);
      break;
    default:
      PcdSet8S (PcdDDISelection, 0);
      break;
  }

  //
  // GFX Detect
  //
  switch (BoardId) {
    default:
      // Not all the boards support GFX_CRB_DET. This is not an error.
      Data64.BoardGpioConfig.Type = BoardGpioTypeNotSupported;
      break;
  }

  PcdSet64S (PcdGfxCrbDetectGpio, Data64.Blob);

  //
  // USB Type-C
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSetBoolS(PcdUsbTypeCSupport, TRUE);
      // Discete Ports
      PcdSet8S(PcdTypeCPortsSupported, 2);
      // TBT Port 1  mapping and properties [TBT AIC]
      PcdSet8S(PcdUsbTypeCPort1, 1);
      PcdSet8S(PcdUsbTypeCPort1Pch, 5);
      // TBT Port 2  mapping and properties [TBT AIC]
      PcdSet8S(PcdUsbTypeCPort2, 2);
      PcdSet8S(PcdUsbTypeCPort2Pch, 7);
      break;
    default:
      PcdSetBoolS (PcdUsbTypeCSupport, FALSE);
      break;
  }

  //
  // Battery Present
  //
  switch (BoardId) {
  default:
    PcdSet8S (PcdBatteryPresent, BOARD_REAL_BATTERY_SUPPORTED | BOARD_VIRTUAL_BATTERY_SUPPORTED);
    break;
  }

  //
  // TS-on-DIMM temperature
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSetBoolS (PcdTsOnDimmTemperature, TRUE);
      break;
    default:
      PcdSetBoolS (PcdTsOnDimmTemperature, FALSE);
      break;
  }
  //
  // Real Battery 1 Control & Real Battery 2 Control
  //
  PcdSet8S (PcdRealBattery1Control, 1);
  PcdSet8S (PcdRealBattery2Control, 2);

  //
  // Mipi Camera Sensor
  //
  PcdSetBoolS (PcdMipiCamSensor, FALSE);
  //
  // Mipi Camera Sensor Link Used
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSet8S (PcdMipiCam0LinkUsed, 3);
      PcdSet8S (PcdMipiCam1LinkUsed, 6);
      PcdSet8S (PcdMipiCam2LinkUsed, 9);
      PcdSet8S (PcdMipiCam3LinkUsed, 7);
      break;
    default:
      break;
  }

  //
  // H8S2113 SIO
  //
  switch(BoardId) {
    default:
    PcdSetBoolS (PcdH8S2113SIO, FALSE);
    break;
  }


  //
  // NCT6776F COM, SIO & HWMON
  //
  PcdSetBoolS (PcdNCT6776FCOM, FALSE);
  PcdSetBoolS (PcdNCT6776FSIO, FALSE);
  PcdSetBoolS (PcdNCT6776FHWMON, FALSE);

  //
  // SMC Runtime Sci Pin
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdSmcRuntimeSciPin, (UINT32) GPIO_CNL_LP_GPP_E16);
      break;
    default:
      PcdSet32S (PcdSmcRuntimeSciPin, 0x00);
      break;
  }

  //
  // Convertable Dock Support
  //
  switch (BoardId) {
    default:
      PcdSetBoolS (PcdConvertableDockSupport, FALSE);
      break;
  }

  //
  // Ec Hotkey F3, F4, F5, F6, F7 and F8 Support
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSet8S (PcdEcHotKeyF3Support, 1);
      PcdSet8S (PcdEcHotKeyF4Support, 1);
      PcdSet8S (PcdEcHotKeyF5Support, 1);
      PcdSet8S (PcdEcHotKeyF6Support, 1);
      PcdSet8S (PcdEcHotKeyF7Support, 1);
      PcdSet8S (PcdEcHotKeyF8Support, 1);
      break;
    default:
      PcdSet8S (PcdEcHotKeyF3Support, 0);
      PcdSet8S (PcdEcHotKeyF4Support, 0);
      PcdSet8S (PcdEcHotKeyF5Support, 0);
      PcdSet8S (PcdEcHotKeyF6Support, 0);
      PcdSet8S (PcdEcHotKeyF7Support, 0);
      PcdSet8S (PcdEcHotKeyF8Support, 0);
      break;
  }

  //
  // Virtual Button Volume Up & Done Support
  // Virtual Button Home Button Support
  // Virtual Button Rotation Lock Support
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSetBoolS (PcdVirtualButtonVolumeUpSupport, TRUE);
      PcdSetBoolS (PcdVirtualButtonVolumeDownSupport, TRUE);
      PcdSetBoolS (PcdVirtualButtonHomeButtonSupport, FALSE);
      PcdSetBoolS (PcdVirtualButtonRotationLockSupport, FALSE);
      break;
    default:
      PcdSetBoolS (PcdVirtualButtonVolumeUpSupport, FALSE);
      PcdSetBoolS (PcdVirtualButtonVolumeDownSupport, FALSE);
      PcdSetBoolS (PcdVirtualButtonHomeButtonSupport, FALSE);
      PcdSetBoolS (PcdVirtualButtonRotationLockSupport, FALSE);
      break;
  }

  //
  // Slate Mode Switch Support
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSetBoolS (PcdSlateModeSwitchSupport, TRUE);
      break;
    default:
      PcdSetBoolS (PcdSlateModeSwitchSupport, FALSE);
      break;
  }

  //
  // Ac Dc Auto Switch Support
  //
  switch (BoardId) {
  default:
    PcdSetBoolS (PcdAcDcAutoSwitchSupport, TRUE);
    break;
  }

  //
  // Pm Power Button Gpio Pin
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdPmPowerButtonGpioPin, (UINT32) GPIO_CNL_LP_GPD3);
      break;
    default:
      PcdSet32S (PcdPmPowerButtonGpioPin, 0x00);
      break;
  }

  //
  // Acpi Enable All Button Support
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSetBoolS (PcdAcpiEnableAllButtonSupport, TRUE);
      break;
    default:
      PcdSetBoolS (PcdAcpiEnableAllButtonSupport, FALSE);
      break;
  }

  //
  // Acpi Hid Driver Button Support
  //
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      PcdSetBoolS (PcdAcpiHidDriverButtonSupport, TRUE);
      break;
    default:
      PcdSetBoolS (PcdAcpiHidDriverButtonSupport, FALSE);
      break;
  }

  //
  // USB Type C EC less
  //
  switch (BoardId) {
    default:
      PcdSetBoolS (PcdUsbTypeCEcLess, FALSE);
      break;
  }

  return EFI_SUCCESS;
}

/**
  Check if given rootport has device connected and enable wake capability

  @param[in]  RpNum           An unsigned integer represent the root port number.

  @retval                     TRUE if endpoint was connected
  @retval                     FALSE if no endpoint was detected
**/
BOOLEAN
IsPcieEndPointPresent (
  IN UINT8 RpNum
  )
{
  EFI_STATUS    Status;
  UINTN         RpDev;
  UINTN         RpFun;
  UINT64        RpBaseAddress;

  Status = GetPchPcieRpDevFun (RpNum, &RpDev, &RpFun);
  if (!EFI_ERROR (Status)) {
    //
    // check if device is present
    //
    RpBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                      DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      RpDev,
                      RpFun,
                      0
                      );

    if ((PciSegmentRead16 (RpBaseAddress) != 0xFFFF) &&
        (PciSegmentRead16 (RpBaseAddress + R_PCH_PCIE_CFG_SLSTS) & B_PCIE_SLSTS_PDS)) {
      return TRUE;
    }
  }

  return FALSE;

}

/**
  Enable Tier2 GPIO Sci wake capability.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
Tier2GpioWakeSupport (
  IN UINT16 BoardId
  )
{
  BOOLEAN Tier2GpioWakeEnable;

  Tier2GpioWakeEnable = FALSE;
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      //
      // Root port #14: M.2 WLAN
      //
      if (IsPcieEndPointPresent (13)) {
        Tier2GpioWakeEnable = TRUE;
      }
      break;
    default:
      break;
  }
  PcdSetBoolS (PcdGpioTier2WakeEnable, Tier2GpioWakeEnable);

  return EFI_SUCCESS;
}

/**
  Board configuration init function for DXE phase.

  @param  Content  pointer to the buffer contain init information for board init.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
BoardConfigInit (
    VOID
  )
{
  EFI_STATUS Status;
  UINT16     BoardId;

  BoardId = BoardIdWhiskeyLakeRvp;

  Status = InitAcpiPlatformPcd (BoardId);
  ASSERT_EFI_ERROR(Status);

  Status = InitCommonPlatformPcd (BoardId);
  ASSERT_EFI_ERROR(Status);

  Status = BoardMiscInit (BoardId);
  ASSERT_EFI_ERROR(Status);

  Status = Tier2GpioWakeSupport (BoardId);
  ASSERT_EFI_ERROR(Status);

  return EFI_SUCCESS;
}

