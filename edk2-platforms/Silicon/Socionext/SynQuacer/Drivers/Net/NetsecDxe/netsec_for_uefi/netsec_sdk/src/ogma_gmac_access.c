/** @file

  Copyright (c) 2016 - 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "ogma_config.h"

#include "ogma_internal.h"
#include "ogma_basic_access.h"


/**********************************************************************
 * Constant definitions
 **********************************************************************/

/**
 * Clock range index for F_GMAC4MT::GAR::CR field.
 */
#if (OGMA_CONFIG_GMAC_CLK_HZ < 35 * OGMA_CLK_MHZ)
#define OGMA_CLOCK_RANGE_IDX OGMA_GMAC_GAR_REG_CR_25_35_MHZ
#elif (OGMA_CONFIG_GMAC_CLK_HZ < 60 * OGMA_CLK_MHZ)
#define OGMA_CLOCK_RANGE_IDX OGMA_GMAC_GAR_REG_CR_35_60_MHZ
#elif (OGMA_CONFIG_GMAC_CLK_HZ < 100 * OGMA_CLK_MHZ)
#define OGMA_CLOCK_RANGE_IDX OGMA_GMAC_GAR_REG_CR_60_100_MHZ
#elif (OGMA_CONFIG_GMAC_CLK_HZ < 150 * OGMA_CLK_MHZ)
#define OGMA_CLOCK_RANGE_IDX OGMA_GMAC_GAR_REG_CR_100_150_MHZ
#elif (OGMA_CONFIG_GMAC_CLK_HZ < 250 * OGMA_CLK_MHZ)
#define OGMA_CLOCK_RANGE_IDX OGMA_GMAC_GAR_REG_CR_150_250_MHZ
#else
#define OGMA_CLOCK_RANGE_IDX OGMA_GMAC_GAR_REG_CR_250_300_MHZ
#endif


/**********************************************************************
 * Local function declarations
 **********************************************************************/
static void ogma_set_phy_reg_sub (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 reg_addr,
    ogma_uint16 value
    );

static ogma_uint16 ogma_get_phy_reg_sub (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 reg_addr
    );

#ifdef OGMA_CONFIG_USE_DUMP_GMAC_STAT
void ogma_dump_gmac_stat (ogma_ctrl_t *ctrl_p);
#endif /*  OGMA_CONFIG_USE_DUMP_GMAC_STAT */

static void ogma_set_phy_target_mmd_reg_addr (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr
    );

static void ogma_set_phy_mmd_reg_sub (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr,
    ogma_uint16 value
    );

static ogma_uint16 ogma_get_phy_mmd_reg_sub (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr
    );

/**********************************************************************
 * Function definitions
 **********************************************************************/

