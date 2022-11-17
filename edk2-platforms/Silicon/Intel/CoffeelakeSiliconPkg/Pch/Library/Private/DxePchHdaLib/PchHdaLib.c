/** @file
  PCH HD Audio Library implementation.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Private/Library/PchHdaLib.h>
#include <Library/PchInfoLib.h>

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
  UINT8               i;
  ENDPOINT_DESCRIPTOR *Endpoint;
  Endpoint = (ENDPOINT_DESCRIPTOR*) (NhltTable->EndpointDescriptors);

  if (EndpointIndex > NhltTable->EndpointCount) {
    return NULL;
  }

  for (i = 0; i < EndpointIndex; i++) {
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
  UINT8          i;
  UINT32         Length;
  FORMATS_CONFIG *FormatsConfig;
  FORMAT_CONFIG  *Format;

  Length = 0;
  FormatsConfig = GetNhltEndpointFormatsConfig (Endpoint);
  Format = FormatsConfig->FormatsConfiguration;

  if (FormatIndex > FormatsConfig->FormatsCount) {
    return NULL;
  }

  for (i = 0; i < FormatIndex; i++) {
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

  if (DevicesInfo == NULL) {
    return NULL;
  }

  if (DeviceInfoIndex > DevicesInfo->DeviceInfoCount) {
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
  UINT32 i;

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
  for (i = 0; i < (  Format->FormatConfiguration.CapabilitiesSize ) ; i++) {
    if (i % 16 == 0) {
      DEBUG ((DEBUG_VERBOSE, "\n"));
    }
    DEBUG ((DEBUG_VERBOSE, "0x%02x, ", Format->FormatConfiguration.Capabilities[i]));
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
  UINT8 i;
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
  for (i = 0; i < (Endpoint->EndpointConfig.CapabilitiesSize ) ; i++) {
    if (i % 16 == 0) DEBUG ((DEBUG_VERBOSE, "\n"));
    DEBUG ((DEBUG_VERBOSE, "0x%02x, ", Endpoint->EndpointConfig.Capabilities[i]));
  }

  FormatsConfigs = GetNhltEndpointFormatsConfig (Endpoint);

  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, " Endpoint->FormatsConfig.FormatsCount = %d\n", FormatsConfigs->FormatsCount));
  for (i = 0; i < FormatsConfigs->FormatsCount; i++) {
    Format = GetNhltEndpointFormat (Endpoint, i);
    if (Format != NULL) {
      NhltFormatDump (Format);
    }
  }

  DevicesInfo = GetNhltEndpointDevicesInfo (Endpoint);
  if (DevicesInfo != NULL) {
    DEBUG ((DEBUG_INFO, "\n"));
    DEBUG ((DEBUG_INFO, " Endpoint->DevicesInfo.DeviceInfoCount = %d\n", DevicesInfo->DeviceInfoCount));
    for (i = 0; i < DevicesInfo->DeviceInfoCount; i++) {
      DeviceInfo = GetNhltEndpointDeviceInfo (Endpoint, i);
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
  UINT8 i;

  DEBUG ((DEBUG_INFO, "-------------------------- OED CONFIGURATION -------------------------\n"));
  DEBUG ((DEBUG_INFO, " OedConfig->CapabilitiesSize = %d B\n", OedConfig->CapabilitiesSize));
  DEBUG ((DEBUG_VERBOSE, " OedConfig->Capabilities:"));
  for (i = 0; i < (OedConfig->CapabilitiesSize) ; i++) {
    if (i % 16 == 0) DEBUG ((DEBUG_VERBOSE, "\n"));
    DEBUG ((DEBUG_VERBOSE, "0x%02x, ", OedConfig->Capabilities[i]));
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
  UINT8 i;

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
  for (i = 0; i < NhltTable->EndpointCount; i++) {
    NhltEndpointDump (GetNhltEndpoint (NhltTable, i));
  }

  NhltOedConfigDump (GetNhltOedConfig (NhltTable));
  DEBUG ((DEBUG_INFO, "----------------------------------------------------------------------\n"));

  DEBUG_CODE_END ();
}

/**
  Constructs FORMATS_CONFIGS structure based on given formats list.

  @param[in][out] *Endpoint     Endpoint for which format structures are created
  @param[in]      FormatBitmask Bitmask of formats supported for given endpoint

  @retval                       Size of created FORMATS_CONFIGS structure
**/
UINT32
NhltFormatsConstructor (
  IN OUT ENDPOINT_DESCRIPTOR    *Endpoint,
  IN CONST UINT32               FormatsBitmask
  )
{
  FORMATS_CONFIG *FormatsConfig;
  FORMAT_CONFIG  *Format;
  UINT8          FormatIndex;
  UINT32         FormatsConfigLength;

  DEBUG ((DEBUG_INFO, "NhltFormatsConstructor() Start, FormatsBitmask = 0x%08x\n", FormatsBitmask));

  FormatsConfig = NULL;
  FormatIndex = 0;
  FormatsConfigLength = 0;

  if (!FormatsBitmask) {
    DEBUG ((DEBUG_WARN, "No supported format found!\n"));
    return 0;
  }

  FormatsConfig = GetNhltEndpointFormatsConfig (Endpoint);
  FormatsConfig->FormatsCount = 0;

  if (FormatsBitmask & B_HDA_DMIC_2CH_48KHZ_16BIT_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_DMIC_2CH_48KHZ_16BIT_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &Ch2_48kHz16bitFormat, sizeof (WAVEFORMATEXTENSIBLE));
      Format->FormatConfiguration.CapabilitiesSize = DmicStereo16BitFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, DmicStereo16BitFormatConfig, DmicStereo16BitFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  if (FormatsBitmask & B_HDA_DMIC_2CH_48KHZ_32BIT_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_DMIC_2CH_48KHZ_32BIT_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &Ch2_48kHz32bitFormat, sizeof (WAVEFORMATEXTENSIBLE));

      Format->FormatConfiguration.CapabilitiesSize = DmicStereo32BitFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, DmicStereo32BitFormatConfig, DmicStereo32BitFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  if (FormatsBitmask & B_HDA_DMIC_4CH_48KHZ_16BIT_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_DMIC_4CH_48KHZ_16BIT_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &Ch4_48kHz16bitFormat, sizeof (WAVEFORMATEXTENSIBLE));
      Format->FormatConfiguration.CapabilitiesSize = DmicQuad16BitFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, DmicQuad16BitFormatConfig, DmicQuad16BitFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  if (FormatsBitmask & B_HDA_DMIC_4CH_48KHZ_32BIT_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_DMIC_4CH_48KHZ_32BIT_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &Ch4_48kHz32bitFormat, sizeof (WAVEFORMATEXTENSIBLE));

      Format->FormatConfiguration.CapabilitiesSize = DmicQuad32BitFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, DmicQuad32BitFormatConfig, DmicQuad32BitFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  if (FormatsBitmask & B_HDA_DMIC_1CH_48KHZ_16BIT_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_DMIC_1CH_48KHZ_16BIT_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &Ch1_48kHz16bitFormat, sizeof (WAVEFORMATEXTENSIBLE));

      Format->FormatConfiguration.CapabilitiesSize = DmicMono16BitFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, DmicMono16BitFormatConfig, DmicMono16BitFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  if (FormatsBitmask & B_HDA_BT_NARROWBAND_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_BT_NARROWBAND_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &NarrowbandFormat, sizeof (WAVEFORMATEXTENSIBLE));

      Format->FormatConfiguration.CapabilitiesSize = BtFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, BtFormatConfig, BtFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  if (FormatsBitmask & B_HDA_BT_WIDEBAND_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_BT_WIDEBAND_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &WidebandFormat, sizeof (WAVEFORMATEXTENSIBLE));

      Format->FormatConfiguration.CapabilitiesSize = BtFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, BtFormatConfig, BtFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  if (FormatsBitmask & B_HDA_BT_A2DP_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_BT_A2DP_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &A2dpFormat, sizeof (WAVEFORMATEXTENSIBLE));

      Format->FormatConfiguration.CapabilitiesSize = BtFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, BtFormatConfig, BtFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  if (FormatsBitmask & B_HDA_I2S_RTK274_RENDER_4CH_48KHZ_24BIT_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_I2S_RTK274_RENDER_4CH_48KHZ_24BIT_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &Ch2_48kHz24bitFormat, sizeof (WAVEFORMATEXTENSIBLE));

      Format->FormatConfiguration.CapabilitiesSize = I2sRtk274Render4ch48kHz24bitFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, I2sRtk274Render4ch48kHz24bitFormatConfig, I2sRtk274Render4ch48kHz24bitFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  if (FormatsBitmask & B_HDA_I2S_RTK274_CAPTURE_4CH_48KHZ_24BIT_FORMAT) {
    DEBUG ((DEBUG_INFO, "Format: B_HDA_I2S_RTK274_CAPTURE_4CH_48KHZ_24BIT_FORMAT\n"));

    Format = GetNhltEndpointFormat (Endpoint, FormatIndex++);
    if (Format != NULL) {
      CopyMem (&(Format->Format), &Ch2_48kHz24bitFormat, sizeof (WAVEFORMATEXTENSIBLE));

      Format->FormatConfiguration.CapabilitiesSize = I2sRtk274Capture4ch48kHz24bitFormatConfigSize;
      CopyMem (Format->FormatConfiguration.Capabilities, I2sRtk274Capture4ch48kHz24bitFormatConfig, I2sRtk274Capture4ch48kHz24bitFormatConfigSize);

      FormatsConfigLength += sizeof (*Format)
        - sizeof (Format->FormatConfiguration.Capabilities)
        + Format->FormatConfiguration.CapabilitiesSize;
      FormatsConfig->FormatsCount++;
    }
  }

  DEBUG ((DEBUG_INFO, "NhltFormatsConstructor() End, FormatsCount = %d, FormatsConfigLength = %d B\n", FormatsConfig->FormatsCount, FormatsConfigLength));
  return FormatsConfigLength;
}

