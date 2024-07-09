/** @file
 *
 *  Synopsys DesignWare MIPI DSI2 controller driver
 *
 *  This was ported from U-Boot downstream.
 *
 *  After all features are merged in and tested, the code should ideally
 *  be refactored to meet EDK II conventions.
 *
 *  U-Boot file: drivers/video/drm/dw_mipi_dsi2.c
 *
 *  Copyright (C) 2021 Rockchip Electronics Co., Ltd
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *
 *  This code can be relicensed as BSD-2-Clause-Patent if permission
 *  is granted by the original authors.
 *
 **/

#include <Uefi/UefiBaseType.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DrmModes.h>
#include <Library/MediaBusFormat.h>
#include <Library/DrmModes.h>
#include <Library/Vop2Regs.h>
#include <Library/uboot-env.h>
#include <Library/drm_mipi_dsi.h>

#include <Protocol/RockchipConnectorProtocol.h>
#include <Protocol/RockchipDsiPanel.h>

#include "rockchip_phy.h"

#define UPDATE(v, h, l)		(((v) << (l)) & GENMASK((h), (l)))

#define DSI2_PWR_UP			0x000c
#define RESET				0
#define POWER_UP			BIT(0)
#define CMD_TX_MODE(x)			UPDATE(x,  24,  24)
#define DSI2_SOFT_RESET			0x0010
#define SYS_RSTN			BIT(2)
#define PHY_RSTN			BIT(1)
#define IPI_RSTN			BIT(0)
#define INT_ST_MAIN			0x0014
#define DSI2_MODE_CTRL			0x0018
#define DSI2_MODE_STATUS		0x001c
#define DSI2_CORE_STATUS		0x0020
#define PRI_RD_DATA_AVAIL		BIT(26)
#define PRI_FIFOS_NOT_EMPTY		BIT(25)
#define PRI_BUSY			BIT(24)
#define CRI_RD_DATA_AVAIL		BIT(18)
#define CRT_FIFOS_NOT_EMPTY		BIT(17)
#define CRI_BUSY			BIT(16)
#define IPI_FIFOS_NOT_EMPTY		BIT(9)
#define IPI_BUSY			BIT(8)
#define CORE_FIFOS_NOT_EMPTY		BIT(1)
#define CORE_BUSY			BIT(0)
#define MANUAL_MODE_CFG			0x0024
#define MANUAL_MODE_EN			BIT(0)
#define DSI2_TIMEOUT_HSTX_CFG		0x0048
#define TO_HSTX(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_HSTXRDY_CFG	0x004c
#define TO_HSTXRDY(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_LPRX_CFG		0x0050
#define TO_LPRXRDY(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_LPTXRDY_CFG	0x0054
#define TO_LPTXRDY(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_LPTXTRIG_CFG	0x0058
#define TO_LPTXTRIG(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_LPTXULPS_CFG	0x005c
#define TO_LPTXULPS(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_BTA_CFG		0x60
#define TO_BTA(x)			UPDATE(x, 15, 0)

#define DSI2_PHY_MODE_CFG		0x0100
#define PPI_WIDTH(x)			UPDATE(x, 9, 8)
#define PHY_LANES(x)			UPDATE(x - 1, 5, 4)
#define PHY_TYPE(x)			UPDATE(x, 0, 0)
#define DSI2_PHY_CLK_CFG		0X0104
#define PHY_LPTX_CLK_DIV(x)		UPDATE(x, 12, 8)
#define NON_CONTINUOUS_CLK		BIT(0)
#define DSI2_PHY_LP2HS_MAN_CFG		0x010c
#define PHY_LP2HS_TIME(x)		UPDATE(x, 28, 0)
#define DSI2_PHY_HS2LP_MAN_CFG		0x0114
#define PHY_HS2LP_TIME(x)		UPDATE(x, 28, 0)
#define DSI2_PHY_MAX_RD_T_MAN_CFG	0x011c
#define PHY_MAX_RD_TIME(x)		UPDATE(x, 26, 0)
#define DSI2_PHY_ESC_CMD_T_MAN_CFG	0x0124
#define PHY_ESC_CMD_TIME(x)		UPDATE(x, 28, 0)
#define DSI2_PHY_ESC_BYTE_T_MAN_CFG	0x012c
#define PHY_ESC_BYTE_TIME(x)		UPDATE(x, 28, 0)

#define DSI2_PHY_IPI_RATIO_MAN_CFG	0x0134
#define PHY_IPI_RATIO(x)		UPDATE(x, 21, 0)
#define DSI2_PHY_SYS_RATIO_MAN_CFG	0x013C
#define PHY_SYS_RATIO(x)		UPDATE(x, 16, 0)

#define DSI2_DSI_GENERAL_CFG		0x0200
#define BTA_EN				BIT(1)
#define EOTP_TX_EN			BIT(0)
#define DSI2_DSI_VCID_CFG		0x0204
#define TX_VCID(x)			UPDATE(x, 1, 0)
#define DSI2_DSI_SCRAMBLING_CFG		0x0208
#define SCRAMBLING_SEED(x)		UPDATE(x, 31, 16)
#define SCRAMBLING_EN			BIT(0)
#define DSI2_DSI_VID_TX_CFG		0x020c
#define LPDT_DISPLAY_CMD_EN		BIT(20)
#define BLK_VFP_HS_EN			BIT(14)
#define BLK_VBP_HS_EN			BIT(13)
#define BLK_VSA_HS_EN			BIT(12)
#define BLK_HFP_HS_EN			BIT(6)
#define BLK_HBP_HS_EN			BIT(5)
#define BLK_HSA_HS_EN			BIT(4)
#define VID_MODE_TYPE(x)		UPDATE(x, 1, 0)
#define DSI2_CRI_TX_HDR			0x02c0
#define CMD_TX_MODE(x)			UPDATE(x, 24, 24)
#define DSI2_CRI_TX_PLD			0x02c4
#define DSI2_CRI_RX_HDR			0x02c8
#define DSI2_CRI_RX_PLD			0x02cc

#define DSI2_IPI_COLOR_MAN_CFG		0x0300
#define IPI_DEPTH(x)			UPDATE(x, 7, 4)
#define IPI_DEPTH_5_6_5_BITS		0x02
#define IPI_DEPTH_6_BITS		0x03
#define IPI_DEPTH_8_BITS		0x05
#define IPI_DEPTH_10_BITS		0x06
#define IPI_FORMAT(x)			UPDATE(x, 3, 0)
#define IPI_FORMAT_RGB			0x0
#define IPI_FORMAT_DSC			0x0b
#define DSI2_IPI_VID_HSA_MAN_CFG	0x0304
#define VID_HSA_TIME(x)			UPDATE(x, 29, 0)
#define DSI2_IPI_VID_HBP_MAN_CFG	0x030c
#define VID_HBP_TIME(x)			UPDATE(x, 29, 0)
#define DSI2_IPI_VID_HACT_MAN_CFG	0x0314
#define VID_HACT_TIME(x)		UPDATE(x, 29, 0)
#define DSI2_IPI_VID_HLINE_MAN_CFG	0x031c
#define VID_HLINE_TIME(x)		UPDATE(x, 29, 0)
#define DSI2_IPI_VID_VSA_MAN_CFG	0x0324
#define VID_VSA_LINES(x)		UPDATE(x, 9, 0)
#define DSI2_IPI_VID_VBP_MAN_CFG	0X032C
#define VID_VBP_LINES(x)		UPDATE(x, 9, 0)
#define DSI2_IPI_VID_VACT_MAN_CFG	0X0334
#define VID_VACT_LINES(x)		UPDATE(x, 13, 0)
#define DSI2_IPI_VID_VFP_MAN_CFG	0X033C
#define VID_VFP_LINES(x)		UPDATE(x, 9, 0)
#define DSI2_IPI_PIX_PKT_CFG		0x0344
#define MAX_PIX_PKT(x)			UPDATE(x, 15, 0)

