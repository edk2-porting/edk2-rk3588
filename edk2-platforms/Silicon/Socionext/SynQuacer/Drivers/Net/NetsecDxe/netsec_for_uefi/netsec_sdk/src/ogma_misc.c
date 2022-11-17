/** @file

  Copyright (c) 2016 - 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "ogma_config.h"
#include "ogma_internal.h"
#include "ogma_basic_access.h"
#include "ogma_misc_internal.h"

#include <Library/IoLib.h>

ogma_global_t ogma_global = {
    OGMA_FALSE,
    0,
    NULL
};


static const ogma_uint32 hw_ver_reg_addr = OGMA_REG_ADDR_HW_VER;
static const ogma_uint32 mc_ver_reg_addr = OGMA_REG_ADDR_MC_VER;

static const ogma_uint32 desc_ring_irq_status_reg_addr[OGMA_DESC_RING_ID_MAX + 1] = {
    OGMA_REG_ADDR_NRM_TX_STATUS,
    OGMA_REG_ADDR_NRM_RX_STATUS,
};

static const ogma_uint32 desc_ring_config_reg_addr[OGMA_DESC_RING_ID_MAX + 1] = {
    OGMA_REG_ADDR_NRM_TX_CONFIG,
    OGMA_REG_ADDR_NRM_RX_CONFIG,

};


/* Internal function definition*/
#ifndef OGMA_CONFIG_DISABLE_CLK_CTRL
STATIC void ogma_set_clk_en_reg (
    ogma_ctrl_t *ctrl_p
    );
#endif /* OGMA_CONFIG_DISABLE_CLK_CTRL */
STATIC void ogma_global_init ( void);

STATIC ogma_err_t ogma_probe_hardware (
    void *base_addr
    );

STATIC void ogma_reset_hardware (
    ogma_ctrl_t *ctrl_p
    );

STATIC void ogma_set_microcode(
    ogma_ctrl_t *ctrl_p,
    const void *dma_hm_mc_addr,
    ogma_uint32 dma_hm_mc_len,
    const void *dma_mh_mc_addr,
    ogma_uint32 dma_mh_mc_len,
    const void *pktc_mc_addr,
    ogma_uint32 pktc_mc_len
    );

STATIC ogma_uint32 ogma_calc_pkt_ctrl_reg_param (
    const ogma_pkt_ctrl_param_t *pkt_ctrl_param_p
    );

STATIC void ogma_internal_terminate (
    ogma_ctrl_t *ctrl_p
    );

#ifdef OGMA_CONFIG_DISABLE_CLK_CTRL

#define ogma_set_clk_en_reg( ctrl_p)

#else /* OGMA_CONFIG_DISABLE_CLK_CTRL */
STATIC void ogma_set_clk_en_reg (
    ogma_ctrl_t *ctrl_p
    )
{
    ogma_uint32 value = 0;

    if ( ctrl_p->clk_ctrl.mac_req_num != 0) {
        value |= OGMA_CLK_EN_REG_DOM_G;
    }

    ogma_write_reg( ctrl_p, OGMA_REG_ADDR_CLK_EN, value);
}
#endif /* OGMA_CONFIG_DISABLE_CLK_CTRL */

void ogma_push_clk_req (
    ogma_ctrl_t *ctrl_p,
    ogma_uint32 domain
    )
{
    pfdep_hard_lock_ctx_t clk_ctrl_hard_lock_ctx;

    pfdep_acquire_hard_lock (
        &ctrl_p->clk_ctrl_hard_lock,
        &clk_ctrl_hard_lock_ctx);

    if ( ( domain & OGMA_CLK_EN_REG_DOM_G) != 0) {
        ++ctrl_p->clk_ctrl.mac_req_num;
    }

    ogma_set_clk_en_reg( ctrl_p);

    pfdep_release_hard_lock(
        &ctrl_p->clk_ctrl_hard_lock,
        &clk_ctrl_hard_lock_ctx);
}

void ogma_pop_clk_req(
    ogma_ctrl_t *ctrl_p,
    ogma_uint32 domain
    )
{
    pfdep_hard_lock_ctx_t clk_ctrl_hard_lock_ctx;

    pfdep_acquire_hard_lock(
        &ctrl_p->clk_ctrl_hard_lock,
        &clk_ctrl_hard_lock_ctx);

    if ( ( domain & OGMA_CLK_EN_REG_DOM_G) != 0) {
        --ctrl_p->clk_ctrl.mac_req_num;
    }

    ogma_set_clk_en_reg( ctrl_p);

    pfdep_release_hard_lock(
        &ctrl_p->clk_ctrl_hard_lock,
        &clk_ctrl_hard_lock_ctx);
}

/* Internal function */
STATIC void ogma_global_init ( void)
{
    ogma_global.valid_flag = OGMA_TRUE;
}

