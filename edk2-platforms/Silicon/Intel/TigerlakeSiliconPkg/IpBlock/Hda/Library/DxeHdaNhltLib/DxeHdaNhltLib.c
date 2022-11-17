/** @file
  HD Audio NHLT Library implementation.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
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
  )
{
  UINT8               Index;
  ENDPOINT_DESCRIPTOR *Endpoint;
  Endpoint = (ENDPOINT_DESCRIPTOR*) (NhltTable->EndpointDescriptors);

  if (EndpointIndex > NhltTable->EndpointCount) {
    return NULL;
  }

  for (Index = 0; Index < EndpointIndex; Index++) {
    Endpoint = (ENDPOINT_DESCRIPTOR*) ((UINT8*) (Endpoint) + Endpoint->EndpointDescriptorLength);
  }

  return Endpoint;
}

/**
  Returns pointer to Endpoint Specific Configuration SPECIFIC_CONFIG structure.

  @param[in] *Endpoint     Endpoint for which config address is retrieved

  @retval                  Pointer to SPECIFIC_CONFIG structure with endpoint's capabilities
**/
SPECIFIC_CONFIG *
GetNhltEndpointDeviceCapabilities (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  )
{
  return (SPECIFIC_CONFIG*) (&Endpoint->EndpointConfig);
}

/**
  Returns pointer to all Formats Configuration FORMATS_CONFIG structure.

  @param[in] *Endpoint     Endpoint for which Formats address is retrieved

  @retval                  Pointer to FORMATS_CONFIG structure
**/
FORMATS_CONFIG *
GetNhltEndpointFormatsConfig (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  )
{
  FORMATS_CONFIG *FormatsConfig;
  FormatsConfig = (FORMATS_CONFIG*) ((UINT8*) (&Endpoint->EndpointConfig)
                                     + sizeof (Endpoint->EndpointConfig.CapabilitiesSize)
                                     + Endpoint->EndpointConfig.CapabilitiesSize);

  return FormatsConfig;
}

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
  )
{
  UINT8          Index;
  UINT32         Length;
  FORMATS_CONFIG *FormatsConfig;
  FORMAT_CONFIG  *Format;

  Length = 0;
  FormatsConfig = GetNhltEndpointFormatsConfig (Endpoint);
  Format = FormatsConfig->FormatsConfiguration;

  if (FormatIndex > FormatsConfig->FormatsCount) {
    return NULL;
  }

  for (Index = 0; Index < FormatIndex; Index++) {
    Length = sizeof (Format->Format) + Format->FormatConfiguration.CapabilitiesSize
      + sizeof (Format->FormatConfiguration.CapabilitiesSize);
    Format = (FORMAT_CONFIG*) ((UINT8*) (Format) + Length);
  }

  return Format;
}

/**
  Returns pointer to all Device Information DEVICES_INFO structure.

  @param[in] *Endpoint     Endpoint for which DevicesInfo address is retrieved

  @retval                  Pointer to DEVICES_INFO structure
**/
DEVICES_INFO *
GetNhltEndpointDevicesInfo (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  )
{
  DEVICES_INFO   *DevicesInfo;
  FORMATS_CONFIG *FormatsConfig;
  FORMAT_CONFIG  *Format;

  FormatsConfig = GetNhltEndpointFormatsConfig (Endpoint);
  Format = GetNhltEndpointFormat (Endpoint, FormatsConfig->FormatsCount);
  DevicesInfo = (DEVICES_INFO*) ((UINT8*) Format);

  return DevicesInfo;
}

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
  )
{
  DEVICES_INFO  *DevicesInfo;
  DEVICE_INFO   *DeviceInfo;

  DevicesInfo = GetNhltEndpointDevicesInfo (Endpoint);
  DeviceInfo = DevicesInfo->DeviceInformation;

  if (DevicesInfo == NULL || DeviceInfoIndex >= DevicesInfo->DeviceInfoCount) {
    return NULL;
  }

  DeviceInfo = (DEVICE_INFO*) ((UINT8*) (DeviceInfo) + sizeof (*DeviceInfo) * DeviceInfoIndex);

  return DeviceInfo;
}