#define DSI2_INT_ST_PHY			0x0400
#define DSI2_INT_MASK_PHY		0x0404
#define DSI2_INT_ST_TO			0x0410
#define DSI2_INT_MASK_TO		0x0414
#define DSI2_INT_ST_ACK			0x0420
#define DSI2_INT_MASK_ACK		0x0424
#define DSI2_INT_ST_IPI			0x0430
#define DSI2_INT_MASK_IPI		0x0434
#define DSI2_INT_ST_FIFO		0x0440
#define DSI2_INT_MASK_FIFO		0x0444
#define DSI2_INT_ST_PRI			0x0450
#define DSI2_INT_MASK_PRI		0x0454
#define DSI2_INT_ST_CRI			0x0460
#define DSI2_INT_MASK_CRI		0x0464
#define DSI2_INT_FORCE_CRI		0x0468
#define DSI2_MAX_REGISGER		DSI2_INT_FORCE_CRI

#define CMD_PKT_STATUS_TIMEOUT_US	1000
#define MODE_STATUS_TIMEOUT_US		20000
#define SYS_CLK				351000000LL
#define PSEC_PER_SEC			1000000000000LL
#define USEC_PER_SEC			1000000L
#define MSEC_PER_SEC			1000L

#define GRF_REG_FIELD(reg, lsb, msb)	(((reg) << 16) | ((lsb) << 8) | (msb))

enum vid_mode_type {
	VID_MODE_TYPE_NON_BURST_SYNC_PULSES,
	VID_MODE_TYPE_NON_BURST_SYNC_EVENTS,
	VID_MODE_TYPE_BURST,
};

enum mode_ctrl {
	IDLE_MODE,
	AUTOCALC_MODE,
	COMMAND_MODE,
	VIDEO_MODE,
	DATA_STREAM_MODE,
	VIDE_TEST_MODE,
	DATA_STREAM_TEST_MODE,
};

enum grf_reg_fields {
	TXREQCLKHS_EN,
	GATING_EN,
	IPI_SHUTDN,
	IPI_COLORM,
	IPI_COLOR_DEPTH,
	IPI_FORMAT,
	MAX_FIELDS,
};

enum phy_type {
	DPHY,
	CPHY,
};

enum ppi_width {
	PPI_WIDTH_8_BITS,
	PPI_WIDTH_16_BITS,
	PPI_WIDTH_32_BITS,
};

#pragma pack(1)
struct rockchip_cmd_header {
	u8 data_type;
	u8 delay_ms;
	u8 payload_length;
};
#pragma pack()

struct rockchip_cmd_desc {
	struct rockchip_cmd_header header;
	const u8 *payload;
};

struct rockchip_panel_cmds {
	struct rockchip_cmd_desc *cmds;
	int cmd_cnt;
};

struct dw_mipi_dsi2_plat_data {
	const u32 *dsi0_grf_reg_fields;
	const u32 *dsi1_grf_reg_fields;
	unsigned long long dphy_max_bit_rate_per_lane;
	unsigned long long cphy_max_symbol_rate_per_lane;
};

struct mipi_dcphy {
	/* Non-SNPS PHY */
	struct rockchip_phy *phy;

	u16 input_div;
	u16 feedback_div;
};

/**
 * struct mipi_dphy_configure - MIPI D-PHY configuration set
 *
 * This structure is used to represent the configuration state of a
 * MIPI D-PHY phy.
 */
struct mipi_dphy_configure {
	unsigned int		clk_miss;
	unsigned int		clk_post;
	unsigned int		clk_pre;
	unsigned int		clk_prepare;
	unsigned int		clk_settle;
	unsigned int		clk_term_en;
	unsigned int		clk_trail;
	unsigned int		clk_zero;
	unsigned int		d_term_en;
	unsigned int		eot;
	unsigned int		hs_exit;
	unsigned int		hs_prepare;
	unsigned int		hs_settle;
	unsigned int		hs_skip;
	unsigned int		hs_trail;
	unsigned int		hs_zero;
	unsigned int		init;
	unsigned int		lpx;
	unsigned int		ta_get;
	unsigned int		ta_go;
	unsigned int		ta_sure;
	unsigned int		wakeup;
	unsigned long		hs_clk_rate;
	unsigned long		lp_clk_rate;
	unsigned char		lanes;
};

struct dw_mipi_dsi2 {
	UINT32 Signature;
	ROCKCHIP_CONNECTOR_PROTOCOL connector;
	UINTN base;
	UINTN grf;
	int id;
	struct dw_mipi_dsi2 *master;
	struct dw_mipi_dsi2 *slave;
	bool prepared;

	bool auto_calc_mode;
	bool c_option;
	bool dsc_enable;
	bool scrambling_en;
	unsigned int slice_width;
	unsigned int slice_height;
	u32 version_major;
	u32 version_minor;

	unsigned int lane_hs_rate; /* Kbps/Ksps per lane */
	u32 channel;
	u32 lanes;
	u32 format;
	u32 mode_flags;
	u64 mipi_pixel_rate;
	struct mipi_dcphy dcphy;
	DRM_DISPLAY_MODE mode;
	bool data_swap;

	struct mipi_dsi_host host;
	struct mipi_dsi_device *device;
	struct mipi_dphy_configure mipi_dphy_cfg;
	const struct dw_mipi_dsi2_plat_data *pdata;
	struct drm_dsc_picture_parameter_set *pps;

	ROCKCHIP_DSI_PANEL_PROTOCOL *RockchipDsiPanel;
};

#define DW_MIPI_DSI2_SIGNATURE          SIGNATURE_32 ('D', 'W', 'd', '2')

#define DW_MIPI_DSI2_FROM_CONNECTOR_PROTOCOL(a) \
	CR (a, struct dw_mipi_dsi2, connector, DW_MIPI_DSI2_SIGNATURE)

#define DW_MIPI_DSI2_FROM_MIPI_DSI_HOST(a) \
	CR (a, struct dw_mipi_dsi2, host, DW_MIPI_DSI2_SIGNATURE)

static int rockchip_panel_parse_cmds(const u8 *data, int length,
				     struct rockchip_panel_cmds *pcmds)
{
	int len;
	const u8 *buf;
	const struct rockchip_cmd_header *header;
	int i, cnt = 0;

	/* scan commands */
	cnt = 0;
	buf = data;
	len = length;
	while (len > sizeof(*header)) {
		header = (const struct rockchip_cmd_header *)buf;
		buf += sizeof(*header) + header->payload_length;
		len -= sizeof(*header) + header->payload_length;
		cnt++;
	}

	pcmds->cmds = calloc(cnt, sizeof(struct rockchip_cmd_desc));
	if (!pcmds->cmds)
		return -ENOMEM;

	pcmds->cmd_cnt = cnt;

	buf = data;
	len = length;
	for (i = 0; i < cnt; i++) {
		struct rockchip_cmd_desc *desc = &pcmds->cmds[i];

		header = (const struct rockchip_cmd_header *)buf;
		length -= sizeof(*header);
		buf += sizeof(*header);
		desc->header.data_type = header->data_type;
		desc->header.delay_ms = header->delay_ms;
		desc->header.payload_length = header->payload_length;
		desc->payload = buf;
		buf += header->payload_length;
		length -= header->payload_length;
	}

	return 0;
}

static int rockchip_panel_send_dsi_cmds(struct mipi_dsi_device *dsi,
					struct rockchip_panel_cmds *cmds)
{
	int i, ret;
	struct drm_dsc_picture_parameter_set *pps = NULL;

	if (!cmds)
		return -EINVAL;