STATIC ogma_err_t ogma_probe_hardware (
    void *base_addr
    )
{

    ogma_uint32 value;

    /* Read HW_VER Register */
    value = pfdep_iomem_read((void *)
                             ((pfdep_cpu_addr_t)base_addr +
                              (OGMA_REG_ADDR_HW_VER << 2)));

    if ( value != OGMA_VER_NETSEC) {
        pfdep_print(PFDEP_DEBUG_LEVEL_WARNING,
                    "Hardware version check warning. Actual:0x%08x, Expected:0x%08x\n",
                    (unsigned int)value,
                    (unsigned int)OGMA_VER_NETSEC);
    }

    if ( OGMA_VER_MAJOR_NUM(value) != OGMA_VER_MAJOR_NUM(OGMA_VER_NETSEC) ) {
        pfdep_print(PFDEP_DEBUG_LEVEL_FATAL,
                    "Hardware Major version check failed. Actual:0x%08x, Expected:0x%08x\n",
                    (unsigned int)OGMA_VER_MAJOR_NUM(value),
                    (unsigned int)OGMA_VER_MAJOR_NUM(OGMA_VER_NETSEC) );
        return OGMA_ERR_NOTAVAIL;
    }

    /* Print hardware version information. */
    pfdep_print(PFDEP_DEBUG_LEVEL_NOTICE,
                "NETSEC found. Hardware version: %08x\n",
                value);


    return OGMA_ERR_OK;
}

STATIC void ogma_reset_hardware (
    ogma_ctrl_t *ctrl_p
    )
{
    ogma_write_reg( ctrl_p,
                    OGMA_REG_ADDR_CLK_EN,
                    OGMA_CLK_EN_REG_DOM_ALL);


    /*
     * Stop dma engines if cores are enabled
     */
    if (ogma_read_reg(ctrl_p, OGMA_REG_ADDR_DIS_CORE) == 0) {

        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_DMA_HM_CTRL,
                        OGMA_DMA_CTRL_REG_STOP);

        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_DMA_MH_CTRL,
                        OGMA_DMA_CTRL_REG_STOP);

        while ( ( ogma_read_reg( ctrl_p, OGMA_REG_ADDR_DMA_HM_CTRL)
                  & OGMA_DMA_CTRL_REG_STOP) != 0) {
            ;
        }

        while ( ( ogma_read_reg( ctrl_p, OGMA_REG_ADDR_DMA_MH_CTRL)
                  & OGMA_DMA_CTRL_REG_STOP) != 0) {
            ;
        }
    }

    if ( ctrl_p->param.use_gmac_flag) {

        /* Reset F_GMAC4MT */
        ogma_set_mac_reg( ctrl_p,
                          OGMA_GMAC_REG_ADDR_BMR,
                          OGMA_GMAC_BMR_REG_RESET);

    }

    ogma_write_reg( ctrl_p,
                    OGMA_REG_ADDR_SOFT_RST,
                    OGMA_SOFT_RST_REG_RESET);

    ogma_write_reg( ctrl_p,
                    OGMA_REG_ADDR_SOFT_RST,
                    OGMA_SOFT_RST_REG_RUN);

    ogma_write_reg( ctrl_p,
                    OGMA_REG_ADDR_COM_INIT,
                    OGMA_COM_INIT_REG_ALL);

    while (ogma_read_reg(ctrl_p, OGMA_REG_ADDR_COM_INIT) != 0) {
        ;
    }

    if ( ctrl_p->param.use_gmac_flag) {


        /* MAC desc init */
        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_MAC_DESC_INIT,
                        OGMA_MAC_DESC_INIT_REG_INIT);

        /* Wait MAC desc init done */
        while ( ( ogma_read_reg( ctrl_p, OGMA_REG_ADDR_MAC_DESC_INIT)
                  & OGMA_MAC_DESC_INIT_REG_INIT) != 0) {
            ;
        }


        /* set MAC_INTF_SEL */
        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_MAC_INTF_SEL,
                        ctrl_p->param.gmac_config.phy_interface);


    }
}

#define OGMA_ROUND_UP(numerator,denominator) (((numerator) + (denominator)) - 1 / (denominator))

STATIC void ogma_set_microcode (
    ogma_ctrl_t *ctrl_p,
    const void *dma_hm_mc_addr,
    ogma_uint32 dma_hm_mc_len,
    const void *dma_mh_mc_addr,
    ogma_uint32 dma_mh_mc_len,
    const void *pktc_mc_addr,
    ogma_uint32 pktc_mc_len
    )
{
    ogma_uint i;

    const UINT32 *dmac_hm_cmd_data = (const UINT32 *)dma_hm_mc_addr;
    const UINT32 *dmac_mh_cmd_data = (const UINT32 *)dma_mh_mc_addr;
    const UINT32 *core_cmd_data = (const UINT32 *)pktc_mc_addr;

    /* Loads microcodes to microengines. */
    for( i = 0; i < dma_hm_mc_len; i++) {
        UINT32 data = MmioRead32((UINTN)dmac_hm_cmd_data);
        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_DMAC_HM_CMD_BUF,
                        data );
        dmac_hm_cmd_data++;
    }

    for( i = 0; i < dma_mh_mc_len; i++) {
        UINT32 data = MmioRead32((UINTN)dmac_mh_cmd_data);
        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_DMAC_MH_CMD_BUF,
                        data );
        dmac_mh_cmd_data++;
    }

    for( i = 0; i < pktc_mc_len; i++) {
        UINT32 data = MmioRead32((UINTN)core_cmd_data);
        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_PKTC_CMD_BUF,
                        data );
        core_cmd_data++;
    }

}