/**
  Constructs DEVICES_INFO structure based on given device info list.

  @param[in][out] *Endpoint      Endpoint for which device info structures are created
  @param[in]      DevicesBitmask Bitmask of devices supported for given endpoint

  @retval                        Size of created DEVICES_INFO structure
**/
UINT32
NhltDevicesInfoConstructor (
  IN OUT   ENDPOINT_DESCRIPTOR  *Endpoint,
  IN CONST UINT32               DevicesBitmask
  )
{
  DEVICES_INFO  *DevicesInfo;
  DEVICE_INFO   *DeviceInfo;
  UINT8         DeviceIndex;
  UINT32        DevicesInfoLength;

  DEBUG ((DEBUG_INFO, "NhltDevicesInfoConstructor() Start, DevicesBitmask = 0x%08x\n", DevicesBitmask));

  DevicesInfo = NULL;
  DeviceIndex = 0;
  DevicesInfoLength = 0;

  if (!DevicesBitmask) {
    return 0;
  }

  DevicesInfo = GetNhltEndpointDevicesInfo (Endpoint);
  if (DevicesInfo == NULL) {
    return 0;
  }
  DevicesInfo->DeviceInfoCount = 0;

  if (DevicesBitmask & B_HDA_I2S_RENDER_DEVICE_INFO) {
    DEBUG ((DEBUG_INFO, "DeviceInfo: B_HDA_I2S_RENDER_DEVICE_INFO\n"));

    DeviceInfo = GetNhltEndpointDeviceInfo (Endpoint, DeviceIndex++);
    if (DeviceInfo != NULL) {
      CopyMem (DeviceInfo, &I2sRenderDeviceInfo, sizeof (DEVICE_INFO));
      DevicesInfo->DeviceInfoCount++;
    }
  } else if (DevicesBitmask & B_HDA_I2S_CAPTURE_DEVICE_INFO) {
    DEBUG ((DEBUG_INFO, "DeviceInfo: B_HDA_I2S_CAPTURE_DEVICE_INFO\n"));

    DeviceInfo = GetNhltEndpointDeviceInfo (Endpoint, DeviceIndex++);
    if (DeviceInfo != NULL) {
      CopyMem (DeviceInfo, &I2sCaptureDeviceInfo, sizeof (DEVICE_INFO));
      DevicesInfo->DeviceInfoCount++;
    }
  }

  DevicesInfoLength = DevicesInfo->DeviceInfoCount * sizeof (DEVICE_INFO);

  DEBUG ((DEBUG_INFO, "NhltDevicesInfoConstructor() End, DevicesCount = %d, DevicesInfoLength = %d B\n", DevicesInfo->DeviceInfoCount, DevicesInfoLength));
  return DevicesInfoLength;
}

