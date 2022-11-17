/** @file
  This header file provides common definitions just for System Agent using to avoid including extra module's file.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_COMMON_DEFINITIONS_H_
#define _SA_COMMON_DEFINITIONS_H_

#define ERROR_BY_16     (0xEE15)
#define ERROR_NOT_BY_16 (0xED15)

#define MAX_PCIE_ASPM_OVERRIDE       500
#define MAX_PCIE_LTR_OVERRIDE        500

#define DISABLED  0
#define ENABLED   1

#define SA_VTD_ENGINE_NUMBER        3

#endif
