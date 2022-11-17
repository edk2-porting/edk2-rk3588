/** @file
  SMM Library instance of SPI Flash Common Library Class

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/SmmServicesTableLib.h>
#include <Protocol/Spi.h>
#include <Library/DebugLib.h>

extern PCH_SPI_PROTOCOL   *mSpiProtocol;

extern UINTN mBiosAreaBaseAddress;
extern UINTN mBiosSize;
extern UINTN mBiosOffset;

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
  UINT32     BaseAddr;
  UINT32     RegionSize;

  mBiosAreaBaseAddress = (UINTN)PcdGet32 (PcdBiosAreaBaseAddress);
  mBiosSize            = (UINTN)PcdGet32 (PcdBiosSize);

  //
  // Locate the SMM SPI protocol.
  //
  Status = gSmst->SmmLocateProtocol (
                    &gPchSmmSpiProtocolGuid,
                    NULL,
                    (VOID **) &mSpiProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  mSpiProtocol->GetRegionAddress (mSpiProtocol, FlashRegionBios, &BaseAddr, &RegionSize);
  mBiosOffset = BaseAddr;
  return Status;
}
