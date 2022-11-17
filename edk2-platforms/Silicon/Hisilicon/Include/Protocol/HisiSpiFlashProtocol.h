/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#ifndef _HISI_SPI_FLASH_PROTOCOL_H_
#define _HISI_SPI_FLASH_PROTOCOL_H_

typedef struct _HISI_SPI_FLASH_PROTOCOL HISI_SPI_FLASH_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *HISI_SPI_FLASH_ERASE_INTERFACE) (
    IN HISI_SPI_FLASH_PROTOCOL   *This,
    IN UINT32                  Offset,
    IN UINT32                  ulLength
    );

typedef
EFI_STATUS
(EFIAPI *HISI_SPI_FLASH_WRITE_INTERFACE) (
    IN HISI_SPI_FLASH_PROTOCOL  *This,
    IN  UINT32                 Offset,
    IN  UINT8                 *Buffer,
    IN  UINT32                 ulLength
    );

typedef
EFI_STATUS
(EFIAPI *HISI_SPI_FLASH_READ_INTERFACE) (
    IN HISI_SPI_FLASH_PROTOCOL   *This,
    IN UINT32                  Offset,
    IN OUT UINT8              *Buffer,
    IN UINT32                  ulLength
    );

typedef
EFI_STATUS
(EFIAPI *HISI_SPI_FLASH_ERASE_WRITE_INTERFACE) (
    IN HISI_SPI_FLASH_PROTOCOL  *This,
    IN  UINT32                 Offset,
    IN  UINT8                 *Buffer,
    IN  UINT32                 ulLength
    );

struct _HISI_SPI_FLASH_PROTOCOL {
    HISI_SPI_FLASH_ERASE_INTERFACE             Erase;
    HISI_SPI_FLASH_WRITE_INTERFACE             Write;
    HISI_SPI_FLASH_READ_INTERFACE              Read;
    HISI_SPI_FLASH_ERASE_WRITE_INTERFACE       EraseWrite;
};

extern EFI_GUID gHisiSpiFlashProtocolGuid;

#endif