/**
  Returns pointer to OED Configuration SPECIFIC_CONFIG structure.

  @param[in] *NhltTable    NHLT table for which OED address is retrieved

  @retval                  Pointer to SPECIFIC_CONFIG structure with NHLT capabilities
**/
SPECIFIC_CONFIG *
GetNhltOedConfig (
  IN CONST NHLT_ACPI_TABLE      *NhltTable
  )
{
  ENDPOINT_DESCRIPTOR *Endpoint;
  SPECIFIC_CONFIG     *OedConfig;

  Endpoint = GetNhltEndpoint (NhltTable, (NhltTable->EndpointCount));
  OedConfig = (SPECIFIC_CONFIG*) ((UINT8*) (Endpoint));

  return OedConfig;
}

/**
  Prints Format configuration.

  @param[in] *Format       Format to be printed

  @retval None
**/
VOID
NhltFormatDump (
  IN CONST FORMAT_CONFIG        *Format
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------------------- FORMAT -------------------------------\n"));
  DEBUG ((DEBUG_INFO, " Format->Format.Format.wFormatTag      = 0x%x\n", Format->Format.Format.wFormatTag));
  DEBUG ((DEBUG_INFO, " Format->Format.Format.nChannels       = %d\n", Format->Format.Format.nChannels));
  DEBUG ((DEBUG_INFO, " Format->Format.Format.nSamplesPerSec  = %d\n", Format->Format.Format.nSamplesPerSec));
  DEBUG ((DEBUG_INFO, " Format->Format.Format.nAvgBytesPerSec = %d\n", Format->Format.Format.nAvgBytesPerSec));
  DEBUG ((DEBUG_INFO, " Format->Format.Format.nBlockAlign     = %d\n", Format->Format.Format.nBlockAlign));
  DEBUG ((DEBUG_INFO, " Format->Format.Format.wBitsPerSample  = %d\n", Format->Format.Format.wBitsPerSample));
  DEBUG ((DEBUG_INFO, " Format->Format.Format.cbSize          = %d\n", Format->Format.Format.cbSize));
  DEBUG ((DEBUG_INFO, " Format->Format.Samples                = %d\n", Format->Format.Samples));
  DEBUG ((DEBUG_INFO, " Format->Format.dwChannelMask          = 0x%x\n", Format->Format.dwChannelMask));
  DEBUG ((DEBUG_INFO, " Format->Format.SubFormat              = %g\n", Format->Format.SubFormat));


  DEBUG ((DEBUG_INFO, " Format->FormatConfiguration.CapabilitiesSize = %d B\n", Format->FormatConfiguration.CapabilitiesSize));
  DEBUG ((DEBUG_VERBOSE, " Format->FormatConfiguration.Capabilities:"));
  for (Index = 0; Index < (  Format->FormatConfiguration.CapabilitiesSize ) ; Index++) {
    if (Index % 16 == 0) {
      DEBUG ((DEBUG_VERBOSE, "\n"));
    }
    DEBUG ((DEBUG_VERBOSE, "0x%02x, ", Format->FormatConfiguration.Capabilities[Index]));
  }
  DEBUG ((DEBUG_VERBOSE, "\n"));
}

/**
  Prints Device Information.

  @param[in] *DeviceInfo       DeviceInfo to be printed

  @retval None
**/
VOID
NhltDeviceInfoDump (
  IN CONST DEVICE_INFO          *DeviceInfo
  )
{
  DEBUG ((DEBUG_INFO, "----------------------------- DEVICE INFO ----------------------------\n"));
  DEBUG ((DEBUG_INFO, " DeviceInfo->DeviceId         = %a\n",   DeviceInfo->DeviceId));
  DEBUG ((DEBUG_INFO, " DeviceInfo->DeviceInstanceId = 0x%x\n", DeviceInfo->DeviceInstanceId));
  DEBUG ((DEBUG_INFO, " DeviceInfo->DevicePortId     = 0x%x\n", DeviceInfo->DevicePortId));
}

