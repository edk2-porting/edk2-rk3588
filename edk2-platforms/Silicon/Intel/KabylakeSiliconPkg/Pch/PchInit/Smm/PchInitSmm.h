/** @file
  Header file for PCH Init SMM Handler

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_INIT_SMM_H_
#define _PCH_INIT_SMM_H_


#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmIoTrapDispatch2.h>
#include <Library/S3BootScriptLib.h>
#include <Library/HobLib.h>
#include <Protocol/SmmCpu.h>

#include <IndustryStandard/Pci30.h>
#include <PchAccess.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/PchEspiLib.h>
#include <Library/MmPciLib.h>
#include <Library/PchPciExpressHelpersLib.h>
#include <Protocol/PchPcieSmiDispatch.h>
#include <Protocol/PchTcoSmiDispatch.h>
#include <Protocol/PchSmiDispatch.h>
#include <Protocol/PchEspiSmiDispatch.h>
#include <Protocol/PchSmmIoTrapControl.h>
#include <Protocol/PchNvs.h>
#include <Protocol/PcieIoTrap.h>
#include <SiConfigHob.h>
#include <PchConfigHob.h>

extern EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL        *mPchIoTrap;
extern EFI_SMM_SX_DISPATCH2_PROTOCOL             *mSxDispatch;

extern PCH_NVS_AREA                              *mPchNvsArea;
extern UINT16                                    mAcpiBaseAddr;

extern EFI_PHYSICAL_ADDRESS                      mResvMmioBaseAddr;
extern EFI_PHYSICAL_ADDRESS                      mXhciMmioBaseAddr;
extern UINTN                                     mResvMmioSize;

//
// NOTE: The module variables of policy here are only valid in post time, but not runtime time.
//
extern PCH_CONFIG_HOB                            *mPchConfigHob;
extern SI_CONFIG_HOB                             *mSiConfigHob;

#define EFI_PCI_CAPABILITY_ID_PCIPM 0x01

#define DeviceD0 0x00
#define DeviceD3 0x03

typedef enum {
  PciCfg,
  PciMmr
} PCH_PCI_ACCESS_TYPE;

typedef enum {
  Acpi,
  Rcrb,
  Tco
} PCH_ACCESS_TYPE;

typedef struct {
  PCH_ACCESS_TYPE AccessType;
  UINT32          Address;
  UINT32          Data;
  UINT32          Mask;
  UINT8           Width;
} PCH_SAVE_RESTORE_REG;

typedef struct {
  PCH_SAVE_RESTORE_REG* PchSaveRestoreReg;
  UINT8                 size;
  PCH_SERIES            PchSeries;
} PCH_SAVE_RESTORE_REG_WRAP;

struct _PCH_SAVE_RESTORE_PCI;

typedef struct _PCH_SAVE_RESTORE_PCI{
  PCH_PCI_ACCESS_TYPE AccessType;
  UINT8            Device;
  UINT8            Function;
  UINT8            BarOffset;
  UINT16           Offset;
  UINT32           Data;
  UINT32           Mask;
  UINT8            Width;
  VOID            (*RestoreFunction)(struct _PCH_SAVE_RESTORE_PCI *PchSaveRestorePci);
} PCH_SAVE_RESTORE_PCI;

typedef struct {
  PCH_SAVE_RESTORE_PCI* PchSaveRestorePci;
  UINT8                 size;
  PCH_SERIES            PchSeries;
} PCH_SAVE_RESTORE_PCI_WRAP;

typedef struct {
  UINT8           Device;
  UINT8           Function;
  UINT8           PowerState;
} DEVICE_POWER_STATE;

VOID
RestorePxDevSlp(
  IN PCH_SAVE_RESTORE_PCI   *PchSaveRestorePci
  );

/**
  Register PCIE Hotplug SMI dispatch function to handle Hotplug enabling

  @param[in] ImageHandle          The image handle of this module
  @param[in] SystemTable          The EFI System Table

  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
EFIAPI
InitializePchPcieSmm (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  );

/**
  Program Common Clock and ASPM of Downstream Devices

  @param[in] PortIndex                  Pcie Root Port Number
  @param[in] RpDevice                   Pcie Root Pci Device Number
  @param[in] RpFunction                 Pcie Root Pci Function Number

  @retval EFI_SUCCESS                   Root port complete successfully
  @retval EFI_UNSUPPORTED               PMC has invalid vendor ID
**/
EFI_STATUS
PchPcieSmi (
  IN  UINT8                             PortIndex,
  IN  UINT8                             RpDevice,
  IN  UINT8                             RpFunction
  );