STATIC ogma_uint32 ogma_calc_pkt_ctrl_reg_param (
    const ogma_pkt_ctrl_param_t *pkt_ctrl_param_p
    )
{
    ogma_uint32 param = 0;

    if ( pkt_ctrl_param_p->log_chksum_er_flag) {
        param |= OGMA_PKT_CTRL_REG_LOG_CHKSUM_ER;
    }

    if ( pkt_ctrl_param_p->log_hd_imcomplete_flag) {
        param |= OGMA_PKT_CTRL_REG_LOG_HD_INCOMPLETE;
    }

    if ( pkt_ctrl_param_p->log_hd_er_flag) {
        param |= OGMA_PKT_CTRL_REG_LOG_HD_ER;
    }

    if ( pkt_ctrl_param_p->drp_no_match_flag) {
        param |= OGMA_PKT_CTRL_REG_DRP_NO_MATCH;
    }

    return param;
}

STATIC
void
ogma_pre_init_microengine (
  ogma_handle_t ogma_handle
  )
{
  UINT16 Data;

  /* Remove dormant settings */
  Data = ogma_get_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL) &
         ~((1U << OGMA_PHY_CONTROL_REG_POWER_DOWN) |
         (1U << OGMA_PHY_CONTROL_REG_ISOLATE));

  ogma_set_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL, Data);

  while ((ogma_get_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL) &
          ((1U << OGMA_PHY_CONTROL_REG_POWER_DOWN) |
           (1U << OGMA_PHY_CONTROL_REG_ISOLATE))) != 0);

  /* Put phy in loopback mode to guarantee RXCLK input */
  Data |= (1U << OGMA_PHY_CONTROL_REG_LOOPBACK);

  ogma_set_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL, Data);

  while ((ogma_get_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL) &
          (1U << OGMA_PHY_CONTROL_REG_LOOPBACK)) == 0);
}

