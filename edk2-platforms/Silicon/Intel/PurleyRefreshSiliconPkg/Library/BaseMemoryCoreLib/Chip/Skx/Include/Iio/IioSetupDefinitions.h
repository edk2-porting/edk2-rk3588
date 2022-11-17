/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef IIOSETUPDEFINITIONS_H_
#define IIOSETUPDEFINITIONS_H_

/**
==================================================================================================
================= Equates common for Setup options (.vfr/.hfr) and source files (.c/.h) ==========
==================================================================================================
**/

#define PCIE_ASPM_AUTO                     7
#define PCIE_ASPM_DISABLE                  0
#define PCIE_ASPM_L0S_ONLY                 1
#define PCIE_ASPM_L1_ONLY                  2
#define PCIE_ASPM_L0S_L1_BOTH              3

#define PCIE_LINK_SPEED_AUTO               0
#define PCIE_LINK_SPEED_GEN1               1
#define PCIE_LINK_SPEED_GEN2               2
#define PCIE_LINK_SPEED_GEN3               3

#define PCIE_L0S_4US_8US                   3
#define PCIE_L1_8US_16US                   4

#define IIO_OPTION_AUTO    2
#define IIO_OPTION_ENABLE  1
#define IIO_OPTION_DISABLE 0

#define GEN3MANUAL_PH2_PRECURSOR_MIN        0
#define GEN3MANUAL_PH2_CURSOR_MIN           0
#define GEN3MANUAL_PH2_POSTCURSOR_MIN       0

#define GEN3MANUAL_PH2_PRECURSOR_MAX        63
#define GEN3MANUAL_PH2_CURSOR_MAX           63
#define GEN3MANUAL_PH2_POSTCURSOR_MAX       63

#define GEN3MANUAL_PH2_PRECURSOR_DEFAULT    11
#define GEN3MANUAL_PH2_CURSOR_DEFAULT       41
#define GEN3MANUAL_PH2_POSTCURSOR_DEFAULT   11

#define GEN3MANUAL_PH3_PRECURSOR_MIN        0
#define GEN3MANUAL_PH3_CURSOR_MIN           0
#define GEN3MANUAL_PH3_POSTCURSOR_MIN       0

#define GEN3MANUAL_PH3_PRECURSOR_MAX        63
#define GEN3MANUAL_PH3_CURSOR_MAX           63
#define GEN3MANUAL_PH3_POSTCURSOR_MAX       63

#define GEN3MANUAL_PH3_PRECURSOR_DEFAULT    11
#define GEN3MANUAL_PH3_CURSOR_DEFAULT       41
#define GEN3MANUAL_PH3_POSTCURSOR_DEFAULT   11

#define RTO_GEN3_OVERRIDE_MODE_UNIPHY         0
#define RTO_GEN3_OVERRIDE_MODE_MANUAL         1
#define RTO_GEN3_OVERRIDE_MODE_TEST_CARD      2
#define RTO_GEN3_OVERRIDE_MODE_ALTERNATE_TXEQ 3

#define RTO_GEN3_TEST_CARD_LAGUNA           0
#define RTO_GEN3_TEST_CARD_NTB              1

#define RTO_GEN3_EQ_MODE_TESTCARD          1
#define RTO_GEN3_EQ_MODE_NTB_TESTCARD      2


#define COMPLETION_TIMEOUT_260MS_900MS     9

#define SNOOP_RESP_DEF_VALUE               6

#define MC_INDEX_POS_12                    0xC

#define MC_NUM_GROUP_8                     8

#define CONFIG_IOU_AUTO                    0xFF

#define NTB_BARSIZE_PBAR23_DEFAULT         0xC
#define NTB_BARSIZE_PBAR45_DEFAULT         0xC
#define NTB_BARSIZE_PBAR4_DEFAULT          0xC
#define NTB_BARSIZE_PBAR5_DEFAULT          0xC
#define NTB_BARSIZE_SBAR23_DEFAULT         0xC
#define NTB_BARSIZE_SBAR45_DEFAULT         0xC
#define NTB_BARSIZE_SBAR4_DEFAULT          0xC
#define NTB_BARSIZE_SBAR5_DEFAULT          0xC
#define NTB_IIO_XLINK_CTL_DSD_USP          2

#define VMD_CFG_BAR_SIZE_DEFAULT           25
#define VMD_MEM_BAR_SIZE1_DEFAULT          25
#define VMD_MEM_BAR_SIZE2_DEFAULT          20

#define VMD_32BIT_NONPREFETCH              0
#define VMD_64BIT_NONPREFETCH              1
#define VMD_64BIT_PREFETCH                 2

#define IODC_DISABLE                       0
#define IODC_AUTO                          1
#define IODC_EN_REM_INVITOM_PUSH           2
#define IODC_EN_REM_INVITOM_ALLOCFLOW      3
#define IODC_EN_REM_INVITOM_ALLOC_NONALLOC 4
#define IODC_EN_REM_INVITOM_AND_WCILF      5
#define IODC_GLOBAL_KTI_OPTION             6

#define PCIE_PORT_DISABLE                   0
#define PCIE_PORT_ENABLE                    1
#define PCIE_PORT_AUTO                      2

#endif /* IIOSETUPDEFINITIONS_H_ */
