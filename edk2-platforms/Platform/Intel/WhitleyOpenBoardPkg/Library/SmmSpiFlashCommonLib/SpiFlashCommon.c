/** @file
  Wrap EFI_SPI_PROTOCOL to provide some library level interfaces
  for module use.

  @copyright
  Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/SpiFlashCommonLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <PchAccess.h>
#include <Protocol/Spi.h>
#include <Protocol/DynamicSiLibrarySmmProtocol.h>
#include <Library/SmmServicesTableLib.h>

PCH_SPI_PROTOCOL       *mSpiProtocol;

//
// FlashAreaBaseAddress and Size for boottime and runtime usage.
//
UINTN mFlashAreaBaseAddress = 0;
UINTN mFlashAreaSize        = 0;

/**
  This function enable SPI.MSE(Memory Space Enable) Bit1 if it is disabled.
  in some cases, the SPI.MSE will be disabled. For example, it is disabled under PCI bus initialization.

  @param None.

  @retval None.

**/
VOID
SpiMemorySpaceCheckandEnable(
  VOID
  )
{
  UINTN                             SpiMemAddress;
  UINT32                            SpiCmdData;
  EFI_STATUS                        Status = EFI_SUCCESS;
  DYNAMIC_SI_LIBARY_SMM_PROTOCOL    *DynamicSiLibrarySmmProtocol = NULL;

  Status = gSmst->SmmLocateProtocol (&gDynamicSiLibrarySmmProtocolGuid, NULL, &DynamicSiLibrarySmmProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  SpiMemAddress = DynamicSiLibrarySmmProtocol->MmPciBase ( DEFAULT_PCI_BUS_NUMBER_PCH,
                                PCI_DEVICE_NUMBER_PCH_SPI,
                                PCI_FUNCTION_NUMBER_PCH_SPI);

  SpiCmdData = MmioRead32 (SpiMemAddress + PCI_COMMAND_OFFSET);

  if (!(SpiCmdData & BIT1)) {  //Bit1: Memory Space Enable
    SpiCmdData |= BIT1;
    MmioWrite32(SpiMemAddress + PCI_COMMAND_OFFSET, SpiCmdData);
  }

}

/**
  Enable block protection on the Serial Flash device.

  @retval     EFI_SUCCESS       Opertion is successful.
  @retval     EFI_DEVICE_ERROR  If there is any device errors.

**/
EFI_STATUS
EFIAPI
SpiFlashLock (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Read NumBytes bytes of data from the address specified by
  PAddress into Buffer.

  @param[in]      Address       The starting physical address of the read.
  @param[in,out]  NumBytes      On input, the number of bytes to read. On output, the number
                                of bytes actually read.
  @param[out]     Buffer        The destination data buffer for the read.

  @retval         EFI_SUCCESS       Opertion is successful.
  @retval         EFI_DEVICE_ERROR  If there is any device errors.

**/
EFI_STATUS
EFIAPI
SpiFlashRead (
  IN     UINTN                        Address,
  IN OUT UINT32                       *NumBytes,
     OUT UINT8                        *Buffer
  )
{
  ASSERT ((NumBytes != NULL) && (Buffer != NULL));
  if ((NumBytes == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // This function is implemented specifically for those platforms
  // at which the SPI device is memory mapped for read. So this
  // function just do a memory copy for Spi Flash Read.
  //
  CopyMem (Buffer, (VOID *) Address, *NumBytes);

  return EFI_SUCCESS;
}

/**
  Write NumBytes bytes of data from Buffer to the address specified by
  PAddresss.

  @param[in]      Address         The starting physical address of the write.
  @param[in,out]  NumBytes        On input, the number of bytes to write. On output,
                                  the actual number of bytes written.
  @param[in]      Buffer          The source data buffer for the write.

  @retval         EFI_SUCCESS       Opertion is successful.
  @retval         EFI_DEVICE_ERROR  If there is any device errors.

**/
EFI_STATUS
EFIAPI
SpiFlashWrite (
  IN     UINTN                      Address,
  IN OUT UINT32                     *NumBytes,
  IN     UINT8                      *Buffer
  )
{
  EFI_STATUS                Status;
  UINTN                     Offset;
  UINT32                    Length;
  UINT32                    RemainingBytes;

  ASSERT ((NumBytes != NULL) && (Buffer != NULL));
  if ((NumBytes == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Address >= mFlashAreaBaseAddress);

  Offset = Address - mFlashAreaBaseAddress;

  ASSERT ((*NumBytes + Offset) <= mFlashAreaSize);

  SpiMemorySpaceCheckandEnable();
  Status = EFI_SUCCESS;
  RemainingBytes = *NumBytes;


  while (RemainingBytes > 0) {
    if (RemainingBytes > SECTOR_SIZE_4KB) {
      Length = SECTOR_SIZE_4KB;
    } else {
      Length = RemainingBytes;
    }
    Status = mSpiProtocol->FlashWrite (
                             mSpiProtocol,
                             FlashRegionBios,
                             (UINT32) Offset,
                             Length,
                             Buffer
                             );
    if (EFI_ERROR (Status)) {
      break;
    }
    RemainingBytes -= Length;
    Offset += Length;
    Buffer += Length;
  }

  //
  // Actual number of bytes written
  //
  *NumBytes -= RemainingBytes;

  return Status;
}

/**
  Erase the block starting at Address.

  @param[in]  Address         The starting physical address of the block to be erased.
                              This library assume that caller garantee that the PAddress
                              is at the starting address of this block.
  @param[in]  NumBytes        On input, the number of bytes of the logical block to be erased.
                              On output, the actual number of bytes erased.

  @retval     EFI_SUCCESS.      Opertion is successful.
  @retval     EFI_DEVICE_ERROR  If there is any device errors.

**/
EFI_STATUS
EFIAPI
SpiFlashBlockErase (
  IN    UINTN                     Address,
  IN    UINTN                     *NumBytes
  )
{
  EFI_STATUS          Status;
  UINTN               Offset;
  UINTN               RemainingBytes;

  ASSERT (NumBytes != NULL);
  if (NumBytes == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Address >= mFlashAreaBaseAddress);

  Offset = Address - mFlashAreaBaseAddress;

  ASSERT ((*NumBytes % SECTOR_SIZE_4KB) == 0);
  ASSERT ((*NumBytes + Offset) <= mFlashAreaSize);

  SpiMemorySpaceCheckandEnable();
  Status = EFI_SUCCESS;
  RemainingBytes = *NumBytes;


  Status = mSpiProtocol->FlashErase (
                           mSpiProtocol,
                           FlashRegionBios,
                           (UINT32) Offset,
                           (UINT32) RemainingBytes
                           );
  return Status;
}

