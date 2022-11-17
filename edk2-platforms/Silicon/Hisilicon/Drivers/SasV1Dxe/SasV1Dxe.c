/** @file

  Copyright (c) 2016 Linaro Ltd.
  Copyright (c) 2016 Hisilicon Limited.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DmaLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/PlatformSasProtocol.h>
#include <Protocol/ScsiPassThruExt.h>
#include <IndustryStandard/Scsi.h>

#define READ_REG32(Base, Offset) MmioRead32 ((Base) + (Offset))
#define WRITE_REG32(Base, Offset, Val)  MmioWrite32 ((Base) + (Offset), (Val))

#define PHY_READ_REG32(Base, Offset, phy) MmioRead32 ((Base) + (Offset) + 0x400 * (phy))
#define PHY_WRITE_REG32(Base, Offset, phy, Val)  MmioWrite32 ((Base) + (Offset) + 0x400 * (phy), (Val))

#define DLVRY_QUEUE_ENABLE              0x0
#define IOST_BASE_ADDR_LO               0x8
#define IOST_BASE_ADDR_HI               0xc
#define ITCT_BASE_ADDR_LO               0x10
#define ITCT_BASE_ADDR_HI               0x14
#define BROKEN_MSG_ADDR_LO              0x18
#define BROKEN_MSG_ADDR_HI              0x1c
#define PHY_CONTEXT                     0x20
#define PHY_PORT_NUM_MA                 0x28
#define HGC_TRANS_TASK_CNT_LIMIT        0x38
#define AXI_AHB_CLK_CFG                 0x3c
#define HGC_SAS_TXFAIL_RETRY_CTRL       0x84
#define HGC_GET_ITV_TIME                0x90
#define DEVICE_MSG_WORK_MODE            0x94
#define I_T_NEXUS_LOSS_TIME             0xa0
#define BUS_INACTIVE_LIMIT_TIME         0xa8
#define REJECT_TO_OPEN_LIMIT_TIME       0xac
#define CFG_AGING_TIME                  0xbc
#define HGC_DFX_CFG2                    0xc0
#define FIS_LIST_BADDR_L                0xc4
#define CFG_1US_TIMER_TRSH              0xcc
#define CFG_SAS_CONFIG                  0xd4
#define INT_COAL_EN                     0x1bc
#define OQ_INT_COAL_TIME                0x1c0
#define OQ_INT_COAL_CNT                 0x1c4
#define ENT_INT_COAL_TIME               0x1c8
#define ENT_INT_COAL_CNT                0x1cc
#define OQ_INT_SRC                      0x1d0
#define OQ_INT_SRC_MSK                  0x1d4
#define ENT_INT_SRC1                    0x1d8
#define ENT_INT_SRC2                    0x1dc
#define ENT_INT_SRC_MSK1                0x1e0
#define ENT_INT_SRC_MSK2                0x1e4
#define SAS_ECC_INTR_MSK                0x1ec
#define HGC_ERR_STAT_EN                 0x238
#define DLVRY_Q_0_BASE_ADDR_LO          0x260
#define DLVRY_Q_0_BASE_ADDR_HI          0x264
#define DLVRY_Q_0_DEPTH                 0x268
#define DLVRY_Q_0_WR_PTR                0x26c
#define DLVRY_Q_0_RD_PTR                0x270
#define COMPL_Q_0_BASE_ADDR_LO          0x4e0
#define COMPL_Q_0_BASE_ADDR_HI          0x4e4
#define COMPL_Q_0_DEPTH                 0x4e8
#define COMPL_Q_0_WR_PTR                0x4ec
#define COMPL_Q_0_RD_PTR                0x4f0
#define AXI_CFG                         0x5100

#define PORT_BASE                       0x800
#define PHY_CFG                         (PORT_BASE + 0x0)
#define PHY_CFG_ENA_OFF                 0
#define PHY_CFG_ENA_MSK                 (0x1 << PHY_CFG_ENA_OFF)
#define PHY_CFG_DC_OPT_OFF              2
#define PHY_CFG_DC_OPT_MSK              (0x1 << PHY_CFG_DC_OPT_OFF)
#define PROG_PHY_LINK_RATE              (PORT_BASE + 0xc)
#define PHY_CTRL                        (PORT_BASE + 0x14)
#define PHY_CTRL_RESET                  BIT0
#define PHY_RATE_NEGO                   (PORT_BASE + 0x30)
#define PHY_PCN                         (PORT_BASE + 0x44)
#define SL_TOUT_CFG                     (PORT_BASE + 0x8c)
#define SL_CONTROL                      (PORT_BASE + 0x94)
#define SL_CONTROL_NOTIFY_EN            BIT0
#define TX_ID_DWORD0                    (PORT_BASE + 0x9c)
#define TX_ID_DWORD1                    (PORT_BASE + 0xa0)
#define TX_ID_DWORD2                    (PORT_BASE + 0xa4)
#define TX_ID_DWORD3                    (PORT_BASE + 0xa8)
#define TX_ID_DWORD4                    (PORT_BASE + 0xaC)
#define TX_ID_DWORD5                    (PORT_BASE + 0xb0)
#define TX_ID_DWORD6                    (PORT_BASE + 0xb4)
#define RX_IDAF_DWORD3                  (PORT_BASE + 0xd0)
#define RX_IDAF_DWORD4                  (PORT_BASE + 0xd4)
#define RXOP_CHECK_CFG_H                (PORT_BASE + 0xfc)
#define DONE_RECEIVED_TIME              (PORT_BASE + 0x12c)
#define CON_CFG_DRIVER                  (PORT_BASE + 0x130)
#define PHY_CONFIG2                     (PORT_BASE + 0x1a8)
#define PHY_CONFIG2_FORCE_TXDEEMPH_OFF  3
#define PHY_CONFIG2_FORCE_TXDEEMPH_MSK  (0x1 << PHY_CONFIG2_FORCE_TXDEEMPH_OFF)
#define CHL_INT_COAL_EN                 (PORT_BASE + 0x1d0)
#define CHL_INT0                        (PORT_BASE + 0x1b0)
#define CHL_INT0_PHYCTRL_NOTRDY         BIT0
#define CHL_INT1                        (PORT_BASE + 0x1b4)
#define CHL_INT2                        (PORT_BASE + 0x1b8)
#define CHL_INT2_SL_PHY_ENA             BIT6
#define CHL_INT0_MSK                    (PORT_BASE + 0x1bc)
#define CHL_INT0_MSK_PHYCTRL_NOTRDY     BIT0
#define CHL_INT1_MSK                    (PORT_BASE + 0x1c0)
#define CHL_INT2_MSK                    (PORT_BASE + 0x1c4)
#define DMA_TX_STATUS                   (PORT_BASE + 0x2d0)
#define DMA_TX_STATUS_BUSY              BIT0
#define DMA_RX_STATUS                   (PORT_BASE + 0x2e8)
#define DMA_RX_STATUS_BUSY              BIT0

#define QUEUE_CNT                       32
#define QUEUE_SLOTS                     256
#define SLOT_ENTRIES                    8192
#define PHY_CNT                         8
#define MAX_ITCT_ENTRIES                1

// Completion header
#define CMPLT_HDR_IPTT_OFF              0
#define CMPLT_HDR_IPTT_MSK              (0xffff << CMPLT_HDR_IPTT_OFF)

#define BIT(x)                          (1 << x)

// HW dma structures
// Delivery queue header
// dw0
#define CMD_HDR_RESP_REPORT_OFF     5
#define CMD_HDR_RESP_REPORT_MSK     0x20
#define CMD_HDR_TLR_CTRL_OFF        6
#define CMD_HDR_TLR_CTRL_MSK        0xc0
#define CMD_HDR_PORT_OFF            17
#define CMD_HDR_PORT_MSK            0xe0000
#define CMD_HDR_PRIORITY_OFF        27
#define CMD_HDR_PRIORITY_MSK        0x8000000
#define CMD_HDR_MODE_OFF            28
#define CMD_HDR_MODE_MSK            0x10000000
#define CMD_HDR_CMD_OFF             29
#define CMD_HDR_CMD_MSK             0xe0000000
// dw1
#define CMD_HDR_VERIFY_DTL_OFF      10
#define CMD_HDR_VERIFY_DTL_MSK      0x400
#define CMD_HDR_SSP_FRAME_TYPE_OFF  13
#define CMD_HDR_SSP_FRAME_TYPE_MSK  0xe000
#define CMD_HDR_DEVICE_ID_OFF       16
#define CMD_HDR_DEVICE_ID_MSK       0xffff0000
// dw2
#define CMD_HDR_CFL_OFF             0
#define CMD_HDR_CFL_MSK             0x1ff
#define CMD_HDR_MRFL_OFF            15
#define CMD_HDR_MRFL_MSK            0xff8000
#define CMD_HDR_FIRST_BURST_OFF     25
#define CMD_HDR_FIRST_BURST_MSK     0x2000000
// dw3
#define CMD_HDR_IPTT_OFF            0
#define CMD_HDR_IPTT_MSK            0xffff
// dw6
#define CMD_HDR_DATA_SGL_LEN_OFF    16
#define CMD_HDR_DATA_SGL_LEN_MSK    0xffff0000

// Completion header
#define CMPLT_HDR_IPTT_OFF          0
#define CMPLT_HDR_IPTT_MSK          (0xffff << CMPLT_HDR_IPTT_OFF)
#define CMPLT_HDR_CMD_CMPLT_MSK     BIT17
#define CMPLT_HDR_ERR_RCRD_XFRD_MSK BIT18
#define CMPLT_HDR_RSPNS_XFRD_MSK    BIT19
#define CMPLT_HDR_IO_CFG_ERR_MSK    BIT27

#define SENSE_DATA_PRES             26

#define SGE_LIMIT 0x10000
#define upper_32_bits(n) ((UINT32)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((UINT32)(n))
#define MAX_TARGET_ID 4

// Generic HW DMA host memory structures
struct hisi_sas_cmd_hdr {
    UINT32 dw0;
    UINT32 dw1;
    UINT32 dw2;
    UINT32 transfer_tags;
    UINT32 data_transfer_len;
    UINT32 first_burst_num;
    UINT32 sg_len;
    UINT32 dw7;
    UINT64 cmd_table_addr;
    UINT64 sts_buffer_addr;
    UINT64 prd_table_addr;
    UINT64 dif_prd_table_addr;
};

struct hisi_sas_complete_hdr {
    UINT32 data;
};

struct hisi_sas_iost {
    UINT64 qw0;
    UINT64 qw1;
    UINT64 qw2;
    UINT64 qw3;
};

struct hisi_sas_itct {
    UINT64 qw0;
    UINT64 sas_addr;
    UINT64 qw2;
    UINT64 qw3;
    UINT64 qw4;
    UINT64 qw_sata_ncq0_3;
    UINT64 qw_sata_ncq7_4;
    UINT64 qw_sata_ncq11_8;
    UINT64 qw_sata_ncq15_12;
    UINT64 qw_sata_ncq19_16;
    UINT64 qw_sata_ncq23_20;
    UINT64 qw_sata_ncq27_24;
    UINT64 qw_sata_ncq31_28;
    UINT64 qw_non_ncq_iptt;
    UINT64 qw_rsvd0;
    UINT64 qw_rsvd1;
};

struct hisi_sas_breakpoint {
    UINT8 data[128];
};

struct hisi_sas_sge {
    UINT64 addr;
    UINT32 page_ctrl_0;
    UINT32 page_ctrl_1;
    UINT32 data_len;
    UINT32 data_off;
};

struct hisi_sas_sge_page {
    struct hisi_sas_sge sg[512];
};

struct hisi_sas_cmd {
    UINT8 cmd[128];
};

struct hisi_sas_sts {
UINT32 status[260];
};

struct hisi_sas_slot {
    BOOLEAN used;
};

struct hisi_hba {
    struct hisi_sas_cmd_hdr      *cmd_hdr[QUEUE_CNT];
    struct hisi_sas_complete_hdr *complete_hdr[QUEUE_CNT];
    struct hisi_sas_sge_page     *sge[QUEUE_CNT];
    struct hisi_sas_sts          *status_buf[QUEUE_CNT];
    struct hisi_sas_cmd          *command_table[QUEUE_CNT];
    struct hisi_sas_iost         *iost;
    struct hisi_sas_itct         *itct;
    struct hisi_sas_breakpoint   *breakpoint;
    struct hisi_sas_slot         *slots;
    UINT32 base;
    int queue;
    int port_id;
    UINT32 LatestTargetId;
    UINT64 LatestLun;
};

#pragma pack (1)
typedef struct {
  VENDOR_DEVICE_PATH                  Vendor;
  UINT64                              PhysBase;
  EFI_DEVICE_PATH_PROTOCOL            End;
} SAS_V1_TRANSPORT_DEVICE_PATH;
#pragma pack ()

typedef struct {
    UINT32 Signature;
    EFI_EXT_SCSI_PASS_THRU_MODE     ExtScsiPassThruMode;
    EFI_EXT_SCSI_PASS_THRU_PROTOCOL ExtScsiPassThru;
    SAS_V1_TRANSPORT_DEVICE_PATH    *DevicePath;
    struct hisi_hba *hba;
    EFI_EVENT TimerEvent;
} SAS_V1_INFO;

#define SAS_DEVICE_SIGNATURE SIGNATURE_32 ('S','A','S','0')
#define SAS_FROM_PASS_THRU(a) CR (a, SAS_V1_INFO, ExtScsiPassThru, SAS_DEVICE_SIGNATURE)

STATIC EFI_STATUS prepare_cmd (
  struct hisi_hba *hba,
  EFI_EXT_SCSI_PASS_THRU_SCSI_REQUEST_PACKET    *Packet
  )
{
  struct hisi_sas_slot *slot;
  struct hisi_sas_cmd_hdr *hdr;
  struct hisi_sas_sge_page *sge;
  struct hisi_sas_sts *sts;
  struct hisi_sas_cmd *cmd;
  EFI_SCSI_SENSE_DATA *SensePtr = Packet->SenseData;
  VOID   *Buffer = NULL;
  UINTN BufferSize = 0;
  int queue = hba->queue;
  UINT32 r, w = 0, slot_idx = 0;
  UINT32 base = hba->base;
  UINT8 *p;
  EFI_PHYSICAL_ADDRESS  BufferAddress;
  EFI_STATUS            Status = EFI_SUCCESS;
  VOID                  *BufferMap = NULL;
  DMA_MAP_OPERATION DmaOperation = MapOperationBusMasterCommonBuffer;

  while (1) {
    w = READ_REG32(base, DLVRY_Q_0_WR_PTR + (queue * 0x14));
    r = READ_REG32(base, DLVRY_Q_0_RD_PTR + (queue * 0x14));
    slot_idx = queue * QUEUE_SLOTS + w;
    slot = &hba->slots[slot_idx];
    if (slot->used || (r == (w+1) % QUEUE_SLOTS)) {
      queue = (queue + 1) % QUEUE_CNT;
      if (queue == hba->queue) {
        DEBUG ((EFI_D_ERROR, "could not find free slot\n"));
        return EFI_NOT_READY;
      }
      continue;
    }
    break;
  }

  hdr = &hba->cmd_hdr[queue][w];
  cmd = &hba->command_table[queue][w];
  sts = &hba->status_buf[queue][w];
  sge = &hba->sge[queue][w];

  ZeroMem (cmd, sizeof (struct hisi_sas_cmd));
  ZeroMem (sts, sizeof (struct hisi_sas_sts));
  if (SensePtr)
    ZeroMem (SensePtr, sizeof (EFI_SCSI_SENSE_DATA));

  slot->used = TRUE;
  hba->queue = (queue + 1) % QUEUE_CNT;

  // Only consider ssp
  hdr->dw0 = (1 << CMD_HDR_RESP_REPORT_OFF) |
       (0x2 << CMD_HDR_TLR_CTRL_OFF) |
       (hba->port_id << CMD_HDR_PORT_OFF) |
       (1 << CMD_HDR_MODE_OFF) |
       (1 << CMD_HDR_CMD_OFF);
  hdr->dw1 = 1 << CMD_HDR_VERIFY_DTL_OFF;
  hdr->dw1 |= 0 << CMD_HDR_DEVICE_ID_OFF;
  hdr->dw2 = 0x83000d;
  hdr->transfer_tags = slot_idx << CMD_HDR_IPTT_OFF;

  if (Packet->DataDirection == EFI_EXT_SCSI_DATA_DIRECTION_READ) {
    Buffer = Packet->InDataBuffer;
    BufferSize = Packet->InTransferLength;
    if (Buffer) {
      hdr->dw1 |= 1 << CMD_HDR_SSP_FRAME_TYPE_OFF;
      DmaOperation = MapOperationBusMasterWrite;
    }
  } else if (Packet->DataDirection == EFI_EXT_SCSI_DATA_DIRECTION_WRITE) {
    Buffer = Packet->OutDataBuffer;
    BufferSize = Packet->OutTransferLength;
    if (Buffer) {
      DmaOperation = MapOperationBusMasterRead;
      hdr->dw1 |= 2 << CMD_HDR_SSP_FRAME_TYPE_OFF;
    }
  } else {
    hdr->dw1 |= 0 << CMD_HDR_SSP_FRAME_TYPE_OFF;
  }

  hdr->data_transfer_len = BufferSize;
  hdr->cmd_table_addr = (UINT64)cmd;
  hdr->sts_buffer_addr = (UINT64)sts;

  CopyMem (&cmd->cmd[36], Packet->Cdb, Packet->CdbLength);

  if (Buffer != NULL) {
    struct hisi_sas_sge *sg;
    UINT32 remain, len, pos = 0, i = 0;

    Status = DmaMap (DmaOperation, Buffer, &BufferSize, &BufferAddress, &BufferMap);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    remain = len = BufferSize;

    while (remain) {
      if (len > SGE_LIMIT)
        len = SGE_LIMIT;
      sg = &sge->sg[i];
      sg->addr = (UINT64)(BufferAddress + pos);
      sg->page_ctrl_0 = sg->page_ctrl_1 = 0;
      sg->data_len = len;
      sg->data_off = 0;
      remain -= len;
      pos += len;
      len = remain;
      i++;
    }

    hdr->prd_table_addr = (UINT64)sge;
    hdr->sg_len = i << CMD_HDR_DATA_SGL_LEN_OFF;
  }

  // Ensure descriptor effective before start dma
  MemoryFence();

  // Start dma
  WRITE_REG32(base, DLVRY_Q_0_WR_PTR + queue * 0x14, ++w % QUEUE_SLOTS);

  // Wait for dma complete
  while (slot->used) {
    if (READ_REG32(base, OQ_INT_SRC) & BIT(queue)) {
      struct hisi_sas_complete_hdr *complete_hdr;
      UINT32 data, rd;
      rd = READ_REG32(base, COMPL_Q_0_RD_PTR + (0x14 * queue));

      complete_hdr = &hba->complete_hdr[queue][rd];
      data = complete_hdr->data;

      // Check whether dma transfer error
      if ((data & CMPLT_HDR_ERR_RCRD_XFRD_MSK) &&
        !(data & CMPLT_HDR_RSPNS_XFRD_MSK)) {
        DEBUG ((EFI_D_VERBOSE, "sas retry data=0x%x\n", data));
        DEBUG ((EFI_D_VERBOSE, "sts[0]=0x%x\n", sts->status[0]));
        DEBUG ((EFI_D_VERBOSE, "sts[1]=0x%x\n", sts->status[1]));
        DEBUG ((EFI_D_VERBOSE, "sts[2]=0x%x\n", sts->status[2]));
        Status = EFI_NOT_READY;
        // wait 1 second and retry, some disk need long time to be ready
        // and ScsiDisk treat retry over 3 times as error
        MicroSecondDelay(1000000);
      }
       // Update read point
       WRITE_REG32(base, COMPL_Q_0_RD_PTR + (0x14 * queue), w);
       // Clear int
       WRITE_REG32(base, OQ_INT_SRC, BIT(queue));
       slot->used = FALSE;
       break;
    }
    // Wait for status change in polling
    NanoSecondDelay (100);
  }

  if (BufferMap)
       DmaUnmap (BufferMap);

  p = (UINT8 *)&sts->status[0];
  if (p[SENSE_DATA_PRES]) {
    // Disk not ready normal return for ScsiDiskTestUnitReady do next try
    SensePtr->Sense_Key = EFI_SCSI_SK_NOT_READY;
    SensePtr->Addnl_Sense_Code = EFI_SCSI_ASC_NOT_READY;
    SensePtr->Addnl_Sense_Code_Qualifier = EFI_SCSI_ASCQ_IN_PROGRESS;
    // wait 1 second for disk spin up, refer drivers/scsi/sd.c
    MicroSecondDelay(1000000);
  }
  return Status;
}

STATIC VOID hisi_sas_v1_init(struct hisi_hba *hba, PLATFORM_SAS_PROTOCOL *plat)
{
  int i, j;
  UINT32 val, base = hba->base;

  // Reset
  for (i = 0; i < PHY_CNT; i++) {
    UINT32 phy_ctrl = PHY_READ_REG32(base, PHY_CTRL, i);

    phy_ctrl |= PHY_CTRL_RESET;
    PHY_WRITE_REG32(base, PHY_CTRL, i, phy_ctrl);
  }
  // spec says safe to wait 50us after reset
  MicroSecondDelay(50);

  // Ensure DMA tx & rx idle
  for (i = 0; i < PHY_CNT; i++) {
    UINT32 dma_tx_status, dma_rx_status;

    for (j = 0; j < 100; j++) {
      dma_tx_status = PHY_READ_REG32(base, DMA_TX_STATUS, i);
      dma_rx_status = PHY_READ_REG32(base, DMA_RX_STATUS, i);

      if (!(dma_tx_status & DMA_TX_STATUS_BUSY) &&
        !(dma_rx_status & DMA_RX_STATUS_BUSY))
        break;

      // Wait for status change in polling
      NanoSecondDelay (100);
    }
  }

  // Ensure axi bus idle
  for (j = 0; j < 100; j++) {
    UINT32 axi_status = READ_REG32(base, AXI_CFG);
    if (axi_status == 0)
      break;

    // Wait for status change in polling
    NanoSecondDelay (100);
  }

  plat->Init(plat);

  WRITE_REG32(base, DLVRY_QUEUE_ENABLE, 0xffffffff);
  WRITE_REG32(base, HGC_TRANS_TASK_CNT_LIMIT, 0x11);
  WRITE_REG32(base, DEVICE_MSG_WORK_MODE, 0x1);
  WRITE_REG32(base, HGC_SAS_TXFAIL_RETRY_CTRL, 0x1ff);
  WRITE_REG32(base, HGC_ERR_STAT_EN, 0x401);
  WRITE_REG32(base, CFG_1US_TIMER_TRSH, 0x64);
  WRITE_REG32(base, HGC_GET_ITV_TIME, 0x1);
  WRITE_REG32(base, I_T_NEXUS_LOSS_TIME, 0x64);
  WRITE_REG32(base, BUS_INACTIVE_LIMIT_TIME, 0x2710);
  WRITE_REG32(base, REJECT_TO_OPEN_LIMIT_TIME, 0x1);
  WRITE_REG32(base, CFG_AGING_TIME, 0x7a12);
  WRITE_REG32(base, HGC_DFX_CFG2, 0x9c40);
  WRITE_REG32(base, FIS_LIST_BADDR_L, 0x2);
  WRITE_REG32(base, INT_COAL_EN, 0xc);
  WRITE_REG32(base, OQ_INT_COAL_TIME, 0x186a0);
  WRITE_REG32(base, OQ_INT_COAL_CNT, 1);
  WRITE_REG32(base, ENT_INT_COAL_TIME, 0x1);
  WRITE_REG32(base, ENT_INT_COAL_CNT, 0x1);
  WRITE_REG32(base, OQ_INT_SRC, 0xffffffff);
  WRITE_REG32(base, ENT_INT_SRC1, 0xffffffff);
  WRITE_REG32(base, ENT_INT_SRC_MSK1, 0);
  WRITE_REG32(base, ENT_INT_SRC2, 0xffffffff);
  WRITE_REG32(base, ENT_INT_SRC_MSK2, 0);
  WRITE_REG32(base, SAS_ECC_INTR_MSK, 0);
  WRITE_REG32(base, AXI_AHB_CLK_CFG, 0x2);
  WRITE_REG32(base, CFG_SAS_CONFIG, 0x22000000);

  for (i = 0; i < PHY_CNT; i++) {
    PHY_WRITE_REG32(base, PROG_PHY_LINK_RATE, i, 0x88a);
    PHY_WRITE_REG32(base, PHY_CONFIG2, i, 0x7c080);
    PHY_WRITE_REG32(base, PHY_RATE_NEGO, i, 0x415ee00);
    PHY_WRITE_REG32(base, PHY_PCN, i, 0x80a80000);
    PHY_WRITE_REG32(base, SL_TOUT_CFG, i, 0x7d7d7d7d);
    PHY_WRITE_REG32(base, DONE_RECEIVED_TIME, i, 0x0);
    PHY_WRITE_REG32(base, RXOP_CHECK_CFG_H, i, 0x1000);
    PHY_WRITE_REG32(base, DONE_RECEIVED_TIME, i, 0);
    PHY_WRITE_REG32(base, CON_CFG_DRIVER, i, 0x13f0a);
    PHY_WRITE_REG32(base, CHL_INT_COAL_EN, i, 3);
    PHY_WRITE_REG32(base, DONE_RECEIVED_TIME, i, 8);
  }

  for (i = 0; i < QUEUE_CNT; i++) {
    WRITE_REG32(base, DLVRY_Q_0_BASE_ADDR_HI + (i * 0x14), upper_32_bits((UINT64)(hba->cmd_hdr[i])));
    WRITE_REG32(base, DLVRY_Q_0_BASE_ADDR_LO + (i * 0x14), lower_32_bits((UINT64)(hba->cmd_hdr[i])));
    WRITE_REG32(base, DLVRY_Q_0_DEPTH + (i * 0x14), QUEUE_SLOTS);

    WRITE_REG32(base, COMPL_Q_0_BASE_ADDR_HI + (i * 0x14), upper_32_bits((UINT64)(hba->complete_hdr[i])));
    WRITE_REG32(base, COMPL_Q_0_BASE_ADDR_LO + (i * 0x14), lower_32_bits((UINT64)(hba->complete_hdr[i])));
    WRITE_REG32(base, COMPL_Q_0_DEPTH + (i * 0x14), QUEUE_SLOTS);
  }

  WRITE_REG32(base, ITCT_BASE_ADDR_LO, lower_32_bits((UINT64)(hba->itct)));
  WRITE_REG32(base, ITCT_BASE_ADDR_HI, upper_32_bits((UINT64)(hba->itct)));

  WRITE_REG32(base, IOST_BASE_ADDR_LO, lower_32_bits((UINT64)(hba->iost)));
  WRITE_REG32(base, IOST_BASE_ADDR_HI, upper_32_bits((UINT64)(hba->iost)));

  WRITE_REG32(base, BROKEN_MSG_ADDR_LO, lower_32_bits((UINT64)(hba->breakpoint)));
  WRITE_REG32(base, BROKEN_MSG_ADDR_HI, upper_32_bits((UINT64)(hba->breakpoint)));

  for (i = 0; i < PHY_CNT; i++) {
    // Clear interrupt status
    val = PHY_READ_REG32(base, CHL_INT0, i);
    PHY_WRITE_REG32(base, CHL_INT0, i, val);
    val = PHY_READ_REG32(base, CHL_INT1, i);
    PHY_WRITE_REG32(base, CHL_INT1, i, val);
    val = PHY_READ_REG32(base, CHL_INT2, i);
    PHY_WRITE_REG32(base, CHL_INT2, i, val);

    // Bypass chip bug mask abnormal intr
    PHY_WRITE_REG32(base, CHL_INT0_MSK, i, 0x3fffff & ~CHL_INT0_MSK_PHYCTRL_NOTRDY);
  }

  // Init phy
  for (i = 0; i < PHY_CNT; i++) {
    PHY_WRITE_REG32(base, TX_ID_DWORD0, i, 0x10010e00);
    PHY_WRITE_REG32(base, TX_ID_DWORD1, i, 0x16);
    PHY_WRITE_REG32(base, TX_ID_DWORD2, i, 0x20880150);
    PHY_WRITE_REG32(base, TX_ID_DWORD3, i, 0x16);
    PHY_WRITE_REG32(base, TX_ID_DWORD4, i, 0x20880150);
    PHY_WRITE_REG32(base, TX_ID_DWORD5, i, 0x0);

    val = PHY_READ_REG32(base, PHY_CFG, i);
    val &= ~PHY_CFG_DC_OPT_MSK;
    val |= 1 << PHY_CFG_DC_OPT_OFF;
    PHY_WRITE_REG32(base, PHY_CFG, i, val);

    val = PHY_READ_REG32(base, PHY_CONFIG2, i);
    val &= ~PHY_CONFIG2_FORCE_TXDEEMPH_MSK;
    PHY_WRITE_REG32(base, PHY_CONFIG2, i, val);

    val = PHY_READ_REG32(base, PHY_CFG, i);
    val |= PHY_CFG_ENA_MSK;
    PHY_WRITE_REG32(base, PHY_CFG, i, val);
  }
}

STATIC VOID sas_init(SAS_V1_INFO *SasV1Info, PLATFORM_SAS_PROTOCOL *plat)
{
  struct hisi_hba *hba = SasV1Info->hba;
  int i, s;

  for (i = 0; i < QUEUE_CNT; i++) {
    s = sizeof(struct hisi_sas_cmd_hdr) * QUEUE_SLOTS;
    DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (s), (VOID *)&hba->cmd_hdr[i]);
    ASSERT (hba->cmd_hdr[i] != NULL);
    ZeroMem (hba->cmd_hdr[i], s);

    s = sizeof(struct hisi_sas_complete_hdr) * QUEUE_SLOTS;
    DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (s), (VOID *)&hba->complete_hdr[i]);
    ASSERT (hba->complete_hdr[i] != NULL);
    ZeroMem (hba->complete_hdr[i], s);

    s = sizeof(struct hisi_sas_sts) * QUEUE_SLOTS;
    DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (s), (VOID *)&hba->status_buf[i]);
    ASSERT (hba->status_buf[i] != NULL);
    ZeroMem (hba->status_buf[i], s);

    s = sizeof(struct hisi_sas_cmd) * QUEUE_SLOTS;
    DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (s), (VOID *)&hba->command_table[i]);
    ASSERT (hba->command_table[i] != NULL);
    ZeroMem (hba->command_table[i], s);

    s = sizeof(struct hisi_sas_sge_page) * QUEUE_SLOTS;
    DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (s), (VOID *)&hba->sge[i]);
    ASSERT (hba->sge[i] != NULL);
    ZeroMem (hba->sge[i], s);
  }

  s = SLOT_ENTRIES * sizeof(struct hisi_sas_iost);
  DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (s), (VOID *)&hba->iost);
  ASSERT (hba->iost != NULL);
  ZeroMem (hba->iost, s);

  s = SLOT_ENTRIES * sizeof(struct hisi_sas_breakpoint);
  DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (s), (VOID *)&hba->breakpoint);
  ASSERT (hba->breakpoint != NULL);
  ZeroMem (hba->breakpoint, s);

  s = MAX_ITCT_ENTRIES * sizeof(struct hisi_sas_itct);
  DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (s), (VOID *)&hba->itct);
  ASSERT (hba->itct != NULL);
  ZeroMem (hba->itct, s);

  hba->slots = AllocateZeroPool (SLOT_ENTRIES * sizeof(struct hisi_sas_slot));
  ASSERT (hba->slots != NULL);

  hisi_sas_v1_init(hba, plat);
}

STATIC
EFI_STATUS
EFIAPI
SasV1ExtScsiPassThruFunction (
  IN EFI_EXT_SCSI_PASS_THRU_PROTOCOL                    *This,
  IN UINT8                                              *Target,
  IN UINT64                                             Lun,
  IN OUT EFI_EXT_SCSI_PASS_THRU_SCSI_REQUEST_PACKET     *Packet,
  IN EFI_EVENT                                          Event OPTIONAL
  )
{
  SAS_V1_INFO *SasV1Info = SAS_FROM_PASS_THRU(This);
  struct hisi_hba *hba = SasV1Info->hba;

  return prepare_cmd(hba, Packet);
}

STATIC
EFI_STATUS
EFIAPI
SasV1ExtScsiPassThruGetNextTargetLun (
  IN  EFI_EXT_SCSI_PASS_THRU_PROTOCOL    *This,
  IN OUT UINT8                           **Target,
  IN OUT UINT64                          *Lun
  )
{
  SAS_V1_INFO *SasV1Info = SAS_FROM_PASS_THRU(This);
  struct hisi_hba *hba = SasV1Info->hba;
  UINT8 ScsiId[TARGET_MAX_BYTES];
  UINT8 TargetId;

  if (*Target == NULL || Lun == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SetMem (ScsiId, TARGET_MAX_BYTES, 0xFF);

  TargetId = (*Target)[0];

  if (TargetId == MAX_TARGET_ID) {
    return EFI_NOT_FOUND;
  }

  if (CompareMem(*Target, ScsiId, TARGET_MAX_BYTES) == 0) {
    SetMem (*Target, TARGET_MAX_BYTES,0);
  } else {
    (*Target)[0] = (UINT8) (hba->LatestTargetId + 1);
  }

  *Lun = 0;

  //
  // Update the LatestTargetId.
  //
  hba->LatestTargetId  = (*Target)[0];
  hba->LatestLun       = *Lun;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
SasV1ExtScsiPassThruBuildDevicePath (
  IN     EFI_EXT_SCSI_PASS_THRU_PROTOCOL    *This,
  IN     UINT8                              *Target,
  IN     UINT64                             Lun,
  IN OUT EFI_DEVICE_PATH_PROTOCOL           **DevicePath
  )
{
  SAS_V1_INFO *SasV1Info = SAS_FROM_PASS_THRU(This);

  *DevicePath  = DuplicateDevicePath ((EFI_DEVICE_PATH_PROTOCOL *)(SasV1Info->DevicePath));
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
SasV1ExtScsiPassThruGetTargetLun (
  IN  EFI_EXT_SCSI_PASS_THRU_PROTOCOL    *This,
  IN  EFI_DEVICE_PATH_PROTOCOL           *DevicePath,
  OUT UINT8                              **Target,
  OUT UINT64                             *Lun
  )
{
  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
SasV1ExtScsiPassThruResetChannel (
  IN  EFI_EXT_SCSI_PASS_THRU_PROTOCOL   *This
  )
{

  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
SasV1ExtScsiPassThruResetTarget (
  IN EFI_EXT_SCSI_PASS_THRU_PROTOCOL    *This,
  IN UINT8                              *Target,
  IN UINT64                             Lun
  )
{

  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
SasV1ExtScsiPassThruGetNextTarget (
  IN  EFI_EXT_SCSI_PASS_THRU_PROTOCOL    *This,
  IN OUT UINT8                           **Target
  )
{

  return EFI_UNSUPPORTED;
}

STATIC EFI_EXT_SCSI_PASS_THRU_PROTOCOL SasV1ExtScsiPassThruProtocolTemplate = {
  NULL,
  SasV1ExtScsiPassThruFunction,
  SasV1ExtScsiPassThruGetNextTargetLun,
  SasV1ExtScsiPassThruBuildDevicePath,
  SasV1ExtScsiPassThruGetTargetLun,
  SasV1ExtScsiPassThruResetChannel,
  SasV1ExtScsiPassThruResetTarget,
  SasV1ExtScsiPassThruGetNextTarget
};

EFI_STATUS
EFIAPI
SasDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  PLATFORM_SAS_PROTOCOL *plat;
  EFI_STATUS Status;

  Status = gBS->OpenProtocol (
                Controller,
                &gPlatformSasProtocolGuid,
                (VOID **) &plat,
                This->DriverBindingHandle,
                Controller,
                EFI_OPEN_PROTOCOL_BY_DRIVER
                );
  if (Status == EFI_ALREADY_STARTED) {
    return EFI_SUCCESS;
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Close the Sas Host used to perform the supported test
  //
  gBS->CloseProtocol (
      Controller,
      &gPlatformSasProtocolGuid,
      This->DriverBindingHandle,
      Controller
      );

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SasDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  EFI_STATUS Status;
  PLATFORM_SAS_PROTOCOL *plat;
  SAS_V1_INFO *SasV1Info = NULL;
  SAS_V1_TRANSPORT_DEVICE_PATH  *DevicePath;
  UINT32 val, base;
  int i, phy_id = 0;
  struct hisi_sas_itct *itct;
  struct hisi_hba *hba;

  Status = gBS->OpenProtocol (
                Controller,
                &gPlatformSasProtocolGuid,
                (VOID **) &plat,
                This->DriverBindingHandle,
                Controller,
                EFI_OPEN_PROTOCOL_BY_DRIVER
                );

  if (EFI_ERROR (Status)) {
    if (Status == EFI_ALREADY_STARTED) {
      return EFI_SUCCESS;
    }
    return Status;
  }

  SasV1Info = AllocateZeroPool (sizeof (SAS_V1_INFO));
  ASSERT (SasV1Info);
  SasV1Info->Signature = SAS_DEVICE_SIGNATURE;

  SasV1Info->hba = AllocateZeroPool (sizeof(struct hisi_hba));
  ASSERT (SasV1Info->hba);
  hba = SasV1Info->hba;
  base = hba->base = plat->BaseAddr;

  sas_init(SasV1Info, plat);

  // Wait for sas controller phyup happen
  MicroSecondDelay(100000);

  for (i = 0; i < PHY_CNT; i++) {
    val = PHY_READ_REG32(base, CHL_INT2, i);

    if (val & CHL_INT2_SL_PHY_ENA) {
      phy_id = i;
    }
  }

  itct = &hba->itct[0]; //device_id = 0

  hba->port_id = (READ_REG32(base, PHY_PORT_NUM_MA) >> (4 * phy_id)) & 0xf;
  // Setup itct
  itct->qw0 = 0x355;
  itct->sas_addr = PHY_READ_REG32(base, RX_IDAF_DWORD3, phy_id);
  itct->sas_addr = itct->sas_addr << 32 | PHY_READ_REG32(base, RX_IDAF_DWORD4, phy_id);
  itct->qw2 = 0;

  // Clear phyup
  PHY_WRITE_REG32(base, CHL_INT2, phy_id, CHL_INT2_SL_PHY_ENA);
  val = PHY_READ_REG32(base, CHL_INT0, phy_id);
  val &= ~CHL_INT0_PHYCTRL_NOTRDY;
  PHY_WRITE_REG32(base, CHL_INT0, phy_id, val);
  PHY_WRITE_REG32(base, CHL_INT0_MSK, phy_id, 0x3ce3ee);

  // Need notify
  val = PHY_READ_REG32(base, SL_CONTROL, phy_id);
  val |= SL_CONTROL_NOTIFY_EN;
  PHY_WRITE_REG32(base, SL_CONTROL, phy_id, val);
  // wait 100ms required for notify takes effect, refer drivers/scsi/hisi_sas/hisi_sas_v1_hw.c
  MicroSecondDelay(100000);
  val = PHY_READ_REG32(base, SL_CONTROL, phy_id);
  val &= ~SL_CONTROL_NOTIFY_EN;
  PHY_WRITE_REG32(base, SL_CONTROL, phy_id, val);

  CopyMem (&SasV1Info->ExtScsiPassThru, &SasV1ExtScsiPassThruProtocolTemplate, sizeof (EFI_EXT_SCSI_PASS_THRU_PROTOCOL));
  SasV1Info->ExtScsiPassThruMode.AdapterId = 2;
  SasV1Info->ExtScsiPassThruMode.Attributes = EFI_EXT_SCSI_PASS_THRU_ATTRIBUTES_PHYSICAL | EFI_EXT_SCSI_PASS_THRU_ATTRIBUTES_LOGICAL;
  SasV1Info->ExtScsiPassThruMode.IoAlign  = 64; //cache line align
  SasV1Info->ExtScsiPassThru.Mode = &SasV1Info->ExtScsiPassThruMode;

  DevicePath = (SAS_V1_TRANSPORT_DEVICE_PATH *)CreateDeviceNode (
                                               HARDWARE_DEVICE_PATH,
                                               HW_VENDOR_DP,
                                               sizeof (SAS_V1_TRANSPORT_DEVICE_PATH));
  ASSERT (DevicePath != NULL);
  SasV1Info->DevicePath = DevicePath;

  CopyMem (&DevicePath->Vendor.Guid, &gPlatformSasProtocolGuid, sizeof (EFI_GUID));
  DevicePath->PhysBase = base;
  SetDevicePathNodeLength (&DevicePath->Vendor,
                           sizeof (*DevicePath) - sizeof (DevicePath->End));
  SetDevicePathEndNode (&DevicePath->End);

  Status = gBS->InstallMultipleProtocolInterfaces (
                &Controller,
                &gEfiDevicePathProtocolGuid, DevicePath,
                &gEfiExtScsiPassThruProtocolGuid, &SasV1Info->ExtScsiPassThru,
                NULL);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SasDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL   *This,
  IN  EFI_HANDLE                    Controller,
  IN  UINTN                         NumberOfChildren,
  IN  EFI_HANDLE                    *ChildHandleBuffer
  )
{
  SAS_V1_INFO *SasV1Info;
  EFI_STATUS Status;
  EFI_EXT_SCSI_PASS_THRU_PROTOCOL *ExtScsi;
  int i, s;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiExtScsiPassThruProtocolGuid,
                  (VOID **) &ExtScsi,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SasV1Info = SAS_FROM_PASS_THRU(ExtScsi);

  Status = gBS->UninstallMultipleProtocolInterfaces (
                Controller,
                &gEfiDevicePathProtocolGuid,
                SasV1Info->DevicePath,
                &gEfiExtScsiPassThruProtocolGuid,
                &SasV1Info->ExtScsiPassThru,
                NULL);
  if (!EFI_ERROR (Status)) {
    gBS->CloseProtocol (
           Controller,
           &gPlatformSasProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );

    gBS->CloseEvent (SasV1Info->TimerEvent);

    for (i = 0; i < QUEUE_CNT; i++) {
      s = sizeof(struct hisi_sas_cmd_hdr) * QUEUE_SLOTS;
      DmaFreeBuffer(EFI_SIZE_TO_PAGES (s), (VOID *)SasV1Info->hba->cmd_hdr[i]);
      s = sizeof(struct hisi_sas_complete_hdr) * QUEUE_SLOTS;
      DmaFreeBuffer(EFI_SIZE_TO_PAGES (s), (VOID *)SasV1Info->hba->complete_hdr[i]);
      s = sizeof(struct hisi_sas_sts) * QUEUE_SLOTS;
      DmaFreeBuffer(EFI_SIZE_TO_PAGES (s), (VOID *)SasV1Info->hba->status_buf[i]);
      s = sizeof(struct hisi_sas_cmd) * QUEUE_SLOTS;
      DmaFreeBuffer(EFI_SIZE_TO_PAGES (s), (VOID *)SasV1Info->hba->command_table[i]);
      s = sizeof(struct hisi_sas_sge_page) * QUEUE_SLOTS;
      DmaFreeBuffer(EFI_SIZE_TO_PAGES (s), (VOID *)SasV1Info->hba->sge[i]);
    }

    s = SLOT_ENTRIES * sizeof(struct hisi_sas_iost);
    DmaFreeBuffer(EFI_SIZE_TO_PAGES (s), (VOID *)SasV1Info->hba->iost);
    s = SLOT_ENTRIES * sizeof(struct hisi_sas_breakpoint);
    DmaFreeBuffer(EFI_SIZE_TO_PAGES (s), (VOID *)SasV1Info->hba->breakpoint);
    s = MAX_ITCT_ENTRIES * sizeof(struct hisi_sas_itct);
    DmaFreeBuffer(EFI_SIZE_TO_PAGES (s), (VOID *)SasV1Info->hba->itct);

    FreePool (SasV1Info->hba->slots);
    FreePool (SasV1Info->hba);
    FreePool (SasV1Info);
    return EFI_SUCCESS;
  }
  return Status;
}

EFI_DRIVER_BINDING_PROTOCOL gSasDriverBinding = {
  SasDriverBindingSupported,
  SasDriverBindingStart,
  SasDriverBindingStop,
  0xa,
  NULL,
  NULL
};

EFI_STATUS
SasV1Initialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  return EfiLibInstallDriverBindingComponentName2 (
           ImageHandle,
           SystemTable,
           &gSasDriverBinding,
           ImageHandle,
           NULL,
           NULL
           );
}