ogma_err_t ogma_start_gmac (
    ogma_handle_t ogma_handle,
    ogma_bool rx_flag,
    ogma_bool tx_flag
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;
    pfdep_err_t pfdep_err;
    ogma_err_t ogma_err;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    if ( !ctrl_p->gmac_mode_valid_flag) {
        return OGMA_ERR_INVALID;
    }

    if ( ( !rx_flag) && ( !tx_flag) ) {
        return OGMA_ERR_OK;
    }

    if ( ctrl_p->gmac_rx_running_flag &&
         ctrl_p->gmac_tx_running_flag) {
        return OGMA_ERR_OK;
    }


    if ( ( rx_flag && ctrl_p->gmac_rx_running_flag) &&
         !tx_flag) {
        return OGMA_ERR_OK;
    }

    if ( ( tx_flag && ctrl_p->gmac_tx_running_flag) &&
         !rx_flag ) {
        return OGMA_ERR_OK;
    }

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "%s call.\n", __func__);

    if ( (! ctrl_p->gmac_rx_running_flag) &&
         (! ctrl_p->gmac_tx_running_flag) ) {

        /* Initializes F_GMAC4MT */
        if ( ctrl_p->gmac_mode.link_speed ==
             OGMA_PHY_LINK_SPEED_1G) {
            /* Writes 0 to FGMAC4 MCR */
            ogma_set_mac_reg( ctrl_p,
                              OGMA_GMAC_REG_ADDR_MCR,
                              0);
        } else {
            /* Writes PS bit to FGMAC4 MCR */
            ogma_set_mac_reg( ctrl_p,
                              OGMA_GMAC_REG_ADDR_MCR,
                              OGMA_GMAC_MCR_10M_HALF);/* 10M half Reset */
        }

        /* F_GMAC4MT soft reset*/
        if ( ( ogma_err = ogma_softreset_gmac( ctrl_p)) != OGMA_ERR_OK) {
            return ogma_err;
        }

        /* MAC desc soft reset */
        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_MAC_DESC_SOFT_RST,
                        OGMA_MAC_DESC_SOFT_RST_SOFT_RST);

        /* Wait MAC desc soft reset */
        pfdep_err = pfdep_msleep( 1);

        if ( pfdep_err == PFDEP_ERR_INTERRUPT) {

            pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                             "An error occurred at ogma_start_gmac.\n");

            pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                             "Wait for MAC desc soft reset error.\n");

            return OGMA_ERR_INTERRUPT;
        }

        /* Check MAC desc soft reset done */
        if ( ( ogma_read_reg( ctrl_p, OGMA_REG_ADDR_MAC_DESC_SOFT_RST) &
               OGMA_MAC_DESC_SOFT_RST_SOFT_RST) != 0) {

            pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                         "ogma_start_gmac():MAC desc soft reset timeout. Try Again.\n");

            return OGMA_ERR_AGAIN;
        }

        /* MAC desc init */
        ogma_write_reg( ctrl_p,
                        OGMA_REG_ADDR_MAC_DESC_INIT,
                        OGMA_MAC_DESC_INIT_REG_INIT);

        /* Wait MAC desc init done */
        pfdep_err = pfdep_msleep( 1);

        if ( pfdep_err == PFDEP_ERR_INTERRUPT) {

            pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                         "An error occurred at ogma_start_gmac().\n");

            pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                         "Wait for MAC desc init done error.\n");

            return OGMA_ERR_INTERRUPT;
        }

        /* Check MAC desc init done */
        if ( ( ogma_read_reg( ctrl_p, OGMA_REG_ADDR_MAC_DESC_INIT) &
               OGMA_MAC_DESC_INIT_REG_INIT) != 0) {

            pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                         "ogma_start_gmac():MAC desc init timeout. Try Again.\n");

            return OGMA_ERR_AGAIN;
        }

        /* set BMR */
        ogma_set_mac_reg( ctrl_p,
                          OGMA_GMAC_REG_ADDR_BMR,
                          OGMA_GMAC_BMR_REG_COMMON);
        /* set RDLAR */
        ogma_set_mac_reg( ctrl_p,
                          OGMA_GMAC_REG_ADDR_RDLAR,
                          OGMA_GMAC_RDLAR_REG_COMMON);
        /* set TDLAR*/
        ogma_set_mac_reg( ctrl_p,
                          OGMA_GMAC_REG_ADDR_TDLAR,
                          OGMA_GMAC_TDLAR_REG_COMMON);
        /* set MFFR*/
        ogma_set_mac_reg( ctrl_p,
                          OGMA_GMAC_REG_ADDR_MFFR,
                          0x80000001UL);

        /* calc MCR setting val */
        value = ( ctrl_p->gmac_mode.half_duplex_flag ?
                  OGMA_GMAC_MCR_REG_HALF_DUPLEX_COMMON :
                  OGMA_GMAC_MCR_REG_FULL_DUPLEX_COMMON);

        if ( ctrl_p->gmac_mode.link_speed != OGMA_PHY_LINK_SPEED_1G) {
            value |= OGMA_GMAC_MCR_REG_PS;
        }

        if ( ( ctrl_p->param.gmac_config.phy_interface !=
               OGMA_PHY_INTERFACE_GMII ) &&
             ( ctrl_p->gmac_mode.link_speed == OGMA_PHY_LINK_SPEED_100M) ) {
            value |= OGMA_GMAC_MCR_REG_FES;
        }
        /* set CST bit  */
        value |= OGMA_GMAC_MCR_REG_CST;

        /* set JE bit  */
        value |= OGMA_GMAC_MCR_REG_JE;

        if ( ctrl_p->param.gmac_config.phy_interface ==
             OGMA_PHY_INTERFACE_RGMII) {
            /* set ignore in-band-status watch option. force tx clk out. */
            value |= 0x40000000U;
        }

        /* set MCR */
        ogma_set_mac_reg( ctrl_p,
                          OGMA_GMAC_REG_ADDR_MCR,
                          value);

        if ( ctrl_p->gmac_mode.flow_ctrl_enable_flag) {
            /* Set Flow Control Threshold */
            value =
                ( ctrl_p->gmac_mode.flow_ctrl_stop_threshold << 16) |
                ctrl_p->gmac_mode.flow_ctrl_start_threshold;

            ogma_write_reg( ctrl_p,
                            OGMA_REG_ADDR_MAC_FLOW_TH,
                            value);
            /* Set Flow Control Threshold F_GMAC4MT*/
            value =
                ( ctrl_p->gmac_mode.pause_time << 16) |
                OGMA_GMAC_FCR_REG_RFE |
                OGMA_GMAC_FCR_REG_TFE;

            ogma_set_mac_reg( ctrl_p,
                              OGMA_GMAC_REG_ADDR_FCR,
                              value);
        }

    }

    if ( ( rx_flag && (! ctrl_p->gmac_rx_running_flag) ) ||
         ( tx_flag && (! ctrl_p->gmac_tx_running_flag) )
        ) {
        /* Read F_GMAC4MT OMR*/
        value = ogma_get_mac_reg( ctrl_p,
                                  OGMA_GMAC_REG_ADDR_OMR);

        if ( rx_flag && (! ctrl_p->gmac_rx_running_flag) ) {
            value |= OGMA_GMAC_OMR_REG_SR;
            ctrl_p->gmac_rx_running_flag = OGMA_TRUE;
        }

        if ( tx_flag && (! ctrl_p->gmac_tx_running_flag) ) {
            value |= OGMA_GMAC_OMR_REG_ST;
            ctrl_p->gmac_tx_running_flag = OGMA_TRUE;
        }

        /* set OMR*/
        ogma_set_mac_reg( ctrl_p,
                          OGMA_GMAC_REG_ADDR_OMR,
                          value);

    }

    return OGMA_ERR_OK;

}

ogma_err_t ogma_stop_gmac (
    ogma_handle_t ogma_handle,
    ogma_bool rx_flag,
    ogma_bool tx_flag
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }


    if ( ( !rx_flag) && ( !tx_flag) ) {
        return OGMA_ERR_OK;
    }

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "%s call.\n", __func__);

    if ( ( rx_flag && ctrl_p->gmac_rx_running_flag) ||
         ( tx_flag && ctrl_p->gmac_tx_running_flag) ) {
        /* Read F_GMAC4MT OMR*/
        value = ogma_get_mac_reg( ctrl_p,
                                  OGMA_GMAC_REG_ADDR_OMR);

        if ( rx_flag && ctrl_p->gmac_rx_running_flag) {
            value &= (~OGMA_GMAC_OMR_REG_SR);
            ctrl_p->gmac_rx_running_flag = OGMA_FALSE;
        }

        if ( tx_flag && ctrl_p->gmac_tx_running_flag) {
            value &= (~OGMA_GMAC_OMR_REG_ST);
            ctrl_p->gmac_tx_running_flag = OGMA_FALSE;
        }

        /* set F_GMAC4MT OMR*/
        ogma_set_mac_reg( ctrl_p,
                          OGMA_GMAC_REG_ADDR_OMR,
                          value);
    }

#ifdef OGMA_CONFIG_USE_DUMP_GMAC_STAT
    ogma_dump_gmac_stat (ctrl_p);
#endif /*  OGMA_CONFIG_USE_DUMP_GMAC_STAT */


    return OGMA_ERR_OK;

}

