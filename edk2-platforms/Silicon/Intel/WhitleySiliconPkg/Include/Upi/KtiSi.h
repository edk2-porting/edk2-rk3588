/** @file

  @copyright
  Copyright 2004 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _KTI_SI_H_
#define _KTI_SI_H_

#include "DataTypes.h"

#if (MAX_SOCKET == 1)
  #define MAX_FW_KTI_PORTS     3    // Maximum KTI PORTS to be used in structure definition.
#else
  #define MAX_FW_KTI_PORTS     6    // Maximum KTI PORTS to be used in structure definition
#endif //(MAX_SOCKET == 1)

#define IIO_PSTACK0         1
#define IIO_PSTACK1         2
#define IIO_PSTACK2         3
#define IIO_PSTACK3         4
#define IIO_PSTACK4         5

#define UBOX_STACK          MAX_LOGIC_IIO_STACK - 1 //use stack 13 for ubox

#define IIO_RESERVED_1                6

#define MAX_CHA_MAP                 4

#endif // _KTI_SI_H_
