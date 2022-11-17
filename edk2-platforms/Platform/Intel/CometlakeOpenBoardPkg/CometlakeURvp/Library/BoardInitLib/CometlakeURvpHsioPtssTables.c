/** @file
 CometlakeURvp HSIO PTSS H File


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef COMETLAKE_RVP3_HSIO_PTSS_H_
#define COMETLAKE_RVP3_HSIO_PTSS_H_

#include <PchHsioPtssTables.h>

#ifndef HSIO_PTSS_TABLE_SIZE
#define HSIO_PTSS_TABLE_SIZE(A) A##_Size = sizeof (A) / sizeof (HSIO_PTSS_TABLES)
#endif

//BoardId CometlakeURvp
HSIO_PTSS_TABLES PchLpHsioPtss_Cx_CometlakeURvp[] = {
  {{14, 0, 0xa0, 0x00000000, (UINT32) ~0x3F3F00}, 0}
};

UINT16 PchLpHsioPtss_Cx_CometlakeURvp_Size = sizeof(PchLpHsioPtss_Cx_CometlakeURvp) / sizeof(HSIO_PTSS_TABLES);

HSIO_PTSS_TABLES PchLpHsioPtss_Bx_CometlakeURvp[] = {
  {{14, 0, 0xa0, 0x00000000, (UINT32) ~0x3F3F00}, 0},
};

UINT16 PchLpHsioPtss_Bx_CometlakeURvp_Size = sizeof(PchLpHsioPtss_Bx_CometlakeURvp) / sizeof(HSIO_PTSS_TABLES);

#endif // COMETLAKE_RVP_HSIO_PTSS_H_

