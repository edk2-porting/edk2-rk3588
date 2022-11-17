/** @file
  This SMM driver will handle SA relevant late initialization

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/BaseLib.h>
#include <Base.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/SaPlatformLib.h>
#include <Protocol/SmmIoTrapDispatch2.h>
#include "SaLateInitSmm.h"
#include <Library/PciSegmentLib.h>
#include <CpuDataStruct.h>
#include <CpuPcieHob.h>
#include <Protocol/SaIotrapSmi.h>
#include <PchPcieRpConfig.h>
#include <SaConfigHob.h>
#include "CpuPcieInfo.h"
#include <Register/CpuPcieRegs.h>
#include <IndustryStandard/Pci30.h>
#include <Register/IgdRegs.h>
#include <Register/CommonMsr.h>

typedef enum {
  EnumSaSmiCallbackForMaxPayLoad,
  EnumSaSmiCallbackForSaSaveRestore,
  EnumSaSmiCallbackForLateInit,
  EnumSaSmiCallbackForS3resume,
  EnumSaSmiCallbackMax
} SMI_OPERATION;

GLOBAL_REMOVE_IF_UNREFERENCED UINT8                           mSaSmiCallbackPhase      = EnumSaSmiCallbackForMaxPayLoad;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL        *mPchIoTrap;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_SX_DISPATCH2_PROTOCOL             *mSxDispatch;


typedef struct {
  UINT64  BaseAddr;
  UINT32  Offset;
  UINT32  AndMask;
  UINT32  OrMask;
} BOOT_SCRIPT_REGISTER_SETTING;

/**
  Initializes the SA SMM handler

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
  )
{
#if FixedPcdGetBool(PcdCpuPcieEnable) == 1
  CPU_PCIE_HOB                              *CpuPcieHob = NULL;
  EFI_STATUS                                Status;
#endif

  DEBUG ((DEBUG_INFO, "SaLateInitSmmEntryPoint()\n"));

#if FixedPcdGetBool(PcdCpuPcieEnable) == 1
  CpuPcieHob = (CPU_PCIE_HOB *) GetFirstGuidHob (&gCpuPcieHobGuid);
  Status = EFI_NOT_FOUND;
  if (CpuPcieHob == NULL) {
    DEBUG ((DEBUG_INFO, "CPU PCIE HOB Not found\n"));
    ASSERT (CpuPcieHob != NULL);
    return Status;
  }

  ///
  /// Locate the PCH Trap dispatch protocol
  ///
  Status = gSmst->SmmLocateProtocol (&gEfiSmmIoTrapDispatch2ProtocolGuid, NULL, (VOID **) &mPchIoTrap);
  ASSERT_EFI_ERROR (Status);
  Status = gSmst->SmmLocateProtocol (&gEfiSmmSxDispatch2ProtocolGuid, NULL, (VOID**) &mSxDispatch);
  ASSERT_EFI_ERROR (Status);
  if (Status == EFI_SUCCESS) {
    ///
    /// If ASPM policy is set to "Before OPROM", this SMI callback is not necessary
    /// Ensure the SMI callback handler will directly return and continue the POST.
    ///
    mSaSmiCallbackPhase = EnumSaSmiCallbackMax;
    Status = EFI_SUCCESS;
  }

  Status = InitializeCpuPcieSmm (ImageHandle, SystemTable);
  ASSERT_EFI_ERROR (Status);

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Failed to register SaIotrapSmiCallback!\n"));
    ///
    /// System will halt when failing to register required SMI handler
    ///
    CpuDeadLoop ();
  }
#endif
  return EFI_SUCCESS;
}
