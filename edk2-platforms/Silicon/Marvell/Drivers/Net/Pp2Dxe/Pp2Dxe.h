/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __PP2_DXE_H__
#define __PP2_DXE_H__

#include <Protocol/AdapterInformation.h>
#include <Protocol/Cpu.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/Ip4.h>
#include <Protocol/Ip6.h>
#include <Protocol/MvPhy.h>
#include <Protocol/SimpleNetwork.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DmaLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NetLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "Mvpp2LibHw.h"

#define MVPP2_MAX_PORT  3

#define PP2DXE_SIGNATURE                    SIGNATURE_32('P', 'P', '2', 'D')
#define INSTANCE_FROM_AIP(a)                CR((a), PP2DXE_CONTEXT, Aip, PP2DXE_SIGNATURE)
#define INSTANCE_FROM_SNP(a)                CR((a), PP2DXE_CONTEXT, Snp, PP2DXE_SIGNATURE)

/* OS API */
#define Mvpp2Alloc(v)                       AllocateZeroPool(v)
#define Mvpp2Free(p)                        FreePool(p)
#define Mvpp2Memset(a, v, s)                SetMem((a), (s), (v))
#define Mvpp2Mdelay(t)                      gBS->Stall((t) * 1000)
#define Mvpp2Fls(v)                         1
#define Mvpp2IsBroadcastEtherAddr(da)       1
#define Mvpp2IsMulticastEtherAddr(da)       1
#define Mvpp2Prefetch(v)                    do {} while(0);
#define Mvpp2Printf(...)                    do {} while(0);
#define Mvpp2SwapVariables(a,b)             do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)
#define Mvpp2SwapBytes16(x)                 SwapBytes16((x))
#define Mvpp2Iphdr                          EFI_IP4_HEADER
#define Mvpp2Ipv6hdr                        EFI_IP6_HEADER
#define MVPP2_ALIGN(x, m)                   ALIGN_VALUE((x), (m))
#define MVPP2_ENOMEM                        -1
#define MVPP2_EINVAL                        -2
#define MVPP2_ERANGE                        -3
#define MVPP2_USEC_PER_SEC                  1000000L

#define DmaAddrT                            UINTN
#define PhysAddrT                           UINTN

#define Upper32Bits(n)                      ((UINT32)(((n) >> 16) >> 16))
#define Lower32Bits(n)                      ((UINT32)(n))

#define ARCH_DMA_MINALIGN                   64

/* Port speeds */
#define MV_PORT_SPEED_10                    SPEED_10
#define MV_PORT_SPEED_100                   SPEED_100
#define MV_PORT_SPEED_1000                  SPEED_1000
#define MV_PORT_SPEED_2500                  SPEED_2500
#define MV_PORT_SPEED_10000                 SPEED_10000

/* L2 and L3 protocol macros */
#define MV_IPPR_TCP                         0
#define MV_IPPR_UDP                         1
#define MV_IPPR_IPIP                        2
#define MV_IPPR_ICMPV6                      3
#define MV_IPPR_IGMP                        4
#define MV_ETH_P_IP                         5
#define MV_ETH_P_IPV6                       6
#define MV_ETH_P_PPP_SES                    7
#define MV_ETH_P_ARP                        8
#define MV_ETH_P_8021Q                      9
#define MV_ETH_P_8021AD                     10
#define MV_ETH_P_EDSA                       11
#define MV_PPP_IP                           12
#define MV_PPP_IPV6                         13
#define MV_ETH_ALEN                         NET_ETHER_ADDR_LEN

/* PHY modes */
#define MV_MODE_SGMII                       PHY_CONNECTION_SGMII
#define MV_MODE_RGMII                       PHY_CONNECTION_RGMII
#define MV_MODE_XAUI                        PHY_CONNECTION_XAUI
#define MV_MODE_RXAUI                       PHY_CONNECTION_RXAUI
#define MV_MODE_SFI                         PHY_CONNECTION_SFI
#define MV_MODE_QSGMII                      100
#define PP2DXE_MAX_PHY                      2

/* Gop */
/* Sets the field located at the specified in data */
#define U32_SET_FIELD(data, mask, val)     ((data) = (((data) & ~(mask)) | (val)))
#define MV_RGMII_TX_FIFO_MIN_TH            0x41
#define MV_SGMII_TX_FIFO_MIN_TH            0x5
#define MV_SGMII2_5_TX_FIFO_MIN_TH         0xB

