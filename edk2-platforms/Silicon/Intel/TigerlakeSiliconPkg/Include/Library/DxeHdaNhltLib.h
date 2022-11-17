/** @file
  Prototype of the DxePchHdaNhltLib library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_HDA_NHLT_LIB_H_
#define _DXE_HDA_NHLT_LIB_H_

#include <DxeHdaNhlt.h>

/**
  Returns pointer to Endpoint ENDPOINT_DESCRIPTOR structure.

  @param[in] *NhltTable    Endpoint for which Format address is retrieved
  @param[in] FormatIndex   Index of Format to be retrieved

  @retval                  Pointer to ENDPOINT_DESCRIPTOR structure with given index
**/
ENDPOINT_DESCRIPTOR *
GetNhltEndpoint (
  IN CONST NHLT_ACPI_TABLE      *NhltTable,
  IN CONST UINT8                EndpointIndex
  );

/**
  Returns pointer to Endpoint Specific Configuration SPECIFIC_CONFIG structure.

  @param[in] *Endpoint     Endpoint for which config address is retrieved

  @retval                  Pointer to SPECIFIC_CONFIG structure with endpoint's capabilities
**/
SPECIFIC_CONFIG *
GetNhltEndpointDeviceCapabilities (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  );

/**
  Returns pointer to all Formats Configuration FORMATS_CONFIG structure.

  @param[in] *Endpoint     Endpoint for which Formats address is retrieved

  @retval                  Pointer to FORMATS_CONFIG structure
**/
FORMATS_CONFIG *
GetNhltEndpointFormatsConfig (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  );

/**
  Returns pointer to Format Configuration FORMAT_CONFIG structure.

  @param[in] *Endpoint     Endpoint for which Format address is retrieved
  @param[in] FormatIndex   Index of Format to be retrieved

  @retval                  Pointer to FORMAT_CONFIG structure with given index
**/
FORMAT_CONFIG *
GetNhltEndpointFormat (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint,
  IN CONST UINT8                FormatIndex
  );

/**
  Returns pointer to all Device Information DEVICES_INFO structure.

  @param[in] *Endpoint     Endpoint for which DevicesInfo address is retrieved

  @retval                  Pointer to DEVICES_INFO structure
**/
DEVICES_INFO *
GetNhltEndpointDevicesInfo (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  );

/**
  Returns pointer to Device Information DEVICES_INFO structure.

  @param[in] *Endpoint       Endpoint for which Device Info address is retrieved
  @param[in] DeviceInfoIndex Index of Device Info to be retrieved

  @retval                    Pointer to DEVICE_INFO structure with given index
**/
DEVICE_INFO *
GetNhltEndpointDeviceInfo (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint,
  IN CONST UINT8                DeviceInfoIndex
  );


/**
  Returns pointer to OED Configuration SPECIFIC_CONFIG structure.

  @param[in] *NhltTable    NHLT table for which OED address is retrieved

  @retval                  Pointer to SPECIFIC_CONFIG structure with NHLT capabilities
**/
SPECIFIC_CONFIG *
GetNhltOedConfig (
  IN CONST NHLT_ACPI_TABLE      *NhltTable
  );

/**
  Prints Format configuration.

  @param[in] *Format       Format to be printed

  @retval None
**/
VOID
NhltFormatDump (
  IN CONST FORMAT_CONFIG        *Format
  );


/**
  Prints Endpoint configuration.

  @param[in] *Endpoint     Endpoint to be printed

  @retval None
**/
VOID
NhltEndpointDump (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  );

/**
  Prints OED (Offload Engine Driver) configuration.

  @param[in] *OedConfig   OED to be printed

  @retval None
**/
VOID
NhltOedConfigDump (
  IN CONST SPECIFIC_CONFIG      *OedConfig
  );

/**
  Prints NHLT (Non HDA-Link Table) to be exposed via ACPI (aka. OED (Offload Engine Driver) Configuration Table).

  @param[in] *NhltTable    The NHLT table to print

  @retval None
**/
VOID
NhltAcpiTableDump (
  IN NHLT_ACPI_TABLE            *NhltTable
  );

#endif // _DXE_HDA_NHLT_LIB_H_
