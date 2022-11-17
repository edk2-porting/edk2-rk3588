/** @file

  Copyright (c) 2016 - 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef OGMA_API_H
#define OGMA_API_H

#include "ogma_version.h"
#include "netsec_for_uefi/ogma_config.h"
#include "netsec_for_uefi/netsec_sdk/include/ogma_basic_type.h"
#include "netsec_for_uefi/pfdep.h"
/**
 * Check configuration macro settings.
 */
#ifdef OGMA_CONFIG_CLK_HZ
#if ( (OGMA_CONFIG_CLK_HZ < 0x200000) || (OGMA_CONFIG_CLK_HZ > 0x10000000) )
#error "OGMA_CONFIG_CLK_HZ is not appropriate."
#endif /* ( (OGMA_CONFIG_CLK_HZ < 0x200000) || (OGMA_CONFIG_CLK_HZ > 0x10000000) ) */
#else /* ! OGMA_CONFIG_CLK_HZ */
#error "OGMA_CONFIG_CLK_HZ is not given."
#endif /* OGMA_CONFIG_CLK_HZ */

#ifndef OGMA_CONFIG_GMAC_CLK_HZ
#define OGMA_CONFIG_GMAC_CLK_HZ OGMA_CONFIG_CLK_HZ
#endif

/**
 * Number of hardware limits
 */


/**
 * Number of Common Descriptor ring id
 */
#define OGMA_DESC_RING_ID_NRM_TX 0
#define OGMA_DESC_RING_ID_NRM_RX 1

#define OGMA_DESC_RING_ID_GMAC   15
#define OGMA_DESC_RING_ID_MAX    1

/**
 * Numbre of TCP Segmentation length limits
 */
#define OGMA_TCP_SEG_LEN_MAX 1460
#define OGMA_TCP_JUMBO_SEG_LEN_MAX 8960

/**
 * Number of ER check result for received packet
 */
#define OGMA_RX_ER_RESULT_NG       0x1
#define OGMA_RX_ER_RESULT_OK       0x0

/**
 * Number of checksum calculation result for received packet
 */
#define OGMA_RX_CKSUM_RESULT_OK       0x1
#define OGMA_RX_CKSUM_RESULT_NG       0x2
#define OGMA_RX_CKSUM_RESULT_NOTAVAIL 0x0

/**
 * Number of ErrorCode for received packet
 */
#define OGMA_RX_ERRCODE_HEADER_INCOMLETE_ERR   0x2
#define OGMA_RX_ERRCODE_IP_HEADER_ERR          0x1
#define OGMA_RX_ERRCODE_NONE                   0x0

/**
 * Number of top interrupt enable register bit field
 */
#define OGMA_TOP_IRQ_REG_ME_START        (1UL << 20)
#define OGMA_TOP_IRQ_REG_MAC             (1UL << 19)
#define OGMA_TOP_IRQ_REG_PKT             (1UL << 18)
#define OGMA_TOP_IRQ_REG_BOOTCODE_TX     (1UL <<  5)
#define OGMA_TOP_IRQ_REG_NRM_RX          (1UL <<  1)
#define OGMA_TOP_IRQ_REG_NRM_TX          (1UL <<  0)


/**
 *  Number of top channel enable register bit field
 */
#define OGMA_CH_IRQ_REG_EMPTY   (1UL << 17)
#define OGMA_CH_IRQ_REG_ERR     (1UL << 16)
#define OGMA_CH_IRQ_REG_PKT_CNT (1UL << 15)
#define OGMA_CH_IRQ_REG_TIMEUP  (1UL << 14)
#define OGMA_CH_IRQ_REG_RCV     (OGMA_CH_IRQ_REG_PKT_CNT | OGMA_CH_IRQ_REG_TIMEUP)

/**
 *  Number of top channel enable register bit field for F_NETSEC_C
 */
#define OGMA_CH_IRQ_REG_TX_DONE (1UL << 15)
#define OGMA_CH_IRQ_REG_SND     (OGMA_CH_IRQ_REG_TX_DONE | OGMA_CH_IRQ_REG_TIMEUP)


/**
 *  Number of packet interrupt enable register bit field
 */
#define OGMA_PKT_IRQ_MAC_ER            (1UL << 5)
#define OGMA_PKT_IRQ_JUMBO_ER          (1UL << 4)
#define OGMA_PKT_IRQ_CHKSUM_ER         (1UL << 3)
#define OGMA_PKT_IRQ_HD_INCOMPLETE     (1UL << 2)
#define OGMA_PKT_IRQ_HD_ER             (1UL << 1)
#define OGMA_PKT_IRQ_DRP_NO_MATCH      (1UL << 0)