/* BM constants */
#define MVPP2_BM_POOLS_NUM                 8
#define MVPP2_BM_LONG_BUF_NUM              1024
#define MVPP2_BM_SHORT_BUF_NUM             2048
#define MVPP2_BM_POOL_SIZE_MAX             (SIZE_16KB - MVPP2_BM_POOL_PTR_ALIGN/4)
#define MVPP2_BM_POOL_PTR_ALIGN            128
#define MVPP2_BM_SWF_LONG_POOL(Port)       ((Port > 2) ? 2 : Port)
#define MVPP2_BM_SWF_SHORT_POOL            3
#define MVPP2_BM_POOL                      0
#define MVPP2_BM_SIZE                      64

/*
 * BM short pool packet Size
 * These value assure that for SWF the total number
 * of bytes allocated for each buffer will be 512
 */
#define MVPP2_BM_SHORT_PKT_SIZE            512

/*
 * Page table entries are set to 1MB, or multiples of 1MB
 * (not < 1MB). driver uses less bd's so use 1MB bdspace.
 */
#define BD_SPACE                           (1 << 20)

/* Buffer has to be aligned to 1M */
#define MVPP2_BUFFER_ALIGN_SIZE            (1 << 20)

/* RX constants */
#define RX_BUFFER_SIZE                     (ALIGN_VALUE(MTU + WRAP, ARCH_DMA_MINALIGN))
#define MVPP2_RXQ_OFFSET                   0
#define BUFF_HDR_OFFS                      32
#define BM_ALIGN                           32
#define ETH_HLEN                           14

/* 2(HW hdr) 14(MAC hdr) 4(CRC) 32(extra for cache prefetch) */
#define WRAP                              (2 + ETH_HLEN + 4 + 32)
#define MTU                               1500

/*
 * Maximum retries of checking, wheter HW really sent the packet
 * after it was done is software.
 */
#define MVPP2_TX_SEND_MAX_POLLING_COUNT   10000

/* Structures */
typedef struct {
  /* Physical number of this Tx queue */
  UINT8 Id;

  /* Logical number of this Tx queue */
  UINT8 LogId;

  /* Number of Tx DMA descriptors in the descriptor ring */
  INT32 Size;

  /* Number of currently used Tx DMA descriptor in the descriptor ring */
  INT32 count;

  UINT32 DonePktsCoal;

  /* Virtual address of thex Tx DMA descriptors array */
  MVPP2_TX_DESC *Descs;

  /* DMA address of the Tx DMA descriptors array */
  DmaAddrT DescsPhys;

  /* Index of the last Tx DMA descriptor */
  INT32 LastDesc;

  /* Index of the next Tx DMA descriptor to process */
  INT32 NextDescToProc;
} MVPP2_TX_QUEUE;

typedef struct {
  /* RX queue number, in the range 0-31 for physical RXQs */
  UINT8 Id;

  /* Num of rx descriptors in the rx descriptor ring */
  INT32 Size;

  UINT32 PktsCoal;
  UINT32 TimeCoal;

  /* Virtual address of the RX DMA descriptors array */
  MVPP2_RX_DESC *Descs;

  /* DMA address of the RX DMA descriptors array */
  DmaAddrT DescsPhys;

  /* Index of the last RX DMA descriptor */
  INT32 LastDesc;

  /* Index of the next RX DMA descriptor to process */
  INT32 NextDescToProc;

  /* ID of Port to which physical RXQ is mapped */
  INT32 Port;

  /* Port's logic RXQ number to which physical RXQ is mapped */
  INT32 LogicRxq;
} MVPP2_RX_QUEUE;

enum Mvpp2BmType {
  MVPP2_BM_FREE,
  MVPP2_BM_SWF_LONG,
  MVPP2_BM_SWF_SHORT
};

typedef struct {
  /* Pool number in the range 0-7 */
  INT32 Id;
  enum Mvpp2BmType type;

  /* Buffer Pointers Pool External (BPPE) Size */
  INT32 Size;
  /* Number of buffers for this pool */
  INT32 BufNum;
  /* Pool buffer Size */
  INT32 BufSize;
  /* Packet Size */
  INT32 PktSize;

  /* BPPE virtual base address */
  UINT32 *VirtAddr;
  /* BPPE physical base address */
  DmaAddrT PhysAddr;

  /* Ports using BM pool */
  UINT32 PortMap;
} MVPP2_BMS_POOL;

typedef struct Pp2DxePort PP2DXE_PORT;

/* Structure for preallocation for buffer */
typedef struct {
  MVPP2_TX_DESC *TxDescs[MVPP2_MAX_PORT];
  MVPP2_TX_DESC *AggrTxDescs;
  MVPP2_RX_DESC *RxDescs[MVPP2_MAX_PORT];
  DmaAddrT RxBuffers[MVPP2_MAX_PORT];
} BUFFER_LOCATION;

