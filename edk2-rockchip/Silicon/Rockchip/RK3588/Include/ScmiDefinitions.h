/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2024, Rockchip, Inc. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RK3588_SCMI_DEFINITIONS_H__
#define __RK3588_SCMI_DEFINITIONS_H__

#define SCMI_CLK_CPUL        0
#define SCMI_CLK_DSU         1
#define SCMI_CLK_CPUB01      2
#define SCMI_CLK_CPUB23      3
#define SCMI_CLK_DDR         4
#define SCMI_CLK_GPU         5
#define SCMI_CLK_NPU         6
#define SCMI_CLK_SBUS        7
#define SCMI_PCLK_SBUS       8
#define SCMI_CCLK_SD         9
#define SCMI_DCLK_SD         10
#define SCMI_ACLK_SECURE_NS  11
#define SCMI_HCLK_SECURE_NS  12
#define SCMI_TCLK_WDT        13
#define SCMI_KEYLADDER_CORE  14
#define SCMI_KEYLADDER_RNG   15
#define SCMI_ACLK_SECURE_S   16
#define SCMI_HCLK_SECURE_S   17
#define SCMI_PCLK_SECURE_S   18
#define SCMI_CRYPTO_RNG      19
#define SCMI_CRYPTO_CORE     20
#define SCMI_CRYPTO_PKA      21
#define SCMI_SPLL            22
#define SCMI_HCLK_SD         23
#define SCMI_CRYPTO_RNG_S    24
#define SCMI_CRYPTO_CORE_S   25
#define SCMI_CRYPTO_PKA_S    26
#define SCMI_A_CRYPTO_S      27
#define SCMI_H_CRYPTO_S      28
#define SCMI_P_CRYPTO_S      29
#define SCMI_A_KEYLADDER_S   30
#define SCMI_H_KEYLADDER_S   31
#define SCMI_P_KEYLADDER_S   32
#define SCMI_TRNG_S          33
#define SCMI_H_TRNG_S        34
#define SCMI_P_OTPC_S        35
#define SCMI_OTPC_S          36
#define SCMI_OTP_PHY         37
#define SCMI_OTPC_AUTO_RD    38
#define SCMI_OTPC_ARB        39
#define SCMI_CCLK_EMMC       40

#endif // __RK3588_SCMI_DEFINITIONS_H__