/**
  Prints Endpoint configuration.

  @param[in] *Endpoint     Endpoint to be printed

  @retval None
**/
VOID
NhltEndpointDump (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  )
{
  UINT8 Index;
  FORMATS_CONFIG *FormatsConfigs;
  FORMAT_CONFIG  *Format;
  DEVICES_INFO   *DevicesInfo;
  DEVICE_INFO    *DeviceInfo;

  DEBUG ((DEBUG_INFO, "------------------------------ ENDPOINT ------------------------------\n"));
  DEBUG ((DEBUG_INFO, " Endpoint->DeviceDescriptorLength = %d B\n", Endpoint->EndpointDescriptorLength));
  DEBUG ((DEBUG_INFO, " Endpoint->LinkType               = 0x%x\n", Endpoint->LinkType));
  DEBUG ((DEBUG_INFO, " Endpoint->InstanceId             = 0x%x\n", Endpoint->InstanceId));
  DEBUG ((DEBUG_INFO, " Endpoint->HwVendorId             = 0x%x\n", Endpoint->HwVendorId));
  DEBUG ((DEBUG_INFO, " Endpoint->HwDeviceId             = 0x%x\n", Endpoint->HwDeviceId));
  DEBUG ((DEBUG_INFO, " Endpoint->HwRevisionId           = 0x%x\n", Endpoint->HwRevisionId));
  DEBUG ((DEBUG_INFO, " Endpoint->HwSubsystemId          = 0x%x\n", Endpoint->HwSubsystemId));
  DEBUG ((DEBUG_INFO, " Endpoint->DeviceType             = 0x%x\n", Endpoint->DeviceType));
  DEBUG ((DEBUG_INFO, " Endpoint->Direction              = 0x%x\n", Endpoint->Direction));
  DEBUG ((DEBUG_INFO, " Endpoint->VirtualBusId           = 0x%x\n", Endpoint->VirtualBusId));

  DEBUG ((DEBUG_INFO, " Endpoint->EndpointConfig.CapabilitiesSize = %d B\n", Endpoint->EndpointConfig.CapabilitiesSize));
  DEBUG ((DEBUG_VERBOSE, " Endpoint->EndpointConfig.Capabilities:"));
  for (Index = 0; Index < (Endpoint->EndpointConfig.CapabilitiesSize ) ; Index++) {
    if (Index % 16 == 0) DEBUG ((DEBUG_VERBOSE, "\n"));
    DEBUG ((DEBUG_VERBOSE, "0x%02x, ", Endpoint->EndpointConfig.Capabilities[Index]));
  }

  FormatsConfigs = GetNhltEndpointFormatsConfig (Endpoint);

  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, " Endpoint->FormatsConfig.FormatsCount = %d\n", FormatsConfigs->FormatsCount));
  for (Index = 0; Index < FormatsConfigs->FormatsCount; Index++) {
    Format = GetNhltEndpointFormat (Endpoint, Index);
    if (Format != NULL) {
      NhltFormatDump (Format);
    }
  }

  DevicesInfo = GetNhltEndpointDevicesInfo (Endpoint);
  if (DevicesInfo != NULL) {
    DEBUG ((DEBUG_INFO, "\n"));
    DEBUG ((DEBUG_INFO, " Endpoint->DevicesInfo.DeviceInfoCount = %d\n", DevicesInfo->DeviceInfoCount));
    for (Index = 0; Index < DevicesInfo->DeviceInfoCount; Index++) {
      DeviceInfo = GetNhltEndpointDeviceInfo (Endpoint, Index);
      if (DeviceInfo != NULL) {
        NhltDeviceInfoDump (DeviceInfo);
      }
    }
  }
  DEBUG ((DEBUG_VERBOSE, "\n"));
}