	for (i = 0; i < cmds->cmd_cnt; i++) {
		struct rockchip_cmd_desc *desc = &cmds->cmds[i];
		const struct rockchip_cmd_header *header = &desc->header;

		switch (header->data_type) {
		case MIPI_DSI_COMPRESSION_MODE:
			ret = mipi_dsi_compression_mode(dsi, desc->payload[0]);
			break;
		case MIPI_DSI_GENERIC_SHORT_WRITE_0_PARAM:
		case MIPI_DSI_GENERIC_SHORT_WRITE_1_PARAM:
		case MIPI_DSI_GENERIC_SHORT_WRITE_2_PARAM:
		case MIPI_DSI_GENERIC_LONG_WRITE:
			ret = mipi_dsi_generic_write(dsi, desc->payload,
						     header->payload_length);
			break;
		case MIPI_DSI_DCS_SHORT_WRITE:
		case MIPI_DSI_DCS_SHORT_WRITE_PARAM:
		case MIPI_DSI_DCS_LONG_WRITE:
			ret = mipi_dsi_dcs_write_buffer(dsi, desc->payload,
							header->payload_length);
			break;
		case MIPI_DSI_PICTURE_PARAMETER_SET:
			pps = AllocateZeroPool(sizeof(*pps));
			if (!pps)
				return -ENOMEM;

			memcpy(pps, desc->payload, header->payload_length);
			ret = mipi_dsi_picture_parameter_set(dsi, pps);
			free(pps);
			break;
		default:
			printf("unsupport command data type: %d\n",
			       header->data_type);
			return -EINVAL;
		}

		if (ret < 0) {
			printf("failed to write cmd%d: %d\n", i, ret);
			return ret;
		}

		if (header->delay_ms)
			mdelay(header->delay_ms);
	}

	return 0;
}

static int rockchip_panel_init(struct dw_mipi_dsi2 *dsi2)
{
	ROCKCHIP_DSI_PANEL_PROTOCOL *Panel = dsi2->RockchipDsiPanel;
	const void *data;
	int len = 0;
	int ret;
	struct rockchip_panel_cmds *on_cmds;

	Panel->Prepare(Panel);

	data = Panel->InitSequence;
	len = Panel->InitSequenceLength;

	if (data) {
		on_cmds = calloc(1, sizeof(*on_cmds));
		if (!on_cmds)
			return -ENOMEM;

		ret = rockchip_panel_parse_cmds(data, len, on_cmds);
		if (ret) {
			printf("failed to parse panel init sequence\n");
			goto free_on_cmds;
		}

		ret = rockchip_panel_send_dsi_cmds(dsi2->device, on_cmds);
		if (ret)
			printf("failed to send on cmds: %d\n", ret);
	}

	return 0;

free_on_cmds:
	free(on_cmds);
	return ret;
}

static inline void dsi_write(struct dw_mipi_dsi2 *dsi2, u32 reg, u32 val)
{
	writel(val, dsi2->base + reg);
}

static inline u32 dsi_read(struct dw_mipi_dsi2 *dsi2, u32 reg)
{
	return readl(dsi2->base + reg);
}

static inline void dsi_update_bits(struct dw_mipi_dsi2 *dsi2,
				   u32 reg, u32 mask, u32 val)
{
	u32 orig, tmp;

	orig = dsi_read(dsi2, reg);
	tmp = orig & ~mask;
	tmp |= val & mask;
	dsi_write(dsi2, reg, tmp);
}

static void grf_field_write(struct dw_mipi_dsi2 *dsi2, enum grf_reg_fields index,
			    unsigned int val)
{
	const u32 field = dsi2->id ? dsi2->pdata->dsi1_grf_reg_fields[index] :
			  dsi2->pdata->dsi0_grf_reg_fields[index];
	u16 reg;
	u8 msb, lsb;

	if (!field)
		return;

	reg = (field >> 16) & 0xffff;
	lsb = (field >>  8) & 0xff;
	msb = (field >>  0) & 0xff;

	regmap_write(dsi2->grf, reg, GENMASK(msb, lsb) << 16 | val << lsb);
}

static unsigned long dw_mipi_dsi2_get_lane_rate(struct dw_mipi_dsi2 *dsi2)
{
	const DRM_DISPLAY_MODE *mode = &dsi2->mode;
	ROCKCHIP_DSI_PANEL_PROTOCOL *Panel = dsi2->RockchipDsiPanel;
	u64 max_lane_rate, lane_rate;
	unsigned int value;
	int bpp, lanes;
	u64 tmp;

	max_lane_rate = (dsi2->c_option) ?
			dsi2->pdata->cphy_max_symbol_rate_per_lane :
			dsi2->pdata->dphy_max_bit_rate_per_lane;

	/*
	 * optional override of the desired bandwidth
	 * High-Speed mode: Differential and terminated: 80Mbps ~ 4500 Mbps
	 */
	value = Panel->DsiLaneRate;
	if (value >= 80000 && value <= 4500000)
		return value * MSEC_PER_SEC;
	else if (value >= 80 && value <= 4500)
		return value * USEC_PER_SEC;

	bpp = mipi_dsi_pixel_format_to_bpp(dsi2->format);
	if (bpp < 0)
		bpp = 24;

	lanes = dsi2->slave ? dsi2->lanes * 2 : dsi2->lanes;
	tmp = (u64)mode->CrtcClock * 1000 * bpp;
	do_div(tmp, lanes);

	if (dsi2->c_option)
		tmp = DIV_ROUND_CLOSEST(tmp * 100, 228);

	/* set BW a little larger only in video burst mode in
	 * consideration of the protocol overhead and HS mode
	 * switching to BLLP mode, take 1 / 0.9, since Mbps must
	 * big than bandwidth of RGB
	 */
	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_BURST) {
		tmp *= 10;
		do_div(tmp, 9);
	}

	if (tmp > max_lane_rate)
		lane_rate = max_lane_rate;
	else
		lane_rate = tmp;

	return lane_rate;
}

static int cri_fifos_wait_avail(struct dw_mipi_dsi2 *dsi2)
{
	u32 sts, mask;
	int ret;

	mask = CRI_BUSY | CRT_FIFOS_NOT_EMPTY;
	ret = readl_poll_timeout(dsi2->base + DSI2_CORE_STATUS,
				 sts, !(sts & mask),
				 CMD_PKT_STATUS_TIMEOUT_US);
	if (ret < 0) {
		printf("command interface is busy: 0x%x\n", sts);
		return ret;
	}

	return 0;
}

static int dw_mipi_dsi2_read_from_fifo(struct dw_mipi_dsi2 *dsi2,
				      const struct mipi_dsi_msg *msg)
{
	u8 *payload = msg->rx_buf;
	u8 data_type;
	u16 wc;
	int i, j, ret, len = msg->rx_len;
	unsigned int vrefresh = DrmModeVRefresh(&dsi2->mode);
	u32 val;

	ret = readl_poll_timeout(dsi2->base + DSI2_CORE_STATUS,
				 val, val & CRI_RD_DATA_AVAIL,
				 DIV_ROUND_UP(1000000, vrefresh));
	if (ret) {
		printf("CRI has no available read data\n");
		return ret;
	}

	val = dsi_read(dsi2, DSI2_CRI_RX_HDR);
	data_type = val & 0x3f;

	if (mipi_dsi_packet_format_is_short(data_type)) {
		for (i = 0; i < len && i < 2; i++)
			payload[i] = (val >> (8 * (i + 1))) & 0xff;

		return 0;
	}

	wc = (val >> 8) & 0xffff;
	/* Receive payload */
	for (i = 0; i < len && i < wc; i += 4) {
		val = dsi_read(dsi2, DSI2_CRI_RX_PLD);
		for (j = 0; j < 4 && j + i < len && j + i < wc; j++)
			payload[i + j] = val >> (8 * j);
	}

	return 0;
}

