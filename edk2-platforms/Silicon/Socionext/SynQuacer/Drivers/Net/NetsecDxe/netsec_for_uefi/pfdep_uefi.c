/** @file

  Copyright (c) 2016 - 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "pfdep.h"

#include <Library/DebugLib.h>
#include <Library/DmaLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NetLib.h>
#include <Library/SynchronizationLib.h>

/**********************************************************************
 * Variable definitions
 **********************************************************************/
pfdep_debug_level_t pfdep_debug_level = PFDEP_DEBUG_LEVEL_NOTICE;


/**********************************************************************
 * Function definitions
 **********************************************************************/

VOID*
pfdep_malloc (
  IN  pfdep_uint32    len
  )
{
  return AllocatePool (len);
}

VOID
pfdep_free (
  IN  VOID       *addr
  )
{
  FreePool (addr);
}

pfdep_err_t
pfdep_dma_malloc (
  IN  pfdep_dev_handle_t        dev_handle,
  IN  pfdep_uint32              len,
  OUT VOID                      **addr_p,
  OUT pfdep_phys_addr_t         *phys_addr_p
  )
{
  EFI_STATUS    Status;
  UINTN         NumPages;
  UINTN         NumBytes;
  VOID          **Mapping;

  NumPages = EFI_SIZE_TO_PAGES (ALIGN_VARIABLE(len) + sizeof *Mapping);
  Status = DmaAllocateBuffer (EfiBootServicesData, NumPages, addr_p);
  if (EFI_ERROR (Status)) {
    return PFDEP_ERR_ALLOC;
  }

  //
  // Stash the address of the MAPINFO struct at the end of the buffer,
  // but make sure it appears aligned (the memory may be mapped uncached)
  //
  Mapping = (VOID **)((UINTN)*addr_p + ALIGN_VARIABLE(len));

  NumBytes = EFI_PAGES_TO_SIZE (NumPages);
  Status = DmaMap (MapOperationBusMasterCommonBuffer, *addr_p, &NumBytes,
             phys_addr_p, Mapping);

  if (EFI_ERROR (Status) || NumBytes < len) {
    DmaFreeBuffer (NumPages, *addr_p);
    return PFDEP_ERR_ALLOC;
  }

  return PFDEP_ERR_OK;
}

VOID
pfdep_dma_free (
  IN  pfdep_dev_handle_t        dev_handle,
  IN  pfdep_uint32              len,
  IN  VOID                      *addr,
  IN  pfdep_phys_addr_t         phys_addr
  )
{
  VOID          *Mapping;

  Mapping = *(VOID **)((UINTN)addr + ALIGN_VARIABLE(len));

  DmaUnmap (Mapping);
  DmaFreeBuffer (EFI_SIZE_TO_PAGES (ALIGN_VARIABLE(len) + sizeof Mapping),
    addr);
}

//
// On the receive path, we allocate a new packet and link it into the RX ring
// before returning the received packet to the caller. This means we perform
// one allocation and one free operation for each buffer received.
// So let's cache a single packet, and get rid of most of the alloc/free
// overhead on the RX path.
//
STATIC pfdep_pkt_handle_t mSparePacketBuffer;
STATIC UINT32 mSparePacketBufferSize;

pfdep_err_t
pfdep_alloc_pkt_buf (
  IN  pfdep_dev_handle_t        dev_handle,
  IN  pfdep_uint16              len,
  OUT VOID                      **addr_p,
  OUT pfdep_phys_addr_t         *phys_addr_p,
  OUT pfdep_pkt_handle_t        *pkt_handle_p
  )
{
  EFI_STATUS    Status;
  UINTN         NumBytes;

  NumBytes = ALIGN_VALUE (len, mCpu->DmaBufferAlignment);

  if (InterlockedCompareExchange32 (&mSparePacketBufferSize, len, 0) == len) {
    *pkt_handle_p = mSparePacketBuffer;
  } else {
    *pkt_handle_p = AllocateZeroPool (NumBytes + sizeof(PACKET_HANDLE) +
                                      (mCpu->DmaBufferAlignment - 8));
    if (*pkt_handle_p == NULL) {
      return PFDEP_ERR_ALLOC;
    }

    (*pkt_handle_p)->Buffer = ALIGN_POINTER (*pkt_handle_p + 1,
                                             mCpu->DmaBufferAlignment);
  }

  *addr_p = (*pkt_handle_p)->Buffer;
  Status = DmaMap (MapOperationBusMasterWrite, *addr_p, &NumBytes, phys_addr_p,
             &(*pkt_handle_p)->Mapping);
  if (EFI_ERROR (Status) || NumBytes < len) {
    FreePool (*pkt_handle_p);
    return PFDEP_ERR_ALLOC;
  }
  return PFDEP_ERR_OK;
}

VOID
pfdep_free_pkt_buf (
  IN  pfdep_dev_handle_t        dev_handle,
  IN  pfdep_uint16              len,
  IN  VOID                      *addr,
  IN  pfdep_phys_addr_t         phys_addr,
  IN  pfdep_bool                last_flag,
  IN  pfdep_pkt_handle_t        pkt_handle
  )
{
  if (last_flag != PFDEP_TRUE) {
    return;
  }

  if (pkt_handle->Mapping != NULL) {
    DmaUnmap (pkt_handle->Mapping);
  }

  if (pkt_handle->RecycleForTx) {
      pkt_handle->Released = TRUE;
  } else if (!InterlockedCompareExchange32 (&mSparePacketBufferSize, 0, len)) {
    mSparePacketBuffer = pkt_handle;
  } else {
    FreePool (pkt_handle);
  }
}