/**
 *  Number of mac irq enable register bit field
 */
#define OGMA_MAC_IRQ_INT_PMT (1UL << 31)
#define OGMA_MAC_IRQ_INT_SBD (1UL << 30)
#define OGMA_MAC_IRQ_INT_LPI (1UL << 29)
#define OGMA_MAC_IRQ_INT_MAC_TX_RX_INFO_INT (1UL << 27)
#define OGMA_MAC_IRQ_INT_LPI_TX_ENTRY (1UL << 26)
#define OGMA_MAC_IRQ_INT_LPI_RX_ENTRY (1UL << 25)
#define OGMA_MAC_IRQ_INT_LPI_TX_EXIT (1UL << 24)
#define OGMA_MAC_IRQ_INT_LPI_RX_EXIT (1UL << 23)


/**
 *  Number of SR IER register bit field
 */
#define OGMA_GMAC_INT_SBD_IRQ_SR_GLPII (1U << 30)
#define OGMA_GMAC_INT_SBD_IRQ_SR_TTI   (1U << 29)
#define OGMA_GMAC_INT_SBD_IRQ_SR_GPI   (1U << 28)
#define OGMA_GMAC_INT_SBD_IRQ_SR_GMI   (1U << 27)
#define OGMA_GMAC_INT_SBD_IRQ_SR_GLI   (1U << 26)
#define OGMA_GMAC_INT_SBD_IRQ_SR_NIS   (1U << 16)
#define OGMA_GMAC_INT_SBD_IRQ_SR_AIS   (1U << 15)
#define OGMA_GMAC_INT_SBD_IRQ_SR_ERI   (1U << 14)
#define OGMA_GMAC_INT_SBD_IRQ_SR_FBI   (1U << 13)
#define OGMA_GMAC_INT_SBD_IRQ_SR_ETI   (1U << 10)
#define OGMA_GMAC_INT_SBD_IRQ_SR_RWT   (1U << 9)
#define OGMA_GMAC_INT_SBD_IRQ_SR_RPS   (1U << 8)
#define OGMA_GMAC_INT_SBD_IRQ_SR_RU    (1U << 7)
#define OGMA_GMAC_INT_SBD_IRQ_SR_RI    (1U << 6)
#define OGMA_GMAC_INT_SBD_IRQ_SR_UNF   (1U << 5)
#define OGMA_GMAC_INT_SBD_IRQ_SR_OVF   (1U << 4)
#define OGMA_GMAC_INT_SBD_IRQ_SR_TJT   (1U << 3)
#define OGMA_GMAC_INT_SBD_IRQ_SR_TU    (1U << 2)
#define OGMA_GMAC_INT_SBD_IRQ_SR_TPS   (1U << 1)
#define OGMA_GMAC_INT_SBD_IRQ_SR_TI    (1U << 0)
#define OGMA_GMAC_INT_SBD_IRQ_SR_WC_ALL ( OGMA_GMAC_INT_SBD_IRQ_SR_NIS | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_AIS | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_ERI | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_FBI | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_ETI | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_RWT | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_RPS | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_RU  | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_RI  | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_UNF | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_OVF | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_TJT | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_TU  | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_TPS | \
                                          OGMA_GMAC_INT_SBD_IRQ_SR_TI)

#define OGMA_GMAC_INT_SBD_IRQ_SR_ALL ( OGMA_GMAC_INT_SBD_IRQ_SR_GLPII | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_TTI   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_GPI   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_GMI   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_GLI   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_NIS   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_AIS   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_ERI   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_FBI   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_ETI   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_RWT   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_RPS   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_RU    | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_RI    | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_UNF   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_OVF   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_TJT   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_TU    | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_TPS   | \
                                       OGMA_GMAC_INT_SBD_IRQ_SR_TI)

#define OGMA_GMAC_INT_SBD_IRQ_IER_ALL ( OGMA_GMAC_INT_SBD_IRQ_SR_NIS | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_AIS | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_ERI | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_FBI | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_ETI | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_RWT | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_RPS | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_RU  | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_RI  | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_UNF | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_OVF | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_TJT | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_TU  | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_TPS | \
                                        OGMA_GMAC_INT_SBD_IRQ_SR_TI)

