/** @file
    WhiskeylakeURvp HSIO PTSS H File


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef WHISKEYLAKE_RVP3_HSIO_PTSS_H_
#define WHISKEYLAKE_RVP3_HSIO_PTSS_H_

#include <PchHsioPtssTables.h>

#ifndef HSIO_PTSS_TABLE_SIZE
#define HSIO_PTSS_TABLE_SIZE(A) A##_Size = sizeof (A) / sizeof (HSIO_PTSS_TABLES)
#endif

//BoardId WhiskeylakeURvp
HSIO_PTSS_TABLES PchLpHsioPtss_Cx_WhiskeylakeURvp[] = {
  {{14, 0, 0xa0, 0x00000000, (UINT32) ~0x3F3F00}, 0}
};

UINT16 PchLpHsioPtss_Cx_WhiskeylakeURvp_Size = sizeof(PchLpHsioPtss_Cx_WhiskeylakeURvp) / sizeof(HSIO_PTSS_TABLES);

HSIO_PTSS_TABLES PchLpHsioPtss_Bx_WhiskeylakeURvp[] = {
  {{14, 0, 0xa0, 0x00000000, (UINT32) ~0x3F3F00}, 0},
};

UINT16 PchLpHsioPtss_Bx_WhiskeylakeURvp_Size = sizeof(PchLpHsioPtss_Bx_WhiskeylakeURvp) / sizeof(HSIO_PTSS_TABLES);

#endif // WHISKEYLAKE_RVP3_HSIO_PTSS_H_