static ssize_t dw_mipi_dsi2_transfer(struct dw_mipi_dsi2 *dsi2,
				    const struct mipi_dsi_msg *msg)
{
	struct mipi_dsi_packet packet;
	int ret;
	int val;
	u32 mode;

	dsi_update_bits(dsi2, DSI2_DSI_VID_TX_CFG, LPDT_DISPLAY_CMD_EN,
			msg->flags & MIPI_DSI_MSG_USE_LPM ?
			LPDT_DISPLAY_CMD_EN : 0);

	/* create a packet to the DSI protocol */
	ret = mipi_dsi_create_packet(&packet, msg);
	if (ret) {
		printf("failed to create packet: %d\n", ret);
		return ret;
	}

	/* check cri interface is not busy */
	ret = cri_fifos_wait_avail(dsi2);
	if (ret)
		return ret;

	/* Send payload */
	while (DIV_ROUND_UP(packet.payload_length, 4)) {
		if (packet.payload_length < 4) {
			/* send residu payload */
			val = 0;
			memcpy(&val, packet.payload, packet.payload_length);
			dsi_write(dsi2, DSI2_CRI_TX_PLD, val);
			packet.payload_length = 0;
		} else {
			val = get_unaligned_le32(packet.payload);
			dsi_write(dsi2, DSI2_CRI_TX_PLD, val);
			packet.payload += 4;
			packet.payload_length -= 4;
		}
	}

	/* Send packet header */
	mode = CMD_TX_MODE(msg->flags & MIPI_DSI_MSG_USE_LPM ? 1 : 0);
	val = get_unaligned_le32(packet.header);
	dsi_write(dsi2, DSI2_CRI_TX_HDR, mode | val);

	ret = cri_fifos_wait_avail(dsi2);
	if (ret)
		return ret;

	if (msg->rx_len) {
		ret = dw_mipi_dsi2_read_from_fifo(dsi2, msg);
		if (ret < 0)
			return ret;
	}

	if (dsi2->slave) {
		ret = dw_mipi_dsi2_transfer(dsi2->slave, msg);
		if (ret < 0)
			return ret;
	}

	return msg->rx_len ? msg->rx_len : msg->tx_len;
}

static void dw_mipi_dsi2_ipi_color_coding_cfg(struct dw_mipi_dsi2 *dsi2)
{
	u32 val, color_depth;

	switch (dsi2->format) {
	case MIPI_DSI_FMT_RGB666:
	case MIPI_DSI_FMT_RGB666_PACKED:
		color_depth = IPI_DEPTH_6_BITS;
		break;
	case MIPI_DSI_FMT_RGB565:
		color_depth = IPI_DEPTH_5_6_5_BITS;
		break;
	case MIPI_DSI_FMT_RGB888:
	default:
		color_depth = IPI_DEPTH_8_BITS;
		break;
	}

	val = IPI_DEPTH(color_depth) |
	      IPI_FORMAT(dsi2->dsc_enable ? IPI_FORMAT_DSC : IPI_FORMAT_RGB);
	dsi_write(dsi2, DSI2_IPI_COLOR_MAN_CFG, val);
	grf_field_write(dsi2, IPI_COLOR_DEPTH, color_depth);

	if (dsi2->dsc_enable)
		grf_field_write(dsi2, IPI_FORMAT, IPI_FORMAT_DSC);
}

static void dw_mipi_dsi2_ipi_set(struct dw_mipi_dsi2 *dsi2)
{
	DRM_DISPLAY_MODE *mode = &dsi2->mode;
	u32 hline, hsa, hbp, hact;
	u64 hline_time, hsa_time, hbp_time, hact_time, tmp;
	u64 pixel_clk, phy_hs_clk;
	u32 vact, vsa, vfp, vbp;
	u16 val;

	if (dsi2->slave || dsi2->master)
		val = mode->HDisplay / 2;
	else
		val = mode->HDisplay;

	dsi_write(dsi2, DSI2_IPI_PIX_PKT_CFG, MAX_PIX_PKT(val));

	dw_mipi_dsi2_ipi_color_coding_cfg(dsi2);

	if (dsi2->auto_calc_mode)
		return;

	/*
	 * if the controller is intended to operate in data stream mode,
	 * no more steps are required.
	 */
	if (!(dsi2->mode_flags & MIPI_DSI_MODE_VIDEO))
		return;

	vact = mode->VDisplay;
	vsa = mode->VSyncEnd - mode->VSyncStart;
	vfp = mode->VSyncStart - mode->VDisplay;
	vbp = mode->VTotal - mode->VSyncEnd;
	hact = mode->HDisplay;
	hsa = mode->HSyncEnd - mode->HSyncStart;
	hbp = mode->HTotal - mode->HSyncEnd;
	hline = mode->HTotal;

	pixel_clk = mode->CrtcClock * MSEC_PER_SEC;

	if (dsi2->c_option)
		phy_hs_clk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 7);
	else
		phy_hs_clk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 16);

	tmp = hsa * phy_hs_clk;
	hsa_time = DIV_ROUND_CLOSEST(tmp << 16, pixel_clk);
	dsi_write(dsi2, DSI2_IPI_VID_HSA_MAN_CFG, VID_HSA_TIME(hsa_time));

	tmp = hbp * phy_hs_clk;
	hbp_time = DIV_ROUND_CLOSEST(tmp << 16, pixel_clk);
	dsi_write(dsi2, DSI2_IPI_VID_HBP_MAN_CFG, VID_HBP_TIME(hbp_time));

	tmp = hact * phy_hs_clk;
	hact_time = DIV_ROUND_CLOSEST(tmp << 16, pixel_clk);
	dsi_write(dsi2, DSI2_IPI_VID_HACT_MAN_CFG, VID_HACT_TIME(hact_time));

	tmp = hline * phy_hs_clk;
	hline_time = DIV_ROUND_CLOSEST(tmp << 16, pixel_clk);
	dsi_write(dsi2, DSI2_IPI_VID_HLINE_MAN_CFG, VID_HLINE_TIME(hline_time));

	dsi_write(dsi2, DSI2_IPI_VID_VSA_MAN_CFG, VID_VSA_LINES(vsa));
	dsi_write(dsi2, DSI2_IPI_VID_VBP_MAN_CFG, VID_VBP_LINES(vbp));
	dsi_write(dsi2, DSI2_IPI_VID_VACT_MAN_CFG, VID_VACT_LINES(vact));
	dsi_write(dsi2, DSI2_IPI_VID_VFP_MAN_CFG, VID_VFP_LINES(vfp));
}

static void dw_mipi_dsi2_set_vid_mode(struct dw_mipi_dsi2 *dsi2)
{
	u32 val = 0, mode;
	int ret;

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_HFP)
		val |= BLK_HFP_HS_EN;

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_HBP)
		val |= BLK_HBP_HS_EN;

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_HSA)
		val |= BLK_HSA_HS_EN;

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_BURST)
		val |= VID_MODE_TYPE_BURST;
	else if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_SYNC_PULSE)
		val |= VID_MODE_TYPE_NON_BURST_SYNC_PULSES;
	else
		val |= VID_MODE_TYPE_NON_BURST_SYNC_EVENTS;

	dsi_write(dsi2, DSI2_DSI_VID_TX_CFG, val);

	dsi_write(dsi2, DSI2_MODE_CTRL, VIDEO_MODE);
	ret = readl_poll_timeout(dsi2->base + DSI2_MODE_STATUS,
				 mode, mode & VIDEO_MODE,
				 MODE_STATUS_TIMEOUT_US);
	if (ret < 0)
		printf("failed to enter video mode\n");
}

static void dw_mipi_dsi2_set_data_stream_mode(struct dw_mipi_dsi2 *dsi2)
{
	u32 mode;
	int ret;

	dsi_write(dsi2, DSI2_MODE_CTRL, DATA_STREAM_MODE);
	ret = readl_poll_timeout(dsi2->base + DSI2_MODE_STATUS,
				 mode, mode & DATA_STREAM_MODE,
				 MODE_STATUS_TIMEOUT_US);
	if (ret < 0)
		printf("failed to enter data stream mode\n");
}