ogma_err_t ogma_set_gmac_mode (
    ogma_handle_t ogma_handle,
    const ogma_gmac_mode_t *gmac_mode_p
    )
{

    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ( ctrl_p == NULL) ||
         ( gmac_mode_p == NULL) ) {
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    if ( ( ctrl_p->gmac_rx_running_flag) ||
         ( ctrl_p->gmac_tx_running_flag) ) {
        return OGMA_ERR_BUSY;
    }

   if ( ( gmac_mode_p->link_speed != OGMA_PHY_LINK_SPEED_1G  ) &&
        ( gmac_mode_p->link_speed != OGMA_PHY_LINK_SPEED_100M) &&
        ( gmac_mode_p->link_speed != OGMA_PHY_LINK_SPEED_10M ) ) {
       return OGMA_ERR_DATA;
    }

   if ( ( gmac_mode_p->link_speed == OGMA_PHY_LINK_SPEED_1G) &&
        ( gmac_mode_p->half_duplex_flag) ) {
       return OGMA_ERR_DATA;
   }

   if ( gmac_mode_p->half_duplex_flag &&
        gmac_mode_p->flow_ctrl_enable_flag) {
       return OGMA_ERR_DATA;
   }

   if ( gmac_mode_p->flow_ctrl_enable_flag) {

       if ( ( gmac_mode_p->flow_ctrl_start_threshold == 0) ||
            ( gmac_mode_p->flow_ctrl_start_threshold >
              OGMA_FLOW_CTRL_START_THRESHOLD_MAX) ) {
           return OGMA_ERR_DATA;
       }

       if ( ( gmac_mode_p->flow_ctrl_stop_threshold <
              gmac_mode_p->flow_ctrl_start_threshold) ||
            ( gmac_mode_p->flow_ctrl_stop_threshold >
              OGMA_FLOW_CTRL_STOP_THRESHOLD_MAX) ) {
           return OGMA_ERR_DATA;
       }

       if ( gmac_mode_p->pause_time < OGMA_FLOW_CTRL_PAUSE_TIME_MIN) {
           return OGMA_ERR_DATA;
       }
   }

   pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "%s call.\n", __func__);

   pfdep_memcpy( ( void *)&ctrl_p->gmac_mode,
                 ( void *)gmac_mode_p,
                 sizeof( ogma_gmac_mode_t) );

   ctrl_p->gmac_mode_valid_flag = OGMA_TRUE;


   return OGMA_ERR_OK;

}

static void ogma_set_phy_reg_sub (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 reg_addr,
    ogma_uint16 value
    )
{

    ogma_uint32 cmd;

    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_GDR,
                      value);

    cmd = ( ( ctrl_p->param.phy_addr << OGMA_GMAC_GAR_REG_SHIFT_PA) |
            ( reg_addr << OGMA_GMAC_GAR_REG_SHIFT_GR) |
            ( OGMA_CLOCK_RANGE_IDX << OGMA_GMAC_GAR_REG_SHIFT_CR) |
            OGMA_GMAC_GAR_REG_GW |
            OGMA_GMAC_GAR_REG_GB);

    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_GAR,
                      cmd);

    while ( ( ogma_get_mac_reg( ctrl_p, OGMA_GMAC_REG_ADDR_GAR) &
              OGMA_GMAC_GAR_REG_GB)
            != 0) {
        ;
    }
}

void ogma_set_phy_reg (
    ogma_handle_t ogma_handle,
    ogma_uint8 reg_addr,
    ogma_uint16 value
    )
{

    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if (( ctrl_p == NULL)
        || ( !ctrl_p->param.use_gmac_flag)
        || ( reg_addr >= 32) ) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_set_phy_reg.\nPlease set valid argument.\n");
        return;
    }

    ogma_set_phy_reg_sub( ctrl_p, reg_addr, value);

}

static ogma_uint16 ogma_get_phy_reg_sub (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 reg_addr
    )
{

    ogma_uint32 cmd;

    cmd = ( ( ctrl_p->param.phy_addr << OGMA_GMAC_GAR_REG_SHIFT_PA) |
            ( reg_addr << OGMA_GMAC_GAR_REG_SHIFT_GR) |
            ( OGMA_CLOCK_RANGE_IDX << OGMA_GMAC_GAR_REG_SHIFT_CR) |
            OGMA_GMAC_GAR_REG_GB);

    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_GAR,
                      cmd);

    while ( ( ogma_get_mac_reg( ctrl_p, OGMA_GMAC_REG_ADDR_GAR) &
              OGMA_GMAC_GAR_REG_GB)
            != 0) {
        ;
    }
    return (ogma_uint16)ogma_get_mac_reg( ctrl_p, OGMA_GMAC_REG_ADDR_GDR);

}

ogma_uint16 ogma_get_phy_reg (
    ogma_handle_t ogma_handle,
    ogma_uint8 reg_addr
    )
{
    ogma_uint16 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ( ctrl_p == NULL)
         || ( !ctrl_p->param.use_gmac_flag)
         || ( reg_addr >= 32) ) {
        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "An error occurred at ogma_get_phy_reg.\nPlease set valid argument.\n");
        return 0;
    }

    value = ogma_get_phy_reg_sub(ctrl_p, reg_addr);


    return value;
}