/**
  PCIE Hotplug SMI call back function for each Root port

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
PchPcieSmiRpHandlerFunction (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  );

/**
  PCIE Link Active State Change Hotplug SMI call back function for all Root ports

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
PchPcieLinkActiveStateChange (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  );

/**
  PCIE Link Equalization Request SMI call back function for all Root ports

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
PchPcieLinkEqHandlerFunction (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  );

/**
  An IoTrap callback to config PCIE power management settings

  @param[in] DispatchHandle  - The handle of this callback, obtained when registering
  @param[in] DispatchContext - Pointer to the EFI_SMM_IO_TRAP_DISPATCH_CALLBACK_CONTEXT

**/
VOID
EFIAPI
PchPcieIoTrapSmiCallback (
  IN     EFI_HANDLE                     DispatchHandle,
  IN     EFI_SMM_IO_TRAP_CONTEXT        *CallbackContext,
  IN OUT VOID                           *CommBuffer,
  IN OUT UINTN                          *CommBufferSize
  );

/**
  Initializes the PCH SMM handler for PCH save and restore

  @param[in] ImageHandle - Handle for the image of this driver
  @param[in] SystemTable - Pointer to the EFI System Table

  @retval EFI_SUCCESS    - PCH SMM handler was installed
**/
EFI_STATUS
EFIAPI
PchInitLateSmm (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  );

/**
  Locate required protocol and register the 61h IO trap

  @param[in] ImageHandle - Handle for the image of this driver
  @param[in] SystemTable - Pointer to the EFI System Table

  @retval EFI_SUCCESS    - PCH SMM handler was installed
**/
EFI_STATUS
EFIAPI
InstallIoTrapPort61h (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

/**
  Initialize PCH Sx entry SMI handler.

  @param[in] ImageHandle - Handle for the image of this driver
**/
VOID
InitializeSxHandler (
  IN EFI_HANDLE        ImageHandle
  );

/**
  PCH Sx entry SMI handler.

  @param[in]     Handle          Handle of the callback
  @param[in]     Context         The dispatch context
  @param[in,out] CommBuffer      A pointer to a collection of data in memory that will
                                 be conveyed from a non-SMM environment into an SMM environment.
  @param[in,out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS
**/

EFI_STATUS
EFIAPI
PchSxHandler (
  IN  EFI_HANDLE                   Handle,
  IN CONST VOID                    *Context OPTIONAL,
  IN OUT VOID                      *CommBuffer OPTIONAL,
  IN OUT UINTN                     *CommBufferSize OPTIONAL
  );

/**
  xHCI S3 entry handler
**/
VOID
PchXhciS3Callback (
  VOID
  );

/**
  GbE Sx entry handler
**/
VOID
PchLanSxCallback (
  VOID
  );

/**
  This function performs GPIO Sx Isolation for DevSlp pins.
**/
VOID
PchGpioSxIsolationCallback (
  VOID
  );

/**
  Register dispatch function to handle GPIO pads Sx isolation
**/
VOID
InitializeGpioSxIsolationSmm (
  VOID
  );

/**
  Entry point for Pch Bios Write Protect driver.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
**/
EFI_STATUS
EFIAPI
InstallPchBiosWriteProtect (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  );

/**
  This fuction install SPI ASYNC SMI handler.

  @retval EFI_SUCCESS             Initialization complete.
**/
EFI_STATUS
EFIAPI
InstallPchSpiAsyncSmiHandler (
  VOID
  );





#endif