static void dw_mipi_dsi2_set_cmd_mode(struct dw_mipi_dsi2 *dsi2)
{
	u32 mode;
	int ret;

	dsi_write(dsi2, DSI2_MODE_CTRL, COMMAND_MODE);
	ret = readl_poll_timeout(dsi2->base + DSI2_MODE_STATUS,
				 mode, mode & COMMAND_MODE,
				 MODE_STATUS_TIMEOUT_US);
	if (ret < 0)
		printf("failed to enter cmd mode\n");
}

static void dw_mipi_dsi2_enable(struct dw_mipi_dsi2 *dsi2)
{
	u32 mode;
	int ret;

	dw_mipi_dsi2_ipi_set(dsi2);

	if (dsi2->auto_calc_mode) {
		dsi_write(dsi2, DSI2_MODE_CTRL, AUTOCALC_MODE);
		ret = readl_poll_timeout(dsi2->base + DSI2_MODE_STATUS,
					 mode, mode == IDLE_MODE,
					 MODE_STATUS_TIMEOUT_US);
		if (ret < 0)
			printf("auto calculation training failed\n");
	}

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO)
		dw_mipi_dsi2_set_vid_mode(dsi2);
	else
		dw_mipi_dsi2_set_data_stream_mode(dsi2);

	if (dsi2->slave)
	    dw_mipi_dsi2_enable(dsi2->slave);
}

static void dw_mipi_dsi2_disable(struct dw_mipi_dsi2 *dsi2)
{
	dsi_write(dsi2, DSI2_IPI_PIX_PKT_CFG, 0);
	dw_mipi_dsi2_set_cmd_mode(dsi2);

	if (dsi2->slave)
		dw_mipi_dsi2_disable(dsi2->slave);
}

static void dw_mipi_dsi2_post_disable(struct dw_mipi_dsi2 *dsi2)
{
	if (!dsi2->prepared)
		return;

	dsi_write(dsi2, DSI2_PWR_UP, RESET);

	if (dsi2->dcphy.phy)
		rockchip_phy_power_off(dsi2->dcphy.phy);

	dsi2->prepared = false;

	if (dsi2->slave)
		dw_mipi_dsi2_post_disable(dsi2->slave);
}

static int dw_mipi_dsi2_connector_pre_init(ROCKCHIP_CONNECTOR_PROTOCOL *conn,
					   DISPLAY_STATE *state)
{
	CONNECTOR_STATE *conn_state = &state->ConnectorState;
	struct dw_mipi_dsi2 *dsi2 = DW_MIPI_DSI2_FROM_CONNECTOR_PROTOCOL(conn);
	struct mipi_dsi_host *host = &dsi2->host;
	struct mipi_dsi_device *device;

	conn_state->Type = DRM_MODE_CONNECTOR_DSI;

	device = dsi2->device;
	if (!device)
		return -ENODEV;

	device->host = host;
	mipi_dsi_attach(device);

	return 0;
}

static int dw_mipi_dsi2_get_dsc_params_from_sink(struct dw_mipi_dsi2 *dsi2)
{
	struct udevice *dev = dsi2->device->dev;
	ROCKCHIP_DSI_PANEL_PROTOCOL *Panel = dsi2->RockchipDsiPanel;
	struct rockchip_cmd_header *header;
	struct drm_dsc_picture_parameter_set *pps = NULL;
	u8 *dsc_packed_pps;
	const void *data;
	int len;

	dsi2->c_option = Panel->CPhyEnable;
	dsi2->scrambling_en = Panel->ScramblingEnable;
	dsi2->dsc_enable = Panel->DscEnable;

	if (dsi2->slave) {
		dsi2->slave->c_option = dsi2->c_option;
		dsi2->slave->scrambling_en = dsi2->scrambling_en;
		dsi2->slave->dsc_enable = dsi2->dsc_enable;
	}

	if (!dsi2->dsc_enable)
		return 0;

	dsi2->slice_width = Panel->SliceWidth;
	dsi2->slice_height = Panel->SliceHeight;
	dsi2->version_major = Panel->VersionMajor;
	dsi2->version_minor = Panel->VersionMinor;

	data = Panel->InitSequence;
	len = Panel->InitSequenceLength;

	if (!data)
		return -EINVAL;

	while (len > sizeof(*header)) {
		header = (struct rockchip_cmd_header *)data;
		data += sizeof(*header);
		len -= sizeof(*header);

		if (header->payload_length > len)
			return -EINVAL;

		if (header->data_type == MIPI_DSI_PICTURE_PARAMETER_SET) {
			dsc_packed_pps = calloc(1, header->payload_length);
			if (!dsc_packed_pps)
				return -ENOMEM;

			memcpy(dsc_packed_pps, data, header->payload_length);
			pps = (struct drm_dsc_picture_parameter_set *)dsc_packed_pps;
			break;
		}

		data += header->payload_length;
		len -= header->payload_length;
	}

	if (!pps) {
		printf("not found dsc pps definition\n");
		return -EINVAL;
	}

	dsi2->pps = pps;

	if (dsi2->slave) {
		u16 pic_width = be16_to_cpu(pps->pic_width) / 2;

		dsi2->pps->pic_width = cpu_to_be16(pic_width);
		printf("dsc pic_width change from %d to %d\n", pic_width * 2, pic_width);
	}

	return 0;
}

static int dw_mipi_dsi2_connector_init(ROCKCHIP_CONNECTOR_PROTOCOL *conn, DISPLAY_STATE *state)
{
	CONNECTOR_STATE *conn_state = &state->ConnectorState;
	CRTC_STATE *cstate = &state->CrtcState;
	struct dw_mipi_dsi2 *dsi2 = DW_MIPI_DSI2_FROM_CONNECTOR_PROTOCOL(conn);
	struct rockchip_phy *phy = NULL;
	struct udevice *phy_dev;
	struct udevice *dev;
	int ret;

	conn_state->OutputMode = ROCKCHIP_OUT_MODE_P888;
	conn_state->ColorSpace = V4L2_COLORSPACE_DEFAULT;
	conn_state->OutputInterface |=
		dsi2->id ? VOP_OUTPUT_IF_MIPI1 : VOP_OUTPUT_IF_MIPI0;

	if (!(dsi2->mode_flags & MIPI_DSI_MODE_VIDEO)) {
		conn_state->OutputFlags |= ROCKCHIP_OUTPUT_MIPI_DS_MODE;
		conn_state->hold_mode = true;
	}

#if 0
	if (dsi2->lanes > 4) {
		ret = uclass_get_device_by_name(UCLASS_DISPLAY,
						"dsi@fde30000",
						&dev);
		if (ret)
			return ret;

		dsi2->slave = dev_get_priv(dev);
		if (!dsi2->slave)
			return -ENODEV;

		dsi2->slave->master = dsi2;
		dsi2->lanes /= 2;

		dsi2->slave->auto_calc_mode = dsi2->auto_calc_mode;
		dsi2->slave->lanes = dsi2->lanes;
		dsi2->slave->format = dsi2->format;
		dsi2->slave->mode_flags = dsi2->mode_flags;
		dsi2->slave->channel = dsi2->channel;
		conn_state->OutputFlags |=
				ROCKCHIP_OUTPUT_DUAL_CHANNEL_LEFT_RIGHT_MODE;
		if (dsi2->data_swap)
			conn_state->OutputFlags |= ROCKCHIP_OUTPUT_DATA_SWAP;

		conn_state->OutputInterface |= VOP_OUTPUT_IF_MIPI1;

		ret = uclass_get_device_by_phandle(UCLASS_PHY, dev,
						   "phys", &phy_dev);
		if (ret)
			return -ENODEV;

		phy = (struct rockchip_phy *)dev_get_driver_data(phy_dev);
		if (!phy)
			return -ENODEV;

		dsi2->slave->dcphy.phy = phy;
		if (phy->funcs && phy->funcs->init)
			return phy->funcs->init(phy);
	}
#endif

	dw_mipi_dsi2_get_dsc_params_from_sink(dsi2);

	if (dsi2->dsc_enable) {
		cstate->dsc_enable = 1;
		cstate->dsc_sink_cap.version_major = dsi2->version_major;
		cstate->dsc_sink_cap.version_minor = dsi2->version_minor;
		cstate->dsc_sink_cap.slice_width = dsi2->slice_width;
		cstate->dsc_sink_cap.slice_height = dsi2->slice_height;
		/* only can support rgb888 panel now */
		cstate->dsc_sink_cap.target_bits_per_pixel_x16 = 8 << 4;
		cstate->dsc_sink_cap.native_420 = 0;
		memcpy(&cstate->pps, dsi2->pps, sizeof(struct drm_dsc_picture_parameter_set));
	}

	return 0;
}

