/** @file
  This file contains HD Audio NHLT Endpoints definitions

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Private/PchHdaEndpoints.h>

GLOBAL_REMOVE_IF_UNREFERENCED
CONST WAVEFORMATEXTENSIBLE Ch1_48kHz16bitFormat =
{
  {
    WAVE_FORMAT_EXTENSIBLE,
    1,
    48000,
    96000,
    2,
    16,
    sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX)
  },
  {16},
  KSAUDIO_SPEAKER_MONO,
  KSDATAFORMAT_SUBTYPE_PCM
};

GLOBAL_REMOVE_IF_UNREFERENCED
CONST WAVEFORMATEXTENSIBLE Ch2_48kHz16bitFormat =
{
  {
    WAVE_FORMAT_EXTENSIBLE,
    2,
    48000,
    192000,
    4,
    16,
    sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX)
  },
  {16},
  KSAUDIO_SPEAKER_STEREO,
  KSDATAFORMAT_SUBTYPE_PCM
};

GLOBAL_REMOVE_IF_UNREFERENCED
CONST WAVEFORMATEXTENSIBLE Ch2_48kHz24bitFormat =
{
  {
    WAVE_FORMAT_EXTENSIBLE,
    2,
    48000,
    384000,
    8,
    32,
    sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX)
  },
  {24},
  KSAUDIO_SPEAKER_STEREO,
  KSDATAFORMAT_SUBTYPE_PCM
};

GLOBAL_REMOVE_IF_UNREFERENCED
CONST WAVEFORMATEXTENSIBLE Ch2_48kHz32bitFormat =
{
  {
    WAVE_FORMAT_EXTENSIBLE,
    2,
    48000,
    384000,
    8,
    32,
    sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX)
  },
  {32},
  KSAUDIO_SPEAKER_STEREO,
  KSDATAFORMAT_SUBTYPE_PCM
};

GLOBAL_REMOVE_IF_UNREFERENCED
CONST WAVEFORMATEXTENSIBLE Ch4_48kHz16bitFormat =
{
  {
    WAVE_FORMAT_EXTENSIBLE,
    4,
    48000,
    384000,
    8,
    16,
    sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX)
  },
  {16},
  KSAUDIO_SPEAKER_QUAD,
  KSDATAFORMAT_SUBTYPE_PCM
};

GLOBAL_REMOVE_IF_UNREFERENCED
CONST WAVEFORMATEXTENSIBLE Ch4_48kHz32bitFormat =
{
  {
    WAVE_FORMAT_EXTENSIBLE,
    4,
    48000,
    384000,
    8,
    32,
    sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX)
  },
  {32},
  KSAUDIO_SPEAKER_QUAD,
  KSDATAFORMAT_SUBTYPE_PCM
};

GLOBAL_REMOVE_IF_UNREFERENCED
CONST WAVEFORMATEXTENSIBLE NarrowbandFormat =
{
  {
    WAVE_FORMAT_EXTENSIBLE,
    1,
    8000,
    16000,
    2,
    16,
    sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX)
  },
  {16},
  KSAUDIO_SPEAKER_MONO,
  KSDATAFORMAT_SUBTYPE_PCM
};

GLOBAL_REMOVE_IF_UNREFERENCED
CONST WAVEFORMATEXTENSIBLE WidebandFormat =
{
  {
    WAVE_FORMAT_EXTENSIBLE,
    1,
    16000,
    32000,
    2,
    16,
    sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX)
  },
  {16},
  KSAUDIO_SPEAKER_MONO,
  KSDATAFORMAT_SUBTYPE_PCM
};

GLOBAL_REMOVE_IF_UNREFERENCED
CONST WAVEFORMATEXTENSIBLE A2dpFormat =
{
  {
    WAVE_FORMAT_EXTENSIBLE,
    2,
    48000,
    384000,
    8,
    32,
    sizeof (WAVEFORMATEXTENSIBLE) - sizeof (WAVEFORMATEX)
  },
  {24},
  KSAUDIO_SPEAKER_STEREO,
  KSDATAFORMAT_SUBTYPE_PCM
};
GLOBAL_REMOVE_IF_UNREFERENCED
ENDPOINT_DESCRIPTOR  HdaEndpointDmicX1 = {
  0,                 // EndpointDescriptorLength
  HdaNhltLinkDmic,   // LinkType
  0,                 // InstanceId
  0x8086,            // HwVendorId
  0xae20,            // HwDeviceId
  1,                 // HwRevisionId
  1,                 // HwSubsystemId
  HdaNhltDeviceDmic, // DeviceType
  1,                 // Direction
  0,                 // VirtualBusId
  { 0 },             // EndpointConfig
  { 0 },             // FormatsConfig
  { 0 }              // DevicesInformation
};

GLOBAL_REMOVE_IF_UNREFERENCED
ENDPOINT_DESCRIPTOR  HdaEndpointDmicX2 = {
  0,                 // EndpointDescriptorLength
  HdaNhltLinkDmic,   // LinkType
  0,                 // InstanceId
  0x8086,           // HwVendorId
  0xae20,           // HwDeviceId
  1,                 // HwRevisionId
  1,                 // HwSubsystemId
  HdaNhltDeviceDmic, // DeviceType
  1,                 // Direction
  0,                 // VirtualBusId
  { 0 },             // EndpointConfig
  { 0 },             // FormatsConfig
  { 0 }              // DevicesInformation
};

GLOBAL_REMOVE_IF_UNREFERENCED
ENDPOINT_DESCRIPTOR  HdaEndpointDmicX4 = {
  0,                 // EndpointDescriptorLength
  HdaNhltLinkDmic,   // LinkType
  0,                 // InstanceId
  0x8086,           // HwVendorId
  0xae20,           // HwDeviceId
  1,                 // HwRevisionId
  1,                 // HwSubsystemId
  HdaNhltDeviceDmic, // DeviceType
  1,                 // Direction
  0,                 // VirtualBusId
  { 0 },             // EndpointConfig
  { 0 },             // FormatsConfig
  { 0 }              // DevicesInformation
};

GLOBAL_REMOVE_IF_UNREFERENCED
ENDPOINT_DESCRIPTOR  HdaEndpointBtRender = {
  0,                 // EndpointDescriptorLength
  HdaNhltLinkSsp,    // LinkType
  0,                 // InstanceId
  0x8086,           // HwVendorId
  0xae30,           // HwDeviceId
  1,                 // HwRevisionId
  1,                 // HwSubsystemId
  HdaNhltDeviceBt,   // DeviceType
  0,                 // Direction
  2,                 // VirtualBusId
  { 0 },             // EndpointConfig
  { 0 },             // FormatsConfig
  { 0 }              // DevicesInformation
};

GLOBAL_REMOVE_IF_UNREFERENCED
ENDPOINT_DESCRIPTOR  HdaEndpointBtCapture = {
  0,                 // EndpointDescriptorLength
  HdaNhltLinkSsp,    // LinkType
  0,                 // InstanceId
  0x8086,           // HwVendorId
  0xae30,           // HwDeviceId
  1,                 // HwRevisionId
  1,                 // HwSubsystemId
  HdaNhltDeviceBt,   // DeviceType
  1,                 // Direction
  2,                 // VirtualBusId
  { 0 },             // EndpointConfig
  { 0 },             // FormatsConfig
  { 0 }              // DevicesInformation
};

GLOBAL_REMOVE_IF_UNREFERENCED
ENDPOINT_DESCRIPTOR  HdaEndpointI2sRender = {
  0,                 // EndpointDescriptorLength
  HdaNhltLinkSsp,    // LinkType
  1,                 // InstanceId
  0x8086,           // HwVendorId
  0xae34,           // HwDeviceId
  1,                 // HwRevisionId
  1,                 // HwSubsystemId
  HdaNhltDeviceI2s,  // DeviceType
  0,                 // Direction
  0,                 // VirtualBusId
  { 0 },             // EndpointConfig
  { 0 },             // FormatsConfig
  { 0 }              // DevicesInformation
};

GLOBAL_REMOVE_IF_UNREFERENCED
ENDPOINT_DESCRIPTOR  HdaEndpointI2sCapture = {
  0,                 // EndpointDescriptorLength
  HdaNhltLinkSsp,    // LinkType
  1,                 // InstanceId
  0x8086,           // HwVendorId
  0xae34,           // HwDeviceId
  1,                 // HwRevisionId
  1,                 // HwSubsystemId
  HdaNhltDeviceI2s,  // DeviceType
  1,                 // Direction
  0,                 // VirtualBusId
  { 0 },             // EndpointConfig
  { 0 },             // FormatsConfig
  { 0 }              // DevicesInformation
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  DmicX1Config[] =
{
  0x00, // VirtualSlot
  0x00, // eIntcConfigTypeMicArray = 1 , eIntcConfigTypeGeneric = 0
};
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 DmicX1ConfigSize = sizeof (DmicX1Config);

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  DmicX2Config[] =
{
  0x00, // VirtualSlot
  0x01, // eIntcConfigTypeMicArray = 1 , eIntcConfigTypeGeneric = 0
  0x0A  // ArrayType
};
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 DmicX2ConfigSize = sizeof (DmicX2Config);

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  DmicX4Config[] =
{
  0x00, // VirtualSlot
  0x01, // eIntcConfigTypeMicArray = 1 , eIntcConfigTypeGeneric = 0
  0x0D  // ArrayType
};
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 DmicX4ConfigSize = sizeof (DmicX4Config);

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  BtConfig[] = {0};
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 BtConfigSize = 0;

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  I2sRender1Config[] = {0};
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 I2sRender1ConfigSize = 0;

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  I2sRender2Config[] = {0x01};
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 I2sRender2ConfigSize = sizeof (I2sRender2Config);

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  I2sCaptureConfig[] = {0};
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 I2sCaptureConfigSize = 0;

GLOBAL_REMOVE_IF_UNREFERENCED CONST DEVICE_INFO I2sRenderDeviceInfo =
{
  "INT34C2", // DeviceId
  0x00,      // DeviceInstanceId
  0x01       // DevicePortId
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST DEVICE_INFO I2sCaptureDeviceInfo =
{
  "INT34C2", // DeviceId
  0x00,      // DeviceInstanceId
  0x01       // DevicePortId
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 NhltConfiguration[] = { 0xEFBEADDE };
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 NhltConfigurationSize = sizeof (NhltConfiguration);