#if ( defined(OGMA_CONFIG_USE_READ_GMAC_STAT) || defined(OGMA_CONFIG_USE_DUMP_GMAC_STAT) )
static const struct {
    ogma_uint32 addr;
    ogma_char *name_p;
} ogma_gmac_mmc_reg_info[] = {
    {OGMA_GMAC_REG_ADDR_MMC_INTR_RX         , "MMC_INTR_RX"},
    {OGMA_GMAC_REG_ADDR_MMC_INTR_TX         , "MMC_INTR_TX"},
    {OGMA_GMAC_REG_ADDR_MMC_INTR_MASK_RX    , "MMC_INTR_MASK_RX"},
    {OGMA_GMAC_REG_ADDR_MMC_INTR_MASK_TX    , "MMC_INTR_MASK_TX"},
    {OGMA_GMAC_REG_ADDR_TXOCTETCOUNT_GB     , "TXOCTETCOUNT_GB"},
    {OGMA_GMAC_REG_ADDR_TXFRAMECOUNT_GB     , "TXFRAMECOUNT_GB"},
    {OGMA_GMAC_REG_ADDR_TXBROADCASTFRAMES_G , "TXBROADCASTFRAMES_G"},
    {OGMA_GMAC_REG_ADDR_TXMULTICASTFRAMES_G , "TXMULTICASTFRAMES_G"},
    {OGMA_GMAC_REG_ADDR_TX64OCTETS_GB       , "TX64OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_TX65TO127OCTETS_GB  , "TX65TO127OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_TX128TO255OCTETS_GB , "TX128TO255OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_TX256TO511OCTETS_GB , "TX256TO511OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_TX512TO1023OCTETS_GB, "TX512TO1023OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_TX1024TOMAXOCTETS_GB, "TX1024TOMAXOCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_TXUNICASTFRAMES_GB  , "TXUNICASTFRAMES_GB"},
    {OGMA_GMAC_REG_ADDR_TXMULTICASTFRAMES_GB, "TXMULTICASTFRAMES_GB"},
    {OGMA_GMAC_REG_ADDR_TXBROADCASTFRAMES_GB, "TXBROADCASTFRAMES_GB"},
    {OGMA_GMAC_REG_ADDR_TXUNDERFLOWERROR    , "TXUNDERFLOWERROR"},
    {OGMA_GMAC_REG_ADDR_TXSINGLECOL_G       , "TXSINGLECOL_G"},
    {OGMA_GMAC_REG_ADDR_TXMULTICOL_G        , "TXMULTICOL_G"},
    {OGMA_GMAC_REG_ADDR_TXDEFERRED          , "TXDEFERRED"},
    {OGMA_GMAC_REG_ADDR_TXLATECOL           , "TXLATECOL"},
    {OGMA_GMAC_REG_ADDR_TXEXESSCOL          , "TXEXESSCOL"},
    {OGMA_GMAC_REG_ADDR_TXCARRIERERRROR     , "TXCARRIERERRROR"},
    {OGMA_GMAC_REG_ADDR_TXOCTETCOUNT_G      , "TXOCTETCOUNT_G"},
    {OGMA_GMAC_REG_ADDR_TXFRAMECOUNT_G      , "TXFRAMECOUNT_G"},
    {OGMA_GMAC_REG_ADDR_TXEXECESSDEF        , "TXEXECESSDEF"},
    {OGMA_GMAC_REG_ADDR_TXPAUSEFRAMES       , "TXPAUSEFRAMES"},
    {OGMA_GMAC_REG_ADDR_TXVLANFRAMES_G      , "TXVLANFRAMES_G"},
    {OGMA_GMAC_REG_ADDR_RXFRAMECOUNT_GB     , "RXFRAMECOUNT_GB"},
    {OGMA_GMAC_REG_ADDR_RXOCTETCOUNT_GB     , "RXOCTETCOUNT_GB"},
    {OGMA_GMAC_REG_ADDR_RXOCTETCOUNT_G      , "RXOCTETCOUNT_G"},
    {OGMA_GMAC_REG_ADDR_RXBROADCASTFRAMES_G , "RXBROADCASTFRAMES_G"},
    {OGMA_GMAC_REG_ADDR_RXMULTICASTFRAMES_G , "RXMULTICASTFRAMES_G"},
    {OGMA_GMAC_REG_ADDR_RXCRCERROR          , "RXCRCERROR"},
    {OGMA_GMAC_REG_ADDR_RXALLIGNMENTERROR   , "RXALLIGNMENTERROR"},
    {OGMA_GMAC_REG_ADDR_RXRUNTERROR         , "RXRUNTERROR"},
    {OGMA_GMAC_REG_ADDR_RXJABBERERROR       , "RXJABBERERROR"},
    {OGMA_GMAC_REG_ADDR_RXUNDERSIZE_G       , "RXUNDERSIZE_G"},
    {OGMA_GMAC_REG_ADDR_RXOVERSIZE_G        , "RXOVERSIZE_G"},
    {OGMA_GMAC_REG_ADDR_RX64OCTETS_GB       , "RX64OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_RX65TO127OCTETS_GB  , "RX65TO127OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_RX128TO255OCTETS_GB , "RX128TO255OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_RX256TO511OCTETS_GB , "RX256TO511OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_RX512TO1023OCTETS_GB, "RX512TO1023OCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_RX1024TOMAXOCTETS_GB, "RX1024TOMAXOCTETS_GB"},
    {OGMA_GMAC_REG_ADDR_RXUNICASTFRAMES_G   , "RXUNICASTFRAMES_G"},
    {OGMA_GMAC_REG_ADDR_RXLENGTHERROR       , "RXLENGTHERROR"},
    {OGMA_GMAC_REG_ADDR_RXOUTOFRANGETYPE    , "RXOUTOFRANGETYPE"},
    {OGMA_GMAC_REG_ADDR_RXPAUSEFRAMES       , "RXPAUSEFRAMES"},
    {OGMA_GMAC_REG_ADDR_RXFIFOOVERFLOW      , "RXFIFOOVERFLOW"},
    {OGMA_GMAC_REG_ADDR_RXVLANFRAMES_GB     , "RXVLANFRAMES_GB"},
    {OGMA_GMAC_REG_ADDR_RXWATCHDOGERROR     , "RXWATCHDOGERROR"},
    {OGMA_GMAC_REG_ADDR_MMC_IPC_INTR_MASK_RX, "MMC_IPC_INTR_MASK_RX"},
    {OGMA_GMAC_REG_ADDR_MMC_IPC_INTR_RX     , "MMC_IPC_INTR_RX"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_GD_FRMS      , "RXIPV4_GD_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_HDRERR_FRMS  , "RXIPV4_HDRERR_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_NOPAY_FRMS   , "RXIPV4_NOPAY_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_FRAG_FRMS    , "RXIPV4_FRAG_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_UDSBL_FRMS   , "RXIPV4_UDSBL_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXIPV6_GD_FRMS      , "RXIPV6_GD_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXIPV6_HDRERR_FRMS  , "RXIPV6_HDRERR_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXIPV6_NOPAY_FRMS   , "RXIPV6_NOPAY_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXUDP_GD_FRMS       , "RXUDP_GD_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXUDP_ERR_FRMS      , "RXUDP_ERR_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXTCP_GD_FRMS       , "RXTCP_GD_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXTCP_ERR_FRMS      , "RXTCP_ERR_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXICMP_GD_FRMS      , "RXICMP_GD_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXICMP_ERR_FRMS     , "RXICMP_ERR_FRMS"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_GD_OCTETS    , "RXIPV4_GD_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_HDRERR_OCTETS, "RXIPV4_HDRERR_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_NOPAY_OCTETS , "RXIPV4_NOPAY_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_FRAG_OCTETS  , "RXIPV4_FRAG_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXIPV4_UDSBL_OCTETS , "RXIPV4_UDSBL_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXIPV6_GD_OCTETS    , "RXIPV6_GD_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXIPV6_HDRERR_OCTETS, "RXIPV6_HDRERR_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXIPV6_NOPAY_OCTETS , "RXIPV6_NOPAY_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXUDP_GD_OCTETS     , "RXUDP_GD_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXUDP_ERR_OCTETS    , "RXUDP_ERR_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXTCP_GD_OCTETS     , "RXTCP_GD_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXTCP_ERR_OCTETS    , "RXTCP_ERR_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXICMP_GD_OCTETS    , "RXICMP_GD_OCTETS"},
    {OGMA_GMAC_REG_ADDR_RXICMP_ERR_OCTETS   , "RXICMP_ERR_OCTETS"}
};
#endif /* ( defined(OGMA_CONFIG_USE_READ_GMAC_STAT) || defined(OGMA_CONFIG_USE_DUMP_GMAC_STAT) ) */

#ifdef OGMA_CONFIG_USE_DUMP_GMAC_STAT
void ogma_dump_gmac_stat (ogma_ctrl_t *ctrl_p)
{

    ogma_uint i;

    pfdep_print(PFDEP_DEBUG_LEVEL_NOTICE,
                "Dumping GMAC statistics registers(MMC registers):\n");

    for (i = 0;
         i < sizeof(ogma_gmac_mmc_reg_info)/sizeof(ogma_gmac_mmc_reg_info[0]);
         i++) {
        pfdep_print(PFDEP_DEBUG_LEVEL_NOTICE,
                    "  %s => 0x%08x\n",
                    ogma_gmac_mmc_reg_info[i].name_p,
                    ( unsigned long)ogma_get_mac_reg(ctrl_p,
                                                     ogma_gmac_mmc_reg_info[i].addr));
    }


    /* Reset all counters. */
    ogma_set_mac_reg(ctrl_p, OGMA_GMAC_REG_ADDR_MMC_CNTL, 1);


}

#endif /* OGMA_CONFIG_USE_DUMP_GMAC_STAT */


ogma_err_t ogma_get_gmac_status (
    ogma_handle_t ogma_handle,
    ogma_bool *valid_flag_p,
    ogma_gmac_mode_t *gmac_mode_p,
    ogma_bool *rx_running_flag_p,
    ogma_bool *tx_running_flag_p
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( valid_flag_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    if ( ( ctrl_p == NULL) ||
         ( gmac_mode_p == NULL) ||
         ( rx_running_flag_p == NULL) ||
         ( tx_running_flag_p == NULL) ) {
        *valid_flag_p = OGMA_FALSE;
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        *valid_flag_p = OGMA_FALSE;
        return OGMA_ERR_NOTAVAIL;
    }


    *valid_flag_p = ctrl_p->gmac_mode_valid_flag;

    *rx_running_flag_p = ctrl_p->gmac_rx_running_flag;
    *tx_running_flag_p = ctrl_p->gmac_tx_running_flag;

    pfdep_memcpy( ( void *)gmac_mode_p,
                  ( const void *)&ctrl_p->gmac_mode,
                  sizeof( ogma_gmac_mode_t) );


    return OGMA_ERR_OK;

}

static void ogma_set_phy_target_mmd_reg_addr (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr
    )
{
    ogma_uint32 cmd;

    /* set MMD ADDR */
    cmd = ( ogma_uint32)dev_addr;

    /*set command to MMD access control register */
    ogma_set_phy_reg_sub( ctrl_p, OGMA_PHY_REG_ADDR_MMD_AC, cmd);

    /* set MMD access address data register Write reg_addr */
    ogma_set_phy_reg_sub( ctrl_p, OGMA_PHY_REG_ADDR_MMD_AAD, reg_addr);

    /* write value to MMD ADDR */
    cmd = ( (1U << 14) | dev_addr);

    /* set command to MMD access control register */
    ogma_set_phy_reg_sub( ctrl_p, OGMA_PHY_REG_ADDR_MMD_AC, cmd);
}

static void ogma_set_phy_mmd_reg_sub (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr,
    ogma_uint16 value
    )
{
    /* set target mmd reg_addr */
    ogma_set_phy_target_mmd_reg_addr( ctrl_p,
                                      dev_addr,
                                      reg_addr);

    /* Write value to MMD access address data register */
    ogma_set_phy_reg_sub( ctrl_p, OGMA_PHY_REG_ADDR_MMD_AAD, value);

}

static ogma_uint16 ogma_get_phy_mmd_reg_sub (
    ogma_ctrl_t *ctrl_p,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr
    )
{
    /* set target mmd reg_addr */
    ogma_set_phy_target_mmd_reg_addr( ctrl_p,
                                      dev_addr,
                                      reg_addr);

    /* Read value for MMD access address data register */
    return ogma_get_phy_reg_sub( ctrl_p, OGMA_PHY_REG_ADDR_MMD_AAD);
}

ogma_err_t ogma_set_gmac_lpictrl_reg (
    ogma_handle_t ogma_handle,
    ogma_uint32 value
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    /* set value tp LSIPCR Register */
    ogma_set_mac_reg( ctrl_p, OGMA_GMAC_REG_ADDR_LPICSR, value);


    return OGMA_ERR_OK;
}

ogma_err_t ogma_get_gmac_lpictrl_reg (
    ogma_handle_t ogma_handle,
    ogma_uint32 *value_p
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ( ctrl_p == NULL) ||
         ( value_p == NULL) ){
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    /* Read LSIPCR Register */
    *value_p = ogma_get_mac_reg( ctrl_p,
                                 OGMA_GMAC_REG_ADDR_LPICSR);


    return OGMA_ERR_OK;
}

ogma_err_t ogma_set_gmac_lpitimer_reg (
    ogma_handle_t ogma_handle,
    ogma_uint16 ls_timer_ms,
    ogma_uint16 tw_timer_ms
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    if ( ls_timer_ms > 1024U) {
        return OGMA_ERR_RANGE;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    /* make LPICTR value*/
    value = ( ( ( ogma_uint32)ls_timer_ms << OGMA_GMAC_LPITCR_REG_LIT) |
              ( tw_timer_ms << OGMA_GMAC_LPITCR_REG_TWT) );

    /* Write timer value to LSIPCR Register */
    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_LPITCR,
                      value);


    return OGMA_ERR_OK;
}

ogma_err_t ogma_get_gmac_lpitimer_reg (
    ogma_handle_t ogma_handle,
    ogma_uint16 *ls_timer_ms_p,
    ogma_uint16 *tw_timer_ms_p
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ( ctrl_p == NULL) ||
         ( ls_timer_ms_p == NULL) ||
         ( tw_timer_ms_p == NULL) ) {
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    /* Read timer value for LSIPCR Register */
    value = ogma_get_mac_reg( ctrl_p,
                              OGMA_GMAC_REG_ADDR_LPITCR);

    /* make ls_timer_ms value*/
    *ls_timer_ms_p = ( ogma_uint16)( ( value >> OGMA_GMAC_LPITCR_REG_LIT) &
                                     OGMA_GMAC_LPITCR_REG_MASK_LIT);

    /* make tw_timer_ms value*/
    *tw_timer_ms_p = ( ogma_uint16)( ( value >> OGMA_GMAC_LPITCR_REG_TWT) &
                                     OGMA_GMAC_LPITCR_REG_MASK_TWT);


    return OGMA_ERR_OK;
}

void ogma_set_phy_mmd_reg (
    ogma_handle_t ogma_handle,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr,
    ogma_uint16 value
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return;
    }

    if ( dev_addr > 31U) {
        return;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return;
    }

    ogma_set_phy_mmd_reg_sub ( ctrl_p,
                               dev_addr,
                               reg_addr,
                               value);


    return;
}

ogma_uint16 ogma_get_phy_mmd_reg (
    ogma_handle_t ogma_handle,
    ogma_uint8 dev_addr,
    ogma_uint16 reg_addr
    )
{
    ogma_uint16 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return 0;
    }

    if ( dev_addr > 31U) {
        return 0;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return 0;
    }

    value = ogma_get_phy_mmd_reg_sub ( ctrl_p,
                                       dev_addr,
                                       reg_addr);


    return value;
}

ogma_err_t ogma_get_phy_link_status (
    ogma_handle_t ogma_handle,
    ogma_phy_link_status_t *phy_link_status_p
    )
{
    ogma_uint32 value, tmp, exp;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ( ctrl_p == NULL) ||
         ( phy_link_status_p == NULL) ){
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    pfdep_memset( phy_link_status_p, 0, sizeof( ogma_phy_link_status_t) );

    /* Read PHY CONTROL Register */
    tmp = ogma_get_phy_reg_sub( ctrl_p,  OGMA_PHY_REG_ADDR_CONTROL);

    /* Read PHY STATUS Register */
    value = ogma_get_phy_reg_sub( ctrl_p, OGMA_PHY_REG_ADDR_STATUS);

    /* check latched_link_down_flag */
    if ( ( value & ( 1U << OGMA_PHY_STATUS_REG_LINK_STATUS) ) == 0) {
        phy_link_status_p->latched_link_down_flag = OGMA_TRUE;

        /* Read PHY STATUS Register */
        value = ogma_get_phy_reg_sub( ctrl_p, OGMA_PHY_REG_ADDR_STATUS);

    }

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "CONTROL Register value %08x\n", tmp);

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "PHY STATUS register value %08x\n", value);

    /* Check Current Link Status */
    if ( ( value & ( 1U << OGMA_PHY_STATUS_REG_LINK_STATUS) ) != 0 ) {
        phy_link_status_p->up_flag = OGMA_TRUE;
    }

    /* check Auto-Negotiation Enable */
    if ( ( ( tmp & ( 1U << OGMA_PHY_CONTROL_REG_AUTO_NEGO_ENABLE) ) != 0) &&
         ( ( value & ( 1U << OGMA_PHY_STATUS_REG_AUTO_NEGO_ABILITY) ) != 0) ) {
        phy_link_status_p->auto_nego_enable_flag = OGMA_TRUE;
    }

    /* Check Current Autonegotiation Complete Status */
    if ( phy_link_status_p->up_flag &&
         phy_link_status_p->auto_nego_enable_flag &&
         ( ( value & ( 1U << OGMA_PHY_STATUS_REG_AUTO_NEGO_COMP) ) != 0) ) {
        phy_link_status_p->auto_nego_complete_flag = OGMA_TRUE;
    }

    /* start Check Current Link Speed */
    if ( phy_link_status_p->up_flag) {

        if ( phy_link_status_p->auto_nego_enable_flag == OGMA_FALSE) {

            /* Speed check */
            if ( ( ( tmp & ( 1U << OGMA_PHY_CONTROL_REG_SPEED_SELECTION_LSB) ) == 0) &&
                 ( ( tmp & ( 1U << OGMA_PHY_CONTROL_REG_SPEED_SELECTION_MSB) ) != 0) ) {

                phy_link_status_p->link_speed = OGMA_PHY_LINK_SPEED_1G;

            } else if ( ( ( tmp & ( 1U << OGMA_PHY_CONTROL_REG_SPEED_SELECTION_LSB) ) != 0) &&
                        ( ( tmp & ( 1U << OGMA_PHY_CONTROL_REG_SPEED_SELECTION_MSB) ) == 0) ) {

                phy_link_status_p->link_speed = OGMA_PHY_LINK_SPEED_100M;

            } else {

                phy_link_status_p->link_speed = OGMA_PHY_LINK_SPEED_10M;
            }

            /* Duplex check */
            if ( ( tmp & ( 1U << OGMA_PHY_CONTROL_REG_DUPLEX_MODE) ) == 0) {

                phy_link_status_p->half_duplex_flag = OGMA_TRUE;
            }

        } else if ( phy_link_status_p->auto_nego_complete_flag == OGMA_TRUE) {
            /* case auto_nego_enable_flag TRUE && auto_nego_complete_flag TRUE */

            /* Read MASTER-SLAVE Control Register */
            value = ogma_get_phy_reg_sub( ctrl_p,
                                          OGMA_PHY_REG_ADDR_MASTER_SLAVE_CONTROL);

            /* Read MASTER-SLAVE Status Register */
            tmp = ogma_get_phy_reg_sub( ctrl_p,
                                        OGMA_PHY_REG_ADDR_MASTER_SLAVE_STATUS);

            /* Check Current Link Speed */
            if ( ( ( value & ( 1U << OGMA_PHY_MSC_REG_1000BASE_FULL) ) != 0) &&
                 ( ( tmp & ( 1U << OGMA_PHY_MSS_REG_LP_1000BASE_FULL) ) != 0) ) {

                    phy_link_status_p->link_speed = OGMA_PHY_LINK_SPEED_1G;

            } else if ( ( ( value & ( 1U << OGMA_PHY_MSC_REG_1000BASE_HALF) ) != 0) &&
                        ( ( tmp & ( 1U << OGMA_PHY_MSS_REG_LP_1000BASE_HALF) ) != 0) ) {

                phy_link_status_p->link_speed = OGMA_PHY_LINK_SPEED_1G;

                phy_link_status_p->half_duplex_flag = OGMA_TRUE;

            } else {

                /* Read Auto-Negotiation Advertisement register */
                value = ogma_get_phy_reg_sub( ctrl_p,
                                              OGMA_PHY_REG_ADDR_AUTO_NEGO_ABILTY);

                /* Read Auto-Negotiation Link Partner Base Page Ability register */
                tmp = ogma_get_phy_reg_sub( ctrl_p,
                                             OGMA_PHY_REG_ADDR_AUTO_NEGO_LINK_PATNER_ABILTY);

                value = ( ( ( value & tmp) >> OGMA_PHY_ANA_REG_TAF) &
                          OGMA_PHY_ANA_REG_TAF_MASK);

                pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                             "TAF value %08x\n", value);

                if ( value & OGMA_PHY_TAF_REG_100BASE_FULL) { /* 100M full */

                    phy_link_status_p->link_speed = OGMA_PHY_LINK_SPEED_100M;

                } else if ( value & OGMA_PHY_TAF_REG_100BASE_HALF) { /* 100M half */

                    phy_link_status_p->link_speed = OGMA_PHY_LINK_SPEED_100M;

                    phy_link_status_p->half_duplex_flag = OGMA_TRUE;

                } else if ( value & OGMA_PHY_TAF_REG_10BASE_FULL) { /* 10M full */

                    phy_link_status_p->link_speed = OGMA_PHY_LINK_SPEED_10M;

                } else { /* value = OGMA_PHY_TAF_REG_10BASE_HALF 10M Half */

                    phy_link_status_p->link_speed = OGMA_PHY_LINK_SPEED_10M;

                    phy_link_status_p->half_duplex_flag = OGMA_TRUE;
                }

            }
        }
    } /* End Check Current Link Speed */

    /* Check LPI Capable */
    if ( phy_link_status_p->up_flag &&
         phy_link_status_p->auto_nego_complete_flag &&
         phy_link_status_p->link_speed != OGMA_PHY_LINK_SPEED_10M &&
         phy_link_status_p->half_duplex_flag == OGMA_FALSE &&
         ctrl_p->param.gmac_config.phy_interface != OGMA_PHY_INTERFACE_RMII) {

        /* Read EEE advertisement register */
        value = ogma_get_phy_mmd_reg_sub( ctrl_p,
                                          OGMA_PHY_DEV_ADDR_AUTO_NEGO,
                                          OGMA_PHY_AUTO_NEGO_REG_ADDR_EEE_ADVERTISE);

        /* Read EEE link partner ability register */
        tmp = ogma_get_phy_mmd_reg_sub( ctrl_p,
                                        OGMA_PHY_DEV_ADDR_AUTO_NEGO,
                                        OGMA_PHY_AUTO_NEGO_REG_ADDR_EEE_LP_ABILITY);

        exp = ( ( phy_link_status_p->link_speed == OGMA_PHY_LINK_SPEED_1G) ?
                OGMA_PHY_AUTO_NEGO_1000BASE_EEE:
                OGMA_PHY_AUTO_NEGO_100BASE_EEE);

        /* Check EEE Advertise and EEE LP Ability */
        if ( ( value & tmp & exp) != 0 ) {
            phy_link_status_p->lpi_capable_flag = OGMA_TRUE;
        }

        pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                     "EEE advertisement register        value %08x\n",
                     value);

        pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                     "EEE link partner ability register value %08x\n",
                     tmp);

        pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                     "EEE LPI EXP                       value %08x\n",
                     exp);
    }

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "phy_link_status_p->latched_link_down_flag %u\n",
                 phy_link_status_p->latched_link_down_flag);

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "phy_link_status_p->up_flag %u\n",
                 phy_link_status_p->up_flag);

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "phy_link_status_p->auto_nego_enable_flag %u\n",
                 phy_link_status_p->auto_nego_enable_flag);

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "phy_link_status_p->auto_nego_complete_flag %u\n",
                 phy_link_status_p->auto_nego_complete_flag);

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "phy_link_status_p->link_speed %u\n",
                 phy_link_status_p->link_speed);

    pfdep_print( PFDEP_DEBUG_LEVEL_DEBUG_MORE_DETAILED,
                 "phy_link_status_p->lpi_capable_flag %u\n",
                 phy_link_status_p->lpi_capable_flag);


    return OGMA_ERR_OK;
}

