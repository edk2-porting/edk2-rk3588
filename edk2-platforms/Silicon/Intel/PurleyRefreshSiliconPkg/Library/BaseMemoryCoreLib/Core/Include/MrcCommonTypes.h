/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MrcCommonTypes_h_
#define _MrcCommonTypes_h_

#include "DataTypes.h"

#define INT32_MIN                       (0x80000000)
#ifndef INT32_MAX  //INT32_MAX->Already defined
#define INT32_MAX                       (0x7FFFFFFF)
#endif
#define INT16_MIN                       (0x8000)
#define INT16_MAX                       (0x7FFF)

#endif // _MrcCommonTypes_h_
