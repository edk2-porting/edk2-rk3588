/** @file
  Header file for PchHdaLib Endpoint descriptors.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_HDA_ENDPOINTS_H_
#define _PCH_HDA_ENDPOINTS_H_

#include <Private/Library/DxePchHdaNhlt.h>

typedef enum {
  HdaDmicX1       = 0,
  HdaDmicX2,
  HdaDmicX4,
  HdaBtRender,
  HdaBtCapture,
  HdaI2sRender1,
  HdaI2sRender2,
  HdaI2sCapture,
  HdaEndpointMax
} NHLT_ENDPOINT;

typedef struct {
  NHLT_ENDPOINT EndpointType;
  UINT32        EndpointFormatsBitmask;
  UINT32        EndpointDevicesBitmask;
  BOOLEAN       Enable;
} PCH_HDA_NHLT_ENDPOINTS;

#define PCH_HDA_NHLT_TABLE_SIZE 0x2000

// Format bitmask
#define B_HDA_DMIC_2CH_48KHZ_16BIT_FORMAT                BIT0
#define B_HDA_DMIC_2CH_48KHZ_32BIT_FORMAT                BIT1
#define B_HDA_DMIC_4CH_48KHZ_16BIT_FORMAT                BIT2
#define B_HDA_DMIC_4CH_48KHZ_32BIT_FORMAT                BIT3
#define B_HDA_DMIC_1CH_48KHZ_16BIT_FORMAT                BIT4
#define B_HDA_BT_NARROWBAND_FORMAT                       BIT5
#define B_HDA_BT_WIDEBAND_FORMAT                         BIT6
#define B_HDA_BT_A2DP_FORMAT                             BIT7
#define B_HDA_I2S_RTK274_RENDER_4CH_48KHZ_24BIT_FORMAT   BIT8
#define B_HDA_I2S_RTK274_CAPTURE_4CH_48KHZ_24BIT_FORMAT  BIT9
#define V_HDA_FORMAT_MAX                                 10

// Formats
extern CONST WAVEFORMATEXTENSIBLE Ch1_48kHz16bitFormat;
extern CONST WAVEFORMATEXTENSIBLE Ch2_48kHz16bitFormat;
extern CONST WAVEFORMATEXTENSIBLE Ch2_48kHz24bitFormat;
extern CONST WAVEFORMATEXTENSIBLE Ch2_48kHz32bitFormat;
extern CONST WAVEFORMATEXTENSIBLE Ch4_48kHz16bitFormat;
extern CONST WAVEFORMATEXTENSIBLE Ch4_48kHz32bitFormat;
extern CONST WAVEFORMATEXTENSIBLE NarrowbandFormat;
extern CONST WAVEFORMATEXTENSIBLE WidebandFormat;
extern CONST WAVEFORMATEXTENSIBLE A2dpFormat;

// Format Config
extern CONST UINT32 DmicStereo16BitFormatConfig[];
extern CONST UINT32 DmicStereo16BitFormatConfigSize;
extern CONST UINT32 DmicStereo32BitFormatConfig[];
extern CONST UINT32 DmicStereo32BitFormatConfigSize;
extern CONST UINT32 DmicQuad16BitFormatConfig[];
extern CONST UINT32 DmicQuad16BitFormatConfigSize;
extern CONST UINT32 DmicQuad32BitFormatConfig[];
extern CONST UINT32 DmicQuad32BitFormatConfigSize;
extern CONST UINT32 DmicMono16BitFormatConfig[];
extern CONST UINT32 DmicMono16BitFormatConfigSize;

extern CONST UINT32 I2sRtk274Render4ch48kHz24bitFormatConfig[];
extern CONST UINT32 I2sRtk274Render4ch48kHz24bitFormatConfigSize;
extern CONST UINT32 I2sRtk274Capture4ch48kHz24bitFormatConfig[];
extern CONST UINT32 I2sRtk274Capture4ch48kHz24bitFormatConfigSize;
extern CONST UINT32 BtFormatConfig[];
extern CONST UINT32 BtFormatConfigSize;

// Endpoints
extern ENDPOINT_DESCRIPTOR  HdaEndpointDmicX1;
extern ENDPOINT_DESCRIPTOR  HdaEndpointDmicX2;
extern ENDPOINT_DESCRIPTOR  HdaEndpointDmicX4;
extern ENDPOINT_DESCRIPTOR  HdaEndpointBtRender;
extern ENDPOINT_DESCRIPTOR  HdaEndpointBtCapture;
extern ENDPOINT_DESCRIPTOR  HdaEndpointI2sRender;
extern ENDPOINT_DESCRIPTOR  HdaEndpointI2sCapture;

// Endpoint Config
extern CONST UINT8  DmicX1Config[];
extern CONST UINT32 DmicX1ConfigSize;
extern CONST UINT8  DmicX2Config[];
extern CONST UINT32 DmicX2ConfigSize;
extern CONST UINT8  DmicX4Config[];
extern CONST UINT32 DmicX4ConfigSize;
extern CONST UINT8  BtConfig[];
extern CONST UINT32 BtConfigSize;
extern CONST UINT8  I2sRender1Config[];
extern CONST UINT32 I2sRender1ConfigSize;
extern CONST UINT8  I2sRender2Config[];
extern CONST UINT32 I2sRender2ConfigSize;
extern CONST UINT8  I2sCaptureConfig[];
extern CONST UINT32 I2sCaptureConfigSize;

// Device Info bitmask
#define B_HDA_I2S_RENDER_DEVICE_INFO  BIT0
#define B_HDA_I2S_CAPTURE_DEVICE_INFO BIT1

// Device Info
extern CONST DEVICE_INFO I2sRenderDeviceInfo;
extern CONST DEVICE_INFO I2sCaptureDeviceInfo;

// Oed Configuration
extern CONST UINT32 NhltConfiguration[];
extern CONST UINT32 NhltConfigurationSize;

#endif // _PCH_HDA_ENDPOINTS_H_
