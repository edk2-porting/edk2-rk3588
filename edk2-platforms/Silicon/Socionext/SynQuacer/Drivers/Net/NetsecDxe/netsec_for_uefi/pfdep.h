/** @file

  Copyright (c) 2016 - 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef PFDEP_H
#define PFDEP_H

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>

#include <Protocol/Cpu.h>

extern EFI_CPU_ARCH_PROTOCOL      *mCpu;

/**********************************************************************
 * Constant definitions
 **********************************************************************/
#define PFDEP_INT64_AVAILABLE

/**********************************************************************
 * Elementary type definitions
 **********************************************************************/
typedef INT8 pfdep_int8;
typedef UINT8 pfdep_uint8;
typedef INT16 pfdep_int16;
typedef UINT16 pfdep_uint16;
typedef INT32 pfdep_int32;
typedef UINT32 pfdep_uint32;
typedef INT64 pfdep_int64;
typedef UINT64 pfdep_uint64;
typedef BOOLEAN pfdep_bool;
typedef CHAR8 pfdep_char;

#define PFDEP_TRUE ((pfdep_bool)1)
#define PFDEP_FALSE ((pfdep_bool)0)


/**********************************************************************
 * Complex type definitions
 **********************************************************************/

typedef enum pfdep_err_e {
    PFDEP_ERR_OK = 0,
    PFDEP_ERR_PARAM,
    PFDEP_ERR_ALLOC,
    PFDEP_ERR_INTERRUPT
} pfdep_err_t;


typedef struct {
    LIST_ENTRY  Link;
    VOID        *Buffer;
    VOID        *Mapping;
    BOOLEAN     RecycleForTx;
    BOOLEAN     Released;
} PACKET_HANDLE;

typedef VOID *pfdep_dev_handle_t;
typedef PACKET_HANDLE *pfdep_pkt_handle_t;
typedef EFI_PHYSICAL_ADDRESS pfdep_phys_addr_t;
typedef UINTN pfdep_cpu_addr_t;

typedef int pfdep_hard_lock_t;
typedef int pfdep_soft_lock_t;

typedef BOOLEAN pfdep_hard_lock_ctx_t;
typedef int pfdep_soft_lock_ctx_t;

typedef unsigned int pfdep_debug_level_t;

#define PFDEP_DEBUG_LEVEL_FATAL               ((pfdep_debug_level_t)1)
#define PFDEP_DEBUG_LEVEL_WARNING             ((pfdep_debug_level_t)2)
#define PFDEP_DEBUG_LEVEL_NOTICE              ((pfdep_debug_level_t)3)
#define PFDEP_DEBUG_LEVEL_DEBUG               ((pfdep_debug_level_t)4)
#define PFDEP_DEBUG_LEVEL_DEBUG_DETAILED      ((pfdep_debug_level_t)5)
#define PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED ((pfdep_debug_level_t)6)

/**********************************************************************
 * Variable declarations
 **********************************************************************/
extern pfdep_debug_level_t pfdep_debug_level; /* defined in pfdep_uefi.c */


/**********************************************************************
 * Function declarations
 **********************************************************************/

static __inline pfdep_uint32 pfdep_iomem_read(void *addr)
{
    return *((volatile pfdep_uint32 *)(addr));
}

static __inline void pfdep_iomem_write(void *addr, pfdep_uint32 val)
{
    *((volatile pfdep_uint32 *)(addr)) = val;


}

#define pfdep_read_mem_barrier()    MemoryFence()
#define pfdep_write_mem_barrier()   MemoryFence()
#define pfdep_mem_barrier()         MemoryFence()

void *pfdep_malloc(pfdep_uint32 len);

void pfdep_free(void *addr);

pfdep_err_t pfdep_dma_malloc (
    pfdep_dev_handle_t dev_handle,
    pfdep_uint32 len,
    void **addr_p,
    pfdep_phys_addr_t *phys_addr_p
    );

