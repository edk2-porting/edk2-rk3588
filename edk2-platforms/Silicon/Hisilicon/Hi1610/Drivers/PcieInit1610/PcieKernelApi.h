/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __PCIE_KERNEL_API_H__
#define __PCIE_KERNEL_API_H__

#define PCIE_MAX_OUTBOUND               (6)
#define PCIE_MAX_INBOUND               (4)
#define PCIE3_MAX_OUTBOUND               (16)
#define PCIE3_MAX_INBOUND                (16)

#define PCIE_LINK_LOOP_CNT          (0x1000)
#define PCIE_IATU_ADDR_MASK         (0xFFFFF000)
#define PCIE_1M_ALIGN_SHIRFT        (20)
#define PCIE_BDF_MASK               (0xF0000FFF)
#define PCIE_BUS_SHIRFT             (20)
#define PCIE_DEV_SHIRFT             (15)
#define PCIE_FUNC_SHIRFT            (12)

#define PCIE_DBI_CS2_ENABLE              (0x1)
#define PCIE_DBI_CS2_DISABLE             (0x0)

#define PCIE_DMA_CHANLE_READ             (0x1)
#define PCIE_DMA_CHANLE_WRITE             (0x0)


#define PCIE_ERR_IATU_TABLE_NULL            EFIERR (1)
#define PCIE_ERR_LINK_OVER_TIME             EFIERR (2)
#define PCIE_ERR_UNIMPLEMENT_PCIE_TYPE      EFIERR (3)
#define PCIE_ERR_ALREADY_INIT               EFIERR (4)
#define PCIE_ERR_PARAM_INVALID              EFIERR (5)
#define PCIE_ERR_MEM_OPT_OVER               EFIERR (6)
#define PCIE_ERR_NOT_INIT                   EFIERR (7)
#define PCIE_ERR_CFG_OPT_OVER               EFIERR (8)
#define PCIE_ERR_DMA_READ_CHANLE_BUSY       EFIERR (9)
#define PCIE_ERR_DMA_WRITE_CHANLE_BUSY      EFIERR (10)
#define PCIE_ERR_DMAR_NO_RESORCE            EFIERR (11)
#define PCIE_ERR_DMAW_NO_RESORCE            EFIERR (12)
#define PCIE_ERR_DMA_OVER_MAX_RESORCE       EFIERR (13)
#define PCIE_ERR_NO_IATU_WINDOW             EFIERR (14)
#define PCIE_ERR_DMA_TRANSPORT_OVER_TIME    EFIERR (15)
#define PCIE_ERR_DMA_MEM_ALLOC_ERROR        EFIERR (16)
#define PCIE_ERR_DMA_ABORT                  EFIERR (17)
#define PCIE_ERR_UNSUPPORT_BAR_TYPE         EFIERR (18)

typedef enum {
    PCIE_ROOT_COMPLEX,
    PCIE_END_POINT,
    PCIE_NTB_TO_NTB,
    PCIE_NTB_TO_RP,
} PCIE_PORT_TYPE;

typedef enum {
    PCIE_GEN1_0 = 1,    //PCIE 1.0
    PCIE_GEN2_0 = 2,    //PCIE 2.0
    PCIE_GEN3_0 = 4     //PCIE 3.0
} PCIE_PORT_GEN;

typedef enum {
    PCIE_WITDH_X1 = 0x1,
    PCIE_WITDH_X2 = 0x3,
    PCIE_WITDH_X4 = 0x7,
    PCIE_WITDH_X8 = 0xf,
    PCIE_WITDH_INVALID
} PCIE_PORT_WIDTH;


typedef struct {
    PCIE_PORT_TYPE      PortType;
    PCIE_PORT_WIDTH     PortWidth;
    PCIE_PORT_GEN       PortGen;
    UINT8               PcieLinkUp;
} PCIE_PORT_INFO;