/**
  Constructs NHLT_ENDPOINT structure based on given endpoint type.

  @param[in][out] *NhltTable              NHLT table for which endpoint is created
  @param[in]      EndpointType            Type of endpoint to be created
  @param[in]      EndpointFormatsBitmask  Bitmask of formats supported by endpoint
  @param[in]      EndpointDevicesBitmask  Bitmask of device info for endpoint
  @param[in]      EndpointIndex           Endpoint index in NHLT table

  @retval                       Size of created NHLT_ENDPOINT structure
**/
UINT32
NhltEndpointConstructor (
  IN OUT NHLT_ACPI_TABLE        *NhltTable,
  IN NHLT_ENDPOINT              EndpointType,
  IN UINT32                     EndpointFormatsBitmask,
  IN UINT32                     EndpointDevicesBitmask,
  IN UINT8                      EndpointIndex
  )
{

  ENDPOINT_DESCRIPTOR *Endpoint;
  SPECIFIC_CONFIG     *EndpointConfig;
  CONST UINT8         *EndpointConfigBuffer;
  UINT32              EndpointConfigBufferSize;
  UINT32              EndpointDescriptorLength;

  DEBUG ((DEBUG_INFO, "NhltEndpointConstructor() Start, EndpointIndex = %d\n", EndpointIndex));

  EndpointDescriptorLength = 0;
  Endpoint = GetNhltEndpoint (NhltTable, EndpointIndex);
  if (Endpoint == NULL) {
    return 0;
  }
  EndpointDescriptorLength = sizeof (ENDPOINT_DESCRIPTOR)
    - sizeof (SPECIFIC_CONFIG)
    - sizeof (FORMAT_CONFIG)
    - sizeof (DEVICE_INFO);

  switch (EndpointType) {
    case HdaDmicX1:
      DEBUG ((DEBUG_INFO, "Endpoint: HdaDmicX1\n"));
      CopyMem (Endpoint, &HdaEndpointDmicX1, sizeof (ENDPOINT_DESCRIPTOR));
      EndpointConfigBuffer = DmicX1Config;
      EndpointConfigBufferSize = DmicX1ConfigSize;
      break;
    case HdaDmicX2:
      DEBUG ((DEBUG_INFO, "Endpoint: HdaDmicX2\n"));
      CopyMem (Endpoint, &HdaEndpointDmicX2, sizeof (ENDPOINT_DESCRIPTOR));
      EndpointConfigBuffer = DmicX2Config;
      EndpointConfigBufferSize = DmicX2ConfigSize;
      break;
    case HdaDmicX4:
      DEBUG ((DEBUG_INFO, "Endpoint: HdaDmicX4\n"));
      CopyMem (Endpoint, &HdaEndpointDmicX4, sizeof (ENDPOINT_DESCRIPTOR));
      EndpointConfigBuffer = DmicX4Config;
      EndpointConfigBufferSize = DmicX4ConfigSize;
      break;
    case HdaBtRender:
      DEBUG ((DEBUG_INFO, "Endpoint: HdaBtRender\n"));
      CopyMem (Endpoint, &HdaEndpointBtRender, sizeof (ENDPOINT_DESCRIPTOR));
      if (IsPchH ()) {
        Endpoint->VirtualBusId = 0;
      }

      EndpointConfigBuffer = BtConfig;
      EndpointConfigBufferSize = BtConfigSize;
      break;
    case HdaBtCapture:
      DEBUG ((DEBUG_INFO, "Endpoint: HdaBtCapture\n"));
      CopyMem (Endpoint, &HdaEndpointBtCapture, sizeof (ENDPOINT_DESCRIPTOR));
      if (IsPchH ()) {
        Endpoint->VirtualBusId = 0;
      }

      EndpointConfigBuffer = BtConfig;
      EndpointConfigBufferSize = BtConfigSize;
      break;
    case HdaI2sRender1:
      DEBUG ((DEBUG_INFO, "Endpoint: HdaI2sRender1\n"));
      CopyMem (Endpoint, &HdaEndpointI2sRender, sizeof (ENDPOINT_DESCRIPTOR));
      EndpointConfigBuffer = I2sRender1Config;
      EndpointConfigBufferSize = I2sRender1ConfigSize;
      break;
    case HdaI2sRender2:
      DEBUG ((DEBUG_INFO, "Endpoint: HdaI2sRender2\n"));
      CopyMem (Endpoint, &HdaEndpointI2sRender, sizeof (ENDPOINT_DESCRIPTOR));
      EndpointConfigBuffer = I2sRender2Config;
      EndpointConfigBufferSize = I2sRender2ConfigSize;
      break;
    case HdaI2sCapture:
      DEBUG ((DEBUG_INFO, "Endpoint: HdaI2sCapture\n"));
      CopyMem (Endpoint, &HdaEndpointI2sCapture, sizeof (ENDPOINT_DESCRIPTOR));
      EndpointConfigBuffer = I2sCaptureConfig;
      EndpointConfigBufferSize = I2sCaptureConfigSize;
      break;
    default:
      DEBUG ((DEBUG_WARN, "Unknown endpoint!\n"));
      return 0;
  }

  EndpointConfig = GetNhltEndpointDeviceCapabilities (Endpoint);
  EndpointConfig->CapabilitiesSize = EndpointConfigBufferSize;
  CopyMem (EndpointConfig->Capabilities, EndpointConfigBuffer, EndpointConfig->CapabilitiesSize);
  EndpointDescriptorLength += sizeof (*EndpointConfig)
    - sizeof (EndpointConfig->Capabilities)
    + EndpointConfig->CapabilitiesSize;

  EndpointDescriptorLength += NhltFormatsConstructor (Endpoint, EndpointFormatsBitmask);
  EndpointDescriptorLength += NhltDevicesInfoConstructor (Endpoint, EndpointDevicesBitmask);

  Endpoint->EndpointDescriptorLength = EndpointDescriptorLength;

  DEBUG ((DEBUG_INFO, "NhltEndpointConstructor() End, EndpointDescriptorLength = %d B\n", Endpoint->EndpointDescriptorLength));
  return Endpoint->EndpointDescriptorLength;
}

