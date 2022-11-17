/** @file
    Aspire VN7-572G HSIO PTSS H File

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef ASPIRE_VN7_572G_HSIO_PTSS_H_
#define ASPIRE_VN7_572G_HSIO_PTSS_H_

#include <PchHsioPtssTables.h>

#ifndef HSIO_PTSS_TABLE_SIZE
#define HSIO_PTSS_TABLE_SIZE(A) A##_Size = sizeof (A) / sizeof (HSIO_PTSS_TABLES)
#endif

//BoardId AspireVn7Dash572G
HSIO_PTSS_TABLES PchLpHsioPtss_AspireVn7Dash572G[] = {
  /* PchSataHsioRxGen3EqBoostMag[1] = "1" */
  {{14, V_PCH_PCR_FIA_LANE_OWN_SATA, 0x150, 0x01000000, (UINT32) ~0x3F000000}, PchSataTopoUnknown}
};

UINT16 PchLpHsioPtss_AspireVn7Dash572G_Size = sizeof(PchLpHsioPtss_AspireVn7Dash572G) / sizeof(HSIO_PTSS_TABLES);

#endif // ASPIRE_VN7_572G_HSIO_PTSS_H_