/*
 * Minimum D-PHY timings based on MIPI D-PHY specification. Derived
 * from the valid ranges specified in Section 6.9, Table 14, Page 41
 * of the D-PHY specification (v2.1).
 */
int mipi_dphy_get_default_config(unsigned long long hs_clk_rate,
				 struct mipi_dphy_configure *cfg)
{
	unsigned long long ui;

	if (!cfg)
		return -EINVAL;

	ui = ALIGN(PSEC_PER_SEC, hs_clk_rate);
	do_div(ui, hs_clk_rate);

	cfg->clk_miss = 0;
	cfg->clk_post = 60000 + 52 * ui;
	cfg->clk_pre = 8000;
	cfg->clk_prepare = 38000;
	cfg->clk_settle = 95000;
	cfg->clk_term_en = 0;
	cfg->clk_trail = 60000;
	cfg->clk_zero = 262000;
	cfg->d_term_en = 0;
	cfg->eot = 0;
	cfg->hs_exit = 100000;
	cfg->hs_prepare = 40000 + 4 * ui;
	cfg->hs_zero = 105000 + 6 * ui;
	cfg->hs_settle = 85000 + 6 * ui;
	cfg->hs_skip = 40000;

	/*
	 * The MIPI D-PHY specification (Section 6.9, v1.2, Table 14, Page 40)
	 * contains this formula as:
	 *
	 *     T_HS-TRAIL = max(n * 8 * ui, 60 + n * 4 * ui)
	 *
	 * where n = 1 for forward-direction HS mode and n = 4 for reverse-
	 * direction HS mode. There's only one setting and this function does
	 * not parameterize on anything other that ui, so this code will
	 * assumes that reverse-direction HS mode is supported and uses n = 4.
	 */
	cfg->hs_trail = max(4 * 8 * ui, 60000 + 4 * 4 * ui);

	cfg->init = 100;
	cfg->lpx = 60000;
	cfg->ta_get = 5 * cfg->lpx;
	cfg->ta_go = 4 * cfg->lpx;
	cfg->ta_sure = 2 * cfg->lpx;
	cfg->wakeup = 1000;

	return 0;
}

static void dw_mipi_dsi2_set_hs_clk(struct dw_mipi_dsi2 *dsi2, unsigned long rate)
{
	mipi_dphy_get_default_config(rate, &dsi2->mipi_dphy_cfg);

	if (!dsi2->c_option)
		rockchip_phy_set_mode(dsi2->dcphy.phy, PHY_MODE_MIPI_DPHY);

	rate = rockchip_phy_set_pll(dsi2->dcphy.phy, rate);
	dsi2->lane_hs_rate = DIV_ROUND_CLOSEST(rate, MSEC_PER_SEC);
}

static void dw_mipi_dsi2_host_softrst(struct dw_mipi_dsi2 *dsi2)
{
	dsi_write(dsi2, DSI2_SOFT_RESET, 0X0);
	udelay(100);
	dsi_write(dsi2, DSI2_SOFT_RESET, SYS_RSTN | PHY_RSTN | IPI_RSTN);
}

static void
dw_mipi_dsi2_work_mode(struct dw_mipi_dsi2 *dsi2, u32 mode)
{
	/*
	 * select controller work in Manual mode
	 * Manual: MANUAL_MODE_EN
	 * Automatic: 0
	 */
	dsi_write(dsi2, MANUAL_MODE_CFG, mode);
}

static void dw_mipi_dsi2_phy_mode_cfg(struct dw_mipi_dsi2 *dsi2)
{
	u32 val = 0;

	/* PPI width is fixed to 16 bits in DCPHY */
	val |= PPI_WIDTH(PPI_WIDTH_16_BITS) | PHY_LANES(dsi2->lanes);
	val |= PHY_TYPE(dsi2->c_option ? CPHY : DPHY);
	dsi_write(dsi2, DSI2_PHY_MODE_CFG, val);
}

static void dw_mipi_dsi2_phy_clk_mode_cfg(struct dw_mipi_dsi2 *dsi2)
{
	u32 sys_clk = SYS_CLK / USEC_PER_SEC;
	u32 esc_clk_div;
	u32 val = 0;

	if (dsi2->mode_flags & MIPI_DSI_CLOCK_NON_CONTINUOUS)
		val |= NON_CONTINUOUS_CLK;

	/* The Escape clock ranges from 1MHz to 20MHz. */
	esc_clk_div = DIV_ROUND_UP(sys_clk, 20 * 2);
	val |= PHY_LPTX_CLK_DIV(esc_clk_div);

	dsi_write(dsi2, DSI2_PHY_CLK_CFG, val);
}

static void dw_mipi_dsi2_phy_ratio_cfg(struct dw_mipi_dsi2 *dsi2)
{
	u64 ipi_clk, phy_hsclk, tmp;

	/*
	 * in DPHY mode, the phy_hstx_clk is exactly 1/16 the Lane high-speed
	 * data rate; In CPHY mode, the phy_hstx_clk is exactly 1/7 the trio
	 * high speed symbol rate.
	 */
	if (dsi2->c_option)
		phy_hsclk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 7);

	else
		phy_hsclk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 16);

	/* IPI_RATIO_MAN_CFG = PHY_HSTX_CLK / IPI_CLK */
	ipi_clk = dsi2->mipi_pixel_rate;

	tmp = DIV_ROUND_CLOSEST(phy_hsclk << 16, ipi_clk);
	dsi_write(dsi2, DSI2_PHY_IPI_RATIO_MAN_CFG, PHY_IPI_RATIO(tmp));

	/* SYS_RATIO_MAN_CFG = MIPI_DCPHY_HSCLK_Freq / SYS_CLK */
	tmp = DIV_ROUND_CLOSEST(phy_hsclk << 16, SYS_CLK);
	dsi_write(dsi2, DSI2_PHY_SYS_RATIO_MAN_CFG, PHY_SYS_RATIO(tmp));
}

static void dw_mipi_dsi2_lp2hs_or_hs2lp_cfg(struct dw_mipi_dsi2 *dsi2)
{
	struct mipi_dphy_configure *cfg = &dsi2->mipi_dphy_cfg;
	unsigned long long tmp, ui;
	unsigned long long hstx_clk;

	hstx_clk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 16);

	ui = ALIGN(PSEC_PER_SEC, hstx_clk);
	do_div(ui, hstx_clk);

	/* PHY_LP2HS_TIME = (TLPX + THS-PREPARE + THS-ZERO) / Tphy_hstx_clk */
	tmp = cfg->lpx + cfg->hs_prepare + cfg->hs_zero;
	tmp = DIV_ROUND_CLOSEST(tmp << 16, ui);
	dsi_write(dsi2, DSI2_PHY_LP2HS_MAN_CFG, PHY_LP2HS_TIME(tmp));

	/* PHY_HS2LP_TIME = (THS-TRAIL + THS-EXIT) / Tphy_hstx_clk */
	tmp = cfg->hs_trail + cfg->hs_exit;
	tmp = DIV_ROUND_CLOSEST(tmp << 16, ui);
	dsi_write(dsi2, DSI2_PHY_HS2LP_MAN_CFG, PHY_HS2LP_TIME(tmp));
}

