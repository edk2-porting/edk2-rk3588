/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __PCIE_INIT_LIB_H__
#define __PCIE_INIT_LIB_H__


#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/PlatformPciLib.h>
#include <Regs/HisiPcieV1RegOffset.h>
#include "PcieKernelApi.h"

#define PCIE_AXI_SLAVE_BASE             (0xb3000000)
#define PCIE_MAX_AXI_SIZE               (0x1000000)
#define PCIE_AXI_BASE(port)             (PCIE_AXI_SLAVE_BASE + port * PCIE_MAX_AXI_SIZE)
#define PCIE_SMMU_BASE                  (0xb0040000)


#define PCIE_DMA_CHANNEL_NUM            (2)
#define PCIE_DMA_RESOURCE_MODE_SIZE     (0x40000)
#define PCIE_DMA_BURST_SIZE             (0x80000000)

#define PCIE_ADDR_BASE_OFFSET                       0x46C00000
#define PCIE_ADDR_BASE_HOST_ADDR                    (PCIE_ADDR_BASE_OFFSET + NP_DDR_BASE_ADDR_HOST)
#define NP_DDR_BASE_ADDR_HOST                       0x236E00000ULL



#define PCIE_GIC_MSI_ITS_BASE       (0xb7010040)
#define PCIE_INT_BASE               (13824)
#define PCIE_INT_LIMIT              (PCIE_INT_BASE + 64)

#define PCIE_NTB_MEM_SIZE             (0x1000000)
#define PCIE_NTB_BAR01_SIZE           (0x10000) // 64K
#define PCIE_NTB_BAR23_SIZE           (0x800000) // 8M
#define PCIE_NTB_BAR45_SIZE           (0x800000)

#define PCIE_IATU_END               {PCIE_IATU_OUTBOUND,0,0,0}
#define PCIE_IATU_INBOUND_MASK      (0x80000000)
#define PCIE_IATU_INDEX_MASK         (0x7f)
#define PCIE_IATU_TYPE_MASK         (0x1f)
#define PCIE_IATU_EN                     (0x1 << 0)
#define PCIE_IATU_SHIFT_MODE             (0x1 << 1)
#define PCIE_IATU_BAR_MODE               (0x1 << 2)
#define PCIE_IATU_FUNC_MODE               (0x1 << 3)
#define PCIE_IATU_AT_MODE                 (0x1 << 4) //AT mach mode
#define PCIE_IATU_ATTR_MODE               (0x1 << 5)
#define PCIE_IATU_TD_MODE                 (0x1 << 6) //TD
#define PCIE_IATU_TC_MODE                 (0x1 << 7) // TC
#define PCIE_IATU_PREFETCH_MODE             (0x1 << 8)
#define PCIE_IATU_DMA_BY_PASS_MODE          (0x1 << 9) //DMA bypass untranslate

#define PCIE_BAR_MASK_SIZE          (0x800000)
#define PCIE_BAR_TYPE_32            (0)
#define PCIE_BAR_TYPE_64            (2)
#define PCIE_BAR_PREFETCH_MODE      (1)

#define PCS_SDS_CFG_REG            0x204
#define SDS_CFG_STRIDE             0x4
#define MUX_LOS_ALOS_REG_OFFSET    0x508
#define MUX_CFG_STRIDE             0x4
#define CH_RXTX_STATUS_CFG_EN      BIT1
#define CH_RXTX_STATUS_CFG         BIT2
#define RegWrite(addr,data)            MmioWrite32((addr), (data))
#define RegRead(addr,data)             ((data) = MmioRead32 (addr))

#define PCIE_ASPM_DISABLE 0x0
#define PCIE_ASPM_ENABLE 0x1

typedef struct tagPcieDebugInfo
{
    UINT32 pcie_rdma_start_cnt;
    UINT32 pcie_wdma_start_cnt;
    UINT64 pcie_wdma_transfer_len;
    UINT64 pcie_rdma_transfer_len;
    UINT32 pcie_rdma_fail_cnt;
    UINT32 pcie_wdma_fail_cnt;
}pcie_debug_info_s;


#define bdf_2_b(bdf)    ((bdf >> 8) & 0xFF)
#define bdf_2_d(bdf)    ((bdf >> 3) & 0x1F)
#define bdf_2_f(bdf)    ((bdf >> 0) & 0x7)
#define b_d_f_2_bdf(b,d,f)    (((b & 0xff) << 8 ) | ((d & 0x1f) << 3) | ((f & 0x7) << 0))



typedef UINT32 (*pcie_dma_func_int)(UINT32  ulErrno, UINT32 ulReserved);


typedef struct {
     UINT32             ViewPort;            //iATU Viewport Register
     UINT32             RegionCtrl1;         //Region Control 1 Register
     UINT32             RegionCtrl2;         //Region Control 2 Register
     UINT32             BaseLow;             //Lower Base Address Register
     UINT32             BaseHigh;            //Upper Base Address Register
     UINT32             Limit;               //Limit Address Register
     UINT32             TargetLow;           //Lower Target Address Register
     UINT32             TargetHigh;          //Upper Target Address Register
} PCIE_IATU_VA;

typedef enum {
    PCIE_IATU_OUTBOUND    = 0x0,
    PCIE_IATU_INBOUND     = 0x1,
} PCIE_IATU_DIR;

typedef struct {
    PCIE_IATU_DIR       IatuType;
    UINT64              IatuBase;
    UINT64              IatuSize;
    UINT64              IatuTarget;
} PCIE_IATU;