/**
 *  Number of ISR IMR register bit field
 */
#define OGMA_GMAC_INT_SBD_IRQ_ISR_LPII (1U << 10)
#define OGMA_GMAC_INT_SBD_IRQ_ISR_TSI  (1U << 9)
#define OGMA_GMAC_INT_SBD_IRQ_ISR_COI  (1U << 7)
#define OGMA_GMAC_INT_SBD_IRQ_ISR_TI   (1U << 6)
#define OGMA_GMAC_INT_SBD_IRQ_ISR_RI   (1U << 5)
#define OGMA_GMAC_INT_SBD_IRQ_ISR_MI   (1U << 4)
#define OGMA_GMAC_INT_SBD_IRQ_ISR_PI   (1U << 3)
#define OGMA_GMAC_INT_SBD_IRQ_ISR_RGI  (1U << 0)
#define OGMA_GMAC_INT_SBD_IRQ_ISR_ALL ( OGMA_GMAC_INT_SBD_IRQ_ISR_LPII | \
                                        OGMA_GMAC_INT_SBD_IRQ_ISR_TSI  | \
                                        OGMA_GMAC_INT_SBD_IRQ_ISR_COI  | \
                                        OGMA_GMAC_INT_SBD_IRQ_ISR_TI   | \
                                        OGMA_GMAC_INT_SBD_IRQ_ISR_RI   | \
                                        OGMA_GMAC_INT_SBD_IRQ_ISR_MI   | \
                                        OGMA_GMAC_INT_SBD_IRQ_ISR_PI   | \
                                        OGMA_GMAC_INT_SBD_IRQ_ISR_RGI)

/**
 *  Number of LPICSR register bit field
 */
#define OGMA_GMAC_LPICSR_REG_LPITXA (1U << 19)
#define OGMA_GMAC_LPICSR_REG_PLSEN  (1U << 18)
#define OGMA_GMAC_LPICSR_REG_PLS    (1U << 17)
#define OGMA_GMAC_LPICSR_REG_LPIEN  (1U << 16)
#define OGMA_GMAC_LPICSR_REG_RLPIST (1U << 9)
#define OGMA_GMAC_LPICSR_REG_TLPIST (1U << 8)
#define OGMA_GMAC_LPICSR_REG_RLPIEX (1U << 3)
#define OGMA_GMAC_LPICSR_REG_RLPIEN (1U << 2)
#define OGMA_GMAC_LPICSR_REG_TLPIEX (1U << 1)
#define OGMA_GMAC_LPICSR_REG_TLPIEN (1U << 0)

/**
 *  Number of RGSR register bit field
 */
#define OGMA_GMAC_RGSR_REG_LS  (1U << 3)
#define OGMA_GMAC_RGSR_REG_LSP (1U << 1)
#define OGMA_GMAC_RGSR_REG_LM  (1U << 0)

/**
 * Number of various limits
 */
#define OGMA_DESC_ENTRY_NUM_MIN        2
#define OGMA_DESC_ENTRY_NUM_MAX        2047
#define OGMA_INT_PKTCNT_MAX            2047
#define OGMA_L4_MIN_LEN_MAX            64

/**
 * Number of ogma phy interface setting
 */
#define OGMA_PHY_INTERFACE_GMII  0
#define OGMA_PHY_INTERFACE_RGMII 1
#define OGMA_PHY_INTERFACE_RMII  4

/**
 * Number of ogma link speed setting
 */
#define OGMA_PHY_LINK_SPEED_1G          0
#define OGMA_PHY_LINK_SPEED_100M        1U
#define OGMA_PHY_LINK_SPEED_10M         2U
#define OGMA_PHY_LINK_SPEED_100M_OR_10M 3U

/**
 * Number of flow control limits
 */
#define OGMA_FLOW_CTRL_START_THRESHOLD_MAX 95
#define OGMA_FLOW_CTRL_STOP_THRESHOLD_MAX  95
#define OGMA_FLOW_CTRL_PAUSE_TIME_MIN      5

enum ogma_err_e{
    OGMA_ERR_OK = 0,
    OGMA_ERR_PARAM,
    OGMA_ERR_ALLOC,
    OGMA_ERR_BUSY,
    OGMA_ERR_RANGE,
    OGMA_ERR_DATA,
    OGMA_ERR_NOTAVAIL,
    OGMA_ERR_INTERRUPT,
    OGMA_ERR_AGAIN,
    OGMA_ERR_INVALID,
};

