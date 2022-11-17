/** @file
  PCH Serial IO Lib implementation.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Pci30.h>
#include <PchAccess.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/MmPciLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchPsfLib.h>
#include <Include/PcieRegs.h>

typedef struct {
  UINT32 Bar0;
  UINT32 Bar1;
} SERIAL_IO_CONTROLLER_DESCRIPTOR;

typedef struct {
  UINT8  DevNum;
  UINT8  FuncNum;
} SERIAL_IO_BDF_NUMBERS;

typedef struct {
  UINT16 PciDevIdPchLp;
  UINT16 PciDevIdSklPchH;
  UINT16 PciDevIdKblPchH;
  CHAR8  AcpiHid[SERIALIO_HID_LENGTH];
} SERIAL_IO_ID;

GLOBAL_REMOVE_IF_UNREFERENCED SERIAL_IO_ID mSerialIoId [PCH_SERIALIO_MAX_CONTROLLERS] =
{
  {V_PCH_LP_SERIAL_IO_I2C0_DEVICE_ID,  V_SKL_PCH_H_SERIAL_IO_I2C0_DEVICE_ID,  V_KBL_PCH_H_SERIAL_IO_I2C0_DEVICE_ID,  "INT3442"},
  {V_PCH_LP_SERIAL_IO_I2C1_DEVICE_ID,  V_SKL_PCH_H_SERIAL_IO_I2C1_DEVICE_ID,  V_KBL_PCH_H_SERIAL_IO_I2C1_DEVICE_ID,  "INT3443"},
  {V_PCH_LP_SERIAL_IO_I2C2_DEVICE_ID,  V_SKL_PCH_H_SERIAL_IO_I2C2_DEVICE_ID,  V_KBL_PCH_H_SERIAL_IO_I2C2_DEVICE_ID,  "INT3444"},
  {V_PCH_LP_SERIAL_IO_I2C3_DEVICE_ID,  V_SKL_PCH_H_SERIAL_IO_I2C3_DEVICE_ID,  V_KBL_PCH_H_SERIAL_IO_I2C3_DEVICE_ID,  "INT3445"},
  {V_PCH_LP_SERIAL_IO_I2C4_DEVICE_ID,  0                                   ,  0                                   ,  "INT3446"},
  {V_PCH_LP_SERIAL_IO_I2C5_DEVICE_ID,  0                                   ,  0                                   ,  "INT3447"},
  {V_PCH_LP_SERIAL_IO_SPI0_DEVICE_ID,  V_SKL_PCH_H_SERIAL_IO_SPI0_DEVICE_ID,  V_KBL_PCH_H_SERIAL_IO_SPI0_DEVICE_ID,  "INT3440"},
  {V_PCH_LP_SERIAL_IO_SPI1_DEVICE_ID,  V_SKL_PCH_H_SERIAL_IO_SPI1_DEVICE_ID,  V_KBL_PCH_H_SERIAL_IO_SPI1_DEVICE_ID,  "INT3441"},
  {V_PCH_LP_SERIAL_IO_UART0_DEVICE_ID, V_SKL_PCH_H_SERIAL_IO_UART0_DEVICE_ID, V_KBL_PCH_H_SERIAL_IO_UART0_DEVICE_ID, "INT3448"},
  {V_PCH_LP_SERIAL_IO_UART1_DEVICE_ID, V_SKL_PCH_H_SERIAL_IO_UART1_DEVICE_ID, V_KBL_PCH_H_SERIAL_IO_UART1_DEVICE_ID, "INT3449"},
  {V_PCH_LP_SERIAL_IO_UART2_DEVICE_ID, V_SKL_PCH_H_SERIAL_IO_UART2_DEVICE_ID, V_KBL_PCH_H_SERIAL_IO_UART2_DEVICE_ID, "INT344A"}
};

GLOBAL_REMOVE_IF_UNREFERENCED SERIAL_IO_BDF_NUMBERS mSerialIoBdf [PCH_SERIALIO_MAX_CONTROLLERS] =
{
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C0,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C0},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C1,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C1},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C2,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C2},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C3,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C3},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C4,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C4},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C5,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C5},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI0,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI0},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI1,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI1},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART0, PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART0},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART1, PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART1},
  {PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2, PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2}
};

GLOBAL_REMOVE_IF_UNREFERENCED SERIAL_IO_CONTROLLER_DESCRIPTOR mSerialIoAcpiAddress [PCH_SERIALIO_MAX_CONTROLLERS] =
{
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x0000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x1000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x2000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x3000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x4000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x5000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x6000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x7000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x8000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x9000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0xA000,  PCH_SERIAL_IO_BASE_ADDRESS + 0xB000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0xC000,  PCH_SERIAL_IO_BASE_ADDRESS + 0xD000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0xE000,  PCH_SERIAL_IO_BASE_ADDRESS + 0xF000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x10000, PCH_SERIAL_IO_BASE_ADDRESS + 0x11000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x12000, PCH_SERIAL_IO_BASE_ADDRESS + 0x13000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x14000, PCH_SERIAL_IO_BASE_ADDRESS + 0x15000}
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchLpSerialIoPciCfgCtrAddr [PCH_SERIALIO_MAX_CONTROLLERS] =
{
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL1,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL2,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL3,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL4,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL5,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL6,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL13,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL14,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL9,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL10,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL11
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchHSerialIoPciCfgCtrAddr[PCH_SERIALIO_MAX_CONTROLLERS] =
{
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL1,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL2,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL3,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL4,
  0,
  0,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL13,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL14,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL9,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL10,
  R_PCH_PCR_SERIAL_IO_PCICFGCTRL11
};


/**
  Returns index of the last i2c controller

  @param[in] Number  Number of SerialIo controller

  @retval            Index of I2C controller
**/
PCH_SERIAL_IO_CONTROLLER
GetMaxI2cNumber (
  VOID
  )
{
  if (GetPchSeries () == PchH) {
    return PchSerialIoIndexI2C3;
  } else {
    return PchSerialIoIndexI2C5;
  }
}