typedef struct tagPciecfg_params
{
    UINT32 preemphasis;
    UINT32 deemphasis;
    UINT32 swing;
    UINT32 balance;
}pcie_cfg_params_s;

typedef enum {
    PCIE_CORRECTABLE_ERROR = 0,
    PCIE_NON_FATAL_ERROR,
    PCIE_FATAL_ERROR,
    PCIE_UNSUPPORTED_REQUEST_ERROR,
    PCIE_ALL_ERROR
} PCIE_ERROR_TYPE;

typedef union tagPcieDeviceStatus
{
       struct
        {
            UINT16 correctable_error : 1;
            UINT16 non_fatal_error : 1;
            UINT16 fatal_error : 1;
            UINT16 unsupported_error : 1;
            UINT16 aux_power : 1;
            UINT16 transaction_pending : 1;
            UINT16 reserved_6_15 : 10;
       }Bits;

    UINT16 Value;
}pcie_device_status_u;


typedef union tagPcieUcAerStatus
{
      struct
        {
            UINT32    undefined   : 1   ; /* [0] undefined  */
            UINT32    reserved_1_3   : 3   ; /* reserved */
            UINT32    data_link_proto_error   : 1   ; /* Data Link Protocol Error Status */
            UINT32    reserved_5_11   : 7   ; /* reserved */
            UINT32    poisoned_tlp_status   : 1   ; /* Poisoned TLP Status */
            UINT32    flow_control_proto_error   : 1   ; /* Flow Control Protocol Error Status */
            UINT32    completion_time_out   : 1   ; /* Completion Timeout Status */
            UINT32    compler_abort_status   : 1   ; /* Completer Abort Status */
            UINT32    unexpect_completion_status   : 1   ; /* Unexpected Completion Status */
            UINT32    receiver_overflow_status   : 1   ; /*Receiver Overflow Status */
            UINT32    malformed_tlp_status   : 1   ; /* Malformed TLP Status*/
            UINT32    ecrc_error_status   : 1   ; /* ECRC Error Status */
            UINT32    unsupport_request_error_status   : 1   ; /* Unsupported Request Error Status */
            UINT32    reserved_21   : 1   ; /*  reserved */
            UINT32    uncorrectable_interal_error   : 1   ; /* Uncorrectable Internal Error Status */
            UINT32    reserved_23   : 1   ; /* reserved*/
            UINT32    atomicop_egress_blocked_status   : 1   ; /* AtomicOp Egress Blocked Status */
            UINT32    tlp_prefix_blocked_error_status   : 1   ; /* TLP Prefix Blocked Error Status */
            UINT32    reserved_26_31   : 1   ; /* reserved */
       }Bits;

       UINT32 Value;
}pcie_uc_aer_status_u;

typedef union tagPcieCoAerStatus
{
       struct
        {
            UINT32    receiver_error_status   : 1   ; /* Receiver Error Status  */
            UINT32    reserved_1_5   : 5   ; /* Reserved */
            UINT32    bad_tlp_status   : 1   ; /* Bad TLP Status */
            UINT32    bad_dllp_status   : 1   ; /* Bad DLLP Status */
            UINT32    reply_num_rollover_status   : 1   ; /* REPLAY_NUM Rollover Status*/
            UINT32    reserved_9_11   : 3   ; /* Reserved */
            UINT32    reply_timer_timeout   : 1   ; /* Replay Timer Timeout Status */
            UINT32    advisory_nonfatal_error   : 1   ; /* Advisory Non-Fatal Error Status*/
            UINT32    corrected_internal_error   : 1   ; /*Corrected Internal Error Status*/
            UINT32    reserved_15_31   : 1   ; /* Reserved */
       }Bits;
       UINT32 Value;
}pcie_co_aer_status_u;

typedef struct tagPcieAerStatus
{
    pcie_uc_aer_status_u uc_aer_status;
    pcie_co_aer_status_u co_aer_status;
}pcie_aer_status_s;



