/** @file
  Header file for SMM Control PEI Library.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_SMM_CONTROL_LIB_H_
#define _PCH_SMM_CONTROL_LIB_H_

/**
  This function install PEI SMM Control PPI

  @retval EFI_STATUS  Results of the installation of the SMM Control PPI
**/
EFI_STATUS
EFIAPI
PchSmmControlInit (
  VOID
  );

#endif
