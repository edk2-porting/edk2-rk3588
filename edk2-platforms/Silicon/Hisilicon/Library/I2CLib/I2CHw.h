/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _I2C_HW_H_
#define _I2C_HW_H_

#include <Uefi.h>
#include <Library/IoLib.h>

// The HNS I2C port 5 is under I2C extender
#define I2C_EXTENDER_PORT_HNS        5

#define I2C_READ_TIMEOUT             500
#define I2C_DRV_ONCE_WRITE_BYTES_NUM 8
#define I2C_DRV_ONCE_READ_BYTES_NUM  8
#define I2C_READ_SIGNAL              0x0100
#define I2C_TXRX_THRESHOLD           0x7
#define I2C_SS_SCLHCNT               0x493
#define I2C_SS_SCLLCNT               0x4fe
#define I2C_CMD_STOP_BIT             BIT9

#define I2C_REG_WRITE(reg,data) \
     MmioWrite32 ((reg), (data))

#define I2C_REG_READ(reg,result) \
     (result) = MmioRead32 ((reg))

 #define    I2C_CON_OFFSET                 0x0
 #define    I2C_TAR_OFFSET                 0x4
 #define    I2C_SAR_OFFSET                 0x8
 #define    I2C_DATA_CMD_OFFSET            0x10
 #define    I2C_SS_SCL_HCNT_OFFSET         0x14
 #define    I2C_SS_SCL_LCNT_OFFSET         0x18
 #define    I2C_FS_SCL_HCNT_OFFSET         0x1c
 #define    I2C_FS_SCL_LCNT_OFFSET         0x20
 #define    I2C_INTR_STAT_OFFSET           0x2c
 #define    I2C_INTR_MASK_OFFSET           0x30
 #define    I2C_RAW_INTR_STAT_OFFSET       0x34
 #define    I2C_RX_TL_OFFSET               0x38
 #define    I2C_TX_TL_OFFSET               0x3c
 #define    I2C_CLR_INTR_OFFSET            0x40
 #define    I2C_CLR_RX_UNDER_OFFSET        0x44
 #define    I2C_CLR_RX_OVER_OFFSET         0x48
 #define    I2C_CLR_TX_OVER_OFFSET         0x4c
 #define    I2C_CLR_RD_REQ_OFFSET          0x50
 #define    I2C_CLR_TX_ABRT_OFFSET         0x54
 #define    I2C_CLR_RX_DONE_OFFSET         0x58
 #define    I2C_CLR_ACTIVITY_OFFSET        0x5c
 #define    I2C_CLR_STOP_DET_OFFSET        0x60
 #define    I2C_CLR_START_DET_OFFSET       0x64
 #define    I2C_CLR_GEN_CALL_OFFSET        0x68
 #define    I2C_ENABLE_OFFSET              0x6c
 #define    I2C_STATUS_OFFSET              0x70
 #define    I2C_TXFLR_OFFSET               0x74
 #define    I2C_RXFLR_OFFSET               0x78
 #define    I2C_SDA_HOLD                   0x7c
 #define    I2C_TX_ABRT_SOURCE_OFFSET      0x80
 #define    I2C_SLV_DATA_ONLY_OFFSET       0x84
 #define    I2C_DMA_CR_OFFSET              0x88
 #define    I2C_DMA_TDLR_OFFSET            0x8c
 #define    I2C_DMA_RDLR_OFFSET            0x90
 #define    I2C_SDA_SETUP_OFFSET           0x94
 #define    I2C_ACK_GENERAL_CALL_OFFSET    0x98
 #define    I2C_ENABLE_STATUS_OFFSET       0x9c


 typedef union tagI2c0Con
 {
     struct
     {
        UINT32      master                : 1   ;
        UINT32      Speed                 : 2   ;
        UINT32      slave_10bit           : 1   ;
        UINT32      master_10bit          : 1   ;
        UINT32      restart_en            : 1   ;
        UINT32      slave_disable         : 1   ;
        UINT32      Reserved_0            : 25  ;
     } bits;
     UINT32     Val32;
 } I2C0_CON_U;


 typedef union tagI2c0Tar
 {
     struct
     {
         UINT32      ic_tar                : 10  ;
         UINT32      gc_or_start           : 1   ;
         UINT32      special               : 1   ;
         UINT32      ic_10bitaddr_master   : 1   ;
         UINT32      Reserved_1            : 19  ;
     } bits;
     UINT32      Val32;
 } I2C0_TAR_U;


 typedef union tagI2c0DataCmd
 {
     struct
     {
         UINT32      dat                   : 8   ;
         UINT32      cmd                   : 1   ;
         UINT32      Reserved_5            : 23  ;
     } bits;
     UINT32      Val32;
 } I2C0_DATA_CMD_U;


 typedef union tagI2c0SsSclHcnt
 {
     struct
     {
         UINT32      ic_ss_scl_hcnt        : 16  ;
         UINT32      Reserved_7            : 16  ;
     } bits;
     UINT32      Val32;
 } I2C0_SS_SCL_HCNT_U;


 typedef union tagI2c0SsSclLcnt
 {
     struct
     {
         UINT32      ic_ss_scl_lcnt        : 16  ;
         UINT32      Reserved_9            : 16  ;
     } bits;
     UINT32      Val32;
 } I2C0_SS_SCL_LCNT_U;


 typedef union tagI2c0FsSclHcnt
 {
     struct
     {
         UINT32      ic_fs_scl_hcnt        : 16  ;
         UINT32      Reserved_11           : 16  ;
     } bits;
     UINT32      Val32;
 } I2C0_FS_SCL_HCNT_U;


 typedef union tagI2c0FsSclLcnt
 {
     struct
     {
         UINT32      ic_fs_scl_lcnt        : 16  ;
         UINT32      Reserved_13           : 16  ;
     } bits;
     UINT32      Val32;
 } I2C0_FS_SCL_LCNT_U;


 typedef union tagI2c0IntrMask
 {
     struct
     {
         UINT32      m_rx_under            : 1   ;
         UINT32      m_rx_over             : 1   ;
         UINT32      m_rx_full             : 1   ;
         UINT32      m_tx_over             : 1   ;
         UINT32      m_tx_empty            : 1   ;
         UINT32      m_rd_req              : 1   ;
         UINT32      m_tx_abrt             : 1   ;
         UINT32      m_rx_done             : 1   ;
         UINT32      m_activity            : 1   ;
         UINT32      m_stop_det            : 1   ;
         UINT32      m_start_det           : 1   ;
         UINT32      m_gen_call            : 1   ;
         UINT32      Reserved_17           : 20  ;
     } bits;
     UINT32      Val32;
 } I2C0_INTR_MASK_U;


 typedef union tagI2c0RxTl
 {
     struct
     {
         UINT32      rx_tl                 : 8   ;
         UINT32      Reserved_21           : 24  ;
     } bits;
     UINT32      Val32;
 } I2C0_RX_TL_U;


 typedef union tagI2c0TxTl
 {
     struct
     {
         UINT32      tx_tl                 : 8   ;
         UINT32      Reserved_23           : 24  ;
     } bits;
     UINT32      Val32;
 } I2C0_TX_TL_U;


 typedef union tagI2c0Enable
 {
     struct
     {
         UINT32      enable                : 1   ;
         UINT32      Reserved_47           : 31  ;
     } bits;
     UINT32      Val32;
 } I2C0_ENABLE_U;


 typedef union tagI2c0Status
 {
     struct
     {
         UINT32      activity              : 1   ;
         UINT32      tfnf                  : 1   ;
         UINT32      tfe                   : 1   ;
         UINT32      rfne                  : 1   ;
         UINT32      rff                   : 1   ;
         UINT32      mst_activity          : 1   ;
         UINT32      slv_activity          : 1   ;
         UINT32      Reserved_49           : 25  ;
     } bits;
     UINT32      Val32;
 } I2C0_STATUS_U;


 typedef union tagI2c0Txflr
 {
     struct
     {
         UINT32      txflr                 : 4   ;
         UINT32      Reserved_51           : 28  ;
     } bits;
     UINT32      Val32;
 } I2C0_TXFLR_U;


 typedef union tagI2c0Rxflr
 {
     struct
     {
         UINT32      rxflr                 : 4   ;
         UINT32      Reserved_53           : 28  ;
     } bits;
     UINT32      Val32;
 } I2C0_RXFLR_U;


 typedef union tagI2c0EnableStatus
 {
     struct
     {
         UINT32      ic_en                 : 1   ;
         UINT32      slv_disable_while_busy: 1   ;
         UINT32      slv_rx_data_lost      : 1   ;
         UINT32      Reserved_69           : 29  ;
     } bits;
     UINT32      Val32;
 } I2C0_ENABLE_STATUS_U;

typedef enum {
  I2CTx,
  I2CRx
} I2CTransfer;

#endif