ogma_gmac_int_sbd_regs_t ogma_get_gmac_int_sbd_irq_enable (
    ogma_handle_t ogma_handle
    )
{
    ogma_gmac_int_sbd_regs_t gmac_int_sbd_regs = {0};
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return gmac_int_sbd_regs;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return gmac_int_sbd_regs;
    }

    /* Read IER register */
    gmac_int_sbd_regs.base = ogma_get_mac_reg( ctrl_p,
                                               OGMA_GMAC_REG_ADDR_IER);
    /* Read IMR register */
    gmac_int_sbd_regs.extended = ( ( ~( ogma_get_mac_reg( ctrl_p,
                                                          OGMA_GMAC_REG_ADDR_IMR) ) ) &
                                   OGMA_GMAC_INT_SBD_IRQ_ISR_ALL);



    return gmac_int_sbd_regs;
}

ogma_gmac_int_sbd_regs_t ogma_get_gmac_int_sbd_irq_status_non_clear (
    ogma_handle_t ogma_handle,
    ogma_bool mask_flag
    )
{
    ogma_gmac_int_sbd_regs_t gmac_int_sbd_regs = {0};
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return gmac_int_sbd_regs;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return gmac_int_sbd_regs;
    }

    /* Read SR register */
    gmac_int_sbd_regs.base = ogma_get_mac_reg( ctrl_p,
                                               OGMA_GMAC_REG_ADDR_SR);

    /* Read ISR register */
    gmac_int_sbd_regs.extended = ogma_get_mac_reg( ctrl_p,
                                                   OGMA_GMAC_REG_ADDR_ISR);

    if ( mask_flag) {

        /* Read IER register */
        gmac_int_sbd_regs.base &= ( ogma_get_mac_reg( ctrl_p,
                                                      OGMA_GMAC_REG_ADDR_IER) |
                                    ( ~OGMA_GMAC_INT_SBD_IRQ_IER_ALL) );
        /* Read IMR register */
        gmac_int_sbd_regs.extended &= ( ( ~( ogma_get_mac_reg( ctrl_p,
                                                               OGMA_GMAC_REG_ADDR_IMR) ) ) &
                                        OGMA_GMAC_INT_SBD_IRQ_ISR_ALL);
    }


    return gmac_int_sbd_regs;
}

