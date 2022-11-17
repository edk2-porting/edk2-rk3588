/** @file
  Header file for PchInfoLib.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_INFO_LIB_H_
#define _PCH_INFO_LIB_H_

#include <PchAccess.h>

typedef enum {
  PchH          = 1,
  PchLp,
  PchMini,
  PchUnknownSeries
} PCH_SERIES;

#endif // _PCH_INFO_LIB_H_