STATIC
void
ogma_post_init_microengine (
  IN ogma_handle_t ogma_handle
  )
{
  UINT16 Data;

  /* Get phy back to normal operation */
  Data = ogma_get_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL) &
         ~(1U << OGMA_PHY_CONTROL_REG_LOOPBACK);

  ogma_set_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL, Data);

  while ((ogma_get_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL) &
          (1U << OGMA_PHY_CONTROL_REG_LOOPBACK)) != 0);

  Data |= (1U << OGMA_PHY_CONTROL_REG_RESET);

  /* Apply software reset */
  ogma_set_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL, Data);

  while ((ogma_get_phy_reg (ogma_handle, OGMA_PHY_REG_ADDR_CONTROL) &
          (1U << OGMA_PHY_CONTROL_REG_RESET)) != 0);
}

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
    )
{
    ogma_int i;
    ogma_uint32 domain = 0, value;
    ogma_err_t ogma_err;
    ogma_ctrl_t *ctrl_p = NULL;

    ogma_bool inten_reg_hard_lock_init_flag = OGMA_FALSE;
    ogma_bool clk_ctrl_hard_lock_init_flag = OGMA_FALSE;

    ogma_bool all_lock_init_done_flag = OGMA_FALSE;

    pfdep_err_t pfdep_err;

    if ( ( param_p == NULL) ||
         ( ogma_handle_p == NULL) ) {
        return OGMA_ERR_PARAM;
    }

    if ( ogma_global.list_entry_num + 1 > OGMA_INSTANCE_NUM_MAX) {
        return OGMA_ERR_INVALID;
    }

    if ((! param_p->desc_ring_param[OGMA_DESC_RING_ID_NRM_TX].valid_flag) &&
        (! param_p->desc_ring_param[OGMA_DESC_RING_ID_NRM_RX].valid_flag)) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_init.\n"
                     "Please set invalid packet desc_ring_param valid_flag.\n");
        return OGMA_ERR_DATA;
    }

    if ( param_p->use_gmac_flag) {
        if ( ( param_p->gmac_config.phy_interface !=
              OGMA_PHY_INTERFACE_GMII) &&
            ( param_p->gmac_config.phy_interface !=
              OGMA_PHY_INTERFACE_RGMII) &&
            ( param_p->gmac_config.phy_interface !=
              OGMA_PHY_INTERFACE_RMII) ) {
            pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                         "An error occurred at ogma_init.\n"
                         "Please set phy_interface to valid value.\n");
            return OGMA_ERR_DATA;
        }
    } else {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_init.\n"
                     "Please set use_gmac_flag OGMA_TRUE.\n");
        return OGMA_ERR_DATA;
    }

    if ( param_p->phy_addr >= 32) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "Error: phy_addr out of range\n");
        return OGMA_ERR_DATA;
    }

    ogma_err = ogma_probe_hardware( base_addr);

    if ( ogma_err != OGMA_ERR_OK) {
        return ogma_err;
    }

    if ( !ogma_global.valid_flag) {
        ogma_global_init();
    }

    if ( ( ctrl_p = pfdep_malloc( sizeof( ogma_ctrl_t) ) ) == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_init.\n"
                     "Failed to ogma_handle memory allocation.\n");
        return OGMA_ERR_ALLOC;
    }

    pfdep_memset( ctrl_p, 0, sizeof( ogma_ctrl_t) );

    ctrl_p->base_addr = base_addr;

    ctrl_p->dev_handle = dev_handle;

    pfdep_memcpy( ( void *)&ctrl_p->param,
                  ( void *)param_p,
                  sizeof( ogma_param_t) );

    /* Initialize hardware lock */
    pfdep_err = pfdep_init_hard_lock( &ctrl_p->inten_reg_hard_lock);
    if ( pfdep_err != PFDEP_ERR_OK) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_init.\n"
                     "Failed to inten_reg_hard_lock's initialization.\n");
       ogma_err = OGMA_ERR_ALLOC;
       goto err;
    }
    inten_reg_hard_lock_init_flag = OGMA_TRUE;

    pfdep_err = pfdep_init_hard_lock( &ctrl_p->clk_ctrl_hard_lock);
    if ( pfdep_err != PFDEP_ERR_OK) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_init.\n"
                     "Failed to clk_ctrl_hard_lock's initialization.\n");
        ogma_err = OGMA_ERR_ALLOC;
        goto err;
    }
    clk_ctrl_hard_lock_init_flag = OGMA_TRUE;

    all_lock_init_done_flag = OGMA_TRUE;

    pfdep_err = pfdep_dma_malloc( dev_handle,
                                  OGMA_DUMMY_DESC_ENTRY_LEN,
                                  &ctrl_p->dummy_desc_entry_addr,
                                  &ctrl_p->dummy_desc_entry_phys_addr);

    if ( pfdep_err != PFDEP_ERR_OK) {
        ogma_err = OGMA_ERR_ALLOC;
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_init.\n"
                     "Failed to dummy_desc_entry's memory allocation.\n");
        goto err;
    }

    /* clear dummy desc entry */
    pfdep_memset( ctrl_p->dummy_desc_entry_addr,
                  0,
                  OGMA_DUMMY_DESC_ENTRY_LEN);

    ogma_reset_hardware( ctrl_p);

    if ( param_p->use_gmac_flag) {
        domain |= OGMA_CLK_EN_REG_DOM_G;
    }

    ogma_push_clk_req( ctrl_p, domain);

    /* set PKT_CTRL */
    value = ogma_calc_pkt_ctrl_reg_param( &param_p->pkt_ctrl_param);

    ogma_write_reg( ctrl_p,
                    OGMA_REG_ADDR_PKT_CTRL,
                    value);

    if ( param_p->use_jumbo_pkt_flag) {

        ctrl_p->rx_pkt_buf_len = OGMA_RX_JUMBO_PKT_BUF_LEN;

        value = ogma_read_reg( ctrl_p,
                               OGMA_REG_ADDR_PKT_CTRL);

        value |= OGMA_PKT_CTRL_REG_EN_JUMBO;

        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_PKT_CTRL,
                        value);
    } else {

        ctrl_p->rx_pkt_buf_len = OGMA_RX_PKT_BUF_LEN;

    }

    /* set pkt desc ring config */
    for ( i = 0; i <= OGMA_DESC_RING_ID_MAX; i++) {
        if (  ctrl_p->param.desc_ring_param[i].valid_flag) {
            value =
                ( ctrl_p->param.desc_ring_param[i].tmr_mode_flag <<
                  OGMA_REG_DESC_RING_CONFIG_TMR_MODE) |
                ( ctrl_p->param.desc_ring_param[i].little_endian_flag <<
                  OGMA_REG_DESC_RING_CONFIG_DAT_ENDIAN) ;

            ogma_write_reg( ctrl_p,
                            desc_ring_config_reg_addr[i],
                            value);

        }
    }

    /* set microengines */
    ogma_set_microcode(ctrl_p,
                       dma_hm_mc_addr,
                       dma_hm_mc_len,
                       dma_mh_mc_addr,
                       dma_mh_mc_len,
                       pktc_mc_addr,
                       pktc_mc_len);

    /* alloc desc_ring*/
    for( i = 0; i <= OGMA_DESC_RING_ID_MAX; i++) {
        ogma_err = ogma_alloc_desc_ring( ctrl_p, (ogma_desc_ring_id_t)i);
        if ( ogma_err != OGMA_ERR_OK) {
            pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                         "An error occurred at ogma_init.\n"
                         "Failed to ring id NO.%d memory allocation.\n",
                         i);
            goto err;
        }
    }

    if ( param_p->desc_ring_param[OGMA_DESC_RING_ID_NRM_RX].valid_flag) {
        if ( ( ogma_err = ogma_setup_rx_desc_ring(
                   ctrl_p,
                   &ctrl_p->desc_ring[OGMA_DESC_RING_ID_NRM_RX] ) )
             != OGMA_ERR_OK) {
            pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                         "An error occurred at ogma_init.\n"
                         "Failed to NRM_RX packet memory allocation.\n");
            goto err;
        }
    }

    /* microengines need domain G */
    ogma_push_clk_req( ctrl_p, OGMA_CLK_EN_REG_DOM_G);

    /* set DMA tmr ctrl*/
    ogma_write_reg( ctrl_p, OGMA_REG_ADDR_DMA_TMR_CTRL,
                    ( ogma_uint32)( ( OGMA_CONFIG_CLK_HZ / OGMA_CLK_MHZ) - 1) );

    /*
     * Do pre-initialization tasks for microengine
     *
     * In particular, we put phy in loopback mode
     * in order to make sure RXCLK keeps provided to mac
     * irrespective of phy link status,
     * which is required for microengine intialization.
     * This will be disabled once microengine initialization complete.
     */
    ogma_pre_init_microengine (ctrl_p);

    /* start microengines */
    ogma_write_reg( ctrl_p, OGMA_REG_ADDR_DIS_CORE, 0);

    if ( pfdep_msleep(1) != PFDEP_ERR_OK) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_init.\n"
                     "Failed to waiting for microcode initialize end.\n");
        ogma_err = OGMA_ERR_INTERRUPT;
        goto err;
    }

    /* check microcode load end & start core */
    value = ogma_read_reg( ctrl_p, OGMA_REG_ADDR_TOP_STATUS);

    if ( ( value & OGMA_TOP_IRQ_REG_ME_START) == 0) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_init.\n"
                     "Failed to microcode loading.\n");
        ogma_err = OGMA_ERR_INVALID;
        goto err;
    }

    /*
     * Do post-initialization tasks for microengine
     *
     * We put phy in normal mode and apply reset.
     */
    ogma_post_init_microengine (ctrl_p);

    /* clear microcode load end status */
    ogma_write_reg( ctrl_p, OGMA_REG_ADDR_TOP_STATUS,
                    OGMA_TOP_IRQ_REG_ME_START);

    ogma_pop_clk_req( ctrl_p, OGMA_CLK_EN_REG_DOM_G);

    ctrl_p->core_enabled_flag = OGMA_TRUE;

    ctrl_p->next_p = ogma_global.list_head_p;

    ogma_global.list_head_p = ctrl_p;

    ++ogma_global.list_entry_num;

    *ogma_handle_p = ctrl_p;

