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
#include <Guid/GlobalVariable.h>
#include <Library/PchGbeLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchHsioLib.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchPcieRpLib.h>
#include <GpioConfig.h>
#include <GpioPinsSklH.h>
#include <Library/DebugLib.h>
#include <Library/PchGbeLib.h>
#include <PcieDeviceOverrideTable.h>

CONST UINT8 mPchSerialIoDevMode[PCH_MAX_SERIALIO_CONTROLLERS] = {
  1 /* I2C0 */,  1 /* I2C1 */,  0 /* I2C2 */, 0 /* I2C3  */, 0 /* I2C4  */, 0 /* I2C5  */,
  0 /* SPI0 */,  0 /* SPI1 */,  0 /* SPI2 */, 1 /* UART0 */, 0 /* UART1 */, 3 /* UART2 */
};

CONST UINT8 mPchLpSerialIoDevMode[PCH_MAX_SERIALIO_CONTROLLERS] = {
  1 /* I2C0 */,  1 /* I2C1 */,  0 /* I2C2 */, 0 /* I2C3  */, 0 /* I2C4  */, 0 /* I2C5  */,
  0 /* SPI0 */,  0 /* SPI1 */,  0 /* SPI2 */, 0 /* UART0 */, 0 /* UART1 */, 0 /* UART2 */
};