ogma_err_t ogma_clear_gmac_int_sbd_irq_status (
    ogma_handle_t ogma_handle,
    ogma_gmac_int_sbd_regs_t int_sbd_regs
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    /* Write Clear SR register */
    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_SR,
                      ( int_sbd_regs.base & OGMA_GMAC_INT_SBD_IRQ_SR_WC_ALL) );


    return OGMA_ERR_OK;
}

ogma_err_t ogma_enable_gmac_int_sbd_irq (
    ogma_handle_t ogma_handle,
    ogma_gmac_int_sbd_regs_t int_sbd_regs
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    /* Read IER register for No Change Value Keep */
    value = ogma_get_mac_reg( ctrl_p,
                              OGMA_GMAC_REG_ADDR_IER);
    /* Write IER register */
    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_IER,
                      ( value | ( int_sbd_regs.base & OGMA_GMAC_INT_SBD_IRQ_IER_ALL) ) );

    /* Read IMR register for No Change Value Keep */
    value = ogma_get_mac_reg( ctrl_p,
                              OGMA_GMAC_REG_ADDR_IMR);
    /* Write IMR register */
    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_IMR,
                      ( value & ( ~( int_sbd_regs.extended &
                                     OGMA_GMAC_INT_SBD_IRQ_ISR_ALL) ) ) );


    return OGMA_ERR_OK;
}