#if 1
    {

#define OGMA_PKT_CTRL_REG_MODE_TAIKI        (1UL << 28)
#define OGMA_DMA_MH_CTRL_REG_MODE_TRANS (1UL << 20)
#define OGMA_TOP_IRQ_REG_MODE_TRANS_COMP (1UL <<  4)
#define OGMA_MODE_TRANS_COMP_IRQ_T2N (1UL << 19)

        /* Read Pkt ctrl register */
        value = ogma_read_reg( ctrl_p, OGMA_REG_ADDR_PKT_CTRL);
        value |= OGMA_PKT_CTRL_REG_MODE_TAIKI;

        /* change to noromal mode */
        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_DMA_MH_CTRL,
                        OGMA_DMA_MH_CTRL_REG_MODE_TRANS);

        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_PKT_CTRL,
                        value);

        while ((ogma_read_reg(ctrl_p, OGMA_REG_ADDR_MODE_TRANS_COMP_STATUS) &
                OGMA_MODE_TRANS_COMP_IRQ_T2N) == 0) {
            ;
        }

    }
#endif

    /* Print microcode version information. */
    pfdep_print(PFDEP_DEBUG_LEVEL_NOTICE,
                "Microcode version: %08x\n",
                ogma_read_reg( ctrl_p,
                               mc_ver_reg_addr) );

    return OGMA_ERR_OK;

err:
    if ( !all_lock_init_done_flag) {


        if ( clk_ctrl_hard_lock_init_flag) {
            pfdep_uninit_hard_lock( &ctrl_p->clk_ctrl_hard_lock);
        }

        if ( inten_reg_hard_lock_init_flag) {
            pfdep_uninit_hard_lock( &ctrl_p->inten_reg_hard_lock);
        }

    } else {

        ogma_internal_terminate( ctrl_p);

    }

    pfdep_free( ctrl_p);

    return ogma_err;
}

STATIC void ogma_internal_terminate (
    ogma_ctrl_t *ctrl_p
    )
{
    ogma_int i;

    ogma_reset_hardware( ctrl_p);

    /* free desc_ring */
    for( i = 0; i <= OGMA_DESC_RING_ID_MAX; i++) {
        ogma_free_desc_ring( ctrl_p, &ctrl_p->desc_ring[i] );
    }

    if ( ctrl_p->dummy_desc_entry_addr != NULL) {
        pfdep_dma_free( ctrl_p->dev_handle,
                        OGMA_DUMMY_DESC_ENTRY_LEN,
                        ctrl_p->dummy_desc_entry_addr,
                        ctrl_p->dummy_desc_entry_phys_addr);
    }

    pfdep_uninit_hard_lock ( &ctrl_p->inten_reg_hard_lock);


    pfdep_uninit_hard_lock ( &ctrl_p->clk_ctrl_hard_lock);

}