void pfdep_dma_free (
    pfdep_dev_handle_t dev_handle,
    pfdep_uint32 len,
    void *addr,
    pfdep_phys_addr_t phys_addr
    );

pfdep_err_t pfdep_alloc_pkt_buf (
    pfdep_dev_handle_t dev_handle,
    pfdep_uint16 len,
    void **addr_p,
    pfdep_phys_addr_t *phys_addr_p,
    pfdep_pkt_handle_t *pkt_handle_p
    );

void pfdep_free_pkt_buf (
    pfdep_dev_handle_t dev_handle,
    pfdep_uint16 len,
    void *addr,
    pfdep_phys_addr_t phys_addr,
    pfdep_bool last_flag,
    pfdep_pkt_handle_t pkt_handle
    );

static __inline pfdep_err_t pfdep_init_hard_lock(pfdep_hard_lock_t *hard_lock_p)
{
    (void)hard_lock_p; /* suppress compiler warning */
    return PFDEP_ERR_OK;
}

static __inline void pfdep_uninit_hard_lock(pfdep_hard_lock_t *hard_lock_p)
{
    (void)hard_lock_p; /* suppress compiler warning */
    return;
}

static __inline void pfdep_acquire_hard_lock (
    pfdep_hard_lock_t *hard_lock_p, /* not used */
    pfdep_hard_lock_ctx_t *ctx_p
    )
{
    (void)hard_lock_p; /* suppress compiler warning */

    *ctx_p = SaveAndDisableInterrupts();
}


static __inline void pfdep_release_hard_lock (
    pfdep_hard_lock_t *hard_lock_p, /* not used */
    pfdep_hard_lock_ctx_t *ctx_p
    )
{
    (void)hard_lock_p; /* suppress compiler warning */

    SetInterruptState(*ctx_p);
}

static __inline pfdep_err_t pfdep_init_soft_lock (
    pfdep_soft_lock_t *soft_lock_p
    )
{
    *soft_lock_p = 0; /* suppress compiler warning */

    return PFDEP_ERR_OK;
}

static __inline void pfdep_uninit_soft_lock (
    pfdep_soft_lock_t *soft_lock_p
    )
{
    (void)soft_lock_p; /* suppress compiler warning */

    return;
}

static __inline pfdep_err_t pfdep_acquire_soft_lock (
    pfdep_soft_lock_t *soft_lock_p,
    pfdep_soft_lock_ctx_t *ctx_p
    )
{
    (void)soft_lock_p; /* suppress compiler warning */
    (void)ctx_p; /* suppress compiler warning */

    return PFDEP_ERR_OK;
}

static __inline void pfdep_release_soft_lock (
    pfdep_soft_lock_t *soft_lock_p,
    pfdep_soft_lock_ctx_t *ctx_p
    )
{
    (void)soft_lock_p; /* suppress compiler warning */
    (void)ctx_p; /* suppress compiler warning */
}


static __inline void pfdep_memcpy(void *dst_p, const void *src_p, pfdep_uint32 len)
{
    CopyMem (dst_p, src_p, (UINTN)len);
}

static __inline void pfdep_memset(void *dst_p, pfdep_uint8 c, pfdep_uint32 len)
{
    SetMem (dst_p, (UINTN)len, c);
}

static __inline pfdep_err_t pfdep_msleep(pfdep_uint32 wait_ms)
{

    MicroSecondDelay ((UINTN)wait_ms * 1000);

    return PFDEP_ERR_OK;
}

#define pfdep_print(level,...) \
do { \
    if (level <= pfdep_debug_level) { \
        DEBUG ((DEBUG_INFO, "[NETSEC] " __VA_ARGS__)); \
    } \
} while (0)


static __inline pfdep_debug_level_t pfdep_get_debug_level(void)
{
    return pfdep_debug_level;
}

static __inline void pfdep_set_debug_level(pfdep_debug_level_t level)
{
    pfdep_debug_level = level;
}


#define pfdep_assert(cond)      ASSERT(cond)

#endif /* PFDEP_H */