typedef void *ogma_handle_t;
typedef struct ogma_param_s ogma_param_t;
typedef struct ogma_pkt_ctrl_param_s ogma_pkt_ctrl_param_t;
typedef struct ogma_desc_ring_param_s ogma_desc_ring_param_t;
typedef enum ogma_err_e ogma_err_t;
typedef ogma_uint8 ogma_desc_ring_id_t;
typedef struct ogma_tx_pkt_ctrl_s ogma_tx_pkt_ctrl_t;
typedef struct ogma_rx_pkt_info_s ogma_rx_pkt_info_t;
typedef struct ogma_frag_info_s ogma_frag_info_t;
typedef struct ogma_gmac_config_s ogma_gmac_config_t;
typedef struct ogma_gmac_mode_s ogma_gmac_mode_t;

struct ogma_gmac_config_s{
    ogma_uint8 phy_interface;
};

struct ogma_pkt_ctrl_param_s{
    ogma_uint log_chksum_er_flag:1;
    ogma_uint log_hd_imcomplete_flag:1;
    ogma_uint log_hd_er_flag:1;
    ogma_uint drp_no_match_flag:1;
};

struct ogma_desc_ring_param_s{
    ogma_uint valid_flag:1;
    ogma_uint little_endian_flag:1;
    ogma_uint tmr_mode_flag:1;
    ogma_uint16 entry_num;
};

struct ogma_param_s{

    ogma_uint use_gmac_flag:1;
    ogma_uint use_jumbo_pkt_flag:1;
    ogma_pkt_ctrl_param_t pkt_ctrl_param;
    ogma_desc_ring_param_t desc_ring_param[OGMA_DESC_RING_ID_MAX+1];
    ogma_gmac_config_t gmac_config;
    ogma_uint8 mac_addr[6];
    ogma_uint8 phy_addr;
};

struct ogma_tx_pkt_ctrl_s{
    ogma_uint pass_through_flag:1;
    ogma_uint cksum_offload_flag:1;
    ogma_uint tcp_seg_offload_flag:1;
    ogma_desc_ring_id_t target_desc_ring_id;
    ogma_uint16 tcp_seg_len;
};

struct ogma_rx_pkt_info_s{
    ogma_uint fragmented_flag:1;
    ogma_uint err_flag:1;
    ogma_uint rx_cksum_result:2;
    ogma_uint8 err_code;
};

struct ogma_frag_info_s{
    pfdep_phys_addr_t phys_addr;
    void *addr;
    ogma_uint32 len;
};

struct ogma_gmac_mode_s{
    ogma_uint half_duplex_flag:1;
    ogma_uint flow_ctrl_enable_flag:1;
    ogma_uint8 link_speed;
    ogma_uint16 flow_ctrl_start_threshold;
    ogma_uint16 flow_ctrl_stop_threshold;
    ogma_uint16 pause_time;
};

#ifdef OGMA_CONFIG_REC_STAT
typedef struct ogma_stat_info_s {
    ogma_uint16 current_busy_entry_num[OGMA_DESC_RING_ID_MAX + 1];
    ogma_uint16 max_busy_entry_num[OGMA_DESC_RING_ID_MAX + 1];
} ogma_stat_info_t;
#endif /* OGMA_CONFIG_REC_STAT */

typedef struct ogma_gmac_int_sbd_regs_s{
    ogma_uint32 base;
    ogma_uint32 extended;
} ogma_gmac_int_sbd_regs_t;

typedef struct ogma_phy_link_status_s{
    ogma_uint up_flag:1;
    ogma_uint auto_nego_enable_flag:1;
    ogma_uint auto_nego_complete_flag:1;
    ogma_uint half_duplex_flag:1;
    ogma_uint latched_link_down_flag:1;
    ogma_uint lpi_capable_flag:1;
    ogma_uint8 link_speed;
} ogma_phy_link_status_t;

/**************************
***************************
***************************/

ogma_err_t ogma_init (
    void *base_addr,
    pfdep_dev_handle_t dev_handle,
    const ogma_param_t *param_p,
    const void *dma_hm_mc_addr,
    ogma_uint32 dma_hm_mc_len,
    const void *dma_mh_mc_addr,
    ogma_uint32 dma_mh_mc_len,
    const void *pktc_mc_addr,
    ogma_uint32 pktc_mc_len,
    ogma_handle_t *ogma_handle_p
    );

