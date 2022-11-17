/** @file
  Silicon Policy PPI is used for specifying platform
  related Intel silicon information and policy setting.
  This PPI is consumed by the silicon PEI modules and carried
  over to silicon DXE modules.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UPI_POLICY_PPI_H_
#define _UPI_POLICY_PPI_H_

#include <Upi/KtiHost.h>

///
/// PPI revision information
/// This PPI will be extended in a backwards compatible manner over time
/// Added interfaces should be documented here with the revisions added
/// Revision 1:  Initial revision
#define UPI_POLICY_PPI_REVISION  0x1

typedef struct _UPI_POLICY_PPI UPI_POLICY_PPI;

/**
  UPI Policy Structure
**/
struct _UPI_POLICY_PPI {
  /**
    This member specifies the revision of the UPI_POLICY_PPI. This field is used to
    indicate backwards compatible changes to the INTERFACE. Platform code that produces
    this INTERFACE must fill with the correct revision value for UPI code
    to correctly interpret the content of the INTERFACE fields.
  **/
  UINT32        Revision;
  KTI_HOST_IN   Upi;        ///< KTIRC input structure
};

#endif  // _UPI_POLICY_PPI_H_

