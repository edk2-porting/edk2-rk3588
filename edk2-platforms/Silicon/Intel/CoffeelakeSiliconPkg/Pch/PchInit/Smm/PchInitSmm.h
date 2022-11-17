/** @file
  Header file for PCH Init SMM Handler

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

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
#include <Library/TimerLib.h>

#include <IndustryStandard/Pci30.h>
#include <Library/PmcLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/PchEspiLib.h>
#include <Library/PciSegmentLib.h>
#include <Private/Library/PchPciExpressHelpersLib.h>
#include <Protocol/PchPcieSmiDispatch.h>
#include <Protocol/PchTcoSmiDispatch.h>
#include <Protocol/PchSmiDispatch.h>
#include <Protocol/PchEspiSmiDispatch.h>
#include <Protocol/PchSmmIoTrapControl.h>
#include <Private/Protocol/PchNvsArea.h>
#include <Private/Protocol/PcieIoTrap.h>
#include <SiConfigHob.h>
#include <Private/PchConfigHob.h>
#include <Private/Library/PmcPrivateLib.h>

extern EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL        *mPchIoTrap;
extern EFI_SMM_SX_DISPATCH2_PROTOCOL             *mSxDispatch;

extern PCH_NVS_AREA                              *mPchNvsArea;
extern UINT16                                    mAcpiBaseAddr;

extern EFI_PHYSICAL_ADDRESS                      mResvMmioBaseAddr;
extern UINTN                                     mResvMmioSize;

//
// NOTE: The module variables of policy here are only valid in post time, but not runtime time.
//
extern PCH_CONFIG_HOB                            *mPchConfigHob;
extern SI_CONFIG_HOB_DATA                        *mSiConfigHobData;

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