void ogma_terminate (
    ogma_handle_t ogma_handle
    );

ogma_err_t ogma_start_gmac (
    ogma_handle_t ogma_handle,
    ogma_bool rx_flag,
    ogma_bool tx_flag
    );

ogma_err_t ogma_stop_gmac (
    ogma_handle_t ogma_handle,
    ogma_bool rx_flag,
    ogma_bool tx_flag
    );

ogma_err_t ogma_set_gmac_mode (
    ogma_handle_t ogma_handle,
    const ogma_gmac_mode_t *gmac_mode_p
    );

void ogma_set_phy_reg (
    ogma_handle_t ogma_handle,
    ogma_uint8 reg_addr,
    ogma_uint16 value
    );

ogma_uint16 ogma_get_phy_reg (
    ogma_handle_t ogma_handle,
    ogma_uint8 reg_addr
    );

ogma_err_t ogma_get_gmac_status (
    ogma_handle_t ogma_handle,
    ogma_bool *valid_flag_p,
    ogma_gmac_mode_t *gmac_mode_p,
    ogma_bool *rx_running_flag_p,
    ogma_bool *tx_running_flag_p
    );

ogma_uint32 ogma_get_top_irq_enable (
    ogma_handle_t ogma_handle
    );

ogma_uint32 ogma_get_top_irq_status (
    ogma_handle_t ogma_handle,
    ogma_bool mask_flag
    );

#define ogma_get_top_irq_status_non_clear(ogma_handle,mask_flag) \
ogma_get_top_irq_status(ogma_handle,mask_flag)

ogma_err_t ogma_clear_top_irq_status (
    ogma_handle_t ogma_handle,
    ogma_uint32 value
    );

ogma_uint32 ogma_get_desc_ring_irq_enable (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id
    );

ogma_uint32 ogma_get_desc_ring_irq_status (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_bool mask_flag
    );

#define ogma_get_desc_ring_irq_status_non_clear(ogma_handle,ring_id,mask_flag) \
ogma_get_desc_ring_irq_status(ogma_handle,ring_id,mask_flag)



ogma_err_t ogma_clear_desc_ring_irq_status (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_uint32 value
    );

ogma_uint32 ogma_get_pkt_irq_enable (
    ogma_handle_t ogma_handle
    );

ogma_uint32 ogma_get_pkt_irq_status (
    ogma_handle_t ogma_handle,
    ogma_bool mask_flag
    );

#define ogma_get_pkt_irq_status_non_clear(ogma_handle,mask_flag) \
ogma_get_pkt_irq_status(ogma_handle,mask_flag)


ogma_err_t ogma_clear_pkt_irq_status (
    ogma_handle_t ogma_handle,
    ogma_uint32 value
    );

/* ogma_desc_ring_access.c */
ogma_err_t ogma_start_desc_ring (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id
    );

ogma_err_t ogma_stop_desc_ring (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id
    );

ogma_uint16 ogma_get_rx_num (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id
    );

ogma_uint16 ogma_get_tx_avail_num (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id
    );

ogma_err_t ogma_clean_tx_desc_ring(
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id
    );

ogma_err_t ogma_clean_rx_desc_ring(
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id
    );

ogma_err_t ogma_set_tx_pkt_data (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    const ogma_tx_pkt_ctrl_t *tx_pkt_ctrl_p,
    ogma_uint8 scat_num,
    const ogma_frag_info_t *scat_info_p,
    pfdep_pkt_handle_t pkt_handle
    );

ogma_err_t ogma_get_rx_pkt_data (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_rx_pkt_info_t *rx_pkt_info_p,
    ogma_frag_info_t *frag_info_p,
    ogma_uint16 *len_p,
    pfdep_pkt_handle_t *pkt_handle_p
    );

ogma_err_t ogma_enable_top_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    );

ogma_err_t ogma_disable_top_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    );

ogma_err_t ogma_enable_desc_ring_irq (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_uint32 irq_factor
    );

ogma_err_t ogma_disable_desc_ring_irq (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_uint32 irq_factor
    );

ogma_err_t ogma_enable_pkt_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    );

ogma_err_t ogma_disable_pkt_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    );

ogma_uint32 ogma_get_hw_ver (
    ogma_handle_t ogma_handle
    );

ogma_uint32 ogma_get_mcr_ver (
    ogma_handle_t ogma_handle
    );