typedef struct tagPcieLoopTestResult
{
   UINT32 tx_pkts_cnt;
   UINT32 rx_pkts_cnt;
   UINT32 error_pkts_cnt;
   UINT32 droped_pkts_cnt;
   UINT32 push_cnt;
   pcie_device_status_u device_status;
   pcie_aer_status_s    pcie_aer_status;
} pcie_loop_test_result_s;

typedef struct tagPcieDmaChannelAttrs {
    UINT32 dma_chan_en;
    UINT32 dma_mode;
    UINT32 channel_status;
}pcie_dma_channel_attrs_s;

typedef enum tagPcieDmaChannelStatus
{
    PCIE_DMA_CS_RESERVED = 0,
    PCIE_DMA_CS_RUNNING = 1,
    PCIE_DMA_CS_HALTED = 2,
    PCIE_DMA_CS_STOPPED = 3
}pcie_dma_channel_status_e;

typedef enum tagPcieDmaIntType{
    PCIE_DMA_INT_TYPE_DONE=0,
    PCIE_DMA_INT_TYPE_ABORT,
    PCIE_DMA_INT_ALL,
    PCIE_DMA_INT_NONE
}pcie_dma_int_type_e;

typedef enum tagPcieMulWinSize
{
    WIN_SIZE_4K = 0xc,
    WIN_SIZE_8K,
    WIN_SIZE_16K,
    WIN_SIZE_32K,
    WIN_SIZE_64K,
    WIN_SIZE_128K,
    WIN_SIZE_256K,
    WIN_SIZE_512K,
    WIN_SIZE_1M,
    WIN_SIZE_2M,
    WIN_SIZE_4M,
    WIN_SIZE_8M,
    WIN_SIZE_16M,
    WIN_SIZE_32M,
    WIN_SIZE_64M,
    WIN_SIZE_128M,
    WIN_SIZE_256M,
    WIN_SIZE_512M,
    WIN_SIZE_1G,
    WIN_SIZE_2G,
    WIN_SIZE_4G,
    WIN_SIZE_8G,
    WIN_SIZE_16G,
    WIN_SIZE_32G,
    WIN_SIZE_64G,
    WIN_SIZE_128G,
    WIN_SIZE_256G,
    WIN_SIZE_512G = 0x27,
}pcie_mul_win_size_e;

typedef struct tagPcieMultiCastCfg
{
    UINT64 multicast_base_addr;
    pcie_mul_win_size_e base_addr_size;
    UINT64 base_translate_addr;
}pcie_multicast_cfg_s;

typedef enum tagPcieMode
{
    PCIE_EP_DEVICE = 0x0,
    LEGACY_PCIE_EP_DEVICE = 0x1,
    RP_OF_PCIE_RC = 0x4,
    PCIE_INVALID = 0x100
}pcie_mode_e;

typedef struct{
    UINT32              PortIndex;
    PCIE_PORT_INFO      PortInfo;
    UINT64              iep_bar01; /*iep bar 01*/
    UINT64              iep_bar23;
    UINT64              iep_bar45;
    UINT64              iep_bar01_xlat;
    UINT64              iep_bar23_xlat;
    UINT64              iep_bar45_xlat;
    UINT64              iep_bar_lmt23;
    UINT64              iep_bar_lmt45; /*bar limit*/
    UINT64              eep_bar01;
    UINT64              eep_bar23;
    UINT64              eep_bar45;
    UINT64              eep_bar23_xlat;
    UINT64              eep_bar45_xlat;
    UINT64              eep_bar_lmt23; /*bar limit*/
    UINT64              eep_bar_lmt45; /*bar limit*/
} PCIE_NTB_CFG;

extern int pcie_mode_get(UINT32 Port, PCIE_PORT_INFO *port_info);

extern int pcie_port_ctrl(UINT32 Port, UINT32 port_ctrl);

extern int pcie_link_speed_set(UINT32 Port, PCIE_PORT_GEN speed);

