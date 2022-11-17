/** @file
  Initializes Serial IO Controllers.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PchInit.h>
#include <Library/PchSerialIoLib.h>
#include <Library/DevicePathLib.h>

typedef struct {
  ACPI_HID_DEVICE_PATH          RootPort;
  ACPI_EXTENDED_HID_DEVICE_PATH AcpiDev;
  CHAR8                         HidString[8];
  CHAR8                         UidString;
  CHAR8                         CidString;
  EFI_DEVICE_PATH_PROTOCOL      End;
} SERIALIO_DEVICE_PATH;

#define gPciRootBridge {{ACPI_DEVICE_PATH, ACPI_DP, {(UINT8)(sizeof(ACPI_HID_DEVICE_PATH)), 0}}, EISA_PNP_ID (0x0A03), 0}
#define gAcpiDev {{ACPI_DEVICE_PATH,ACPI_EXTENDED_DP,{(UINT8)(sizeof(ACPI_EXTENDED_HID_DEVICE_PATH)+SERIALIO_TOTAL_ID_LENGTH),0}},0,0,0}
#define gEndEntire {END_DEVICE_PATH_TYPE,END_ENTIRE_DEVICE_PATH_SUBTYPE,{END_DEVICE_PATH_LENGTH,0}}

GLOBAL_REMOVE_IF_UNREFERENCED SERIALIO_DEVICE_PATH gSerialIoPath = {
  gPciRootBridge,
  gAcpiDev,
  "\0\0\0\0\0\0\0",
  '\0',
  '\0',
  gEndEntire
};

/**
Mark memory used by SerialIo devices in ACPI mode as allocated

@retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
AllocateSerialIoMemory (
  VOID
  )
{
  PCH_SERIAL_IO_CONTROLLER i;
  UINT8                    BarNumber;
  UINTN                    Bar;
  EFI_STATUS               Status;

  for (i=0; i<PchSerialIoIndexMax; i++) {
    if (mPchConfigHob->SerialIo.DevMode[i] == PchSerialIoAcpiHidden ||
        mPchConfigHob->SerialIo.DevMode[i] == PchSerialIoLegacyUart ||
        mPchConfigHob->SerialIo.DevMode[i] == PchSerialIoAcpi) {
      for (BarNumber = 0; BarNumber<=1; BarNumber++) {
        Bar = FindSerialIoBar (i,BarNumber);
        Status = gDS->AddMemorySpace (
                        EfiGcdMemoryTypeReserved,
                        Bar,
                        V_PCH_SERIAL_IO_BAR_SIZE,
                        0
                        );
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR (Status)) {
          return Status;
        }
        Status = gDS->AllocateMemorySpace (
                        EfiGcdAllocateAddress,
                        EfiGcdMemoryTypeReserved,
                        N_PCH_SERIAL_IO_BAR_ALIGNMENT,
                        V_PCH_SERIAL_IO_BAR_SIZE,
                        &Bar,
                        mImageHandle,
                        NULL
                        );
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR (Status)) {
          return Status;
        }
      }
    }
  }
  return EFI_SUCCESS;
}

VOID
CreateSerialIoHandles (
  VOID
  )
{
  EFI_HANDLE NewHandle;
  EFI_DEVICE_PATH_PROTOCOL *NewPath;
  EFI_STATUS Status;
  UINT32 Controller;

  for (Controller = 0; Controller < PchSerialIoIndexMax; Controller++) {
    if (mPchConfigHob->SerialIo.DevMode[Controller] == PchSerialIoAcpi) {
      NewHandle = NULL;
      CopyMem (gSerialIoPath.HidString, GetSerialIoAcpiHID (Controller), SERIALIO_HID_LENGTH);
      NewPath = DuplicateDevicePath ((EFI_DEVICE_PATH_PROTOCOL*)&gSerialIoPath);
      Status = gBS->InstallMultipleProtocolInterfaces (
                      &NewHandle,
                      &gEfiDevicePathProtocolGuid,
                      NewPath,
                      NULL );
    }
  }
}