void ogma_terminate (
    ogma_handle_t ogma_handle
    )
{

    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;
    ogma_ctrl_t *tmp_ctrl_p = NULL, *old_tmp_ctrl_p = NULL;
    ogma_uint32 domain = 0;

    if ( ( ctrl_p == NULL) ||
         ( ogma_global.list_entry_num == 0) ) {
       return;
    }

    pfdep_assert( ogma_global.list_head_p != NULL);

    if ( ctrl_p->param.use_gmac_flag) {
        domain |= OGMA_CLK_EN_REG_DOM_G;
    }

    /* pop domain clock */
    ogma_pop_clk_req( ctrl_p, domain);

    tmp_ctrl_p = ogma_global.list_head_p;

    while(1) {
        if ( tmp_ctrl_p == NULL) {
            /* Could not found ctrl_p specified from the list */
            return;
        }
        if ( ctrl_p == tmp_ctrl_p) {
            if ( old_tmp_ctrl_p != NULL) {
                old_tmp_ctrl_p->next_p = ctrl_p->next_p;
            } else {
                ogma_global.list_head_p = ctrl_p->next_p;
            }
            break;
        }
        old_tmp_ctrl_p = tmp_ctrl_p;
        tmp_ctrl_p = tmp_ctrl_p->next_p;
    }

    ogma_internal_terminate( ctrl_p);

    --ogma_global.list_entry_num;

    pfdep_free( ctrl_p);
    return;
}

ogma_err_t ogma_enable_top_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;


    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    /* set irq_factor*/
    ogma_write_reg( ctrl_p, OGMA_REG_ADDR_TOP_INTEN_SET, irq_factor);

    return OGMA_ERR_OK;
}

ogma_err_t ogma_disable_top_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    /* clear irq_factor*/
    ogma_write_reg( ctrl_p, OGMA_REG_ADDR_TOP_INTEN_CLR, irq_factor);

    return OGMA_ERR_OK;
}

ogma_err_t ogma_enable_desc_ring_irq (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_uint32 irq_factor
    )
{
    ogma_err_t ogma_err = OGMA_ERR_OK;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;
    ogma_desc_ring_t *desc_ring_p;

    pfdep_err_t pfdep_err;
    pfdep_soft_lock_ctx_t soft_lock_ctx;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_enable_desc_ring_irq.\n"
                     "Please set valid ogma_handle.\n");
        return OGMA_ERR_PARAM;
    }

    if ( ring_id > OGMA_DESC_RING_ID_MAX) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_enable_desc_ring_irq.\n"
                     "Please select ring id number between 0 and %d.\n",
                     OGMA_DESC_RING_ID_MAX);
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->desc_ring[ring_id].param.valid_flag) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_enable_desc_ring_irq.\n"
                     "Please select valid desc ring.\n");
        return OGMA_ERR_NOTAVAIL;
    }

    desc_ring_p = &ctrl_p->desc_ring[ring_id];

    /* get soft lock */
    pfdep_err = pfdep_acquire_soft_lock (
        &desc_ring_p->soft_lock,
        &soft_lock_ctx);

    if ( pfdep_err != PFDEP_ERR_OK) {
        return OGMA_ERR_INTERRUPT;
    }

    if ( !desc_ring_p->running_flag) {
        pfdep_release_soft_lock( &desc_ring_p->soft_lock,
                                 &soft_lock_ctx);
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_enable_desc_ring_irq.\n"
                     "Please select running desc ring.\n");
        return OGMA_ERR_NOTAVAIL;
    }

    /* set irq_factor*/
    ogma_write_reg( ctrl_p,
                    desc_ring_irq_inten_set_reg_addr[ring_id],
                    irq_factor);

    /* free soft_lock*/
    pfdep_release_soft_lock( &desc_ring_p->soft_lock,
                             &soft_lock_ctx);
    return ogma_err;
}

ogma_err_t ogma_disable_desc_ring_irq (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_uint32 irq_factor
    )
{
    ogma_err_t ogma_err = OGMA_ERR_OK;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_disable_desc_ring_irq.\n"
                     "Please set valid ogma_handle.\n");
        return OGMA_ERR_PARAM;
    }

    if ( ring_id > OGMA_DESC_RING_ID_MAX) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_disable_desc_ring_irq.\n"
                     "Please select ring id number between 0 and %d.\n",
                     OGMA_DESC_RING_ID_MAX);
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->desc_ring[ring_id].param.valid_flag) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_disable_desc_ring_irq.\n"
                     "Please select valid desc ring.\n");
        return OGMA_ERR_NOTAVAIL;
    }

    /* Clear irq factor*/
    ogma_write_reg( ctrl_p,
                    desc_ring_irq_inten_clr_reg_addr[ring_id],
                    irq_factor);

    return ogma_err;
}

ogma_err_t ogma_enable_pkt_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    )
{
    ogma_err_t ogma_err = OGMA_ERR_OK;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    /* set irq_factor*/
    ogma_write_reg( ctrl_p, OGMA_REG_ADDR_PKT_INTEN_SET,
                    ( irq_factor & OGMA_PKT_IRQ_ALL) );

    return ogma_err;
}

ogma_err_t ogma_disable_pkt_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    )
{
    ogma_err_t ogma_err = OGMA_ERR_OK;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    /* clear irq_factor*/
    ogma_write_reg( ctrl_p,
                    OGMA_REG_ADDR_PKT_INTEN_CLR,
                    ( irq_factor & OGMA_PKT_IRQ_ALL) );

    return ogma_err;
}


ogma_uint32 ogma_get_top_irq_enable (
    ogma_handle_t ogma_handle
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_top_irq_enable.\n"
                     "Please set valid ogma_handle.\n");
        return 0;
    }

    value = ogma_read_reg( ctrl_p, OGMA_REG_ADDR_TOP_INTEN);

    return value;

}