extern int pcie_port_cfg_set(UINT32 Port, pcie_cfg_params_s *cfg_params);

extern int pcie_port_cfg_get(UINT32 Port, pcie_cfg_params_s *cfg_params);


extern int pcie_dma_chan_ctl(UINT32 Port,UINT32 channel,UINT32 control);

extern int pcie_dma_chan_attribu_set(UINT32 Port,UINT32 channel, pcie_dma_channel_attrs_s *dma_attribute);

extern int pcie_dma_cur_status_get(UINT32 Port, UINT32 channel, pcie_dma_channel_status_e *dma_channel_status);

extern int pcie_dma_int_enable(UINT32 Port, UINT32 channel, pcie_dma_int_type_e int_type);

extern int pcie_dma_int_mask(UINT32 Port, UINT32 channel, pcie_dma_int_type_e int_type);

extern int pcie_dma_tranfer_stop(UINT32 Port, UINT32 channel);


extern int pcie_dma_int_status_get(UINT32 Port, UINT32 channel, int *dma_int_status);

extern int pcie_dma_int_clear(UINT32 Port, UINT32 channel, pcie_dma_int_type_e dma_int_type);


extern int pcie_dma_read(UINT32 Port,void *source, void *dest,UINT32 transfer_size, UINT32 burst_size);

extern int pcie_dma_write(UINT32 Port,void *source, void *dest,UINT32 transfer_size, UINT32 burst_size);

extern int pcie_multicast_cfg_set(UINT32 Port,pcie_multicast_cfg_s *multicast_cfg,UINT32 win_num);

extern int pcie_setup_ntb(UINT32 Port, PCIE_NTB_CFG *ntb_cfg);

extern int pcie_ntb_doorbell_send(UINT32 Port,UINT32 doorbell);

extern int pcie_loop_test_start(UINT32 Port, UINT32 loop_type);

extern int pcie_loop_test_stop(UINT32 Port, UINT32 loop_type);

extern int pcie_loop_test_get(UINT32 Port, UINT32 loop_type, pcie_loop_test_result_s *test_result);
extern int pcie_port_reset(UINT32 Port);

extern int pcie_port_error_report_enable(UINT32 Port, UINT32 bus, UINT32 dev, UINT32 func, PCIE_ERROR_TYPE pcie_error);

extern int pcie_port_error_report_disable(UINT32 Port, UINT32 bus, UINT32 dev, UINT32 func, PCIE_ERROR_TYPE pcie_error);

extern int pcie_device_error_status_get(UINT32 Port, UINT32 bus, UINT32 dev, UINT32 func,UINT32 clear, \
pcie_device_status_u *pcie_stat);
extern int pcie_port_aer_cap_get(UINT32 Port, UINT32 bus, UINT32 dev, UINT32 func,UINT32 *aer_cap);

extern int pcie_port_aer_status_get(UINT32 Port, UINT32 bus, UINT32 dev, UINT32 func,pcie_uc_aer_status_u *pcie_aer_status);
extern int pcie_port_aer_status_clr(UINT32 Port, UINT32 bus, UINT32 dev, UINT32 func);

extern int pcie_port_aer_report_enable(UINT32 Port, PCIE_ERROR_TYPE pcie_aer_type);


extern int pcie_port_aer_report_disable(UINT32 Port, PCIE_ERROR_TYPE pcie_aer_type);


extern int pcie_cfg_read(UINT32 Port, UINT32 bus, UINT32 dev, UINT32 func, UINT32 reg_offset, UINT32 * value, UINT32 length);

extern int pcie_cfg_write(UINT32 Port, UINT32 bus, UINT32 dev, UINT32 func, UINT32 reg_offset, UINT8 * data, UINT32 length);

extern int pcie_mem_read(UINT32 Port,void * local_addr, void *pcie_mem_addr,UINT32 length);

extern int pcie_mem_write(UINT32 Port,void *local_addr , void *pcie_mem_addr,UINT32 length);

#endif
