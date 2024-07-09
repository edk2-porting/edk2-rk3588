/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <Library/uboot-env.h>
#include "rockchip_phy.h"

int rockchip_phy_init(struct rockchip_phy *phy)
{
	if (!phy)
		return -ENODEV;

	if (phy->funcs && phy->funcs->init)
		return phy->funcs->init(phy);

	return 0;
}

int rockchip_phy_power_on(struct rockchip_phy *phy)
{
	if (!phy)
		return -ENODEV;

	if (phy->funcs && phy->funcs->power_on)
		return phy->funcs->power_on(phy);

	return 0;
}

int rockchip_phy_power_off(struct rockchip_phy *phy)
{
	if (!phy)
		return -ENODEV;

	if (phy->funcs && phy->funcs->power_off)
		return phy->funcs->power_off(phy);

	return 0;
}

unsigned long rockchip_phy_set_pll(struct rockchip_phy *phy,
				   unsigned long rate)
{
	if (!phy)
		return -ENODEV;

	if (phy->funcs && phy->funcs->set_pll)
		return phy->funcs->set_pll(phy, rate);

	return 0;
}

int rockchip_phy_set_bus_width(struct rockchip_phy *phy, u32 bus_width)
{
	if (!phy)
		return -ENODEV;

	if (phy->funcs && phy->funcs->set_bus_width)
		return phy->funcs->set_bus_width(phy, bus_width);

	return 0;
}

long rockchip_phy_round_rate(struct rockchip_phy *phy, unsigned long rate)
{
	if (!phy)
		return -ENODEV;

	if (phy->funcs && phy->funcs->round_rate)
		return phy->funcs->round_rate(phy, rate);

	return 0;
}

int rockchip_phy_set_mode(struct rockchip_phy *phy, enum phy_mode mode)
{
	if (!phy)
		return -ENODEV;

	if (phy->funcs && phy->funcs->set_mode)
		return phy->funcs->set_mode(phy, mode);

	return 0;
}