static void dw_mipi_dsi2_phy_init(struct dw_mipi_dsi2 *dsi2)
{
	dw_mipi_dsi2_phy_mode_cfg(dsi2);
	dw_mipi_dsi2_phy_clk_mode_cfg(dsi2);

	if (dsi2->auto_calc_mode)
		return;

	dw_mipi_dsi2_phy_ratio_cfg(dsi2);
	dw_mipi_dsi2_lp2hs_or_hs2lp_cfg(dsi2);

	/* phy configuration 8 - 10 */
}

static void dw_mipi_dsi2_tx_option_set(struct dw_mipi_dsi2 *dsi2)
{
	u32 val;

	val = BTA_EN | EOTP_TX_EN;

	if (dsi2->mode_flags & MIPI_DSI_MODE_EOT_PACKET)
		val &= ~EOTP_TX_EN;

	dsi_write(dsi2, DSI2_DSI_GENERAL_CFG, val);
	dsi_write(dsi2, DSI2_DSI_VCID_CFG, TX_VCID(dsi2->channel));

	if (dsi2->scrambling_en)
		dsi_write(dsi2, DSI2_DSI_SCRAMBLING_CFG, SCRAMBLING_EN);
}

static void dw_mipi_dsi2_irq_enable(struct dw_mipi_dsi2 *dsi2, bool enable)
{
	if (enable) {
		dsi_write(dsi2, DSI2_INT_MASK_PHY, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_TO, 0xf);
		dsi_write(dsi2, DSI2_INT_MASK_ACK, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_IPI, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_FIFO, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_PRI, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_CRI, 0x1);
	} else {
		dsi_write(dsi2, DSI2_INT_MASK_PHY, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_TO, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_ACK, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_IPI, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_FIFO, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_PRI, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_CRI, 0x0);
	};
}

static void mipi_dcphy_power_on(struct dw_mipi_dsi2 *dsi2)
{
	if (!dsi2->dcphy.phy)
		return;

	rockchip_phy_power_on(dsi2->dcphy.phy);
}

static void dw_mipi_dsi2_pre_enable(struct dw_mipi_dsi2 *dsi2)
{
	if (dsi2->prepared)
		return;

	dw_mipi_dsi2_host_softrst(dsi2);
	dsi_write(dsi2, DSI2_PWR_UP, RESET);

	dw_mipi_dsi2_work_mode(dsi2, dsi2->auto_calc_mode ? 0 : MANUAL_MODE_EN);
	dw_mipi_dsi2_phy_init(dsi2);
	dw_mipi_dsi2_tx_option_set(dsi2);
	dw_mipi_dsi2_irq_enable(dsi2, 0);
	mipi_dcphy_power_on(dsi2);
	dsi_write(dsi2, DSI2_PWR_UP, POWER_UP);
	dw_mipi_dsi2_set_cmd_mode(dsi2);

	dsi2->prepared = true;

	if (dsi2->slave)
		dw_mipi_dsi2_pre_enable(dsi2->slave);
}

static void dw_mipi_dsi2_get_mipi_pixel_clk(struct dw_mipi_dsi2 *dsi2,
					    CRTC_STATE *s)
{
	DRM_DISPLAY_MODE *mode = &dsi2->mode;
	u8 k = dsi2->slave ? 2 : 1;

	/* 1.When MIPI works in uncompressed mode:
	 * (Video Timing Pixel Rate)/(4)=(MIPI Pixel ClockxK)=(dclk_out×K)=dclk_core
	 * 2.When MIPI works in compressed mode:
	 * MIPI Pixel Clock = cds_clk / 2
	 * MIPI is configured as double channel display mode, K=2, otherwise K=1.
	 */
	if (dsi2->dsc_enable) {
		dsi2->mipi_pixel_rate = s->dsc_cds_clk_rate / 2;
		if (dsi2->slave)
			dsi2->slave->mipi_pixel_rate = dsi2->mipi_pixel_rate;

		return;
	}

	dsi2->mipi_pixel_rate = (mode->CrtcClock * MSEC_PER_SEC) / (4 * k);
	if (dsi2->slave)
		dsi2->slave->mipi_pixel_rate = dsi2->mipi_pixel_rate;
}

static int dw_mipi_dsi2_connector_prepare(ROCKCHIP_CONNECTOR_PROTOCOL *conn,
					  DISPLAY_STATE *state)
{
	struct dw_mipi_dsi2 *dsi2 = DW_MIPI_DSI2_FROM_CONNECTOR_PROTOCOL(conn);
	CONNECTOR_STATE *conn_state = &state->ConnectorState;
	CRTC_STATE *cstate = &state->CrtcState;
	unsigned long lane_rate;

	memcpy(&dsi2->mode, &conn_state->DisplayMode, sizeof(DRM_DISPLAY_MODE));
	if (dsi2->slave)
		memcpy(&dsi2->slave->mode, &dsi2->mode,
		       sizeof(DRM_DISPLAY_MODE));

	dw_mipi_dsi2_get_mipi_pixel_clk(dsi2, cstate);

	lane_rate = dw_mipi_dsi2_get_lane_rate(dsi2);
	if (dsi2->dcphy.phy)
		dw_mipi_dsi2_set_hs_clk(dsi2, lane_rate);

	if (dsi2->slave && dsi2->slave->dcphy.phy)
		dw_mipi_dsi2_set_hs_clk(dsi2->slave, lane_rate);

	printf("final DSI-Link bandwidth: %u %a x %d\n",
	       dsi2->lane_hs_rate, dsi2->c_option ? "Ksps" : "Kbps",
	       dsi2->slave ? dsi2->lanes * 2 : dsi2->lanes);

	dw_mipi_dsi2_pre_enable(dsi2);

	return 0;
}

static void dw_mipi_dsi2_connector_unprepare(ROCKCHIP_CONNECTOR_PROTOCOL *conn,
					     DISPLAY_STATE *state)
{
	struct dw_mipi_dsi2 *dsi2 = DW_MIPI_DSI2_FROM_CONNECTOR_PROTOCOL(conn);

	dw_mipi_dsi2_post_disable(dsi2);
}

static int dw_mipi_dsi2_connector_enable(ROCKCHIP_CONNECTOR_PROTOCOL *conn,
					 DISPLAY_STATE *state)
{
	struct dw_mipi_dsi2 *dsi2 = DW_MIPI_DSI2_FROM_CONNECTOR_PROTOCOL(conn);

	dw_mipi_dsi2_enable(dsi2);

	rockchip_panel_init(dsi2);

	return 0;
}

static int dw_mipi_dsi2_connector_disable(ROCKCHIP_CONNECTOR_PROTOCOL *conn,
					  DISPLAY_STATE *state)
{
	struct dw_mipi_dsi2 *dsi2 = DW_MIPI_DSI2_FROM_CONNECTOR_PROTOCOL(conn);

	dw_mipi_dsi2_disable(dsi2);

	return 0;
}

EFI_STATUS
DwMipiDsi2ConnectorPreInit (
	OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
	OUT DISPLAY_STATE                        *DisplayState
	)
{
	int ret;

	ret = dw_mipi_dsi2_connector_pre_init(This, DisplayState);
	if (ret)
		return EFI_DEVICE_ERROR;

	return EFI_SUCCESS;
};

EFI_STATUS
DwMipiDsi2ConnectorInit (
	OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
	OUT DISPLAY_STATE                        *DisplayState
	)
{
	int ret;

	ret = dw_mipi_dsi2_connector_init(This, DisplayState);
	if (ret)
		return EFI_DEVICE_ERROR;

	return EFI_SUCCESS;
}

