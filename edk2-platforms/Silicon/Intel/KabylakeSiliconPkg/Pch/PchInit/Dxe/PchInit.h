/** @file
  Header file for PCH Initialization Driver.

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_INITIALIZATION_DRIVER_H_
#define _PCH_INITIALIZATION_DRIVER_H_

typedef UINT16                    STRING_REF;
#ifdef FSP_FLAG
#include <Library/PeiServicesLib.h>
#include <Uefi/UefiSpec.h>
#endif
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Protocol/DriverSupportedEfiVersion.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchP2sbLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchEspiLib.h>
#include <Library/PchInfoLib.h>
#include <Library/TraceHubInitLib.h>
#include <Guid/EventGroup.h>
#include <Library/S3BootScriptLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciPlatform.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#ifndef FSP_FLAG
#include <Library/DxeServicesTableLib.h>
#endif

#include <PchAccess.h>
#include <SiConfigHob.h>
#include <Protocol/PchInfo.h>
#include <IndustryStandard/Pci30.h>
#include <Library/AslUpdateLib.h>
#include <Library/MmPciLib.h>
#include <Library/CpuPlatformLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/PchEmmcTuning.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Protocol/PchNvs.h>
#include <Protocol/PcieIoTrap.h>
#include <Library/PchInitCommonLib.h>
#include <Library/PchPciExpressHelpersLib.h>
#include <Library/PchPsfLib.h>
#include <Library/PchPsfPrivateLib.h>
#include <PchConfigHob.h>

typedef struct {
  PCH_INFO_PROTOCOL PchInfo;
} PCH_INSTANCE_PRIVATE_DATA;

//
// This struct is used to record the fields that is required to be saved and restored during RST PCIe Storage Remapping Configuration
//
typedef struct {
  UINT8     PmCapPtr;
  UINT8     PcieCapPtr;
  UINT16    L1ssCapPtr;
  UINT8     EndpointL1ssControl2;
  UINT32    EndpointL1ssControl1;
  UINT16    LtrCapPtr;
  UINT32    EndpointLtrData;
  UINT16    EndpointLctlData16;
  UINT16    EndpointDctlData16;
  UINT16    EndpointDctl2Data16;
  UINT16    RootPortDctl2Data16;
} PCH_RST_PCIE_STORAGE_SAVE_RESTORE;

//
// This struct is used to record the result of RST PCIe Storage detection for each RST PCIe Storage Cycle Router supported on the platform
//
typedef struct {
  BOOLEAN                               SupportRstPcieStoragRemapping;    // Indicates if RST PCIe Storage Remapping is supported and PCIe storage device is found under a Cycle Router
  UINT8                                 RootPortNum;                      // Indicates the root port number with RST PCIe Storage Remapping remapping supported and PCIe storage device plugged on, numbering is 0-based
  UINT8                                 RootPortLane;                     // Indicates the root port lanes occupied by the PCIe storage device with 4-bit mask
  UINT8                                 DeviceInterface;                  // Indicates the interface of the PCIe storage device (AHCI or NVMe)
  UINT8                                 IsMsixSupported;                  // Indicates if the PCIe storage device support MSI-X cap
  UINT16                                MsixStartingVector;               // Records the starting vector of PCIe storage device's MSI-X (if supported)
  UINT16                                MsixEndingVector;                 // Records the ending vector of PCIe storage device's MSI-X (if supported)
  UINT32                                EndPointBarSize;                  // Records the PCIe storage device's BAR size
  UINT32                                EndPointUniqueMsixTableBar;       // Records the PCIe storage device's MSI-X Table BAR if it supports unique MSI-X Table BAR
  UINT32                                EndPointUniqueMsixTableBarValue;  // Records the PCIe storage device's MSI-X Table BAR value if it supports unique MSI-X Table BAR
  UINT32                                EndPointUniqueMsixPbaBar;         // Records the PCIe storage device's MSI-X PBA BAR if it supports unique MSI-X PBA BAR
  UINT32                                EndPointUniqueMsixPbaBarValue;    // Records the PCIe storage device's MSI-X PBA BAR value if it supports unique MSI-X PBA BAR
  UINT8                                 EndPointBcc;                      // Records the PCIe storage device's Base Class Code
  UINT8                                 EndPointScc;                      // Records the PCIe storage device's Sub Class Code
  UINT8                                 EndPointPi;                       // Records the PCIe storage device's Programming Interface
  PCH_RST_PCIE_STORAGE_SAVE_RESTORE     PchRstPcieStorageSaveRestore;     // Records the fields that is required to be saved and restored
} PCH_RST_PCIE_STORAGE_DETECTION;

//
// Data definitions
//
extern EFI_HANDLE               mImageHandle;

//
// Pch NVS area definition
//
extern PCH_NVS_AREA_PROTOCOL    mPchNvsAreaProtocol;

extern PCH_CONFIG_HOB           *mPchConfigHob;
extern SI_CONFIG_HOB            *mSiConfigHob;

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
  Install PCH EMMC TUNING PROTOCOL

**/
VOID
InstallPchEmmcTuningProtocol (
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
  Update ASL data for CIO2 Device.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
UpdateCio2AcpiData (
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

/**
  Initialize PCH Nvs Area opeartion region.

  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
PatchPchNvsAreaAddress (
  VOID
  );

/**
  PCH Update NvsArea ExitBootServicesFlag on ExitBootService. This event is used if only ExitBootService is used
  and not in legacy boot

  @retval None
**/
VOID
EFIAPI
PchUpdateNvsOnExitBootServices (
  VOID
  );
#endif // _PCH_INITIALIZATION_DRIVER_H_
