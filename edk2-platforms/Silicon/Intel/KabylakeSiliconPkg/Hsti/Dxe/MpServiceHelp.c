/** @file
  This file contains the mpservices helper functions

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiSiliconDxe.h"

typedef struct {
  UINT32  Index;
  UINT64  Value;
} AP_PRPCEDURE_ARGUMENT_READMSR;

typedef struct {
  UINT32  Index;
  UINT32  Eax;
  UINT32  Ebx;
  UINT32  Ecx;
  UINT32  Edx;
} AP_PRPCEDURE_ARGUMENT_CPUID;

EFI_MP_SERVICES_PROTOCOL  *mMpService;
UINTN                     mBspNumber;
UINTN                     mNumberOfProcessors;
UINTN                     mNumberOfEnabledProcessors;

/**
  Initialize MP Helper
**/
VOID
InitMp (
  VOID
  )
{
  EFI_STATUS  Status;

  DEBUG ((EFI_D_INFO, "InitMp\n"));

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **) &mMpService);
  if (EFI_ERROR (Status)) {
    mMpService = NULL;
  }

  if (mMpService != NULL) {
    Status = mMpService->WhoAmI (mMpService, &mBspNumber);
    ASSERT_EFI_ERROR (Status);

    Status = mMpService->GetNumberOfProcessors (mMpService, &mNumberOfProcessors, &mNumberOfEnabledProcessors);
    ASSERT_EFI_ERROR (Status);
  } else {
    mBspNumber = 0;
    mNumberOfProcessors = 1;
    mNumberOfEnabledProcessors = 1;
  }

  DEBUG ((DEBUG_INFO, "BspNumber                 - 0x%x\n", mBspNumber));
  DEBUG ((DEBUG_INFO, "NumberOfProcessors        - 0x%x\n", mNumberOfProcessors));
  DEBUG ((DEBUG_INFO, "NumberOfEnabledProcessors - 0x%x\n", mNumberOfEnabledProcessors));
}

/**
  Concatenate error string.

  @retval UINTN - CpuNumber.
**/
UINTN
GetCpuNumber (
  VOID
  )
{
  return mNumberOfEnabledProcessors;
}

VOID
EFIAPI
ApReadMsr64 (
  IN OUT VOID  *Buffer
  )
{
  AP_PRPCEDURE_ARGUMENT_READMSR  *Argument;

  Argument = Buffer;
  Argument->Value = AsmReadMsr64 (Argument->Index);
}

/**
  Concatenate error string.

  @param[in, out] Buffer     - Pointer to Argument

**/
VOID
EFIAPI
ApCpuId (
  IN OUT VOID  *Buffer
  )
{
  AP_PRPCEDURE_ARGUMENT_CPUID  *Argument;

  Argument = Buffer;
  AsmCpuid (Argument->Index, &Argument->Eax, &Argument->Ebx, &Argument->Ecx, &Argument->Edx);
}

/**
  Concatenate error string.

  @param[in] ProcessorNumber     - Processor ID
  @param[in] Index               - Index

  @retval UINT64 - Msr Value.
**/
UINT64
ProcessorReadMsr64 (
  IN UINTN   ProcessorNumber,
  IN UINT32  Index
  )
{
  EFI_STATUS                      Status;
  AP_PRPCEDURE_ARGUMENT_READMSR   Argument;
  UINT8                           WakeApRetry;

  ASSERT (ProcessorNumber < mNumberOfEnabledProcessors);
  if (ProcessorNumber == mBspNumber) {
    return AsmReadMsr64 (Index);
  }

  ZeroMem (&Argument, sizeof (Argument));
  Argument.Index = Index;
  WakeApRetry = 0;
  do {
    Status = mMpService->StartupThisAP (
                           mMpService,
                           ApReadMsr64,
                           ProcessorNumber,
                           NULL,   // WaitEvent
                           1000,   // 1ms timeout
                           &Argument,
                           NULL    // Finished
                           );
    WakeApRetry++;
  } while ((Status == EFI_TIMEOUT) && (WakeApRetry < 3));
  ASSERT_EFI_ERROR (Status);

  return Argument.Value;
}

/**
  Concatenate error string.

  @param[in]  ProcessorNumber     - Processor ID
  @param[in]  Index               - Index
  @param[out] Eax                 - Eax
  @param[out] Ebx                 - Ebx
  @param[out] Ecx                 - Ecx
  @param[out] Edx                 - Edx
**/
VOID
ProcessorCpuid (
  IN  UINTN   ProcessorNumber,
  IN  UINT32  Index,
  OUT UINT32  *Eax,  OPTIONAL
  OUT UINT32  *Ebx,  OPTIONAL
  OUT UINT32  *Ecx,  OPTIONAL
  OUT UINT32  *Edx   OPTIONAL
  )
{
  EFI_STATUS                      Status;
  AP_PRPCEDURE_ARGUMENT_CPUID     Argument;
  UINT8                           WakeApRetry;

  ASSERT (ProcessorNumber < mNumberOfEnabledProcessors);
  if (ProcessorNumber == mBspNumber) {
    AsmCpuid (Index, Eax, Ebx, Ecx, Edx);
    return;
  }

  ZeroMem (&Argument, sizeof (Argument));
  Argument.Index = Index;
  WakeApRetry = 0;
  do {
    Status = mMpService->StartupThisAP (
                           mMpService,
                           ApCpuId,
                           ProcessorNumber,
                           NULL,   // WaitEvent
                           1000,   // 1ms timeout
                           &Argument,
                           NULL    // Finished
                           );
    WakeApRetry++;
  } while ((Status == EFI_TIMEOUT) && (WakeApRetry < 3));
  ASSERT_EFI_ERROR (Status);

  if (Eax != NULL) {
    *Eax = Argument.Eax;
  }
  if (Ebx != NULL) {
    *Ebx = Argument.Ebx;
  }
  if (Ecx != NULL) {
    *Ecx = Argument.Ecx;
  }
  if (Edx != NULL) {
    *Edx = Argument.Edx;
  }

  return;
}