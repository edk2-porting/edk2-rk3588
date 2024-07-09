/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ROCKCHIP_PHY_H_
#define _ROCKCHIP_PHY_H_

enum phy_mode {
	PHY_MODE_INVALID,
	PHY_MODE_MIPI_DPHY,
	PHY_MODE_VIDEO_LVDS,
	PHY_MODE_VIDEO_TTL,
};

struct rockchip_phy;

struct rockchip_phy_funcs {
	int (*init)(struct rockchip_phy *phy);
	int (*power_on)(struct rockchip_phy *phy);
	int (*power_off)(struct rockchip_phy *phy);
	unsigned long (*set_pll)(struct rockchip_phy *phy, unsigned long rate);
	int (*set_bus_width)(struct rockchip_phy *phy, u32 bus_width);
	long (*round_rate)(struct rockchip_phy *phy, unsigned long rate);
	int (*set_mode)(struct rockchip_phy *phy, enum phy_mode mode);
};

struct rockchip_phy {
	const struct rockchip_phy_funcs *funcs;
};

int rockchip_phy_init(struct rockchip_phy *phy);
int rockchip_phy_power_off(struct rockchip_phy *phy);
int rockchip_phy_power_on(struct rockchip_phy *phy);
unsigned long rockchip_phy_set_pll(struct rockchip_phy *phy,
				   unsigned long rate);
int rockchip_phy_set_bus_width(struct rockchip_phy *phy, u32 bus_width);
long rockchip_phy_round_rate(struct rockchip_phy *phy, unsigned long rate);
int rockchip_phy_set_mode(struct rockchip_phy *phy, enum phy_mode mode);

struct rockchip_phy *rockchip_phy_by_id(unsigned int id);

#endif