/* Shared Packet Processor resources */
typedef struct {
  /* Shared registers' base addresses */
  UINT64 Base;
  UINT64 MpcsBase;
  UINT64 Rfu1Base;
  UINT64 SmiBase;
  UINT64 XpcsBase;

  /* Preallocated buffers */
  BUFFER_LOCATION BufferLocation;

  /* List of pointers to Port structures */
  PP2DXE_PORT **PortList;

  /* Aggregated TXQs */
  MVPP2_TX_QUEUE *AggrTxqs;

  /* BM pools */
  MVPP2_BMS_POOL *BmPools[MVPP2_MAX_PORT];
  BOOLEAN BmEnabled;

  /* PRS shadow table */
  MVPP2_PRS_SHADOW *PrsShadow;
  /* PRS auxiliary table for double vlan entries control */
  BOOLEAN *PrsDoubleVlans;

  /* Tclk value */
  UINT32 Tclk;
} MVPP2_SHARED;

/* Individual Port structure */
struct Pp2DxePort {
  UINT8 Id;
  UINT8 GopIndex;

  INT32 Irq;

  MVPP2_SHARED *Priv;

  /* Per-Port registers' base address */
  UINT64 GmacBase;
  UINT64 XlgBase;

  MVPP2_RX_QUEUE *Rxqs;
  MVPP2_TX_QUEUE *Txqs;

  INT32 PktSize;

  UINT32 PendingCauseRx;

  /* Flags */
  UINTN Flags;

  UINT16 TxRingSize;
  UINT16 RxRingSize;

  INT32 PhyInterface;
  UINT32 PhyIndex;
  BOOLEAN Link;
  BOOLEAN Duplex;
  BOOLEAN AlwaysUp;
  PHY_SPEED Speed;

  MVPP2_BMS_POOL *PoolLong;
  MVPP2_BMS_POOL *PoolShort;

  UINT8 TxpNum;

  /* Index of first Port's physical RXQ */
  UINT8 FirstRxq;
};

