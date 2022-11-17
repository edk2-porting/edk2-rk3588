/** @file
  This library provides PCH HD Audio functions.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_HDA_LIB_H_
#define _PCH_HDA_LIB_H_

#include <Library/DxePchHdaNhlt.h>
#include <PchHdaEndpoints.h>

/**
  Prints NHLT (Non HDA-Link Table) to be exposed via ACPI (aka. OED (Offload Engine Driver) Configuration Table).

  @param[in] *NhltAcpiTable    The NHLT table to print
**/
VOID
NhltAcpiTableDump(
  IN NHLT_ACPI_TABLE           *NhltTable
  );

/**
  Constructs EFI_ACPI_DESCRIPTION_HEADER structure for NHLT table.

  @param[in][out] *NhltTable            NHLT table for which header will be created
  @param[in]      NhltTableSize         Size of NHLT table

  @retval None
**/
VOID
NhltAcpiHeaderConstructor (
  IN OUT NHLT_ACPI_TABLE        *NhltTable,
  IN UINT32                     NhltTableSize
  );

/**
  Constructs NHLT_ACPI_TABLE structure based on given Endpoints list.

  @param[in]      *EndpointTable List of endpoints for NHLT
  @param[in][out] **NhltTable    NHLT table to be created
  @param[in][out] *NhltTableSize Size of created NHLT table

  @retval EFI_SUCCESS            NHLT created successfully
  @retval EFI_BAD_BUFFER_SIZE    Not enough resources to allocate NHLT
**/
EFI_STATUS
NhltConstructor(
  IN PCH_HDA_NHLT_ENDPOINTS    *EndpointTable,
  IN OUT NHLT_ACPI_TABLE       **NhltTable,
  IN OUT UINT32                *NhltTableSize
  );

#endif
