/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __MVPP2_LIB_H__
#define __MVPP2_LIB_H__

#include "Mvpp2LibHw.h"
#include "Pp2Dxe.h"

/* number of RXQs used by single Port */
STATIC INT32 RxqNumber = 1;
/* number of TXQs used by single Port */
STATIC INT32 TxqNumber = 1;

VOID
Mvpp2PrsMacPromiscSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN BOOLEAN Add
  );

VOID
Mvpp2PrsMacMultiSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN INT32 Index,
  IN BOOLEAN Add
  );

INT32
Mvpp2PrsDefaultInit (
  IN MVPP2_SHARED *Priv
  );

INT32
Mvpp2PrsMacDaAccept (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN const UINT8 *Da,
  IN BOOLEAN Add
  );

VOID
Mvpp2PrsMcastDelAll (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId
  );

INT32
Mvpp2PrsTagModeSet (
  IN MVPP2_SHARED *Priv,
  IN INT32 PortId,
  IN INT32 type
  );

INT32
Mvpp2PrsDefFlow (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2ClsInit (
  IN MVPP2_SHARED *Priv
  );

VOID
Mvpp2ClsPortConfig (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2ClsOversizeRxqSet (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2BmPoolHwCreate (
  IN MVPP2_SHARED *Priv,
  IN MVPP2_BMS_POOL *BmPool,
  IN INT32 Size
  );

VOID
Mvpp2BmPoolBufsizeSet (
  IN MVPP2_SHARED *Priv,
  IN MVPP2_BMS_POOL *BmPool,
  IN INT32 BufSize
  );

VOID
Mvpp2BmStop (
  IN MVPP2_SHARED *Priv,
  IN INT32 Pool
  );

VOID
Mvpp2BmIrqClear (
  IN MVPP2_SHARED *Priv,
  IN INT32 Pool
  );

VOID
Mvpp2RxqLongPoolSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Lrxq,
  IN INT32 LongPool
  );

VOID
Mvpp2RxqShortPoolSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Lrxq,
  IN INT32 ShortPool
  );

VOID
Mvpp2BmPoolMcPut (
  IN PP2DXE_PORT *Port,
  IN INT32 Pool,
  IN UINT32 BufPhysAddr,
  IN UINT32 BufVirtAddr,
  IN INT32 McId
  );

VOID
Mvpp2PoolRefill (
  IN PP2DXE_PORT *Port,
  IN UINT32 Bm,
  IN UINT32 PhysAddr,
  IN UINT32 Cookie
  );

INTN
Mvpp2BmPoolCtrl (
  IN MVPP2_SHARED *Priv,
  IN INTN Pool,
  IN enum Mvpp2Command cmd
  );

VOID
Mvpp2InterruptsMask (
  IN VOID *arg
  );

VOID
Mvpp2InterruptsUnmask (
  IN VOID *arg
  );

VOID
Mvpp2PortEnable (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2PortDisable (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2DefaultsSet (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2IngressEnable (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2IngressDisable (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2EgressEnable (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2EgressDisable (
  IN PP2DXE_PORT *Port
  );

UINT32
Mvpp2BmCookieBuild (
  IN MVPP2_RX_DESC *RxDesc,
  IN INT32 Cpu
  );

INT32
Mvpp2TxqDrainSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Txq,
  IN BOOLEAN En
  );

INT32
Mvpp2TxqPendDescNumGet (
  IN PP2DXE_PORT *Port,
  IN MVPP2_TX_QUEUE *Txq
  );

UINT32
Mvpp2AggrTxqPendDescNumGet (
  IN MVPP2_SHARED *Priv,
  IN INT32 Cpu
  );

MVPP2_TX_DESC *
Mvpp2TxqNextDescGet (
  MVPP2_TX_QUEUE *Txq
  );

VOID
Mvpp2AggrTxqPendDescAdd (
  IN PP2DXE_PORT *Port,
  IN INT32 Pending
  );

INT32
Mvpp2AggrDescNumCheck (
  IN MVPP2_SHARED *Priv,
  IN MVPP2_TX_QUEUE *AggrTxq,
  IN INT32 Num,
  IN INT32 Cpu
  );

INT32
Mvpp2TxqAllocReservedDesc (
  IN MVPP2_SHARED *Priv,
  IN MVPP2_TX_QUEUE *Txq,
  IN INT32 Num
  );

VOID
Mvpp2TxqDescPut (
  IN MVPP2_TX_QUEUE *Txq
  );

UINT32
Mvpp2TxqDescCsum (
  IN INT32 L3Offs,
  IN INT32 L3Proto,
  IN INT32 IpHdrLen,
  IN INT32 L4Proto
  );

VOID
Mvpp2TxqSentCounterClear (
  IN OUT VOID *arg
  );

VOID
Mvpp2GmacMaxRxSizeSet (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2TxpMaxTxSizeSet (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2RxPktsCoalSet (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq,
  IN UINT32 Pkts
  );

VOID
Mvpp2RxTimeCoalSet (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq,
  IN UINT32 Usec
  );

VOID
Mvpp2AggrTxqHwInit (
  IN OUT MVPP2_TX_QUEUE *AggrTxq,
  IN INT32 DescNum,
  IN INT32 Cpu,
  IN MVPP2_SHARED *Priv
  );

VOID
Mvpp2RxqHwInit (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq
  );

VOID
Mvpp2RxqDropPkts (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq,
  IN INT32 Cpu
  );

VOID
Mvpp2TxqHwInit (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_TX_QUEUE *Txq
  );

VOID
Mvpp2TxqHwDeinit (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_TX_QUEUE *Txq
  );

VOID
Mvpp2PortPowerUp (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2RxFifoInit (
  IN MVPP2_SHARED *Priv
  );

VOID
Mvpp2RxqHwDeinit (
  IN PP2DXE_PORT *Port,
  IN OUT MVPP2_RX_QUEUE *Rxq
  );

INT32
MvGop110NetcInit (
  IN PP2DXE_PORT *Port,
  IN UINT32 NetCompConfig,
  IN enum MvNetcPhase phase
  );

UINT32
MvpPp2xGop110NetcCfgCreate (
  IN PP2DXE_PORT *Port
  );

INT32
MvGop110PortInit (
  IN PP2DXE_PORT *Port
  );

INT32
MvGop110GmacReset (
  IN PP2DXE_PORT *Port,
  IN enum MvReset ResetCmd
  );

INT32
MvGop110GpcsModeCfg (
  IN PP2DXE_PORT *Port,
  BOOLEAN En
  );

INT32
MvGop110BypassClkCfg (
  IN PP2DXE_PORT *Port,
  IN BOOLEAN En
  );

INT32
MvGop110GpcsReset (
  IN PP2DXE_PORT *Port,
  IN enum MvReset ResetCmd
  );

VOID
MvGop110Xlg2GigMacCfg (
  IN PP2DXE_PORT *Port
  );

INT32
MvGop110GmacModeCfg (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110GmacRgmiiCfg (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110GmacSgmii25Cfg (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110GmacSgmiiCfg (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110GmacQsgmiiCfg (
  IN PP2DXE_PORT *Port
  );

INT32
Mvpp2SmiPhyAddrCfg (
  IN PP2DXE_PORT *Port,
  IN INT32 PortId,
  IN INT32 Addr
  );

EFI_STATUS
MvGopXpcsModeCfg (
  IN PP2DXE_PORT *Port,
  IN INT32 NumOfLanes
  );

VOID
MvGopMpcsModeCfg (
  IN PP2DXE_PORT *Port
  );

VOID
MvGopXlgMacModeCfg (
  IN PP2DXE_PORT *Port
  );

VOID
MvGopXpcsUnreset (
  IN PP2DXE_PORT *Port
  );

VOID
MvGopXlgMacUnreset (
  IN PP2DXE_PORT *Port
  );

BOOLEAN
MvGop110PortIsLinkUp (
  IN PP2DXE_PORT *Port
  );

BOOLEAN
MvGop110GmacLinkStatusGet (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110PortDisable (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110PortEnable (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110GmacPortEnable (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110GmacPortDisable (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110GmacPortLinkEventMask (
  IN PP2DXE_PORT *Port
  );

INT32
MvGop110PortEventsMask (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110XlgPortLinkEventMask (
  IN PP2DXE_PORT *Port
  );

VOID
MvGop110GmacForceLinkUp (
  IN PP2DXE_PORT *Port
  );

INT32
MvGop110FlCfg (
  IN PP2DXE_PORT *Port
  );

INT32
MvGop110SpeedDuplexSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Speed,
  IN enum MvPortDuplex Duplex
  );

INT32
MvGop110GmacSpeedDuplexSet (
  IN PP2DXE_PORT *Port,
  IN INT32 Speed,
  IN enum MvPortDuplex Duplex
  );

VOID
Mvpp2AxiConfig (
  IN MVPP2_SHARED *Priv
  );

VOID
Mvpp2TxpClean (
  IN PP2DXE_PORT *Port,
  IN INT32 Txp,
  IN MVPP2_TX_QUEUE *Txq
  );

VOID
Mvpp2CleanupTxqs (
  IN PP2DXE_PORT *Port
  );

VOID
Mvpp2CleanupRxqs (
  IN PP2DXE_PORT *Port
  );

/* Get number of physical egress Port */
STATIC
inline
INT32
Mvpp2EgressPort (
  IN PP2DXE_PORT *Port
  )
{
  return MVPP2_MAX_TCONT + Port->Id;
}

/* Get number of physical TXQ */
STATIC
inline
INT32
Mvpp2TxqPhys (
  IN INT32 PortId,
  IN INT32 Txq
  )
{
  return (MVPP2_MAX_TCONT + PortId) * MVPP2_MAX_TXQ + Txq;
}

/* Set Pool number in a BM Cookie */
STATIC
inline
UINT32
Mvpp2BmCookiePoolSet (
  IN UINT32 Cookie,
  IN INT32 Pool
  )
{
  UINT32 Bm;

  Bm = Cookie & ~(0xFF << MVPP2_BM_COOKIE_POOL_OFFS);
  Bm |= ((Pool & 0xFF) << MVPP2_BM_COOKIE_POOL_OFFS);

  return Bm;
}

/* Get Pool number from a BM Cookie */
STATIC
inline
INT32
Mvpp2BmCookiePoolGet (
  IN UINT32 Cookie
  )
{
  return (Cookie >> MVPP2_BM_COOKIE_POOL_OFFS) & 0xFF;
}

/* Release buffer to BM */
STATIC
inline
VOID
Mvpp2BmPoolPut (
  IN MVPP2_SHARED *Priv,
  IN INT32 Pool,
  IN UINT64 BufPhysAddr,
  IN UINT64 BufVirtAddr
  )
{
  UINT32 Val = 0;

  Val = (Upper32Bits(BufVirtAddr) & MVPP22_ADDR_HIGH_MASK) << MVPP22_BM_VIRT_HIGH_RLS_OFFST;
  Val |= (Upper32Bits(BufPhysAddr) & MVPP22_ADDR_HIGH_MASK) << MVPP22_BM_PHY_HIGH_RLS_OFFSET;
  Mvpp2Write(Priv, MVPP22_BM_PHY_VIRT_HIGH_RLS_REG, Val);
  Mvpp2Write(Priv, MVPP2_BM_VIRT_RLS_REG, (UINT32)BufVirtAddr);
  Mvpp2Write(Priv, MVPP2_BM_PHY_RLS_REG(Pool), (UINT32)BufPhysAddr);
}

STATIC
inline
VOID
Mvpp2InterruptsEnable (
  IN PP2DXE_PORT *Port,
  IN INT32 CpuMask
  )
{
  Mvpp2Write(Port->Priv, MVPP2_ISR_ENABLE_REG(Port->Id), MVPP2_ISR_ENABLE_INTERRUPT(CpuMask));
}

STATIC
inline
VOID
Mvpp2InterruptsDisable (
  IN PP2DXE_PORT *Port,
  IN INT32 CpuMask
  )
{
  Mvpp2Write(Port->Priv, MVPP2_ISR_ENABLE_REG(Port->Id), MVPP2_ISR_DISABLE_INTERRUPT(CpuMask));
}

/* Get number of Rx descriptors occupied by received packets */
STATIC
inline
INT32
Mvpp2RxqReceived (
  IN PP2DXE_PORT *Port,
  IN INT32 RxqId
  )
{
  UINT32 Val = Mvpp2Read(Port->Priv, MVPP2_RXQ_STATUS_REG(RxqId));

  return Val & MVPP2_RXQ_OCCUPIED_MASK;
}

/*
 * Update Rx Queue status with the number of occupied and available
 * Rx descriptor slots.
 */
STATIC
inline
VOID
Mvpp2RxqStatusUpdate (
  IN PP2DXE_PORT *Port,
  IN INT32 RxqId,
  IN INT32 UsedCount,
  IN INT32 FreeCount
  )
{
  /*
   * Decrement the number of used descriptors and increment count
   * increment the number of free descriptors.
   */
  UINT32 Val = UsedCount | (FreeCount << MVPP2_RXQ_NUM_NEW_OFFSET);

  Mvpp2Write(Port->Priv, MVPP2_RXQ_STATUS_UPDATE_REG(RxqId), Val);
}

/* Get pointer to next RX descriptor to be processed by SW */
STATIC
inline
MVPP2_RX_DESC *
Mvpp2RxqNextDescGet (
  IN MVPP2_RX_QUEUE *Rxq
  )
{
  INT32 RxDesc = Rxq->NextDescToProc;

  Rxq->NextDescToProc = MVPP2_QUEUE_NEXT_DESC(Rxq, RxDesc);
  Mvpp2Prefetch(Rxq->Descs + Rxq->NextDescToProc);
  return Rxq->Descs + RxDesc;
}

/*
 * Get number of sent descriptors and decrement counter.
 * The number of sent descriptors is returned.
 * Per-CPU access
 */
STATIC
inline
INT32
Mvpp2TxqSentDescProc (
  IN PP2DXE_PORT *Port,
  IN MVPP2_TX_QUEUE *Txq
  )
{
  UINT32 Val;

  /* Reading status reg resets transmitted descriptor counter */
#ifdef MVPP2V1
  Val = Mvpp2Read(Port->Priv, MVPP2_TXQ_SENT_REG(Txq->Id));
#else
  Val = Mvpp2Read(Port->Priv, MVPP22_TXQ_SENT_REG(Txq->Id));
#endif

  return (Val & MVPP2_TRANSMITTED_COUNT_MASK) >> MVPP2_TRANSMITTED_COUNT_OFFSET;
}

STATIC
inline
MVPP2_RX_QUEUE *
Mvpp2GetRxQueue (
  IN PP2DXE_PORT *Port,
  IN UINT32 Cause
  )
{
  INT32 Queue = Mvpp2Fls(Cause) - 1;

  return &Port->Rxqs[Queue];
}

STATIC
inline
MVPP2_TX_QUEUE *
Mvpp2GetTxQueue (
  IN PP2DXE_PORT *Port,
  IN UINT32 Cause
  )
{
  INT32 Queue = Mvpp2Fls(Cause) - 1;

  return &Port->Txqs[Queue];
}

STATIC
inline
void
Mvpp2x2TxdescPhysAddrSet (
  IN DmaAddrT PhysAddr,
  IN MVPP2_TX_DESC *TxDesc
  )
{
  UINT64 *BufPhysAddrP = &TxDesc->BufPhysAddrHwCmd2;

  *BufPhysAddrP &= ~(MVPP22_ADDR_MASK);
  *BufPhysAddrP |= PhysAddr & MVPP22_ADDR_MASK;
}
#endif /* __MVPP2_LIB_H__ */