ogma_uint32 ogma_get_top_irq_status (
    ogma_handle_t ogma_handle,
    ogma_bool mask_flag
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_top_irq_status.\n"
                     "Please set valid ogma_handle.\n");
        return 0;
    }

    value = ogma_read_reg( ctrl_p, OGMA_REG_ADDR_TOP_STATUS);

    if ( mask_flag) {
        value &= ogma_read_reg( ctrl_p, OGMA_REG_ADDR_TOP_INTEN);
    }

    return value;
}

ogma_err_t ogma_clear_top_irq_status (
    ogma_handle_t ogma_handle,
    ogma_uint32 value
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_clear_top_irq_status.\n"
                     "Please set valid ogma_handle.\n");
        return OGMA_ERR_PARAM;
    }

    /* Write clear irq top  status */
    ogma_write_reg( ctrl_p,
                    OGMA_REG_ADDR_TOP_STATUS,

                    ( value & OGMA_TOP_IRQ_REG_ME_START) );

    return OGMA_ERR_OK;
}

ogma_uint32 ogma_get_desc_ring_irq_enable (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_desc_ring_irq_enable.\n"
                     "Please set valid ogma_handle.\n");
        return 0;
    }

    if ( ring_id > OGMA_DESC_RING_ID_MAX) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_desc_ring_irq_enable.\n"
                     "Please select ring id number between 0 and %d.\n",
                     OGMA_DESC_RING_ID_MAX);
        return 0;
    }

    if ( !ctrl_p->desc_ring[ring_id].param.valid_flag) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_desc_ring_irq_enable.\n"
                     "Please select valid desc ring.\n");
        return 0;
    }

    value = ogma_read_reg( ctrl_p,
                           desc_ring_irq_inten_reg_addr[ring_id] );

    return value;
}


ogma_uint32 ogma_get_desc_ring_irq_status (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_bool mask_flag
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_desc_ring_irq_status.\n"
                     "Please set valid ogma_handle.\n");
        return 0;
    }

    if ( ring_id > OGMA_DESC_RING_ID_MAX) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_desc_ring_irq_status.\n"
                     "Please select ring id number between 0 and %d.\n",
                     OGMA_DESC_RING_ID_MAX);
        return 0;
    }

    if ( !ctrl_p->desc_ring[ring_id].param.valid_flag) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_desc_ring_irq_status.\n"
                     "Please select valid desc ring.\n");
        return 0;
    }

    value = ogma_read_reg( ctrl_p,
                           desc_ring_irq_status_reg_addr[ring_id] );

    if ( mask_flag) {
        value &= ogma_read_reg( ctrl_p,
                                desc_ring_irq_inten_reg_addr[ring_id] );
    }

    return value;
}



ogma_err_t ogma_clear_desc_ring_irq_status (
    ogma_handle_t ogma_handle,
    ogma_desc_ring_id_t ring_id,
    ogma_uint32 value
    )
{

    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_clear_desc_ring_irq_status.\n"
                     "Please set valid ogma_handle.\n");
        return OGMA_ERR_PARAM;
    }

    if ( ring_id > OGMA_DESC_RING_ID_MAX) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_clear_desc_ring_irq_status.\n"
                     "Please select ring id number between 0 and %d.\n",
                     OGMA_DESC_RING_ID_MAX);
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->desc_ring[ring_id].param.valid_flag) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_clear_desc_ring_irq_status.\n"
                     "Please select valid desc ring.\n");
        return OGMA_ERR_NOTAVAIL;
    }

    /* Write clear descring irq status */
    ogma_write_reg( ctrl_p,
                    desc_ring_irq_status_reg_addr[ring_id],
                    ( value & ( OGMA_CH_IRQ_REG_EMPTY |
                                OGMA_CH_IRQ_REG_ERR) ) );

    return OGMA_ERR_OK;

}


ogma_uint32 ogma_get_pkt_irq_enable (
    ogma_handle_t ogma_handle
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;


    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_pkt_irq_enable.\n"
                     "Please set valid ogma_handle.\n");
        return 0;
    }

    value = ogma_read_reg( ctrl_p, OGMA_REG_ADDR_PKT_INTEN);

    value &= OGMA_PKT_IRQ_ALL;

    return value;

}

ogma_uint32 ogma_get_pkt_irq_status (
    ogma_handle_t ogma_handle,
    ogma_bool mask_flag
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_pkt_irq_statusnon_clear.\n"
                     "Please set valid ogma_handle.\n");
        return 0;
    }

    value = ogma_read_reg( ctrl_p, OGMA_REG_ADDR_PKT_STATUS);

    if ( mask_flag) {
        value &= ogma_read_reg( ctrl_p, OGMA_REG_ADDR_PKT_INTEN);
    }

    return value;
}

ogma_err_t ogma_clear_pkt_irq_status (
    ogma_handle_t ogma_handle,
    ogma_uint32 value
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_clear_pkt_irq_status.\n"
                     "Please set valid ogma_handle.\n");
        return OGMA_ERR_PARAM;
    }

    /* Write clear irq pkt  status */
    ogma_write_reg( ctrl_p,
                    OGMA_REG_ADDR_PKT_STATUS,
                    ( value & ( ( OGMA_PKT_IRQ_MAC_ER        |
                                  OGMA_PKT_IRQ_JUMBO_ER      |
                                  OGMA_PKT_IRQ_CHKSUM_ER     |
                                  OGMA_PKT_IRQ_HD_INCOMPLETE |
                                  OGMA_PKT_IRQ_HD_ER         |
                                  OGMA_PKT_IRQ_DRP_NO_MATCH) ) ) );

    return OGMA_ERR_OK;
}