/**
  Prints OED (Offload Engine Driver) configuration.

  @param[in] *OedConfig   OED to be printed

  @retval None
**/
VOID
NhltOedConfigDump (
  IN CONST SPECIFIC_CONFIG      *OedConfig
  )
{
  UINT8 Index;

  DEBUG ((DEBUG_INFO, "-------------------------- OED CONFIGURATION -------------------------\n"));
  DEBUG ((DEBUG_INFO, " OedConfig->CapabilitiesSize = %d B\n", OedConfig->CapabilitiesSize));
  DEBUG ((DEBUG_VERBOSE, " OedConfig->Capabilities:"));
  for (Index = 0; Index < (OedConfig->CapabilitiesSize) ; Index++) {
    if (Index % 16 == 0) DEBUG ((DEBUG_VERBOSE, "\n"));
    DEBUG ((DEBUG_VERBOSE, "0x%02x, ", OedConfig->Capabilities[Index]));
  }

  DEBUG ((DEBUG_VERBOSE, "\n"));
}

/**
  Prints NHLT (Non HDA-Link Table) to be exposed via ACPI (aka. OED (Offload Engine Driver) Configuration Table).

  @param[in] *NhltTable    The NHLT table to print

  @retval None
**/
VOID
NhltAcpiTableDump (
  IN NHLT_ACPI_TABLE            *NhltTable
  )
{
  DEBUG_CODE_BEGIN ();
  UINT8 Index;

  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, "--- NHLT ACPI Table Dump [OED (Offload Engine Driver) Configuration] ---\n"));

  DEBUG ((DEBUG_INFO, "sizeof NHLT_ACPI_TABLE = %d B\n", sizeof (NHLT_ACPI_TABLE)));
  DEBUG ((DEBUG_INFO, "sizeof EFI_ACPI_DESCRIPTION_HEADER = %d B\n", sizeof (EFI_ACPI_DESCRIPTION_HEADER)));
  DEBUG ((DEBUG_INFO, "sizeof ENDPOINT_DESCRIPTOR = %d B\n", sizeof (ENDPOINT_DESCRIPTOR)));
  DEBUG ((DEBUG_INFO, "sizeof SPECIFIC_CONFIG = %d B\n", sizeof (SPECIFIC_CONFIG)));
  DEBUG ((DEBUG_INFO, "sizeof FORMATS_CONFIG = %d B\n", sizeof (FORMATS_CONFIG)));
  DEBUG ((DEBUG_INFO, "sizeof FORMAT_CONFIG = %d B\n", sizeof (FORMAT_CONFIG)));
  DEBUG ((DEBUG_INFO, "sizeof WAVEFORMATEXTENSIBLE = %d B\n", sizeof (WAVEFORMATEXTENSIBLE)));
  DEBUG ((DEBUG_INFO, "sizeof DEVICES_INFO = %d B\n", sizeof (DEVICES_INFO)));
  DEBUG ((DEBUG_INFO, "sizeof DEVICE_INFO = %d B\n", sizeof (DEVICE_INFO)));

  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE Header.Signature       = 0x%08x\n", NhltTable->Header.Signature));
  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE Header.Length          = 0x%08x\n", NhltTable->Header.Length));
  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE Header.Revision        = 0x%02x\n", NhltTable->Header.Revision));
  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE Header.Checksum        = 0x%02x\n", NhltTable->Header.Checksum));
  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE Header.OemId           = %a\n",     NhltTable->Header.OemId));
  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE Header.OemTableId      = 0x%lx\n",  NhltTable->Header.OemTableId));
  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE Header.OemRevision     = 0x%08x\n", NhltTable->Header.OemRevision));
  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE Header.CreatorId       = 0x%08x\n", NhltTable->Header.CreatorId));
  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE Header.CreatorRevision = 0x%08x\n", NhltTable->Header.CreatorRevision));
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG ((DEBUG_INFO, " NHLT_ACPI_TABLE EndpointCount = %d\n", NhltTable->EndpointCount));
  for (Index = 0; Index < NhltTable->EndpointCount; Index++) {
    NhltEndpointDump (GetNhltEndpoint (NhltTable, Index));
  }

  NhltOedConfigDump (GetNhltOedConfig (NhltTable));
  DEBUG ((DEBUG_INFO, "----------------------------------------------------------------------\n"));

  DEBUG_CODE_END ();
}