/**
 * Set up IRQ coalesce parameters.
 *
 * [Note]
 *  - This is a tentative implementation.
 *    Not tested enough. Use with care.
 *
 *  - Call this function after every invocation of ogma_start_desc_ring()
 *    because ogma_start_desc_ring() resets IRQ coalesce settings.
 *
 */
ogma_err_t ogma_set_irq_coalesce_param (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_uint16 int_pktcnt,
    ogma_bool int_tmr_unit_ms_flag,
    ogma_uint16 int_tmr_cnt
    );

ogma_uint32 ogma_get_mac_irq_enable (
    ogma_handle_t ogma_handle
    );


ogma_uint32 ogma_get_mac_irq_status (
    ogma_handle_t ogma_handle,
    ogma_bool mask_flag
    );

#define ogma_get_mac_irq_status_non_clear(ogma_handle,mask_flag) \
ogma_get_mac_irq_status(ogma_handle,mask_flag)


ogma_err_t ogma_clear_mac_irq_status (
    ogma_handle_t ogma_handle,
    ogma_uint32 value
    );

ogma_err_t ogma_enable_mac_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    );

ogma_err_t ogma_disable_mac_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    );

#ifdef OGMA_CONFIG_REC_STAT
/**
 * Get statistics information.
 */
ogma_err_t ogma_get_stat_info (
    ogma_handle_t ogma_handle,
    ogma_stat_info_t *stat_info_p,
    ogma_bool clear_flag
    );
#endif /* OGMA_CONFIG_REC_STAT */

ogma_err_t ogma_set_gmac_lpictrl_reg (
    ogma_handle_t ogma_handle,
    ogma_uint32 value
    );

ogma_err_t ogma_get_gmac_lpictrl_reg (
    ogma_handle_t ogma_handle,
    ogma_uint32 *value_p
    );

ogma_err_t ogma_set_gmac_lpitimer_reg (
    ogma_handle_t ogma_handle,
    ogma_uint16 ls_timer_ms,
    ogma_uint16 tw_timer_ms
    );

ogma_err_t ogma_get_gmac_lpitimer_reg (
    ogma_handle_t ogma_handle,
    ogma_uint16 *ls_timer_ms_p,
    ogma_uint16 *tw_timer_ms_p
    );

void ogma_set_phy_mmd_reg (
    ogma_handle_t ogma_handle,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr,
    ogma_uint16 value
    );

ogma_uint16 ogma_get_phy_mmd_reg (
    ogma_handle_t ogma_handle,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr
    );

ogma_err_t ogma_get_phy_link_status (
    ogma_handle_t ogma_handle,
    ogma_phy_link_status_t *phy_link_status_p
    );

ogma_gmac_int_sbd_regs_t ogma_get_gmac_int_sbd_irq_enable (
    ogma_handle_t ogma_handle
    );

ogma_gmac_int_sbd_regs_t ogma_get_gmac_int_sbd_irq_status (
    ogma_handle_t ogma_handle,
    ogma_bool mask_flag
    );

#define ogma_get_gmac_int_sbd_irq_status_non_clear(ogma_handle,mask_flag) \
ogma_get_gmac_int_sbd_irq_status(ogma_handle,mask_flag)

ogma_err_t ogma_clear_gmac_int_sbd_irq_status (
    ogma_handle_t ogma_handle,
    ogma_gmac_int_sbd_regs_t int_sbd_regs
    );

ogma_err_t ogma_enable_gmac_int_sbd_irq (
    ogma_handle_t ogma_handle,
    ogma_gmac_int_sbd_regs_t int_sbd_regs
    );

ogma_err_t ogma_disable_gmac_int_sbd_irq (
    ogma_handle_t ogma_handle,
    ogma_gmac_int_sbd_regs_t int_sbd_regs
    );

ogma_err_t ogma_get_gmac_rgmii_status_reg (
    ogma_handle_t ogma_handle,
    ogma_uint32 *value_p
    );

#ifdef OGMA_CONFIG_USE_READ_GMAC_STAT
ogma_err_t ogma_read_gmac_stat (
    ogma_handle_t ogma_handle,
    ogma_uint32 *value_p,
    ogma_bool reset_flag
    );
#endif /* OGMA_CONFIG_USE_READ_GMAC_STAT */

ogma_err_t ogma_reset_gmac_stat (
    ogma_handle_t ogma_handle
    );

/**************************
***************************
***************************/

#endif /* OGMA_API_H*/