ogma_err_t ogma_disable_gmac_int_sbd_irq (
    ogma_handle_t ogma_handle,
    ogma_gmac_int_sbd_regs_t int_sbd_regs
    )
{
    ogma_uint32 value;
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    /* Read IER register for No Change Value Keep */
    value = ogma_get_mac_reg( ctrl_p,
                              OGMA_GMAC_REG_ADDR_IER);
    /* Write IER register */
    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_IER,
                      ( value & ( ~(int_sbd_regs.base) ) ) );

    /* Read IMR register for No Change Value Keep */
    value = ogma_get_mac_reg( ctrl_p,
                              OGMA_GMAC_REG_ADDR_IMR);
    /* Write IMR register */
    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_IMR,
                      ( value | ( int_sbd_regs.extended &
                                  OGMA_GMAC_INT_SBD_IRQ_ISR_ALL) ) );


    return OGMA_ERR_OK;
}

ogma_err_t ogma_get_gmac_rgmii_status_reg (
    ogma_handle_t ogma_handle,
    ogma_uint32 *value_p
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ( ctrl_p == NULL) ||
         ( value_p == NULL) ){
        return OGMA_ERR_PARAM;
    }

    if ( !ctrl_p->param.use_gmac_flag) {
        return OGMA_ERR_NOTAVAIL;
    }

    /* Read RGMII Status Register */
    *value_p = ogma_get_mac_reg( ctrl_p,
                                 OGMA_GMAC_REG_ADDR_RSR);


    return OGMA_ERR_OK;
}

