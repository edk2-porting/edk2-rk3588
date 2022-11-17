/** @file
  Initializes Serial IO Controllers.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <Library/PchInfoLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/SerialIoRegs.h>
#include <PchInit.h>

typedef struct {
  ACPI_HID_DEVICE_PATH          RootPort;
  ACPI_EXTENDED_HID_DEVICE_PATH AcpiDev;
  CHAR8                         HidString[8];
  UINT16                        DeviceId;
  UINT8                         UartIndex;
  EFI_DEVICE_PATH_PROTOCOL      End;
} SERIALIO_UART_DEVICE_PATH;

#define mPciRootBridge {{ACPI_DEVICE_PATH, ACPI_DP, {(UINT8)(sizeof (ACPI_HID_DEVICE_PATH)), 0}}, EISA_PNP_ID (0x0A03), 0}
#define mAcpiDev {{ACPI_DEVICE_PATH, ACPI_EXTENDED_DP, {(UINT8)(sizeof (ACPI_EXTENDED_HID_DEVICE_PATH) + (sizeof(CHAR8) * 8) + sizeof (UINT16) + sizeof (UINT8)), 0}},0,0,0}
#define mEndEntire {END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, {END_DEVICE_PATH_LENGTH, 0}}

GLOBAL_REMOVE_IF_UNREFERENCED SERIALIO_UART_DEVICE_PATH mSerialIoUartPath = {
  mPciRootBridge,
  mAcpiDev,
  "UART\0\0\0",
  0xFFFF,
  0xFF,
  mEndEntire
};

/**
  Add Serial Io UART Hidden Handles
**/
VOID
CreateSerialIoUartHiddenHandle (
  VOID
  )
{
  EFI_HANDLE               NewHandle;
  EFI_DEVICE_PATH_PROTOCOL *NewPath;
  EFI_STATUS               Status;
  UINT8                    Index;
  UINT16                   DeviceId;

  DEBUG ((DEBUG_INFO, "CreateSerialIoUartHandle\n"));

  for (Index = 0; Index < GetPchMaxSerialIoUartControllersNum (); Index++) {
    DEBUG ((DEBUG_INFO, "UART Index: %d Mode: %d\n", Index, mPchConfigHob->SerialIo.UartDeviceConfig[Index].Mode));
    if (mPchConfigHob->SerialIo.UartDeviceConfig[Index].Mode == SerialIoUartHidden) {
      NewHandle = NULL;
      DeviceId = MmioRead16 (GetSerialIoUartFixedPciCfgAddress (Index) + PCI_DEVICE_ID_OFFSET);
      DEBUG ((DEBUG_INFO, "Creating Handle for UART DEVICE_ID: 0x%X \n", DeviceId));
      mSerialIoUartPath.AcpiDev.HID  = 0x5432 + (Index << 16); //UAR
      mSerialIoUartPath.HidString[4] = (CHAR8)('0' + Index);
      mSerialIoUartPath.DeviceId     = DeviceId;
      mSerialIoUartPath.UartIndex    = Index;
      NewPath = DuplicateDevicePath ((EFI_DEVICE_PATH_PROTOCOL*) &mSerialIoUartPath);
      Status = gBS->InstallMultipleProtocolInterfaces (
                      &NewHandle,
                      &gEfiDevicePathProtocolGuid,
                      NewPath,
                      NULL
                      );
      DEBUG ((DEBUG_INFO, "CreateSerialIoUartHandle Status: %r\n", Status));
    }
  }
}

/**
  Stores Pme Control Status and Command register values in S3 Boot Script

  @param[in] S3PciCfgBase      S3 Boot Script Pci Config Base
  @param[in] Command           Pci Command register data to save
  @param[in] Pme               Pci Pme Control register data to save

**/
VOID
STATIC
SerialIoS3Save (
  IN UINTN   S3PciCfgBase,
  IN UINTN   Command,
  IN UINTN   Pme
  )
{
  if (S3PciCfgBase != 0) {
    S3BootScriptSavePciCfgWrite (S3BootScriptWidthUint32, S3PciCfgBase + R_SERIAL_IO_CFG_PME_CTRL_STS, 1, &Pme);
    S3BootScriptSavePciCfgWrite (S3BootScriptWidthUint32, S3PciCfgBase + PCI_COMMAND_OFFSET, 1, &Command);
  }
}

/**
  Puts all SerialIo controllers (except UARTs in debug mode) in D3.
  Clears MemoryEnable for all PCI-mode controllers on S3 resume
**/
VOID
ConfigureSerialIoAtS3Resume (
  VOID
  )
{
  UINT8     Index;
  UINTN     S3PciCfgBase;
  UINT32    Command;
  UINT32    Pme;

  S3PciCfgBase = 0;
  for (Index = 0; Index < GetPchMaxSerialIoSpiControllersNum (); Index++) {
    SerialIoSpiS3Handler (Index, &mPchConfigHob->SerialIo.SpiDeviceConfig[Index], &S3PciCfgBase, &Command, &Pme);
    SerialIoS3Save (S3PciCfgBase, Command, Pme);
  }

  S3PciCfgBase = 0;
  for (Index = 0; Index < GetPchMaxSerialIoI2cControllersNum (); Index++) {
    SerialIoI2cS3Handler (Index, &mPchConfigHob->SerialIo.I2cDeviceConfig[Index], &S3PciCfgBase, &Command, &Pme);
    SerialIoS3Save (S3PciCfgBase, Command, Pme);
  }

  S3PciCfgBase = 0;
  for (Index = 0; Index < GetPchMaxSerialIoUartControllersNum (); Index++) {
    SerialIoUartS3Handler (Index, &mPchConfigHob->SerialIo.UartDeviceConfig[Index], &S3PciCfgBase, &Command, &Pme);
    SerialIoS3Save (S3PciCfgBase, Command, Pme);
  }
}

