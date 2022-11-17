/** @file
  Header file for SMM Access Driver.

  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _SMM_ACCESS_LIB_H_
#define _SMM_ACCESS_LIB_H_

/**
  This function is to install an SMM Access PPI
  - <b>Introduction</b> \n
    An API to install an instance of EFI_PEI_MM_ACCESS_PPI. This PPI is commonly used to control SMM mode memory access for S3 resume.

    @retval EFI_SUCCESS           - Ppi successfully started and installed.
    @retval EFI_NOT_FOUND         - Ppi can't be found.
    @retval EFI_OUT_OF_RESOURCES  - Ppi does not have enough resources to initialize the driver.
**/
EFI_STATUS
EFIAPI
PeiInstallSmmAccessPpi (
  VOID
  );
#endif