EFI_STATUS
DwMipiDsi2ConnectorEnable (
	OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
	OUT DISPLAY_STATE                        *DisplayState
	)
{
	int ret;

	/* this should be called by the GOP driver instead */
	dw_mipi_dsi2_connector_prepare (This, DisplayState);

	ret = dw_mipi_dsi2_connector_enable(This, DisplayState);
	if (ret)
		return EFI_DEVICE_ERROR;

	return EFI_SUCCESS;
}

EFI_STATUS
DwMipiDsi2ConnectorDisable (
	OUT ROCKCHIP_CONNECTOR_PROTOCOL          *This,
	OUT DISPLAY_STATE                        *DisplayState
	)
{
	int ret;

	ret = dw_mipi_dsi2_connector_disable(This, DisplayState);
	if (ret)
		return EFI_DEVICE_ERROR;

	return EFI_SUCCESS;
}

ROCKCHIP_CONNECTOR_PROTOCOL mDwMipiDsi2ConnectorOps = {
  NULL,
  DwMipiDsi2ConnectorPreInit,
  DwMipiDsi2ConnectorInit,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  DwMipiDsi2ConnectorEnable,
  DwMipiDsi2ConnectorDisable,
  NULL
};

static const u32 rk3588_dsi0_grf_reg_fields[MAX_FIELDS] = {
	[TXREQCLKHS_EN]		= GRF_REG_FIELD(0x0000, 11, 11),
	[GATING_EN]		= GRF_REG_FIELD(0x0000, 10, 10),
	[IPI_SHUTDN]		= GRF_REG_FIELD(0x0000,  9,  9),
	[IPI_COLORM]		= GRF_REG_FIELD(0x0000,  8,  8),
	[IPI_COLOR_DEPTH]	= GRF_REG_FIELD(0x0000,  4,  7),
	[IPI_FORMAT]		= GRF_REG_FIELD(0x0000,  0,  3),
};

static const u32 rk3588_dsi1_grf_reg_fields[MAX_FIELDS] = {
	[TXREQCLKHS_EN]		= GRF_REG_FIELD(0x0004, 11, 11),
	[GATING_EN]		= GRF_REG_FIELD(0x0004, 10, 10),
	[IPI_SHUTDN]		= GRF_REG_FIELD(0x0004,  9,  9),
	[IPI_COLORM]		= GRF_REG_FIELD(0x0004,  8,  8),
	[IPI_COLOR_DEPTH]	= GRF_REG_FIELD(0x0004,  4,  7),
	[IPI_FORMAT]		= GRF_REG_FIELD(0x0004,  0,  3),
};

static const struct dw_mipi_dsi2_plat_data rk3588_mipi_dsi2_plat_data = {
	.dsi0_grf_reg_fields = rk3588_dsi0_grf_reg_fields,
	.dsi1_grf_reg_fields = rk3588_dsi1_grf_reg_fields,
	.dphy_max_bit_rate_per_lane = 4500000000ULL,
	.cphy_max_symbol_rate_per_lane = 2000000000ULL,
};

static ssize_t dw_mipi_dsi2_host_transfer(struct mipi_dsi_host *host,
					 const struct mipi_dsi_msg *msg)
{
	struct dw_mipi_dsi2 *dsi2 = DW_MIPI_DSI2_FROM_MIPI_DSI_HOST(host);

	return dw_mipi_dsi2_transfer(dsi2, msg);
}

static int dw_mipi_dsi2_host_attach(struct mipi_dsi_host *host,
				   struct mipi_dsi_device *device)
{
	struct dw_mipi_dsi2 *dsi2 = DW_MIPI_DSI2_FROM_MIPI_DSI_HOST(host);

	if (device->lanes < 1 || device->lanes > 8)
		return -EINVAL;

	dsi2->lanes = device->lanes;
	dsi2->channel = device->channel;
	dsi2->format = device->format;
	dsi2->mode_flags = device->mode_flags;
	dsi2->device = device;

	return 0;
}

static const struct mipi_dsi_host_ops dw_mipi_dsi2_host_ops = {
	.attach = dw_mipi_dsi2_host_attach,
	.transfer = dw_mipi_dsi2_host_transfer,
};

static int dw_mipi_dsi2_child_post_bind(struct dw_mipi_dsi2 *dsi2)
{
	struct mipi_dsi_host *host = &dsi2->host;
	struct mipi_dsi_device *device = NULL;
	ROCKCHIP_DSI_PANEL_PROTOCOL *Panel = dsi2->RockchipDsiPanel;

	device = AllocatePool(sizeof (struct mipi_dsi_device));
	if (device == NULL) {
		return -ENOMEM;
	}

	host->ops = &dw_mipi_dsi2_host_ops;

	device->host = host;

	device->lanes = Panel->DsiLanes;
	device->format = Panel->DsiFormat;
	device->mode_flags = Panel->DsiFlags;
	device->channel = 0;

	dw_mipi_dsi2_host_attach(host, device);

	return 0;
}

struct dw_mipi_dsi2 mRk3588MipiDsi[] = {
	{
		.base = 0xfde20000,
		.grf = RK3588_VOP_GRF_BASE,
		.id = 0,
		.pdata = &rk3588_mipi_dsi2_plat_data,
	},
	{
		.base = 0xfde30000,
		.grf = RK3588_VOP_GRF_BASE,
		.id = 1,
		.pdata = &rk3588_mipi_dsi2_plat_data,
	},
};

STATIC VOID  *mDsiPanelEventRegistration;

STATIC
VOID
DsiPanelRegistrationEventHandler (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
	EFI_HANDLE                		Handle;
	UINTN                     		BufferSize;
	EFI_STATUS                		Status;
	ROCKCHIP_DSI_PANEL_PROTOCOL		*DsiPanel;
	struct dw_mipi_dsi2 	  		*dsi2;
	UINTN					Index;

	while (TRUE) {
		BufferSize = sizeof (EFI_HANDLE);
		Status = gBS->LocateHandle (ByRegisterNotify,
					NULL,
					mDsiPanelEventRegistration,
					&BufferSize,
					&Handle);
		if (EFI_ERROR (Status)) {
			if (Status != EFI_NOT_FOUND) {
				DEBUG ((DEBUG_WARN, "%a: Failed to locate gRockchipDsiPanelProtocolGuid. Status=%r\n",
				__FUNCTION__, Status));
			}
			break;
		}

		Status = gBS->HandleProtocol (Handle, &gRockchipDsiPanelProtocolGuid, (VOID **)&DsiPanel);
		ASSERT_EFI_ERROR (Status);

		for (Index = 0; Index < ARRAY_SIZE (mRk3588MipiDsi); Index++) {
			dsi2 = &mRk3588MipiDsi[Index];
			if (dsi2->id == DsiPanel->DsiId) {
				dsi2->Signature = DW_MIPI_DSI2_SIGNATURE;
				dsi2->dcphy.phy = rockchip_phy_by_id(dsi2->id);
				dsi2->RockchipDsiPanel = DsiPanel;
				CopyMem (&dsi2->connector, &mDwMipiDsi2ConnectorOps, sizeof (ROCKCHIP_CONNECTOR_PROTOCOL));

				dw_mipi_dsi2_child_post_bind(dsi2);

				Status = gBS->InstallMultipleProtocolInterfaces (
						&Handle,
						&gRockchipConnectorProtocolGuid,
						&dsi2->connector,
						NULL
						);
				break;
			}
		}

		ASSERT_EFI_ERROR (Status);
		break;
	}
}

EFI_STATUS
EFIAPI
DwMipiDsi2Init (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
	EfiCreateProtocolNotifyEvent (&gRockchipDsiPanelProtocolGuid,
					TPL_CALLBACK,
					DsiPanelRegistrationEventHandler,
					NULL,
					&mDsiPanelEventRegistration);
	return EFI_SUCCESS;
}
