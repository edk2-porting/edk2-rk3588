/** @file
  Header file for the PCI Express library.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_PCI_EXPRESS_LIB_H_
#define _SA_PCI_EXPRESS_LIB_H_


/**
  Gets the base address of PCI Express.

  This internal functions retrieves PCI Express Base Address.

  @return The base address of PCI Express.
**/
VOID*
GetPciExpressBaseAddress (
  VOID
);

#endif