typedef struct {
  MAC_ADDR_DEVICE_PATH      Pp2Mac;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PP2_DEVICE_PATH;

#define QUEUE_DEPTH 64
typedef struct {
  UINT32                      Signature;
  INTN                        Instance;
  EFI_HANDLE                  Controller;
  EFI_LOCK                    Lock;
  EFI_SIMPLE_NETWORK_PROTOCOL Snp;
  MARVELL_PHY_PROTOCOL        *Phy;
  PHY_DEVICE                  *PhyDev;
  PP2DXE_PORT                 Port;
  BOOLEAN                     Initialized;
  BOOLEAN                     LateInitialized;
  VOID                        *CompletionQueue[QUEUE_DEPTH];
  UINTN                       CompletionQueueHead;
  UINTN                       CompletionQueueTail;
  EFI_EVENT                   EfiExitBootServicesEvent;
  PP2_DEVICE_PATH             *DevicePath;
  EFI_ADAPTER_INFORMATION_PROTOCOL Aip;
} PP2DXE_CONTEXT;

/* Inline helpers */
STATIC
inline
VOID
Mvpp2Write (
  IN MVPP2_SHARED *Priv,
  IN UINT32 Offset,
  IN UINT32 data
  )
{
  ASSERT (Priv->Base != 0);
  MmioWrite32 (Priv->Base + Offset, data);
}

STATIC
inline
UINT32
Mvpp2Read (
  IN MVPP2_SHARED *Priv,
  IN UINT32 Offset
  )
{
  ASSERT (Priv->Base != 0);
  return MmioRead32 (Priv->Base + Offset);
}

STATIC
inline
UINT32
Mvpp2Rfu1Read (
  IN MVPP2_SHARED *Priv,
  UINT32 Offset
  )
{
  ASSERT (Priv->Rfu1Base != 0);
  return MmioRead32 (Priv->Rfu1Base + Offset);
}

STATIC
inline
UINT32
Mvpp2Rfu1Write (
  IN MVPP2_SHARED *Priv,
  IN UINT32 Offset,
  IN UINT32 Data
  )
{
  ASSERT (Priv->Rfu1Base != 0);
  return MmioWrite32 (Priv->Rfu1Base + Offset, Data);
}

STATIC
inline
UINT32
Mvpp2SmiRead (
  IN MVPP2_SHARED *Priv,
  IN UINT32 Offset
  )
{
  ASSERT (Priv->SmiBase != 0);
  return MmioRead32 (Priv->SmiBase + Offset);
}

STATIC
inline
UINT32
Mvpp2SmiWrite (
  IN MVPP2_SHARED *Priv,
  IN UINT32 Offset,
  IN UINT32 Data
  )
{
  ASSERT (Priv->SmiBase != 0);
  return MmioWrite32 (Priv->SmiBase + Offset, Data);
}

STATIC
inline
VOID
Mvpp2GmacWrite (
  IN PP2DXE_PORT *Port,
  IN UINT32 Offset,
  IN UINT32 Data
  )
{
  ASSERT (Port->Priv->Base != 0);
  MmioWrite32 (Port->Priv->Base + Offset, Data);
}

STATIC
inline
UINT32
Mvpp2GmacRead (
  IN PP2DXE_PORT *Port,
  IN UINT32 Offset
  )
{
  ASSERT (Port->Priv->Base != 0);
  return MmioRead32 (Port->Priv->Base + Offset);
}

STATIC
inline
VOID
MvGop110GmacWrite (
  IN PP2DXE_PORT *Port,
  IN UINT32 Offset,
  IN UINT32 Data
  )
{
  ASSERT (Port->GmacBase != 0);
  MmioWrite32 (Port->GmacBase + Offset, Data);
}

STATIC
inline
UINT32
MvGop110GmacRead (
  IN PP2DXE_PORT *Port,
  IN UINT32 Offset
  )
{
  ASSERT (Port->GmacBase != 0);
  return MmioRead32 (Port->GmacBase + Offset);
}

STATIC
inline
VOID
Mvpp2XlgWrite (
  IN PP2DXE_PORT *Port,
  IN UINT32 Offset,
  IN UINT32 Data
  )
{
  ASSERT (Port->XlgBase != 0);
  MmioWrite32 (Port->XlgBase + Offset, Data);
}

STATIC
inline
UINT32
Mvpp2XlgRead (
  IN PP2DXE_PORT *Port,
  IN UINT32 Offset
  )
{
  ASSERT (Port->XlgBase != 0);
  return MmioRead32 (Port->XlgBase + Offset);
}

/* SNP callbacks */
EFI_STATUS
EFIAPI
Pp2SnpStart (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This
  );

EFI_STATUS
EFIAPI
Pp2SnpStop (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This
  );

EFI_STATUS
EFIAPI
Pp2DxeSnpInitialize (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This,
  IN UINTN                       ExtraRxBufferSize OPTIONAL,
  IN UINTN                       ExtraTxBufferSize OPTIONAL
  );

EFI_STATUS
EFIAPI
Pp2SnpReset (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This,
  IN BOOLEAN                     ExtendedVerification
  );

EFI_STATUS
EFIAPI
Pp2SnpShutdown (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This
  );

EFI_STATUS
EFIAPI
Pp2SnpReceiveFilters (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This,
  IN UINT32                      Enable,
  IN UINT32                      Disable,
  IN BOOLEAN                     ResetMCastFilter,
  IN UINTN                       MCastFilterCnt OPTIONAL,
  IN EFI_MAC_ADDRESS             *MCastFilter OPTIONAL
  );

EFI_STATUS
EFIAPI
Pp2SnpStationAddress (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *Snp,
  IN BOOLEAN Reset,
  IN EFI_MAC_ADDRESS *NewMac
);

EFI_STATUS
EFIAPI
Pp2SnpNetStat (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This,
  IN BOOLEAN                     Reset,
  IN OUT UINTN                   *StatisticsSize OPTIONAL,
  OUT EFI_NETWORK_STATISTICS     *StatisticsTable OPTIONAL
  );

EFI_STATUS
EFIAPI
Pp2SnpIpToMac (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This,
  IN BOOLEAN                     IPv6,
  IN EFI_IP_ADDRESS              *IP,
  OUT EFI_MAC_ADDRESS            *MAC
  );

EFI_STATUS
EFIAPI
Pp2SnpGetStatus (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *Snp,
  OUT UINT32                     *InterruptStatus OPTIONAL,
  OUT VOID                       **TxBuf OPTIONAL
  );

EFI_STATUS
EFIAPI
Pp2SnpTransmit (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This,
  IN UINTN                       HeaderSize,
  IN UINTN                       BufferSize,
  IN VOID                        *Buffer,
  IN EFI_MAC_ADDRESS             *SrcAddr  OPTIONAL,
  IN EFI_MAC_ADDRESS             *DestAddr OPTIONAL,
  IN UINT16                      *EtherTypePtr OPTIONAL
  );

EFI_STATUS
EFIAPI
Pp2SnpReceive (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This,
  OUT UINTN                      *HeaderSize OPTIONAL,
  IN OUT UINTN                   *BufferSize,
  OUT VOID                       *Buffer,
  OUT EFI_MAC_ADDRESS            *SrcAddr OPTIONAL,
  OUT EFI_MAC_ADDRESS            *DstAddr OPTIONAL,
  OUT UINT16                     *EtherType OPTIONAL
  );
#endif