typedef struct {
    UINT32              IatuType;
    UINT64              IatuBase;
    UINT32              IatuLimit;
    UINT64              IatuTarget;
    UINT32              Valid;
} PCIE_IATU_HW;

typedef struct {
    UINT32              PortIndex;
    PCIE_PORT_INFO      PortInfo;
    PCIE_IATU_HW        OutBound[PCIE_MAX_OUTBOUND];
    PCIE_IATU_HW        InBound[PCIE_MAX_INBOUND];
} PCIE_DRIVER_CFG;

typedef enum {
    PCIE_CONFIG_REG     = 0x0,
    PCIE_SYS_CONTROL    = 0x1,
} PCIE_RW_MODE;

typedef union {
    PCIE_DRIVER_CFG     PcieDevice;
    PCIE_NTB_CFG        NtbDevice;
} DRIVER_CFG_U;

typedef struct {
    VOID                *MappedOutbound[PCIE_MAX_OUTBOUND];
    UINT32              OutboundType[PCIE_MAX_OUTBOUND];
    UINT32              OutboundEn[PCIE_MAX_OUTBOUND];
} PCIE_MAPPED_IATU_ADDR;

typedef struct {
    BOOLEAN             PortIsInitilized[PCIE_MAX_ROOTBRIDGE];
    DRIVER_CFG_U        Dev[PCIE_MAX_ROOTBRIDGE];
    VOID                *DmaResource[PCIE_MAX_ROOTBRIDGE];
    UINT32              DmaChannel[PCIE_MAX_ROOTBRIDGE][PCIE_DMA_CHANNEL_NUM];
    VOID                *RegResource[PCIE_MAX_ROOTBRIDGE];
    VOID                *CfgResource[PCIE_MAX_ROOTBRIDGE];
} PCIE_INIT_CFG;

typedef enum {
    PCIE_MMIO_IEP_CFG  = 0x1000,
    PCIE_MMIO_IEP_CTRL = 0x0,
    PCIE_MMIO_EEP_CFG  = 0x9000,
    PCIE_MMIO_EEP_CTRL = 0x8000,
} NTB_MMIO_MODE;

typedef struct tagPcieDmaDes
{
    UINT32 uwChanCtrl;
    UINT32 uwLen;
    UINT32 uwLocalLow;
    UINT32 uwLocalHigh;
    UINT32 uwTagetLow;
    UINT32 uwTagetHigh;
}pcie_dma_des_s,*pcie_dma_des_ps;

typedef enum {
    PCIE_IATU_MEM,
    PCIE_IATU_CFG = 0x4,
    PCIE_IATU_IO
} PCIE_IATU_OUT_TYPE;

typedef enum {
    PCIE_PAYLOAD_128B = 0,
    PCIE_PAYLOAD_256B,
    PCIE_PAYLOAD_512B,
    PCIE_PAYLOAD_1024B,
    PCIE_PAYLOAD_2048B,
    PCIE_PAYLOAD_4096B,
    PCIE_RESERVED_PAYLOAD
} PCIE_PAYLOAD_SIZE;

typedef struct tagPcieDfxInfo
{
    PCIE_EP_AER_CAP0_U aer_cap0;
    PCIE_EP_AER_CAP1_U aer_cap1;
    PCIE_EP_AER_CAP2_U aer_cap2;
    PCIE_EP_AER_CAP3_U aer_cap3;
    PCIE_EP_AER_CAP4_U aer_cap4;
    PCIE_EP_AER_CAP5_U aer_cap5;
    PCIE_EP_AER_CAP6_U aer_cap6;
    UINT32 hdr_log0;
    UINT32 hdr_log1;
    UINT32 hdr_log2;
    UINT32 hdr_log3;
    PCIE_EP_AER_CAP11_U aer_cap11;
    PCIE_EP_AER_CAP12_U aer_cap12;
    PCIE_EP_AER_CAP13_U aer_cap13;

    PCIE_EP_PORTLOGIC62_U port_logic62;
    PCIE_EP_PORTLOGIC64_U port_logic64;
    PCIE_EP_PORTLOGIC66_U port_logic66;
    PCIE_EP_PORTLOGIC67_U port_logic67;
    PCIE_EP_PORTLOGIC69_U port_logic69;
    PCIE_EP_PORTLOGIC75_U port_logic75;
    PCIE_EP_PORTLOGIC76_U port_logic76;
    PCIE_EP_PORTLOGIC77_U port_logic77;
    PCIE_EP_PORTLOGIC79_U port_logic79;
    PCIE_EP_PORTLOGIC80_U port_logic80;
    PCIE_EP_PORTLOGIC81_U port_logic81;
    PCIE_EP_PORTLOGIC87_U port_logic87;

    PCIE_CTRL_10_U  pcie_ctrl10;
    UINT32 slve_rerr_addr_low;
    UINT32 slve_rerr_addr_up;
    UINT32 slve_werr_addr_low;
    UINT32 slve_werr_addr_up;
    UINT32 pcie_state4;
    UINT32 pcie_state5;
}PCIE_DFX_INFO_S;

VOID PcieChangeRwMode(UINT32 HostBridgeNum, UINT32 Port, PCIE_RW_MODE Mode);

UINT32 PcieIsLinkDown(UINT32 Port);

BOOLEAN PcieIsLinkUp(UINT32 soctype, UINT32 HostBridgeNum, UINT32 Port);

EFI_STATUS PcieWaitLinkUp(UINT32 Port);

EFI_STATUS PcieSetDBICS2Enable(UINT32 HostBridgeNum, UINT32 Port, UINT32 Enable);

#endif