#ifdef OGMA_CONFIG_USE_READ_GMAC_STAT
ogma_err_t ogma_read_gmac_stat (
    ogma_handle_t ogma_handle,
    ogma_uint32 *value_p,
    ogma_bool reset_flag
    )
{

    ogma_uint i;

    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ( ctrl_p == NULL) ||
         ( value_p == NULL) ) {
        return OGMA_ERR_PARAM;
    }


    for ( i = 0;
          i < sizeof(ogma_gmac_mmc_reg_info)/sizeof(ogma_gmac_mmc_reg_info[0] );
          i++) {

        value_p[i] = ogma_get_mac_reg(ctrl_p,
                                      ogma_gmac_mmc_reg_info[i].addr);
    }

    if ( reset_flag) {
        /* Reset all counters. */
        ogma_set_mac_reg(ctrl_p, OGMA_GMAC_REG_ADDR_MMC_CNTL, 1);
    }


    return OGMA_ERR_OK;
}
#endif /* OGMA_CONFIG_USE_READ_GMAC_STAT */

ogma_err_t ogma_reset_gmac_stat (
    ogma_handle_t ogma_handle
    )
{
    ogma_ctrl_t *ctrl_p = (ogma_ctrl_t *)ogma_handle;

    if ( ctrl_p == NULL) {
        return OGMA_ERR_PARAM;
    }


    /* Reset all counters. */
    ogma_set_mac_reg(ctrl_p, OGMA_GMAC_REG_ADDR_MMC_CNTL, 1);


    return OGMA_ERR_OK;
}

ogma_err_t ogma_softreset_gmac (
    ogma_ctrl_t *ctrl_p
    )
{
    ogma_uint32 value;
    pfdep_err_t pfdep_err;

    /* F_GMAC4MT soft reset*/
    ogma_set_mac_reg( ctrl_p,
                      OGMA_GMAC_REG_ADDR_BMR,
                      OGMA_GMAC_BMR_REG_RESET);
    /* Wait soft reset */
    pfdep_err = pfdep_msleep( 1);

    if ( pfdep_err == PFDEP_ERR_INTERRUPT) {

        pfdep_print( PFDEP_DEBUG_LEVEL_FATAL,
                     "Wait for BMR soft reset error.\n");

        return OGMA_ERR_INTERRUPT;
    }

    /* Read F_GMAC4MT BMR */
    value = ogma_get_mac_reg( ctrl_p,
                              OGMA_GMAC_REG_ADDR_BMR);

    /* check software reset result*/
    if ( value & OGMA_GMAC_BMR_REG_SWR) {
        return OGMA_ERR_AGAIN;
    }

    return OGMA_ERR_OK;
}