/**
  Constructs SPECIFIC_CONFIG structure for OED configuration.

  @param[in][out] *NhltTable    NHLT table for which OED config is created

  @retval                       Size of created SPECIFIC_CONFIG structure
**/
UINT32
NhltOedConfigConstructor (
  IN OUT NHLT_ACPI_TABLE        *NhltTable
  )
{
  SPECIFIC_CONFIG *OedConfig;
  UINT32          OedConfigLength;

  OedConfigLength = 0;
  OedConfig = GetNhltOedConfig (NhltTable);

  OedConfig->CapabilitiesSize = NhltConfigurationSize;
  CopyMem (OedConfig->Capabilities, (UINT8*) NhltConfiguration, NhltConfigurationSize);

  OedConfigLength = sizeof (*OedConfig)
    - sizeof (OedConfig->Capabilities)
    + OedConfig->CapabilitiesSize;

  return OedConfigLength;
}

/**
  Constructs NHLT_ACPI_TABLE structure based on given Endpoints list.

  @param[in]      *EndpointTable List of endpoints for NHLT
  @param[in][out] **NhltTable    NHLT table to be created
  @param[in][out] *NhltTableSize Size of created NHLT table

  @retval EFI_SUCCESS            NHLT created successfully
  @retval EFI_BAD_BUFFER_SIZE    Not enough resources to allocate NHLT
**/
EFI_STATUS
NhltConstructor (
  IN PCH_HDA_NHLT_ENDPOINTS     *EndpointTable,
  IN OUT NHLT_ACPI_TABLE        **NhltTable,
  IN OUT UINT32                 *NhltTableSize
  )
{
  EFI_STATUS Status;
  UINT8  Index;
  UINT32 TableSize;
  UINT32 EndpointDescriptorsLength;
  UINT32 OedConfigLength;
  NHLT_ACPI_TABLE *Table;


  Status = EFI_SUCCESS;
  TableSize = PCH_HDA_NHLT_TABLE_SIZE;
  EndpointDescriptorsLength = 0;
  OedConfigLength = 0;

  Table = AllocateZeroPool (TableSize);

  if (Table == NULL) {
    return EFI_BAD_BUFFER_SIZE;
  }

  Table->EndpointCount = 0;

  for (Index = 0; Index < HdaEndpointMax; Index++) {
    if (EndpointTable[Index].Enable == TRUE) {
      EndpointDescriptorsLength += NhltEndpointConstructor (Table,
                                     EndpointTable[Index].EndpointType,
                                     EndpointTable[Index].EndpointFormatsBitmask,
                                     EndpointTable[Index].EndpointDevicesBitmask,
                                     Table->EndpointCount++);
    }
  }
  DEBUG ((DEBUG_INFO, "NhltConstructor: EndpointCount = %d, All EndpointDescriptorsLength = %d B\n", Table->EndpointCount, EndpointDescriptorsLength));

  OedConfigLength = NhltOedConfigConstructor (Table);
  DEBUG ((DEBUG_INFO, "NhltConstructor: OedConfigLength = %d B\n", OedConfigLength));

  TableSize = EndpointDescriptorsLength + OedConfigLength;

  *NhltTableSize = TableSize;
  *NhltTable = Table;

  return Status;
}

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
  )
{
  DEBUG ((DEBUG_INFO, "NhltAcpiHeaderConstructor() Start\n"));

  // Header
  NhltTable->Header.Signature = NHLT_ACPI_TABLE_SIGNATURE;
  NhltTable->Header.Length = (UINT32) (NhltTableSize + sizeof (NHLT_ACPI_TABLE) - sizeof (ENDPOINT_DESCRIPTOR) - sizeof (SPECIFIC_CONFIG));
  NhltTable->Header.Revision = 0x0;
  NhltTable->Header.Checksum = 0x0;

  CopyMem (NhltTable->Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (NhltTable->Header.OemId));
  NhltTable->Header.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  NhltTable->Header.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  NhltTable->Header.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  NhltTable->Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  DEBUG ((DEBUG_INFO, "NhltAcpiHeaderConstructor(), NhltAcpiTable->Header.Length = %d B\n", NhltTable->Header.Length));
}

