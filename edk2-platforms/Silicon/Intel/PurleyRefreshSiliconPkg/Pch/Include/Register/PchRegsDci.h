/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_REGS_DCI_H_
#define _PCH_REGS_DCI_H_

//
// DCI PCR Registers
//
#define R_PCH_PCR_DCI_ECTRL                       0x04            ///< DCI Control Register
#define B_PCH_PCR_DCI_ECTRL_HDCILOCK              BIT0            ///< Host DCI lock
#define B_PCH_PCR_DCI_ECTRL_HDCIEN                BIT4            ///< Host DCI enable
#define R_PCH_PCR_DCI_ECKPWRCTL                   0x08            ///< DCI Power Control
#define R_PCH_PCR_DCI_PCE                         0x30            ///< DCI Power Control Enable Register
#define B_PCH_PCR_DCI_PCE_HAE                     BIT5            ///< Hardware Autonomous Enable
#define B_PCH_PCR_DCI_PCE_D3HE                    BIT2            ///< D3-Hot Enable
#define B_PCH_PCR_DCI_PCE_I3E                     BIT1            ///< I3 Enable
#define B_PCH_PCR_DCI_PCE_PMCRE                   BIT0            ///< PMC Request Enable

#endif
