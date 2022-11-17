/** @file
  Header file for PCH Initialization Driver.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_INIT_DXE_H_
#define _PCH_INIT_DXE_H_

#include <SiConfigHob.h>
#include <PchConfigHob.h>
#include <Protocol/PchNvsArea.h>

//
// Data definitions
//
extern EFI_HANDLE               mImageHandle;

//
// Pch NVS area definition
//
extern PCH_NVS_AREA_PROTOCOL    mPchNvsAreaProtocol;

extern PCH_CONFIG_HOB           *mPchConfigHob;
extern SI_CONFIG_HOB_DATA       *mSiConfigHobData;

//
// Function Prototype
//

//
// Local function prototypes
//
/**
  Common PchInit Module Entry Point
**/
VOID
PchInitEntryPointCommon (
  VOID
  );

/**
  Calls Boot Os Hand off routine for each Serial IO Controller
**/
VOID
ConfigureSerialIoAtBoot (
  VOID
  );

/**
  Puts all SerialIo controllers (except UARTs in debug mode) in D3.
  Clears MemoryEnable for all PCI-mode controllers on S3 resume
**/
VOID
ConfigureSerialIoAtS3Resume (
  VOID
  );

/**
  Add Serial Io UART Hidden Handles
**/
VOID
CreateSerialIoUartHiddenHandle (
  VOID
  );

/**
  Initialize Intel High Definition Audio ACPI Tables

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_LOAD_ERROR          ACPI table cannot be installed
  @retval EFI_UNSUPPORTED         ACPI table not set because DSP is disabled
**/
EFI_STATUS
PchHdAudioAcpiInit (
  VOID
  );

/**
  Perform the remaining configuration on SATA to perform device detection,
  then set the SATA SPD and PxE corresponding, and set the Register Lock on PCH SATA
**/
VOID
ConfigureSataOnEndOfDxe (
  VOID
  );

/**
  Update ASL data for CNVI Device.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
UpdateCnviAcpiData (
   VOID
   );

/**
  Initialize Pch acpi
  @param[in] ImageHandle          Handle for the image of this driver

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchAcpiInit (
  IN EFI_HANDLE         ImageHandle
  );

/**
  Update ASL object before Boot

  @retval EFI_STATUS
  @retval EFI_NOT_READY         The Acpi protocols are not ready.
**/
EFI_STATUS
PchUpdateNvsArea (
  VOID
  );

#endif // _PCH_INIT_DXE_H_
