/** @file

  Copyright (c) 2020, Linaro Ltd. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef OPTEE_RPMB_FVB_H_
#define OPTEE_RPMB_FVB_H_

/**
 Those are not currently defined in any spec, it's an internal
 contract between OP-TEE and EDK2.
 For more details check core/arch/arm/include/kernel/stmm_sp.h in OP-TEE
**/
#define SP_SVC_RPMB_READ_AARCH64        0xC4000066
#define SP_SVC_RPMB_WRITE_AARCH64       0xC4000067

#define SP_SVC_RPMB_READ_AARCH32        0x84000066
#define SP_SVC_RPMB_WRITE_AARCH32       0x84000067

#ifdef MDE_CPU_AARCH64
#define SP_SVC_RPMB_READ                SP_SVC_RPMB_READ_AARCH64
#define SP_SVC_RPMB_WRITE               SP_SVC_RPMB_WRITE_AARCH64
#endif
#ifdef MDE_CPU_ARM
#define SP_SVC_RPMB_READ                SP_SVC_RPMB_READ_AARCH32
#define SP_SVC_RPMB_WRITE               SP_SVC_RPMB_WRITE_AARCH32
#endif

#define FLASH_SIGNATURE            SIGNATURE_32 ('r', 'p', 'm', 'b')
#define INSTANCE_FROM_FVB_THIS(a)  CR (a, MEM_INSTANCE, FvbProtocol, \
                                      FLASH_SIGNATURE)

typedef struct _MEM_INSTANCE         MEM_INSTANCE;
typedef EFI_STATUS (*MEM_INITIALIZE) (MEM_INSTANCE* Instance);

/**
  This struct is used by the RPMB driver. Since the upper EDK2 layers
  expect byte addressable memory, we allocate a memory area of certain
  size and sync it to the hardware on reads/writes.
**/
struct _MEM_INSTANCE
{
    /// Internal signature used to discover the instance
    UINT32                              Signature;
    /// Function used to initialize the instance
    MEM_INITIALIZE                      Initialize;
    /// Set to true if initialized
    BOOLEAN                             Initialized;
    /// FVB protocol of the instance
    EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  FvbProtocol;
    /// Handle to install
    EFI_HANDLE                          Handle;
    /// Physical address of the beggining of the allocated memory
    EFI_PHYSICAL_ADDRESS                MemBaseAddress;
    /// Blocksize
    UINT16                              BlockSize;
    /// Number of allocated blocks
    UINT16                              NBlocks;
};

#endif
