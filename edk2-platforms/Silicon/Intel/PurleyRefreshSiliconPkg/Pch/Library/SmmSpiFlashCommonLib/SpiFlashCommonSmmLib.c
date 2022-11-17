/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/SpiFlashCommonLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/Spi.h>

extern EFI_SPI_PROTOCOL   *mSpiProtocol;

extern UINTN mFlashAreaBaseAddress;
extern UINTN mFlashAreaSize;

/**
  The library constructuor.

  The function does the necessary initialization work for this library
  instance.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       The function always return EFI_SUCCESS for now.
                                It will ASSERT on error for debug version.
  @retval     EFI_ERROR         Please reference LocateProtocol for error code details.
**/
EFI_STATUS
EFIAPI
SmmSpiFlashCommonLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;

  mFlashAreaBaseAddress = (UINTN)PcdGet32 (PcdFlashAreaBaseAddress);
  mFlashAreaSize        = (UINTN)PcdGet32 (PcdFlashAreaSize);

  //
  // Locate the SMM SPI protocol.
  //
  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSpiProtocolGuid,
                    NULL,
                    (VOID **) &mSpiProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
