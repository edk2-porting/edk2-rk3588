/** @file
  File to contain all the hardware specific stuff for the Smm Sw dispatch protocol.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchSmmHelpers.h"

#include <Protocol/SmmCpu.h>
//
// There is only one instance for SwCommBuffer.
// It's safe in SMM since there is no re-entry for the function.
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_SW_CONTEXT            mPchSwCommBuffer;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_CPU_PROTOCOL          *mSmmCpuProtocol;

GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC SW_SOURCE_DESC = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_APMC
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_APM
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_APM
  }
};

/**
  Get the Software Smi value

  @param[in] Record               No use
  @param[out] Context             The context that includes Software Smi value to be filled

**/
VOID
EFIAPI
SwGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  )
{
  UINT8 ApmCnt;

  ApmCnt                      = IoRead8 ((UINTN) R_PCH_APM_CNT);

  Context->Sw.SwSmiInputValue = ApmCnt;
}

/**
  Check whether software SMI value of two contexts match

  @param[in] Context1             Context 1 that includes software SMI value 1
  @param[in] Context2             Context 2 that includes software SMI value 2

  @retval FALSE                   Software SMI value match
  @retval TRUE                    Software SMI value don't match
**/
BOOLEAN
EFIAPI
SwCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  )
{
  return (BOOLEAN) (Context1->Sw.SwSmiInputValue == Context2->Sw.SwSmiInputValue);
}

/**
  Gather the CommBuffer information of SmmSwDispatch2.

  @param[in]  Record              No use
  @param[out] CommBuffer          Point to the CommBuffer structure
  @param[out] CommBufferSize      Point to the Size of CommBuffer structure

**/
VOID
EFIAPI
SwGetCommBuffer (
  IN  DATABASE_RECORD    *Record,
  OUT VOID               **CommBuffer,
  OUT UINTN              *CommBufferSize
  )
{
  EFI_STATUS                            Status;
  EFI_SMM_SAVE_STATE_IO_INFO            SmiIoInfo;
  UINTN                                 Index;

  ASSERT (Record->ProtocolType == SwType);

  mPchSwCommBuffer.CommandPort = IoRead8 (R_PCH_APM_CNT);
  mPchSwCommBuffer.DataPort    = IoRead8 (R_PCH_APM_STS);

  //
  // Try to find which CPU trigger SWSMI
  //
  mPchSwCommBuffer.SwSmiCpuIndex = 0;
  for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
    Status = mSmmCpuProtocol->ReadSaveState (
                                mSmmCpuProtocol,
                                sizeof (EFI_SMM_SAVE_STATE_IO_INFO),
                                EFI_SMM_SAVE_STATE_REGISTER_IO,
                                Index,
                                &SmiIoInfo
                                );
    if (EFI_ERROR (Status)) {
      continue;
    }
    if (SmiIoInfo.IoPort == R_PCH_APM_CNT) {
      //
      // Find matched CPU.
      //
      mPchSwCommBuffer.SwSmiCpuIndex = Index;
      break;
    }
  }

  ///
  /// Return the CommBuffer
  ///
  *CommBuffer = (VOID *) &mPchSwCommBuffer;
  *CommBufferSize  = sizeof (EFI_SMM_SW_CONTEXT);
}

/**
  Init required protocol for Pch Sw Dispatch protocol.


**/
VOID
PchSwDispatchInit (
  VOID
  )
{
  EFI_STATUS                            Status;
  //
  // Locate PI SMM CPU protocol
  //
  Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuProtocolGuid, NULL, (VOID **)&mSmmCpuProtocol);
  ASSERT_EFI_ERROR (Status);
}