#ifdef OGMA_CONFIG_REC_STAT
ogma_err_t ogma_get_stat_info (
    ogma_handle_t ogma_handle,
    ogma_stat_info_t *stat_info_p,
    ogma_bool clear_flag
    )
{

    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ( ctrl_p == NULL) || (stat_info_p == NULL) ) {
        return OGMA_ERR_PARAM;
    }

    pfdep_memcpy( stat_info_p,
                  &ctrl_p->stat_info,
                  sizeof( ogma_stat_info_t) );


    if ( clear_flag) {
        pfdep_memset( &ctrl_p->stat_info, 0, sizeof( ogma_stat_info_t) );
    }

    return OGMA_ERR_OK;

}
#endif /* OGMA_CONFIG_REC_STAT */

ogma_uint32 ogma_get_hw_ver (
    ogma_handle_t ogma_handle
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return 0;
    }

    value = ogma_read_reg( ctrl_p,
                           hw_ver_reg_addr);

    return value;
}


ogma_uint32 ogma_get_mcr_ver (
    ogma_handle_t ogma_handle
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return 0;
    }

    value = ogma_read_reg( ctrl_p,
                           mc_ver_reg_addr);

    return value;
}

ogma_uint32 ogma_get_mac_irq_enable (
    ogma_handle_t ogma_handle
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_ogma_get_mac_irq_enable.\n"
                     "Please set valid ogma_handle.\n");
        return 0;
    }

    value = ogma_read_reg( ctrl_p, OGMA_REG_ADDR_MAC_INTEN);


    return value;
}

ogma_uint32 ogma_get_mac_irq_status (
    ogma_handle_t ogma_handle,
    ogma_bool mask_flag
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_mac_irq_status.\n"
                     "Please set valid ogma_handle.\n");
        return 0;
    }


    value = ogma_read_reg( ctrl_p, OGMA_REG_ADDR_MAC_STATUS);

    if ( mask_flag) {
        value &= ogma_read_reg( ctrl_p, OGMA_REG_ADDR_MAC_INTEN);
    }


    return value;
}

ogma_err_t ogma_clear_mac_irq_status (
    ogma_handle_t ogma_handle,
    ogma_uint32 value
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_clear_mac_irq_status.\n"
                     "Please set valid ogma_handle.\n");
        return OGMA_ERR_PARAM;
    }


    /* Commented out because no write-clear bit exists now. */
#if 0
    ogma_write_reg( ctrl_p,
                    OGMA_REG_ADDR_MAC_STATUS,
                    ( value & /* T.B.D. */) );
#endif


    return OGMA_ERR_OK;

}

ogma_err_t ogma_enable_mac_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    )
{
    ogma_uint32 value;
    ogma_err_t ogma_err = OGMA_ERR_OK;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;
    pfdep_hard_lock_ctx_t inten_reg_hard_lock_ctx;

    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_enable_mac_irq.\n"
                     "Please set valid ogma_handle.\n");
        return OGMA_ERR_PARAM;
    }

    /* get inten_reg_hard_lock */
    pfdep_acquire_hard_lock( &ctrl_p->inten_reg_hard_lock,
                             &inten_reg_hard_lock_ctx);

   value = ogma_read_reg( ctrl_p,
                          OGMA_REG_ADDR_MAC_INTEN);

   value |= irq_factor;

   ogma_write_reg( ctrl_p,
                   OGMA_REG_ADDR_MAC_INTEN,
                   value);


    /* free inten_reg_hard_lock*/
    pfdep_release_hard_lock( &ctrl_p->inten_reg_hard_lock,
                             &inten_reg_hard_lock_ctx);
    return ogma_err;
}

ogma_err_t ogma_disable_mac_irq (
    ogma_handle_t ogma_handle,
    ogma_uint32 irq_factor
    )
{
    ogma_uint32 value;
    ogma_err_t ogma_err = OGMA_ERR_OK;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    pfdep_hard_lock_ctx_t inten_reg_hard_lock_ctx;


    if ( ctrl_p == NULL) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_disable_mac_irq.\n"
                     "Please set valid ogma_handle.\n");
        return OGMA_ERR_PARAM;
    }

    /* get inten_reg_hard_lock */
    pfdep_acquire_hard_lock( &ctrl_p->inten_reg_hard_lock,
                             &inten_reg_hard_lock_ctx);

   value = ogma_read_reg( ctrl_p,
                          OGMA_REG_ADDR_MAC_INTEN);

   value &= (~irq_factor);

   ogma_write_reg( ctrl_p,
                   OGMA_REG_ADDR_MAC_INTEN,
                   value);


    /* free inten_reg_hard_lock*/
    pfdep_release_hard_lock( &ctrl_p->inten_reg_hard_lock,
                             &inten_reg_hard_lock_ctx);
    return ogma_err;

}
