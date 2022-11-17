/** @file
  Header file for CnviLib.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CNVI_LIB_H_
#define _CNVI_LIB_H_

/**
  Check if CNVi is present.

  @retval TRUE                    CNVi is enabled
  @retval FALSE                   CNVi is disabled

**/
BOOLEAN
CnviIsPresent (
  VOID
  );

#endif // _CNVI_LIB_H_
