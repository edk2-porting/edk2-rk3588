/** @file
  Header file for SA SMM Handler

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SaLateInitSmm_H_
#define _SaLateInitSmm_H_

///
/// Driver Consumed Protocol Prototypes
///

#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmIoTrapDispatch2.h>
#include <Protocol/SaPolicy.h>
#include <Library/S3BootScriptLib.h>
#include <CpuPcieConfig.h>
#include <Library/CpuPcieInfoFruLib.h>

extern EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL        *mPchIoTrap;
extern EFI_SMM_SX_DISPATCH2_PROTOCOL             *mSxDispatch;

///
/// The value before AutoConfig match the setting of PCI Express Base Specification 1.1, please be careful for adding new feature
///
typedef enum {
  PcieAspmDisabled,
  PcieAspmL0s,
  PcieAspmL1,
  PcieAspmL0sL1,
  PcieAspmAutoConfig,
  PcieAspmMax
} CPU_PCIE_ASPM_CONFIG;

typedef struct {
  UINT64                    Address;
  S3_BOOT_SCRIPT_LIB_WIDTH  Width;
  UINT32                    Value;
} BOOT_SCRIPT_PCI_REGISTER_SAVE;


/**
  <b>System Agent Initialization SMM Driver Entry Point</b>
  - <b>Introduction</b> \n
    This is an optional driver to support PCIe ASPM initialization later than Option ROM initialization.\n
    In this scenario S3 Save Boot Script table has been closed per security consideration so the ASPM settings will be stored in SMM memory and restored during S3 resume.
    If platform does not support this scenario this driver can be excluded and SI_SA_POLICY_PPI -> PCIE_CONFIG -> InitPcieAspmAfterOprom must be set to FALSE. \n
    Note: When InitPcieAspmAfterOprom enabled, the SMI callback handler must be registered successfully, otherwise it will halt the system.

  - @pre
    - _EFI_SMM_BASE_PROTOCOL (or _EFI_SMM_BASE2_PROTOCOL for EDK2): Provides SMM infrastructure services.
    - _EFI_SMM_IO_TRAP_DISPATCH_PROTOCOL (or _EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL for EDK2): Interface structure for the SMM IO trap specific SMI Dispatch Protocol
    - SA_POLICY_PROTOCOL: A protocol published by a platform DXE module executed earlier; this is documented in this document as well.

  - @result
    PCIe ASPM has been initialized on all end point devices discovered and same settings will be restored during S3 resume.

  @param[in] ImageHandle - The image handle of Wake On Lan driver
  @param[in] SystemTable - The standard EFI system table

  @retval EFI_SUCCESS    - SA SMM handler was installed or not necessary
  @retval EFI_NOT_FOUND  - Fail to register SMI callback or required protocol/hob missing.
**/
EFI_STATUS
EFIAPI
SaLateInitSmmEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

/**
An IoTrap callback to config PCIE power management settings

@param[in] DispatchHandle  - The handle of this callback, obtained when registering
@param[in] DispatchContext - Pointer to the EFI_SMM_IO_TRAP_DISPATCH_CALLBACK_CONTEXT

**/
VOID
EFIAPI
CpuPcieIoTrapSmiCallback(
IN     EFI_HANDLE                     DispatchHandle,
IN     EFI_SMM_IO_TRAP_CONTEXT        *CallbackContext,
IN OUT VOID                           *CommBuffer,
IN OUT UINTN                          *CommBufferSize
);

/**
  This function is used to set or clear flags at S3 entry
  Clear the Io trap executed flag before enter S3

  @param[in] Handle              Handle of the callback
  @param[in] Context             The dispatch context
  @param[in,out] CommBuffer      A pointer to a collection of data in memory that will be conveyed from a non-SMM environment into an SMM environment.
  @param[in,out] CommBufferSize  The size of the CommBuffer.
  @retval EFI_SUCCESS            SA register saved
**/
EFI_STATUS
EFIAPI
CpuPcieS3EntryCallBack (
  IN  EFI_HANDLE                   Handle,
  IN CONST VOID                    *Context OPTIONAL,
  IN OUT VOID                      *CommBuffer OPTIONAL,
  IN OUT UINTN                     *CommBufferSize OPTIONAL
  );

/**
  Register PCIE Hotplug SMI dispatch function to handle Hotplug enabling

  @param[in] ImageHandle          The image handle of this module
  @param[in] SystemTable          The EFI System Table

  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
EFIAPI
InitializeCpuPcieSmm (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  );
#endif