/**
  Performs FSP PCH PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyUpdate (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
  UINT32                    Index;
  UINT32                    Length;

  FspsUpd->FspsConfig.PchPcieDeviceOverrideTablePtr = (UINT32) mPcieDeviceTable;

  FspsUpd->FspsConfig.PchPmSlpS3MinAssert       = 0;
  FspsUpd->FspsConfig.PchPmSlpS4MinAssert       = 0;
  FspsUpd->FspsConfig.PchPmSlpSusMinAssert      = 0;
  FspsUpd->FspsConfig.PchPmSlpAMinAssert        = 0;
  FspsUpd->FspsConfig.PchPmLpcClockRun          = 1;
  FspsUpd->FspsConfig.EnableTcoTimer            = 0;
  FspsUpd->FspsConfig.Enable8254ClockGating     = 0;
  FspsUpd->FspsConfig.Enable8254ClockGatingOnS3 = 0;

  FspsUpd->FspsConfig.ScsEmmcEnabled      = 1;
  FspsUpd->FspsConfig.ScsEmmcHs400Enabled = 1;
  FspsUpd->FspsConfig.ScsSdCardEnabled    = 0;
  FspsUpd->FspsConfig.ScsUfsEnabled       = 0;

  FspsUpd->FspsConfig.SataPwrOptEnable    = 1;

  FspsUpd->FspsConfig.GpioIrqRoute        = 14;
  FspsUpd->FspsConfig.SciIrqSelect        = 9;
  FspsUpd->FspsConfig.TcoIrqEnable        = 0;
  FspsUpd->FspsConfig.TcoIrqSelect        = 9;

  AddPlatformVerbTables (
    PchHdaCodecPlatformOnboard,
    &(FspsUpd->FspsConfig.PchHdaVerbTableEntryNum),
    &(FspsUpd->FspsConfig.PchHdaVerbTablePtr)
    );

DEBUG_CODE_BEGIN();
  if (
    (FixedPcdGet8 (PcdSerialIoUartDebugEnable) == 1) &&
    FspsUpd->FspsConfig.SerialIoDevMode[PchSerialIoIndexUart0 + PcdGet8 (PcdSerialIoUartNumber)] == PchSerialIoDisabled
    ) {
    FspsUpd->FspsConfig.SerialIoDevMode[PchSerialIoIndexUart0 + PcdGet8 (PcdSerialIoUartNumber)] = PchSerialIoHidden;
  }
  FspsUpd->FspsConfig.SerialIoDebugUartNumber = PcdGet8 (PcdSerialIoUartNumber);
  FspsUpd->FspsConfig.SerialIoEnableDebugUartAfterPost = TRUE;
  FspsUpd->FspsConfig.SerialIoUartHwFlowCtrl[PcdGet8 (PcdSerialIoUartNumber)] = 0;
DEBUG_CODE_END();

  //
  // SerialIo config
  //
  if (IsPchLp()) {
    CopyMem (&FspsUpd->FspsConfig.SerialIoDevMode, mPchLpSerialIoDevMode, PCH_MAX_SERIALIO_CONTROLLERS);
  } else {
    CopyMem (&FspsUpd->FspsConfig.SerialIoDevMode, mPchSerialIoDevMode, PCH_MAX_SERIALIO_CONTROLLERS);
  }

  // Set debug UART in PCI mode
  FspsUpd->FspsConfig.SerialIoDevMode[PCH_MAX_SERIALIO_I2C_CONTROLLERS + PCH_MAX_SERIALIO_SPI_CONTROLLERS + 2] = 1;

  FspsUpd->FspsConfig.SerialIoSpiCsPolarity[0]              = 1;
  FspsUpd->FspsConfig.SerialIoSpiCsPolarity[1]              = 0;
  FspsUpd->FspsConfig.SerialIoSpiCsPolarity[2]              = 0;
  FspsUpd->FspsConfig.SerialIoUartHwFlowCtrl[0]             = 1;
  FspsUpd->FspsConfig.SerialIoUartHwFlowCtrl[1]             = 1;
  FspsUpd->FspsConfig.SerialIoUartHwFlowCtrl[2]             = 1;
  FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[0]      = 1;
  FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[1]      = 1;
  FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[2]      = 1;
  FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[3]      = 1;
  FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[4]      = 1;
  FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[5]      = 1;

  //
  // USB config
  //
  FspsUpd->FspsConfig.XdciEnable                            = FALSE;
  FspsUpd->FspsConfig.PchEnableComplianceMode               = FALSE;
  FspsUpd->FspsConfig.UsbPdoProgramming                     = TRUE;
  FspsUpd->FspsConfig.PchUsbOverCurrentEnable               = TRUE;
  FspsUpd->FspsConfig.PchUsb2PhySusPgEnable                 = TRUE;
  FspsUpd->FspsTestConfig.PchXhciOcLock                     = TRUE;

  Length = GetPchXhciMaxUsb2PortNum ();
  for (Index = 0; Index < Length; Index++) {
    FspsUpd->FspsConfig.PortUsb20Enable[Index]              = TRUE;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[Index]           = UsbOverCurrentPinMax;
    FspsUpd->FspsConfig.Usb2AfePetxiset[Index]              = 7;
    FspsUpd->FspsConfig.Usb2AfeTxiset[Index]                = 5;
    FspsUpd->FspsConfig.Usb2AfePredeemp[Index]              = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[Index]             = 0;
  }

  Length = GetPchXhciMaxUsb3PortNum ();
  for (Index = 0; Index < Length; Index++) {
    FspsUpd->FspsConfig.PortUsb30Enable[Index]              = TRUE;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[Index]           = UsbOverCurrentPinMax;
    FspsUpd->FspsConfig.Usb3HsioTxDeEmphEnable[Index]       = 0;
    FspsUpd->FspsConfig.Usb3HsioTxDeEmph[Index]             = 0;
    FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmpEnable[Index] = 0;
    FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmp[Index]       = 0;
    FspsUpd->FspsConfig.PchUsbHsioRxTuningEnable[Index]     = 0;
    FspsUpd->FspsConfig.PchUsbHsioRxTuningParameters[Index] = 3;
    FspsUpd->FspsConfig.PchUsbHsioFilterSel[Index]          = 0;
  }

  if (IsPchLp()) {
    FspsUpd->FspsConfig.Usb2OverCurrentPin[0]               = UsbOverCurrentPin2;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[1]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[2]               = UsbOverCurrentPin2;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[3]               = UsbOverCurrentPin2;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[4]               = UsbOverCurrentPin3;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[5]               = UsbOverCurrentPin3;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[6]               = UsbOverCurrentPin3;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[7]               = UsbOverCurrentPin3;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[8]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[9]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[10]              = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[11]              = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[12]              = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[13]              = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[14]              = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[15]              = UsbOverCurrentPinSkip;

    FspsUpd->FspsConfig.Usb3OverCurrentPin[0]               = UsbOverCurrentPin2;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[1]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[2]               = UsbOverCurrentPin2;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[3]               = UsbOverCurrentPin2;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[4]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[5]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[6]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[7]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[8]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[9]               = UsbOverCurrentPinSkip;

    Length = GetPchUsb2MaxPhysicalPortNum ();
    for (Index = 0; Index < Length; Index++) {
      FspsUpd->FspsConfig.Usb2AfePetxiset[Index]              = 6;
      FspsUpd->FspsConfig.Usb2AfeTxiset[Index]                = 0;
      FspsUpd->FspsConfig.Usb2AfePredeemp[Index]              = 3;
      FspsUpd->FspsConfig.Usb2AfePehalfbit[Index]             = 0;
    }
  } else {
    FspsUpd->FspsConfig.Usb2OverCurrentPin[0]               = UsbOverCurrentPin4;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[1]               = UsbOverCurrentPin0;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[2]               = UsbOverCurrentPin2;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[3]               = UsbOverCurrentPin5;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[4]               = UsbOverCurrentPin5;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[5]               = UsbOverCurrentPin0;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[6]               = UsbOverCurrentPin1;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[7]               = UsbOverCurrentPin1;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[8]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[9]               = UsbOverCurrentPin3;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[10]              = UsbOverCurrentPin3;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[11]              = UsbOverCurrentPin6;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[12]              = UsbOverCurrentPin6;
    FspsUpd->FspsConfig.Usb2OverCurrentPin[13]              = UsbOverCurrentPin0;

    FspsUpd->FspsConfig.Usb3OverCurrentPin[0]               = UsbOverCurrentPin4;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[1]               = UsbOverCurrentPin0;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[2]               = UsbOverCurrentPin2;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[3]               = UsbOverCurrentPin5;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[4]               = UsbOverCurrentPin5;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[5]               = UsbOverCurrentPin0;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[6]               = UsbOverCurrentPin1;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[7]               = UsbOverCurrentPin1;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[8]               = UsbOverCurrentPinSkip;
    FspsUpd->FspsConfig.Usb3OverCurrentPin[9]               = UsbOverCurrentPin3;

    FspsUpd->FspsConfig.Usb2AfePetxiset[0]                  = 7;
    FspsUpd->FspsConfig.Usb2AfeTxiset[0]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[0]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[0]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[1]                  = 6;
    FspsUpd->FspsConfig.Usb2AfeTxiset[1]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[1]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[1]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[2]                  = 6;
    FspsUpd->FspsConfig.Usb2AfeTxiset[2]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[2]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[2]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[3]                  = 6;
    FspsUpd->FspsConfig.Usb2AfeTxiset[3]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[3]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[3]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[4]                  = 6;
    FspsUpd->FspsConfig.Usb2AfeTxiset[4]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[4]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[4]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[5]                  = 6;
    FspsUpd->FspsConfig.Usb2AfeTxiset[5]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[5]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[5]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[6]                  = 7;
    FspsUpd->FspsConfig.Usb2AfeTxiset[6]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[6]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[6]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[7]                  = 7;
    FspsUpd->FspsConfig.Usb2AfeTxiset[7]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[7]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[7]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[8]                  = 6;
    FspsUpd->FspsConfig.Usb2AfeTxiset[8]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[8]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[8]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[9]                  = 6;
    FspsUpd->FspsConfig.Usb2AfeTxiset[9]                    = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[9]                  = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[9]                 = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[10]                 = 6;
    FspsUpd->FspsConfig.Usb2AfeTxiset[10]                   = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[10]                 = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[10]                = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[11]                 = 7;
    FspsUpd->FspsConfig.Usb2AfeTxiset[11]                   = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[11]                 = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[11]                = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[12]                 = 7;
    FspsUpd->FspsConfig.Usb2AfeTxiset[12]                   = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[12]                 = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[12]                = 0;

    FspsUpd->FspsConfig.Usb2AfePetxiset[13]                 = 7;
    FspsUpd->FspsConfig.Usb2AfeTxiset[13]                   = 0;
    FspsUpd->FspsConfig.Usb2AfePredeemp[13]                 = 3;
    FspsUpd->FspsConfig.Usb2AfePehalfbit[13]                = 0;
  }

  FspsUpd->FspsConfig.PcieSwEqCoeffListCm[0]                = 4;
  FspsUpd->FspsConfig.PcieSwEqCoeffListCp[0]                = 8;
  FspsUpd->FspsConfig.PcieSwEqCoeffListCm[1]                = 6;
  FspsUpd->FspsConfig.PcieSwEqCoeffListCp[1]                = 2;
  FspsUpd->FspsConfig.PcieSwEqCoeffListCm[2]                = 8;
  FspsUpd->FspsConfig.PcieSwEqCoeffListCp[2]                = 6;
  FspsUpd->FspsConfig.PcieSwEqCoeffListCm[3]                = 10;
  FspsUpd->FspsConfig.PcieSwEqCoeffListCp[3]                = 8;
  FspsUpd->FspsConfig.PcieSwEqCoeffListCm[4]                = 12;
  FspsUpd->FspsConfig.PcieSwEqCoeffListCp[4]                = 2;

  return EFI_SUCCESS;
}
