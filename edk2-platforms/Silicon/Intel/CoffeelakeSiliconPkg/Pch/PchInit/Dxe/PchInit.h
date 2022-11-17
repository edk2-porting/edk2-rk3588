/** @file
  Header file for PCH Initialization Driver.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_INIT_DXE_H_
#define _PCH_INIT_DXE_H_

#include <Protocol/PchEmmcTuning.h>
#include <SiConfigHob.h>
#include <Private/PchConfigHob.h>
#include <Private/Protocol/PchNvsArea.h>

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
  Initialize the PCH device according to the PCH Policy HOB
  and install PCH info instance.

**/
VOID
InitializePchDevice (
  VOID
  );

/**
  Common PchInit Module Entry Point
**/
VOID
PchInitEntryPointCommon (
  VOID
  );

/**
  Common PCH initialization on PCI enumeration complete.
**/
VOID
PchOnPciEnumCompleteCommon (
  VOID
  );

/**
  Configures Serial IO Controllers

**/
EFI_STATUS
ConfigureSerialIoAtBoot (
  VOID
  );

/**
  Creates device handles for SerialIo devices in ACPI mode

**/
VOID
CreateSerialIoHandles (
  VOID
  );

/**
  Mark memory used by SerialIo devices in ACPI mode as allocated

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
AllocateSerialIoMemory (
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
  Update ASL definitions for SerialIo devices.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
UpdateSerialIoAcpiData (
  VOID
  );

/**
  Initialize PCIE SRC clocks in ICC subsystem

  @param[in] GbePortNumber        Number of PCIE rootport assigned to GbE adapter

**/
VOID
ConfigurePchPcieClocks (
  IN UINTN                        GbePortNumber
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
  Configure eMMC in HS400 Mode

  @param[in] This                         A pointer to PCH_EMMC_TUNING_PROTOCOL structure
  @param[in] Revision                     Revision parameter used to verify the layout of EMMC_INFO and TUNINGDATA.
  @param[in] EmmcInfo                     A pointer to EMMC_INFO structure
  @param[out] EmmcTuningData              A pointer to EMMC_TUNING_DATA structure

  @retval EFI_SUCCESS                     The function completed successfully
  @retval EFI_NOT_FOUND                   The item was not found
  @retval EFI_OUT_OF_RESOURCES            The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER           A parameter was incorrect.
  @retval EFI_DEVICE_ERROR                Hardware Error
  @retval EFI_NO_MEDIA                    No media
  @retval EFI_MEDIA_CHANGED               Media Change
  @retval EFI_BAD_BUFFER_SIZE             Buffer size is bad
  @retval EFI_CRC_ERROR                   Command or Data CRC Error
**/
EFI_STATUS
EFIAPI
ConfigureEmmcHs400Mode (
  IN  PCH_EMMC_TUNING_PROTOCOL          *This,
  IN  UINT8                             Revision,
  IN  EMMC_INFO                         *EmmcInfo,
  OUT EMMC_TUNING_DATA                  *EmmcTuningData
  );

/**
  Get eMMC PCI cfg space address

  @return UINT64  PCI base address
**/
UINT64
ScsGetEmmcBaseAddress (
  VOID
  );

/**
  Perform the remaining configuration on PCH SATA to perform device detection,
  then set the SATA SPD and PxE corresponding, and set the Register Lock on PCH SATA

  @retval None
**/
VOID
ConfigurePchSataOnEndOfDxe (
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

/**
  Initialize PCH Nvs Area opeartion region.

**/
VOID
PatchPchNvsAreaAddress (
  VOID
  );

#endif // _PCH_INIT_DXE_H_