/**
  Returns string with AcpiHid assigned to selected SerialIo controller

  @param[in] Number  Number of SerialIo controller

  @retval            pointer to 8-byte string
**/
CHAR8*
GetSerialIoAcpiHID (
  IN PCH_SERIAL_IO_CONTROLLER Number
  )
{
  return mSerialIoId[Number].AcpiHid;
}

/**
  Checks if Device with given PciDeviceId is one of SerialIo controllers
  If yes, its number is returned through Number parameter, otherwise Number is not updated

  @param[in]  PciDevId  Device ID
  @param[out] Number    Number of SerialIo controller

  @retval TRUE          Yes it is a SerialIo controller
  @retval FALSE         No it isn't a SerialIo controller
**/
BOOLEAN
IsSerialIoPciDevId (
  IN  UINT16                    PciDevId,
  OUT PCH_SERIAL_IO_CONTROLLER  *Number
  )
{
  PCH_SERIAL_IO_CONTROLLER Controller;
  PCH_SERIES               PchSeries;
  PCH_GENERATION           PchGen;

  PchSeries =  GetPchSeries ();
  PchGen =  GetPchGeneration ();
  for (Controller = 0; Controller < PCH_SERIALIO_MAX_CONTROLLERS; Controller++) {
    if (((PchSeries == PchLp) && (PciDevId == mSerialIoId[Controller].PciDevIdPchLp)) ||
        ((PchSeries == PchH) && (PchGen == SklPch) && (PciDevId == mSerialIoId[Controller].PciDevIdSklPchH)) ||
        ((PchSeries == PchH) && (PchGen == KblPch) && (PciDevId == mSerialIoId[Controller].PciDevIdKblPchH))) {
      *Number = Controller;
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Checks if Device with given AcpiHid string is one of SerialIo controllers
  If yes, its number is returned through Number parameter, otherwise Number is not updated

  @param[in]  AcpiHid   String
  @param[out] Number    Number of SerialIo controller

  @retval TRUE          yes it is a SerialIo controller
  @retval FALSE         no it isn't a SerialIo controller
**/
BOOLEAN
IsSerialIoAcpiHid (
  IN CHAR8                      *AcpiHid,
  OUT PCH_SERIAL_IO_CONTROLLER  *Number
  )
{
  PCH_SERIAL_IO_CONTROLLER Controller;
  for (Controller = 0; Controller < PCH_SERIALIO_MAX_CONTROLLERS; Controller++) {
    if (!AsciiStrCmp ((const CHAR8 *) AcpiHid, mSerialIoId[Controller].AcpiHid)) {
      *Number = Controller;
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Finds PCI Device Number of SerialIo devices.

  @param[in] SerialIoNumber             0=I2C0, ..., 11=UART2

  @retval                               SerialIo device number
**/
UINT8
GetSerialIoDeviceNumber (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoNumber
  )
{
  return mSerialIoBdf[SerialIoNumber].DevNum;
}

/**
  Finds PCI Function Number of SerialIo devices.

  @param[in] SerialIoNumber             0=I2C0, ..., 11=UART2

  @retval                               SerialIo funciton number
**/
UINT8
GetSerialIoFunctionNumber (
  IN PCH_SERIAL_IO_CONTROLLER  SerialIoNumber
  )
{
  return mSerialIoBdf[SerialIoNumber].FuncNum;
}

/**
  Finds BAR values of SerialIo devices.
  SerialIo devices can be configured to not appear on PCI so traditional method of reading BAR might not work.
  If the SerialIo device is in PCI mode, a request for BAR1 will return its PCI CFG space instead

  @param[in] SerialIoDevice             0=I2C0, ..., 11=UART2
  @param[in] BarNumber                  0=BAR0, 1=BAR1

  @retval                               SerialIo Bar value
**/
UINTN
FindSerialIoBar (
  IN PCH_SERIAL_IO_CONTROLLER           SerialIoDevice,
  IN UINT8                              BarNumber
  )
{
  UINT64  Bar;
  UINTN   PcieBase;
  UINT32  VenId;

  PcieBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, GetSerialIoDeviceNumber (SerialIoDevice), GetSerialIoFunctionNumber (SerialIoDevice));
  VenId = MmioRead32 (PcieBase + PCI_VENDOR_ID_OFFSET) & 0xFFFF;
  if (VenId == V_PCH_INTEL_VENDOR_ID) {
    if (BarNumber == 1) {
      return PcieBase;
    }
    Bar = MmioRead32 (PcieBase + PCI_BASE_ADDRESSREG_OFFSET);
    //
    // For 64-Bit Memory Space BARs ((BAR[x] & 0xFFFFFFF0) + ((BAR[x+1] & 0xFFFFFFFF) << 32)
    //
    if ((Bar & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
      Bar = (Bar & 0xFFFFF000) + LShiftU64 (MmioRead32 (PcieBase + PCI_BASE_ADDRESSREG_OFFSET + 4) & 0xFFFFFFFF, 32);
      return (UINTN) Bar;
    }
    return (UINTN) (Bar & 0xFFFFF000);
  }
  //
  //PCI mode failed? Try hardcoded addresses from ACPI
  //
  if (BarNumber == 0) {
    Bar = mSerialIoAcpiAddress[SerialIoDevice].Bar0;
  } else {
    Bar = mSerialIoAcpiAddress[SerialIoDevice].Bar1;
  }
  return (UINTN) Bar;
}

/**
  Configures Serial IO Controller

  @param[in] Controller          Serial Io controller selector
  @param[in] DeviceMode          Device mode selector
**/
VOID
ConfigureSerialIoController (
  IN PCH_SERIAL_IO_CONTROLLER Controller,
  IN PCH_SERIAL_IO_MODE       DeviceMode
  )
{
  UINTN     PciCfgBase;
  UINT32    Data32And;
  UINT32    Data32Or;
  UINT16    *SerialIoPciCfgCtrAddr;

  if (Controller >= PchSerialIoIndexMax) {
    return;
  }

  if (DeviceMode == PchSerialIoSkipInit) {
    return;
  }

  PciCfgBase = MmPciBase (0, GetSerialIoDeviceNumber (Controller), GetSerialIoFunctionNumber (Controller));
  //
  // Do not modify a device that has already been disabled/hidden
  //
  if (MmioRead16 (PciCfgBase + PCI_VENDOR_ID_OFFSET) != V_PCH_INTEL_VENDOR_ID) {
    return;
  }

  ///
  /// Step 0. set Bit 16,17,18.
  ///
  MmioOr32 (PciCfgBase + R_PCH_SERIAL_IO_D0I3MAXDEVPG, BIT18 | BIT17 | BIT16);

  //
  // special case for I2C4 and I2C5 on SPT-H
  //
  if (GetPchSeries () == PchH && (Controller  == PchSerialIoIndexI2C4 || Controller  == PchSerialIoIndexI2C5)) {
    return;
  }

  if (GetPchSeries () == PchLp) {
    SerialIoPciCfgCtrAddr = mPchLpSerialIoPciCfgCtrAddr;
  } else {
    SerialIoPciCfgCtrAddr = mPchHSerialIoPciCfgCtrAddr;
  }

  switch (DeviceMode) {
    case PchSerialIoDisabled:
      ///
      /// Step 1. Put device in D3
      /// Step 2. Function Disable in PSF
      ///
      MmioOr32 (PciCfgBase + R_PCH_SERIAL_IO_PME_CTRL_STS, BIT1 | BIT0);
      PsfDisableSerialIoDevice (Controller);
      break;

    case PchSerialIoAcpi:
    case PchSerialIoAcpiHidden:
    case PchSerialIoLegacyUart:
      ///
      /// reenable BAR1 in case it was disabled earlier
      ///
      PchPcrAndThenOr32 (PID_SERIALIO, SerialIoPciCfgCtrAddr[Controller],(UINT32) ~(B_PCH_PCR_SERIAL_IO_PCICFGCTRL_BAR1_DIS),0x0);
      PsfEnableSerialIoDeviceBar1 (Controller);
      ///
      /// Step 1. Assign BAR0
      /// Step 2. Assign BAR1
      ///
      MmioWrite32 (PciCfgBase + R_PCH_SERIAL_IO_BAR0_LOW,  mSerialIoAcpiAddress[Controller].Bar0);
      MmioWrite32 (PciCfgBase + R_PCH_SERIAL_IO_BAR0_HIGH, 0x0);
      MmioWrite32 (PciCfgBase + R_PCH_SERIAL_IO_BAR1_LOW,  mSerialIoAcpiAddress[Controller].Bar1);
      MmioWrite32 (PciCfgBase + R_PCH_SERIAL_IO_BAR1_HIGH, 0x0);
      ///
      /// Step 3. Set Memory space Enable
      ///
      MmioOr32 (PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
      ///
      /// Step 4. Disable device's PciCfg and enable ACPI interrupts
      ///
      PchPcrAndThenOr32 (PID_SERIALIO, SerialIoPciCfgCtrAddr[Controller], 0xFFFFFFFF, (B_PCH_PCR_SERIAL_IO_PCICFGCTRL_PCI_CFG_DIS | B_PCH_PCR_SERIAL_IO_PCICFGCTRL_ACPI_INTR_EN));
      ///
      /// Step 5. Disable device's PciCfg in PSF
      ///
      PsfHideSerialIoDevice (Controller);
      ///
      /// get controller out of reset
      ///
      MmioOr32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_PPR_RESETS,
        B_PCH_SERIAL_IO_PPR_RESETS_FUNC | B_PCH_SERIAL_IO_PPR_RESETS_APB | B_PCH_SERIAL_IO_PPR_RESETS_IDMA);
      break;
    case PchSerialIoPci:
      ///
      /// reenable PciCfg in case it was disabled earlier
      ///
      PchPcrAndThenOr32 (PID_SERIALIO, SerialIoPciCfgCtrAddr[Controller], (UINT32) ~(B_PCH_PCR_SERIAL_IO_PCICFGCTRL_PCI_CFG_DIS | B_PCH_PCR_SERIAL_IO_PCICFGCTRL_ACPI_INTR_EN),0x0);
      PsfRevealSerialIoDevice (Controller);
      ///
      /// Disable Bar1
      /// Disable Bar1 in PSF
      ///
      PchPcrAndThenOr32 (PID_SERIALIO, SerialIoPciCfgCtrAddr[Controller], 0xFFFFFFFF, B_PCH_PCR_SERIAL_IO_PCICFGCTRL_BAR1_DIS);
      PsfDisableSerialIoDeviceBar1 (Controller);
      //
      // Assign BAR0 and Set Memory space Enable
      //
      MmioWrite32 (PciCfgBase + R_PCH_SERIAL_IO_BAR0_LOW,  mSerialIoAcpiAddress[Controller].Bar0);
      MmioWrite32 (PciCfgBase + R_PCH_SERIAL_IO_BAR0_HIGH, 0x0);
      MmioOr32    (PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
      ///
      /// get controller out of reset
      ///
      MmioOr32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_PPR_RESETS,
        B_PCH_SERIAL_IO_PPR_RESETS_FUNC | B_PCH_SERIAL_IO_PPR_RESETS_APB | B_PCH_SERIAL_IO_PPR_RESETS_IDMA);
      break;
    default:
      return;
  }

  ///
  /// Step X. Program clock dividers for UARTs
  /// Step Y. Enable Byte addressing for UARTs in legacy mode
  ///
  if (Controller >= PchSerialIoIndexUart0 && Controller <= PchSerialIoIndexUart2) {
    MmioWrite32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_PPR_CLK,
      (B_PCH_SERIAL_IO_PPR_CLK_UPDATE | (V_PCH_SERIAL_IO_PPR_CLK_N_DIV << 16) |
       (V_PCH_SERIAL_IO_PPR_CLK_M_DIV << 1) | B_PCH_SERIAL_IO_PPR_CLK_EN )
      );

    Data32And = (UINT32) (~(B_PCH_PCR_SERIAL_IO_GPPRVRW7_UART0_BYTE_ADDR_EN << (Controller - PchSerialIoIndexUart0)));
    Data32Or = 0x0;
    if (DeviceMode == PchSerialIoLegacyUart) {
      Data32Or = (B_PCH_PCR_SERIAL_IO_GPPRVRW7_UART0_BYTE_ADDR_EN << (Controller - PchSerialIoIndexUart0));
    }
    PchPcrAndThenOr32 (PID_SERIALIO, R_PCH_PCR_SERIAL_IO_GPPRVRW7,Data32And,Data32Or);
    //
    // Dummy read after setting any of GPPRVRW7.
    // Required for UART 16550 8-bit Legacy mode to become active
    //
    MmioRead32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_PPR_CLK);
  }
  ///
  /// Step Z. Program I2C SDA hold registers
  ///
  if (Controller >= PchSerialIoIndexI2C0 && Controller <= GetMaxI2cNumber ()) {
    if (DeviceMode != PchSerialIoDisabled) {
      MmioOr32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_I2C_SDA_HOLD, V_PCH_SERIAL_IO_I2C_SDA_HOLD_VALUE);
    }
  }

}

/**
  Initializes GPIO pins used by SerialIo I2C devices

  @param[in] Controller          Serial Io controller selector
  @param[in] DeviceMode          Device mode selector
  @param[in] I2cVoltage          Voltage selector
**/
VOID
SerialIoI2cGpioInit (
  IN PCH_SERIAL_IO_CONTROLLER Controller,
  IN PCH_SERIAL_IO_MODE       DeviceMode,
  IN UINT32                   I2cVoltage
  )
{
  if ((DeviceMode == PchSerialIoDisabled) || (DeviceMode == PchSerialIoSkipInit)) {
    return;
  }
  GpioSetSerialIoI2cPinsIntoNativeMode (Controller);
  if (I2cVoltage == PchSerialIoIs18V) {
    GpioSetSerialIoI2CPinsTolerance (Controller, TRUE);
  }
}

/**
  Initializes GPIO pins used by SerialIo SPI devices

  @param[in] Controller          Serial Io controller selector
  @param[in] DeviceMode          Device mode selector
  @param[in] SpiCsPolarity       Polarity selector
**/
VOID
SerialIoSpiGpioInit (
  IN PCH_SERIAL_IO_CONTROLLER Controller,
  IN PCH_SERIAL_IO_MODE       DeviceMode,
  IN UINT32                   SpiCsPolarity
  )
{
  if ((DeviceMode == PchSerialIoDisabled) || (DeviceMode == PchSerialIoSkipInit)) {
    return;
  }

  //
  // set Invert Frame Signal before enabling pins to ensure correct initial ChipSelect polarity
  //
  if (SpiCsPolarity == PchSerialIoCsActiveLow) {
    MmioAnd32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_SSCR1, (UINT32) ~(B_PCH_SERIAL_IO_SSCR1_IFS));
    MmioOr32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_SPI_CS_CONTROL, B_PCH_SERIAL_IO_SPI_CS_CONTROL_STATE);
  } else {
    MmioOr32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_SSCR1, B_PCH_SERIAL_IO_SSCR1_IFS);
    MmioAnd32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_SPI_CS_CONTROL, (UINT32) ~B_PCH_SERIAL_IO_SPI_CS_CONTROL_STATE);
  }

  MmioOr32 (mSerialIoAcpiAddress[Controller].Bar0 + R_PCH_SERIAL_IO_SPI_CS_CONTROL, B_PCH_SERIAL_IO_SPI_CS_CONTROL_MODE);
  GpioSetSerialIoSpiPinsIntoNativeMode (Controller-PchSerialIoIndexSpi0);
}

/**
  Initializes GPIO pins used by SerialIo devices

  @param[in] Controller          Serial Io controller selector
  @param[in] DeviceMode          Device mode selector
  @param[in] HardwareFlowControl Hardware flow control selector
**/
VOID
SerialIoUartGpioInit (
  IN PCH_SERIAL_IO_CONTROLLER Controller,
  IN PCH_SERIAL_IO_MODE       DeviceMode,
  IN BOOLEAN                  HardwareFlowControl
  )
{

  if ((DeviceMode == PchSerialIoDisabled) || (DeviceMode == PchSerialIoSkipInit)) {
    return;
  }
  GpioSetSerialIoUartPinsIntoNativeMode (Controller-PchSerialIoIndexUart0, HardwareFlowControl);
}


