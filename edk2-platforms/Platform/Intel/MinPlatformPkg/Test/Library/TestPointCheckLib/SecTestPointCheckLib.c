/** @file

Copyright (c) 2017 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/TestPointCheckLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#define TEST_PATTERN  0x5A

VOID
TestPointDumpCpuInfo (
  VOID
  );

EFI_STATUS
TestTempRam (
  IN VOID   *TempRamTestStart,
  IN VOID   *TempRamTestEnd
  )
{
  UINTN    Index;
  BOOLEAN  Result;

  Result = TRUE;

  DEBUG ((DEBUG_INFO, "TempRamTestStart - 0x%08x\n", TempRamTestStart));
  DEBUG ((DEBUG_INFO, "TempRamTestEnd   - 0x%08x\n", TempRamTestEnd));

  SetMem (TempRamTestStart, (UINTN)TempRamTestEnd - (UINTN)TempRamTestStart, TEST_PATTERN);
  for (Index = 0; Index < (UINTN)TempRamTestEnd - (UINTN)TempRamTestStart; Index++) {
    if (*((UINT8 *)TempRamTestStart + Index) != TEST_PATTERN) {
      Result = FALSE;
    }
  }
  if (Result) {
    DEBUG ((DEBUG_INFO, "TempRam Test PASS\n"));
    return EFI_SUCCESS;
  } else {
    DEBUG ((DEBUG_INFO, "TempRam Test FAIL\n"));
    return EFI_DEVICE_ERROR;
  }
}

/**
  This service tests temporary memory functionality.

  Test subject: Temporary Memory
  Test overview: Reads/writes results on the stack and heap.
  Reporting mechanism: PPI

  @param[in]  TempRamStart    A pointer to the start of temporary memory.
  @param[in]  TempRamEnd      A pointer to the end of temporary memory.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointTempMemoryFunction (
  IN VOID   *TempRamStart,
  IN VOID   *TempRamEnd
  )
{
  UINTN  StackMark;
  DEBUG ((DEBUG_INFO, "======== TestPointTempMemoryFunction - Enter\n"));

  DEBUG ((DEBUG_INFO, "TempRamStart - 0x%08x\n", TempRamStart));
  DEBUG ((DEBUG_INFO, "TempRamEnd   - 0x%08x\n", TempRamEnd));

  TestPointDumpCpuInfo ();

  StackMark = 0;
  TestTempRam (TempRamStart, (VOID *)((UINTN)&StackMark & ~0xFFF));

  DEBUG ((DEBUG_INFO, "======== TestPointTempMemoryFunction - Exit\n"));

  return EFI_SUCCESS;
}
