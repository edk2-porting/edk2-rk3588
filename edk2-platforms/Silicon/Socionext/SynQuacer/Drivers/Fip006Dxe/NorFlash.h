/** @file  NorFlashDxe.h

  Copyright (c) 2011 - 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __NOR_FLASH_DXE_H__
#define __NOR_FLASH_DXE_H__


#include <PiDxe.h>

#include <Guid/EventGroup.h>

#include <Protocol/BlockIo.h>
#include <Protocol/DiskIo.h>
#include <Protocol/FirmwareVolumeBlock.h>

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/NorFlashPlatformLib.h>

#include "Fip006Reg.h"

#define NOR_FLASH_ERASE_RETRY                     10

#define GET_NOR_BLOCK_ADDRESS(BaseAddr, Lba, LbaSize) \
                                      ((BaseAddr) + (UINTN)((Lba) * (LbaSize)))

#define NOR_FLASH_SIGNATURE          SIGNATURE_32('S', 'n', 'o', 'r')
#define INSTANCE_FROM_FVB_THIS(a)    CR(a, NOR_FLASH_INSTANCE, FvbProtocol, \
                                        NOR_FLASH_SIGNATURE)
#define INSTANCE_FROM_BLKIO_THIS(a)  CR(a, NOR_FLASH_INSTANCE, BlockIoProtocol,\
                                        NOR_FLASH_SIGNATURE)
#define INSTANCE_FROM_DISKIO_THIS(a) CR(a, NOR_FLASH_INSTANCE, DiskIoProtocol, \
                                        NOR_FLASH_SIGNATURE)

#define CSDC(Data, Cont, Trp, Dec) \
  (((Data) << 8) | (((Cont) & 1) << 3) | (((Trp) & 3) << 1) | ((Dec) & 1))

#define CSDC_TRP_MBM              0
#define CSDC_TRP_DUAL             1
#define CSDC_TRP_QUAD             2
#define CSDC_TRP_SINGLE           3

#define CSDC_DEC_LEAVE_ASIS       0
#define CSDC_DEC_DECODE           1

#define CSDC_CONT_NON_CONTINUOUS  0
#define CSDC_CONT_CONTINUOUS      1

#define CSDC_ADDRESS_7_0          0x0
#define CSDC_ADDRESS_15_8         0x1
#define CSDC_ADDRESS_23_16        0x2
#define CSDC_ADDRESS_31_24        0x3
#define CSDC_HIGH_Z               0x4
#define CSDC_END                  0x7

typedef struct {
  UINT8       Code;
  BOOLEAN     AddrAccess;
  BOOLEAN     AddrMode4Byte;
  BOOLEAN     HighZ;
  BOOLEAN     ReadWrite;
  UINT8       CscfgMbm;
  UINT8       CsdcTrp;
} CSDC_DEFINITION;

typedef struct _NOR_FLASH_INSTANCE                NOR_FLASH_INSTANCE;

typedef EFI_STATUS (*NOR_FLASH_INITIALIZE)        (NOR_FLASH_INSTANCE* Instance);

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH                  Vendor;
  UINT8                               Index;
  EFI_DEVICE_PATH_PROTOCOL            End;
} NOR_FLASH_DEVICE_PATH;
#pragma pack()

struct _NOR_FLASH_INSTANCE {
  UINT32                              Signature;
  EFI_HANDLE                          Handle;

  BOOLEAN                             Initialized;
  NOR_FLASH_INITIALIZE                Initialize;

  UINTN                               HostRegisterBaseAddress;
  UINTN                               DeviceBaseAddress;
  UINTN                               RegionBaseAddress;
  UINTN                               Size;
  UINTN                               BlockSize;
  UINTN                               LastBlock;
  EFI_LBA                             StartLba;
  EFI_LBA                             OffsetLba;

  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL FvbProtocol;
  VOID*                               ShadowBuffer;

  NOR_FLASH_DEVICE_PATH               DevicePath;

  UINT32                              Flags;
#define NOR_FLASH_POLL_FSR      BIT0
};

typedef struct {
  EFI_TPL         OriginalTPL;
  BOOLEAN         InterruptsEnabled;
} NOR_FLASH_LOCK_CONTEXT;

VOID
EFIAPI
NorFlashLock (
  NOR_FLASH_LOCK_CONTEXT    *Context
  );

VOID
EFIAPI
NorFlashUnlock (
  NOR_FLASH_LOCK_CONTEXT    *Context
  );

EFI_STATUS
NorFlashReadCfiData (
  IN  UINTN                   DeviceBaseAddress,
  IN  UINTN                   CFI_Offset,
  IN  UINT32                  NumberOfBytes,
  OUT UINT32                  *Data
  );

EFI_STATUS
NorFlashWriteBuffer (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN UINTN                  TargetAddress,
  IN UINTN                  BufferSizeInBytes,
  IN UINT32                 *Buffer
  );

extern UINTN     mFlashNvStorageVariableBase;

EFI_STATUS
NorFlashCreateInstance (
  IN UINTN                  HostRegisterBase,
  IN UINTN                  NorFlashDeviceBase,
  IN UINTN                  NorFlashRegionBase,
  IN UINTN                  NorFlashSize,
  IN UINT32                 Index,
  IN UINT32                 BlockSize,
  IN BOOLEAN                HasVarStore,
  OUT NOR_FLASH_INSTANCE**  NorFlashInstance
  );

EFI_STATUS
EFIAPI
NorFlashFvbInitialize (
  IN NOR_FLASH_INSTANCE* Instance
  );

EFI_STATUS
ValidateFvHeader (
  IN  NOR_FLASH_INSTANCE *Instance
  );

EFI_STATUS
InitializeFvAndVariableStoreHeaders (
  IN NOR_FLASH_INSTANCE *Instance
  );

EFI_STATUS
EFIAPI
FvbGetAttributes(
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL     *This,
  OUT       EFI_FVB_ATTRIBUTES_2                    *Attributes
  );

EFI_STATUS
EFIAPI
FvbSetAttributes(
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL     *This,
  IN OUT    EFI_FVB_ATTRIBUTES_2                    *Attributes
  );

EFI_STATUS
EFIAPI
FvbGetPhysicalAddress(
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL     *This,
  OUT       EFI_PHYSICAL_ADDRESS                    *Address
  );

EFI_STATUS
EFIAPI
FvbGetBlockSize(
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL     *This,
  IN        EFI_LBA                                 Lba,
  OUT       UINTN                                   *BlockSize,
  OUT       UINTN                                   *NumberOfBlocks
  );

EFI_STATUS
EFIAPI
FvbRead(
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL     *This,
  IN        EFI_LBA                                 Lba,
  IN        UINTN                                   Offset,
  IN OUT    UINTN                                   *NumBytes,
  IN OUT    UINT8                                   *Buffer
  );

EFI_STATUS
EFIAPI
FvbWrite(
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL     *This,
  IN        EFI_LBA                                 Lba,
  IN        UINTN                                   Offset,
  IN OUT    UINTN                                   *NumBytes,
  IN        UINT8                                   *Buffer
  );

EFI_STATUS
EFIAPI
FvbEraseBlocks(
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL     *This,
  ...
  );

//
// NorFlashDxe.c
//

EFI_STATUS
NorFlashUnlockAndEraseSingleBlock (
  IN NOR_FLASH_INSTANCE     *Instance,
  IN UINTN                  BlockAddress
  );

EFI_STATUS
NorFlashWriteSingleBlock (
  IN        NOR_FLASH_INSTANCE   *Instance,
  IN        EFI_LBA               Lba,
  IN        UINTN                 Offset,
  IN OUT    UINTN                *NumBytes,
  IN        UINT8                *Buffer
  );

EFI_STATUS
NorFlashWriteBlocks (
  IN  NOR_FLASH_INSTANCE *Instance,
  IN  EFI_LBA           Lba,
  IN  UINTN             BufferSizeInBytes,
  IN  VOID              *Buffer
  );

EFI_STATUS
NorFlashReadBlocks (
  IN NOR_FLASH_INSTANCE   *Instance,
  IN EFI_LBA              Lba,
  IN UINTN                BufferSizeInBytes,
  OUT VOID                *Buffer
  );

EFI_STATUS
NorFlashRead (
  IN NOR_FLASH_INSTANCE   *Instance,
  IN EFI_LBA              Lba,
  IN UINTN                Offset,
  IN UINTN                BufferSizeInBytes,
  OUT VOID                *Buffer
  );

EFI_STATUS
NorFlashWrite (
  IN        NOR_FLASH_INSTANCE   *Instance,
  IN        EFI_LBA               Lba,
  IN        UINTN                 Offset,
  IN OUT    UINTN                *NumBytes,
  IN        UINT8                *Buffer
  );

EFI_STATUS
NorFlashReset (
  IN  NOR_FLASH_INSTANCE *Instance
  );

EFI_STATUS
NorFlashReadID (
  IN  NOR_FLASH_INSTANCE  *Instance,
  OUT UINT8               JedecId[3]
  );

#define SPINOR_SR_WIP                 BIT0  // Write in progress
#define SPINOR_FSR_READY              BIT7  // Flag Status Register: ready

#define SPINOR_OP_WREN                0x06  // Write enable
#define SPINOR_OP_WRDIS               0x04  // Write enable
#define SPINOR_OP_RDSR                0x05  // Read status register
#define SPINOR_OP_WRSR                0x01  // Write status register 1 byte
#define SPINOR_OP_RDSR2               0x3f  // Read status register 2
#define SPINOR_OP_WRSR2               0x3e  // Write status register 2
#define SPINOR_OP_READ                0x03  // Read data bytes (low frequency)
#define SPINOR_OP_READ_FAST           0x0b  // Read data bytes (high frequency)
#define SPINOR_OP_READ_1_1_2          0x3b  // Read data bytes (Dual Output SPI)
#define SPINOR_OP_READ_1_2_2          0xbb  // Read data bytes (Dual I/O SPI)
#define SPINOR_OP_READ_1_1_4          0x6b  // Read data bytes (Quad Output SPI)
#define SPINOR_OP_READ_1_4_4          0xeb  // Read data bytes (Quad I/O SPI)
#define SPINOR_OP_PP                  0x02  // Page program (up to 256 bytes)
#define SPINOR_OP_PP_1_1_4            0x32  // Quad page program
#define SPINOR_OP_PP_1_4_4            0x38  // Quad page program
#define SPINOR_OP_BE_4K               0x20  // Erase 4KiB block
#define SPINOR_OP_BE_4K_PMC           0xd7  // Erase 4KiB block on PMC chips
#define SPINOR_OP_BE_32K              0x52  // Erase 32KiB block
#define SPINOR_OP_CHIP_ERASE          0xc7  // Erase whole flash chip
#define SPINOR_OP_SE                  0xd8  // Sector erase (usually 64KiB)
#define SPINOR_OP_RDID                0x9f  // Read JEDEC ID
#define SPINOR_OP_RDSFDP              0x5a  // Read SFDP
#define SPINOR_OP_RDCR                0x35  // Read configuration register
#define SPINOR_OP_RDFSR               0x70  // Read flag status register
#define SPINOR_OP_READ_4B             0x13  // Read data bytes (low frequency)
#define SPINOR_OP_READ_FAST_4B        0x0c  // Read data bytes (high frequency)
#define SPINOR_OP_READ_1_1_2_4B       0x3c  // Read data bytes (Dual Output SPI)
#define SPINOR_OP_READ_1_2_2_4B       0xbc  // Read data bytes (Dual I/O SPI)
#define SPINOR_OP_READ_1_1_4_4B       0x6c  // Read data bytes (Quad Output SPI)
#define SPINOR_OP_READ_1_4_4_4B       0xec  // Read data bytes (Quad I/O SPI)
#define SPINOR_OP_PP_4B               0x12  // Page program (up to 256 bytes)
#define SPINOR_OP_PP_1_1_4_4B         0x34  // Quad page program
#define SPINOR_OP_PP_1_4_4_4B         0x3e  // Quad page program
#define SPINOR_OP_BE_4K_4B            0x21  // Erase 4KiB block
#define SPINOR_OP_BE_32K_4B           0x5c  // Erase 32KiB block
#define SPINOR_OP_SE_4B               0xdc  // Sector erase (usually 64KiB)
#define SPINOR_OP_RD_ARRAY            0xe8  // Read array
#define SPINOR_OP_RD_NVCFG            0xb5  // Read non-volatile config register
#define SPINOR_OP_RD_VCR              0x85  // Read VCR register
#define SPINOR_OP_RD_EVCR             0x65  // Read EVCR register

#endif /* __NOR_FLASH_DXE_H__ */
