/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#ifndef __SPI_MASTER_H__
#define __SPI_MASTER_H__

#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Protocol/Spi.h>

#define SPI_MASTER_SIGNATURE                      SIGNATURE_32 ('M', 'S', 'P', 'I')
#define SPI_MASTER_FROM_SPI_MASTER_PROTOCOL(a)  CR (a, SPI_MASTER, SpiMasterProtocol, SPI_MASTER_SIGNATURE)

// Marvell Flash Device Controller Registers
#define SPI_CTRL_REG                    (0x00)
#define SPI_CONF_REG                    (0x04)
#define SPI_DATA_OUT_REG                (0x08)
#define SPI_DATA_IN_REG                 (0x0c)
#define SPI_INT_CAUSE_REG               (0x10)

// Serial Memory Interface Control Register Masks
#define SPI_CS_NUM_OFFSET               2
#define SPI_CS_NUM_MASK                 (0x7 << SPI_CS_NUM_OFFSET)
#define SPI_MEM_READY_MASK              (0x1 << 1)
#define SPI_CS_EN_MASK                  (0x1 << 0)

// Serial Memory Interface Configuration Register Masks
#define SPI_BYTE_LENGTH_OFFSET          5
#define SPI_BYTE_LENGTH                 (0x1  << SPI_BYTE_LENGTH_OFFSET)
#define SPI_CPOL_OFFSET                 11
#define SPI_CPOL_MASK                   (0x1 << SPI_CPOL_OFFSET)
#define SPI_CPHA_OFFSET                 12
#define SPI_CPHA_MASK                  (0x1 << SPI_CPHA_OFFSET)
#define SPI_TXLSBF_OFFSET               13
#define SPI_TXLSBF_MASK                 (0x1 << SPI_TXLSBF_OFFSET)
#define SPI_RXLSBF_OFFSET               14
#define SPI_RXLSBF_MASK                 (0x1 << SPI_RXLSBF_OFFSET)

#define SPI_SPR_OFFSET                  0
#define SPI_SPR_MASK                    (0xf << SPI_SPR_OFFSET)
#define SPI_SPPR_0_OFFSET               4
#define SPI_SPPR_0_MASK                 (0x1 << SPI_SPPR_0_OFFSET)
#define SPI_SPPR_HI_OFFSET              6
#define SPI_SPPR_HI_MASK                (0x3 << SPI_SPPR_HI_OFFSET)

#define SPI_TRANSFER_BEGIN              0x01  // Assert CS before transfer
#define SPI_TRANSFER_END                0x02  // Deassert CS after transfers

#define SPI_TIMEOUT                     100000

typedef struct {
  MARVELL_SPI_MASTER_PROTOCOL SpiMasterProtocol;
  UINTN                   Signature;
  EFI_HANDLE              Handle;
  EFI_LOCK                Lock;
} SPI_MASTER;

EFI_STATUS
EFIAPI
MvSpiTransfer (
  IN MARVELL_SPI_MASTER_PROTOCOL *This,
  IN SPI_DEVICE *Slave,
  IN UINTN DataByteCount,
  IN VOID *DataOut,
  IN VOID *DataIn,
  IN UINTN Flag
  );

EFI_STATUS
EFIAPI
MvSpiReadWrite (
  IN  MARVELL_SPI_MASTER_PROTOCOL *This,
  IN  SPI_DEVICE *Slave,
  IN  UINT8 *Cmd,
  IN  UINTN CmdSize,
  IN  UINT8 *DataOut,
  OUT UINT8 *DataIn,
  IN  UINTN DataSize
  );

EFI_STATUS
EFIAPI
MvSpiInit (
  IN MARVELL_SPI_MASTER_PROTOCOL     * This
  );

SPI_DEVICE *
EFIAPI
MvSpiSetupSlave (
  IN MARVELL_SPI_MASTER_PROTOCOL     * This,
  IN SPI_DEVICE *Slave,
  IN UINTN Cs,
  IN SPI_MODE Mode
  );

EFI_STATUS
EFIAPI
MvSpiFreeSlave (
  IN SPI_DEVICE *Slave
  );

EFI_STATUS
EFIAPI
SpiMasterEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  );

#endif // __SPI_MASTER_H__
